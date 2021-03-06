#include <map>
#include "enum.h"

std::ostream& operator<<(std::ostream& os, const StepOrPhaseId& step) {
	const static std::map<StepOrPhaseId, std::string> StepOrPhaseMapping{
		{ UNTAP, "Untap" },{ UPKEEP, "Upkeep" },{ DRAW, "Draw" },{ PRECOMBATMAIN, "Precombat Main" },
		{ BEGINCOMBAT, "Begin Combat" },{ DECLAREATTACKERS, "Declare Attackers" },{ DECLAREBLOCKERS, "Declare Blockers" },
		{ FIRSTSTRIKEDAMAGE, "First Strike Damage" },{ COMBATDAMAGE, "Combat Damage" },{ ENDCOMBAT, "End Combat" },
		{ POSTCOMBATMAIN, "Postcombat Main" },{ END, "End" },{ CLEANUP, "Cleanup" } };
	return os << StepOrPhaseMapping.at(step);
}

std::ostream& operator<<(std::ostream& os, const ZoneType& step) {
	const static std::map<ZoneType, std::string> ZoneTypeMapping{
		{ HAND, "Hand" }, { GRAVEYARD, "Graveyard" }, { LIBRARY, "Library" }, { BATTLEFIELD, "Battlefield" },
		{ STACK, "Stack" }, { EXILE, "Exile" }, { COMMAND, "Command" } };
	return os << ZoneTypeMapping.at(step);
}