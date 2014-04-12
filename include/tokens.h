#ifndef _TOKENS_H_
#define _TOKENS_H_

#define STRING_MAX 256

enum token_type {
  TOK_NONE = -1,
  // Terminal tokens
  TOK_COLON,
  TOK_COMMA,
  TOK_EOF,
  TOK_NEWLINE,
  TOK_SEMI_COLON,
  TOK_LEFT_PARENTHESE,
  TOK_RIGHT_PARENTHESE,
  TOK_LEFT_BRACKET,
  TOK_RIGHT_BRACKET,
  TOK_NUMBER,
  TOK_ID,

  TOK_VECTOR,
  TOK_FUNCTION
};

struct token_id {
  enum token_type type;
  char name[STRING_MAX];
};

struct token_number {
  enum token_type type;
  double value;
};

typedef union token {
  enum token_type type;
  struct token_number number;
  //TOK_ID or TOK_FUNCTION
  struct token_id id;
} Token;

Token gettoken();

#endif //_TOKENS_H_
