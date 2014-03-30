#ifndef _LEXICAL_ANALYZER_H_
#define _LEXICAL_ANALYZER_H_

#define NAME_MAX 256

enum token_type {
  TOKEN_EOF,
  TOKEN_ID,
  TOKEN_NUMBER,
  // tokens for each function
  TOKEN_MATRIX,
  TOKEN_ADDITION,
  TOKEN_SUB,
  TOKEN_MULT,
  TOKEN_MULT_SCAL,
  TOKEN_EXPO,
  TOKEN_TRANSPOSE,
  TOKEN_DETERMINANT,
  TOKEN_INVERT,
  TOKEN_SOLVE,
  TOKEN_RANK,
  TOKEN_SPEEDTEST,
  TOKEN_QUIT,
  // tokens for punctuation
  TOKEN_COMMA,
  TOKEN_COLON,
  TOKEN_LEFT_PARENTHESE,
  TOKEN_RIGHT_PARENTHESE,
  TOKEN_LEFT_BRACKET,
  TOKEN_RIGHT_BRACKET,
};

struct token_id {
  enum token_type type;
  char name[NAME_MAX];
};

struct token_number {
  enum token_type type;
  float value;
};

union token {
  enum token_type type;

  struct token_number number;

  struct token_id id;
};

union token get_next_token();

#endif // _LEXICAL_ANALYZER_H_
