#include <iostream>
#include <vector>
#include <string>

using namespace std;

// 1. IMPLEMENTASI STRUKTUR DATA (Target Minggu 7)
struct Kategori {
    int id_kategori;
    string nama;
    int parent_id;
    int level;
    
    // Vector berisi pointer untuk menunjuk ke anak-anaknya secara dinamis
    vector<Kategori*> sub_kategori; 

    // Konstruktor
    Kategori(int id, string n, int pid, int lvl) {
        id_kategori = id;
        nama = n;
        parent_id = pid;
        level = lvl;
    }
};

// Vektor global untuk menyimpan kategori utama (Root / Level 1)
vector<Kategori*> root_kategori;


// Menggunakan konsep DFS (Depth-First Search) dari Materi Minggu 4 & 5
Kategori* cariKategori(vector<Kategori*>& list_kat, int id_cari) {
    for (Kategori* kat : list_kat) {
        if (kat->id_kategori == id_cari) {
            return kat; // Ketemu di level ini
        }
        // Jika tidak ketemu, masuk dan cari ke dalam sub-kategorinya secara rekursif
        Kategori* hasil_anak = cariKategori(kat->sub_kategori, id_cari);
        if (hasil_anak != nullptr) {
            return hasil_anak;
        }
    }
    return nullptr; // Benar-benar tidak ketemu
}


// 3. FUNGSI INSERT KATEGORI (Target Minggu 7)
void tambahKategori(int id, string nama, int parent_id, int level) {
    // Alokasi memori dinamis menggunakan pointer
    Kategori* kategori_baru = new Kategori(id, nama, parent_id, level);

    if (parent_id == 0) {
        // Jika parent_id 0, berarti ini Kategori Utama (Root)
        root_kategori.push_back(kategori_baru);
        cout << "[+] Berhasil menambah kategori utama: " << nama << endl;
    } else {
        // Cari dulu parent-nya ada di mana
        Kategori* parent = cariKategori(root_kategori, parent_id);
        if (parent != nullptr) {
            parent->sub_kategori.push_back(kategori_baru);
            cout << "[+] Berhasil menambah sub-kategori: " << nama << " (di bawah " << parent->nama << ")" << endl;
        } else {
            cout << "[-] Gagal! Parent ID " << parent_id << " tidak ditemukan." << endl;
        }
    }
}


// 4. FUNGSI MENAMPILKAN HIERARKI (Menggunakan Rekursif)
void tampilkanHierarki(const vector<Kategori*>& list_kat, string indentasi = "") {
    for (Kategori* kat : list_kat) {
        cout << indentasi << "|-- [" << kat->id_kategori << "] " << kat->nama << endl;
        tampilkanHierarki(kat->sub_kategori, indentasi + "    ");
    }
}


int main() {
    cout << "=== PENGUJIAN AWAL SISTEM KATEGORI ===" << endl;
    
    // Uji Coba Insert
    tambahKategori(1, "Elektronik", 0, 1);
    tambahKategori(2, "Pakaian", 0, 1);
    tambahKategori(101, "Handphone", 1, 2);
    tambahKategori(102, "Laptop", 1, 2);
    tambahKategori(201, "Kemeja", 2, 2);
    tambahKategori(1011, "Android", 101, 3); 

    cout << "\n=== STRUKTUR HIERARKI SAAT INI ===" << endl;
    tampilkanHierarki(root_kategori);

    cout << "\n=== UJI PENCARIAN KATEGORI ===" << endl;
    int id_dicari = 101;
    Kategori* hasil = cariKategori(root_kategori, id_dicari);
    
    if (hasil != nullptr) {
        cout << "Ketemu! Nama Kategori: " << hasil->nama << " (Level " << hasil->level << ")" << endl;
    } else {
        cout << "Kategori tidak ditemukan." << endl;
    }

    return 0;
}