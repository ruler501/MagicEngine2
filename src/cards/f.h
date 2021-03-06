#include "../ability.h"
#include "../card.h"
#include "../environment.h"

#include "cardManager.h"

#include "../staticeffects/lordEffect.h"
#include "../staticeffects/flying.h"

Card FavorableWinds = newCard("Favorable Winds", 2, {}, { ENCHANTMENT }, {}, 0, 0, 0, { BLUE }, Mana(1, { BLUE }),
	{}, {}, {}, {}, { std::make_shared<LordEffect>(LambdaProposition([](const CardToken& c, const Environment& e) { return e.hasStaticEffect<FlyingHandler>(c.id, BATTLEFIELD); }), 1, 1) });
Card Forest = newCard("Forest", 0, std::set<CardSuperType>{BASIC}, std::set<CardType>{LAND}, std::set<CardSubType>{FOREST}, 0, 0, 0,
					  std::set<Color>{},
					  std::shared_ptr<TargetingRestriction>(new NoTargets()),
						{ LandPlayCost() }, {},
					  {},
					  std::vector<std::shared_ptr<ActivatedAbility>>{
					  std::shared_ptr<ActivatedAbility>(new ManaAbility(Mana(std::multiset<Color>{GREEN}),
						  { TapCost() }))});

class FManager : public LetterManager {
public:
	void getCards(std::map<std::string, Card>& cards, std::map<int, std::string>&)
	{
		insertCard(cards, FavorableWinds);
		cards.insert(std::make_pair(Forest.name, Forest));
	}
};
