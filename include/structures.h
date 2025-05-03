#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <time.h>

// Structure pour les utilisateurs
typedef struct {
    int id;
    char username[50];
    char password[255];
    char email[100];
    char full_name[100];
    char role[20]; // 'admin' ou 'user'
    time_t created_at;
} User;

// Structure pour les documents (thèses/mémoires)
typedef struct {
    int id;
    char title[255];
    char author[100];
    char author_email[100];
    char document_type[20]; // 'these' ou 'memoire'
    char faculty[100];
    char keywords[500];
    char abstract[2000];
    char file_path[255];
    char publication_date[11]; // Format YYYY-MM-DD
    time_t created_at;
    int created_by;
} Document;

// Structure pour les critères de recherche
typedef struct {
    char title[255];
    char author[100];
    char keywords[500];
    char document_type[20];
    char faculty[100];
    char date_from[11];
    char date_to[11];
} SearchCriteria;

// Énumération des types d'utilisateurs
typedef enum {
    USER_ROLE_ADMIN,
    USER_ROLE_USER
} UserRole;

#endif /* STRUCTURES_H */ 