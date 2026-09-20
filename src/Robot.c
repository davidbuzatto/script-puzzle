#include <stdlib.h>
#include <math.h>

#include "raylib/raylib.h"

#include "GameWorld.h"
#include "Robot.h"

void inputRobot( Robot *r, GameWorld *gw ) {
    
    if ( r->state == ROBOT_STATE_IDLE ) {

        if ( IsKeyPressed( KEY_LEFT ) ) {
            r->state = ROBOT_STATE_TURNING_LEFT;
        } else if ( IsKeyPressed( KEY_RIGHT ) ) {
            r->state = ROBOT_STATE_TURNING_RIGHT;
        } else if ( IsKeyPressed( KEY_UP ) ) {

            int currentRow = (int) ( r->pos.y / r->walkDistance );
            int currentCol = (int) ( r->pos.x / r->walkDistance );

            int targetRow = currentRow;
            int targetCol = currentCol;

            int angle = (int) r->angle;
            switch ( angle ) {
                case 0:   targetCol++; break;
                case 90:  targetRow++; break;
                case 180: targetCol--; break;
                case 270: targetRow--; break;
            }

            // check if it can move
            if ( targetRow >= 0 && targetRow < gw->rows && targetCol >= 0 && targetCol < gw->cols ) {
                int targetPos = targetRow * gw->cols + targetCol;
                if ( gw->grid[targetPos] == ' ' ) {
                    r->state = ROBOT_STATE_MOVING;
                }
            }

        } else if ( IsKeyPressed( KEY_SPACE ) ) {

            int currentRow = (int) ( r->pos.y / r->walkDistance );
            int currentCol = (int) ( r->pos.x / r->walkDistance );

            int targetRow = currentRow;
            int targetCol = currentCol;

            int angle = (int) r->angle;
            switch ( angle ) {
                case 0:   targetCol++; break;
                case 90:  targetRow++; break;
                case 180: targetCol--; break;
                case 270: targetRow--; break;
            }

            // check if it can pick or drop an object
            if ( targetRow >= 0 && targetRow < gw->rows && targetCol >= 0 && targetCol < gw->cols ) {
                int targetPos = targetRow * gw->cols + targetCol;
                if ( !r->carryingObject && gw->grid[targetPos] == 'o' ) {
                    r->carryingObject = true;
                    gw->grid[targetPos] = ' ';
                } else if ( r->carryingObject ) {
                    if ( gw->grid[targetPos] == ' ' ) {
                        r->carryingObject = false;
                        gw->grid[targetPos] = 'o';
                    } else if ( gw->grid[targetPos] == 't' ) {
                        r->carryingObject = false;
                        gw->grid[targetPos] = 'c';
                        gw->remainingTargets--;
                    }
                }
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
            r->pos.x += (int) r->posOffset.x;
            r->pos.y += (int) r->posOffset.y;
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
