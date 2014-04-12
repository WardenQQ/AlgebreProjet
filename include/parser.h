/**
 * https://en.wikipedia.org/wiki/Backus-Naur_Form
 * La grammaire de l'interpreteur peut être décrite de la façon suivante.
 * <statement> ::= ";" | <expression> ";" | <eof>
 * <expression> ::= <id> <id-followup> | <number> | <list>
 * <id-followup> ::= "" | "(" <param-list> ")" | ":" <expression>
 * <param-list> ::= "" | <param> <optparam>
 * <param> ::= <expression>
 * <optparam> ::= "" | "," <param> <optparam>
 * <list> ::= "[" <listparam> <optlistparam> "]"
 * <listparam> ::= <id> | <number>
 * <optlistparam> ::= "" | "," <listparam> <optlistparam>
 */
#ifndef _PARSER_H_
#define _PARSER_H_

void parsing_loop();
void parser();

#endif //_PARSER_H_
