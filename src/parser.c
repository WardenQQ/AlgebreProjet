#include <stdio.h>

#include "interpreter.h"
#include "parser.h"
#include "tokens.h"
#include "tree.h"

// TODO: message d'erreur

static void match(enum token_type first, Token *lookahead);
static Tree statement(Token *lookahead);
static Tree expression(Token *lookahead);
static Tree id_followup(Token *lookahead, Tree node);
static void param_list(Token *lookahead, Tree parent);
static void param(Token *lookahead, Tree parent);
static void optparam(Token *lookahead, Tree parent);
static void list(Token *lookahead, Tree parent);

Tree parser(Token *lookahead)
{
  return statement(lookahead);
}

static void match(enum token_type first, Token *lookahead)
{
  if (lookahead->type == first) {
    *lookahead = gettoken();
  } else {
    fprintf(stderr, "Syntax error\n");
  }
}

static Tree statement(Token *lookahead)
{
  Tree node;
  switch (lookahead->type) {
    case TOK_EOF:
      return NULL;
    case TOK_SEMI_COLON:
      return NULL;
    case TOK_ID:// Fall through!!
    case TOK_NUMBER:
      node = expression(lookahead);
      //TODO: s'occuper de cette vérification
      //match(TOK_SEMI_COLON, lookahead);
      return node;
    default:
      return NULL;
  }
}

static Tree expression(Token *lookahead)
{
  Tree node = newTree();
  Token vec = {.type = TOK_VECTOR};
  switch (lookahead->type) {
    case TOK_ID:
      setValue(node, *lookahead);
      match(TOK_ID, lookahead);
      node = id_followup(lookahead, node);
      return node;
    case TOK_NUMBER:
      setValue(node, *lookahead);
      match(TOK_NUMBER, lookahead);
      return node;
    case TOK_LEFT_BRACKET:
      setValue(node, vec);
      list(lookahead, node);
      return node;
    default:
        return NULL;
  }
}

static Tree id_followup(Token *lookahead, Tree node)
{
  Tree assign = NULL;
  Tree rvalue = NULL;
  switch(lookahead->type) {
    case TOK_LEFT_PARENTHESE:
      node->value.type = TOK_FUNCTION;
      match(TOK_LEFT_PARENTHESE, lookahead);
      param_list(lookahead, node);
      match(TOK_RIGHT_PARENTHESE, lookahead);
      return node;
    case TOK_COLON:
      assign = newTree();
      setValue(assign, *lookahead);
      setNbChild(assign, 2);
      match(TOK_COLON, lookahead);
      rvalue = expression(lookahead);
      addChild(assign, node);
      addChild(assign, rvalue);
      return assign;
    default: 
      // Empty token 
      return node;
  }
}

static void param_list(Token *lookahead, Tree parent)
{
  switch (lookahead->type) {
    case TOK_ID:
    case TOK_NUMBER:
    case TOK_LEFT_BRACKET:
      param(lookahead, parent);
      optparam(lookahead, parent);
      break;
    default:
      break;
  }
}

static void param(Token *lookahead, Tree parent)
{
  Tree child = expression(lookahead);
  addChild(parent, child);
}

static void optparam(Token *lookahead, Tree parent)
{
  switch (lookahead->type) {
    case TOK_COMMA:
      match(TOK_COMMA, lookahead);
      param(lookahead, parent);
      optparam(lookahead, parent);
      break;
    default:
      // Empty token
      break;
  }
}

static void list(Token *lookahead, Tree parent)
{
  switch (lookahead->type) {
    case TOK_LEFT_BRACKET:
      match(TOK_LEFT_BRACKET, lookahead);
      param_list(lookahead, parent);
      match(TOK_RIGHT_BRACKET, lookahead);
      break;
    default:
      break;
  }
}
