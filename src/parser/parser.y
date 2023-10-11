%{
#include "notation.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void yyerror(char*);
int yywrap(void);
int yylex(void);

struct meta_t meta;
struct bar_t staff[MAX_BAR_PER_STAFF];
int bar_count = 0;
%}

%token TITLE COMPOSER INSTRUMENT
%token STRESS STACCATO TREMOLO FERMATA
%token NOTE
%token SHARP FLAT NATURAL
%token OCTAVE_HIGHER OCTAVE_LOWER
%token DOT TIE LINE PLACEHOLDER
%token MACRO
%token SET_TYPE_KEY SET_DISPLAY_KEY SET_DISPLAY_SHARP SET_DISPLAY_FLAT
%token SET_SHARP SET_FLAT SET_NATURAL

%%

Staff
    : Meta Bar {
        staff[bar_count] = $2.bar;
        bar_count ++;
    }
    | Staff Bar {
        staff[bar_count] = $2.bar;
        bar_count ++;
    }
    ;

Meta
    : TITLE COMPOSER INSTRUMENT {
        strcpy(meta.title, $1.lval.str);
        strcpy(meta.composer, $2.lval.str);
        strcpy(meta.instrument, $3.lval.str);
    }

Bar
    : ElementsInBar '|' {
        $$.bar = $1.bar;
    }
    ;

ElementsInBar
    : Element {
        $$.bar.elements = malloc(sizeof($1) * MAX_ELEMENT_PER_BAR);
        $$.bar.elements[0] = $1.element;
        $$.bar.element_count = 1;
    }
    | ElementsInBar Element {
        $$.bar = $1.bar;
        $$.bar.elements[$1.bar.element_count] = $2.element;
        $$.bar.element_count += 1;
    }
    ;

Element
    : Note              {
        $$.element.type = ELEMENT_NOTE;
        $$.element.data.note = $1.note;
    }
    | Chord {
        $$.element.type = ELEMENT_CHORD;
        $$.element.data.chord = $1.chord;
    }
    | Macro {
        $$.element.type = ELEMENT_MACRO;
        $$.element.data.macro = $1.macro;
    }
    | Setting {
        $$.element.type = ELEMENT_SETTING;
        $$.element.data.setting = $1.setting;
    }
    ;

Note
    : NoteWithDuration { 
        $$ = $1;
    }
    | DOT {
        $$ = $1;
        $$.note.type = NOTE_DOT;
    }
    | TIE {
        $$ = $1;
        $$.note.type = NOTE_TIE;
    }
    ;

Chord
    : ChordWithDuration { 
        $$ = $1;
    }

Macro
    : MACRO {
        char macro_text[128];
        // Remove the brackets
        strncpy(macro_text, $1.lval.macro + 1, strlen($1.lval.macro) - 2);
        macro_text[strlen($1.lval.macro) - 2] = '\0';
        if (str_to_macro(&$$.macro, macro_text) == 1) {
            yyerror("Invalid macro!");
        }
    }
    ;

Setting
    : SET_TYPE_KEY {
        $$.setting.type = SETTING_TYPE_KEY;
        strcpy($$.setting.key, $1.lval.setting);
    }
    | SET_DISPLAY_KEY {
        $$.setting.type = SETTING_DISPLAY_KEY;
        strcpy($$.setting.key, $1.lval.setting);
    }
    | SET_DISPLAY_SHARP {
        $$.setting.type = SETTING_DISPLAY_SHARP;
    }
    | SET_DISPLAY_FLAT {
        $$.setting.type = SETTING_DISPLAY_FLAT;
    }
    | SET_SHARP {
        $$.setting.type = SETTING_SHARP;
        $$.setting.note = atoi($1.lval.setting);
    }
    | SET_FLAT {
        $$.setting.type = SETTING_FLAT;
        $$.setting.note = atoi($1.lval.setting);
    }
    | SET_NATURAL {
        $$.setting.type = SETTING_NATURAL;
        $$.setting.note = atoi($1.lval.setting);
    }
    ;

PureNote
    : NOTE                { 
        if ($1.lval.note == 0) {
            $$.note.type = NOTE_REST;
            $$.note.articulation = ART_NONE;
            $$.note.accidental = ACC_NONE;
            $$.note.note = $1.lval.note;
            $$.note.octave = 0;
            $$.note.duration = DUR_QUARTER;
        } else {
            $$.note.type = NOTE_NOTE;
            $$.note.articulation = ART_NONE;
            $$.note.accidental = ACC_NONE;
            $$.note.note = $1.lval.note;
            $$.note.octave = 0;
            $$.note.duration = DUR_QUARTER;
        }
    }
    ;

PureNoteWithAccidental
    : PureNote               { 
        $$ = $1;
    }
    | SHARP PureNote          { 
        $$ = $2;
        $$.note.accidental = ACC_SHARP;
    }
    | FLAT PureNote          { 
        $$ = $2;
        $$.note.accidental = ACC_FLAT;
    }
    | NATURAL PureNote       { 
        $$ = $2;
        $$.note.accidental = ACC_NATURAL;
    }
    ;

PureNoteWithAccidentalAndOctave
    : PureNoteWithAccidental {
        $$ = $1;
    }
    | PureNoteWithAccidentalAndOctave OCTAVE_HIGHER { 
        $$ = $1;
        if ($1.note.octave < 0) {
            yyerror("Invalid octave!");
        }
        $$.note.octave = $1.note.octave + 1;
    }
    | PureNoteWithAccidentalAndOctave OCTAVE_LOWER  { 
        $$ = $1;
        if ($1.note.octave > 0) {
            yyerror("Invalid octave!");
        }
        $$.note.octave = $1.note.octave - 1;
    }
    ;

PureNoteWithAccidentalAndOctaveAndArticulation
    : PureNoteWithAccidentalAndOctave { 
        $$ = $1;
    }
    | STRESS PureNoteWithAccidentalAndOctaveAndArticulation { 
        $$ = $2;
        $$.note.articulation |= ART_STRESS;
    }
    | STACCATO PureNoteWithAccidentalAndOctaveAndArticulation { 
        $$ = $2;
        $$.note.articulation |= ART_STACCATO;
    }
    | TREMOLO PureNoteWithAccidentalAndOctaveAndArticulation { 
        $$ = $2;
        $$.note.articulation |= ART_TREMOLO;
    }
    | FERMATA PureNoteWithAccidentalAndOctaveAndArticulation { 
        $$ = $2;
        $$.note.articulation |= ART_FERMATA;
    }
    ;

NoteWithDuration
    : PureNoteWithAccidentalAndOctaveAndArticulation { 
        $$ = $1;
    }
    | PureNoteWithAccidentalAndOctaveAndArticulation LINE { 
        $$ = $1;
        $$.note.duration = DUR_EIGHTH;
    }
    | PureNoteWithAccidentalAndOctaveAndArticulation LINE LINE { 
        $$ = $1;
        $$.note.duration = DUR_SIXTEENTH;
    }
    ;

PureChord
    : '(' PureNoteWithAccidentalAndOctave PureNoteWithAccidentalAndOctave ')' {
        $$.chord.note_count = 2;
        $$.chord.notes[0] = $2.note;
        $$.chord.notes[1] = $3.note;
        $$.chord.articulation = ART_NONE;
        $$.chord.duration = DUR_QUARTER;
    }
    | '(' PureNoteWithAccidentalAndOctave PureNoteWithAccidentalAndOctave PureNoteWithAccidentalAndOctave ')' {
        $$.chord.note_count = 3;
        $$.chord.notes[0] = $2.note;
        $$.chord.notes[1] = $3.note;
        $$.chord.notes[2] = $4.note;
        $$.chord.articulation = ART_NONE;
        $$.chord.duration = DUR_QUARTER;
    }
    | '(' PureNoteWithAccidentalAndOctave PureNoteWithAccidentalAndOctave PureNoteWithAccidentalAndOctave PureNoteWithAccidentalAndOctave ')' {
        $$.chord.note_count = 4;
        $$.chord.notes[0] = $2.note;
        $$.chord.notes[1] = $3.note;
        $$.chord.notes[2] = $4.note;
        $$.chord.notes[3] = $5.note;
        $$.chord.articulation = ART_NONE;
        $$.chord.duration = DUR_QUARTER;
    }
    ;

PureChordWithArticulation
    : PureChord { 
        $$ = $1;
    }
    | STRESS PureChordWithArticulation { 
        $$ = $2;
        $$.chord.articulation |= ART_STRESS;
    }
    | STACCATO PureChordWithArticulation { 
        $$ = $2;
        $$.chord.articulation |= ART_STACCATO;
    }
    | TREMOLO PureChordWithArticulation { 
        $$ = $2;
        $$.chord.articulation |= ART_TREMOLO;
    }
    | FERMATA PureChordWithArticulation { 
        $$ = $2;
        $$.chord.articulation |= ART_FERMATA;
    }
    ;

ChordWithDuration
    : PureChordWithArticulation { 
        $$ = $1;
    }
    | PureChordWithArticulation LINE { 
        $$ = $1;
        $$.chord.duration = DUR_EIGHTH;
    }
    | PureChordWithArticulation LINE LINE { 
        $$ = $1;
        $$.chord.duration = DUR_SIXTEENTH;
    }
    ;

%%

int main(int argc, char *argv[]) {
    freopen(argv[1], "r", stdin);
    yyparse();
    parse_staff_by_settings(staff, bar_count);
    latex_render(&meta, staff, bar_count, argv[2]);
    return 0;
}