#include <stdlib.h>
#include <stdbool.h>

#include "raylib/raylib.h"

#include "Source.h"

void initSource( Source *source, const char *file ) {
    source->filePointer = LoadFileText( file );
    source->current = source->filePointer;
    source->lineNumber = 1;
    source->charNumber = 1;
    source->eofReached = false;
}

void destroySource( Source *source ) {
    UnloadFileText( source->filePointer );
}

bool advanceSource( Source *source ) {

    if ( source->eofReached ) {
        return false;
    }

    if ( *source->current == '\0' ) {
        source->eofReached = true;
        return false;
    }

    source->current++;
    if ( *source->current == '\0' ) {
        source->eofReached = true;
        return false;
    }

    if ( *source->current == '\n' ) {
        source->lineNumber++;
        source->charNumber = 0;
    } else {
        source->charNumber++;
    }

    return true;

}

char getCharSource( Source *source ) {

    if ( source->eofReached ) {
        return SOURCE_CHAR_EOF;
    }

    return *source->current;

}
