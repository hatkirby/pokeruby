CaveOfOrigin_B1F_MapScripts:: @ 815DD7C
	map_script 3, CaveOfOrigin_B1F_MapScript1_15DD82
	.byte 0

CaveOfOrigin_B1F_MapScript1_15DD82:: @ 815DD82
	checkflag FLAG_LEGENDARY_BATTLE_COMPLETED
	call_if 1, CaveOfOrigin_B1F_EventScript_1A04E3
	end
