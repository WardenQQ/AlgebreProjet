#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "lexical_analyzer.h"

static enum token_type match_function(const char * string);

union token gettoken()
{
  // Initialise last_char à un caractère ignoré
  static int last_char = '\0';

  while (last_char == ' ' || last_char == '\t' || last_char == '\0') {
    last_char = getchar();
  }

  union token tok;

  switch (last_char) {
    case EOF:
      tok.type = TOK_EOF;
      return tok;
    case ';':
      tok.type = TOK_SEMI_COLON;
      last_char = getchar();
      return tok;
    case '\n':
      tok.type = TOK_NEWLINE;
      last_char = getchar();
      return tok;
    case ':':
      tok.type = TOK_COLON;
      last_char = getchar();
      return tok;
    case '(':
      tok.type = TOK_LEFT_PARENTHESE;
      last_char = getchar();
      return tok;
    case ')':
      tok.type = TOK_RIGHT_PARENTHESE;
      last_char = getchar();
      return tok;
    case '[':
      tok.type = TOK_LEFT_BRACKET;
      last_char = getchar();
      return tok;
    case ']':
      tok.type = TOK_RIGHT_BRACKET;
      last_char = getchar();
      return tok;
    case ',':
      tok.type = TOK_COMMA;
      last_char = getchar();
      return tok;
    default:
      break;
  }

  if (isdigit(last_char)) {
    int hasdot = false;
    int i = 0;
    char number[STRING_MAX];
    do {
      if (last_char == '.') {
        hasdot = true;
      }
      number[i] = last_char;
      ++i;
      last_char = getchar();
    } while((isdigit(last_char) || (last_char == '.' && !hasdot)) 
            && i < STRING_MAX - 1);
    number[i] = '\0';
    tok.type = TOK_NUMBER;
    tok.number.value = strtod(number, NULL);
    return tok;
  }

  if (isalpha(last_char)) {
    int i = 0;
    do {
      tok.id.name[i] = last_char;
      ++i;
      last_char = getchar();
    } while(isalnum(last_char) && i < STRING_MAX - 1);
    tok.id.name[i] = '\0';
    tok.type = match_function(tok.id.name);
    return tok;
  }

  tok.type = TOK_ERROR;
  return tok;
}

static enum token_type match_function(const char * string)
{
  if (strncmp(string, "matrix", STRING_MAX) == 0) {
    return TOK_MATRIX;
  } else if (strncmp(string, "addition", STRING_MAX) == 0) {
    return TOK_ADDITION;
  } else if (strncmp(string, "sub", STRING_MAX) == 0) {
    return TOK_SUB;
  } else if (strncmp(string, "mult", STRING_MAX) == 0) {
    return TOK_MULT;
  } else if (strncmp(string, "mult_scal", STRING_MAX) == 0) {
    return TOK_MULT_SCAL;
  } else if (strncmp(string, "expo", STRING_MAX) == 0) {
    return TOK_EXPO;
  } else if (strncmp(string, "transpose", STRING_MAX) == 0) {
    return TOK_TRANSPOSE;
  } else if (strncmp(string, "determinant", STRING_MAX) == 0) {
    return TOK_DETERMINANT;
  } else if (strncmp(string, "invert", STRING_MAX) == 0) {
    return TOK_INVERT;
  } else if (strncmp(string, "solve", STRING_MAX) == 0) {
    return TOK_SOLVE;
  } else if (strncmp(string, "rank", STRING_MAX) == 0) {
    return TOK_RANK;
  } else if (strncmp(string, "speedtest", STRING_MAX) == 0) {
    return TOK_SPEEDTEST;
  } else if (strncmp(string, "quit", STRING_MAX) == 0) {
    return TOK_QUIT;
  } else {
    return TOK_ID;
  }
}
