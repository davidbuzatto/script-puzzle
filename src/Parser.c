#include <stdlib.h>
#include <stdbool.h>

#include "Lexer.h"
#include "Macros.h"
#include "Token.h"
#include "include/Parser.h"

static ASTNode *createASTNode( ASTNodeType type, int childCapacity );
static void addASTChildNode( ASTNode *parent, ASTNode *child );
static void addASTChildToken( ASTNode *parent, Token token );
static void printASTHelper( ASTNode *node, int level, int indentation );

static bool isStatementToken( Token *t );
static bool isDirectionToken( Token *t );
static bool isIntegerToken( Token *t );

ASTNode *parseScript( Lexer *lexer ) {

    ASTNode *script = createASTNode( AST_NODE_TYPE_SCRIPT, 2 );
    addASTChildNode( script, parseStatements( lexer ) );

    Token t = getTokenLexer( lexer );
    if ( t.type != TOKEN_TYPE_EOF ) {
        trace( "syntax error (script) - expecting EOF, found:" );
        printToken( &t, 0 );
    }
    addASTChildToken( script, t );

    return script;

}

ASTNode *parseStatements( Lexer *lexer ) {
    
    ASTNode *statements = createASTNode( AST_NODE_TYPE_STATEMENTS, 40 );

    // required
    Token token = getTokenLexer( lexer );
    if ( !isStatementToken( &token ) ) {
        trace( "syntax error (statements)" );
        return statements;
    }

    do {
        addASTChildNode( statements, parseStatement( lexer ) );
        token = getTokenLexer( lexer );
    } while ( isStatementToken( &token ) );

    return statements;

}

ASTNode *parseStatement( Lexer *lexer ) {

    ASTNode *statement = createASTNode( AST_NODE_TYPE_STATEMENT, 1 );

    Token token = getTokenLexer( lexer );

    switch ( token.type ) {
        case TOKEN_TYPE_MOVE:
            addASTChildNode( statement, parseMove( lexer ) );
            break;
        case TOKEN_TYPE_TURN:
            addASTChildNode( statement, parseTurn( lexer ) );
            break;
        case TOKEN_TYPE_PICK:
            addASTChildNode( statement, parsePick( lexer ) );
            break;
        case TOKEN_TYPE_DROP:
            addASTChildNode( statement, parseDrop( lexer ) );
            break;
        default:
            trace( "syntax error (statement) -- probably unreachable" );
            break;
    }

    return statement;
    
}

ASTNode *parseMove( Lexer *lexer ) {

    ASTNode *move = createASTNode( AST_NODE_TYPE_MOVE, 1 );
    addASTChildToken( move, getTokenLexer( lexer ) );
    advanceLexer( lexer );

    Token t = getTokenLexer( lexer );
    advanceLexer( lexer );

    if ( isIntegerToken( &t ) ) {
        addASTChildToken( move, t );
    } else {
        trace( "syntax error (move) - expecting an integer, found:" );
        printToken( &t, 0 );
    }

    return move;
}

ASTNode *parseTurn( Lexer *lexer ) {

    ASTNode *turn = createASTNode( AST_NODE_TYPE_TURN, 1 );
    addASTChildToken( turn, getTokenLexer( lexer ) );
    advanceLexer( lexer );

    Token t = getTokenLexer( lexer );
    advanceLexer( lexer );

    if ( isDirectionToken( &t ) ) {
        addASTChildToken( turn, t );
    } else {
        trace( "syntax error (turn) - expecting a direction, found:" );
        printToken( &t, 0 );
    }

    return turn;

}

ASTNode *parsePick( Lexer *lexer ) {
    ASTNode *pick = createASTNode( AST_NODE_TYPE_PICK, 1 );
    addASTChildToken( pick, getTokenLexer( lexer ) );
    advanceLexer( lexer );
    return pick;
}

ASTNode *parseDrop( Lexer *lexer ) {
    ASTNode *drop = createASTNode( AST_NODE_TYPE_DROP, 1 );
    addASTChildToken( drop, getTokenLexer( lexer ) );
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

    switch ( node->type ) {
        case AST_NODE_TYPE_SCRIPT:
            trace( "%*s%s", level, "", "SCRIPT" );
            break;
        case AST_NODE_TYPE_STATEMENTS:
            trace( "%*s%s", level, "", "STATEMENTS" );
            break;
        case AST_NODE_TYPE_STATEMENT:
            trace( "%*s%s", level, "", "STATEMENT" );
            break;
        case AST_NODE_TYPE_MOVE:
            trace( "%*s%s", level, "", "MOVE" );
            break;
        case AST_NODE_TYPE_TURN:
            trace( "%*s%s", level, "", "TURN" );
            break;
        case AST_NODE_TYPE_PICK:
            trace( "%*s%s", level, "", "PICK" );
            break;
        case AST_NODE_TYPE_DROP:
            trace( "%*s%s", level, "", "DROP" );
            break;
        case AST_NODE_TYPE_TOKEN:
            printToken( &node->token, level );
            break;
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
