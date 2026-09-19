#include <stdlib.h>
#include <stdbool.h>

#include "raylib/raylib.h"

#include "Scanner.h"
#include "Macros.h"
#include "Lexer.h"

static char valueBuffer[100];
static int bufferPos;

static Token currentToken;

static void scanIdentifier( Scanner *scan );
static void classifyCurrentToken( void );

void initLexer( Lexer *lexer, Scanner *scan ) {
    lexer->scan = scan;
    lexer->eofReached = false;
    advanceLexer( lexer );
}

void destroyLexer( Lexer *lexer ) {
}

bool advanceLexer( Lexer *lexer ) {

    Scanner *scan = lexer->scan;

    if ( scan->eofReached ) {
        lexer->eofReached = true;
        return false;
    }

    bufferPos = 0;

    char c = getCharScanner( scan );
    
    // discard spaces
    while ( c != CHAR_STREAM_EOF && ( c == ' ' || c == '\t' || c == '\n' ) ) {
        advanceScanner( scan );
        c = getCharScanner( scan );
    }

    if ( scan->eofReached ) {
        lexer->eofReached = true;
        return false;
    }

    TokenType type = TOKEN_TYPE_IDENTIFIER;

    switch ( c ) {
        default:
            scanIdentifier( scan );
            break;
    }

    currentToken.type = type;
    TextCopy( currentToken.value, valueBuffer );

    if ( type == TOKEN_TYPE_IDENTIFIER ) {
        classifyCurrentToken();
    }

    return true;

}

Token getTokenLexer( Lexer *lexer ) {

    if ( lexer->eofReached ) {
        return (Token) { .type = TOKEN_TYPE_EOF };
    }

    return currentToken;

}

static void scanIdentifier( Scanner *scan ) {

    char c = getCharScanner( scan );

    while ( c != CHAR_STREAM_EOF && c != ' ' && c != '\t' && c != '\n' ) {
        valueBuffer[bufferPos++] = c;
        advanceScanner( scan );
        c = getCharScanner( scan );
    }

    valueBuffer[bufferPos] = '\0';

}

static void classifyCurrentToken( void ) {

    char *tokenValue = currentToken.value;

    if ( TextIsEqual( tokenValue, "mover" ) ) {
        currentToken.type = TOKEN_TYPE_COMMAND;
    } else if ( TextIsEqual( tokenValue, "girar" ) ) {
        currentToken.type = TOKEN_TYPE_COMMAND;
    } else if ( TextIsEqual( tokenValue, "pegar" ) ) {
        currentToken.type = TOKEN_TYPE_COMMAND;
    } else if ( TextIsEqual( tokenValue, "soltar" ) ) {
        currentToken.type = TOKEN_TYPE_COMMAND;
    } else {

        // check if it is an integer or invalid
        int length = TextLength( currentToken.value );
        bool invalid = false;
        bool isFirstCharDigit = false;

        for ( int i = 0; i < length; i++ ) {
            char c = currentToken.value[i];
            if ( c < '0' || c > '9' ) {
                invalid = true;
                break;
            } else if ( i == 0 ) {
                isFirstCharDigit = true;
            }
        }

        if ( isFirstCharDigit ) {
            currentToken.type = invalid ? TOKEN_TYPE_INVALID : TOKEN_TYPE_INTEGER;
        }

    }

}
