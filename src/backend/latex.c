#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "notation.h"

#define MAX_STR_LEN 102400
#define MAX_LINE 1024
#define MAX_LENGTH_PER_LINE 100
#define MAX_BAR_PER_LINE 12
#define MAX_NOTE_STRING_LEN 512
#define MAX_MACRO_STRING_LEN 32

int latex_parse_note(char *str, struct note_t *note,
		     enum duration_t previous_note_duration)
{
	int left_brackets = 0;
	if (note->type == NOTE_DOT) {
		if (previous_note_duration == DUR_EIGHTH) {
			strcat(str, " \\underline{\\cdot} ");
		} else if (previous_note_duration == DUR_QUARTER) {
			strcat(str, " \\cdot \\hspace{1.5em}");
		}
	}
	if (note->type == NOTE_TIE) {
		strcat(str, " - \\hspace{1.5em} ");
	}
	if (note->type == NOTE_REST || note->type == NOTE_NOTE) {
		char note_str[MAX_NOTE_STRING_LEN];
		note_str[0] = '\0';
		// Proceed articulation
		if (ART_STRESS & note->articulation) {
			strcat(note_str, " \\stress{ ");
			left_brackets += 1;
		}
		if (ART_STACCATO & note->articulation) {
			strcat(note_str, " \\staccato{ ");
			left_brackets += 1;
		}
		if (ART_TREMOLO & note->articulation) {
			strcat(note_str, " \\tremolo{ ");
			left_brackets += 1;
		}
		// Proceed octave higher
		for (int i = 0; i < note->octave; i++) {
			strcat(note_str, " \\dot{ ");
			left_brackets += 1;
		}
		// Proceed octave lower
		for (int i = 0; i > note->octave; i--) {
			strcat(note_str, " \\udot{ ");
			left_brackets += 1;
		}
		// Proceed duration
		if (note->duration == DUR_EIGHTH) {
			strcat(note_str, " \\underline{ ");
			left_brackets += 1;
		} else if (note->duration == DUR_SIXTEENTH) {
			strcat(note_str, " \\underline{\\underline{ ");
			left_brackets += 2;
		}
		// Proceed accidental
		if (note->accidental == ACC_SHARP) {
			strcat(note_str, " ^\\sharp ");
		} else if (note->accidental == ACC_FLAT) {
			strcat(note_str, " ^\\flat ");
		} else if (note->accidental == ACC_NATURAL) {
			strcat(note_str, " ^\\natural ");
		}
		// Proceed note
		char note_char[2];
		note_char[0] = '0' + note->note;
		note_char[1] = '\0';
		strcat(note_str, note_char);
		// Add brackets
		for (int i = 0; i < left_brackets; i++) {
			strcat(note_str, " } ");
		}

		// Proceed padding
		if (note->duration == DUR_QUARTER) {
			strcat(note_str, " \\hspace{1.5em}");
		} else if (note->duration == DUR_EIGHTH) {
			strcat(note_str, " \\underline{\\hspace{0.5em}}");
		}

		// Add to str
		strcat(str, note_str);
	}
	return 0;
}

int latex_parse_chord(char *str, struct chord_t *chord)
{
	int chord_left_brackets = 0;
	char chord_str[MAX_NOTE_STRING_LEN * MAX_NOTE_PER_CHORD];
	chord_str[0] = '\0';
	// From the bottom to the top
	for (int i = chord->note_count - 1; i >= 0; i--) {
		struct note_t *note = &chord->notes[i];
		if (note->type == NOTE_REST || note->type == NOTE_NOTE) {
			int note_left_brackets = 0;
			// Left brackets numbers for the stackon
			if (i == chord->note_count - 1 || i == 0) {
				note_left_brackets = 1;
			} else {
				note_left_brackets = 2;
			}
			// Proceed octave lower, duration and stackon for the bottom
			if (i == chord->note_count - 1) {
				// Proceed octave lower
				for (int i = 0; i > note->octave; i--) {
					strcat(chord_str, " \\udot{ ");
					chord_left_brackets += 1;
				}
				// Proceed duration
				if (chord->duration == DUR_EIGHTH) {
					strcat(chord_str, " \\underline{ ");
					chord_left_brackets += 1;
				} else if (chord->duration == DUR_SIXTEENTH) {
					strcat(chord_str,
					       " \\underline{\\underline{ ");
					chord_left_brackets += 2;
				}
				// Proceed stackon
				for (int j = 0; j < chord->note_count - 1;
				     j++) {
					strcat(chord_str, " \\stackon{ ");
				}
			}
			// Proceed the stackon left bracket
			if (i != chord->note_count - 1) {
				strcat(chord_str, " { ");
			}
			// Proceed articulation for the top
			if (i == 0) {
				if (ART_STRESS & chord->articulation) {
					strcat(chord_str, " \\stress{ ");
					note_left_brackets += 1;
				}
				if (ART_STACCATO & chord->articulation) {
					strcat(chord_str, " \\staccato{ ");
					note_left_brackets += 1;
				}
				if (ART_TREMOLO & chord->articulation) {
					strcat(chord_str, " \\tremolo{ ");
					note_left_brackets += 1;
				}
			}
			// Proceed octave higher
			for (int i = 0; i < note->octave; i++) {
				strcat(chord_str, " \\dot{ ");
				note_left_brackets += 1;
			}
			// Proceed octave lower for the notes not on the bottom
			if (i != chord->note_count - 1) {
				for (int i = 0; i > note->octave; i--) {
					strcat(chord_str, " \\udot{ ");
					note_left_brackets += 1;
				}
			}
			// Proceed accidental
			if (note->accidental == ACC_SHARP) {
				strcat(chord_str, " ^\\sharp ");
			} else if (note->accidental == ACC_FLAT) {
				strcat(chord_str, " ^\\flat ");
			} else if (note->accidental == ACC_NATURAL) {
				strcat(chord_str, " ^\\natural ");
			}
			// Proceed note
			char note_char[2];
			note_char[0] = '0' + note->note;
			note_char[1] = '\0';
			strcat(chord_str, note_char);
			// Add brackets
			for (int i = 0; i < note_left_brackets; i++) {
				strcat(chord_str, " } ");
			}
		} else {
			return 1;
		}
	}

	for (int i = 0; i < chord_left_brackets; i++) {
		strcat(chord_str, " } ");
	}
	// Proceed padding
	if (chord->duration == DUR_QUARTER) {
		strcat(chord_str, " \\hspace{1.5em}");
	} else if (chord->duration == DUR_EIGHTH) {
		strcat(chord_str, " \\underline{\\hspace{0.5em}}");
	}

	// Add to str
	strcat(str, chord_str);
	return 0;
}

int latex_parse_macro(char *str, struct macro_t *macro)
{
	char macro_str[MAX_MACRO_STRING_LEN];
	macro_to_str(macro_str, macro);
	strcat(str, " \\macro{ ");
	strcat(str, macro_str);
	strcat(str, " } ");
	return 0;
}

int latex_parse_inner_macro(char *str, struct macro_t *macro)
{
	if (macro->type == MACRO_4_4) {
		strcat(str, " \\frac{4}{4} \\hspace{1.5em} ");
	}
	if (macro->type == MACRO_3_4) {
		strcat(str, " \\frac{3}{4} \\hspace{1.5em} ");
	}
	if (macro->type == MACRO_2_4) {
		strcat(str, " \\frac{2}{4} \\hspace{1.5em}  ");
	}
	return 0;
}

int latex_parse_key_macro(char *str, struct macro_t *macro)
{
	strcat(str, "$ 1 = ");
	if (macro->type == MACRO_C_KEY) {
		strcat(str, "\\text{C}");
	} else if (macro->type == MACRO_G_KEY) {
		strcat(str, "\\text{G}");
	} else if (macro->type == MACRO_D_KEY) {
		strcat(str, "\\text{D}");
	} else if (macro->type == MACRO_A_KEY) {
		strcat(str, "\\text{A}");
	} else if (macro->type == MACRO_E_KEY) {
		strcat(str, "\\text{E}");
	} else if (macro->type == MACRO_B_KEY) {
		strcat(str, "\\text{B}");
	} else if (macro->type == MACRO_F_SHARP_KEY) {
		strcat(str, "^\\sharp \\text{F}");
	} else if (macro->type == MACRO_C_SHARP_KEY) {
		strcat(str, "^\\sharp \\text{C}");
	} else if (macro->type == MACRO_F_KEY) {
		strcat(str, "\\text{F}");
	} else if (macro->type == MACRO_B_FLAT_KEY) {
		strcat(str, "^\\flat \\text{B}");
	} else if (macro->type == MACRO_E_FLAT_KEY) {
		strcat(str, "^\\flat \\text{E}");
	} else if (macro->type == MACRO_A_FLAT_KEY) {
		strcat(str, "^\\flat \\text{A}");
	} else if (macro->type == MACRO_D_FLAT_KEY) {
		strcat(str, "^\\flat \\text{D}");
	} else if (macro->type == MACRO_G_FLAT_KEY) {
		strcat(str, "^\\flat \\text{G}");
	} else if (macro->type == MACRO_C_FLAT_KEY) {
		strcat(str, "^\\flat \\text{C}");
	} else {
		return 1;
	}
	strcat(str, " $ ");
	return 0;
}

int latex_parse(char *str, struct meta_t *meta, struct bar_t *staff,
		int bar_count)
{
	char header[] =
		"\\documentclass[12pt]{article}\n"
		"\\usepackage{amsmath}\n"
		"\\usepackage{amssymb}\n"
		"\\usepackage{accents}\n"
		"\\usepackage{setspace}\n"
		"\\usepackage{geometry}\n"
		"\\usepackage{stackengine}\n"
		"\\usepackage{tikz}\n"
		"\\stackMath\n"
		"\\geometry{a4paper, scale=0.8}\n"
		"\\parskip 1ex\n"
		"\\newcommand\\udot[1]{\\underaccent{\\dot}{#1}}\n"
		"\\newcommand\\stress[1]{\\accentset{>}{#1}}\n"
		"\\newcommand\\staccato[1]{\\accentset{\\blacktriangledown}{#1}}\n"
		"\\newcommand\\macro[1]{\\scriptsize{\\textbf{\\textit{#1}}}}\n"
		"\\newcommand\\tremolo[1]{\\accentset{\n"
		"\\begin{tikzpicture}\n"
		"    \\draw (0.2em, 0.4em) -- (0.6em, 0.8em);\n"
		"    \\draw (0.3em, 0.3em) -- (0.7em, 0.7em);\n"
		"    \\draw (0.4em, 0.2em) -- (0.8em, 0.6em);\n"
		"\\end{tikzpicture}}{#1}\n"
		"}\n";

	strcat(str, header);

	// Generate meta data
	char meta_str[MAX_META_STRING_LENGTH * 3];
	meta_str[0] = '\0';
	strcat(meta_str, "\\title{ ");
	strcat(meta_str, meta->title);
	strcat(meta_str, "}\n\\author{ ");
	strcat(meta_str, meta->composer);
	strcat(meta_str, "}\n\\date{ ");
	strcat(meta_str, meta->instrument);

	strcat(str, meta_str);

	strcat(str, "}\n\\begin{document}\n\\maketitle\n");

	// Determine line break
	int line_break_id[MAX_LINE];
	line_break_id[0] = 0;
	int line_count = 0;
	int length_of_this_line = 0;
	for (int b = 0; b < bar_count; b++) {
		struct bar_t *bar = &staff[b];
		int new_line_macro = 0;
		for (int i = 0; i < bar->element_count; i++) {
			if (bar->elements[i].type == ELEMENT_NOTE) {
				if (bar->elements[i].data.note.duration ==
				    DUR_SIXTEENTH) {
					length_of_this_line += 1;
				} else if (bar->elements[i].data.note.duration ==
					   DUR_EIGHTH) {
					length_of_this_line += 2;
				} else if (bar->elements[i].data.note.duration ==
					   DUR_QUARTER) {
					length_of_this_line += 4;
				}
			} else if (bar->elements[i].type == ELEMENT_MACRO &&
				   IS_INNER_MACRO(
					   bar->elements[i].data.macro.type)) {
				length_of_this_line += 4;
			} else if (bar->elements[i].type == ELEMENT_MACRO &&
				   bar->elements[i].data.macro.type ==
					   MACRO_NEW_LINE) {
				new_line_macro = 1;
			}
		}
		if (length_of_this_line >= MAX_LENGTH_PER_LINE ||
		    new_line_macro) {
			line_break_id[line_count + 1] = b;
			line_count += 1;
			length_of_this_line = 0;
		}
	}
	line_break_id[line_count + 1] = bar_count;
	line_count += 1;

	// Render staff
	for (int lineno = 0; lineno < line_count; lineno++) {
		// Line start
		char line_start[] = "\\begin{tabular}{";
		char align[MAX_LENGTH_PER_LINE];
		for (int i = line_break_id[lineno];
		     i < line_break_id[lineno + 1]; i++) {
			align[i - line_break_id[lineno]] = 'l';
		}
		align[line_break_id[lineno + 1] - line_break_id[lineno]] = '\0';
		strcat(str, line_start);
		strcat(str, align);
		strcat(str, "}\n");
		// Bar number and Key macros
		for (int b = line_break_id[lineno];
		     b < line_break_id[lineno + 1]; b++) {
			struct bar_t *bar = &staff[b];
			// Bar number
			if (b % 5 == 4) {
				char bar_number_str[8];
				sprintf(bar_number_str, "%d", b + 1);
				strcat(str, "\\scriptsize{\\boxed{");
				strcat(str, bar_number_str);
				strcat(str, "}}");
			}
			// Key macros
			for (int i = 0; i < bar->element_count; i++) {
				if (bar->elements[i].type == ELEMENT_MACRO &&
				    IS_KEY_MACRO(
					    bar->elements[i].data.macro.type)) {
					latex_parse_key_macro(
						str,
						&bar->elements[i].data.macro);
				}
			}
			// Separator
			if (b != line_break_id[lineno + 1] - 1) {
				strcat(str, " & ");
			}
		}
		strcat(str, " \\\\ \n");
		// Upper Macros
		for (int b = line_break_id[lineno];
		     b < line_break_id[lineno + 1]; b++) {
			struct bar_t *bar = &staff[b];
			for (int i = 0; i < bar->element_count; i++) {
				if (bar->elements[i].type == ELEMENT_NOTE) {
					continue;
				} else if (bar->elements[i].type ==
						   ELEMENT_MACRO &&
					   IS_UPPER_MACRO(
						   bar->elements[i]
							   .data.macro.type)) {
					latex_parse_macro(
						str,
						&bar->elements[i].data.macro);
				}
			}
			if (b != line_break_id[lineno + 1] - 1) {
				strcat(str, " & ");
			}
		}
		strcat(str, " \\\\ \n");
		// Detect barline
		int l_repeat_barline[MAX_BAR_PER_LINE];
		int r_repeat_barline[MAX_BAR_PER_LINE];
		int end_barline[MAX_BAR_PER_LINE];
		for (int b = line_break_id[lineno];
		     b < line_break_id[lineno + 1]; b++) {
			l_repeat_barline[b - line_break_id[lineno]] = 0;
			r_repeat_barline[b - line_break_id[lineno]] = 0;
			end_barline[b - line_break_id[lineno]] = 0;
			struct bar_t *bar = &staff[b];
			for (int i = 0; i < bar->element_count; i++) {
				if (bar->elements[i].type == ELEMENT_MACRO) {
					struct macro_t *macro =
						&bar->elements[i].data.macro;
					if (macro->type == MACRO_L_REPEAT) {
						l_repeat_barline
							[b -
							 line_break_id[lineno]] =
								1;
					} else if (macro->type ==
						   MACRO_R_REPEAT) {
						r_repeat_barline
							[b -
							 line_break_id[lineno]] =
								1;
					} else if (macro->type == MACRO_END) {
						end_barline
							[b -
							 line_break_id[lineno]] =
								1;
					}
				}
			}
		}

		// Inner
		for (int b = line_break_id[lineno];
		     b < line_break_id[lineno + 1]; b++) {
			// Debug barline
			struct bar_t *bar = &staff[b];
			// Left barline
			if (l_repeat_barline[b - line_break_id[lineno]]) {
				strcat(str, " \\multicolumn{1}{||l|}{$ : ");
			} else if (r_repeat_barline[b - line_break_id[lineno]]) {
				strcat(str, " \\multicolumn{1}{l||}{$ ");
			} else if (end_barline[b - line_break_id[lineno]]) {
				strcat(str, " \\multicolumn{1}{l||}{$ ");
			} else {
				strcat(str, "\\multicolumn{1}{l|}{$ ");
			}
			int number_of_sixteenth_duration =
				0; // To control space between sixteenth notes
			int previous_note_duration = -1;
			for (int i = 0; i < bar->element_count; i++) {
				if (bar->elements[i].type == ELEMENT_NOTE) {
					latex_parse_note(
						str,
						&bar->elements[i].data.note,
						previous_note_duration);
					if (bar->elements[i].data.note.duration ==
					    DUR_SIXTEENTH) {
						number_of_sixteenth_duration +=
							1;
					} else if (bar->elements[i]
							   .data.note.duration ==
						   DUR_EIGHTH) {
						number_of_sixteenth_duration +=
							2;
					} else if (bar->elements[i]
							   .data.note.duration ==
						   DUR_QUARTER) {
						number_of_sixteenth_duration +=
							4;
					}
					previous_note_duration =
						bar->elements[i]
							.data.note.duration;
				} else if (bar->elements[i].type ==
					   ELEMENT_CHORD) {
					latex_parse_chord(
						str,
						&bar->elements[i].data.chord);
					if (bar->elements[i]
						    .data.chord.duration ==
					    DUR_SIXTEENTH) {
						number_of_sixteenth_duration +=
							1;
					} else if (bar->elements[i]
							   .data.chord
							   .duration ==
						   DUR_EIGHTH) {
						number_of_sixteenth_duration +=
							2;
					} else if (bar->elements[i]
							   .data.chord
							   .duration ==
						   DUR_QUARTER) {
						number_of_sixteenth_duration +=
							4;
					}
					previous_note_duration =
						bar->elements[i]
							.data.chord.duration;
				} else if (bar->elements[i].type ==
						   ELEMENT_MACRO &&
					   IS_INNER_MACRO(
						   bar->elements[i]
							   .data.macro.type)) {
					latex_parse_inner_macro(
						str,
						&bar->elements[i].data.macro);
				}

				if (number_of_sixteenth_duration == 4) {
					strcat(str, " \\hspace{0.5em} ");
					number_of_sixteenth_duration = 0;
				}
			}
			// Right barline
			if (r_repeat_barline[b - line_break_id[lineno]]) {
				strcat(str, " : ");
			}
			if (b != line_break_id[lineno + 1] - 1) {
				strcat(str, " $}& ");
			}
		}
		strcat(str, " $} \\\\ \n");
		// Lower
		for (int b = line_break_id[lineno];
		     b < line_break_id[lineno + 1]; b++) {
			struct bar_t *bar = &staff[b];
			for (int i = 0; i < bar->element_count; i++) {
				if (bar->elements[i].type == ELEMENT_NOTE) {
					continue;
				} else if (bar->elements[i].type ==
						   ELEMENT_MACRO &&
					   IS_LOWER_MACRO(
						   bar->elements[i]
							   .data.macro.type)) {
					latex_parse_macro(
						str,
						&bar->elements[i].data.macro);
				}
			}
			if (b != line_break_id[lineno + 1] - 1) {
				strcat(str, " & ");
			}
		}
		strcat(str, " \n");
		// End Line
		char line_end[] = "\\end{tabular}\n\n";
		strcat(str, line_end);
	}

	char footer[] = "\\end{document}\n";
	strcat(str, footer);
	return 0;
}

int latex_render(struct meta_t *meta, struct bar_t *bar_t, int bar_count,
		 char *filename)
{
	char str[MAX_STR_LEN];
	str[0] = '\0';
	latex_parse(str, meta, bar_t, bar_count);

	// Write to file
	FILE *fp = fopen(filename, "w");
	fprintf(fp, "%s", str);
	fclose(fp);

	// Compile to pdf
	char command[MAX_STR_LEN];
	command[0] = '\0';
	strcat(command, "pdflatex ");
	strcat(command, filename);
	system(command);

	return 0;
}