#pragma once

#define CHAR_STREAM_EOF '\0'

typedef struct Scanner {
    char *filePointer;
    char *current;
    int lineNumber;
    int charNumber;
    bool eofReached;
} Scanner;

void initScanner( Scanner *scan, const char *file );
void destroyScanner( Scanner *scan );
bool advanceScanner( Scanner *scan );
char getCharScanner( Scanner *scan );
