#include <stdlib.h>
#include <stdbool.h>

#include "raylib/raylib.h"

#include "Macros.h"
#include "Token.h"

static char tokenTypeTable[][100] = {

    [TOKEN_TYPE_IDENTIFIER] = "IDENTIFIER",

    [TOKEN_TYPE_MOVE] = "MOVE",
    [TOKEN_TYPE_TURN] = "TURN",
    [TOKEN_TYPE_PICK] = "PICK",
    [TOKEN_TYPE_DROP] = "DROP",

    [TOKEN_TYPE_LEFT] = "LEFT",
    [TOKEN_TYPE_RIGHT] = "RIGHT",

    [TOKEN_TYPE_ADD] = "ADD",
    [TOKEN_TYPE_SUB] = "SUB",
    [TOKEN_TYPE_MUL] = "MUL",
    [TOKEN_TYPE_DIV] = "DIV",
    [TOKEN_TYPE_MOD] = "MOD",

    [TOKEN_TYPE_LT] = "LT",
    [TOKEN_TYPE_LEQ] = "LEQ",
    [TOKEN_TYPE_GT] = "GT",
    [TOKEN_TYPE_GEQ] = "GEQ",
    [TOKEN_TYPE_EQ] = "EQ",
    [TOKEN_TYPE_NEQ] = "NEQ",
    [TOKEN_TYPE_NOT] = "NOT",

    [TOKEN_TYPE_LEFT_PAR] = "LEFT PAR",
    [TOKEN_TYPE_RIGHT_PAR] = "RIGHT PAR",
    [TOKEN_TYPE_COMMA] = "COMMA",
    [TOKEN_TYPE_SEMICOLON] = "SEMICOLON",

    [TOKEN_TYPE_INTEGER] = "INTEGER",

    [TOKEN_TYPE_UNKNOWN] = "UNKNOWN",
    [TOKEN_TYPE_EOF] = "EOF",

};

void printToken( Token *token, int indentation ) {

    if ( token->type == TOKEN_TYPE_EOF ) {
        trace( 
            "%*sTOKEN [%s] (%d, %d)",
            indentation,
            "",
            tokenTypeTable[token->type],
            token->lineNumber,
            token->charNumber
        );
    } else if ( token->value[0] == '\0' ) {
        trace(
            "%*sTOKEN [%s] (%d, %d)",
            indentation,
            "",
            tokenTypeTable[token->type],
            token->lineNumber,
            token->charNumber
        );
    } else {
        trace( 
            "%*sTOKEN [%s] -> %s (%d, %d)",
            indentation,
            "",
            tokenTypeTable[token->type],
            token->value,
            token->lineNumber,
            token->charNumber
        );
    }
}
