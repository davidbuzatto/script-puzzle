#pragma once

#include "Token.h"
#include "Source.h"

typedef struct Lexer {
    Source *source;
    bool eofReached;
} Lexer;

void initLexer( Lexer *lexer, Source *source );
void destroyLexer( Lexer *lexer );
bool advanceLexer( Lexer *lexer );
Token getTokenLexer( Lexer *lexer );
