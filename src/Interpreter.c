#include <stdlib.h>
#include <stdbool.h>

#include "GameWorld.h"
#include "Interpreter.h"
#include "Macros.h"
#include "Robot.h"
#include "include/Parser.h"

void initInterpreter( Interpreter *in, ASTNode *root ) {
    in->root = root;
    in->current = NULL;
    in->stack[0] = (Frame) {
        .node = root,
        .next = 0
    };
    in->top = 1;
    in->pendingMoveSteps = 0;
}

void destroyInterpreter( Interpreter *in ) {
    destroyAST( in->root );
    in->root = NULL;
    in->top = 0;
}

bool stepInterpreter( Interpreter *in, GameWorld *gw ) {
    
    while ( in->top > 0 || in->pendingMoveSteps > 0 ) {

        if ( in->pendingMoveSteps > 0 ) {
            in->pendingMoveSteps--;
            bool moved = startMoveRobot( &gw->robot, gw );
            if ( !moved ) {
                in->pendingMoveSteps = 0;
            }
            return moved;
        }

        Frame *frame = &in->stack[in->top - 1];

        if ( frame->next >= frame->node->childCount ) {
            in->top--;
            continue;
        }

        ASTNode *stmt = frame->node->children[frame->next++];
        in->current = stmt;

        switch ( stmt->type ) {
            case AST_NODE_TYPE_MOVE:
                if ( stmt->childCount > 0 ) {
                    in->pendingMoveSteps = atoi( stmt->children[0]->token.value );
                }
                break;
            case AST_NODE_TYPE_TURN:
                if ( stmt->childCount > 0 ) {
                    startTurnRobot( &gw->robot, stmt->children[0]->token.type == TOKEN_TYPE_LEFT );
                    return true;
                }
                break;
            case AST_NODE_TYPE_PICK:
                return startPickRobot( &gw->robot, gw );
            case AST_NODE_TYPE_DROP:
                return startDropRobot( &gw->robot, gw );
            default:
                break;
        }

    }

    return false;

}
