#include "cards/cardManager.h"
#include "runner.h"
#include "strategies/fliers.h"

int main(){
	std::vector<std::vector<Card>> libraries{ std::vector<Card>(), std::vector<Card>() };
    CardManager cardManager;
	libraries[0].reserve(60);
	libraries[1].reserve(60);
    for(int i=0; i < 4; i++) libraries[0].push_back(cardManager.getCard("Siren Stormtamer"));
	for (int i = 0; i < 4; i++) libraries[0].push_back(cardManager.getCard("Siren Reaver"));
	for (int i = 0; i < 4; i++) libraries[0].push_back(cardManager.getCard("Kitesail Corsair"));
	for (int i = 0; i < 4; i++) libraries[0].push_back(cardManager.getCard("Favorable Winds"));
	for (int i = 0; i < 4; i++) libraries[0].push_back(cardManager.getCard("Chart a Course"));
	for (int i = 0; i < 4; i++) libraries[0].push_back(cardManager.getCard("Opt"));
	for (int i = 0; i < 4; i++) libraries[0].push_back(cardManager.getCard("Storm Fleet Aerialist"));
	for (int i = 0; i < 4; i++) libraries[0].push_back(cardManager.getCard("Skyship Plunderer"));
	for (int i = 0; i < 4; i++) libraries[0].push_back(cardManager.getCard("Tempest Djinn"));
	for (int i = 0; i < 4; i++) libraries[0].push_back(cardManager.getCard("Curious Obsession"));
    for(int i=0; i < 20; i++) libraries[0].push_back(cardManager.getCard("Island"));
    for(int i=0; i < 25; i++) libraries[1].push_back(cardManager.getCard("Goblin Instigator"));
	for(int i=0; i < 5; i++) libraries[1].push_back(cardManager.getCard("Impact Tremors"));
	for(int i=0; i < 5; i++) libraries[1].push_back(cardManager.getCard("Panharmonicon"));
    for(int i=0; i < 25; i++) libraries[1].push_back(cardManager.getCard("Mountain"));
    std::vector<Player> players{Player(std::shared_ptr<Strategy>(new FliersStrategy())),
                                Player(std::shared_ptr<Strategy>(new RandomStrategy()))};
    Runner runner(libraries, players);
#if DEBUG
	const int count = 1;
#else
	const int count = 1'000;
#endif
	for (int i = 0; i < count; i++) {
		runner.runGame();
		std::cout << i << std::endl;
	}
}
