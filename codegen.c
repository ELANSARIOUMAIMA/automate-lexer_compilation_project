#include "codegen.h"
#include <stdio.h>
#include <string.h>

//==============================================================================
// GÉNÉRATION DE CODE
//==============================================================================

void generer_entete(FILE* f) {
    // Inclure la définition de la structure Automate
    fprintf(f, "#include \"automate.h\"\n\n");
}

void generer_structure_automate(FILE* f, Automate* aut) {
    fprintf(f, "static Automate automate_data = {\n");
    
    // SECTION 1 : GÉNÉRATION DES ÉTATS
    fprintf(f, "    .etats = {");
    for (int i = 0; i < aut->nb_etats; i++) {
        fprintf(f, "\"%s\"", aut->etats[i]);
        if (i < aut->nb_etats - 1) {
            fprintf(f, ", ");
        }
    }
    fprintf(f, "},\n");
    fprintf(f, "    .nb_etats = %d,\n\n", aut->nb_etats);
    
    // SECTION 2 : GÉNÉRATION DE L'ALPHABET
    fprintf(f, "    .alphabet = {");
    for (int i = 0; i < aut->nb_symboles; i++) {
        fprintf(f, "'%c'", aut->alphabet[i]);
        if (i < aut->nb_symboles - 1) {
            fprintf(f, ", ");
        }
    }
    fprintf(f, "},\n");
    fprintf(f, "    .nb_symboles = %d,\n\n", aut->nb_symboles);
    
    // SECTION 3 : GÉNÉRATION DE L'ÉTAT INITIAL
    fprintf(f, "    // État initial\n");
    fprintf(f, "    .etat_initial = \"%s\",\n\n", aut->etat_initial);
    
    // SECTION 4 : GÉNÉRATION DES ÉTATS FINAUX
    fprintf(f, "    // États finaux (acceptants)\n");
    fprintf(f, "    .etats_finaux = {");
    for (int i = 0; i < aut->nb_etats_finaux; i++) {
        fprintf(f, "\"%s\"", aut->etats_finaux[i]);
        
        if (i < aut->nb_etats_finaux - 1) {
            fprintf(f, ", ");
        }
    }
    fprintf(f, "},\n");
    fprintf(f, "    .nb_etats_finaux = %d,\n\n", aut->nb_etats_finaux);
    
    // SECTION 5 : GÉNÉRATION DES TRANSITIONS
    fprintf(f, "    // Table des transitions\n");
    fprintf(f, "    .transitions = {\n");
    for (int i = 0; i < aut->nb_transitions; i++) {
        Transition t = aut->transitions[i];
        fprintf(f, "        {.source = \"%s\", .destination = \"%s\", .symbole = '%c'}",
                t.source,
                t.destination,
                t.symbole);
        if (i < aut->nb_transitions - 1) {
            fprintf(f, ",\n");
        } else {
            fprintf(f, "\n");
        }
    }
    fprintf(f, "    },\n");
    fprintf(f, "    .nb_transitions = %d\n", aut->nb_transitions);
    
    fprintf(f, "};\n\n");
}


void generer_fonction_get(FILE* f) {
    fprintf(f, "Automate* get_automate() {\n");
    fprintf(f, "    return &automate_data;\n");
    fprintf(f, "}\n");
}


// FONCTION PRINCIPALE : GÉNÉRATION COMPLÈTE DU FICHIER
void generer_code(Automate* aut, const char* nom_fichier_sortie) {
    printf("\n");
    printf("================ GÉNÉRATION DE CODE ================\n");
    printf("Fichier de sortie : %s\n", nom_fichier_sortie);
    FILE* f = fopen(nom_fichier_sortie, "w");
    if (!f) {
        printf("ERREUR : Impossible de créer le fichier %s\n", nom_fichier_sortie);
        return;
    }
    
    generer_entete(f);
    generer_structure_automate(f, aut);
    generer_fonction_get(f);
    
    fclose(f);
    
    printf("Code généré avec succès !\n");
    printf("====================================================\n\n");
}