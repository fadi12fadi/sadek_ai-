-- Base de données pour la Plateforme de Gestion des Thèses et Mémoires

-- Table des utilisateurs
CREATE TABLE users (
    user_id INTEGER PRIMARY KEY AUTOINCREMENT,
    username VARCHAR(50) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL, -- Stockage sécurisé du mot de passe (hasher)
    email VARCHAR(100) NOT NULL UNIQUE,
    full_name VARCHAR(100) NOT NULL,
    role VARCHAR(20) NOT NULL, -- 'admin' ou 'user'
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Table des thèses/mémoires
CREATE TABLE documents (
    doc_id INTEGER PRIMARY KEY AUTOINCREMENT,
    title VARCHAR(255) NOT NULL,
    author VARCHAR(100) NOT NULL,
    author_email VARCHAR(100),
    document_type VARCHAR(20) NOT NULL, -- 'these' ou 'memoire'
    faculty VARCHAR(100) NOT NULL,
    keywords TEXT NOT NULL,
    abstract TEXT,
    file_path VARCHAR(255), -- Chemin vers le fichier PDF
    publication_date DATE NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    created_by INTEGER,
    FOREIGN KEY (created_by) REFERENCES users(user_id)
);

-- Table des favoris
CREATE TABLE favorites (
    favorite_id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER NOT NULL,
    doc_id INTEGER NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (user_id) REFERENCES users(user_id),
    FOREIGN KEY (doc_id) REFERENCES documents(doc_id),
    UNIQUE(user_id, doc_id)
);

-- Insertions de données initiales (admin par défaut)
INSERT INTO users (username, password, email, full_name, role)
VALUES ('admin', 'admin_password_hash', 'admin@example.com', 'Administrateur', 'admin');

-- Index pour améliorer les performances des recherches
CREATE INDEX idx_doc_title ON documents(title);
CREATE INDEX idx_doc_author ON documents(author);
CREATE INDEX idx_doc_keywords ON documents(keywords);
CREATE INDEX idx_doc_type ON documents(document_type);
CREATE INDEX idx_doc_faculty ON documents(faculty); 