#ifndef _RULESEFFECTS_H_
#define _RULESEFFECTS_H_

#include "changeset.h"
#include "linq/util.h"

class TokenMovementEffect : public clone_inherit<TokenMovementEffect, EventHandler> {
public:
	std::optional<std::vector<Changeset>> handleEvent(Changeset& changeset, const Environment& env) const {
		// 110.5g. A token that has left the battlefield can't move to another zone or come back onto the battlefield. If such a token would change zones, it remains in its current zone instead. It ceases to exist the next time state-based actions are checked; see rule 704.
		std::vector<std::shared_ptr<ObjectMovement>> toRemove;
		for (const std::shared_ptr<ObjectMovement>& move : changeset.ofType<ObjectMovement>()) {
			if (move->sourceZone != env.battlefield->id && move->sourceZone != env.stack->id && std::dynamic_pointer_cast<Token>(env.gameObjects.at(move->object))) {
				toRemove.push_back(move);
			}
		}
		for (std::shared_ptr<ObjectMovement>& removed : reverse(toRemove)) {
			changeset.changes.erase(std::remove(changeset.changes.begin(), changeset.changes.end(), removed), changeset.changes.end());
		}
		if (toRemove.empty()) return std::nullopt;
		else return std::vector<Changeset>{ changeset };
	}

	TokenMovementEffect()
		: clone_inherit({}, {})
	{}
};

class ZeroDamageEffect : public clone_inherit<ZeroDamageEffect, EventHandler> {
public:
	std::optional<std::vector<Changeset>> handleEvent(Changeset& changeset, const Environment&) const {
		// 110.5g. A token that has left the battlefield can't move to another zone or come back onto the battlefield. If such a token would change zones, it remains in its current zone instead. It ceases to exist the next time state-based actions are checked; see rule 704.
		std::vector<std::shared_ptr<DamageToTarget>> toRemove;
		for (const std::shared_ptr<DamageToTarget>& damage : changeset.ofType<DamageToTarget>()) {
			if (damage->amount <= 0) {
				toRemove.push_back(damage);
			}
		}
		for (std::shared_ptr<DamageToTarget>& removed : reverse(toRemove)) {
			changeset.changes.erase(std::remove(changeset.changes.begin(), changeset.changes.end(), removed), changeset.changes.end());
		}
		if (toRemove.empty()) return std::nullopt;
		else return std::vector<Changeset>{ changeset };
	}

	ZeroDamageEffect()
		: clone_inherit({}, {})
	{}
};

// CodeReview: Create StateQueryHandler for +1/+1 and -1/-1 counters
class CounterPowerToughnessEffect : public clone_inherit<CounterPowerToughnessEffect, StaticEffectHandler> {
public:
	StaticEffectQuery& handleEvent(StaticEffectQuery& query, const Environment& env) const {
		if (PowerQuery* powerQuery = std::get_if<PowerQuery>(&query)) {
			if (env.permanentCounters.find(powerQuery->target.id) != env.permanentCounters.end()) {
				powerQuery->power += tryAtMap(env.permanentCounters.at(powerQuery->target.id), PLUSONEPLUSONECOUNTER, 0u);
				powerQuery->power -= tryAtMap(env.permanentCounters.at(powerQuery->target.id), MINUSONEMINUSONECOUNTER, 0u);
			}
		}
		else if (ToughnessQuery* toughnessQuery = std::get_if<ToughnessQuery>(&query)) {
			if (env.permanentCounters.find(toughnessQuery->target.id) != env.permanentCounters.end()) {
				toughnessQuery->toughness += tryAtMap(env.permanentCounters.at(toughnessQuery->target.id), PLUSONEPLUSONECOUNTER, 0u);
				toughnessQuery->toughness -= tryAtMap(env.permanentCounters.at(toughnessQuery->target.id), MINUSONEMINUSONECOUNTER, 0u);
			}
		}
		return query;
	}

	bool appliesTo(StaticEffectQuery& query, const Environment& env) const override {
		if (PowerQuery* powerQuery = std::get_if<PowerQuery>(&query)) {
			if (env.permanentCounters.find(powerQuery->target.id) != env.permanentCounters.end()) {
				return tryAtMap(env.permanentCounters.at(powerQuery->target.id), PLUSONEPLUSONECOUNTER, 0u) > 0
					|| tryAtMap(env.permanentCounters.at(powerQuery->target.id), MINUSONEMINUSONECOUNTER, 0u) > 0;
			}
		}
		else if (ToughnessQuery* toughnessQuery = std::get_if<ToughnessQuery>(&query)) {
			if (env.permanentCounters.find(toughnessQuery->target.id) != env.permanentCounters.end()) {
				return tryAtMap(env.permanentCounters.at(toughnessQuery->target.id), PLUSONEPLUSONECOUNTER, 0u) > 0
					|| tryAtMap(env.permanentCounters.at(toughnessQuery->target.id), MINUSONEMINUSONECOUNTER, 0u) > 0;
			}
		}
		return false;
	}

	bool dependsOn(StaticEffectQuery&, StaticEffectQuery&, const Environment&) const override { return false; }

	CounterPowerToughnessEffect()
		: clone_inherit({}, {})
	{}
};
#endif