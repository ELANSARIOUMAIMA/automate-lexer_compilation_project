#include "parser.h"
#include "lexer.h"
#include "automate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Après #include <string.h>, ajoute :
static int sections_ok[5] = {0}; // 0:etats, 1:alphabet, 2:initial, 3:final, 4:transitions
//------------------------------------------------------------------------------
// FONCTION D'ERREUR
//------------------------------------------------------------------------------
void erreur_syntaxe(const char* message) {
    printf("\n ERREUR DE SYNTAXE: %s\n", message);
    printf("Token courant: %s (code: %s)\n", SYM_COUR.NOM, afficherToken(SYM_COUR.CODE));
    exit(1);
}
void verif_sections() {
    printf("\n[VERIFICATION...]\n");
    if (!sections_ok[0]) printf("'etats' manquant\n");
    if (!sections_ok[1]) printf("'alphabet' manquant\n");
    if (!sections_ok[2]) printf("'initial' manquant\n");
    if (!sections_ok[3]) printf("'final' manquant\n");
    if (!sections_ok[4]) printf("'transitions' manquant\n");
    if (!sections_ok[0] || !sections_ok[1] || !sections_ok[2] || !sections_ok[3] || !sections_ok[4]) {
        printf("PARSING ECHOUE : Sections manquantes !\n");
        exit(1);
    }
    printf("✅ Toutes les sections OK !\n");
}

//------------------------------------------------------------------------------
// PARSER DES eTATS : etats = q0,q1,q2;
//------------------------------------------------------------------------------
void parse_etats(Automate* aut) {
    sections_ok[0] = 1;
    printf(" Parsing des etats...\n");
    
    // On a dejà lu "etats", maintenant on attend "="
    Sym_Suiv();
    if (SYM_COUR.CODE != EGAL_TOKEN) {
        erreur_syntaxe("Attendu '=' apres 'etats'");
    }
    
    // Lire les etats separes par des virgules
    Sym_Suiv();
    while (SYM_COUR.CODE == ID_ETAT_TOKEN) {
        // Ajouter l'etat à l'automate
        strcpy(aut->etats[aut->nb_etats], SYM_COUR.NOM);
        aut->nb_etats++;
        
        printf(" - Etat ajoute: %s\n", SYM_COUR.NOM);
        
        Sym_Suiv();
        
        // Si c'est une virgule, continuer
        if (SYM_COUR.CODE == VIRGULE_TOKEN) {
            Sym_Suiv();
        }
        // Si c'est un point-virgule, on a fini
        else if (SYM_COUR.CODE == POINTVIRGULE_TOKEN) {
            break;
        }
        else {
            erreur_syntaxe("Attendu ',' ou ';' apres un etat");
        }
    }
    
    if (SYM_COUR.CODE != POINTVIRGULE_TOKEN) {
        erreur_syntaxe("Attendu ';' à la fin de la liste des etats");
    }
    
    printf("   %d etats parses\n\n", aut->nb_etats);
}

//------------------------------------------------------------------------------
// PARSER DE L'ALPHABET : alphabet = a,b;
//------------------------------------------------------------------------------
void parse_alphabet(Automate* aut) {
    sections_ok[1] = 1;
    printf(" Parsing de l'alphabet...\n");
    
    Sym_Suiv();
    if (SYM_COUR.CODE != EGAL_TOKEN) {
        erreur_syntaxe("Attendu '=' apres 'alphabet'");
    }
    
    Sym_Suiv();
    while (SYM_COUR.CODE == ID_SYMBOLE_TOKEN) {
        // Ajouter le symbole à l'alphabet
        aut->alphabet[aut->taille_alphabet] = SYM_COUR.NOM[0];
        aut->taille_alphabet++;
        
        printf("-Symbole ajoute: %c\n", SYM_COUR.NOM[0]);
        
        Sym_Suiv();
        
        if (SYM_COUR.CODE == VIRGULE_TOKEN) {
            Sym_Suiv();
        }
        else if (SYM_COUR.CODE == POINTVIRGULE_TOKEN) {
            break;
        }
        else {
            erreur_syntaxe("Attendu ',' ou ';' apres un symbole");
        }
    }
    
    if (SYM_COUR.CODE != POINTVIRGULE_TOKEN) {
        erreur_syntaxe("Attendu ';' à la fin de l'alphabet");
    }
    
    printf("   %d symboles parses\n\n", aut->taille_alphabet);
}

//------------------------------------------------------------------------------
// PARSER DE L'eTAT INITIAL : initial = q0;
//------------------------------------------------------------------------------
void parse_initial(Automate* aut) {
    sections_ok[2] = 1;
    printf(" Parsing de l'etat initial...\n");
    
    Sym_Suiv();
    if (SYM_COUR.CODE != EGAL_TOKEN) {
        erreur_syntaxe("Attendu '=' apres 'initial'");
    }
    
    Sym_Suiv();
    if (SYM_COUR.CODE != ID_ETAT_TOKEN) {
        erreur_syntaxe("Attendu un nom d'etat apres 'initial ='");
    }
    
    strcpy(aut->etat_initial, SYM_COUR.NOM);
    printf("-Etat initial: %s\n", SYM_COUR.NOM);
    
    Sym_Suiv();
    if (SYM_COUR.CODE != POINTVIRGULE_TOKEN) {
        erreur_syntaxe("Attendu ';' apres l'etat initial");
    }
    
    printf("   Etat initial parse\n\n");
}

//------------------------------------------------------------------------------
// PARSER DES eTATS FINAUX : final = q2; ou final = q1,q2;
//------------------------------------------------------------------------------
void parse_final(Automate* aut) {
    sections_ok[3] = 1;
    printf(" Parsing des etats finaux...\n");
    
    Sym_Suiv();
    if (SYM_COUR.CODE != EGAL_TOKEN) {
        erreur_syntaxe("Attendu '=' apres 'final'");
    }
    
    Sym_Suiv();
    while (SYM_COUR.CODE == ID_ETAT_TOKEN) {
        strcpy(aut->etats_finaux[aut->nb_etats_finaux], SYM_COUR.NOM);
        aut->nb_etats_finaux++;
        
        printf("-Etat final ajoute: %s\n", SYM_COUR.NOM);
        
        Sym_Suiv();
        
        if (SYM_COUR.CODE == VIRGULE_TOKEN) {
            Sym_Suiv();
        }
        else if (SYM_COUR.CODE == POINTVIRGULE_TOKEN) {
            break;
        }
        else {
            erreur_syntaxe("Attendu ',' ou ';' apres un etat final");
        }
    }
    
    if (SYM_COUR.CODE != POINTVIRGULE_TOKEN) {
        erreur_syntaxe("Attendu ';' à la fin des etats finaux");
    }
    
    printf("   %d etats finaux parses\n\n", aut->nb_etats_finaux);
}

//------------------------------------------------------------------------------
// PARSER DES TRANSITIONS : transitions { q0 -> q1 : a; q1 -> q2 : a,b; }
//------------------------------------------------------------------------------
void parse_transitions(Automate* aut) {
    sections_ok[4] = 1;
    printf("Parsing des transitions...\n");
    
    Sym_Suiv();
    if (SYM_COUR.CODE != ACCOLADE_OUV_TOKEN) {
        erreur_syntaxe("Attendu '{' apres 'transitions'");
    }
    
    Sym_Suiv();
    
    // Lire toutes les transitions jusqu'à '}'
    while (SYM_COUR.CODE != ACCOLADE_FERM_TOKEN) {
        // Format: etat_source -> etat_dest : symbole;
        //     ou: etat_source -> etat_dest : symbole1,symbole2;
        
        if (SYM_COUR.CODE != ID_ETAT_TOKEN) {
            erreur_syntaxe("Attendu un etat source dans une transition");
        }
        
        char etat_src[MAX_NOM];
        strcpy(etat_src, SYM_COUR.NOM);
        
        Sym_Suiv();
        if (SYM_COUR.CODE != FLECHE_TOKEN) {
            erreur_syntaxe("Attendu '->' dans une transition");
        }
        
        Sym_Suiv();
        if (SYM_COUR.CODE != ID_ETAT_TOKEN) {
            erreur_syntaxe("Attendu un etat destination dans une transition");
        }
        
        char etat_dest[MAX_NOM];
        strcpy(etat_dest, SYM_COUR.NOM);
        
        Sym_Suiv();
        if (SYM_COUR.CODE != DEUXPOINTS_TOKEN) {
            erreur_syntaxe("Attendu ':' avant les symboles de transition");
        }
        
        // Lire les symboles (peut être plusieurs separes par des virgules)
        Sym_Suiv();
        while (SYM_COUR.CODE == ID_SYMBOLE_TOKEN) {
            // Creer une transition pour chaque symbole
            Transition t;
            strcpy(t.etat_source, etat_src);
            strcpy(t.etat_dest, etat_dest);
            t.symbole = SYM_COUR.NOM[0];
            
            aut->transitions[aut->nb_transitions] = t;
            aut->nb_transitions++;
            
            printf("-Transition: %s --%c--> %s\n", etat_src, SYM_COUR.NOM[0], etat_dest);
            
            Sym_Suiv();
            
            if (SYM_COUR.CODE == VIRGULE_TOKEN) {
                Sym_Suiv();  // continuer avec le prochain symbole
            }
            else if (SYM_COUR.CODE == POINTVIRGULE_TOKEN) {
                break;  // fin de cette transition
            }
            else {
                erreur_syntaxe("Attendu ',' ou ';' apres un symbole de transition");
            }
        }
        
        if (SYM_COUR.CODE != POINTVIRGULE_TOKEN) {
            erreur_syntaxe("Attendu ';' à la fin d'une transition");
        }
        
        Sym_Suiv();  // passer au token suivant
    }
    
    // On doit être sur '}'
    if (SYM_COUR.CODE != ACCOLADE_FERM_TOKEN) {
        erreur_syntaxe("Attendu '}' à la fin du bloc transitions");
    }
    
    printf(" %d transitions parsees\n\n", aut->nb_transitions);
}

//------------------------------------------------------------------------------
// FONCTION PRINCIPALE DU PARSER
//------------------------------------------------------------------------------
Automate* parser(const char* nomFichier) {
    printf("---------------- DEBUT DU PARSING ----------------\n");
    
    // Initialiser le lexer
    Init_Lexer(nomFichier);
    
    // Creer l'automate vide
    Automate* aut = creer_automate();
    
    // Lire le premier token
    Sym_Suiv();
    
    // Parser chaque section
    while (SYM_COUR.CODE != FIN_TOKEN) {
        switch (SYM_COUR.CODE) {
            case ETATS_TOKEN:
                parse_etats(aut);
                break;
                
            case ALPHABET_TOKEN:
                parse_alphabet(aut);
                break;
                
            case INITIAL_TOKEN:
                parse_initial(aut);
                break;
                
            case FINAL_TOKEN:
                parse_final(aut);
                break;
                
            case TRANSITIONS_TOKEN:
                parse_transitions(aut);
                break;
                
            case ERREUR_TOKEN:
                erreur_syntaxe("Token invalide detecte par le lexer");
                break;
                
            default:
                // Ignorer les symboles non pertinents (comme les ';' apres sections)
                break;
        }
        
        Sym_Suiv();  // passer au token suivant
    }
    
    printf("---------------- PARSING TERMINE ---------------- \n");
    verif_sections();
    
    return aut;
}