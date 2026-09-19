#pragma once

#define SOURCE_CHAR_EOF '\0'

typedef struct Source {
    char *filePointer;
    char *current;
    int lineNumber;
    int charNumber;
    bool eofReached;
} Source;

void initSource( Source *source, const char *file );
void destroySource( Source *source );
bool advanceSource( Source *source );
char getCharSource( Source *source );
