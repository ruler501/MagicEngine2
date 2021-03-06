#include <memory>

#include "g.h"
#include "s.h"


TokenManager::TokenManager(){
	if (!tokens.empty()) return;
	tokens = {};
	GTokenManager().getTokens(tokens, mvids);
	STokenManager().getTokens(tokens, mvids);
}

Token TokenManager::getToken(int mvid){
    std::string name = this->mvids.at(mvid);
    return getToken(name);
}
Token TokenManager::getToken(std::string name) {
    return this->tokens.at(name);
}

Token newToken(std::string name, unsigned int cmc, std::set<CardSuperType> superTypes, std::set<CardType> types,
			   std::set<CardSubType> subTypes, int power, int toughness, int loyalty, std::set<Color> colors,
			   std::shared_ptr<TargetingRestriction> targeting,
			   std::vector<std::function<std::optional<Changeset>(xg::Guid, const Environment& env)>> applyAbilities,
			   std::vector<std::shared_ptr<ActivatedAbility>> activatedAbilities,
			   std::vector<std::shared_ptr<EventHandler>> replacementEffects, std::vector<std::shared_ptr<TriggerHandler>> triggerEffects,
			   std::vector<std::shared_ptr<StaticEffectHandler>> staticEffects, std::vector<size_t> thisOnlyReplacementIndexes) {
	std::vector<std::shared_ptr<const ActivatedAbility>> activatedAbilities2(activatedAbilities.begin(), activatedAbilities.end());
	
	return Token(std::make_shared<std::set<CardSuperType>>(superTypes), std::make_shared<std::set<CardType>>(types), std::make_shared<std::set<CardSubType>>(subTypes),
				 power, toughness, loyalty, name, cmc, colors, std::make_shared<std::vector<std::shared_ptr<const ActivatedAbility>>>(activatedAbilities2), LambdaEffects(applyAbilities),
				 targeting, replacementEffects, triggerEffects, staticEffects, thisOnlyReplacementIndexes);
}

void insertToken(std::map<std::string, Token>& tokens, const Token& Token) {
	tokens.insert(std::make_pair(Token.name, Token));
}