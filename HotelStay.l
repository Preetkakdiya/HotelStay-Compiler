%{
#include "HotelStay.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s);
%}

%option noyywrap

%%

[ \t\r]+                { /* Ignore whitespace */ }
\n                      { /* Ignore newline or track lines */ }
"//".*                  { /* Ignore single-line comments */ }
"/*"([^*]|\*+[^*/])*\*+"/" { /* Ignore multi-line comments */ }

"GUEST"                 { return GUEST; }
"ROOM"                  { return ROOM; }
"NIGHTS"                { return NIGHTS; }
"AMENITY"               { return AMENITY; }
"DISCOUNT"              { return DISCOUNT; }
"TAX"                   { return TAX; }
"CHECKOUT"              { return CHECKOUT; }
"STATUS"                { return STATUS; }
"PRINT"                 { return PRINT; }
"RESET"                 { return RESET; }

"STANDARD"              { return STANDARD; }
"DELUXE"                { return DELUXE; }
"SUITE"                 { return SUITE; }
"PRESIDENTIAL"          { return PRESIDENTIAL; }

[0-9]+                  {
                            yylval.num = atoi(yytext);
                            return NUMBER;
                        }

\"[^\"]*\"              {
                            int len = strlen(yytext);
                            char *str = (char *)malloc(len - 1);
                            strncpy(str, yytext + 1, len - 2);
                            str[len - 2] = '\0';
                            yylval.str = str;
                            return STRING;
                        }

[a-zA-Z_][a-zA-Z0-9_]*  {
                            yylval.str = strdup(yytext);
                            return ID;
                        }

";"                     { return ';'; }
"="                     { return '='; }
"+"                     { return '+'; }
"-"                     { return '-'; }
"*"                     { return '*'; }
"/"                     { return '/'; }
"("                     { return '('; }
")"                     { return ')'; }

.                       {
                            printf("[Lexical Error] Unrecognized character: '%s'\n", yytext);
                        }

%%
