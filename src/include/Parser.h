#pragma once

#include <stdlib.h>
#include <stdbool.h>

#include "Lexer.h"
#include "Token.h"

typedef enum ASTNodeType {
    AST_NODE_TYPE_SCRIPT,
    AST_NODE_TYPE_STATEMENTS,
    AST_NODE_TYPE_STATEMENT,
    AST_NODE_TYPE_MOVE,
    AST_NODE_TYPE_TURN,
    AST_NODE_TYPE_PICK,
    AST_NODE_TYPE_DROP,
} ASTNodeType;

typedef struct ASTNode {
    ASTNodeType type;
    Token tokens[100];
    int tokensCount;
} ASTNode;

ASTNode parseScript( Lexer *lexer );
ASTNode parseStatements( Lexer *lexer );
ASTNode parseStatement( Lexer *lexer );
ASTNode parseMove( Lexer *lexer );
ASTNode parseTurn( Lexer *lexer );
ASTNode parsePick( Lexer *lexer );
ASTNode parseDrop( Lexer *lexer );

void printAST( ASTNode *root );
