#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/database.h"
#include "../include/ui.h"
#include "../include/structures.h"

// Constantes
#define DB_PATH "theses.db"

// Fonction pour initialiser la base de données
bool init_database(Database *db) {
    // Ouvrir ou créer la base de données
    if (!db_connect(db, DB_PATH)) {
        printf("Erreur: Impossible de se connecter à la base de données.\n");
        return false;
    }
    
    // Exécuter le script SQL pour créer les tables si elles n'existent pas
    FILE *schema_file = fopen("db/schema.sql", "r");
    if (!schema_file) {
        printf("Erreur: Impossible d'ouvrir le fichier de schéma SQL.\n");
        db_disconnect(db);
        return false;
    }
    
    // Lire le contenu du fichier de schéma
    fseek(schema_file, 0, SEEK_END);
    long schema_size = ftell(schema_file);
    rewind(schema_file);
    
    char *schema_content = (char*)malloc(schema_size + 1);
    if (!schema_content) {
        printf("Erreur: Allocation mémoire pour le schéma SQL.\n");
        fclose(schema_file);
        db_disconnect(db);
        return false;
    }
    
    size_t read_size = fread(schema_content, 1, schema_size, schema_file);
    schema_content[read_size] = '\0';
    fclose(schema_file);
    
    // Exécuter le script SQL
    char *error_message = NULL;
    int rc = sqlite3_exec(db->db, schema_content, NULL, 0, &error_message);
    
    free(schema_content);
    
    if (rc != SQLITE_OK) {
        printf("Erreur SQL: %s\n", error_message);
        sqlite3_free(error_message);
        db_disconnect(db);
        return false;
    }
    
    return true;
}

// Fonction principale du programme
int main() {
    // Initialiser la base de données
    Database db;
    if (!init_database(&db)) {
        printf("Erreur lors de l'initialisation de la base de données. Le programme va se terminer.\n");
        return 1;
    }
    
    // Initialiser l'interface utilisateur
    ui_init();
    
    // Variables de l'utilisateur courant
    User current_user;
    memset(&current_user, 0, sizeof(User));
    
    // Variables pour les recherches
    SearchCriteria search_criteria;
    
    bool running = true;
    int choice;
    
    // Boucle principale du programme
    while (running) {
        // Si l'utilisateur n'est pas connecté
        if (current_user.id == 0) {
            choice = ui_show_main_menu();
            
            switch (choice) {
                case MENU_LOGIN:
                    ui_login(&db, &current_user);
                    break;
                    
                case MENU_REGISTER:
                    ui_register(&db);
                    break;
                    
                case MENU_EXIT:
                    running = false;
                    break;
                    
                default:
                    printf("\nOption invalide. Veuillez réessayer.\n");
                    break;
            }
        }
        // Si l'utilisateur est un administrateur
        else if (strcmp(current_user.role, "admin") == 0) {
            choice = ui_show_admin_menu();
            
            switch (choice) {
                case 1: // Ajouter une thèse/mémoire
                    ui_add_document(&db, &current_user);
                    break;
                    
                case 2: // Modifier une thèse/mémoire
                    // Fonctionnalité à implémenter
                    break;
                    
                case 3: // Supprimer une thèse/mémoire
                    ui_delete_document(&db);
                    break;
                    
                case 4: // Gérer les utilisateurs
                    // Fonctionnalité à implémenter
                    break;
                    
                case 5: // Rechercher des thèses/mémoires
                    ui_search_documents(&db, &search_criteria);
                    break;
                    
                case 6: // Consulter mes favoris
                    ui_view_favorites(&db, &current_user);
                    break;
                    
                case 0: // Déconnexion
                    ui_logout(&current_user);
                    break;
                    
                default:
                    printf("\nOption invalide. Veuillez réessayer.\n");
                    break;
            }
        }
        // Si l'utilisateur est un utilisateur standard
        else {
            choice = ui_show_user_menu();
            
            switch (choice) {
                case 1: // Rechercher des thèses/mémoires
                    ui_search_documents(&db, &search_criteria);
                    break;
                    
                case 2: // Consulter mes favoris
                    ui_view_favorites(&db, &current_user);
                    break;
                    
                case 0: // Déconnexion
                    ui_logout(&current_user);
                    break;
                    
                default:
                    printf("\nOption invalide. Veuillez réessayer.\n");
                    break;
            }
        }
    }
    
    // Nettoyer l'interface utilisateur
    ui_cleanup();
    
    // Fermer la connexion à la base de données
    db_disconnect(&db);
    
    printf("\nMerci d'avoir utilisé la Plateforme de Gestion des Thèses et Mémoires!\n");
    
    return 0;
} 