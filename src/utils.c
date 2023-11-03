#include <string.h>
#include <stdio.h>

#include "notation.h"

int macro_to_str(char *str, struct macro_t *macro)
{
	switch (macro->type) {
	case MACRO_F:
		strcpy(str, "f");
		break;
	case MACRO_MF:
		strcpy(str, "mf");
		break;
	case MACRO_FF:
		strcpy(str, "ff");
		break;
	case MACRO_P:
		strcpy(str, "p");
		break;
	case MACRO_MP:
		strcpy(str, "mp");
		break;
	case MACRO_PP:
		strcpy(str, "pp");
		break;
	case MACRO_SFZ:
		strcpy(str, "sfz");
		break;
	case MACRO_CRESC:
		strcpy(str, "cresc.");
		break;
	case MACRO_DIM:
		strcpy(str, "dim.");
		break;
	case MACRO_RIT:
		strcpy(str, "rit.");
		break;
	case MACRO_ACCEL:
		strcpy(str, "accel.");
		break;
	case MACRO_SFP:
		strcpy(str, "sfp");
		break;
	case MACRO_SF:
		strcpy(str, "sf");
		break;
	case MACRO_FP:
		strcpy(str, "fp");
		break;
	case MACRO_DIV:
		strcpy(str, "div.");
		break;
	case MACRO_UNIS:
		strcpy(str, "unis.");
		break;
	case MACRO_LARGO:
		strcpy(str, "Largo");
		break;
	case MACRO_LARGHETTO:
		strcpy(str, "Larghetto");
		break;
	case MACRO_ADAGIO:
		strcpy(str, "Adagio");
		break;
	case MACRO_ADAGIETTO:
		strcpy(str, "Adagietto");
		break;
	case MACRO_ANDANTE:
		strcpy(str, "Andante");
		break;
	case MACRO_ANDANTINO:
		strcpy(str, "Andantino");
		break;
	case MACRO_MODERATO:
		strcpy(str, "Moderato");
		break;
	case MACRO_ALLEGRO:
		strcpy(str, "Allegro");
		break;
	case MACRO_VIVACE:
		strcpy(str, "Vivace");
		break;
	case MACRO_PRESTO:
		strcpy(str, "Presto");
		break;
	case MACRO_PRESTISSIMO:
		strcpy(str, "Prestissimo");
		break;
	case MACRO_LARGAMENTE:
		strcpy(str, "Largamente");
		break;
	case MACRO_SOLO:
		strcpy(str, "Solo");
		break;
	case MACRO_TUTTI:
		strcpy(str, "tutti");
		break;
	case MACRO_4_4:
		strcpy(str, "4/4");
		break;
	case MACRO_3_4:
		strcpy(str, "3/4");
		break;
	case MACRO_2_4:
		strcpy(str, "2/4");
		break;
	default:
		return 1;
		break;
	}
	return 0;
}

int str_to_macro(struct macro_t *macro, char *str)
{
	if (strcmp(str, "f") == 0) {
		macro->type = MACRO_F;
	} else if (strcmp(str, "mf") == 0) {
		macro->type = MACRO_MF;
	} else if (strcmp(str, "ff") == 0) {
		macro->type = MACRO_FF;
	} else if (strcmp(str, "p") == 0) {
		macro->type = MACRO_P;
	} else if (strcmp(str, "mp") == 0) {
		macro->type = MACRO_MP;
	} else if (strcmp(str, "pp") == 0) {
		macro->type = MACRO_PP;
	} else if (strcmp(str, "sfz") == 0) {
		macro->type = MACRO_SFZ;
	} else if (strcmp(str, "cresc.") == 0) {
		macro->type = MACRO_CRESC;
	} else if (strcmp(str, "dim.") == 0) {
		macro->type = MACRO_DIM;
	} else if (strcmp(str, "rit.") == 0) {
		macro->type = MACRO_RIT;
	} else if (strcmp(str, "accel.") == 0) {
		macro->type = MACRO_ACCEL;
	} else if (strcmp(str, "sfp") == 0) {
		macro->type = MACRO_SFP;
	} else if (strcmp(str, "sf") == 0) {
		macro->type = MACRO_SF;
	} else if (strcmp(str, "fp") == 0) {
		macro->type = MACRO_FP;
	} else if (strcmp(str, "div.") == 0) {
		macro->type = MACRO_DIV;
	} else if (strcmp(str, "unis.") == 0) {
		macro->type = MACRO_UNIS;
	} else if (strcmp(str, "Largo") == 0) {
		macro->type = MACRO_LARGO;
	} else if (strcmp(str, "Larghetto") == 0) {
		macro->type = MACRO_LARGHETTO;
	} else if (strcmp(str, "Adagio") == 0) {
		macro->type = MACRO_ADAGIO;
	} else if (strcmp(str, "Adagietto") == 0) {
		macro->type = MACRO_ADAGIETTO;
	} else if (strcmp(str, "Andante") == 0) {
		macro->type = MACRO_ANDANTE;
	} else if (strcmp(str, "Andantino") == 0) {
		macro->type = MACRO_ANDANTINO;
	} else if (strcmp(str, "Moderato") == 0) {
		macro->type = MACRO_MODERATO;
	} else if (strcmp(str, "Allegro") == 0) {
		macro->type = MACRO_ALLEGRO;
	} else if (strcmp(str, "Vivace") == 0) {
		macro->type = MACRO_VIVACE;
	} else if (strcmp(str, "Presto") == 0) {
		macro->type = MACRO_PRESTO;
	} else if (strcmp(str, "Prestissimo") == 0) {
		macro->type = MACRO_PRESTISSIMO;
	} else if (strcmp(str, "Largamente") == 0) {
		macro->type = MACRO_LARGAMENTE;
	} else if (strcmp(str, "solo") == 0) {
		macro->type = MACRO_SOLO;
	} else if (strcmp(str, "tutti") == 0) {
		macro->type = MACRO_TUTTI;
	} else if (strcmp(str, "4/4") == 0) {
		macro->type = MACRO_4_4;
	} else if (strcmp(str, "3/4") == 0) {
		macro->type = MACRO_3_4;
	} else if (strcmp(str, "2/4") == 0) {
		macro->type = MACRO_2_4;
	} else if (strcmp(str, "arpeggio") == 0) {
		macro->type = MACRO_ARPEGGIO;
	} else if (strcmp(str, "newline") == 0) {
		macro->type = MACRO_NEW_LINE;
	} else if (strcmp(str, "1=C") == 0) {
		macro->type = MACRO_C_KEY;
	} else if (strcmp(str, "1=G") == 0) {
		macro->type = MACRO_G_KEY;
	} else if (strcmp(str, "1=D") == 0) {
		macro->type = MACRO_D_KEY;
	} else if (strcmp(str, "1=A") == 0) {
		macro->type = MACRO_A_KEY;
	} else if (strcmp(str, "1=E") == 0) {
		macro->type = MACRO_E_KEY;
	} else if (strcmp(str, "1=B") == 0) {
		macro->type = MACRO_B_KEY;
	} else if (strcmp(str, "1=#F") == 0) {
		macro->type = MACRO_F_SHARP_KEY;
	} else if (strcmp(str, "1=#C") == 0) {
		macro->type = MACRO_C_SHARP_KEY;
	} else if (strcmp(str, "1=F") == 0) {
		macro->type = MACRO_F_KEY;
	} else if (strcmp(str, "1=bB") == 0) {
		macro->type = MACRO_B_FLAT_KEY;
	} else if (strcmp(str, "1=bE") == 0) {
		macro->type = MACRO_E_FLAT_KEY;
	} else if (strcmp(str, "1=bA") == 0) {
		macro->type = MACRO_A_FLAT_KEY;
	} else if (strcmp(str, "1=bD") == 0) {
		macro->type = MACRO_D_FLAT_KEY;
	} else if (strcmp(str, "1=bG") == 0) {
		macro->type = MACRO_G_FLAT_KEY;
	} else if (strcmp(str, "1=bC") == 0) {
		macro->type = MACRO_C_FLAT_KEY;
	} else if (strcmp(str, "lrep") == 0) {
		macro->type = MACRO_L_REPEAT;
	} else if (strcmp(str, "rrep") == 0) {
		macro->type = MACRO_R_REPEAT;
	} else if (strcmp(str, "end") == 0) {
		macro->type = MACRO_END;
	} else if (strcmp(str, "volta1") == 0) {
		macro->type = MACRO_VOLTA_1;
	} else if (strcmp(str, "volta1begin") == 0) {
		macro->type = MACRO_VOLTA_1_BEGIN;
	} else if (strcmp(str, "volta1end") == 0) {
		macro->type = MACRO_VOLTA_1_END;
	} else if (strcmp(str, "volta2") == 0) {
		macro->type = MACRO_VOLTA_2;
	} else if (strcmp(str, "volta2begin") == 0) {
		macro->type = MACRO_VOLTA_2_BEGIN;
	} else if (strcmp(str, "volta2end") == 0) {
		macro->type = MACRO_VOLTA_2_END;
	} else if (strcmp(str, "crescbegin") == 0) {
		macro->type = MACRO_CRESC_BEGIN;
	} else if (strcmp(str, "crescend") == 0) {
		macro->type = MACRO_CRESC_END;
	} else if (strcmp(str, "dimbegin") == 0) {
		macro->type = MACRO_DIM_BEGIN;
	} else if (strcmp(str, "dimend") == 0) {
		macro->type = MACRO_DIM_END;
	} else {
		return 1;
	}
	return 0;
}

int string_key_to_int(char *key)
{
	char base = key[0];
	int bias = 0;
	if (strlen(key) == 2) {
		if (key[0] == '#') {
			bias = 1;
		} else if (key[0] == 'b') {
			bias = -1;
		}
		base = key[1];
	}
	switch (base) {
	case 'C':
		return 0 + bias;
	case 'D':
		return 2 + bias;
	case 'E':
		return 4 + bias;
	case 'F':
		return 5 + bias;
	case 'G':
		return -5 + bias;
	case 'A':
		return -3 + bias;
	case 'B':
		return -1 + bias;
	default:
		return -100;
	}
}

#define SET_BIT(bitset, bit) ((bitset) |= (1 << (bit)))
#define CLEAR_BIT(bitset, bit) ((bitset) &= ~(1 << (bit)))

int note_to_midi_number_(struct note_t *note, int sharp_bitset, int flat_bitset,
			 int type_key)
{
	static const int pure_note_to_midi_number[] = { 0, 2, 4, 5, 7, 9, 11 };
	int midi_number = pure_note_to_midi_number[note->note - 1];
	if (sharp_bitset & (1 << note->note)) {
		midi_number++;
	} else if (flat_bitset & (1 << note->note)) {
		midi_number--;
	}
	midi_number += note->octave * 12;
	midi_number += 60; // C4 in MIDI
	midi_number += type_key;
	return midi_number;
}

/**
 * @brief Change MIDI number to sharp notes
 * 
 * Only change note, octave and accidental.
 */
void midi_number_to_sharp_note_(struct note_t *note, int midi_number,
				int *display_local_sharp_bitset,
				int display_key)
{
	midi_number -= display_key;
	note->octave = midi_number / 12 - 5;
	midi_number %= 12;
	static const int midi_number_to_pure_note[] = { 1, 1, 2, 2, 3, 4,
							4, 5, 5, 6, 6, 7 };
	static const enum accidental_t midi_number_to_sharp[] = { 0, 1, 0, 1,
								  0, 0, 1, 0,
								  1, 0, 1, 0 };
	note->note = midi_number_to_pure_note[midi_number];
	int is_sharp = midi_number_to_sharp[midi_number];
	if (is_sharp && !((*display_local_sharp_bitset) & (1 << note->note))) {
		note->accidental = ACC_SHARP;
		SET_BIT(*display_local_sharp_bitset, note->note);
	} else if (!is_sharp &&
		   (((*display_local_sharp_bitset) & (1 << note->note)))) {
		note->accidental = ACC_NATURAL;
		CLEAR_BIT(*display_local_sharp_bitset, note->note);
	} else {
		note->accidental = ACC_NONE;
	}
}

/**
 * @brief Change MIDI number to flat notes
 * 
 * Only change note, octave and accidental.
 */
void midi_number_to_flat_note_(struct note_t *note, int midi_number,
			       int *display_local_flat_bitset, int display_key)
{
	midi_number -= display_key;
	note->octave = midi_number / 12 - 5;
	midi_number %= 12;
	static const int midi_number_to_pure_note[] = { 1, 2, 2, 3, 3, 4,
							5, 5, 6, 6, 7, 7 };
	static const enum accidental_t midi_number_to_flat[] = { 0, 1, 0, 1,
								 0, 0, 1, 0,
								 1, 0, 1, 0 };
	note->note = midi_number_to_pure_note[midi_number];
	int is_flat = midi_number_to_flat[midi_number];
	if (is_flat && !((*display_local_flat_bitset) & (1 << note->note))) {
		note->accidental = ACC_FLAT;
		SET_BIT(*display_local_flat_bitset, note->note);
	} else if (!is_flat &&
		   (((*display_local_flat_bitset) & (1 << note->note)))) {
		note->accidental = ACC_NATURAL;
		CLEAR_BIT(*display_local_flat_bitset, note->note);
	} else {
		note->accidental = ACC_NONE;
	}
}

void update_note(struct note_t *note, int type_key, int display_key,
		 int is_display_sharp, int sharp_bitset, int flat_bitset,
		 int *display_local_sharp_bitset,
		 int *display_local_flat_bitset)
{
	if (type_key == display_key) {
		// Do not need to update.
		return;
	}
	int midi_number =
		note_to_midi_number_(note, sharp_bitset, flat_bitset, type_key);
	if (is_display_sharp) {
		midi_number_to_sharp_note_(note, midi_number,
					   display_local_sharp_bitset,
					   display_key);
	} else {
		midi_number_to_flat_note_(note, midi_number,
					  display_local_flat_bitset,
					  display_key);
	}
}

int parse_staff_by_settings(struct bar_t *staff, int bar_count)
{
	int sharp_bitset = 0;
	int flat_bitset = 0;
	int type_key = 0;
	int display_key = 0;
	int is_display_sharp = 1;

	for (int b = 0; b < bar_count; b++) {
		struct bar_t *bar = &staff[b];
		int local_sharp_bitset = sharp_bitset;
		int local_flat_bitset = flat_bitset;
		int display_local_sharp_bitset = 0;
		int display_local_flat_bitset = 0;
		for (int i = 0; i < bar->element_count; i++) {
			struct element_t *element = &bar->elements[i];
			// Process settings
			if (element->type == ELEMENT_SETTING) {
				struct setting_t *setting =
					&element->data.setting;
				if (setting->type == SETTING_SHARP) {
					SET_BIT(sharp_bitset, setting->note);
					CLEAR_BIT(flat_bitset, setting->note);
					SET_BIT(local_sharp_bitset,
						setting->note);
					CLEAR_BIT(local_flat_bitset,
						  setting->note);
				} else if (setting->type == SETTING_FLAT) {
					CLEAR_BIT(sharp_bitset, setting->note);
					SET_BIT(flat_bitset, setting->note);
					CLEAR_BIT(local_sharp_bitset,
						  setting->note);
					SET_BIT(local_flat_bitset,
						setting->note);
				} else if (setting->type == SETTING_NATURAL) {
					CLEAR_BIT(sharp_bitset, setting->note);
					CLEAR_BIT(flat_bitset, setting->note);
					CLEAR_BIT(local_sharp_bitset,
						  setting->note);
					CLEAR_BIT(local_flat_bitset,
						  setting->note);
				} else if (setting->type == SETTING_TYPE_KEY) {
					type_key =
						string_key_to_int(setting->key);
				} else if (setting->type ==
					   SETTING_DISPLAY_KEY) {
					display_key =
						string_key_to_int(setting->key);
				} else if (setting->type ==
					   SETTING_DISPLAY_SHARP) {
					is_display_sharp = 1;
				} else if (setting->type ==
					   SETTING_DISPLAY_FLAT) {
					is_display_sharp = 0;
				}
			} else if (element->type == ELEMENT_NOTE) {
				struct note_t *note = &element->data.note;
				if (note->type != NOTE_NOTE) {
					continue;
				}
				if (note->accidental == ACC_SHARP) {
					SET_BIT(local_sharp_bitset, note->note);
					CLEAR_BIT(local_flat_bitset,
						  note->note);
				} else if (note->accidental == ACC_FLAT) {
					CLEAR_BIT(local_sharp_bitset,
						  note->note);
					SET_BIT(local_flat_bitset, note->note);
				} else if (note->accidental == ACC_NATURAL) {
					CLEAR_BIT(local_sharp_bitset,
						  note->note);
					CLEAR_BIT(local_flat_bitset,
						  note->note);
				}
				update_note(note, type_key, display_key,
					    is_display_sharp,
					    local_sharp_bitset,
					    local_flat_bitset,
					    &display_local_sharp_bitset,
					    &display_local_flat_bitset);
			} else if (element->type == ELEMENT_CHORD) {
				struct chord_t *chord = &element->data.chord;
				for (int j = 0; j < chord->note_count; j++) {
					struct note_t *note = &chord->notes[j];
					if (note->accidental == ACC_SHARP) {
						SET_BIT(local_sharp_bitset,
							note->note);
						CLEAR_BIT(local_flat_bitset,
							  note->note);
					} else if (note->accidental ==
						   ACC_FLAT) {
						CLEAR_BIT(local_sharp_bitset,
							  note->note);
						SET_BIT(local_flat_bitset,
							note->note);
					} else if (note->accidental ==
						   ACC_NATURAL) {
						CLEAR_BIT(local_sharp_bitset,
							  note->note);
						CLEAR_BIT(local_flat_bitset,
							  note->note);
					}
				}
				for (int j = 0; j < chord->note_count; j++) {
					struct note_t *note = &chord->notes[j];
					update_note(note, type_key, display_key,
						    is_display_sharp,
						    local_sharp_bitset,
						    local_flat_bitset,
						    &display_local_sharp_bitset,
						    &display_local_flat_bitset);
				}
			}
		}
	}
	return 0;
}