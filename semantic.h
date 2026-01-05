#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "automate.h" // On utilise la définition unique de automate.h

// --- SUPPRIMEZ LES BLOCS TYPEDEF STRUCT ICI ---

// Gardez seulement les prototypes
int state_exists(Automate *a, char *state);
int symbol_exists(Automate *a, char symbol);
int check_initial_state(Automate *a);
int check_final_states(Automate *a);
int check_transitions(Automate *a);
int check_determinism(Automate *a);
int check_semantic(Automate *a);

#endif