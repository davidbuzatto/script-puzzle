#include <stdlib.h>
#include <stdbool.h>

#include "raylib/raylib.h"

#include "Scanner.h"

void initScanner( Scanner *scan, const char *file ) {
    char *source = LoadFileText( file );
    scan->filePointer = source;
    scan->current = source;
    scan->eofReached = false;
}

void destroyScanner( Scanner *scan ) {
    UnloadFileText( scan->filePointer );
}

bool advanceScanner( Scanner *scan ) {

    if ( scan->eofReached ) {
        return false;
    }

    if ( *scan->current == '\0' ) {
        scan->eofReached = true;
        return false;
    }

    scan->current++;
    if ( *scan->current == '\0' ) {
        scan->eofReached = true;
        return false;
    }

    return true;

}

char getCharScanner( Scanner *scan ) {

    if ( scan->eofReached ) {
        return CHAR_STREAM_EOF;
    }

    return *scan->current;

}
