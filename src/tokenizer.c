#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "tokens.h"

Token gettoken()
{
  // Initialise last_char à un caractère ignoré
  static int last_char = '\0';

  while (last_char != ',' && last_char != ':' && last_char != ';'
      && last_char != '(' && last_char != ')' && last_char != '['
      && last_char != ']' && last_char != '.' && last_char != EOF
      && last_char != '_' && last_char != '-' && !isalnum(last_char)
      ) {
    last_char = getchar();
  }

  Token tok;

  switch (last_char) {
    case EOF:
      tok.type = TOK_EOF;
      return tok;
    case ',':
      tok.type = TOK_COMMA;
      last_char = getchar();
      return tok;
    case ';':
      tok.type = TOK_SEMI_COLON;
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
    default:
      break;
  }

  if (isdigit(last_char) || last_char == '.' || last_char == '-') {
    bool hasdot = false;
    bool hasexpo = false;
    int i = 0;
    char number[STRING_MAX];

    do {
      if (last_char == '.') {
        hasdot = true;
      }
      if (last_char == 'e' || last_char == 'E') {
        hasdot = true;
        hasexpo = true;
      }
      number[i] = last_char;
      ++i;
      last_char = getchar();
    } while ((isdigit(last_char) || (last_char == '.' && !hasdot) 
          || (last_char == 'e' && !hasexpo)  || (last_char == 'E' && !hasexpo))
          && i < STRING_MAX - 1);

    number[i] = '\0';
    tok.type = TOK_NUMBER;
    tok.number.value = strtod(number, NULL);
    return tok;
  }

  if (isalpha(last_char) || last_char == '_') {
    int i = 0;
    do {
      tok.id.name[i] = last_char;
      ++i;
      last_char = getchar();
    } while((isalnum(last_char) || last_char == '_') && i < STRING_MAX - 1);
    tok.id.name[i] = '\0';
    tok.type = TOK_ID;
    return tok;
  }
  // Atteint seulement si ce n'etait pas un caractère ignoré au départ.
  last_char = getchar();
  return gettoken(); 
}
