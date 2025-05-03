#ifndef UI_H
#define UI_H

#include "structures.h"
#include "database.h"

// Constantes pour les menus
#define MENU_EXIT 0
#define MENU_LOGIN 1
#define MENU_REGISTER 2
#define MENU_ADMIN 3
#define MENU_USER 4

// Fonctions d'initialisation et de nettoyage de l'interface
void ui_init();
void ui_cleanup();

// Fonctions de menu principal
int ui_show_main_menu();
int ui_show_admin_menu();
int ui_show_user_menu();

// Fonctions d'authentification
void ui_login(Database *db, User *current_user);
void ui_register(Database *db);
void ui_logout(User *current_user);

// Fonctions admin
void ui_add_document(Database *db, User *current_user);
void ui_edit_document(Database *db);
void ui_delete_document(Database *db);
void ui_manage_users(Database *db);

// Fonctions utilisateur
void ui_search_documents(Database *db, SearchCriteria *criteria);
void ui_view_document(Database *db, int doc_id);
void ui_add_to_favorites(Database *db, User *current_user, int doc_id);
void ui_view_favorites(Database *db, User *current_user);
void ui_contact_author(Database *db, int doc_id);

// Fonctions utilitaires d'affichage
void ui_display_document(Document *doc);
void ui_display_user(User *user);
void ui_display_search_results(Document *results, int count);
void ui_display_error(const char *message);
void ui_display_success(const char *message);

// Fonctions pour obtenir les entrées utilisateur
void ui_get_search_criteria(SearchCriteria *criteria);
void ui_get_document_details(Document *doc);
void ui_get_user_details(User *user);

#endif /* UI_H */ 