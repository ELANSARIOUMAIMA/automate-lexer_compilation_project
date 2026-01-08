#include <stdio.h>
#include <string.h>
#include "../include/semantic.h"

int state_exists(Automate *a, char *state) {
    for (int i = 0; i < a->nb_etats; i++) {
        if (strcmp(a->etats[i], state) == 0)
            return 1;
    }
    return 0;
}

int symbol_exists(Automate *a, char symbol) {
    for (int i = 0; i < a->nb_symboles; i++) {
        if (a->alphabet[i] == symbol)
            return 1;
    }
    return 0;
}

int check_initial_state(Automate *a) {
    if (!state_exists(a, a->etat_initial)) {
        printf("Erreur semantique : etat initial '%s' non defini\n", a->etat_initial);
        return 0;
    }
    return 1;
}

int check_final_states(Automate *a) {
    for (int i = 0; i < a->nb_etats_finaux; i++) {
        if (!state_exists(a, a->etats_finaux[i])) {
            printf("Erreur semantique : etat final '%s' non defini\n", a->etats_finaux[i]);
            return 0;
        }
    }
    return 1;
}

int check_transitions(Automate *a) {
    for (int i = 0; i < a->nb_transitions; i++) {
        Transition t = a->transitions[i];

        if (!state_exists(a, t.source)) { 
            printf("Erreur semantique : etat source '%s' inconnu\n", t.source);
            return 0;
        }

        if (!state_exists(a, t.destination)) { 
            printf("Erreur semantique : etat destination '%s' inconnu\n", t.destination);
            return 0;
        }

        if (!symbol_exists(a, t.symbole)) { 
            printf("Erreur semantique : symbole '%c' non defini dans l'alphabet\n", t.symbole);
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

                printf("Avertissement : automate non deterministe detecte (etat '%s', symbole '%c')\n",
                       a->transitions[i].source,
                       a->transitions[i].symbole);
                return 1;
            }
        }
    }
    return 0;
}

int check_semantic(Automate *a) {
    printf("----------------- ANALYSE SEMANTIQUE -----------------\n");

    int valid = 1;

    valid &= check_initial_state(a);
    valid &= check_final_states(a);
    valid &= check_transitions(a);

    if (valid) {
        check_determinism(a);
        printf("Analyse semantique reussie\n");
    }

    return valid;
}