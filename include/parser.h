/**
 * https://en.wikipedia.org/wiki/Backus-Naur_Form
 * La grammaire de l'interpreteur peut être décrite de la façon suivante.
 * <statement> ::= ";" | <expression> ";" | <eof>
 * <expression> ::= <id> <id-followup> | <number> | <list>
 * <id-followup> ::= "" | "(" <param-list> ")" | ":" <expression>
 * <param-list> ::= "" | <param> <optparam>
 * <param> ::= <expression>
 * <optparam> ::= "" | "," <param> <optparam>
 * <list> ::= "[" <param-list> "]"
 */
#ifndef _PARSER_H_
#define _PARSER_H_

#include "tree.h"

Tree parser(Token *lookahead);

#endif //_PARSER_H_
