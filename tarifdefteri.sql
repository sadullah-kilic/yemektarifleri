-- Veritabanı oluştur
CREATE DATABASE IF NOT EXISTS tarifdefteri;
USE tarifdefteri;

-- Tarifler tablosu
CREATE TABLE IF NOT EXISTS tarif (
    tarif_id INT AUTO_INCREMENT PRIMARY KEY,
    ad VARCHAR(100) NOT NULL,
    kategori VARCHAR(50),
    hazirlama_suresi INT,
    pisirme_suresi INT,
    aciklama TEXT
);

-- Malzemeler tablosu
CREATE TABLE IF NOT EXISTS malzeme (
    malzeme_id INT AUTO_INCREMENT PRIMARY KEY,
    isim VARCHAR(100) NOT NULL
);

-- Tarif-Malzeme ilişkisi (çoktan çoğa)
CREATE TABLE IF NOT EXISTS tarif_malzeme (
    tarif_id INT,
    malzeme_id INT,
    miktar VARCHAR(50),
    PRIMARY KEY(tarif_id, malzeme_id),
    FOREIGN KEY (tarif_id) REFERENCES tarif(tarif_id) ON DELETE CASCADE,
    FOREIGN KEY (malzeme_id) REFERENCES malzeme(malzeme_id) ON DELETE CASCADE
);

-- Örnek tarif
INSERT INTO tarif (ad, kategori, hazirlama_suresi, pisirme_suresi, aciklama)
VALUES ('Spagetti Bolonez', 'Makarna', 15, 30, 'Klasik İtalyan spagetti soslu tarif');

-- Örnek malzemeler
INSERT INTO malzeme (isim) VALUES 
('Spagetti'), ('Kıyma'), ('Domates Sosu'), ('Soğan'), ('Sarımsak'), ('Zeytinyağı');

-- Tarif-Malzeme bağlantısı
INSERT INTO tarif_malzeme (tarif_id, malzeme_id, miktar) VALUES
(1, 1, '200g'), (1, 2, '150g'), (1, 3, '100ml'), (1, 4, '1 adet'), (1, 5, '2 diş'), (1, 6, '2 yemek kaşığı');
