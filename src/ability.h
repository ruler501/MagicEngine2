#ifndef _ABILITY_H_
#define _ABILITY_H_

#include <functional>
#include <variant>
#include <vector>

#include "card.h"
#include "changeset.h"

class TargetingRestriction;

struct Ability : public clone_inherit<Ability, HasEffect> {
    std::variant<std::shared_ptr<const Card>, std::shared_ptr<const Token>, std::shared_ptr<const Emblem>> source;
    const std::set<Color> colors;

	Ability(EffectValue effect);

	Ability(EffectValue effect, std::shared_ptr<const TargetingRestriction> targeting);
};

struct AbilityWithCost : public Ability, public HasCost {
	AbilityWithCost(EffectValue effect, std::shared_ptr<const TargetingRestriction> targeting, std::vector<CostValue> costs,
					std::vector<CostValue> additionalCosts = {}, std::shared_ptr<const std::set<ZoneType>> playableFrom = std::shared_ptr<const std::set<ZoneType>>(new std::set{ BATTLEFIELD }));
};

struct ActivatedAbility : public clone_inherit<ActivatedAbility, AbilityWithCost> {
	using clone_inherit<ActivatedAbility, AbilityWithCost>::clone_inherit;
};

struct ManaAbility : public clone_inherit<ManaAbility, ActivatedAbility> {
    Changeset applyEffect(const Environment& env) const;

    ManaAbility(Mana mana, std::vector<CostValue> costs, std::vector<CostValue> additionalCosts={},
				std::shared_ptr<const std::set<ZoneType>> playableFrom = std::shared_ptr<const std::set<ZoneType>>(new std::set{ BATTLEFIELD }));

private:
    const Mana mana;
};

#endif
