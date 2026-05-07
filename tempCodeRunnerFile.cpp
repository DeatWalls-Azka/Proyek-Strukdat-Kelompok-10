#include <iostream>
#include <vector>
#include <string>
#include <fstream>  

#include <unordered_map> // Untuk Hash Map O(1)
#include <chrono>        // Untuk menghitung waktu eksekusi (profiling)
using namespace std;

// 1. DEFINISI STRUKTUR DATA (ADT)
// Implementasi Non-Linear Linked Tree menggunakan pointer
struct Kategori {
    int id_kategori;
    string nama;
    int parent_id;
    int level;
    
    // Vector pointer untuk menyimpan anak-anak kategori secara dinamis
    vector<Kategori*> sub_kategori; 


    // Konstruktor untuk inisialisasi data kategori baru
    Kategori(int id, string n, int pid, int lvl) {
        id_kategori = id;
        nama = n;
        parent_id = pid;
        level = lvl;
    }
};

// Vektor global untuk menyimpan kategori di level paling atas (Root)
vector<Kategori*> root_kategori;

// Struktur Data 2: Hash Map untuk pencarian O(1)
 unordered_map<int, Kategori*> map_kategori;


// 2. OPERASI UTAMA (SEARCH & INSERT)


// Fungsi pencarian menggunakan algoritma Depth-First Search (DFS) rekursif
Kategori* cariKategori(vector<Kategori*>& list_kat, int id_cari) {
    for (Kategori* kat : list_kat) {
        if (kat->id_kategori == id_cari) {
            return kat; 
        }
        // Rekursi: mencari ke dalam sub-kategori jika belum ditemukan
        Kategori* hasil_anak = cariKategori(kat->sub_kategori, id_cari);
        if (hasil_anak != nullptr) {
            return hasil_anak;
        }
    }
    return nullptr; 
}
 // Fungsi pencarian menggunakan Hash Map untuk O(1)
Kategori* cariDenganHash(int id_cari) {
    if (map_kategori.find(id_cari) != map_kategori.end()) {
        return map_kategori[id_cari]; // Langsung ketemu O(1)
    }
    return nullptr;
}

// Fungsi penyisipan data kategori baru ke dalam struktur pohon
void tambahKategori(int id, string nama, int parent_id, int level) {
    Kategori* kategori_baru = new Kategori(id, nama, parent_id, level);
    // Di dalam fungsi tambahKategori, setelah "new Kategori(...)"
    map_kategori[id] = kategori_baru; 

    if (parent_id == 0) {
        // Jika parent_id 0, otomatis menjadi Root
        root_kategori.push_back(kategori_baru);
    } else {
        // Cari alamat memori parent sebelum menyisipkan anak
        Kategori* parent = cariKategori(root_kategori, parent_id);
        if (parent != nullptr) {
            parent->sub_kategori.push_back(kategori_baru);
        } else {
            cout << "[-] Gagal! Parent ID " << parent_id << " tidak ditemukan." << endl;
            delete kategori_baru; // Hindari memory leak jika gagal insert
        }
    }
}

// Fungsi untuk menampilkan seluruh hierarki secara rekursif
void tampilkanHierarki(const vector<Kategori*>& list_kat, string indentasi = "") {
    for (Kategori* kat : list_kat) {
        cout << indentasi << "|-- [" << kat->id_kategori << "] " << kat->nama << endl;
        tampilkanHierarki(kat->sub_kategori, indentasi + "    ");
    }
}



// 3. IMPLEMENTASI FILE I/O (PERSISTENT STORAGE)
// Fungsi pembantu untuk menulis data ke file secara rekursif
void simpanKeFileRekursif(ofstream& file, const vector<Kategori*>& list_kat) {
    for (Kategori* kat : list_kat) {
        // Format: ID;Nama;ParentID;Level
        file << kat->id_kategori << ";" << kat->nama << ";" 
             << kat->parent_id << ";" << kat->level << "\n";
        
        simpanKeFileRekursif(file, kat->sub_kategori);
    }
}

// Fungsi menggunakan ofstream untuk menulis ke file eksternal
void simpanData() {
    ofstream file("dataset_kategori.txt"); 
    if (file.is_open()) {
        simpanKeFileRekursif(file, root_kategori);
        file.close();
        cout << "[OK] Data berhasil disimpan ke 'dataset_kategori.txt'!" << endl;
    }
}

// Fungsi menggunakan ifstream untuk membaca data dari file eksternal
void muatData() {
    ifstream file("dataset_kategori.txt"); 
    if (!file.is_open()) {
        cout << "[!] File tidak ditemukan. Memulai dengan sesi baru." << endl;
        return;
    }

    string id_str, nama, parent_str, level_str;
    cout << "[OK] Memuat data dari database file..." << endl;
    
    // Membaca baris demi baris menggunakan delimiter ';'
    while (getline(file, id_str, ';')) {
        getline(file, nama, ';');
        getline(file, parent_str, ';');
        getline(file, level_str, '\n'); 

        if (!id_str.empty()) {
            tambahKategori(stoi(id_str), nama, stoi(parent_str), stoi(level_str));
        }
    }
    file.close();
}


// 4. TERMINASI & DEALOKASI MEMORI
// Fungsi untuk membebaskan memori dinamis (mencegah memory leak)
void hapusMemori(vector<Kategori*>& list_kat) {
    for (Kategori* kat : list_kat) {
        hapusMemori(kat->sub_kategori); // Post-order traversal
        delete kat; 
    }
    list_kat.clear();
}


int main() {
    cout << "=== SISTEM MANAJEMEN KATEGORI (PROGRES MINGGU 7) ===" << endl;
    
    // Inisialisasi: Memuat data dari dataset_kategori.txt (5.600+ data UCI)
    muatData();

    // Inisialisasi data awal (Hanya jalan jika file kosong)
    if (root_kategori.empty()) {
        cout << "\n[!] Data kosong, menginisialisasi dataset awal..." << endl;
        tambahKategori(1, "Semua Produk", 0, 1);
        simpanData(); 
    }

    // --- STRESS TEST: MENCARI DATA DI PALING BAWAH ---
    // Gunakan ID terakhir dari dataset kamu (contoh: 15676)
    int id_dicari = 15676; 
    int jumlah_iterasi = 10000; // Kita cari 10.000 kali biar waktunya terkumpul

    cout << "\n=== PENGUJIAN PERFORMA (10.000x Pencarian) ===" << endl;
    cout << "Mencari ID: " << id_dicari << endl;

    // 1. Mengukur kecepatan Linked Tree (DFS) - O(n)
    // Tree harus menelusuri ribuan node secara rekursif berulang kali
    auto start_tree = chrono::high_resolution_clock::now();
    for(int i = 0; i < jumlah_iterasi; i++) {
        cariKategori(root_kategori, id_dicari);
    }
    auto end_tree = chrono::high_resolution_clock::now();
    auto durasi_tree = chrono::duration_cast<chrono::microseconds>(end_tree - start_tree).count();

    // 2. Mengukur kecepatan Hash Map - O(1)
    // Hash Map langsung lompat ke alamat memori (Direct Addressing)
    auto start_hash = chrono::high_resolution_clock::now();
    for(int i = 0; i < jumlah_iterasi; i++) {
        cariDenganHash(id_dicari);
    }
    auto end_hash = chrono::high_resolution_clock::now();
    auto durasi_hash = chrono::duration_cast<chrono::microseconds>(end_hash - start_hash).count();

    // Output Hasil
    cout << "Total Waktu Tree (O(n)) : " << durasi_tree << " mikrodetik\n";
    cout << "Total Waktu Hash (O(1)) : " << durasi_hash << " mikrodetik\n";

    if (durasi_tree > 0) {
        double perbandingan = (double)durasi_tree / (durasi_hash > 0 ? durasi_hash : 1);
        cout << "Kesimpulan: Hash Map " << perbandingan << "x lebih cepat dari Tree!\n";
    }

    // Dealokasi memori
    hapusMemori(root_kategori); 
    cout << "\n[OK] Memori RAM dibebaskan. Program selesai." << endl;

    return 0;
}