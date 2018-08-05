#ifndef _COMBATDAMAGE_H_
#define _COMBATDAMAGE_H_

#include "../changeset.h"

class CombatDamageTrigger : public TriggerHandler {
public:
	CombatDamageTrigger(std::function<std::shared_ptr<Ability>(DamageToTarget)> createAbility, bool justOwner = true)
		: TriggerHandler({}, { BATTLEFIELD }), createAbility(createAbility), justOwner(justOwner)
	{}

protected:
	std::vector<QueueTrigger> createTriggers(const Changeset& changes, const Environment& env) const override {
		std::vector<QueueTrigger> results;
		for (auto& cdamage : changes.combatDamage) {
			if (cdamage.amount <= 0) continue;
			if (!this->justOwner || cdamage.source == this->owner) {
				Changeset triggered;
				triggered.combatDamage.push_back(cdamage);
				results.push_back(QueueTrigger{ env.getController(this->owner), triggered, this->createAbility(cdamage) });
			}
		}
		return results;
	}

private:
	const std::function<std::shared_ptr<Ability>(DamageToTarget)> createAbility;
	const bool justOwner;
};

#endif 