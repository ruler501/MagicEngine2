#include <memory>

#include "../ability.h"

#include "../tokens/tokenManager.h"

#include "cardManager.h"

#include "a.h"
#include "b.h"
#include "c.h"
#include "d.h"
#include "f.h"
#include "g.h"
#include "i.h"
#include "k.h"
#include "l.h"
#include "m.h"
#include "o.h"
#include "p.h"
#include "s.h"
#include "t.h"
#include "w.h"

CardManager::CardManager(){
	AManager().getCards(this->cards, this->mvids);
	BManager().getCards(this->cards, this->mvids);
	CManager().getCards(this->cards, this->mvids);
    DManager().getCards(this->cards, this->mvids);
	FManager().getCards(this->cards, this->mvids);
	GManager().getCards(this->cards, this->mvids);
    IManager().getCards(this->cards, this->mvids);
	KManager().getCards(this->cards, this->mvids);
	LManager().getCards(this->cards, this->mvids);
	MManager().getCards(this->cards, this->mvids);
	OManager().getCards(this->cards, this->mvids);
	PManager().getCards(this->cards, this->mvids);
	SManager().getCards(this->cards, this->mvids);
	TManager().getCards(this->cards, this->mvids);
	WManager().getCards(this->cards, this->mvids);
}

Card CardManager::getCard(int mvid){
    std::string name = this->mvids.at(mvid);
    return getCard(name);
}
Card CardManager::getCard(std::string name) {
    return this->cards.at(name);
}

Card newCard(std::string name, unsigned int cmc, std::set<CardSuperType> superTypes, std::set<CardType> types,
	std::set<CardSubType> subTypes, int power, int toughness, int loyalty, std::set<Color> colors,
	Mana cost, std::vector<CostValue> additionalCosts,
	std::vector<std::shared_ptr<ActivatedAbility>> activatedAbilities,
	std::vector<std::shared_ptr<EventHandler>> replacementEffects, std::vector<std::shared_ptr<TriggerHandler>> triggerEffects,
	std::vector<std::shared_ptr<StaticEffectHandler>> staticEffects, std::vector<size_t> thisOnlyReplacementIndexes) {
	return newCard(name, cmc, superTypes, types, subTypes, power, toughness, loyalty, colors, std::shared_ptr<TargetingRestriction>(new NoTargets()), cost,
		additionalCosts, activatedAbilities, replacementEffects, triggerEffects, staticEffects, thisOnlyReplacementIndexes);
}

Card newCard(std::string name, unsigned int cmc, std::set<CardSuperType> superTypes, std::set<CardType> types,
			 std::set<CardSubType> subTypes, int power, int toughness, int loyalty, std::set<Color> colors,
			 std::shared_ptr<TargetingRestriction> targeting,
			 Mana cost, std::vector<CostValue> additionalCosts,
			 std::vector<std::shared_ptr<ActivatedAbility>> activatedAbilities,
			 std::vector<std::shared_ptr<EventHandler>> replacementEffects, std::vector<std::shared_ptr<TriggerHandler>> triggerEffects,
			 std::vector<std::shared_ptr<StaticEffectHandler>> staticEffects, std::vector<size_t> thisOnlyReplacementIndexes) {
	return newCard(name, cmc, superTypes, types, subTypes, power, toughness, loyalty, colors, targeting, {ManaCost(cost)},
				   additionalCosts, {}, activatedAbilities, replacementEffects, triggerEffects, staticEffects, thisOnlyReplacementIndexes);
}

Card newCard(std::string name, unsigned int cmc, std::set<CardSuperType> superTypes, std::set<CardType> types,
	std::set<CardSubType> subTypes, int power, int toughness, int loyalty, std::set<Color> colors,
	std::shared_ptr<TargetingRestriction> targeting,
	std::vector<CostValue> costs, std::vector<CostValue> additionalCosts,
	std::vector<std::function<std::optional<Changeset>(xg::Guid, const Environment& env)>> applyAbilities,
	std::vector<std::shared_ptr<ActivatedAbility>> activatedAbilities,
	std::vector<std::shared_ptr<EventHandler>> replacementEffects, std::vector<std::shared_ptr<TriggerHandler>> triggerEffects,
	std::vector<std::shared_ptr<StaticEffectHandler>> staticEffects, std::vector<size_t> thisOnlyReplacementIndexes) {
	std::vector<std::shared_ptr<const ActivatedAbility>> activatedAbilities2(activatedAbilities.begin(), activatedAbilities.end());

	return Card(std::make_shared<std::set<CardSuperType>>(superTypes), std::make_shared<std::set<CardType>>(types), std::make_shared<std::set<CardSubType>>(subTypes),
				power, toughness, loyalty, name, cmc, colors, std::make_shared<std::vector<std::shared_ptr<const ActivatedAbility>>>(activatedAbilities2), LambdaEffects(applyAbilities),
				targeting, std::make_shared<std::set<ZoneType>>(std::set<ZoneType>{ HAND }), replacementEffects, triggerEffects, staticEffects, thisOnlyReplacementIndexes, costs, additionalCosts);
}

void insertCard(std::map<std::string, Card>& cards, Card card) {
	cards.insert(std::make_pair(card.name, card));
}

TokenManager tokenManager = TokenManager();