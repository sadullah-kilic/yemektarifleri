#include <iostream>
#include <string>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

using namespace std;

// -------------------- Fonksiyonlar --------------------

// Tarifleri listele
void listele(sql::Connection* con) {
    try {
        sql::Statement* stmt = con->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM tarif");
        cout << "\n--- Tarifler ---\n";
        while (res->next()) {
            int tarif_id = res->getInt("tarif_id");
            cout << tarif_id << ". " << res->getString("ad") 
                 << " (" << res->getString("kategori") << ")";
            cout << " [Hazırlama: " << res->getInt("hazirlama_suresi") << "dk, ";
            cout << "Pişirme: " << res->getInt("pisirme_suresi") << "dk]\n";
            cout << "Açıklama: " << res->getString("aciklama") << "\n";

            // Malzemeleri listele
            sql::PreparedStatement* pstmt = con->prepareStatement(
                "SELECT m.isim, tm.miktar FROM tarif_malzeme tm "
                "JOIN malzeme m ON tm.malzeme_id = m.malzeme_id "
                "WHERE tm.tarif_id = ?");
            pstmt->setInt(1, tarif_id);
            sql::ResultSet* malzemeRes = pstmt->executeQuery();
            cout << "Malzemeler: ";
            while (malzemeRes->next()) {
                cout << malzemeRes->getString("isim") << "(" 
                     << malzemeRes->getString("miktar") << "), ";
            }
            cout << "\n--------------------\n";
            delete malzemeRes;
            delete pstmt;
        }
        delete res;
        delete stmt;
    } catch(sql::SQLException& e) {
        cout << "SQL Hatası: " << e.what() << endl;
    }
}

// Yeni tarif ekle
void ekle(sql::Connection* con) {
    string ad, kategori, aciklama;
    int hazir, pisirme;
    cin.ignore();
    cout << "Tarif adı: "; getline(cin, ad);
    cout << "Kategori: "; getline(cin, kategori);
    cout << "Hazırlama süresi (dk): "; cin >> hazir;
    cout << "Pişirme süresi (dk): "; cin >> pisirme;
    cin.ignore();
    cout << "Açıklama: "; getline(cin, aciklama);

    try {
        sql::PreparedStatement* pstmt = 
            con->prepareStatement("INSERT INTO tarif(ad,kategori,hazirlama_suresi,pisirme_suresi,aciklama) VALUES(?,?,?,?,?)");
        pstmt->setString(1, ad);
        pstmt->setString(2, kategori);
        pstmt->setInt(3, hazir);
        pstmt->setInt(4, pisirme);
        pstmt->setString(5, aciklama);
        pstmt->execute();
        delete pstmt;
        cout << "Tarif eklendi.\n";
    } catch(sql::SQLException& e) {
        cout << "SQL Hatası: " << e.what() << endl;
    }
}

// Tarif güncelle
void guncelle(sql::Connection* con) {
    int id;
    cout << "Güncellenecek tarif ID: "; cin >> id;
    cin.ignore();
    string ad, kategori, aciklama;
    int hazir, pisirme;
    cout << "Yeni Tarif adı: "; getline(cin, ad);
    cout << "Yeni Kategori: "; getline(cin, kategori);
    cout << "Yeni Hazırlama süresi (dk): "; cin >> hazir;
    cout << "Yeni Pişirme süresi (dk): "; cin >> pisirme;
    cin.ignore();
    cout << "Yeni Açıklama: "; getline(cin, aciklama);

    try {
        sql::PreparedStatement* pstmt = 
            con->prepareStatement("UPDATE tarif SET ad=?, kategori=?, hazirlama_suresi=?, pisirme_suresi=?, aciklama=? WHERE tarif_id=?");
        pstmt->setString(1, ad);
        pstmt->setString(2, kategori);
        pstmt->setInt(3, hazir);
        pstmt->setInt(4, pisirme);
        pstmt->setString(5, aciklama);
        pstmt->setInt(6, id);
        int updated = pstmt->executeUpdate();
        if(updated > 0) cout << "Tarif güncellendi.\n";
        else cout << "Tarif bulunamadı.\n";
        delete pstmt;
    } catch(sql::SQLException& e) {
        cout << "SQL Hatası: " << e.what() << endl;
    }
}

// Tarif sil
void sil(sql::Connection* con) {
    int id;
    cout << "Silinecek tarif ID: "; cin >> id;
    try {
        sql::PreparedStatement* pstmt = 
            con->prepareStatement("DELETE FROM tarif WHERE tarif_id=?");
        pstmt->setInt(1, id);
        int deleted = pstmt->executeUpdate();
        if(deleted > 0) cout << "Tarif silindi.\n";
        else cout << "Tarif bulunamadı.\n";
        delete pstmt;
    } catch(sql::SQLException& e) {
        cout << "SQL Hatası: " << e.what() << endl;
    }
}

// Malzeme ekle
void malzemeEkle(sql::Connection* con) {
    string isim;
    cin.ignore();
    cout << "Malzeme adı: "; getline(cin, isim);
    try {
        sql::PreparedStatement* pstmt =
            con->prepareStatement("INSERT INTO malzeme(isim) VALUES(?)");
        pstmt->setString(1, isim);
        pstmt->execute();
        delete pstmt;
        cout << "Malzeme eklendi.\n";
    } catch(sql::SQLException& e) {
        cout << "SQL Hatası: " << e.what() << endl;
    }
}

// Malzemeyi bir tarife bağla
void malzemeBagla(sql::Connection* con) {
    int tarifID, malzemeID;
    string miktar;
    cout << "Tarif ID: "; cin >> tarifID;
    cout << "Malzeme ID: "; cin >> malzemeID;
    cin.ignore();
    cout << "Miktar (ör: 200g, 2 yemek kaşığı): "; getline(cin, miktar);
    try {
        sql::PreparedStatement* pstmt =
            con->prepareStatement("INSERT INTO tarif_malzeme(tarif_id, malzeme_id, miktar) VALUES(?,?,?)");
        pstmt->setInt(1, tarifID);
        pstmt->setInt(2, malzemeID);
        pstmt->setString(3, miktar);
        pstmt->execute();
        delete pstmt;
        cout << "Malzeme tarife eklendi.\n";
    } catch(sql::SQLException& e) {
        cout << "SQL Hatası: " << e.what() << endl;
    }
}

// -------------------- Ana Program --------------------
int main() {
    try {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection* con = driver->connect("tcp://127.0.0.1:3306", "admin", "1234"); 
        
        con->setSchema("tarifdefteri");

        int secim;
        do {
            cout << "\n--- Yemek Tarif Defteri ---\n";
            cout << "1. Tarifleri Listele\n2. Yeni Tarif Ekle\n3. Tarif Güncelle\n4. Tarif Sil\n";
            cout << "5. Malzeme Ekle\n6. Malzemeyi Tarife Bağla\n0. Çıkış\nSeçiminiz: ";
            cin >> secim;

            switch(secim) {
                case 1: listele(con); break;
                case 2: ekle(con); break;
                case 3: guncelle(con); break;
                case 4: sil(con); break;
                case 5: malzemeEkle(con); break;
                case 6: malzemeBagla(con); break;
                case 0: cout << "Çıkılıyor...\n"; break;
                default: cout << "Geçersiz seçenek!\n";
            }
        } while(secim != 0);

        delete con;
    } catch(sql::SQLException& e) {
        cout << "SQL Hatası: " << e.what() << endl;
    }

    return 0;
}
