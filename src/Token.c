#include <stdlib.h>
#include <stdbool.h>

#include "raylib/raylib.h"

#include "Macros.h"
#include "Token.h"

static char tokenTypeTable[][100] = {
    [TOKEN_TYPE_IDENTIFIER] = "IDENTIFIER",
    [TOKEN_TYPE_COMMAND] = "COMMAND",
    [TOKEN_TYPE_INTEGER] = "INTEGER",
    [TOKEN_TYPE_INVALID] = "INVALID",
    [TOKEN_TYPE_EOF] = "EOF",
};

void printToken( Token *token ) {
    if ( token->type == TOKEN_TYPE_EOF ) {
        trace( "TOKEN [%s] (%d, %d)", tokenTypeTable[token->type], token->lineNumber, token->charNumber );
    } else {
        trace( "TOKEN [%s] -> %s (%d, %d)", tokenTypeTable[token->type], token->value, token->lineNumber, token->charNumber );
    }
}
