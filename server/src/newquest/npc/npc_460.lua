local sq = the_script();
sq:add(ScriptQuest.NPC_VISIT, 460);

require "define823"

function visit_460(npc)
	return visit_npc(npc);
end

function state_460(npc)
	return state_npc(npc);
end