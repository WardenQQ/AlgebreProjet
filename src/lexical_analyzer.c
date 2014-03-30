#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "lexical_analyzer.h"

static enum token_type match_function(const char * string);

union token get_next_token()
{
  static int last_char = ' ';

  while (isspace(last_char)) {
    last_char = getchar();
  }

  union token tok;
  tok.type = TOKEN_MATRIX;

  if (last_char == EOF) {
    tok.type = TOKEN_EOF;
    return tok;
  }

  if (isalpha(last_char)) {
    tok.id.type = TOKEN_ID;
    tok.id.name[0] = last_char;
    last_char = getchar();
    int i = 1;
    while (isalnum(last_char) && i < NAME_MAX - 1) {
      tok.id.name[i] = last_char;
      last_char = getchar();
      ++i;
    }
    tok.id.name[i] = '\0';
    tok.id.type = match_function(tok.id.name);
  } else if (isdigit(last_char)) {
    tok.id.type = TOKEN_NUMBER;
    char number[NAME_MAX];
    number[0] = last_char;
    last_char = getchar();
    int i = 1;
    while (isdigit(last_char) && i < NAME_MAX - 1) {
      number[i] = last_char;
      last_char = getchar();
      ++i;
    }
    number[i] = '\0';
    tok.number.value = strtod(number, NULL);
  } else if(last_char == ',') {
    tok.type = TOKEN_COMMA; 
    last_char = getchar();
  } else if(last_char == ':') {
    tok.type = TOKEN_COLON; 
    last_char = getchar();
  } else if(last_char == '(') {
    tok.type = TOKEN_LEFT_PARENTHESE; 
    last_char = getchar();
  } else if(last_char == ')') {
    tok.type = TOKEN_RIGHT_PARENTHESE; 
    last_char = getchar();
  } else if(last_char == '[') {
    tok.type = TOKEN_LEFT_BRACKET; 
    last_char = getchar();
  } else if(last_char == ']') {
    tok.type = TOKEN_RIGHT_BRACKET; 
    last_char = getchar();
  } else {
    abort();
    last_char = getchar();
  }

  return tok;
}

static enum token_type match_function(const char * string)
{
  if (strncmp(string, "matrix", NAME_MAX) == 0) {
    return TOKEN_MATRIX;
  } else if (strncmp(string, "addition", NAME_MAX) == 0) {
    return TOKEN_ADDITION;
  } else if (strncmp(string, "sub", NAME_MAX) == 0) {
    return TOKEN_SUB;
  } else if (strncmp(string, "mult", NAME_MAX) == 0) {
    return TOKEN_MULT;
  } else if (strncmp(string, "mult_scal", NAME_MAX) == 0) {
    return TOKEN_MULT_SCAL;
  } else if (strncmp(string, "expo", NAME_MAX) == 0) {
    return TOKEN_EXPO;
  } else if (strncmp(string, "transpose", NAME_MAX) == 0) {
    return TOKEN_TRANSPOSE;
  } else if (strncmp(string, "determinant", NAME_MAX) == 0) {
    return TOKEN_DETERMINANT;
  } else if (strncmp(string, "invert", NAME_MAX) == 0) {
    return TOKEN_INVERT;
  } else if (strncmp(string, "solve", NAME_MAX) == 0) {
    return TOKEN_SOLVE;
  } else if (strncmp(string, "rank", NAME_MAX) == 0) {
    return TOKEN_RANK;
  } else if (strncmp(string, "speedtest", NAME_MAX) == 0) {
    return TOKEN_SPEEDTEST;
  } else if (strncmp(string, "quit", NAME_MAX) == 0) {
    return TOKEN_QUIT;
  } else {
    return TOKEN_ID;
  }
}
