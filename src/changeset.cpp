#include "changeset.h"
#include "environment.h"

Targetable::Targetable()
    : id(xg::newGuid())
{}

Changeset Changeset::operator+(Changeset& other){
    std::vector<ObjectMovement> moves = this->moves;
    moves.insert(moves.end(), other.moves.begin(), other.moves.end());
    std::vector<AddPlayerCounter> playerCounters = this->playerCounters;
    playerCounters.insert(playerCounters.end(), other.playerCounters.begin(), other.playerCounters.end());
    std::vector<AddPermanentCounter> permanentCounters = this->permanentCounters;
    permanentCounters.insert(permanentCounters.end(), other.permanentCounters.begin(), other.permanentCounters.end());
    std::vector<ObjectCreation> create = this->create;
    create.insert(create.end(), other.create.begin(), other.create.end());
    std::vector<RemoveObject> remove = this->remove;
    remove.insert(remove.end(), other.remove.begin(), other.remove.end());
    std::vector<LifeTotalChange> lifeTotalChanges = this->lifeTotalChanges;
    lifeTotalChanges.insert(lifeTotalChanges.end(), other.lifeTotalChanges.begin(), other.lifeTotalChanges.end());
    std::vector<std::reference_wrapper<EventHandler>> eventsToAdd = this->eventsToAdd;
    eventsToAdd.insert(eventsToAdd.end(), other.eventsToAdd.begin(), other.eventsToAdd.end());
    std::vector<std::reference_wrapper<EventHandler>> eventsToRemove = this->eventsToRemove;
    eventsToRemove.insert(eventsToRemove.end(), other.eventsToRemove.begin(), other.eventsToRemove.end());
    std::vector<std::reference_wrapper<StateQueryHandler>> propertiesToAdd = this->propertiesToAdd;
    propertiesToAdd.insert(propertiesToAdd.end(), other.propertiesToAdd.begin(), other.propertiesToAdd.end());
    std::vector<std::reference_wrapper<StateQueryHandler>> propertiesToRemove = this->propertiesToRemove;
    propertiesToRemove.insert(propertiesToRemove.end(), other.propertiesToRemove.begin(), other.propertiesToRemove.end());
    std::vector<xg::Guid> loseTheGame = this->loseTheGame;
    loseTheGame.insert(loseTheGame.end(), other.loseTheGame.begin(), other.loseTheGame.end());
    std::vector<AddMana> addMana = this->addMana;
    addMana.insert(addMana.end(), other.addMana.begin(), other.addMana.end());
    std::vector<RemoveMana> removeMana = this->removeMana;
    removeMana.insert(removeMana.end(), other.removeMana.begin(), other.removeMana.end());
    StepOrPhaseChange phaseChange = this->phaseChange;
    if(!phaseChange.changed && other.phaseChange.changed){
        phaseChange = other.phaseChange;
    }

    return Changeset{moves, playerCounters, permanentCounters, create, remove, lifeTotalChanges, eventsToAdd,
                     eventsToRemove, propertiesToAdd, propertiesToRemove, loseTheGame, addMana, removeMana, phaseChange};
}

Changeset& Changeset::operator+=(Changeset other){
    moves.insert(moves.end(), other.moves.begin(), other.moves.end());
    playerCounters.insert(playerCounters.end(), other.playerCounters.begin(), other.playerCounters.end());
    permanentCounters.insert(permanentCounters.end(), other.permanentCounters.begin(), other.permanentCounters.end());
    create.insert(create.end(), other.create.begin(), other.create.end());
    remove.insert(remove.end(), other.remove.begin(), other.remove.end());
    lifeTotalChanges.insert(lifeTotalChanges.end(), other.lifeTotalChanges.begin(), other.lifeTotalChanges.end());
    eventsToAdd.insert(eventsToAdd.end(), other.eventsToAdd.begin(), other.eventsToAdd.end());
    eventsToRemove.insert(eventsToRemove.end(), other.eventsToRemove.begin(), other.eventsToRemove.end());
    propertiesToAdd.insert(propertiesToAdd.end(), other.propertiesToAdd.begin(), other.propertiesToAdd.end());
    propertiesToRemove.insert(propertiesToRemove.end(), other.propertiesToRemove.begin(), other.propertiesToRemove.end());
    loseTheGame.insert(loseTheGame.end(), other.loseTheGame.begin(), other.loseTheGame.end());
    addMana.insert(addMana.end(), other.addMana.begin(), other.addMana.end());
    removeMana.insert(removeMana.end(), other.removeMana.begin(), other.removeMana.end());
    if(!phaseChange.changed && other.phaseChange.changed){
        phaseChange = other.phaseChange;
    }

    return *this;
}

Changeset Changeset::drawCards(xg::Guid player, unsigned int amount, Environment& env){
    Changeset result = Changeset();
    Zone<Card, Token> libraryZone = env.libraries[player];
    auto library = libraryZone.objects;
    Zone<Card, Token> handZone = env.hands[player];
    
    if(amount > library.size()){
        // Cause lose game
        amount = library.size();
    }
    auto card = library.begin() + library.size() - amount;
    for(; card != library.end(); card++) {
        Targetable& c = getBaseClass<Targetable>(*card);
        result.moves.push_back(ObjectMovement{c.id, libraryZone.id, handZone.id});
    }
    return result;
}
