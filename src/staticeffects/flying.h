#ifndef _FLYING_H_
#define _FLYING_H_

#include "../changeset.h"
#include "../combat.h"
#include "../environment.h"

bool canBlockFlying(std::shared_ptr<CardToken> card, const Environment& env);

class FlyingRestriction : public BlockRestriction {
public:
	virtual std::set<xg::Guid> canBlock(const std::shared_ptr<CardToken>& card, const std::set<xg::Guid>& possibleBlocks,
		const std::multimap<std::shared_ptr<CardToken>, xg::Guid>&,
		const Environment& env) const override {
		std::set<xg::Guid> result;
		for (auto& possibility : possibleBlocks) {
			if (possibility == this->flyer && !canBlockFlying(card, env)) continue;
			result.insert(possibility);
		}
		return result;
	}

	FlyingRestriction(xg::Guid flyer)
		: flyer(flyer)
	{}

private:
	xg::Guid flyer;
};

class FlyingHandler : public clone_inherit<FlyingHandler, StaticEffectHandler> {
public:
	StaticEffectQuery& handleEvent(StaticEffectQuery& query, const Environment& env) const override {
		if (BlockRestrictionQuery* block = std::get_if<BlockRestrictionQuery>(&query)) {
			bool found = false;
			for (auto& attack : env.declaredAttacks) {
				if (attack.first->id == this->owner) {
					found = true;
					break;
				}
			}
			if (!found) return query;

			block->restrictions.push_back(FlyingRestriction(this->owner));
		}
		return query;
	}

	bool appliesTo(StaticEffectQuery& query, const Environment& env) const override {
		if (BlockRestrictionQuery* block = std::get_if<BlockRestrictionQuery>(&query)) {
			bool found = false;
			for (auto& attack : env.declaredAttacks) {
				if (attack.first->id == this->owner) {
					found = true;
					break;
				}
			}
			return found;
		}
		return false;
	}

	bool dependsOn(StaticEffectQuery&, StaticEffectQuery&, const Environment&) const override { return false; }

	FlyingHandler()
		: clone_inherit({}, { BATTLEFIELD })
	{}
};

bool canBlockFlying(std::shared_ptr<CardToken> card, const Environment& env) {
	return env.hasStaticEffect<FlyingHandler>(card, BATTLEFIELD);
}
#endif