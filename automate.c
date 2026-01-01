#include "automate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//------------------------------------------------------------------------------
// CReER UN AUTOMATE VIDE
//------------------------------------------------------------------------------
Automate* creer_automate() {
    Automate* aut = (Automate*)malloc(sizeof(Automate));
    
    // Initialiser tous les compteurs à zero
    aut->nb_etats = 0;
    aut->taille_alphabet = 0;
    aut->nb_etats_finaux = 0;
    aut->nb_transitions = 0;
    
    // Initialiser l'etat initial à vide
    strcpy(aut->etat_initial, "");
    
    return aut;
}

//------------------------------------------------------------------------------
// LIBeRER LA MeMOIRE
//------------------------------------------------------------------------------
void liberer_automate(Automate* aut) {
    if (aut) {
        free(aut);
    }
}

//------------------------------------------------------------------------------
// AFFICHER L'AUTOMATE (POUR DeBOGUER)
//------------------------------------------------------------------------------
void afficher_automate(Automate* aut) {
    if (!aut) {
        printf("Automate NULL\n");
        return;
    }
    
    printf("\n");
    printf("________________ AUTOMATE PARSE ________________\n");
    
    // Afficher les etats
    printf("ETATS (%d): ", aut->nb_etats);
    for (int i = 0; i < aut->nb_etats; i++) {
        printf("%s ", aut->etats[i]);
    }
    printf("\n\n");
    
    // Afficher l'alphabet
    printf("ALPHABET (%d): ", aut->taille_alphabet);
    for (int i = 0; i < aut->taille_alphabet; i++) {
        printf("%c ", aut->alphabet[i]);
    }
    printf("\n\n");
    
    // Afficher l'etat initial
    printf("INITIAL: %s\n\n", aut->etat_initial);
    
    // Afficher les etats finaux
    printf("FINAUX (%d): ", aut->nb_etats_finaux);
    for (int i = 0; i < aut->nb_etats_finaux; i++) {
        printf("%s ", aut->etats_finaux[i]);
    }
    printf("\n\n");
    
    // Afficher les transitions
    printf("TRANSITIONS (%d):\n", aut->nb_transitions);
    for (int i = 0; i < aut->nb_transitions; i++) {
        printf("   %s --%c--> %s\n", 
               aut->transitions[i].etat_source,
               aut->transitions[i].symbole,
               aut->transitions[i].etat_dest);
    }
    printf("\n");
}