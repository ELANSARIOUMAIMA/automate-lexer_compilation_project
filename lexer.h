#ifndef LEXER_H
#define LEXER_H
#include <stdio.h>

//------------------------------------------------------------------------------ 
// DECLARATION DES TOKENS POUR LE LANGAGE DES AUTOMATES
//------------------------------------------------------------------------------ 
typedef enum
{
    // Mots-clés du langage
    ETATS_TOKEN,        // mot-clé "etats"
    ALPHABET_TOKEN,     // mot-clé "alphabet"
    INITIAL_TOKEN,      // mot-clé "initial"
    FINAL_TOKEN,        // mot-clé "final"
    TRANSITIONS_TOKEN,  // mot-clé "transitions"
    
    // Identificateurs et valeurs
    ID_ETAT_TOKEN,      // identificateurs d'états (q0, q1, premier_etat, etc.) - LIBRE
    ID_SYMBOLE_TOKEN,   // symboles de l'alphabet (a, b, c, etc.) - UN SEUL CARACTERE
    
    // Symboles spéciaux
    EGAL_TOKEN,         // =
    VIRGULE_TOKEN,      // ,
    POINTVIRGULE_TOKEN, // ;
    ACCOLADE_OUV_TOKEN, // {
    ACCOLADE_FERM_TOKEN,// }
    FLECHE_TOKEN,       // ->
    DEUXPOINTS_TOKEN,   // :
    
    // Tokens système
    FIN_TOKEN,          // EOF (fin de fichier)
    ERREUR_TOKEN        // erreur lexicale
} CODES_LEX;



//------------------------------------------------------------------------------ 
// STRUCTURE POUR STOCKER LE SYMBOLE COURANT
//------------------------------------------------------------------------------ 
typedef struct {
    CODES_LEX CODE;     // code du token
    char NOM[50];       // valeur textuelle du token
} TSym_Cour;

//------------------------------------------------------------------------------
// Variables globales partagées entre lexer et main
//------------------------------------------------------------------------------
extern TSym_Cour SYM_COUR;      // token courant
extern FILE *fichier;           // fichier source qu'on a en train de le lire
extern char car_cour;

//------------------------------------------------------------------------------
// Fonctions du lexer
//------------------------------------------------------------------------------
void Init_Lexer(const char* nomFichier);      // initialisation du lexer
void Sym_Suiv();                             // lire le token suivant
const char* afficherToken(CODES_LEX code);  // obtenir le nom du token
void Lire_car();                           // lire le prochain caractère

#endif
