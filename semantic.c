#include <stdio.h>
#include <string.h>
#include "semantic.h"

int state_exists(Automate *a, char *state) {
    for (int i = 0; i < a->nb_etats; i++) {
        if (strcmp(a->etats[i], state) == 0)
            return 1;
    }
    return 0;
}

int symbol_exists(Automate *a, char symbol) {
    for (int i = 0; i < a->nb_symboles; i++) { // Changé nb_alphabet->taille_alphabet
        if (a->alphabet[i] == symbol)
            return 1;
    }
    return 0;
}

int check_initial_state(Automate *a) {
    if (!state_exists(a, a->etat_initial)) { // Changé initial -> etat_initial
        printf("Erreur sémantique : état initial '%s' non défini\n", a->etat_initial);
        return 0;
    }
    return 1;
}

int check_final_states(Automate *a) {
    for (int i = 0; i < a->nb_etats_finaux; i++) { // Changé nb_final -> nb_etats_finaux
        if (!state_exists(a, a->etats_finaux[i])) { // Changé finals -> etats_finaux
            printf("Erreur sémantique : état final '%s' non défini\n", a->etats_finaux[i]);
            return 0;
        }
    }
    return 1;
}

int check_transitions(Automate *a) {
    for (int i = 0; i < a->nb_transitions; i++) {
        Transition t = a->transitions[i];

        if (!state_exists(a, t.source)) { // Changé from -> etat_source
            printf("Erreur sémantique : état source '%s' inconnu\n", t.source);
            return 0;
        }

        if (!state_exists(a, t.destination)) { // Changé to -> etat_dest
            printf("Erreur sémantique : état destination '%s' inconnu\n", t.destination);
            return 0;
        }

        if (!symbol_exists(a, t.symbole)) { // Changé symbol -> symbole
            printf("Erreur sémantique : symbole '%c' non défini dans l'alphabet\n", t.symbole);
            return 0;
        }
    }
    return 1;
}

int check_determinism(Automate *a) {
    for (int i = 0; i < a->nb_transitions; i++) {
        for (int j = i + 1; j < a->nb_transitions; j++) {
            if (strcmp(a->transitions[i].source, a->transitions[j].source) == 0 &&
                a->transitions[i].symbole == a->transitions[j].symbole) {

                printf("Avertissement : automate non déterministe détecté (état '%s', symbole '%c')\n",
                       a->transitions[i].source,
                       a->transitions[i].symbole);
                return 1;
            }
        }
    }
    return 0;
}

int check_semantic(Automate *a) {
    int valid = 1;

    valid &= check_initial_state(a);
    valid &= check_final_states(a);
    valid &= check_transitions(a);

    if (valid) {
        check_determinism(a);
        printf("Analyse sémantique réussie\n");
    }

    return valid;
}