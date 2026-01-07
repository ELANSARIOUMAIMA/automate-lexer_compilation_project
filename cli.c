#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "automate.h"
#include "semantic.h"
#include "algorithmes.h"

void afficher_aide() {
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║         COMPILATEUR D'AUTOMATES - GUIDE D'UTILISATION        ║\n");
    printf("╚═══════════════════════════════════════════════════════════════╝\n\n");
    
    printf("USAGE :\n");
    printf("  ./aut <commande> <fichier> [options]\n\n");
    
    printf("COMMANDES DISPONIBLES :\n\n");
    
    printf("   - compile <fichier>\n");
    printf("     Vérifie la syntaxe et la sémantique de l'automate\n");
    
    printf("   - afficher <fichier>\n");
    printf("     Affiche la structure complète de l'automate\n");
    
    printf("   - accepter <fichier> <chaine>\n");
    printf("     Teste si une chaîne est acceptée par l'automate\n");
    
    printf("   - determiniser <fichier> [--save <fichier_sortie>]\n");
    printf("     Déterminise l'automate\n");
    printf("     Option --save : sauvegarde le résultat dans un fichier\n");
    
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
    
    if (strcmp(commande, "help") == 0 || 
        strcmp(commande, "--help") == 0 || 
        strcmp(commande, "-h") == 0) {
        afficher_aide();
        return 0;
    }
    
    if (strcmp(commande, "compile") == 0) {
        if (argc < 3) {
            printf("❌ Erreur : Fichier manquant\n");
            printf("Usage : ./aut compile <fichier>\n\n");
            return 1;
        }
        
        char* fichier = argv[2];
        
        printf("\n╔═══════════════════════════════════════════════════════════════╗\n");
        printf("║              COMPILATION DE L'AUTOMATE                        ║\n");
        printf("╚═══════════════════════════════════════════════════════════════╝\n");
        
        Automate* aut = parser(fichier);
        
        if (!check_semantic(aut)) {
            printf("\nCOMPILATION ÉCHOUÉE\n\n");
            liberer_automate(aut);
            return 1;
        }
        printf("COMPILATION RÉUSSIE\n");
        
        liberer_automate(aut);
        return 0;
    }
    
    if (strcmp(commande, "afficher") == 0) {
        if (argc < 3) {
            printf("Erreur : Fichier manquant\n");
            return 1;
        }
        
        char* fichier = argv[2];
        
        printf("\n╔═══════════════════════════════════════════════════════════════╗\n");
        printf("║              AFFICHAGE DE L'AUTOMATE                          ║\n");
        printf("╚═══════════════════════════════════════════════════════════════╝\n");
        
        Automate* aut = parser(fichier);
        
        if (!check_semantic(aut)) {
            printf("\nImpossible d'afficher\n\n");
            liberer_automate(aut);
            return 1;
        }
        
        afficher_automate_algo(aut);
        
        liberer_automate(aut);
        return 0;
    }
    
    if (strcmp(commande, "accepter") == 0) {
        if (argc < 4) {
            printf("Erreur : Arguments manquants\n");
            return 1;
        }
        
        char* fichier = argv[2];
        char* chaine = argv[3];
        
        printf("\n╔═══════════════════════════════════════════════════════════════╗\n");
        printf("║              TEST D'ACCEPTATION DE CHAÎNE                     ║\n");
        printf("╚═══════════════════════════════════════════════════════════════╝\n");
        
        Automate* aut = parser(fichier);
        
        if (!check_semantic(aut)) {
            liberer_automate(aut);
            return 1;
        }
        
        int resultat = accepter_chaine(aut, chaine);
        
        liberer_automate(aut);
        
        return (resultat == 1) ? 0 : 1;
    }
    
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
        
        Automate* aut_det = determiniser(aut);
        
        printf("\nAUTOMATE DÉTERMINISÉ :\n\n");
        afficher_automate_algo(aut_det);
        
        if (fichier_sortie != NULL) {
            sauvegarder_automate(aut_det, fichier_sortie);
            printf("SAUVEGARDE RÉUSSIE\n");
        }
        
        // Libérer la mémoire
        if (aut_det != aut) {
            liberer_automate(aut_det);
        }
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