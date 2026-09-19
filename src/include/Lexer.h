#pragma once

#include "Token.h"
#include "Scanner.h"

typedef struct Lexer {
    Scanner *scan;
    bool eofReached;
} Lexer;

void initLexer( Lexer *lexer, Scanner *scan );
void destroyLexer( Lexer *lexer );
bool advanceLexer( Lexer *lexer );
Token getTokenLexer( Lexer *lexer );
