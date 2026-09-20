#include <stdlib.h>
#include <stdbool.h>

#include "Lexer.h"
#include "Macros.h"
#include "Token.h"
#include "include/Parser.h"

static char nodeTypeTable[][100] = {
    [AST_NODE_TYPE_SCRIPT] = "SCRIPT",
    [AST_NODE_TYPE_MOVE] = "MOVE",
    [AST_NODE_TYPE_TURN] = "TURN",
    [AST_NODE_TYPE_PICK] = "PICK",
    [AST_NODE_TYPE_DROP] = "DROP",
    [AST_NODE_TYPE_TOKEN] = "TOKEN",
};

static ASTNode *createASTNode( ASTNodeType type, int childCapacity );
static void addASTChildNode( ASTNode *parent, ASTNode *child );
static void addASTChildToken( ASTNode *parent, Token token );
static void printASTHelper( ASTNode *node, int level, int indentation );

static bool isStatementToken( Token *t );
static bool isDirectionToken( Token *t );
static bool isIntegerToken( Token *t );

ASTNode *parseScript( Lexer *lexer ) {

    ASTNode *script = createASTNode( AST_NODE_TYPE_SCRIPT, 2 );
    parseStatements( lexer, script );

    Token t = getTokenLexer( lexer );
    if ( t.type != TOKEN_TYPE_EOF ) {
        trace( "syntax error (script) - expecting EOF, found:" );
        printToken( &t, 0 );
    }

    return script;

}

void parseStatements( Lexer *lexer, ASTNode *script ) {

    // required
    Token token = getTokenLexer( lexer );
    if ( !isStatementToken( &token ) ) {
        trace( "syntax error (statements)" );
        return;
    }

    do {
        addASTChildNode( script, parseStatement( lexer ) );
        token = getTokenLexer( lexer );
    } while ( isStatementToken( &token ) );

}

ASTNode *parseStatement( Lexer *lexer ) {

    Token token = getTokenLexer( lexer );

    switch ( token.type ) {
        case TOKEN_TYPE_MOVE:
            return parseMove( lexer );
        case TOKEN_TYPE_TURN:
            return parseTurn( lexer );
        case TOKEN_TYPE_PICK:
            return parsePick( lexer );
        case TOKEN_TYPE_DROP:
            return parseDrop( lexer );
        default:
            trace( "syntax error (statement) -- probably unreachable" );
            return NULL;
    }
    
}

ASTNode *parseMove( Lexer *lexer ) {

    ASTNode *move = createASTNode( AST_NODE_TYPE_MOVE, 1 );
    move->token = getTokenLexer( lexer );
    advanceLexer( lexer );

    Token t = getTokenLexer( lexer );

    if ( isIntegerToken( &t ) ) {
        addASTChildToken( move, t );
        advanceLexer( lexer );
    } else {
        trace( "syntax error (move) - expecting an integer, found:" );
        printToken( &t, 0 );
    }

    return move;

}

ASTNode *parseTurn( Lexer *lexer ) {

    ASTNode *turn = createASTNode( AST_NODE_TYPE_TURN, 1 );
    turn->token = getTokenLexer( lexer );
    advanceLexer( lexer );

    Token t = getTokenLexer( lexer );

    if ( isDirectionToken( &t ) ) {
        addASTChildToken( turn, t );
        advanceLexer( lexer );
    } else {
        trace( "syntax error (turn) - expecting a direction, found:" );
        printToken( &t, 0 );
    }

    return turn;

}

ASTNode *parsePick( Lexer *lexer ) {
    ASTNode *pick = createASTNode( AST_NODE_TYPE_PICK, 0 );
    pick->token = getTokenLexer( lexer );
    advanceLexer( lexer );
    return pick;
}

ASTNode *parseDrop( Lexer *lexer ) {
    ASTNode *drop = createASTNode( AST_NODE_TYPE_DROP, 0 );
    drop->token = getTokenLexer( lexer );
    advanceLexer( lexer );
    return drop;
}

void destroyAST( ASTNode *node ) {

    if ( node == NULL ) {
        return;
    }

    for ( int i = 0; i < node->childCount; i++ ) {
        destroyAST( node->children[i] );
    }

    free( node->children );
    free( node );

}

void printAST( ASTNode *node ) {
    printASTHelper( node, 0, 2 );
}

const char *getASTNodeTypeName( ASTNodeType type ) {
    return nodeTypeTable[type];
}

static ASTNode *createASTNode( ASTNodeType type, int childCapacity ) {

    ASTNode *newNode = malloc( sizeof( ASTNode ) );
    newNode->type = type;
    newNode->childCount = 0;
    newNode->childCapacity = childCapacity;

    if ( childCapacity == 0 ) {
        newNode->children = NULL;
    } else {
        newNode->children = malloc( childCapacity * sizeof( *newNode->children ) );
    }

    newNode->token = (Token) { 0 };
    
    return newNode;

}

static void addASTChildNode( ASTNode *parent, ASTNode *child ) {

    if ( parent->childCount == parent->childCapacity ) {
        int newCapacity = parent->childCapacity == 0 ? 1 : parent->childCapacity * 2;
        parent->children = realloc( parent->children, newCapacity * sizeof( *parent->children ) );
        parent->childCapacity = newCapacity;
    }

    parent->children[parent->childCount++] = child;

}

static void addASTChildToken( ASTNode *parent, Token token ) {

    ASTNode *tokenNode = createASTNode( AST_NODE_TYPE_TOKEN, 0 );
    tokenNode->token = token;

    addASTChildNode( parent, tokenNode );

}

static void printASTHelper( ASTNode *node, int level, int indentation ) {

    if ( node == NULL ) {
        trace( "%*s%s", level, "", "NULL" );
        return;
    }

    if ( node->type == AST_NODE_TYPE_TOKEN ) {
        printToken( &node->token, level );
    } else if ( node->type == AST_NODE_TYPE_SCRIPT ) {
        trace( "%*s%s", level, "", nodeTypeTable[node->type] );
    } else {
        trace( 
            "%*s%s (%d, %d)", level, "", 
            nodeTypeTable[node->type], 
            node->token.lineNumber, node->token.charNumber
        );
    }

    for ( int i = 0; i < node->childCount; i++ ) {
        printASTHelper( node->children[i], level + indentation, indentation );
    }

}

static bool isStatementToken( Token *t ) {
    return 
        t->type == TOKEN_TYPE_MOVE ||   
        t->type == TOKEN_TYPE_TURN ||   
        t->type == TOKEN_TYPE_PICK ||   
        t->type == TOKEN_TYPE_DROP;
}

static bool isDirectionToken( Token *t ) {
    return 
        t->type == TOKEN_TYPE_LEFT ||   
        t->type == TOKEN_TYPE_RIGHT;
}

static bool isIntegerToken( Token *t ) {
    return t->type == TOKEN_TYPE_INTEGER;
}
