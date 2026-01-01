#ifndef AUTOMATE_H
#define AUTOMATE_H

// Limites maximales
#define MAX_ETATS 50
#define MAX_ALPHABET 26
#define MAX_TRANSITIONS 200
#define MAX_NOM 50

//------------------------------------------------------------------------------
// STRUCTURE POUR UNE TRANSITION
//------------------------------------------------------------------------------
typedef struct {
    char etat_source[MAX_NOM];      // Ex: "q0"
    char etat_dest[MAX_NOM];        // Ex: "q1"
    char symbole;                   // Ex: 'a'
} Transition;

//------------------------------------------------------------------------------
// STRUCTURE POUR L'AUTOMATE COMPLET
//------------------------------------------------------------------------------
typedef struct {
    // États
    char etats[MAX_ETATS][MAX_NOM]; // Ex: ["q0", "q1", "q2"]
    int nb_etats;                   // Ex: 3
    
    // Alphabet
    char alphabet[MAX_ALPHABET];    // Ex: ['a', 'b']
    int taille_alphabet;            // Ex: 2
    
    // État initial
    char etat_initial[MAX_NOM];     // Ex: "q0"
    
    // États finaux
    char etats_finaux[MAX_ETATS][MAX_NOM];  // Ex: ["q2"]
    int nb_etats_finaux;                    // Ex: 1
    
    // Transitions
    Transition transitions[MAX_TRANSITIONS];  // Tableau de transitions
    int nb_transitions;                       // Nombre de transitions
} Automate;

//------------------------------------------------------------------------------
// FONCTIONS UTILITAIRES
//------------------------------------------------------------------------------
Automate* creer_automate();             // Créer un automate vide
void liberer_automate(Automate* aut);   // Libérer la mémoire
void afficher_automate(Automate* aut);  // Afficher l'automate (debug)

#endif