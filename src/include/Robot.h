#pragma once

#include <stdbool.h>

#include "raylib/raylib.h"

typedef struct GameWorld GameWorld;

typedef enum RobotState {
    ROBOT_STATE_IDLE,
    ROBOT_STATE_TURNING_LEFT,
    ROBOT_STATE_TURNING_RIGHT,
    ROBOT_STATE_MOVING,
} RobotState;

typedef struct Robot {

    Vector2 pos;
    Vector2 posOffset;
    float size;

    float angle;
    float angleOffset;

    float walkDistance;

    float animationTime;
    float animationCounter;

    bool carryingObject;
    RobotState state;

} Robot;

void inputRobot( Robot *r, GameWorld *gw );
void updateRobot( Robot *r, float delta );
void drawRobot( Robot *r );
void turnRobot( Robot *r );
