# Plateforme de Gestion des Thèses et Mémoires

Ce projet consiste à développer une plateforme centralisée permettant la gestion et la consultation des thèses de Doctorat et les mémoires de Master.

## Fonctionnalités
- **Pour les administrateurs :**
  - Ajouter de nouvelles thèses/mémoires
  - Supprimer des thèses/mémoires existants
  - Gérer les droits des utilisateurs
  - Gérer les comptes des utilisateurs

- **Pour les utilisateurs :**
  - Créer un compte et s'authentifier
  - Rechercher des thèses/mémoires selon divers critères
  - Consulter les informations détaillées
  - Ajouter des documents aux favoris
  - Contacter l'auteur par mail

## Structure du Projet
- `src/` - Code source
- `include/` - Fichiers d'en-tête
- `db/` - Scripts SQL pour la base de données
- `docs/` - Documentation
- `bin/` - Exécutables compilés

## Compilation
```bash
make
```

## Exécution
```bash
./bin/thesis_manager
```

## Auteur
Développé en se basant sur la proposition de projet de Sonia Zemmour. 