/**
 * @file main.c
 * @author Prof. Dr. David Buzatto
 * @brief Simple script languague + puzzle game.
 * 
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "GameWindow.h"
#include "Lexer.h"
#include "Macros.h"
#include "Source.h"
#include "Token.h"
#include "include/Parser.h"

static void testSource( const char *file );
static void testLexer( const char *file );
static void testParser( const char *file );

int main( void ) {

    /*testSource( "testSource.txt" );
    testLexer( "testLexer.txt" );
    testParser( "testParser.txt" );*/

    GameWindow *gameWindow = createGameWindow(
        800,                 // width
        450,                 // height
        "Scriptable Puzzle", // title
        60,                  // target FPS
        true,                // antialiasing
        false,               // resizable
        false,               // full screen
        false,               // undecorated
        false,               // always on top
        false,               // invisible background
        false,               // always run
        false,               // load resources
        false                // init audio
    );

    initGameWindow( gameWindow );

    return 0;

}

static void testSource( const char *file ) {

    Source source;
    initSource( &source, file );

    do {
        trace( "%c (%d %d)", getCharSource( &source ), source.lineNumber, source.charNumber );
    } while ( advanceSource( &source ) );

    destroySource( &source );

}

static void testLexer( const char *file ) {

    Source source;
    initSource( &source, file );

    Lexer lexer;
    initLexer( &lexer, &source );

    Token t;
    do {
        t = getTokenLexer( &lexer );
        printToken( &t, 0 );
        advanceLexer( &lexer );
    } while ( t.type != TOKEN_TYPE_EOF );

    destroySource( &source );

}

static void testParser( const char *file ) {

    Source source;
    initSource( &source, file );

    Lexer lexer;
    initLexer( &lexer, &source );

    ASTNode *root = parseScript( &lexer );
    destroySource( &source );

    printAST( root );
    destroyAST( root );

}
