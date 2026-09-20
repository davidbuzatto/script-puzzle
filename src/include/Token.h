#pragma once

typedef enum TokenType {

    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_MOVE,
    TOKEN_TYPE_TURN,
    TOKEN_TYPE_PICK,
    TOKEN_TYPE_DROP,

    TOKEN_TYPE_LEFT,
    TOKEN_TYPE_RIGHT,

    TOKEN_TYPE_ADD,
    TOKEN_TYPE_SUB,
    TOKEN_TYPE_MUL,
    TOKEN_TYPE_DIV,
    TOKEN_TYPE_MOD,

    TOKEN_TYPE_LT,
    TOKEN_TYPE_LEQ,
    TOKEN_TYPE_GT,
    TOKEN_TYPE_GEQ,
    TOKEN_TYPE_EQ,
    TOKEN_TYPE_NEQ,
    TOKEN_TYPE_NOT,

    TOKEN_TYPE_LEFT_PAR,
    TOKEN_TYPE_RIGHT_PAR,
    TOKEN_TYPE_COMMA,
    TOKEN_TYPE_SEMICOLON,

    TOKEN_TYPE_INTEGER,
    TOKEN_TYPE_UNKNOWN,

    TOKEN_TYPE_EOF,

} TokenType;

typedef struct Token {
    TokenType type;
    char value[100];
    int lineNumber;
    int charNumber;
} Token;

void printToken( Token *token, int indentation );
