#include <stdio.h>
#include "tree.h"
#include "parser.h"
#include "tokens.h"
#include "interpreter.h"

// TODO: message d'erreur

static void match(enum token_type first, union token *lookahead);
static Tree statement(union token *lookahead);
static Tree expression(union token *lookahead);
static Tree id_followup(union token *lookahead, Tree node);
static void param_list(union token *lookahead, Tree parent);
static void param(union token *lookahead, Tree parent);
static void optparam(union token *lookahead, Tree parent);
static void vector(union token *lookahead, Tree parent);
static void vectorparam(union token *lookahead, Tree parent);
static void optvectorparam(union token *lookahead, Tree parent);

void parsing_loop()
{
  union token lookahead;
  lookahead = gettoken();
  do {

    Tree root = statement(&lookahead);
    if (root != NULL) {
      interpreter(root);
      deleteTree(root);
    }

  lookahead = gettoken();
  } while (lookahead.type != TOK_EOF);
}

static void match(enum token_type first, union token *lookahead)
{
  if (lookahead->type == first) {
    *lookahead = gettoken();
  } else {
    fprintf(stderr, "Syntax error\n");
  }
}

static Tree statement(union token *lookahead)
{
  Tree node;
  switch (lookahead->type) {
    case TOK_EOF:
      return NULL;
    case TOK_SEMI_COLON:
      //match(TOK_SEMI_COLON, lookahead);
      return NULL;
    case TOK_ID:// Fall through!!
    case TOK_NUMBER:
      node = expression(lookahead);
      //match(TOK_SEMI_COLON, lookahead);
      return node;
    default:
      return NULL;
  }
}

static Tree expression(union token *lookahead)
{
  Tree node = newTree();
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
    default:
        return NULL;
  }
}

static Tree id_followup(union token *lookahead, Tree node)
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

static void param_list(union token *lookahead, Tree parent)
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

static void param(union token *lookahead, Tree parent)
{
  Tree child;
  union token vec;
  switch (lookahead->type) {
    case TOK_ID:
      child = newTree();
      setValue(child, *lookahead);
      addChild(parent, child);
      match(TOK_ID, lookahead);
      break;
    case TOK_NUMBER:
      child = newTree();
      setValue(child, *lookahead);
      addChild(parent, child);
      match(TOK_NUMBER, lookahead);
      break;
    case TOK_LEFT_BRACKET:
      child = newTree();
      vec.type = TOK_FUNCTION;
      setValue(child, vec);
      addChild(parent, child);
      vector(lookahead, child);
      break;
    default:
      break;
  }
}

static void optparam(union token *lookahead, Tree parent)
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

static void vector(union token *lookahead, Tree parent)
{
  switch (lookahead->type) {
    case TOK_LEFT_BRACKET:
      match(TOK_LEFT_BRACKET, lookahead);
      vectorparam(lookahead, parent);
      optvectorparam(lookahead, parent);
      match(TOK_RIGHT_BRACKET, lookahead);
      break;
    default:
      break;
  }
}

static void vectorparam(union token *lookahead, Tree parent)
{
  Tree child;
  switch (lookahead->type) {
    case TOK_ID:
      child = newTree();
      setValue(child, *lookahead);
      addChild(parent, child);
      match(TOK_ID, lookahead);
      break;
    case TOK_NUMBER:
      child = newTree();
      setValue(child, *lookahead);
      addChild(parent, child);
      match(TOK_NUMBER, lookahead);
      break;
    default:
      break;
  }
}

static void optvectorparam(union token *lookahead, Tree parent)
{
  switch (lookahead->type) {
    case TOK_COMMA:
      match(TOK_COMMA, lookahead);
      vectorparam(lookahead, parent);
      optvectorparam(lookahead, parent);
      break;
    default:
      // Empty token
      break;
  }
}
