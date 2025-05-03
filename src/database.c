#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <time.h>
#include "../include/database.h"
#include "../include/structures.h"

// Fonction pour se connecter à la base de données
bool db_connect(Database *db, const char *db_path) {
    int rc = sqlite3_open(db_path, &db->db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur lors de l'ouverture de la base de données: %s\n", sqlite3_errmsg(db->db));
        sqlite3_close(db->db);
        return false;
    }
    return true;
}

// Fonction pour fermer la connexion à la base de données
void db_disconnect(Database *db) {
    if (db->db != NULL) {
        sqlite3_close(db->db);
        db->db = NULL;
    }
}

// Callback pour récupérer un seul utilisateur
static int user_callback(void *data, int argc, char **argv, char **azColName) {
    User *user = (User*)data;
    
    for (int i = 0; i < argc; i++) {
        if (strcmp(azColName[i], "user_id") == 0) {
            user->id = atoi(argv[i]);
        } else if (strcmp(azColName[i], "username") == 0) {
            strncpy(user->username, argv[i], sizeof(user->username) - 1);
        } else if (strcmp(azColName[i], "password") == 0) {
            strncpy(user->password, argv[i], sizeof(user->password) - 1);
        } else if (strcmp(azColName[i], "email") == 0) {
            strncpy(user->email, argv[i], sizeof(user->email) - 1);
        } else if (strcmp(azColName[i], "full_name") == 0) {
            strncpy(user->full_name, argv[i], sizeof(user->full_name) - 1);
        } else if (strcmp(azColName[i], "role") == 0) {
            strncpy(user->role, argv[i], sizeof(user->role) - 1);
        } else if (strcmp(azColName[i], "created_at") == 0) {
            // Conversion de la chaîne de date en time_t si nécessaire
            struct tm tm;
            strptime(argv[i], "%Y-%m-%d %H:%M:%S", &tm);
            user->created_at = mktime(&tm);
        }
    }
    
    return 0;
}

// Fonction pour ajouter un utilisateur
bool db_add_user(Database *db, User *user) {
    char *sql = "INSERT INTO users (username, password, email, full_name, role) "
                "VALUES (?, ?, ?, ?, ?);";
    
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db->db, sql, -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur de préparation SQL: %s\n", sqlite3_errmsg(db->db));
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, user->username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, user->password, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, user->email, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, user->full_name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, user->role, -1, SQLITE_STATIC);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Erreur d'insertion: %s\n", sqlite3_errmsg(db->db));
        return false;
    }
    
    user->id = (int)sqlite3_last_insert_rowid(db->db);
    return true;
}

// Fonction pour valider les informations de connexion
bool db_validate_login(Database *db, const char *username, const char *password, User *user) {
    char sql[512];
    snprintf(sql, sizeof(sql), 
             "SELECT user_id, username, password, email, full_name, role, created_at "
             "FROM users WHERE username = '%s' AND password = '%s';", 
             username, password);
    
    int rc = sqlite3_exec(db->db, sql, user_callback, user, &db->error_message);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur SQL: %s\n", db->error_message);
        sqlite3_free(db->error_message);
        return false;
    }
    
    // Vérifier si un utilisateur a été trouvé
    return user->id > 0;
}

// Callback pour récupérer un document
static int document_callback(void *data, int argc, char **argv, char **azColName) {
    Document *doc = (Document*)data;
    
    for (int i = 0; i < argc; i++) {
        if (strcmp(azColName[i], "doc_id") == 0) {
            doc->id = atoi(argv[i]);
        } else if (strcmp(azColName[i], "title") == 0) {
            strncpy(doc->title, argv[i], sizeof(doc->title) - 1);
        } else if (strcmp(azColName[i], "author") == 0) {
            strncpy(doc->author, argv[i], sizeof(doc->author) - 1);
        } else if (strcmp(azColName[i], "author_email") == 0 && argv[i]) {
            strncpy(doc->author_email, argv[i], sizeof(doc->author_email) - 1);
        } else if (strcmp(azColName[i], "document_type") == 0) {
            strncpy(doc->document_type, argv[i], sizeof(doc->document_type) - 1);
        } else if (strcmp(azColName[i], "faculty") == 0) {
            strncpy(doc->faculty, argv[i], sizeof(doc->faculty) - 1);
        } else if (strcmp(azColName[i], "keywords") == 0) {
            strncpy(doc->keywords, argv[i], sizeof(doc->keywords) - 1);
        } else if (strcmp(azColName[i], "abstract") == 0 && argv[i]) {
            strncpy(doc->abstract, argv[i], sizeof(doc->abstract) - 1);
        } else if (strcmp(azColName[i], "file_path") == 0 && argv[i]) {
            strncpy(doc->file_path, argv[i], sizeof(doc->file_path) - 1);
        } else if (strcmp(azColName[i], "publication_date") == 0) {
            strncpy(doc->publication_date, argv[i], sizeof(doc->publication_date) - 1);
        } else if (strcmp(azColName[i], "created_by") == 0 && argv[i]) {
            doc->created_by = atoi(argv[i]);
        } else if (strcmp(azColName[i], "created_at") == 0 && argv[i]) {
            struct tm tm;
            strptime(argv[i], "%Y-%m-%d %H:%M:%S", &tm);
            doc->created_at = mktime(&tm);
        }
    }
    
    return 0;
}

// Fonction pour ajouter un document
bool db_add_document(Database *db, Document *doc) {
    char *sql = "INSERT INTO documents (title, author, author_email, document_type, faculty, "
                "keywords, abstract, file_path, publication_date, created_by) "
                "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
    
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db->db, sql, -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur de préparation SQL: %s\n", sqlite3_errmsg(db->db));
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, doc->title, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, doc->author, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, doc->author_email, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, doc->document_type, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, doc->faculty, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, doc->keywords, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, doc->abstract, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 8, doc->file_path, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 9, doc->publication_date, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 10, doc->created_by);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Erreur d'insertion: %s\n", sqlite3_errmsg(db->db));
        return false;
    }
    
    doc->id = (int)sqlite3_last_insert_rowid(db->db);
    return true;
}

// Fonction pour récupérer un document par son ID
bool db_get_document_by_id(Database *db, int doc_id, Document *doc) {
    char sql[256];
    snprintf(sql, sizeof(sql), 
             "SELECT * FROM documents WHERE doc_id = %d;", 
             doc_id);
    
    int rc = sqlite3_exec(db->db, sql, document_callback, doc, &db->error_message);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur SQL: %s\n", db->error_message);
        sqlite3_free(db->error_message);
        return false;
    }
    
    return doc->id > 0;
}

// Structure pour stocker les résultats de recherche
typedef struct {
    Document *docs;
    int capacity;
    int count;
} DocumentResult;

// Callback pour la recherche de documents
static int search_callback(void *data, int argc, char **argv, char **azColName) {
    DocumentResult *result = (DocumentResult*)data;
    
    if (result->count >= result->capacity) {
        // Augmenter la capacité si nécessaire
        result->capacity *= 2;
        Document *new_docs = (Document*)realloc(result->docs, result->capacity * sizeof(Document));
        if (!new_docs) {
            fprintf(stderr, "Erreur d'allocation mémoire\n");
            return 1;
        }
        result->docs = new_docs;
    }
    
    Document *doc = &result->docs[result->count];
    memset(doc, 0, sizeof(Document));
    
    for (int i = 0; i < argc; i++) {
        if (strcmp(azColName[i], "doc_id") == 0) {
            doc->id = atoi(argv[i]);
        } else if (strcmp(azColName[i], "title") == 0) {
            strncpy(doc->title, argv[i], sizeof(doc->title) - 1);
        } else if (strcmp(azColName[i], "author") == 0) {
            strncpy(doc->author, argv[i], sizeof(doc->author) - 1);
        } else if (strcmp(azColName[i], "author_email") == 0 && argv[i]) {
            strncpy(doc->author_email, argv[i], sizeof(doc->author_email) - 1);
        } else if (strcmp(azColName[i], "document_type") == 0) {
            strncpy(doc->document_type, argv[i], sizeof(doc->document_type) - 1);
        } else if (strcmp(azColName[i], "faculty") == 0) {
            strncpy(doc->faculty, argv[i], sizeof(doc->faculty) - 1);
        } else if (strcmp(azColName[i], "keywords") == 0) {
            strncpy(doc->keywords, argv[i], sizeof(doc->keywords) - 1);
        } else if (strcmp(azColName[i], "abstract") == 0 && argv[i]) {
            strncpy(doc->abstract, argv[i], sizeof(doc->abstract) - 1);
        } else if (strcmp(azColName[i], "file_path") == 0 && argv[i]) {
            strncpy(doc->file_path, argv[i], sizeof(doc->file_path) - 1);
        } else if (strcmp(azColName[i], "publication_date") == 0) {
            strncpy(doc->publication_date, argv[i], sizeof(doc->publication_date) - 1);
        } else if (strcmp(azColName[i], "created_by") == 0 && argv[i]) {
            doc->created_by = atoi(argv[i]);
        } else if (strcmp(azColName[i], "created_at") == 0 && argv[i]) {
            struct tm tm;
            strptime(argv[i], "%Y-%m-%d %H:%M:%S", &tm);
            doc->created_at = mktime(&tm);
        }
    }
    
    result->count++;
    return 0;
}

// Fonction pour rechercher des documents selon des critères
bool db_search_documents(Database *db, SearchCriteria *criteria, Document **results, int *count) {
    char sql[1024] = "SELECT * FROM documents WHERE 1=1";
    
    // Ajouter les critères de recherche
    if (strlen(criteria->title) > 0) {
        char title_clause[200];
        snprintf(title_clause, sizeof(title_clause), " AND title LIKE '%%%s%%'", criteria->title);
        strcat(sql, title_clause);
    }
    
    if (strlen(criteria->author) > 0) {
        char author_clause[200];
        snprintf(author_clause, sizeof(author_clause), " AND author LIKE '%%%s%%'", criteria->author);
        strcat(sql, author_clause);
    }
    
    if (strlen(criteria->keywords) > 0) {
        char keywords_clause[200];
        snprintf(keywords_clause, sizeof(keywords_clause), " AND keywords LIKE '%%%s%%'", criteria->keywords);
        strcat(sql, keywords_clause);
    }
    
    if (strlen(criteria->document_type) > 0) {
        char type_clause[200];
        snprintf(type_clause, sizeof(type_clause), " AND document_type = '%s'", criteria->document_type);
        strcat(sql, type_clause);
    }
    
    if (strlen(criteria->faculty) > 0) {
        char faculty_clause[200];
        snprintf(faculty_clause, sizeof(faculty_clause), " AND faculty = '%s'", criteria->faculty);
        strcat(sql, faculty_clause);
    }
    
    if (strlen(criteria->date_from) > 0) {
        char date_from_clause[200];
        snprintf(date_from_clause, sizeof(date_from_clause), " AND publication_date >= '%s'", criteria->date_from);
        strcat(sql, date_from_clause);
    }
    
    if (strlen(criteria->date_to) > 0) {
        char date_to_clause[200];
        snprintf(date_to_clause, sizeof(date_to_clause), " AND publication_date <= '%s'", criteria->date_to);
        strcat(sql, date_to_clause);
    }
    
    strcat(sql, ";");
    
    // Initialiser la structure de résultat
    DocumentResult result;
    result.capacity = 10;
    result.count = 0;
    result.docs = (Document*)malloc(result.capacity * sizeof(Document));
    
    if (!result.docs) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        return false;
    }
    
    int rc = sqlite3_exec(db->db, sql, search_callback, &result, &db->error_message);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur SQL: %s\n", db->error_message);
        sqlite3_free(db->error_message);
        free(result.docs);
        return false;
    }
    
    *results = result.docs;
    *count = result.count;
    
    return true;
}

// Fonction pour supprimer un document
bool db_delete_document(Database *db, int doc_id) {
    char sql[100];
    snprintf(sql, sizeof(sql), "DELETE FROM documents WHERE doc_id = %d;", doc_id);
    
    int rc = sqlite3_exec(db->db, sql, NULL, 0, &db->error_message);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur SQL: %s\n", db->error_message);
        sqlite3_free(db->error_message);
        return false;
    }
    
    return true;
}

// Fonction pour ajouter un document aux favoris
bool db_add_favorite(Database *db, int user_id, int doc_id) {
    char *sql = "INSERT OR IGNORE INTO favorites (user_id, doc_id) VALUES (?, ?);";
    
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db->db, sql, -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur de préparation SQL: %s\n", sqlite3_errmsg(db->db));
        return false;
    }
    
    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, doc_id);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Erreur d'insertion: %s\n", sqlite3_errmsg(db->db));
        return false;
    }
    
    return true;
}

// Fonction pour obtenir les favoris d'un utilisateur
bool db_get_user_favorites(Database *db, int user_id, Document **results, int *count) {
    char sql[256];
    snprintf(sql, sizeof(sql), 
             "SELECT d.* FROM documents d "
             "JOIN favorites f ON d.doc_id = f.doc_id "
             "WHERE f.user_id = %d;", 
             user_id);
    
    // Initialiser la structure de résultat
    DocumentResult result;
    result.capacity = 10;
    result.count = 0;
    result.docs = (Document*)malloc(result.capacity * sizeof(Document));
    
    if (!result.docs) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        return false;
    }
    
    int rc = sqlite3_exec(db->db, sql, search_callback, &result, &db->error_message);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erreur SQL: %s\n", db->error_message);
        sqlite3_free(db->error_message);
        free(result.docs);
        return false;
    }
    
    *results = result.docs;
    *count = result.count;
    
    return true;
} 