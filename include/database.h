#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <stdbool.h>
#include "structures.h"

// Structure pour la connexion à la base de données
typedef struct {
    sqlite3 *db;
    char *error_message;
} Database;

// Fonctions de gestion de la base de données
bool db_connect(Database *db, const char *db_path);
void db_disconnect(Database *db);

// Fonctions pour les utilisateurs
bool db_add_user(Database *db, User *user);
bool db_validate_login(Database *db, const char *username, const char *password, User *user);
bool db_update_user(Database *db, User *user);
bool db_delete_user(Database *db, int user_id);
bool db_get_user_by_id(Database *db, int user_id, User *user);

// Fonctions pour les documents (thèses/mémoires)
bool db_add_document(Database *db, Document *doc);
bool db_update_document(Database *db, Document *doc);
bool db_delete_document(Database *db, int doc_id);
bool db_get_document_by_id(Database *db, int doc_id, Document *doc);
bool db_search_documents(Database *db, SearchCriteria *criteria, Document **results, int *count);

// Fonctions pour les favoris
bool db_add_favorite(Database *db, int user_id, int doc_id);
bool db_remove_favorite(Database *db, int user_id, int doc_id);
bool db_get_user_favorites(Database *db, int user_id, Document **results, int *count);

#endif /* DATABASE_H */ 