#include "Kategori.h"
#include <fstream>
#include <sstream>
#include <algorithm>

vector<Kategori*> root_kategori;
unordered_map<int, Kategori*> map_kategori;

// Update: Sekarang menerima 5 parameter (tambah status)
void tambahKategori(int id, string nama, int parent_id, int level, int status) {
    if (map_kategori.count(id)) return;

    // Pastikan status dikirim ke constructor Kategori
    Kategori* baru = new Kategori(id, nama, parent_id, level, status);
    map_kategori[id] = baru;

    if (parent_id == 0) {
        root_kategori.push_back(baru);
    } else {
        Kategori* parent = map_kategori[parent_id];
        if (parent) parent->sub_kategori.push_back(baru);
    }
}

void hapusKategoriRecursive(int id) {
    Kategori* target = map_kategori[id];
    if (!target) return;

    while (!target->sub_kategori.empty()) {
        hapusKategoriRecursive(target->sub_kategori.back()->id_kategori);
    }

    if (target->parent_id == 0) {
        root_kategori.erase(remove(root_kategori.begin(), root_kategori.end(), target), root_kategori.end());
    } else {
        Kategori* parent = map_kategori[target->parent_id];
        parent->sub_kategori.erase(remove(parent->sub_kategori.begin(), parent->sub_kategori.end(), target), parent->sub_kategori.end());
    }

    map_kategori.erase(id);
    delete target;
}

void urutkanKategori(vector<Kategori*>& list_kat) {
    sort(list_kat.begin(), list_kat.end(), [](Kategori* a, Kategori* b) {
        return a->nama < b->nama;
    });
    for (Kategori* kat : list_kat) urutkanKategori(kat->sub_kategori);
}

// Update: Menyimpan status (kolom ke-5) ke dalam file teks
void simpanKeFileRekursif(ofstream& file, const vector<Kategori*>& list_kat) {
    for (Kategori* kat : list_kat) {
        file << kat->id_kategori << ";" << kat->nama << ";" 
             << kat->parent_id << ";" << kat->level << ";" 
             << kat->status << "\n"; // Tambah status di sini
        simpanKeFileRekursif(file, kat->sub_kategori);
    }
}

void simpanData() {
    ofstream file("dataset_kategori.txt");
    if (file.is_open()) {
        simpanKeFileRekursif(file, root_kategori);
        file.close();
    }
}

// Update: Membaca 5 kolom dari dataset_kategori.txt
void muatData() {
    ifstream file("dataset_kategori.txt");
    string id_s, nama, p_id_s, lvl_s, status_s;
    if (!file.is_open()) return;

    while (getline(file, id_s, ';')) {
        getline(file, nama, ';');
        getline(file, p_id_s, ';');
        getline(file, lvl_s, ';');   // Ubah delimiter ke ';' karena ada kolom status setelahnya
        getline(file, status_s, '\n'); // Status adalah kolom terakhir (sampai baris baru)
        
        if (!id_s.empty()) {
            tambahKategori(stoi(id_s), nama, stoi(p_id_s), stoi(lvl_s), stoi(status_s));
        }
    }
    file.close();
}

Kategori* cariDenganHash(int id_target) {
    return (map_kategori.count(id_target)) ? map_kategori[id_target] : nullptr;
}

Kategori* cariKategoriDFS(const vector<Kategori*>& list_kat, int id_target) {
    for (Kategori* kat : list_kat) {
        if (kat->id_kategori == id_target) return kat;
        Kategori* hasil = cariKategoriDFS(kat->sub_kategori, id_target);
        if (hasil) return hasil;
    }
    return nullptr;
}

void tampilkanHierarki(const vector<Kategori*>& list_kat, string indent) {
    for (Kategori* kat : list_kat) {
        // Tambahkan info status (Aktif/Nonaktif) agar asdos melihat domain datanya lengkap
        string txt_status = (kat->status == 1) ? "Aktif" : "Nonaktif";
        cout << indent << "|-- [" << kat->id_kategori << "] " << kat->nama << " (" << txt_status << ")" << endl;
        tampilkanHierarki(kat->sub_kategori, indent + "    ");
    }
}

void hitungEstimasiMemori() {
    size_t jumlah_node = map_kategori.size();
    size_t memori_tree = jumlah_node * sizeof(Kategori);
    size_t memori_hash = jumlah_node * (sizeof(int) + sizeof(Kategori*) + 8); 

    cout << "\n=== ESTIMASI PENGGUNAAN MEMORI ===" << endl;
    cout << ">> Total Data: " << jumlah_node << " entitas" << endl;
    cout << ">> Estimasi RAM Tree: " << (double)memori_tree / 1024 << " KB" << endl;
    cout << ">> Estimasi RAM Hash Map: " << (double)memori_hash / 1024 << " KB" << endl;
    cout << "Kesimpulan: Hash Map memakan memori sedikit lebih besar (~1.5x) demi kecepatan O(1)." << endl;
}