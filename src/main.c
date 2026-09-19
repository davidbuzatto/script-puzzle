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

int main( void ) {

    Source cs;
    initSource( &cs, "source.txt");

    Lexer ts;
    initLexer( &ts, &cs );

    Token t;

    do {
        t = getTokenLexer( &ts );
        printToken( &t );
        advanceLexer( &ts );
    } while ( t.type != TOKEN_TYPE_EOF );

    /*do {
        trace( "%c", getSource( &cs ) );
    } while ( advanceSource( &cs ) );*/

    destroySource( &cs );

    return 0;

}

/*int main( void ) {

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

}*/