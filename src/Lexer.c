#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "raylib/raylib.h"

#include "Scanner.h"
#include "Macros.h"
#include "Lexer.h"

static char valueBuffer[100];
static int bufferPos;

static TokenType type;
static int lineNumber;
static int charNumber;

static void skipWhiteSpace( Scanner *scan );
static void skipToEndOfLine( Lexer *lexer );
static void scanIdentifier( Scanner *scan );
static void scanIntegerLiteral( Scanner *scan );
static void classifyTokenType( char *tokenValue );

void initLexer( Lexer *lexer, Scanner *scan ) {
    lexer->scan = scan;
    lexer->eofReached = false;
    advanceLexer( lexer );
}

void destroyLexer( Lexer *lexer ) {
}

bool advanceLexer( Lexer *lexer ) {

    Scanner *scan = lexer->scan;
    skipWhiteSpace( scan );

    if ( scan->eofReached ) {
        lexer->eofReached = true;
        return false;
    }

    lineNumber = scan->lineNumber;
    charNumber = scan->charNumber;

    char c = getCharScanner( scan );

    bufferPos = 0;
    valueBuffer[bufferPos] = '\0';

    if ( isalpha( c ) ) {
        type = TOKEN_TYPE_IDENTIFIER;
        scanIdentifier( scan );
        classifyTokenType( valueBuffer );
        if ( type != TOKEN_TYPE_IDENTIFIER ) {
            bufferPos = 0;
            valueBuffer[bufferPos] = '\0';
        }
    } else if ( isdigit( c ) ) {
        type = TOKEN_TYPE_INTEGER;
        scanIntegerLiteral( scan );
    } else {

        switch ( c ) {

            case '+':
                type = TOKEN_TYPE_ADD;
                advanceScanner( scan );
                break;

            case '-':
                type = TOKEN_TYPE_SUB;
                advanceScanner( scan );
                break;

            case '*':
                type = TOKEN_TYPE_MUL;
                advanceScanner( scan );
                break;

            case '/':
                advanceScanner( scan );
                if ( getCharScanner( scan ) == '/' ) {
                    skipToEndOfLine( lexer );
                    advanceScanner( scan );
                    advanceLexer( lexer );
                } else {
                    type = TOKEN_TYPE_DIV;
                }
                break;
            
            case '%':
                type = TOKEN_TYPE_MOD;
                advanceScanner( scan );
                break;
            
            case '<':
                advanceScanner( scan );
                if ( getCharScanner( scan ) == '=' ) {
                    type = TOKEN_TYPE_LEQ;
                    advanceScanner( scan );
                } else {
                    type = TOKEN_TYPE_LT;
                }
                break;

            case '>':
                advanceScanner( scan );
                if ( getCharScanner( scan ) == '=' ) {
                    type = TOKEN_TYPE_GEQ;
                    advanceScanner( scan );
                } else {
                    type = TOKEN_TYPE_GT;
                }
                break;

            case '=':
                advanceScanner( scan );
                if ( getCharScanner( scan ) == '=' ) {
                    type = TOKEN_TYPE_EQ;
                    advanceScanner( scan );
                } else {
                    trace( "Invalid character '%c'", c );
                    advanceScanner( scan );
                }
                break;

            case '!':
                advanceScanner( scan );
                if ( getCharScanner( scan ) == '=' ) {
                    type = TOKEN_TYPE_NEQ;
                    advanceScanner( scan );
                } else {
                    type = TOKEN_TYPE_NOT;
                }
                break;
            
            case '(':
                type = TOKEN_TYPE_LEFT_PAR;
                advanceScanner( scan );
                break;

            case ')':
                type = TOKEN_TYPE_RIGHT_PAR;
                advanceScanner( scan );
                break;

            case ',':
                type = TOKEN_TYPE_COMMA;
                advanceScanner( scan );
                break;

            default:
                trace( "Invalid character '%c'", c );
                advanceScanner( scan );
                break;

        }

    }

    return true;

}

Token getTokenLexer( Lexer *lexer ) {

    if ( lexer->eofReached ) {
        return (Token) {
            .type = TOKEN_TYPE_EOF,
            .value = "",
            .lineNumber = lexer->scan->lineNumber,
            .charNumber = lexer->scan->charNumber + 1
         };
    }

    Token token = {
        .type = type,
        .lineNumber = lineNumber,
        .charNumber = charNumber
    };

    TextCopy( token.value, valueBuffer );

    return token;

}

static void skipWhiteSpace( Scanner *scan ) {

    char c = getCharScanner( scan );
    
    // discard spaces
    while ( c != CHAR_STREAM_EOF && ( c == ' ' || c == '\t' || c == '\n' ) ) {
        advanceScanner( scan );
        c = getCharScanner( scan );
    }

}

static void skipToEndOfLine( Lexer *lexer ) {

    Scanner *scan = lexer->scan;

    while ( getCharScanner( scan ) != '\n' ) {
        advanceScanner( scan );
        if ( scan->eofReached ) {
            lexer->eofReached = true;
            break;
        }
    }

}

static void scanIdentifier( Scanner *scan ) {

    char c = getCharScanner( scan );
    bufferPos = 0;

    while ( c != CHAR_STREAM_EOF && c != ' ' && c != '\t' && c != '\n' ) {
        valueBuffer[bufferPos++] = c;
        advanceScanner( scan );
        c = getCharScanner( scan );
    }

    valueBuffer[bufferPos] = '\0';

}

static void scanIntegerLiteral( Scanner *scan ) {

    char c = getCharScanner( scan );
    bufferPos = 0;

    do {
        valueBuffer[bufferPos++] = c;
        advanceScanner( scan );
        c = getCharScanner( scan );
    } while ( isdigit( c ) );

    valueBuffer[bufferPos] = '\0';

}

static void classifyTokenType( char *tokenValue ) {

    if ( TextIsEqual( tokenValue, "mover" ) ) {
        type = TOKEN_TYPE_MOVER;
    } else if ( TextIsEqual( tokenValue, "girar" ) ) {
        type = TOKEN_TYPE_GIRAR;
    } else if ( TextIsEqual( tokenValue, "pegar" ) ) {
        type = TOKEN_TYPE_PEGAR;
    } else if ( TextIsEqual( tokenValue, "soltar" ) ) {
        type = TOKEN_TYPE_SOLTAR;
    } else {
        type = TOKEN_TYPE_IDENTIFIER;
    }

}
