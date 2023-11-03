%{
#include "notation.h"
#include "parser.y.h"

void yyerror(char*);
int yywrap(void);
int yylex(void);

int lineno = 0;
%}

NewLine             \n
Whitespace          [ \t\r\v\f]

Title				title\:[a-zA-Z0-9\ ]*
Composer			composer\:[a-zA-Z0-9\ ]*
Instrument			instrument\:[a-zA-Z0-9\ ]*

Stress              \>
Staccato            v
Tremolo				t
Fermata				f
Note                [0-7]
Sharp               \#
Flat                b
Natural             n
OctaveHigher        '
OctaveLower         ,
Dot                 \.
Tie                 \-
Line				\/
PlaceHolder         \_

ChordBrackets		[\(\)]

Macro           	\[[a-zA-Z0-9\/\.\=\#]*\]

SetTypeKey			\{typekey:[b\#]?[A-G]\}
SetDisplayKey		\{displaykey:[b\#]?[A-G]\}
SetDisplaySharp		\{display_sharp\}
SetDisplayFlat		\{display_flat\}
SetSharp			\{sharp:[1-7]\}
SetFlat				\{flat:[1-7]\}
SetNatural			\{natural:[1-7]\}

Barline				\|

UnkonwnInput        .

%%

{Title}             { yylval.lval.str = strdup(yytext + 6); return TITLE; }
{Composer}          { yylval.lval.str = strdup(yytext + 9); return COMPOSER; }
{Instrument}        { yylval.lval.str = strdup(yytext + 11); return INSTRUMENT; }

{NewLine}           { lineno++; }
{Whitespace}        { /* ignore */ }

{Stress}            { return STRESS; }
{Staccato}          { return STACCATO; }
{Tremolo}         	{ return TREMOLO; }
{Fermata}         	{ return FERMATA; }
{Note}              { yylval.lval.note = atoi(yytext); return NOTE; }
{Sharp}             { return SHARP; }
{Flat}              { return FLAT; }
{Natural}           { return NATURAL; }
{OctaveHigher}      { return OCTAVE_HIGHER; }
{OctaveLower}       { return OCTAVE_LOWER; }
{Dot}               { return DOT; }
{Tie}               { return TIE; }
{Line}              { return LINE; }
{PlaceHolder}       { return PLACEHOLDER; }

{ChordBrackets}		{ return yytext[0]; }

{Macro}          	{ yylval.lval.macro = strdup(yytext); return MACRO; }

{SetTypeKey}		{
		yylval.lval.setting = strdup(yytext + 9);
		yylval.lval.setting[strlen(yylval.lval.setting) - 1] = '\0';
		return SET_TYPE_KEY;
	}
{SetDisplayKey}		{
		yylval.lval.setting = strdup(yytext + 12);
		yylval.lval.setting[strlen(yylval.lval.setting) - 1] = '\0';
		return SET_DISPLAY_KEY;
	}
{SetDisplaySharp}	{ return SET_DISPLAY_SHARP; }
{SetDisplayFlat}	{ return SET_DISPLAY_FLAT; }
{SetSharp}			{
		yylval.lval.setting = strdup(yytext + 7);
		yylval.lval.setting[strlen(yylval.lval.setting) - 1] = '\0';
		return SET_SHARP;
	}
{SetFlat}			{
		yylval.lval.setting = strdup(yytext + 6);
		yylval.lval.setting[strlen(yylval.lval.setting) - 1] = '\0';
		return SET_FLAT;
	}
{SetNatural}		{
		yylval.lval.setting = strdup(yytext + 9);
		yylval.lval.setting[strlen(yylval.lval.setting) - 1] = '\0';
		return SET_NATURAL;
	}

{Barline}           { return yytext[0]; }

{UnkonwnInput}      {
	char msg[32] = "Unkonwn Input! ";
	strcat(msg, yytext);
	yyerror(msg);
}
%%

int yywrap() {
    return 1;
}

void yyerror(char* msg) {
    printf("[Error]: at line %d:\n%s\n", lineno + 1, msg);
}
