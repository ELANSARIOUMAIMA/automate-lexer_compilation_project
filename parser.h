#ifndef PARSER_H
#define PARSER_H

#include "automate.h"
#include "lexer.h"

//------------------------------------------------------------------------------
// FONCTION PRINCIPALE DU PARSER
//------------------------------------------------------------------------------
// Parse un fichier d'automate et retourne la structure Automate remplie
Automate* parser(const char* nomFichier);

//------------------------------------------------------------------------------
// FONCTIONS AUXILIAIRES (PRIVÉES - déclarées ici pour la modularité)
//------------------------------------------------------------------------------
void parse_etats(Automate* aut);
void parse_alphabet(Automate* aut);
void parse_initial(Automate* aut);
void parse_final(Automate* aut);
void parse_transitions(Automate* aut);

//------------------------------------------------------------------------------
// FONCTION D'ERREUR
//------------------------------------------------------------------------------
void erreur_syntaxe(const char* message);

#endif