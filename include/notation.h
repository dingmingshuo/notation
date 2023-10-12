#ifndef __NOTATION__
#define __NOTATION__

#define MAX_ELEMENT_PER_BAR 64
#define MAX_BAR_PER_STAFF 512
#define MAX_NOTE_PER_CHORD 4
#define MAX_META_STRING_LENGTH 256
#define MAX_SETTING_STRING_LENGTH 16

enum element_type_t {
	ELEMENT_NOTE,
	ELEMENT_CHORD,
	ELEMENT_MACRO,
	ELEMENT_SETTING
};

enum note_type_t { NOTE_REST, NOTE_NOTE, NOTE_DOT, NOTE_TIE };

enum macro_type_t {
	// Lower macros (1~99)
	MACRO_F = 1, // forte
	MACRO_MF = 2, // mezzo forte
	MACRO_FF = 3, // fortissimo
	MACRO_P = 4, // piano
	MACRO_MP = 5, // mezzo piano
	MACRO_PP = 6, // pianissimo
	MACRO_SFZ = 7, // sforzando
	MACRO_CRESC = 8, // crescendo
	MACRO_RIT = 9, // ritardando
	MACRO_ACCEL = 10, // accelerando
	MACRO_SFP = 11, // sforzando piano
	MACRO_SF = 12, // sforzato
	MACRO_FP = 13, // forzando piano
	// Upper macros (101~199)
	MACRO_DIV = 101, // divisi
	MACRO_UNIS = 102, // unisono
	MACRO_LARGO = 103, // largo
	MACRO_LARGHETTO = 104, // larghetto
	MACRO_ADAGIO = 105, // adagio
	MACRO_ADAGIETTO = 106, // adagietto
	MACRO_ANDANTE = 107, // andante
	MACRO_ANDANTINO = 108, // andantino
	MACRO_MODERATO = 109, // moderato
	MACRO_ALLEGRO = 110, // allegro
	MACRO_VIVACE = 111, // vivace
	MACRO_PRESTO = 112, // presto
	MACRO_PRESTISSIMO = 113, // prestissimo
	MACRO_LARGAMENTE = 114, // largamente
	MACRO_SOLO = 115, // solo
	MACRO_TUTTI = 116, // tutti
	// Inner macros (201~299)
	MACRO_4_4 = 201, // 4/4
	MACRO_3_4 = 202, // 3/4
	MACRO_2_4 = 203, // 2/4
	MACRO_ARPEGGIO = 204, // arpeggio
	// Invisible macros (301~399)
	MACRO_NEW_LINE = 301,
	// Key macros (401~499)
	MACRO_C_KEY = 401, // C key
	MACRO_G_KEY = 402, // G key
	MACRO_D_KEY = 403, // D key
	MACRO_A_KEY = 404, // A key
	MACRO_E_KEY = 405, // E key
	MACRO_B_KEY = 406, // B key
	MACRO_F_SHARP_KEY = 407, // F# key
	MACRO_C_SHARP_KEY = 408, // C# key
	MACRO_F_KEY = 409, // F key
	MACRO_B_FLAT_KEY = 410, // Bb key
	MACRO_E_FLAT_KEY = 411, // Eb key
	MACRO_A_FLAT_KEY = 412, // Ab key
	MACRO_D_FLAT_KEY = 413, // Db key
	MACRO_G_FLAT_KEY = 414, // Gb key
	MACRO_C_FLAT_KEY = 415, // Cb key
	// Barline macros (501~599)
	MACRO_L_REPEAT = 501, // left repeat sign
	MACRO_R_REPEAT = 502, // right repeat sign
	MACRO_END = 503, // end sign
	// Upper longterm macros (601~699)
	MACRO_VOLTA_1 = 601, // volta 1
	MACRO_VOLTA_1_BEGIN = 602, // volta 1 begin
	MACRO_VOLTA_1_END = 603, // volta 1 end
	MACRO_VOLTA_2 = 604, // volta 2
	MACRO_VOLTA_2_BEGIN = 605, // volta 2 begin
	MACRO_VOLTA_2_END = 606, // volta 2 end
	// Lower longterm macros (701~799)
	MACRO_CRESC_BEGIN = 701, // crescendo begin
	MACRO_CRESC_END = 702, // crescendo end
	MACRO_DIM_BEGIN = 703, // diminuendo begin
	MACRO_DIM_END = 704 // diminuendo end
};

#define IS_LOWER_MACRO(macro) ((macro) < 100)
#define IS_UPPER_MACRO(macro) (100 < (macro) && (macro) < 200)
#define IS_INNER_MACRO(macro) (200 < (macro) && (macro) < 300)
#define IS_INVISIBLE_MACRO(macro) (300 < (macro) && (macro) < 400)
#define IS_KEY_MACRO(macro) (400 < (macro) && (macro) < 500)
#define IS_BARLINE_MACRO(macro) (500 < (macro) && (macro) < 600)
#define IS_UPPER_LONG_TERM_MACRO(macro) (600 < (macro) && (macro) < 700)
#define IS_LOWER_LONG_TERM_MACRO(macro) (700 < (macro) && (macro) < 800)

enum setting_type_t {
	SETTING_TYPE_KEY,
	SETTING_DISPLAY_KEY,
	SETTING_DISPLAY_SHARP,
	SETTING_DISPLAY_FLAT,
	SETTING_SHARP,
	SETTING_FLAT,
	SETTING_NATURAL
};

enum articulation_t {
	ART_NONE,
	ART_STRESS = 1,
	ART_STACCATO = 2,
	ART_TREMOLO = 4,
	ART_FERMATA = 8
};

enum accidental_t { ACC_NONE, ACC_SHARP, ACC_FLAT, ACC_NATURAL };

enum duration_t { DUR_QUARTER, DUR_EIGHTH, DUR_SIXTEENTH };

struct meta_t {
	char title[MAX_META_STRING_LENGTH];
	char composer[MAX_META_STRING_LENGTH];
	char instrument[MAX_META_STRING_LENGTH];
};

struct note_t {
	enum note_type_t type;
	enum articulation_t articulation; // is a bitfield
	enum accidental_t accidental;
	int note;
	int octave;
	enum duration_t duration;
};

struct chord_t {
	int note_count;
	enum articulation_t articulation; // is a bitfield
	struct note_t notes[MAX_NOTE_PER_CHORD]; // 0 is higher
	enum duration_t duration;
};

struct macro_t {
	enum macro_type_t type;
};

struct setting_t {
	enum setting_type_t type;
	char key[MAX_SETTING_STRING_LENGTH];
	int note;
};

struct element_t {
	enum element_type_t type;
	union {
		struct note_t note;
		struct chord_t chord;
		struct macro_t macro;
		struct setting_t setting;
	} data;
};

struct bar_t {
	int element_count;
	struct element_t *elements;
};

int latex_render(struct meta_t *meta, struct bar_t *bar_t, int bar_count,
		 char *filename);
int macro_to_str(char *str, struct macro_t *macro);
int str_to_macro(struct macro_t *macro, char *str);
int parse_staff_by_settings(struct bar_t *staff, int bar_count);

typedef union {
	int note;
	char *macro;
	char *setting;
	char *str;
} lval_t;

typedef union {
	struct note_t note;
	struct chord_t chord;
	struct macro_t macro;
	struct setting_t setting;
	struct element_t element;
	struct bar_t bar;
	lval_t lval;
} yylval_t;

#define YYSTYPE yylval_t

#endif