#include "../ability.h"
#include "../card.h"
#include "../environment.h"

#include "cardManager.h"

Card Plains = newCard("Plains", 0, std::set<CardSuperType>{BASIC}, std::set<CardType>{LAND}, std::set<CardSubType>{PLAINS}, 0, 0, 0,
					  std::set<Color>{}, std::shared_ptr<TargetingRestriction>(new NoTargets()),
					  std::vector<std::shared_ptr<Cost>>{std::shared_ptr<Cost>(new LandPlayCost())}, std::vector<std::shared_ptr<Cost>>{},
					  std::vector<std::function<Changeset&(Changeset&, const Environment&, xg::Guid)>>{},
					  std::vector<std::shared_ptr<ActivatedAbility>>{
						  std::shared_ptr<ActivatedAbility>(new ManaAbility(Mana(std::multiset<Color>{WHITE}),
							  std::vector<std::shared_ptr<Cost>>{std::shared_ptr<Cost>(new TapCost())}))});

class PManager : public LetterManager {
public:
	void getCards(std::map<std::string, Card>& cards, std::map<int, std::string>&)
	{
		cards[Plains.name] = Plains;
	}
};
