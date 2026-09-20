#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "raylib/raylib.h"

#include "GameWorld.h"
#include "Macros.h"
#include "Robot.h"

static bool getTargetCell( Robot *r, GameWorld *gw, int *targetPos );

void inputRobot( Robot *r, GameWorld *gw ) {
    
    if ( r->state == ROBOT_STATE_IDLE ) {

        if ( IsKeyPressed( KEY_LEFT ) ) {
            startTurnRobot( r, true );
        } else if ( IsKeyPressed( KEY_RIGHT ) ) {
            startTurnRobot( r, false );
        } else if ( IsKeyPressed( KEY_UP ) ) {
            startMoveRobot( r, gw );
        } else if ( IsKeyPressed( KEY_SPACE ) ) {
            if ( !startPickRobot( r, gw ) ) {
                startDropRobot( r, gw );
            }
        }

    }

}

void updateRobot( Robot *r, float delta ) {
    
    if ( r->state == ROBOT_STATE_TURNING_LEFT || r->state == ROBOT_STATE_TURNING_RIGHT ) {

        r->animationCounter += delta;
        r->angleOffset = ( r->state == ROBOT_STATE_TURNING_LEFT ? -90 : 90 ) * r->animationCounter / r->animationTime;

        if ( r->animationCounter >= r->animationTime ) {
            r->animationCounter = 0;
            if ( r->state == ROBOT_STATE_TURNING_LEFT ) {
                int newAngle = (int) r->angle - 90;
                if ( newAngle < 0 ) {
                    r->angle = 270;
                } else {
                    r->angle = ( (int) r->angle - 90 ) % 360;
                }
            } else {
                r->angle = ( (int) r->angle + 90 ) % 360;
            }
            r->angleOffset = 0;
            r->state = ROBOT_STATE_IDLE;
        }
        
    } else if ( r->state == ROBOT_STATE_MOVING ) {
        
        r->animationCounter += delta;

        float xd = r->walkDistance * cosf( DEG2RAD * r->angle );
        float yd = r->walkDistance * sinf( DEG2RAD * r->angle );

        r->posOffset.x = xd * r->animationCounter / r->animationTime;
        r->posOffset.y = yd * r->animationCounter / r->animationTime;

        if ( r->animationCounter >= r->animationTime ) {
            r->animationCounter = 0;
            r->pos.x += roundf( xd );
            r->pos.y += roundf( yd );
            r->posOffset = (Vector2) { 0, 0 };
            r->state = ROBOT_STATE_IDLE;
        }
        
    }

}

void drawRobot( Robot *r ) {
    float half = r->size / 2;
    Rectangle rect = {
        r->pos.x + r->posOffset.x, 
        r->pos.y + r->posOffset.y, 
        r->size, 
        r->size
    };
    DrawRectanglePro( rect, (Vector2) { half, half }, r->angle + r->angleOffset, BLUE );
    DrawCircle( 
        r->pos.x + r->posOffset.x + half * cosf( DEG2RAD * ( r->angle + r->angleOffset ) ), 
        r->pos.y + r->posOffset.y + half * sinf( DEG2RAD * ( r->angle + r->angleOffset ) ), 
        5,
        r->carryingObject ? GREEN : BLACK
    );
}

bool startMoveRobot( Robot *r, GameWorld *gw ) {
    
    int targetPos;

    if ( getTargetCell( r, gw, &targetPos ) && gw->grid[targetPos] == ' ' ) {
        r->state = ROBOT_STATE_MOVING;
        return true;
    }

    return false;

}

void startTurnRobot( Robot *r, bool turnLeft ) {
    if ( turnLeft ) {
        r->state = ROBOT_STATE_TURNING_LEFT;
    } else {
        r->state = ROBOT_STATE_TURNING_RIGHT;
    }
}

bool startPickRobot( Robot *r, GameWorld *gw ) {
    
    int targetPos;
            
    if ( getTargetCell( r, gw, &targetPos ) ) {
        if ( !r->carryingObject && gw->grid[targetPos] == 'o' ) {
            r->carryingObject = true;
            gw->grid[targetPos] = ' ';
            return true;
        }
        return false;
    }

    return false;

}

bool startDropRobot( Robot *r, GameWorld *gw ) {
    
    int targetPos;
            
    if ( getTargetCell( r, gw, &targetPos ) ) {
        if ( r->carryingObject ) {
            if ( gw->grid[targetPos] == ' ' ) {
                r->carryingObject = false;
                gw->grid[targetPos] = 'o';
                return true;
            } else if ( gw->grid[targetPos] == 't' ) {
                r->carryingObject = false;
                gw->grid[targetPos] = 'c';
                gw->remainingTargets--;
                return true;
            }
        }
        return false;
    }

    return false;

}

static bool getTargetCell( Robot *r, GameWorld *gw, int *targetPos ) {

    int row = (int) ( r->pos.y / r->walkDistance );
    int col = (int) ( r->pos.x / r->walkDistance );

    switch ( (int) r->angle ) {
        case 0:   col++; break;
        case 90:  row++; break;
        case 180: col--; break;
        case 270: row--; break;
    }

    if ( row < 0 || row >= gw->rows || col < 0 || col >= gw->cols ) {
        return false;
    }

    *targetPos = row * gw->cols + col;
    return true;

}
