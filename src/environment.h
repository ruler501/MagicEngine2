#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#include <functional>
#include <map>
#include <memory>
#include <set>
#include <typeinfo>
#include <typeindex>
#include <variant>
#include <vector>

#include "guid.hpp"

#include "changeset.h"
#include "player.h"
#include "card.h"
#include "ability.h"

enum ZoneType {
    HAND,
    GRAVEYARD,
    LIBRARY,
    BATTLEFIELD,
    STACK,
    EXILE,
    COMMAND
};

struct ZoneInterface {
    xg::Guid addObject(Targetable& object) {
        return addObject(object, xg::newGuid());
    }
    virtual xg::Guid addObject(Targetable& object, xg::Guid newGuid) = 0;
    virtual std::shared_ptr<Targetable> removeObject(xg::Guid object) = 0;
};

template<typename... Args>
struct Zone : public Targetable, public ZoneInterface {
    ZoneType type;
    std::vector<std::variant<std::shared_ptr<Args>...>> objects;
    
    xg::Guid addObject(Targetable& object, xg::Guid newGuid) {
        object.id = newGuid;
        this->addObjectInternal<Args...>(object);
        return newGuid;
    }

    std::shared_ptr<Targetable> removeObject(xg::Guid object){
        for(auto iter=objects.rbegin(); iter != objects.rend(); iter++) {
            std::shared_ptr<Targetable> val = getBaseClassPtr<Targetable>(*iter);
            if(val->id == object){
                std::advance(iter, 1);
                this->objects.erase(iter.base());
                return val;
            }
        }
#ifdef DEBUG
        std::cerr << "Failed to find object for removal" << std::endl;
#endif
        throw "Failed to find object for removal";
    }

private:
    template<typename T, typename... Extra>
    void addObjectInternal(Targetable& object){
        if(std::type_index(typeid(T)) == std::type_index(typeid(object))){
            this->objects.push_back(std::shared_ptr<T>(&static_cast<T&>(object)));
        }
        else {
            if constexpr(sizeof...(Extra) == 0) {
#ifdef DEBUG
                std::cerr << "Could not convert to internal types" << std::endl;
#endif
                throw "Could not convert to internal types";
            }
            else {
                return addObjectInternal<Extra...>(object);
            }
        }
    }
};

template<typename... Args>
using PrivateZones = std::map<xg::Guid, Zone<Args...>>;

struct Environment {
    std::map<xg::Guid, std::shared_ptr<Targetable>> gameObjects;

    PrivateZones<Card, Token> hands;
    PrivateZones<Card, Token> libraries;
    Zone<Card, Token> graveyard;
    Zone<Card, Token> battlefield;
    Zone<Card, Token, Ability> stack;
    Zone<Card, Token> exile;
    Zone<Card, Emblem> command;
    
    std::map<xg::Guid, Mana> manaPools;
    // CodeReview: Does this maintain across undoing turn changes
    std::map<xg::Guid, unsigned int> landPlays;
    std::vector<Player> players;

    std::map<xg::Guid, std::map<PermanentCounterType, unsigned int>> permanentCounters;
    std::map<xg::Guid, std::map<PlayerCounterType, unsigned int>> playerCounters;
    std::map<xg::Guid, int> lifeTotals;

    std::vector<std::shared_ptr<EventHandler>> triggerHandlers;
    std::vector<std::shared_ptr<EventHandler>> replacementEffects;
    std::vector<std::shared_ptr<StateQueryHandler>> stateQueryHandlers;

    std::vector<Changeset> changes;

    StepOrPhase currentPhase;
    unsigned int currentPlayer;
    unsigned int turnPlayer;

    Changeset passPriority(xg::Guid player);

    Environment(std::vector<Player>& players, std::vector<std::vector<std::shared_ptr<Card>>>& libraries);
};

#endif
