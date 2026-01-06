#ifndef ALGORITHMES_H
#define ALGORITHMES_H

#include "automate.h"

int accepter_chaine(Automate* aut, const char* chaine);

int est_deterministe(Automate* aut);

Automate* determiniser(Automate* aut);

void afficher_automate_algo(Automate* aut);

#endif