/**
 * https://en.wikipedia.org/wiki/Backus-Naur_Form
 * La grammaire de l'interpreteur peut être décrite de la façon suivante.
 * <statement> ::= ";" | <expression> ";" | <eof>
 * <expression> ::= <id> <id-followup> | <number>
 * <id-followup> ::= "" | "(" <param-list> ")" | ":" <expression>
 * <param-list> ::= "" | <param> <optparam>
 * <param> ::= <id> | <number> | <vector>
 * <optparam> ::= "" | "," <param> <optparam>
 * <vector> ::= "[" <vectorparam> <optvectorparam> "]"
 * <vectorparam> ::= <id> | <number>
 * <optvectorparam> ::= "" | "," <vectorparam> <optvectorparam>
 */
#ifndef _PARSER_H_
#define _PARSER_H_

void parsing_loop();

#endif //_PARSER_H_
