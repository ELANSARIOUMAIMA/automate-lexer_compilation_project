#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./include/parser.h"
#include "./include/automate.h"
#include "./include/semantic.h"
#include "./include/codegen.h"
#include "./include/algorithmes.h"

void afficher_aide() {
    printf("\n");
    printf("=================================================================\n");
    printf("          COMPILATEUR D'AUTOMATES - GUIDE D'UTILISATION          \n");
    printf("=================================================================\n\n");
    
    printf("USAGE :\n");
    printf("  ./aut <commande> <fichier> [options]\n\n");
    
    printf("COMMANDES DISPONIBLES :\n\n");
    
    printf("   - compile <fichier>\n");
    printf("     Verifie la syntaxe et la semantique de l'automate\n\n");
    
    printf("   - afficher <fichier>\n");
    printf("     Affiche la structure complete de l'automate\n\n");
    
    printf("   - accepter <fichier> <chaine>\n");
    printf("     Teste si une chaine est acceptee par l'automate\n\n");
    
    printf("   - determiniser <fichier> [--save <fichier_sortie>]\n");
    printf("     Determinise l'automate\n");
    printf("     Option --save : sauvegarde le resultat dans un fichier\n\n");
    
    printf("   - generer <fichier> [nom_sortie]\n");
    printf("     Genere un fichier C contenant la structure de donnees\n\n");
    
    printf("   - help\n");
    printf("     Affiche ce message d'aide\n\n");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Erreur : Commande manquante\n");
        printf("Utilisez './aut help' pour voir les commandes disponibles\n\n");
        return 1;
    }
    
    char* commande = argv[1];
    
    //--------------------------------------------------------------------------
    // COMMANDE : help
    //--------------------------------------------------------------------------
    if (strcmp(commande, "help") == 0 || 
        strcmp(commande, "--help") == 0 || 
        strcmp(commande, "-h") == 0) {
        afficher_aide();
        return 0;
    }
    
    //--------------------------------------------------------------------------
    // COMMANDE : compile
    //--------------------------------------------------------------------------
    if (strcmp(commande, "compile") == 0) {
        if (argc < 3) {
            printf("Erreur : Fichier manquant\n");
            printf("Usage : ./aut compile <fichier>\n\n");
            return 1;
        }
        
        char* fichier = argv[2];
        
        printf("\n=================================================================\n");
        printf("               COMPILATION DE L'AUTOMATE                         \n");
        printf("=================================================================\n");
        
        Automate* aut = parser(fichier);
        
        if (!check_semantic(aut)) {
            printf("\nCOMPILATION ECHOUEE\n\n");
            liberer_automate(aut);
            return 1;
        }
        
        printf("\nCOMPILATION REUSSIE\n");
        
        liberer_automate(aut);
        return 0;
    }
    
    //--------------------------------------------------------------------------
    // COMMANDE : afficher
    //--------------------------------------------------------------------------
    if (strcmp(commande, "afficher") == 0) {
        if (argc < 3) {
            printf("Erreur : Fichier manquant\n");
            return 1;
        }
        
        char* fichier = argv[2];
        
        Automate* aut = parser(fichier);
        
        if (!check_semantic(aut)) {
            printf("\nImpossible d'afficher\n\n");
            liberer_automate(aut);
            return 1;
        }

        printf("\n=================================================================\n");
        printf("               AFFICHAGE DE L'AUTOMATE                           \n");
        printf("=================================================================\n\n");
        
        afficher_automate_algo(aut);
        
        liberer_automate(aut);
        return 0;
    }
    
    //--------------------------------------------------------------------------
    // COMMANDE : accepter
    //--------------------------------------------------------------------------
    if (strcmp(commande, "accepter") == 0) {
        if (argc < 4) {
            printf("Erreur : Arguments manquants\n");
            return 1;
        }
        
        char* fichier = argv[2];
        char* chaine = argv[3];

        Automate* aut = parser(fichier);
        
        if (!check_semantic(aut)) {
            liberer_automate(aut);
            return 1;
        }
        
        printf("\n=================================================================\n");
        printf("               TEST D'ACCEPTATION DE CHAINE                      \n");
        printf("=================================================================\n");     

        int resultat = accepter_chaine(aut, chaine);
        
        liberer_automate(aut);
        
        return (resultat == 1) ? 0 : 1;
    }
    
    //--------------------------------------------------------------------------
    // COMMANDE : determiniser
    //--------------------------------------------------------------------------
    if (strcmp(commande, "determiniser") == 0) {
        if (argc < 3) {
            printf("Erreur : Fichier manquant\n");
            return 1;
        }
        
        char* fichier = argv[2];
        char* fichier_sortie = NULL;
        
        // Vérifier si option --save est présente
        if (argc >= 5 && strcmp(argv[3], "--save") == 0) {
            fichier_sortie = argv[4];
        }
        
        Automate* aut = parser(fichier);
        
        if (!check_semantic(aut)) {
            liberer_automate(aut);
            return 1;
        }
        
        printf("\n=================================================================\n");
        printf("               DETERMINISATION DE L'AUTOMATE                       \n");
        printf("=================================================================\n\n");

        Automate* aut_det = determiniser(aut);
        
        afficher_automate_algo(aut_det);
        
        if (fichier_sortie != NULL) {
            printf("Sauvegarder ce resultat dans '%s' ...\n",fichier_sortie);
            sauvegarder_automate(aut_det, fichier_sortie);
        }
        
        // Libérer la mémoire
        if (aut_det != aut) {
            liberer_automate(aut_det);
        }
        liberer_automate(aut);
        
        return 0;
    }
    
    //--------------------------------------------------------------------------
    // COMMANDE : generer
    //--------------------------------------------------------------------------
    if (strcmp(commande, "generer") == 0) {
        if (argc < 3) {
            printf("Erreur : Fichier manquant\n");
            printf("Usage : ./aut generer <fichier> [nom_sortie]\n\n");
            return 1;
        }
        
        char* fichier = argv[2];
        char* fichier_sortie = (argc >= 4) ? argv[3] : "automate_data.c";
        
        Automate* aut = parser(fichier);
        
        if (!check_semantic(aut)) {
            printf("\nGeneration annulee : erreurs semantiques\n\n");
            liberer_automate(aut);
            return 1;
        }
        
        printf("\n=================================================================\n");
        printf("               GÉNÉRATION DE CODE C                              \n");
        printf("=================================================================\n");
        
        // Générer le code
        generer_code(aut, fichier_sortie);
        
        printf("\nLe fichier '%s' est genere avec succes\n\n", fichier_sortie);
        
        liberer_automate(aut);
        
        return 0;
    }
    
    //--------------------------------------------------------------------------
    // COMMANDE INCONNUE
    //--------------------------------------------------------------------------
    printf("Erreur : Commande inconnue '%s'\n", commande);
    printf("Utilisez './aut help' pour voir les commandes disponibles\n\n");
    return 1;
}