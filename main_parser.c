#include <stdio.h>
#include "parser.h"
#include "automate.h"

int main() {
    printf("\n");
    printf("**************** TEST DU PARSER - PROJET COMPILATION **************** \n");
    
    // Parser le fichier
    Automate* aut = parser("automate.txt");
    
    // Afficher l'automate construit
    afficher_automate(aut);
    
    // Liberer la memoire
    liberer_automate(aut);
    
    printf("Programme termine avec succes!\n\n");
    return 0;
}