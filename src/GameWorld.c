/**
 * @file GameWorld.c
 * @author Prof. Dr. David Buzatto
 * @brief GameWorld implementation.
 * 
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "raylib/raylib.h"
//#include "raylib/raymath.h"
//#define RAYGUI_IMPLEMENTATION    // to use raygui, comment these three lines.
//#include "raylib/raygui.h"       // other compilation units must only include
//#undef RAYGUI_IMPLEMENTATION     // raygui.h

#include "Robot.h"
#include "GameWorld.h"
#include "ResourceManager.h"

/**
 * @brief Creates a dinamically allocated GameWorld struct instance.
 */
GameWorld *createGameWorld( void ) {

    GameWorld *gw = (GameWorld*) malloc( sizeof( GameWorld ) );

    gw->size = 50;
    gw->rows = GetScreenHeight() / gw->size;
    gw->cols = GetScreenWidth() / gw->size;

    gw->robot = (Robot) {
        .pos = {
            gw->size / 2,
            GetScreenHeight() - gw->size / 2
        },
        .posOffset = { 0, 0 },
        .size = 40,
        .angle = 0,
        .angleOffset = 0,
        .walkDistance = gw->size,
        .animationCounter = 0,
        .animationTime = 0.5f,
        .carryingObject = false,
        .state = ROBOT_STATE_IDLE
    };

    gw->grid = (char*) calloc( gw->rows * gw->cols, sizeof( char ) );
    TextCopy( 
        gw->grid, 
        "      x        t"
        "  ooo x         "
        "      x t  xxx  "
        "   xxxx    xxx  "
        "   x       xxx  "
        "   x       xxx  "
        "           xxx  "
        "   x    xxxxxx  "
        "   x    xxxxxx t"
    );

    gw->remainingTargets = 0;
    for ( int row = 0; row < gw->rows; row++ ) {
        for ( int col = 0; col < gw->cols; col++ ) {
            int pos = row * gw->cols + col;
            if ( gw->grid[pos] == 't' ) {
                gw->remainingTargets++;
            }
        }
    }

    return gw;

}

/**
 * @brief Destroys a GameWindow object and its dependecies.
 */
void destroyGameWorld( GameWorld *gw ) {
    free( gw->grid );
    free( gw );
}

/**
 * @brief Reads user input and updates the state of the game.
 */
void updateGameWorld( GameWorld *gw, float delta ) {
    inputRobot( &gw->robot, gw );
    updateRobot( &gw->robot, delta );
}

/**
 * @brief Draws the state of the game.
 */
void drawGameWorld( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );

    for ( int row = 0; row < gw->rows; row++ ) {
        for ( int col = 0; col < gw->cols; col++ ) {
            char c = gw->grid[row * gw->cols + col];
            switch ( c ) {
                case 'x':
                    DrawRectangle( col * gw->size, row * gw->size, gw->size, gw->size, GRAY );
                    break;
                case 'o':
                    DrawCircle( col * gw->size + gw->size / 2, row * gw->size + gw->size / 2, gw->size / 2 * 0.8, GREEN );
                    break;
                case 't':
                    DrawCircle( col * gw->size + gw->size / 2, row * gw->size + gw->size / 2, gw->size / 2 * 0.8, ORANGE );
                    break;
                case 'c':
                    DrawCircle( col * gw->size + gw->size / 2, row * gw->size + gw->size / 2, gw->size / 2 * 0.8, BLUE );
                    break;
            }
        }
    }

    for ( int row = 0; row <= gw->rows; row++ ) {
        DrawLine( 0, gw->size * row, GetScreenWidth(), gw->size * row, BLACK );
    }

    for ( int col = 0; col <= gw->cols; col++ ) {
        DrawLine( gw->size * col, 0, gw->size * col, GetScreenHeight(), BLACK );
    }

    drawRobot( &gw->robot );

    DrawText( TextFormat( "Remaining targets: %d", gw->remainingTargets ), 10, 10, 20, BLACK );

    EndDrawing();

}
