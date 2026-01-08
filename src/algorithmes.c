#include "../include/algorithmes.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//==============================================================================
// ALGORITHME 1 : SIMULATION D'AUTOMATE (Test d'appartenance)
//==============================================================================

int accepter_chaine(Automate* aut, const char* chaine) {
    printf("Chaine a tester : \"%s\"\n", chaine);
    printf("====================================================\n");
    
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
            printf("====================================================\n");
            printf("Lecture '%c' depuis %s -> Pas de transition\n", 
                   symbole, etat_courant);
            printf("Resultat : CHAINE REJETEE\n");
            printf("====================================================\n\n");
            return 0;
        }
        printf("Lecture '%c' depuis %s -> %s\n", 
               symbole, etat_courant, nouvel_etat);
        strcpy(etat_courant, nouvel_etat);
    }
    printf("====================================================\n");
    printf("Etat final atteint : %s\n", etat_courant);
    
    for (int i = 0; i < aut->nb_etats_finaux; i++) {
        if (strcmp(aut->etats_finaux[i], etat_courant) == 0) {
            printf("Resultat : CHAINE ACCEPTEE\n");
            printf("====================================================\n\n");
            return 1;
        }
    }
    printf("Resultat : CHAINE REJETEE\n");
    printf("====================================================\n\n");
    return 0;
}

//=============================================================================
// ALGORITHME 2 : TEST DE DÉTERMINISME
//=============================================================================

int est_deterministe(Automate* aut) {
    printf("  VERIFICATION DU DETERMINISME\n");
    
    for (int i = 0; i < aut->nb_transitions; i++) {
        for (int j = i + 1; j < aut->nb_transitions; j++) {
            Transition t1 = aut->transitions[i];
            Transition t2 = aut->transitions[j];
            
            if (strcmp(t1.source, t2.source) == 0 && 
                t1.symbole == t2.symbole) {
                printf("    Etat source : %s\n", t1.source);
                printf("    Symbole     : %c\n", t1.symbole);
                printf("    Destination 1 : %s (transition %d)\n", t1.destination, i+1);
                printf("    Destination 2 : %s (transition %d)\n", t2.destination, j+1);
                printf("Resultat : AUTOMATE NON DETERMINISTE\n\n");
                return 0;
            }
        }
    }
    
    printf("Aucun conflit detecte\n");
    printf("Resultat : AUTOMATE DETERMINISTE\n\n");
    return 1;
}

//==============================================================================
// FONCTIONS AUXILIAIRES POUR LA DÉTERMINISATION
//==============================================================================

// Parser un macro-état "{q1,q2}" en tableau d'états individuels
int parser_macro_etat(char* macro_etat, char etats[][20]) {
    if (macro_etat[0] != '{') {
        // État simple
        strcpy(etats[0], macro_etat);
        return 1;
    }
    
    // Copier le contenu sans { et }
    char temp[100];
    size_t len = strlen(macro_etat) - 2;
    memcpy(temp, macro_etat + 1, len);
    temp[len] = '\0';
    
    int nb = 0;
    char* token = strtok(temp, ",");
    while (token != NULL && nb < 50) {
        // Nettoyer les espaces au début
        while (*token == ' ') token++;
        
        // Nettoyer les espaces à la fin
        char* end = token + strlen(token) - 1;
        while (end > token && *end == ' ') {
            *end = '\0';
            end--;
        }
        
        if (*token != '\0') {  // Ne pas ajouter si vide
            strcpy(etats[nb++], token);
        }
        token = strtok(NULL, ",");
    }
    return nb;
}

// Construire un macro-état trié à partir d'un tableau de destinations
void construire_macro_etat(char destinations[][20], int nb_dest, char* resultat) {
    if (nb_dest == 0) {
        strcpy(resultat, "VIDE");
        return;
    }
    
    if (nb_dest == 1) {
        strcpy(resultat, destinations[0]);
        return;
    }
    
    // Trier les destinations (bubble sort)
    for (int i = 0; i < nb_dest - 1; i++) {
        for (int j = i + 1; j < nb_dest; j++) {
            if (strcmp(destinations[i], destinations[j]) > 0) {
                char temp[20];
                strcpy(temp, destinations[i]);
                strcpy(destinations[i], destinations[j]);
                strcpy(destinations[j], temp);
            }
        }
    }
    
    // Construire le nom avec vérification de taille
    int pos = 0;
    resultat[pos++] = '{';
    
    for (int d = 0; d < nb_dest; d++) {
        int len = strlen(destinations[d]);
        
        // Vérifier l'espace disponible (+ virgule + } + '\0')
        if (pos + len + 3 >= 100) {
            fprintf(stderr, "ERREUR : Nom de macro-etat trop long (limite 100 car)\n");
            strcpy(resultat, "ERROR_OVERFLOW");
            return;
        }
        
        strcpy(resultat + pos, destinations[d]);
        pos += len;
        
        if (d < nb_dest - 1) {
            resultat[pos++] = ',';
        }
    }
    
    resultat[pos++] = '}';
    resultat[pos] = '\0';
}

//==============================================================================
// ALGORITHME 3 : DÉTERMINISATION (Construction des sous-ensembles)
//==============================================================================

Automate* determiniser(Automate* aut) {
    if (est_deterministe(aut)) {
        printf("L'automate est deja deterministe.\n");
        printf("Retour de l'automate original.\n");
        printf("=======================================\n\n");
        return aut;
    }
    
    printf("\nDeterminisation en cours...\n");
    
    Automate* det = creer_automate();
    
    // Copier l'alphabet
    det->nb_symboles = aut->nb_symboles;
    for (int i = 0; i < aut->nb_symboles; i++) {
        det->alphabet[i] = aut->alphabet[i];
    }
    
    // État initial
    strcpy(det->etat_initial, aut->etat_initial);
    strcpy(det->etats[det->nb_etats], aut->etat_initial);
    det->nb_etats = 1;
    
    // File BFS
    char etats_a_traiter[50][100];
    int debut = 0, fin = 0;
    strcpy(etats_a_traiter[fin++], det->etat_initial);
    
    // Parcours en largeur
    while (debut < fin && det->nb_etats < 50) {
        char etat_courant[100];
        strcpy(etat_courant, etats_a_traiter[debut++]);
        
        // Décomposer le macro-état en états individuels
        char etats_composants[50][20];
        int nb_composants = parser_macro_etat(etat_courant, etats_composants);
        
        // Pour chaque symbole de l'alphabet
        for (int sym = 0; sym < det->nb_symboles; sym++) {
            char symbole = det->alphabet[sym];
            char destinations[50][20];
            int nb_dest = 0;
            
            // Chercher toutes les transitions depuis chaque composant
            for (int comp = 0; comp < nb_composants; comp++) {
                for (int t = 0; t < aut->nb_transitions; t++) {
                    if (strcmp(aut->transitions[t].source, etats_composants[comp]) == 0 &&
                        aut->transitions[t].symbole == symbole) {
                        
                        // Vérifier si destination déjà présente
                        int deja_present = 0;
                        for (int d = 0; d < nb_dest; d++) {
                            if (strcmp(destinations[d], aut->transitions[t].destination) == 0) {
                                deja_present = 1;
                                break;
                            }
                        }
                        
                        // Ajouter si unique et limite non atteinte
                        if (!deja_present && nb_dest < 50) {
                            strcpy(destinations[nb_dest++], aut->transitions[t].destination);
                        }
                    }
                }
            }
            
            // Si aucune destination, passer au symbole suivant
            if (nb_dest == 0) continue;
            
            // Construire le nouvel état (simple ou macro)
            char nouvel_etat[100];
            construire_macro_etat(destinations, nb_dest, nouvel_etat);
            
            // Vérifier si cet état existe déjà
            int etat_existe = -1;
            for (int e = 0; e < det->nb_etats; e++) {
                if (strcmp(det->etats[e], nouvel_etat) == 0) {
                    etat_existe = e;
                    break;
                }
            }
            
            // Ajouter si nouvel état
            if (etat_existe == -1) {
                if (det->nb_etats >= 50) {
                    fprintf(stderr, "ERREUR : Limite de 50 etats atteinte\n");
                    break;
                }
                strcpy(det->etats[det->nb_etats], nouvel_etat);
                etat_existe = det->nb_etats;
                det->nb_etats++;
                
                if (fin < 50) {
                    strcpy(etats_a_traiter[fin++], nouvel_etat);
                }
            }
            
            // Créer la transition
            if (det->nb_transitions < 200) {
                Transition t;
                strcpy(t.source, etat_courant);
                strcpy(t.destination, nouvel_etat);
                t.symbole = symbole;
                det->transitions[det->nb_transitions++] = t;
            } else {
                fprintf(stderr, "ERREUR : Limite de 200 transitions atteinte\n");
            }
        }
    }
    
    // Déterminer les états finaux
    det->nb_etats_finaux = 0;
    for (int i = 0; i < det->nb_etats; i++) {
        char etat[100];
        strcpy(etat, det->etats[i]);
        
        // Décomposer l'état
        char composants[50][20];
        int nb_comp = parser_macro_etat(etat, composants);
        
        // Un état est final si au moins un composant est final
        int est_final = 0;
        for (int c = 0; c < nb_comp; c++) {
            for (int f = 0; f < aut->nb_etats_finaux; f++) {
                if (strcmp(composants[c], aut->etats_finaux[f]) == 0) {
                    est_final = 1;
                    break;
                }
            }
            if (est_final) break;
        }
        
        if (est_final && det->nb_etats_finaux < 50) {
            strcpy(det->etats_finaux[det->nb_etats_finaux++], etat);
        }
    }
    
    printf("\nDeterminisation terminee\n");
    printf("   Etats : %d -> %d\n", aut->nb_etats, det->nb_etats);
    printf("   Transitions : %d -> %d\n", aut->nb_transitions, det->nb_transitions);
    printf("=======================================\n\n");
    
    return det;
}

//==============================================================================
// ALGORITHME 4 : AFFICHAGE DE L'AUTOMATE
//==============================================================================

void afficher_automate_algo(Automate* aut) {
    printf("ETATS (%d) :\n   ", aut->nb_etats);
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
    
    printf("ETAT INITIAL :\n   %s\n\n", aut->etat_initial);
    
    printf("ETATS FINAUX (%d) :\n   ", aut->nb_etats_finaux);
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

//==============================================================================
// ALGORITHME 5 : SAUVEGARDE D'AUTOMATE
//==============================================================================

int sauvegarder_automate(Automate* aut, const char* nom_fichier) {
    printf("\n=======================================\n");
    printf("  SAUVEGARDE DE L'AUTOMATE\n");
    printf("========================================\n");
    
    FILE* f = fopen(nom_fichier, "w");
    
    if (!f) {
        printf("ERREUR : Impossible de creer le fichier %s\n", nom_fichier);
        printf("=======================================\n\n");
        return 0;
    }
    
    fprintf(f, "etats = ");
    for (int i = 0; i < aut->nb_etats; i++) {
        fprintf(f, "%s", aut->etats[i]);
        if (i < aut->nb_etats - 1) {
            fprintf(f, ", ");
        }
    }
    fprintf(f, ";\n");
    
    fprintf(f, "alphabet = ");
    for (int i = 0; i < aut->nb_symboles; i++) {
        fprintf(f, "%c", aut->alphabet[i]);
        if (i < aut->nb_symboles - 1) {
            fprintf(f, ", ");
        }
    }
    fprintf(f, ";\n");
    
    fprintf(f, "initial = %s;\n", aut->etat_initial);
    
    fprintf(f, "final = ");
    for (int i = 0; i < aut->nb_etats_finaux; i++) {
        fprintf(f, "%s", aut->etats_finaux[i]);
        if (i < aut->nb_etats_finaux - 1) {
            fprintf(f, ", ");
        }
    }
    fprintf(f, ";\n");
    
    fprintf(f, "transitions {\n");
    for (int i = 0; i < aut->nb_transitions; i++) {
        Transition t = aut->transitions[i];
        fprintf(f, "    %s -> %s : %c;\n", 
                t.source, t.destination, t.symbole);
    }
    fprintf(f, "}\n");
    
    fclose(f);
    
    printf("Automate sauvegarde avec succes !\n");
    
    return 1;
}