#ifndef _HANDLERS_H_
#define _HANDLERS_H_

#include <memory>
#include <set>
#include <variant>
#include <vector>

#include "guid.hpp"

#include "enum.h"
#include "stateQuery.h"
#include "linq/util.h"

struct Changeset;
struct Environment;

struct Targetable : public clone_inherit<abstract_method<Targetable>> {
	// This is mutable state
	xg::Guid id;
	// CodeReview: potentially make const
	xg::Guid owner;

	Targetable();
	virtual ~Targetable() {}
};

class Handler : public clone_inherit<abstract_method<Handler>, Targetable> {
public:
	const std::set<ZoneType> activeSourceZones;
	const std::set<ZoneType> activeDestinationZones;
	virtual bool operator==(const Handler& other) const {
		return this->id == other.id;
	}

	Handler(std::set<ZoneType> activeSourceZones, std::set<ZoneType> activeDestinationZones)
		: activeSourceZones(activeSourceZones), activeDestinationZones(activeDestinationZones)
	{}
};

class EventHandler : public clone_inherit<abstract_method<EventHandler>, Handler> {
public:
	virtual std::optional<std::vector<Changeset>> handleEvent(Changeset&, const Environment&) const = 0;
	using clone_inherit<abstract_method<EventHandler>, Handler>::clone_inherit;
};

class StaticEffectHandler : public clone_inherit<abstract_method<StaticEffectHandler>, Handler> {
public:
	virtual StaticEffectQuery& handleEvent(StaticEffectQuery&, const Environment&) const = 0;
	virtual bool appliesTo(StaticEffectQuery&, const Environment&) const = 0;
	virtual bool dependsOn(StaticEffectQuery& start, StaticEffectQuery& end, const Environment& env) const = 0;

	// CodeReview: Is CDA
	using clone_inherit<abstract_method<StaticEffectHandler>, Handler>::clone_inherit;
};

struct QueueTrigger;

class TriggerHandler : public clone_inherit<abstract_method<TriggerHandler>, EventHandler> {
public:
	std::optional<std::vector<Changeset>> handleEvent(Changeset&, const Environment&) const;
	using clone_inherit<abstract_method<TriggerHandler>, EventHandler>::clone_inherit;

protected:
	virtual std::vector<QueueTrigger> createTriggers(const Changeset&, const Environment&) const = 0;
};
#endif
