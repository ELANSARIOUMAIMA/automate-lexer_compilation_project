#ifndef AUTOMATE_H
#define AUTOMATE_H

// Limites maximales
#define MAX_ETATS 50
#define MAX_ALPHABET 26
#define MAX_TRANSITIONS 200
#define MAX_NOM 50

//------------------------------------------------------------------------------
// STRUCTURE POUR UNE TRANSITION
//--------------------------------------------------------------------------
typedef struct {
    char source[20];
    char destination[20];
    char symbole;
} Transition;

typedef struct {
    char etats[20][20];
    int nb_etats;

    char alphabet[20];
    int nb_symboles;

    char etat_initial[20];

    char etats_finaux[20][20];
    int nb_etats_finaux;

    Transition transitions[50];
    int nb_transitions;
} Automate;



//------------------------------------------------------------------------------
// FONCTIONS UTILITAIRES
//------------------------------------------------------------------------------
Automate* creer_automate();             // Créer un automate vide
void liberer_automate(Automate* aut);   // Libérer la mémoire
void afficher_automate(Automate* aut);  // Afficher l'automate (debug)

#endif