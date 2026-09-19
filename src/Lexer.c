#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "raylib/raylib.h"

#include "Lexer.h"
#include "Macros.h"
#include "Source.h"

static char valueBuffer[100];
static int bufferPos;

static TokenType type;
static int lineNumber;
static int charNumber;

static void skipWhiteSpace( Source *source );
static void skipToEndOfLine( Lexer *lexer );
static void scanIdentifier( Source *source );
static void scanIntegerLiteral( Source *source );
static void classifyTokenType( char *tokenValue );

void initLexer( Lexer *lexer, Source *source ) {
    lexer->source = source;
    lexer->eofReached = false;
    advanceLexer( lexer );
}

void destroyLexer( Lexer *lexer ) {
}

bool advanceLexer( Lexer *lexer ) {

    Source *source = lexer->source;
    skipWhiteSpace( source );

    if ( source->eofReached ) {
        lexer->eofReached = true;
        return false;
    }

    lineNumber = source->lineNumber;
    charNumber = source->charNumber;

    char c = getCharSource( source );

    bufferPos = 0;
    valueBuffer[bufferPos] = '\0';

    if ( isalpha( c ) ) {
        type = TOKEN_TYPE_IDENTIFIER;
        scanIdentifier( source );
        classifyTokenType( valueBuffer );
        if ( type != TOKEN_TYPE_IDENTIFIER ) {
            bufferPos = 0;
            valueBuffer[bufferPos] = '\0';
        }
    } else if ( isdigit( c ) ) {
        type = TOKEN_TYPE_INTEGER;
        scanIntegerLiteral( source );
    } else {

        switch ( c ) {

            case '+':
                type = TOKEN_TYPE_ADD;
                advanceSource( source );
                break;

            case '-':
                type = TOKEN_TYPE_SUB;
                advanceSource( source );
                break;

            case '*':
                type = TOKEN_TYPE_MUL;
                advanceSource( source );
                break;

            case '/':
                advanceSource( source );
                if ( getCharSource( source ) == '/' ) {
                    skipToEndOfLine( lexer );
                    advanceSource( source );
                    advanceLexer( lexer );
                } else {
                    type = TOKEN_TYPE_DIV;
                }
                break;
            
            case '%':
                type = TOKEN_TYPE_MOD;
                advanceSource( source );
                break;
            
            case '<':
                advanceSource( source );
                if ( getCharSource( source ) == '=' ) {
                    type = TOKEN_TYPE_LEQ;
                    advanceSource( source );
                } else {
                    type = TOKEN_TYPE_LT;
                }
                break;

            case '>':
                advanceSource( source );
                if ( getCharSource( source ) == '=' ) {
                    type = TOKEN_TYPE_GEQ;
                    advanceSource( source );
                } else {
                    type = TOKEN_TYPE_GT;
                }
                break;

            case '=':
                advanceSource( source );
                if ( getCharSource( source ) == '=' ) {
                    type = TOKEN_TYPE_EQ;
                    advanceSource( source );
                } else {
                    trace( "Invalid character '%c'", c );
                    advanceSource( source );
                }
                break;

            case '!':
                advanceSource( source );
                if ( getCharSource( source ) == '=' ) {
                    type = TOKEN_TYPE_NEQ;
                    advanceSource( source );
                } else {
                    type = TOKEN_TYPE_NOT;
                }
                break;
            
            case '(':
                type = TOKEN_TYPE_LEFT_PAR;
                advanceSource( source );
                break;

            case ')':
                type = TOKEN_TYPE_RIGHT_PAR;
                advanceSource( source );
                break;

            case ',':
                type = TOKEN_TYPE_COMMA;
                advanceSource( source );
                break;

            case ';':
                type = TOKEN_TYPE_SEMICOLON;
                advanceSource( source );
                break;

            default:
                trace( "Invalid character '%c'", c );
                advanceSource( source );
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
            .lineNumber = lexer->source->lineNumber,
            .charNumber = lexer->source->charNumber + 1
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

static void skipWhiteSpace( Source *source ) {

    char c = getCharSource( source );
    
    // discard spaces
    while ( c != SOURCE_CHAR_EOF && ( c == ' ' || c == '\t' || c == '\n' ) ) {
        advanceSource( source );
        c = getCharSource( source );
    }

}

static void skipToEndOfLine( Lexer *lexer ) {

    Source *source = lexer->source;

    while ( getCharSource( source ) != '\n' ) {
        advanceSource( source );
        if ( source->eofReached ) {
            lexer->eofReached = true;
            break;
        }
    }

}

static void scanIdentifier( Source *source ) {

    char c = getCharSource( source );
    bufferPos = 0;

    while ( c != SOURCE_CHAR_EOF && c != ' ' && c != '\t' && c != '\n' ) {
        valueBuffer[bufferPos++] = c;
        advanceSource( source );
        c = getCharSource( source );
    }

    valueBuffer[bufferPos] = '\0';

}

static void scanIntegerLiteral( Source *source ) {

    char c = getCharSource( source );
    bufferPos = 0;

    do {
        valueBuffer[bufferPos++] = c;
        advanceSource( source );
        c = getCharSource( source );
    } while ( isdigit( c ) );

    valueBuffer[bufferPos] = '\0';

}

static void classifyTokenType( char *tokenValue ) {

    if ( TextIsEqual( tokenValue, "move" ) ) {
        type = TOKEN_TYPE_MOVE;
    } else if ( TextIsEqual( tokenValue, "turn" ) ) {
        type = TOKEN_TYPE_TURN;
    } else if ( TextIsEqual( tokenValue, "pick" ) ) {
        type = TOKEN_TYPE_PICK;
    } else if ( TextIsEqual( tokenValue, "drop" ) ) {
        type = TOKEN_TYPE_DROP;
    } else {
        type = TOKEN_TYPE_IDENTIFIER;
    }

}
