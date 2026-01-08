#==============================================================================
# MAKEFILE - COMPILATEUR D'AUTOMATES (CORRIGÉ)
#==============================================================================

CC = gcc
CFLAGS = -Wall -Wextra -g

SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
TEST_DIR = tests

#------------------------------------------------------------------------------
# FICHIERS OBJETS
#------------------------------------------------------------------------------
OBJS = $(BUILD_DIR)/lexer.o \
       $(BUILD_DIR)/parser.o \
       $(BUILD_DIR)/automate.o \
       $(BUILD_DIR)/semantic.o \
       $(BUILD_DIR)/codegen.o \
       $(BUILD_DIR)/algorithmes.o

#------------------------------------------------------------------------------
# CIBLE PRINCIPALE (CORRIGÉE)
#------------------------------------------------------------------------------
all: $(BUILD_DIR) aut
	@echo ""
	@echo "================================================================"
	@echo "           COMPILATION REUSSIE                                  "
	@echo "================================================================"
	@echo ""
	@echo "Pour consulter le guide d'utilisation :"
	@echo "   ./aut help"
	@echo ""

#------------------------------------------------------------------------------
# CRÉER LE DOSSIER BUILD
#------------------------------------------------------------------------------
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

#------------------------------------------------------------------------------
# COMPILATION DE L'EXÉCUTABLE (AVEC DÉPENDANCES EXPLICITES)
#------------------------------------------------------------------------------
aut: cli.c $(OBJS)
	@echo "Liaison de l'executable..."
	@$(CC) $(CFLAGS) -o $@ $^

#------------------------------------------------------------------------------
# COMPILATION DES FICHIERS OBJETS
#------------------------------------------------------------------------------

$(BUILD_DIR)/lexer.o: $(SRC_DIR)/lexer.c $(INC_DIR)/lexer.h
	@echo "Compilation de lexer.c..."
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/parser.o: $(SRC_DIR)/parser.c $(INC_DIR)/parser.h $(INC_DIR)/lexer.h $(INC_DIR)/automate.h
	@echo "Compilation de parser.c..."
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/automate.o: $(SRC_DIR)/automate.c $(INC_DIR)/automate.h
	@echo "Compilation de automate.c..."
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/semantic.o: $(SRC_DIR)/semantic.c $(INC_DIR)/semantic.h $(INC_DIR)/automate.h
	@echo "Compilation de semantic.c..."
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/codegen.o: $(SRC_DIR)/codegen.c $(INC_DIR)/codegen.h $(INC_DIR)/automate.h
	@echo "Compilation de codegen.c..."
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/algorithmes.o: $(SRC_DIR)/algorithmes.c $(INC_DIR)/algorithmes.h $(INC_DIR)/automate.h
	@echo "Compilation de algorithmes.c..."
	@$(CC) $(CFLAGS) -c $< -o $@

#------------------------------------------------------------------------------
# PROGRAMMES DE TEST
#------------------------------------------------------------------------------
tests: test_lexer test_parser

test_lexer: $(TEST_DIR)/main_lexer.c $(BUILD_DIR)/lexer.o $(BUILD_DIR)/automate.o
	@echo "Compilation de test_lexer..."
	@$(CC) $(CFLAGS) -o $(BUILD_DIR)/test_lexer $^

test_parser: $(TEST_DIR)/main_parser.c $(OBJS)
	@echo "Compilation de test_parser..."
	@$(CC) $(CFLAGS) -o $(BUILD_DIR)/test_parser $^

#------------------------------------------------------------------------------
# AIDE
#------------------------------------------------------------------------------
help:
	@echo ""
	@echo "================================================================"
	@echo "            MAKEFILE - COMPILATEUR D'AUTOMATES                  "
	@echo "================================================================"
	@echo ""
	@echo "COMMANDES :"
	@echo "  make              - Compile le projet"
	@echo "  make tests        - Compile les tests"
	@echo "  make help         - Affiche cette aide"
	@echo ""

.PHONY: all clean clean_all help tests test_lexer test_parser