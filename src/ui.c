#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/ui.h"
#include "../include/database.h"
#include "../include/structures.h"

// Fonction pour effacer l'écran (pour améliorer l'affichage)
static void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Fonction pour initialiser l'interface utilisateur
void ui_init() {
    // Rien de spécial à faire pour l'instant
}

// Fonction pour nettoyer l'interface utilisateur
void ui_cleanup() {
    // Rien de spécial à faire pour l'instant
}

// Fonction pour afficher le menu principal
int ui_show_main_menu() {
    int choice;
    
    clear_screen();
    printf("\n");
    printf("============================================\n");
    printf("  PLATEFORME DE GESTION DES THÈSES ET MÉMOIRES\n");
    printf("============================================\n\n");
    printf("1. Connexion\n");
    printf("2. Créer un compte\n");
    printf("0. Quitter\n\n");
    printf("Votre choix: ");
    scanf("%d", &choice);
    getchar(); // Consommer le caractère newline
    
    return choice;
}

// Fonction pour afficher le menu administrateur
int ui_show_admin_menu() {
    int choice;
    
    clear_screen();
    printf("\n");
    printf("====== MENU ADMINISTRATEUR ======\n\n");
    printf("1. Ajouter une thèse/mémoire\n");
    printf("2. Modifier une thèse/mémoire\n");
    printf("3. Supprimer une thèse/mémoire\n");
    printf("4. Gérer les utilisateurs\n");
    printf("5. Rechercher des thèses/mémoires\n");
    printf("6. Consulter mes favoris\n");
    printf("0. Déconnexion\n\n");
    printf("Votre choix: ");
    scanf("%d", &choice);
    getchar(); // Consommer le caractère newline
    
    return choice;
}

// Fonction pour afficher le menu utilisateur
int ui_show_user_menu() {
    int choice;
    
    clear_screen();
    printf("\n");
    printf("====== MENU UTILISATEUR ======\n\n");
    printf("1. Rechercher des thèses/mémoires\n");
    printf("2. Consulter mes favoris\n");
    printf("0. Déconnexion\n\n");
    printf("Votre choix: ");
    scanf("%d", &choice);
    getchar(); // Consommer le caractère newline
    
    return choice;
}

// Fonction pour la connexion
void ui_login(Database *db, User *current_user) {
    char username[50];
    char password[50];
    
    clear_screen();
    printf("\n");
    printf("====== CONNEXION ======\n\n");
    
    printf("Nom d'utilisateur: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0; // Enlever le newline
    
    printf("Mot de passe: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0; // Enlever le newline
    
    if (db_validate_login(db, username, password, current_user)) {
        printf("\nConnexion réussie! Bienvenue, %s.\n", current_user->full_name);
        printf("Appuyez sur Entrée pour continuer...");
        getchar();
    } else {
        printf("\nNom d'utilisateur ou mot de passe incorrect.\n");
        printf("Appuyez sur Entrée pour continuer...");
        getchar();
        current_user->id = 0; // Assurer que l'utilisateur n'est pas connecté
    }
}

// Fonction pour l'inscription
void ui_register(Database *db) {
    User new_user;
    memset(&new_user, 0, sizeof(User));
    
    clear_screen();
    printf("\n");
    printf("====== CRÉATION DE COMPTE ======\n\n");
    
    printf("Nom complet: ");
    fgets(new_user.full_name, sizeof(new_user.full_name), stdin);
    new_user.full_name[strcspn(new_user.full_name, "\n")] = 0;
    
    printf("Email: ");
    fgets(new_user.email, sizeof(new_user.email), stdin);
    new_user.email[strcspn(new_user.email, "\n")] = 0;
    
    printf("Nom d'utilisateur: ");
    fgets(new_user.username, sizeof(new_user.username), stdin);
    new_user.username[strcspn(new_user.username, "\n")] = 0;
    
    printf("Mot de passe: ");
    fgets(new_user.password, sizeof(new_user.password), stdin);
    new_user.password[strcspn(new_user.password, "\n")] = 0;
    
    // Par défaut, nouvel utilisateur est un utilisateur standard
    strcpy(new_user.role, "user");
    
    if (db_add_user(db, &new_user)) {
        printf("\nCompte créé avec succès!\n");
    } else {
        printf("\nErreur lors de la création du compte. Veuillez réessayer.\n");
    }
    
    printf("Appuyez sur Entrée pour continuer...");
    getchar();
}

// Fonction pour la déconnexion
void ui_logout(User *current_user) {
    // Réinitialiser l'utilisateur courant
    memset(current_user, 0, sizeof(User));
    printf("\nVous avez été déconnecté.\n");
    printf("Appuyez sur Entrée pour continuer...");
    getchar();
}

// Fonction pour ajouter un document
void ui_add_document(Database *db, User *current_user) {
    Document new_doc;
    memset(&new_doc, 0, sizeof(Document));
    
    clear_screen();
    printf("\n");
    printf("====== AJOUTER UNE THÈSE/MÉMOIRE ======\n\n");
    
    printf("Titre: ");
    fgets(new_doc.title, sizeof(new_doc.title), stdin);
    new_doc.title[strcspn(new_doc.title, "\n")] = 0;
    
    printf("Auteur: ");
    fgets(new_doc.author, sizeof(new_doc.author), stdin);
    new_doc.author[strcspn(new_doc.author, "\n")] = 0;
    
    printf("Email de l'auteur: ");
    fgets(new_doc.author_email, sizeof(new_doc.author_email), stdin);
    new_doc.author_email[strcspn(new_doc.author_email, "\n")] = 0;
    
    printf("Type de document (these/memoire): ");
    fgets(new_doc.document_type, sizeof(new_doc.document_type), stdin);
    new_doc.document_type[strcspn(new_doc.document_type, "\n")] = 0;
    
    printf("Faculté: ");
    fgets(new_doc.faculty, sizeof(new_doc.faculty), stdin);
    new_doc.faculty[strcspn(new_doc.faculty, "\n")] = 0;
    
    printf("Mots-clés (séparés par des virgules): ");
    fgets(new_doc.keywords, sizeof(new_doc.keywords), stdin);
    new_doc.keywords[strcspn(new_doc.keywords, "\n")] = 0;
    
    printf("Résumé: ");
    fgets(new_doc.abstract, sizeof(new_doc.abstract), stdin);
    new_doc.abstract[strcspn(new_doc.abstract, "\n")] = 0;
    
    printf("Chemin du fichier: ");
    fgets(new_doc.file_path, sizeof(new_doc.file_path), stdin);
    new_doc.file_path[strcspn(new_doc.file_path, "\n")] = 0;
    
    printf("Date de publication (YYYY-MM-DD): ");
    fgets(new_doc.publication_date, sizeof(new_doc.publication_date), stdin);
    new_doc.publication_date[strcspn(new_doc.publication_date, "\n")] = 0;
    
    // Définir l'utilisateur qui a créé le document
    new_doc.created_by = current_user->id;
    
    if (db_add_document(db, &new_doc)) {
        printf("\nDocument ajouté avec succès!\n");
    } else {
        printf("\nErreur lors de l'ajout du document. Veuillez réessayer.\n");
    }
    
    printf("Appuyez sur Entrée pour continuer...");
    getchar();
}

// Fonction pour supprimer un document
void ui_delete_document(Database *db) {
    int doc_id;
    Document doc;
    
    clear_screen();
    printf("\n");
    printf("====== SUPPRIMER UNE THÈSE/MÉMOIRE ======\n\n");
    
    printf("Entrez l'ID du document à supprimer: ");
    scanf("%d", &doc_id);
    getchar(); // Consommer le caractère newline
    
    // Vérifier si le document existe
    memset(&doc, 0, sizeof(Document));
    if (db_get_document_by_id(db, doc_id, &doc)) {
        // Afficher les informations du document
        ui_display_document(&doc);
        
        char confirm;
        printf("\nÊtes-vous sûr de vouloir supprimer ce document? (o/n): ");
        scanf("%c", &confirm);
        getchar(); // Consommer le caractère newline
        
        if (confirm == 'o' || confirm == 'O') {
            if (db_delete_document(db, doc_id)) {
                printf("\nDocument supprimé avec succès!\n");
            } else {
                printf("\nErreur lors de la suppression du document.\n");
            }
        } else {
            printf("\nSuppression annulée.\n");
        }
    } else {
        printf("\nDocument non trouvé.\n");
    }
    
    printf("Appuyez sur Entrée pour continuer...");
    getchar();
}

// Fonction pour afficher un document
void ui_display_document(Document *doc) {
    printf("\n");
    printf("=== Document #%d ===\n", doc->id);
    printf("Titre: %s\n", doc->title);
    printf("Auteur: %s\n", doc->author);
    printf("Email de l'auteur: %s\n", doc->author_email);
    printf("Type: %s\n", doc->document_type);
    printf("Faculté: %s\n", doc->faculty);
    printf("Mots-clés: %s\n", doc->keywords);
    printf("Date de publication: %s\n", doc->publication_date);
    
    if (strlen(doc->abstract) > 0) {
        printf("\nRésumé:\n%s\n", doc->abstract);
    }
}

// Fonction pour obtenir les critères de recherche
void ui_get_search_criteria(SearchCriteria *criteria) {
    memset(criteria, 0, sizeof(SearchCriteria));
    
    clear_screen();
    printf("\n");
    printf("====== RECHERCHE DE THÈSES/MÉMOIRES ======\n\n");
    printf("Laissez un champ vide pour ne pas l'utiliser comme critère.\n\n");
    
    printf("Titre: ");
    fgets(criteria->title, sizeof(criteria->title), stdin);
    criteria->title[strcspn(criteria->title, "\n")] = 0;
    
    printf("Auteur: ");
    fgets(criteria->author, sizeof(criteria->author), stdin);
    criteria->author[strcspn(criteria->author, "\n")] = 0;
    
    printf("Mots-clés: ");
    fgets(criteria->keywords, sizeof(criteria->keywords), stdin);
    criteria->keywords[strcspn(criteria->keywords, "\n")] = 0;
    
    printf("Type de document (these/memoire): ");
    fgets(criteria->document_type, sizeof(criteria->document_type), stdin);
    criteria->document_type[strcspn(criteria->document_type, "\n")] = 0;
    
    printf("Faculté: ");
    fgets(criteria->faculty, sizeof(criteria->faculty), stdin);
    criteria->faculty[strcspn(criteria->faculty, "\n")] = 0;
    
    printf("Date de début (YYYY-MM-DD): ");
    fgets(criteria->date_from, sizeof(criteria->date_from), stdin);
    criteria->date_from[strcspn(criteria->date_from, "\n")] = 0;
    
    printf("Date de fin (YYYY-MM-DD): ");
    fgets(criteria->date_to, sizeof(criteria->date_to), stdin);
    criteria->date_to[strcspn(criteria->date_to, "\n")] = 0;
}

// Fonction pour rechercher des documents
void ui_search_documents(Database *db, SearchCriteria *criteria) {
    Document *results = NULL;
    int count = 0;
    
    // Obtenir les critères de recherche
    ui_get_search_criteria(criteria);
    
    // Effectuer la recherche
    if (db_search_documents(db, criteria, &results, &count)) {
        clear_screen();
        printf("\n");
        printf("====== RÉSULTATS DE RECHERCHE ======\n\n");
        printf("Nombre de documents trouvés: %d\n\n", count);
        
        for (int i = 0; i < count; i++) {
            ui_display_document(&results[i]);
            printf("\n------------------------------\n");
        }
        
        if (count > 0) {
            int doc_id;
            printf("\nEntrez l'ID d'un document pour plus de détails (0 pour quitter): ");
            scanf("%d", &doc_id);
            getchar(); // Consommer le caractère newline
            
            if (doc_id > 0) {
                ui_view_document(db, doc_id);
            }
        }
        
        // Libérer la mémoire allouée pour les résultats
        free(results);
    } else {
        printf("\nErreur lors de la recherche.\n");
        printf("Appuyez sur Entrée pour continuer...");
        getchar();
    }
}

// Fonction pour consulter un document
void ui_view_document(Database *db, int doc_id) {
    Document doc;
    memset(&doc, 0, sizeof(Document));
    
    if (db_get_document_by_id(db, doc_id, &doc)) {
        clear_screen();
        ui_display_document(&doc);
        
        printf("\nOptions:\n");
        printf("1. Ajouter aux favoris\n");
        printf("2. Contacter l'auteur\n");
        printf("0. Retour\n");
        
        int choice;
        printf("\nVotre choix: ");
        scanf("%d", &choice);
        getchar(); // Consommer le caractère newline
        
        // Implémentation des actions selon le choix
    } else {
        printf("\nDocument non trouvé.\n");
    }
    
    printf("Appuyez sur Entrée pour continuer...");
    getchar();
}

// Fonction pour ajouter un document aux favoris
void ui_add_to_favorites(Database *db, User *current_user, int doc_id) {
    if (current_user->id == 0) {
        printf("\nVous devez être connecté pour ajouter un document aux favoris.\n");
    } else {
        if (db_add_favorite(db, current_user->id, doc_id)) {
            printf("\nDocument ajouté aux favoris avec succès!\n");
        } else {
            printf("\nErreur lors de l'ajout aux favoris.\n");
        }
    }
    
    printf("Appuyez sur Entrée pour continuer...");
    getchar();
}

// Fonction pour consulter les favoris
void ui_view_favorites(Database *db, User *current_user) {
    if (current_user->id == 0) {
        printf("\nVous devez être connecté pour consulter vos favoris.\n");
        printf("Appuyez sur Entrée pour continuer...");
        getchar();
        return;
    }
    
    Document *favorites = NULL;
    int count = 0;
    
    if (db_get_user_favorites(db, current_user->id, &favorites, &count)) {
        clear_screen();
        printf("\n");
        printf("====== MES FAVORIS ======\n\n");
        printf("Nombre de documents favoris: %d\n\n", count);
        
        for (int i = 0; i < count; i++) {
            ui_display_document(&favorites[i]);
            printf("\n------------------------------\n");
        }
        
        if (count > 0) {
            int doc_id;
            printf("\nEntrez l'ID d'un document pour plus de détails (0 pour quitter): ");
            scanf("%d", &doc_id);
            getchar(); // Consommer le caractère newline
            
            if (doc_id > 0) {
                ui_view_document(db, doc_id);
            }
        }
        
        // Libérer la mémoire allouée pour les favoris
        free(favorites);
    } else {
        printf("\nErreur lors de la récupération des favoris.\n");
        printf("Appuyez sur Entrée pour continuer...");
        getchar();
    }
}

// Fonction pour contacter l'auteur d'un document
void ui_contact_author(Database *db, int doc_id) {
    Document doc;
    memset(&doc, 0, sizeof(Document));
    
    if (db_get_document_by_id(db, doc_id, &doc)) {
        if (strlen(doc.author_email) > 0) {
            printf("\nPour contacter l'auteur, envoyez un email à: %s\n", doc.author_email);
        } else {
            printf("\nAucune adresse email disponible pour cet auteur.\n");
        }
    } else {
        printf("\nDocument non trouvé.\n");
    }
    
    printf("Appuyez sur Entrée pour continuer...");
    getchar();
}

// Fonction pour afficher un message d'erreur
void ui_display_error(const char *message) {
    printf("\nErreur: %s\n", message);
    printf("Appuyez sur Entrée pour continuer...");
    getchar();
}

// Fonction pour afficher un message de succès
void ui_display_success(const char *message) {
    printf("\nSuccès: %s\n", message);
    printf("Appuyez sur Entrée pour continuer...");
    getchar();
} 