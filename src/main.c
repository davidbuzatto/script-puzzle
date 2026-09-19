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

static void testSource( const char *file );
static void testLexer( const char *file );
static void testParser( const char *file );

int main( void ) {

    //testSource( "source.txt" );
    //testLexer( "source.txt" );
    //testParser( "source.txt" );

    GameWindow *gameWindow = createGameWindow(
        800,             // width
        450,             // height
        "Script Puzzle", // title
        60,              // target FPS
        true,            // antialiasing
        false,           // resizable
        false,           // full screen
        false,           // undecorated
        false,           // always on top
        false,           // invisible background
        false,           // always run
        false,           // load resources
        false            // init audio
    );

    initGameWindow( gameWindow );

    return 0;

}

static void testSource( const char *file ) {

    Source cs;
    initSource( &cs, file );

    do {
        trace( "%c", getCharSource( &cs ) );
    } while ( advanceSource( &cs ) );

    destroySource( &cs );

}

static void testLexer( const char *file ) {

    Source cs;
    initSource( &cs, file );

    Lexer ts;
    initLexer( &ts, &cs );

    Token t;
    do {
        t = getTokenLexer( &ts );
        printToken( &t );
        advanceLexer( &ts );
    } while ( t.type != TOKEN_TYPE_EOF );

    destroySource( &cs );

}

static void testParser( const char *file ) {

    Source cs;
    initSource( &cs, file );

    Lexer ts;
    initLexer( &ts, &cs );

    Token t;
    do {
        t = getTokenLexer( &ts );
        printToken( &t );
        advanceLexer( &ts );
    } while ( t.type != TOKEN_TYPE_EOF );

    destroySource( &cs );

}
