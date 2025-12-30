#include "lexer.h"
#include <stdio.h>   



//------------------------------------------------------------------------------ 
// FONCTION MAIN : POINT D'ENTREE DU PROGRAMME
//------------------------------------------------------------------------------ 
int main() {
    // Ouvrir le fichier contenant la définition de l'automate
    fichier = fopen("automate.txt", "r");
    
    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier 'automate.txt'\n");
        return 1;
    }

    printf("=================================================================\n");
    printf("ANALYSE LEXICALE DE LA DEFINITION D'AUTOMATE\n");
    printf("=================================================================\n");
    printf("Règles : - États = identificateurs libres (q0, etat1, etc.)\n");
    printf("         - Symboles = un seul caractère (a, b, c, etc.)\n");
    printf("=================================================================\n\n");
    printf("%-20s -> %-25s\n", "LEXEME", "TOKEN");
    printf("-----------------------------------------------------------------\n");

    // Lire le premier caractère
    Lire_car();

    // Boucle principale : lire tous les tokens jusqu'à EOF
    while (car_cour != EOF) {
        Sym_Suiv();
        
        // Afficher le token reconnu
        printf("%-20s -> %-25s\n", SYM_COUR.NOM, afficherToken(SYM_COUR.CODE));
        
        // Si on a atteint EOF, sortir
        if (SYM_COUR.CODE == FIN_TOKEN) {
            break;
        }
    }

    fclose(fichier);
    return 0;
}