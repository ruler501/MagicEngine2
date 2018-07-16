#ifndef _CARD_H_
#define _CARD_H_

#include <functional>
#include <map>
#include <memory>
#include <set>
#include <variant>
#include <vector>

#include "guid.hpp"

#include "changeset.h"
#include "cost.h"
#include "enum.h"
#include "mana.h"

struct ActivatedAbility;
struct Token;
struct Emblem;
struct Player;
struct Card;
class TargetingRestriction;

struct HasEffect {
    virtual Changeset applyEffect(const Environment& env) = 0;
	std::shared_ptr<TargetingRestriction> targeting;

	HasEffect(std::shared_ptr<TargetingRestriction> targeting);
};

struct CostedEffect {
    std::vector<std::shared_ptr<Cost>> costs;
    std::vector<std::shared_ptr<Cost>> additionalCosts;
	std::variant<std::shared_ptr<Card>, std::shared_ptr<Token>> source;
    std::shared_ptr<Cost> canPlay(Player& player, Environment& env);

	CostedEffect();
    CostedEffect(std::vector<std::shared_ptr<Cost>> costs, std::vector<std::shared_ptr<Cost>> additionalCosts,
		         std::variant<std::shared_ptr<Card>, std::shared_ptr<Token>> source);
};

struct CardToken : public Targetable, public HasEffect {
    Changeset applyEffect(const Environment& env); 
    // CodeReview: Move to environment
    bool is_tapped;

    std::set<CardSuperType> baseSuperTypes;
    std::set<CardType> baseTypes;
    std::set<CardSubType> baseSubTypes;
    // CodeReview: Implement devotion
    unsigned int basePower;
    unsigned int baseToughness;
    unsigned int startingLoyalty;
    std::string name;
    unsigned int cmc;
    std::set<Color> baseColors;
    std::vector<std::shared_ptr<ActivatedAbility>> activatableAbilities;
    std::vector<std::function<Changeset&(Changeset&, const Environment&, xg::Guid)>> applyEffects;

	CardToken();
    CardToken(std::set<CardSuperType> superTypes, std::set<CardType> types, std::set<CardSubType> subTypes, int power,
              int toughness, int loyalty, std::string name, unsigned int cmc, std::set<Color> colors,
              std::vector<std::shared_ptr<ActivatedAbility>> activatedAbilities,
			  std::shared_ptr<TargetingRestriction> targeting,
              std::vector<std::function<Changeset&(Changeset&, const Environment&, xg::Guid)>> applyAbilities);
};

struct Card : public CardToken, public CostedEffect {
	Card();
	Card(std::set<CardSuperType> superTypes, std::set<CardType> types, std::set<CardSubType> subTypes, int power,
         int toughness, int loyalty, std::string name, unsigned int cmc, std::set<Color> colors,
         std::vector<std::shared_ptr<ActivatedAbility>> activatedAbilities,
		 std::shared_ptr<TargetingRestriction> targeting,
         std::vector<std::function<Changeset&(Changeset&, const Environment&, xg::Guid)>> applyAbilities,
         std::vector<std::shared_ptr<Cost>> costs, std::vector<std::shared_ptr<Cost>> additionalCosts);
};

struct Token : public CardToken {
    Token(std::set<CardSuperType> superTypes, std::set<CardType> types, std::set<CardSubType> subTypes, int power,
          int toughness, int loyalty, std::string name, unsigned int cmc, std::set<Color> colors,
          std::vector<std::shared_ptr<ActivatedAbility>> activatedAbilities,
		  std::shared_ptr<TargetingRestriction> targeting,
          std::vector<std::function<Changeset&(Changeset&, const Environment&, xg::Guid)>> applyAbilities);
};

struct Emblem : public Targetable {
};
#endif
