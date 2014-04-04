#include <stdio.h>
#include "parser.h"
#include "lexical_analyzer.h"

// TODO: message d'erreur

static void match(enum token_type first, union token *lookahead);
static void statement(union token *lookahead);
static void expression(union token *lookahead);
static void id_followup(union token *lookahead);
static void param_list(union token *lookahead);
static void param(union token *lookahead);
static void optparam(union token *lookahead);
static void vector(union token *lookahead);
static void vectorparam(union token *lookahead);
static void optvectorparam(union token *lookahead);

void parsing_loop()
{
  union token lookahead = gettoken();
  while (lookahead.type != TOK_EOF) {
    printf("> ");
    statement(&lookahead);
  }
}

static void match(enum token_type first, union token *lookahead)
{
  if (lookahead->type == first) {
    *lookahead = gettoken();
  } else {
    fprintf(stderr, "Syntax error\n");
  }
}

static void statement(union token *lookahead)
{
  switch (lookahead->type) {
    case TOK_EOF:
      match(TOK_EOF, lookahead);
      break;
    case TOK_SEMI_COLON:
      match(TOK_SEMI_COLON, lookahead);
      break;
    case TOK_ID:// Fall through!!
    case TOK_NUMBER:
      expression(lookahead);
      match(TOK_SEMI_COLON, lookahead);
      break;
    default:
      break;
  }
}

static void expression(union token *lookahead)
{
  switch (lookahead->type) {
    case TOK_ID:
      match(TOK_ID, lookahead);
      id_followup(lookahead);
      break;
    case TOK_NUMBER:
      match(TOK_NUMBER, lookahead);
      break;
    default:
      break;
  }
}

static void id_followup(union token *lookahead)
{
  switch(lookahead->type) {
    case TOK_LEFT_PARENTHESE:
      match(TOK_LEFT_PARENTHESE, lookahead);
      param_list(lookahead);
      match(TOK_RIGHT_PARENTHESE, lookahead);
      break;
    case TOK_COLON:
      match(TOK_COLON, lookahead);
      expression(lookahead);
      break;
    default: 
      // Empty token 
      break;
  }
}

static void param_list(union token *lookahead)
{
  switch (lookahead->type) {
    case TOK_ID:
    case TOK_NUMBER:
    case TOK_LEFT_BRACKET:
      param(lookahead);
      optparam(lookahead);
      break;
    default:
      break;
  }
}

static void param(union token *lookahead)
{
  switch (lookahead->type) {
    case TOK_ID:
      match(TOK_ID, lookahead);
      break;
    case TOK_NUMBER:
      match(TOK_NUMBER, lookahead);
      break;
    case TOK_LEFT_BRACKET:
      vector(lookahead);
      break;
    default:
      break;
  }
}

static void optparam(union token *lookahead)
{
  switch (lookahead->type) {
    case TOK_COMMA:
      match(TOK_COMMA, lookahead);
      param(lookahead);
      optparam(lookahead);
      break;
    default:
      // Empty token
      break;
  }
}

static void vector(union token *lookahead)
{
  switch (lookahead->type) {
    case TOK_LEFT_BRACKET:
      match(TOK_LEFT_BRACKET, lookahead);
      vectorparam(lookahead);
      optvectorparam(lookahead);
      match(TOK_RIGHT_BRACKET, lookahead);
      break;
    default:
      break;
  }
}

static void vectorparam(union token *lookahead)
{
  switch (lookahead->type) {
    case TOK_ID:
      match(TOK_ID, lookahead);
      break;
    case TOK_NUMBER:
      match(TOK_NUMBER, lookahead);
      break;
    default:
      break;
  }
}

static void optvectorparam(union token *lookahead)
{
  switch (lookahead->type) {
    case TOK_COMMA:
      match(TOK_COMMA, lookahead);
      vectorparam(lookahead);
      optvectorparam(lookahead);
      break;
    default:
      // Empty token
      break;
  }
}
