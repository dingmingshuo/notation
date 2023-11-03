#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "notation.h"

#define MAX_STR_LEN 102400
#define MAX_LINE 1024
#define MAX_LENGTH_PER_LINE 100
#define MAX_BAR_PER_LINE 12
#define MAX_NOTE_STRING_LEN 512
#define MAX_MACRO_STRING_LEN 64

// Lengths are represented in mu to em
#define LENGTH_ACCIDENTAL (7.0 / 14.0)

int latex_parse_note(char *str, struct note_t *note,
		     enum duration_t previous_note_duration)
{
	int left_brackets = 0;
	if (note->type == NOTE_DOT) {
		if (previous_note_duration == DUR_EIGHTH) {
			strcat(str, " \\underline{\\cdot} ");
		} else if (previous_note_duration == DUR_QUARTER) {
			strcat(str, " \\cdot \\hspace{0.2em}");
		}
	}
	if (note->type == NOTE_TIE) {
		strcat(str, " - \\hspace{0.5em} ");
	}
	if (note->type == NOTE_REST || note->type == NOTE_NOTE) {
		char note_str[MAX_NOTE_STRING_LEN];
		note_str[0] = '\0';
		// Proceed padding
		if (note->duration == DUR_EIGHTH) {
			strcat(note_str, " \\underline{\\hspace{0.1em}}");
		}
		// Proceed accidental
		if (note->duration == DUR_EIGHTH) {
			strcat(note_str, " \\underline{ ");
		} else if (note->duration == DUR_SIXTEENTH) {
			strcat(note_str, " \\underline{\\underline{ ");
		}
		if (note->accidental == ACC_SHARP) {
			strcat(note_str, " ^\\sharp ");
		} else if (note->accidental == ACC_FLAT) {
			strcat(note_str, " ^\\flat ");
		} else if (note->accidental == ACC_NATURAL) {
			strcat(note_str, " ^\\natural ");
		}
		if (note->duration == DUR_EIGHTH) {
			strcat(note_str, " } ");
		} else if (note->duration == DUR_SIXTEENTH) {
			strcat(note_str, " }} ");
		}

		// Proceed articulation
		if (ART_STRESS & note->articulation) {
			strcat(note_str, " \\stress{ ");
			left_brackets += 1;
		}
		if (ART_STACCATO & note->articulation) {
			strcat(note_str, " \\staccato{ ");
			left_brackets += 1;
		}
		if (ART_FERMATA & note->articulation) {
			strcat(note_str, " \\fermata{ ");
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
			strcat(note_str, " \\hspace{0.5em}");
		} else if (note->duration == DUR_EIGHTH) {
			strcat(note_str, " \\underline{\\hspace{0.1em}}");
		}

		// Add to str
		strcat(str, note_str);
	}
	return 0;
}

int latex_parse_chord(char *str, struct chord_t *chord)
{
	char chord_str[MAX_NOTE_STRING_LEN * MAX_NOTE_PER_CHORD];
	chord_str[0] = '\0';

	// Proceed padding
	if (chord->duration == DUR_EIGHTH) {
		strcat(chord_str, " \\underline{\\hspace{0.1em}}");
	}
	// Proceed accidental from the bottom to the top
	int has_accidental = 0;
	for (int i = chord->note_count - 1; i >= 0; i--) {
		struct note_t *note = &chord->notes[i];
		if (note->accidental != ACC_NONE) {
			has_accidental = 1;
			break;
		}
	}
	if (has_accidental) {
		// Process duration
		if (chord->duration == DUR_EIGHTH) {
			strcat(chord_str, " \\underline{ ");
		} else if (chord->duration == DUR_SIXTEENTH) {
			strcat(chord_str, " \\underline{\\underline{ ");
		}
		// Process Longstack
		strcat(chord_str, " \\Longstack{");
		for (int i = 0; i < chord->note_count; i++) {
			// Process accidentals
			struct note_t *note = &chord->notes[i];
			if (note->accidental == ACC_SHARP) {
				strcat(chord_str, "{^\\sharp}");
			} else if (note->accidental == ACC_FLAT) {
				strcat(chord_str, "{^\\flat}");
			} else if (note->accidental == ACC_NATURAL) {
				strcat(chord_str, "{^\\natural}");
			} else if (note->accidental == ACC_NONE) {
				strcat(chord_str, "{\\vphantom{^\\sharp}}");
			}
			if (i != chord->note_count - 1) {
				strcat(chord_str, " ");
			}
		}
		// Process right bracket of Longstack
		strcat(chord_str, "} ");
		// Process duration right brackets.
		if (chord->duration == DUR_EIGHTH) {
			strcat(chord_str, "} ");
		} else if (chord->duration == DUR_SIXTEENTH) {
			strcat(chord_str, "}} ");
		}
	}

	// Proceed notes from the top to the bottom
	// Proceed octave lower, duration and stackon for the bottom
	int chord_left_brackets = 0;
	struct note_t *note = &chord->notes[chord->note_count - 1];
	for (int i = 0; i > note->octave; i--) {
		strcat(chord_str, " \\udot{ ");
		chord_left_brackets += 1;
	}
	// Proceed duration
	if (chord->duration == DUR_EIGHTH) {
		strcat(chord_str, " \\underline{ ");
		chord_left_brackets += 1;
	} else if (chord->duration == DUR_SIXTEENTH) {
		strcat(chord_str, " \\underline{\\underline{ ");
		chord_left_brackets += 2;
	}
	// Process Longstack
	strcat(chord_str, " \\Longstack{");
	chord_left_brackets += 1;
	for (int i = 0; i < chord->note_count; i++) {
		struct note_t *note = &chord->notes[i];
		if (note->type == NOTE_REST || note->type == NOTE_NOTE) {
			int note_left_brackets = 1;
			strcat(chord_str, "{");
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
				if (ART_FERMATA & chord->articulation) {
					strcat(chord_str, " \\fermata{ ");
					note_left_brackets += 1;
				}
				if (ART_TREMOLO & chord->articulation) {
					strcat(chord_str, " \\tremolo{ ");
					note_left_brackets += 1;
				}
			}
			// Proceed octave higher
			for (int j = 0; j < note->octave; j++) {
				strcat(chord_str, " \\dot{ ");
				note_left_brackets += 1;
			}
			// Proceed octave lower for the notes not on the bottom
			if (i != chord->note_count - 1) {
				for (int j = 0; j > note->octave; j--) {
					strcat(chord_str, " \\udot{ ");
					note_left_brackets += 1;
				}
			}
			// Proceed note
			char note_char[2];
			note_char[0] = '0' + note->note;
			note_char[1] = '\0';
			strcat(chord_str, note_char);
			// Add brackets
			for (int j = 0; j < note_left_brackets; j++) {
				strcat(chord_str, "}");
			}
			if (i != chord->note_count - 1) {
				strcat(chord_str, " ");
			}
		} else {
			return 1;
		}
	}

	for (int i = 0; i < chord_left_brackets; i++) {
		strcat(chord_str, "}");
	}
	// Proceed padding
	if (chord->duration == DUR_QUARTER) {
		strcat(chord_str, " \\hspace{0.5em}");
	} else if (chord->duration == DUR_EIGHTH) {
		strcat(chord_str, " \\underline{\\hspace{0.1em}}");
	}

	// Add to str
	strcat(str, chord_str);
	return 0;
}

int latex_parse_macro(char *str, struct macro_t *macro)
{
	char macro_str[MAX_MACRO_STRING_LEN];
	if (macro->type == MACRO_TEMPO) {
		char tempo_str[MAX_MACRO_STRING_LEN];
		sprintf(tempo_str, "\\tempomacro{%d}", (int)(macro->value));
		strcat(str, tempo_str);
	} else {
		macro_to_str(macro_str, macro);
		strcat(str, "\\macro{");
		strcat(str, macro_str);
		strcat(str, " }");
	}
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
	if (macro->type == MACRO_ARPEGGIO) {
		strcat(str, " \\arpeggio ");
	}
	return 0;
}

int latex_parse_key_macro(char *str, struct macro_t *macro)
{
	strcat(str, "$\\normalsize 1 = ");
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
	strcat(str, " \\hspace{0.5em}$ ");
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
		"\\usepackage{fancyhdr}\n"
		"\\usepackage{geometry}\n"
		"\\usepackage{stackengine}\n"
		"\\usepackage{tikz}\n"
		"\\usepackage{array}\n"
		"\\usepackage{ctex}\n"
		"\\usepackage{xfp}\n"
		"\\stackMath\n"
		"\\geometry{a4paper, scale=0.8, left=0.5cm, right=0.5cm, top=1.5cm, bottom=2cm}\n"
		"\\setlength{\\headheight}{15pt}\n"
		"\\parskip 1ex\n"
		"\\newcommand\\VRule[1][\\arrayrulewidth]{\\vrule width #1}\n"
		"\\newcommand\\thickbar{\\VRule[1.5pt]}\n"
		"\\newcommand\\udot[1]{\\oalign{$\\m@th#1$\\cr\\hidewidth\\scalebox{0.38}{\\textbullet}\\hidewidth}}\n"
		"\\newcommand\\stress[1]{\\accentset{>}{#1}}\n"
		"\\newcommand\\staccato[1]{\\accentset{\\blacktriangledown}{#1}}\n"
		"\\newcommand\\macro[1]{\\scriptsize{\\textbf{\\textit{#1}}}}\n"
		"\\newcommand\\tremolo[1]{\\accentset{\n"
		"\\begin{tikzpicture}\n"
		"    \\draw (0.2em, 0.4em) -- (0.6em, 0.8em);\n"
		"    \\draw (0.3em, 0.3em) -- (0.7em, 0.7em);\n"
		"    \\draw (0.4em, 0.2em) -- (0.8em, 0.6em);\n"
		"\\end{tikzpicture}}{#1}\n"
		"}\n"
		"\\newcommand\\fermata[1]{\\accentset{\n"
		"\\begin{tikzpicture}\n"
		"    \\filldraw (0.5em, 0em) arc (0:180:0.5em)\n"
		"        arc (180:0:0.5em and 0.4em);\n"
		"    \\filldraw (0em, 0em) circle (0.05em);\n"
		"    \\fill [white] (-0.5em, -0.3em) rectangle (0.5em, -0.1em);\n"
		"\\end{tikzpicture}}{#1}\n"
		"}\n"
		"\\newcommand\\arpeggio[1]{\n"
		"\\begin{tikzpicture}\n"
		"\\draw (0em, 0em) arc (45:-45:0.3em)\n"
		"    arc (135:225:0.3em) arc (45:-45:0.3em)\n"
		"    arc (135:225:0.3em) arc (45:-45:0.3em)\n"
		"    arc (135:225:0.3em) arc (45:-45:0.3em);\n"
		"\\end{tikzpicture}\n"
		"}\n"
		"\\newcommand\\tempomacro[1]{\\small\n"
		"\\begin{tikzpicture}[x=0.75pt,y=0.75pt,yscale=-1,xscale=1]\n"
		"\\draw [line width=0.75]    (199.62,99) -- (199.62,111.59) ;\n"
		"\\draw  [fill={rgb, 255:red, 0; green, 0; blue, 0 }  ,fill opacity=1 ] (194.68,112.93) .. controls (194.47,112.15) and (195.4,111.22) .. (196.77,110.85) .. controls (198.13,110.48) and (199.41,110.81) .. (199.62,111.59) .. controls (199.83,112.37) and (198.9,113.3) .. (197.53,113.67) .. controls (196.17,114.04) and (194.89,113.71) .. (194.68,112.93) -- cycle ;\n"
		"\\draw (201.62,102) node [anchor=north west][inner sep=0.75pt]   [align=left] {$=#1$};\n"
		"\\end{tikzpicture}\n"
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
	float length_of_this_line = 0;
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
					length_of_this_line += 3.5;
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
	int upper_long_term_macro = 0;
	int lower_long_term_macro = 0;
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
		// Upper longterm Macros
		int has_upper_long_term_macros = 0;
		if (upper_long_term_macro) {
			has_upper_long_term_macros = 1;
		}
		for (int b = line_break_id[lineno];
		     b < line_break_id[lineno + 1]; b++) {
			struct bar_t *bar = &staff[b];
			for (int i = 0; i < bar->element_count; i++) {
				if (bar->elements[i].type == ELEMENT_MACRO &&
				    IS_UPPER_LONG_TERM_MACRO(
					    bar->elements[i].data.macro.type)) {
					has_upper_long_term_macros = 1;
				}
			}
		}
		if (has_upper_long_term_macros) {
			// cline of volta brackets
			int has_cline = upper_long_term_macro;
			for (int b = line_break_id[lineno];
			     b < line_break_id[lineno + 1]; b++) {
				struct bar_t *bar = &staff[b];
				for (int i = 0; i < bar->element_count; i++) {
					if (bar->elements[i].type ==
						    ELEMENT_MACRO &&
					    IS_UPPER_LONG_TERM_MACRO(
						    bar->elements[i]
							    .data.macro.type)) {
						enum macro_type_t macro_type =
							bar->elements[i]
								.data.macro.type;
						if (macro_type ==
							    MACRO_VOLTA_1_BEGIN ||
						    macro_type ==
							    MACRO_VOLTA_2_BEGIN) {
							has_cline = 1;
						}
						if (macro_type ==
							    MACRO_VOLTA_1 ||
						    macro_type ==
							    MACRO_VOLTA_2 ||
						    macro_type ==
							    MACRO_VOLTA_1_END ||
						    macro_type ==
							    MACRO_VOLTA_2_END) {
							char cline_str
								[MAX_MACRO_STRING_LEN];
							sprintf(cline_str,
								"\\cline{%d-%d}",
								b - line_break_id[lineno] +
									1,
								b - line_break_id[lineno] +
									1);
							strcat(str, cline_str);
							has_cline = 0;
						}
					}
				}
				if (has_cline) {
					char cline_str[MAX_MACRO_STRING_LEN];
					sprintf(cline_str, "\\cline{%d-%d}",
						b - line_break_id[lineno] + 1,
						b - line_break_id[lineno] + 1);
					strcat(str, cline_str);
				}
			}
			// Process upper long term macros
			for (int b = line_break_id[lineno];
			     b < line_break_id[lineno + 1]; b++) {
				struct bar_t *bar = &staff[b];
				char macro_str[MAX_MACRO_STRING_LEN];
				macro_str[0] = '\0';
				int has_volta_begin =
					0; // To process begin and end in the same bar
				for (int i = 0; i < bar->element_count; i++) {
					if (bar->elements[i].type ==
						    ELEMENT_MACRO &&
					    IS_UPPER_LONG_TERM_MACRO(
						    bar->elements[i]
							    .data.macro.type)) {
						enum macro_type_t macro_type =
							bar->elements[i]
								.data.macro.type;
						if (macro_type ==
						    MACRO_VOLTA_1) {
							strcpy(macro_str,
							       "\\multicolumn{1}{@{\\hspace{0.1em}}|l}{1.}");
							upper_long_term_macro =
								0;
						} else if (macro_type ==
							   MACRO_VOLTA_2) {
							strcpy(macro_str,
							       "\\multicolumn{1}{@{\\hspace{0.1em}}|l}{2.}");
							upper_long_term_macro =
								0;
						} else if (macro_type ==
							   MACRO_VOLTA_1_BEGIN) {
							strcpy(macro_str,
							       "\\multicolumn{1}{@{\\hspace{0.1em}}|l}{1.}");
							has_volta_begin =
								macro_type;
							upper_long_term_macro =
								macro_type;
						} else if (macro_type ==
							   MACRO_VOLTA_2_BEGIN) {
							strcpy(macro_str,
							       "\\multicolumn{1}{@{\\hspace{0.1em}}|l}{2.}");
							has_volta_begin =
								macro_type;
							upper_long_term_macro =
								macro_type;
						} else if (
							macro_type ==
								MACRO_VOLTA_1_END ||
							macro_type ==
								MACRO_VOLTA_2_END) {
							printf("%d\n",
							       has_volta_begin);
							if (has_volta_begin ==
							    MACRO_VOLTA_1_BEGIN) {
								strcpy(macro_str,
								       "\\multicolumn{1}{@{\\hspace{0.1em}}|l|@{\\hspace{0.1em}}}{1.}");
							} else if (
								has_volta_begin ==
								MACRO_VOLTA_2_BEGIN) {
								strcpy(macro_str,
								       "\\multicolumn{1}{@{\\hspace{0.1em}}|l|@{\\hspace{0.1em}}}{2.}");
							} else {
								strcpy(macro_str,
								       "\\multicolumn{1}{l|@{\\hspace{0.1em}}}{}");
							}
							upper_long_term_macro =
								0;
						}
					}
				}
				strcat(str, macro_str);
				if (b != line_break_id[lineno + 1] - 1) {
					strcat(str, " & ");
				}
			}
			strcat(str, "\\\\ \n");
		}
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
				strcat(str,
				       " \\multicolumn{1}{!{\\thickbar}|!{:}l|}{$ ");
			} else if (r_repeat_barline[b - line_break_id[lineno]]) {
				strcat(str,
				       " \\multicolumn{1}{l!{:}|!{\\thickbar}}{$ ");
			} else if (end_barline[b - line_break_id[lineno]]) {
				strcat(str,
				       " \\multicolumn{1}{l|!{\\thickbar}}{$ ");
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
					if (bar->elements[i].data.note.type ==
					    NOTE_DOT) {
						if (previous_note_duration ==
						    DUR_QUARTER)
							number_of_sixteenth_duration +=
								2;
						else if (previous_note_duration ==
							 DUR_EIGHTH)
							number_of_sixteenth_duration +=
								1;
					}
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

				if (number_of_sixteenth_duration > 0 &&
				    number_of_sixteenth_duration % 4 == 0) {
					strcat(str, " \\hspace{0.2em} ");
					number_of_sixteenth_duration = 0;
				}
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
		 char *filename, char *output_dir)
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
	strcat(command, "xelatex -output-directory=");
	strcat(command, output_dir);
	strcat(command, " ");
	strcat(command, filename);
	system(command);

	return 0;
}