#pragma once

#include <stdlib.h>
#include <stdbool.h>

#include "Lexer.h"
#include "Token.h"

typedef enum ASTNodeType {
    AST_NODE_TYPE_SCRIPT,
    AST_NODE_TYPE_MOVE,
    AST_NODE_TYPE_TURN,
    AST_NODE_TYPE_PICK,
    AST_NODE_TYPE_DROP,
    AST_NODE_TYPE_TOKEN,
} ASTNodeType;

typedef struct ASTNode ASTNode;

struct ASTNode {
    ASTNodeType type;
    ASTNode **children;
    int childCount;
    int childCapacity;
    Token token;
};

ASTNode *parseScript( Lexer *lexer );
void parseStatements( Lexer *lexer, ASTNode *script );
ASTNode *parseStatement( Lexer *lexer );
ASTNode *parseMove( Lexer *lexer );
ASTNode *parseTurn( Lexer *lexer );
ASTNode *parsePick( Lexer *lexer );
ASTNode *parseDrop( Lexer *lexer );

void destroyAST( ASTNode *node );
void printAST( ASTNode *node );
const char *getASTNodeTypeName( ASTNodeType type );
