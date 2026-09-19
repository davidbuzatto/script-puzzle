#pragma once

typedef enum TokenType {
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_COMMAND,
    TOKEN_TYPE_INTEGER,
    TOKEN_TYPE_INVALID,
    TOKEN_TYPE_EOF,
} TokenType;

typedef struct Token {
    TokenType type;
    char value[100];
} Token;

void printToken( Token *token );
