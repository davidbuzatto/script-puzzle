#pragma once

#include <stdbool.h>

#include "Parser.h"

typedef struct GameWorld GameWorld;

typedef struct Frame {
    ASTNode *node;
    int next;
} Frame;

typedef struct Interpreter {
    ASTNode *root;
    ASTNode *current;
    Frame stack[64];
    int top;
    int pendingMoveSteps;
} Interpreter;

void initInterpreter( Interpreter *in, ASTNode *root );
void destroyInterpreter( Interpreter *in );
bool stepInterpreter( Interpreter *in, GameWorld *gw );