#include "global.h"

void sub_80BB4AC(struct SecretBaseRecord *record) {
	u16 i;
	u16 j;
	record->sbr_field_0 = 0;
	for (i=0; i<7; i++)
		record->sbr_field_2[i] = 0xff;
	for (i=0; i<4; i++)
		record->trainerId[i] = 0x00;
	record->sbr_field_e = 0;
	record->sbr_field_10 = 0;
	record->sbr_field_11 = 0;
	record->sbr_field_1_0 = 0;
	record->gender = 0;
	record->sbr_field_1_5 = 0;
	record->sbr_field_1_6 = 0;
	for (i=0; i<16; i++) {
		record->decorations[i] = 0;
		record->sbr_field_22[i] = 0;
	}
	for (i=0; i<6; i++) {
		for (j=0; j<4; j++) {
			record->partyMoves[i * 4 + j] = 0;
		}
		record->partyPersonality[i] = 0;
		record->partyEVs[i] = 0;
		record->partySpecies[i] = 0;
		record->partyHeldItems[i] = 0;
		record->partyLevels[i] = 0;
	}
}
