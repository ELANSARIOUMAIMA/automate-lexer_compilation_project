#include "algorithmes.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//==============================================================================
// ALGORITHME 1 : SIMULATION D'AUTOMATE (Test d'appartenance)
//==============================================================================

int accepter_chaine(Automate* aut, const char* chaine) {
    printf("\n═══════════════════════════════════════════════════\n");
    printf("Chaîne à tester : \"%s\"\n", chaine);
    printf("───────────────────────────────────────────────────\n");
    
    char etat_courant[20];
    strcpy(etat_courant, aut->etat_initial);
    
    for (int i = 0; chaine[i] != '\0'; i++) {
        char symbole = chaine[i];
        
        int transition_trouvee = 0;
        char nouvel_etat[20];
        for (int j = 0; j < aut->nb_transitions; j++) {
            Transition t = aut->transitions[j];
            
            if (strcmp(t.source, etat_courant) == 0 && t.symbole == symbole) {
                strcpy(nouvel_etat, t.destination);
                transition_trouvee = 1;
                break;
            }
        }
        if (!transition_trouvee) {
            printf("───────────────────────────────────────────────────\n");
            printf("Lecture '%c' depuis %s → Pas de transition\n", 
                   symbole, etat_courant);
            printf("Résultat : CHAÎNE REJETÉE\n");
            printf("═══════════════════════════════════════════════════\n\n");
            return 0;
        }
        printf("Lecture '%c' depuis %s → %s\n", 
               symbole, etat_courant, nouvel_etat);
        strcpy(etat_courant, nouvel_etat);
    }
    printf("───────────────────────────────────────────────────\n");
    printf("État final atteint : %s\n", etat_courant);
    
    for (int i = 0; i < aut->nb_etats_finaux; i++) {
        if (strcmp(aut->etats_finaux[i], etat_courant) == 0) {
            printf("Résultat : CHAÎNE ACCEPTÉE (état final)\n");
            printf("═══════════════════════════════════════════════════\n\n");
            return 1;
        }
    }
    printf("Résultat : CHAÎNE REJETÉE (état non-final)\n");
    printf("═══════════════════════════════════════════════════\n\n");
    return 0;
}

//==============================================================================
// ALGORITHME 2 : TEST DE DÉTERMINISME
//==============================================================================

int est_deterministe(Automate* aut) {
    printf("\n═══════════════════════════════════════════════════\n");
    printf("  VÉRIFICATION DU DÉTERMINISME\n");
    printf("═══════════════════════════════════════════════════\n");
    
    for (int i = 0; i < aut->nb_transitions; i++) {
        for (int j = i + 1; j < aut->nb_transitions; j++) {
            Transition t1 = aut->transitions[i];
            Transition t2 = aut->transitions[j];
            
            if (strcmp(t1.source, t2.source) == 0 && 
                t1.symbole == t2.symbole) {
                printf("    État source : %s\n", t1.source);
                printf("    Symbole     : %c\n", t1.symbole);
                printf("    Destination 1 : %s (transition %d)\n", t1.destination, i+1);
                printf("    Destination 2 : %s (transition %d)\n", t2.destination, j+1);
                printf("───────────────────────────────────────────────────\n");
                printf("Résultat : AUTOMATE NON DÉTERMINISTE\n");
                printf("═══════════════════════════════════════════════════\n\n");
                return 0;
            }
        }
    }
    
    printf("Aucun conflit détecté\n");
    printf("───────────────────────────────────────────────────\n");
    printf("Résultat : AUTOMATE DÉTERMINISTE\n");
    printf("═══════════════════════════════════════════════════\n\n");
    return 1;
}

//==============================================================================
// ALGORITHME 3 : DÉTERMINISATION (Construction des sous-ensembles)
//==============================================================================

Automate* determiniser(Automate* aut) {
    printf("\n═══════════════════════════════════════════════════\n");
    printf("  DÉTERMINISATION DE L'AUTOMATE\n");
    printf("═══════════════════════════════════════════════════\n");
    
    if (est_deterministe(aut)) {
        printf("L'automate est déjà déterministe.\n");
        printf("Retour de l'automate original.\n");
        printf("═══════════════════════════════════════════════════\n\n");
        return aut;
    }
    
    printf("\nDéterminisation en cours...\n\n");
    
    Automate* det = creer_automate();
    
    det->nb_symboles = aut->nb_symboles;
    for (int i = 0; i < aut->nb_symboles; i++) {
        det->alphabet[i] = aut->alphabet[i];
    }
    
    strcpy(det->etat_initial, aut->etat_initial);
    strcpy(det->etats[det->nb_etats], aut->etat_initial);
    det->nb_etats = 1;
    
    char etats_a_traiter[50][20];
    int debut = 0, fin = 0;
    
    strcpy(etats_a_traiter[fin++], det->etat_initial);
    
    int etats_traites[50] = {0};
    etats_traites[0] = 1;
    
    // Parcours en largeur
    while (debut < fin && det->nb_etats < 50) {
        char etat_courant[20];
        strcpy(etat_courant, etats_a_traiter[debut++]);
        
        for (int sym = 0; sym < det->nb_symboles; sym++) {
            char symbole = det->alphabet[sym];
            char destinations[50][20];
            int nb_dest = 0;
            
            for (int t = 0; t < aut->nb_transitions; t++) {
                if (strcmp(aut->transitions[t].source, etat_courant) == 0 &&
                    aut->transitions[t].symbole == symbole) {
                    
                    int deja_present = 0;
                    for (int d = 0; d < nb_dest; d++) {
                        if (strcmp(destinations[d], aut->transitions[t].destination) == 0) {
                            deja_present = 1;
                            break;
                        }
                    }
                    if (!deja_present) {
                        strcpy(destinations[nb_dest++], aut->transitions[t].destination);
                    }
                }
            }
            if (nb_dest == 0) {
                continue;
            }
            char nouvel_etat[20];
            strcpy(nouvel_etat, destinations[0]);
            
            int etat_existe = -1;
            for (int e = 0; e < det->nb_etats; e++) {
                if (strcmp(det->etats[e], nouvel_etat) == 0) {
                    etat_existe = e;
                    break;
                }
            }
            
            if (etat_existe == -1) {
                strcpy(det->etats[det->nb_etats], nouvel_etat);
                etat_existe = det->nb_etats;
                det->nb_etats++;
                
                strcpy(etats_a_traiter[fin++], nouvel_etat);
            }
            
            Transition nouvelle_trans;
            strcpy(nouvelle_trans.source, etat_courant);
            strcpy(nouvelle_trans.destination, nouvel_etat);
            nouvelle_trans.symbole = symbole;
            
            det->transitions[det->nb_transitions++] = nouvelle_trans;
        }
    }
    
    det->nb_etats_finaux = 0;
    for (int i = 0; i < det->nb_etats; i++) {
        char etat[20];
        strcpy(etat, det->etats[i]);
        
        for (int f = 0; f < aut->nb_etats_finaux; f++) {
            if (strcmp(etat, aut->etats_finaux[f]) == 0) {
                strcpy(det->etats_finaux[det->nb_etats_finaux++], etat);
                break;
            }
        }
    }
    
    printf("\nDéterminisation terminée\n");
    printf("   Nombre d'états : %d → %d\n", aut->nb_etats, det->nb_etats);
    printf("   Nombre de transitions : %d → %d\n", 
           aut->nb_transitions, det->nb_transitions);
    printf("═══════════════════════════════════════════════════\n\n");
    
    return det;
}

//==============================================================================
// ALGORITHME 4 : AFFICHAGE DE L'AUTOMATE
//==============================================================================

void afficher_automate_algo(Automate* aut) {
    printf("ÉTATS (%d) :\n   ", aut->nb_etats);
    for (int i = 0; i < aut->nb_etats; i++) {
        printf("%s", aut->etats[i]);
        if (i < aut->nb_etats - 1) printf(", ");
    }
    printf("\n\n");
    
    printf("ALPHABET (%d) :\n   ", aut->nb_symboles);
    for (int i = 0; i < aut->nb_symboles; i++) {
        printf("'%c'", aut->alphabet[i]);
        if (i < aut->nb_symboles - 1) printf(", ");
    }
    printf("\n\n");
    
    printf("ÉTAT INITIAL :\n   %s\n\n", aut->etat_initial);
    
    printf("ÉTATS FINAUX (%d) :\n   ", aut->nb_etats_finaux);
    for (int i = 0; i < aut->nb_etats_finaux; i++) {
        printf("%s", aut->etats_finaux[i]);
        if (i < aut->nb_etats_finaux - 1) printf(", ");
    }
    printf("\n\n");
    
    printf("TRANSITIONS (%d) :\n", aut->nb_transitions);
    for (int i = 0; i < aut->nb_transitions; i++) {
        printf("   %s --(%c)--> %s\n",
               aut->transitions[i].source,
               aut->transitions[i].symbole,
               aut->transitions[i].destination);
    }
    printf("\n");
}