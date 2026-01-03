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
    for (int i = 0; i < a->nb_alphabet; i++) {
        if (a->alphabet[i] == symbol)
            return 1;
    }
    return 0;
}

int check_initial_state(Automate *a) {
    if (!state_exists(a, a->initial)) {
        printf("Erreur sémantique : état initial '%s' non défini\n", a->initial);
        return 0;
    }
    return 1;
}

int check_final_states(Automate *a) {
    for (int i = 0; i < a->nb_final; i++) {
        if (!state_exists(a, a->finals[i])) {
            printf("Erreur sémantique : état final '%s' non défini\n", a->finals[i]);
            return 0;
        }
    }
    return 1;
}

int check_transitions(Automate *a) {
    for (int i = 0; i < a->nb_transitions; i++) {
        Transition t = a->transitions[i];

        if (!state_exists(a, t.from)) {
            printf("Erreur sémantique : état source '%s' inconnu\n", t.from);
            return 0;
        }

        if (!state_exists(a, t.to)) {
            printf("Erreur sémantique : état destination '%s' inconnu\n", t.to);
            return 0;
        }

        if (!symbol_exists(a, t.symbol)) {
            printf("Erreur sémantique : symbole '%c' non défini dans l’alphabet\n", t.symbol);
            return 0;
        }
    }
    return 1;
}

int check_determinism(Automate *a) {
    for (int i = 0; i < a->nb_transitions; i++) {
        for (int j = i + 1; j < a->nb_transitions; j++) {
            if (strcmp(a->transitions[i].from, a->transitions[j].from) == 0 &&
                a->transitions[i].symbol == a->transitions[j].symbol) {

                printf("Avertissement : automate non déterministe détecté (état '%s', symbole '%c')\n",
                       a->transitions[i].from,
                       a->transitions[i].symbol);
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
