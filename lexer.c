#include "lexer.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>




//------------------------------------------------------------------------------ 
// VARIABLES GLOBALES
//------------------------------------------------------------------------------ 
FILE *fichier;          // fichier source contenant la définition de l'automate
char car_cour;          // caractère courant lu
TSym_Cour SYM_COUR;     // symbole courant



//------------------------------------------------------------------------------ 
// MODE DE LECTURE DU LEXER (pour distinguer contexte états/symboles)
//------------------------------------------------------------------------------ 
typedef enum {
    MODE_NORMAL,        // lecture normale
    MODE_ETATS,         // après "etats =", "initial =", "final =", ou avant "->"
    MODE_ALPHABET       // après "alphabet =", ou après ":" dans transitions
} MODE_LECTURE;

MODE_LECTURE mode_courant = MODE_NORMAL;



//------------------------------------------------------------------------------ 
// FONCTION POUR AFFICHER LE NOM DU TOKEN(utile pour notre main)
//------------------------------------------------------------------------------ 
const char* afficherToken(CODES_LEX code) {
    switch (code) {
        case ETATS_TOKEN: return "ETATS_TOKEN";
        case ALPHABET_TOKEN: return "ALPHABET_TOKEN";
        case INITIAL_TOKEN: return "INITIAL_TOKEN";
        case FINAL_TOKEN: return "FINAL_TOKEN";
        case TRANSITIONS_TOKEN: return "TRANSITIONS_TOKEN";
        case ID_ETAT_TOKEN: return "ID_ETAT_TOKEN";
        case ID_SYMBOLE_TOKEN: return "ID_SYMBOLE_TOKEN";
        case EGAL_TOKEN: return "EGAL_TOKEN";
        case VIRGULE_TOKEN: return "VIRGULE_TOKEN";
        case POINTVIRGULE_TOKEN: return "POINTVIRGULE_TOKEN";
        case ACCOLADE_OUV_TOKEN: return "ACCOLADE_OUV_TOKEN";
        case ACCOLADE_FERM_TOKEN: return "ACCOLADE_FERM_TOKEN";
        case FLECHE_TOKEN: return "FLECHE_TOKEN";
        case DEUXPOINTS_TOKEN: return "DEUXPOINTS_TOKEN";
        case FIN_TOKEN: return "FIN_TOKEN";
        case ERREUR_TOKEN: return "ERREUR_TOKEN";
        default: return "UNKNOWN_TOKEN";
    }
}




//------------------------------------------------------------------------------ 
// FONCTION POUR LIRE LE CARACTERE PROCHAIN DEPUIS LE FICHIER
//------------------------------------------------------------------------------ 
void Lire_car() {
    car_cour = fgetc(fichier);
}




//------------------------------------------------------------------------------ 
// FONCTION POUR PASSER LES SEPARATEURS (ESPACES, TABS, RETOURS A LA LIGNE)
//------------------------------------------------------------------------------ 
void Passer_Separateurs() {
    // Passer tous les espaces blancs
    while (car_cour == ' ' || car_cour == '\t' || car_cour == '\n' || car_cour == '\r') {
        Lire_car();
    }
    
    //Gestion des commentaires style C (// jusqu'à la fin de ligne)
    if (car_cour == '/') {
        char next = fgetc(fichier);
        if (next == '/') {
            // Commentaire de ligne : lire jusqu'au retour à la ligne
            while (car_cour != '\n' && car_cour != EOF) {
                Lire_car();
            }
            // Récursion pour gérer les espaces après le commentaire
            Passer_Separateurs();
        } else {
            // Ce n'est pas un commentaire, remettre le caractère
            ungetc(next, fichier);
        }
    }
}




//------------------------------------------------------------------------------ 
// FONCTION POUR LIRE UN MOT (IDENTIFICATEUR OU MOT-CLE)
//------------------------------------------------------------------------------ 
void lire_mot() {
    char mot[50];
    int i = 0;

    // Lire tous les caractères alphanumériques et underscore
    while (isalnum(car_cour) || car_cour == '_') {
        mot[i++] = car_cour;
        Lire_car();
    }
    mot[i] = '\0';   // fin de la chaîne
    strcpy(SYM_COUR.NOM, mot);  // copier le lexème


    // Vérifier si c'est un mot-clé du langage
    if (strcasecmp(mot, "etats") == 0) {
        SYM_COUR.CODE = ETATS_TOKEN;
        mode_courant = MODE_ETATS;  // Après "etats", on attend des états
    }
    else if (strcasecmp(mot, "alphabet") == 0) {
        SYM_COUR.CODE = ALPHABET_TOKEN;
        mode_courant = MODE_ALPHABET;  // Après "alphabet", on attend des symboles
    }
    else if (strcasecmp(mot, "initial") == 0) {
        SYM_COUR.CODE = INITIAL_TOKEN;
        mode_courant = MODE_ETATS;  // Après "initial", on attend un état
    }
    else if (strcasecmp(mot, "final") == 0) {
        SYM_COUR.CODE = FINAL_TOKEN;
        mode_courant = MODE_ETATS;  // Après "final", on attend des états
    }
    else if (strcasecmp(mot, "transitions") == 0) {
        SYM_COUR.CODE = TRANSITIONS_TOKEN;
        mode_courant = MODE_ETATS;  // Dans transitions, on commence par des états
    }
    else {
        // Ce n'est pas un mot-clé, c'est un identificateur
        // On détermine le type selon le contexte et la longueur
        
        if (mode_courant == MODE_ALPHABET) {
            // Dans le contexte alphabet : doit être un seul caractère
            if (strlen(mot) == 1) {
                SYM_COUR.CODE = ID_SYMBOLE_TOKEN;
            } else {
                // Plus d'un caractère dans le contexte alphabet = ERREUR
                SYM_COUR.CODE = ERREUR_TOKEN;
            }
        }
        else {
            // Dans le contexte états (ou normal) : identificateur libre
            SYM_COUR.CODE = ID_ETAT_TOKEN;
        }
    }
}



//------------------------------------------------------------------------------ 
// FONCTION POUR LIRE LES SYMBOLES SPECIAUX  (= , ; { } -> :)
//------------------------------------------------------------------------------ 
void lire_special() {
    switch (car_cour) {
        case '=':
            SYM_COUR.CODE = EGAL_TOKEN;
            strcpy(SYM_COUR.NOM, "=");
            Lire_car();
            break;
            
        case ',':
            SYM_COUR.CODE = VIRGULE_TOKEN;
            strcpy(SYM_COUR.NOM, ",");
            Lire_car();
            // Après une virgule, on reste dans le même mode (états ou alphabet)
            break;
            
        case ';':
            SYM_COUR.CODE = POINTVIRGULE_TOKEN;
            strcpy(SYM_COUR.NOM, ";");
            Lire_car();
            // Après un point-virgule, retour au mode normal
            mode_courant = MODE_NORMAL;
            break;
            
        case '{':
            SYM_COUR.CODE = ACCOLADE_OUV_TOKEN;
            strcpy(SYM_COUR.NOM, "{");
            Lire_car();
            break;
            
        case '}':
            SYM_COUR.CODE = ACCOLADE_FERM_TOKEN;
            strcpy(SYM_COUR.NOM, "}");
            Lire_car();
            mode_courant = MODE_NORMAL;
            break;
            
        case ':':
            SYM_COUR.CODE = DEUXPOINTS_TOKEN;
            strcpy(SYM_COUR.NOM, ":");
            Lire_car();
            // Après ':', on attend des symboles dans les transitions
            mode_courant = MODE_ALPHABET;
            break;
            
        case '-':
            // Vérifier si c'est une flèche "->"
            Lire_car();
            if (car_cour == '>') {
                SYM_COUR.CODE = FLECHE_TOKEN;
                strcpy(SYM_COUR.NOM, "->");
                Lire_car();
                // Après "->", on attend un état
                mode_courant = MODE_ETATS;
            } else {
                // Si ce n'est pas "->", c'est une erreur
                SYM_COUR.CODE = ERREUR_TOKEN;
                strcpy(SYM_COUR.NOM, "-");
            }
            break;
            
        case EOF:
            SYM_COUR.CODE = FIN_TOKEN;
            strcpy(SYM_COUR.NOM, "EOF");
            break;
            
        default:
            // Caractère non reconnu = erreur lexicale
            SYM_COUR.CODE = ERREUR_TOKEN;
            SYM_COUR.NOM[0] = car_cour;
            SYM_COUR.NOM[1] = '\0';
            Lire_car();
            break;
    }
}




//------------------------------------------------------------------------------ 
// FONCTION PRINCIPALE : LIRE LE SYMBOLE SUIVANT
//------------------------------------------------------------------------------ 
void Sym_Suiv() {
    // Passer les espaces et commentaires
    Passer_Separateurs();

    // Déterminer le type de token selon le caractère courant
    if (isalpha(car_cour) || car_cour == '_') {
        // Si c'est une lettre ou underscore : lire un mot
        lire_mot();
    }
    else if (car_cour == EOF) {
        // Fin de fichier
        SYM_COUR.CODE = FIN_TOKEN;
        strcpy(SYM_COUR.NOM, "EOF");
    }
    else {
        // Sinon, c'est un symbole spécial
        lire_special();
    }
}


//------------------------------------------------------------------------------ 
// Initialiser le lexer avec un fichier 
//------------------------------------------------------------------------------ 
void Init_Lexer(const char* nomFichier){
    fichier=fopen(nomFichier,"r");
    if(!fichier){ printf("Erreur ouverture %s\n",nomFichier); return; }
    Lire_car();
}

