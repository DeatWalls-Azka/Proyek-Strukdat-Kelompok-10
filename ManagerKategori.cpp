#include "Kategori.h"
#include <fstream>
#include <sstream>
#include <algorithm>

vector<Kategori*> root_kategori;
unordered_map<int, Kategori*> map_kategori;

const string DATA_FILE = "dataset_kategori.txt";

bool tambahKategoriDenganLevel(int id, string nama, int parent_id, int level, int status) {
    if (map_kategori.count(id)) return false;

    if (parent_id != 0 && !map_kategori.count(parent_id)) {
        return false;
    }

    // Pastikan status dikirim ke constructor Kategori
    Kategori* baru = new Kategori(id, nama, parent_id, level, status);
    map_kategori[id] = baru;

    if (parent_id == 0) {
        root_kategori.push_back(baru);
    } else {
        Kategori* parent = map_kategori[parent_id];
        parent->sub_kategori.push_back(baru);
    }

    return true;
}

bool tambahKategori(int id, string nama, int parent_id, int status) {
    int level_otomatis = 1;

    if (parent_id != 0) {
        if (!map_kategori.count(parent_id)) return false;

        level_otomatis = map_kategori[parent_id]->level + 1;
    }

    return tambahKategoriDenganLevel(id, nama, parent_id, level_otomatis, status);
}

bool apakahTurunan(Kategori* parent, int id_dicari) {
    if (parent == nullptr) return false;

    for (Kategori* anak : parent->sub_kategori) {
        if (anak->id_kategori == id_dicari) return true;
        if (apakahTurunan(anak, id_dicari)) return true;
    }

    return false;
}

void updateLevelTurunan(Kategori* kat, int level_baru) {
    if (kat == nullptr) return;

    kat->level = level_baru;

    for (Kategori* anak : kat->sub_kategori) {
        updateLevelTurunan(anak, level_baru + 1);
    }
}

bool pindahParentKategori(int id, int parent_id_baru) {
    if (!map_kategori.count(id)) return false;
    if (id == parent_id_baru) return false;
    if (parent_id_baru != 0 && !map_kategori.count(parent_id_baru)) return false;

    Kategori* target = map_kategori[id];

    if (parent_id_baru != 0 && apakahTurunan(target, parent_id_baru)) {
        return false;
    }

    if (target->parent_id == 0) {
        root_kategori.erase(remove(root_kategori.begin(), root_kategori.end(), target), root_kategori.end());
    } else {
        Kategori* parent_lama = map_kategori[target->parent_id];
        parent_lama->sub_kategori.erase(
            remove(parent_lama->sub_kategori.begin(), parent_lama->sub_kategori.end(), target),
            parent_lama->sub_kategori.end()
        );
    }

    target->parent_id = parent_id_baru;

    if (parent_id_baru == 0) {
        root_kategori.push_back(target);
        updateLevelTurunan(target, 1);
    } else {
        Kategori* parent_baru = map_kategori[parent_id_baru];
        parent_baru->sub_kategori.push_back(target);
        updateLevelTurunan(target, parent_baru->level + 1);
    }

    return true;
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
    ofstream file(DATA_FILE);
    if (file.is_open()) {
        simpanKeFileRekursif(file, root_kategori);
        file.close();
    }
}

// Update: Membaca 5 kolom dari dataset_kategori.txt
void muatData() {
    ifstream file(DATA_FILE);
    string id_s, nama, p_id_s, lvl_s, status_s;
    if (!file.is_open()) return;

    while (getline(file, id_s, ';')) {
        if (!getline(file, nama, ';')) break;
        if (!getline(file, p_id_s, ';')) break;
        if (!getline(file, lvl_s, ';')) break;   // Ubah delimiter ke ';' karena ada kolom status setelahnya
        if (!getline(file, status_s, '\n')) break; // Status adalah kolom terakhir (sampai baris baru)
        
        if (!id_s.empty()) {
            tambahKategoriDenganLevel(stoi(id_s), nama, stoi(p_id_s), stoi(lvl_s), stoi(status_s));
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

void tampilkanHierarkiTerbatasHelper(
    const vector<Kategori*>& list_kat,
    string indent,
    int& jumlah_tampil,
    int batas
) {
    for (Kategori* kat : list_kat) {
        if (jumlah_tampil >= batas) {
            return;
        }

        string txt_status = (kat->status == 1) ? "Aktif" : "Nonaktif";

        cout << indent << "|-- [" << kat->id_kategori << "] "
             << kat->nama << " (" << txt_status << ")" << endl;

        jumlah_tampil++;

        tampilkanHierarkiTerbatasHelper(
            kat->sub_kategori,
            indent + "    ",
            jumlah_tampil,
            batas
        );
    }
}

void tampilkanHierarkiTerbatas(const vector<Kategori*>& list_kat, int batas) {
    int jumlah_tampil = 0;

    tampilkanHierarkiTerbatasHelper(list_kat, "", jumlah_tampil, batas);

    cout << "\n[Info] Menampilkan " << jumlah_tampil
         << " data teratas dari struktur hierarki." << endl;
}

void hitungEstimasiMemori() {
    size_t jumlah_node = map_kategori.size();
    size_t memori_tree = jumlah_node * sizeof(Kategori);
    size_t memori_hash_index = jumlah_node * (sizeof(int) + sizeof(Kategori*) + 8);
    size_t memori_hybrid = memori_tree + memori_hash_index;

    cout << "\n=== ESTIMASI PENGGUNAAN MEMORI ===" << endl;
    cout << ">> Total Data: " << jumlah_node << " entitas" << endl;
    cout << ">> Estimasi RAM Tree saja: " << (double)memori_tree / 1024 << " KB" << endl;
    cout << ">> Estimasi RAM Hash Map index: " << (double)memori_hash_index / 1024 << " KB" << endl;
    cout << ">> Estimasi RAM Tree + Hash Map: " << (double)memori_hybrid / 1024 << " KB" << endl;
    cout << "Kesimpulan: Hash Map dipakai sebagai index tambahan, sehingga sistem hybrid membutuhkan memori lebih besar demi pencarian ID O(1)." << endl;
}


Kategori* cariDenganNama(const vector<Kategori*>& list_kat, string nama_target) {
    for (Kategori* kat : list_kat) {
        // Menggunakan pencarian case-insensitive atau exact match
        if (kat->nama == nama_target) return kat;
        
        // Rekursif cari ke anak-anaknya
        Kategori* hasil = cariDenganNama(kat->sub_kategori, nama_target);
        if (hasil) return hasil;
    }
    return nullptr;
}

void tampilkanDaftarSubkategoriNonLeaf(const vector<Kategori*>& list_kat, string indent)
{
    for (Kategori* kat : list_kat)
    {
        // Tampilkan hanya kategori/subkategori yang masih punya anak
        if (!kat->sub_kategori.empty())
        {
            cout << indent << "[" << kat->id_kategori << "] "
                 << kat->nama
                 << " | Level: " << kat->level
                 << endl;
        }

        tampilkanDaftarSubkategoriNonLeaf(kat->sub_kategori, indent + "    ");
    }
}

void tampilkanSubkategoriRekursif(Kategori* kat, string indent, int& jumlah_data)
{
    if (kat == nullptr)
    {
        return;
    }

    for (Kategori* anak : kat->sub_kategori)
    {
        string txt_status = (anak->status == 1) ? "Aktif" : "Nonaktif";

        cout << indent << "|-- [" << anak->id_kategori << "] "
             << anak->nama
             << " | Level: " << anak->level
             << " | " << txt_status
             << endl;

        jumlah_data++;
        tampilkanSubkategoriRekursif(anak, indent + "    ", jumlah_data);
    }
}

void tampilkanSemuaSubkategoriBerdasarkanKategori(int id_kategori)
{
    Kategori* target = cariDenganHash(id_kategori);

    if (target == nullptr)
    {
        cout << "[!] ID kategori tidak ditemukan." << endl;
        return;
    }

    cout << "\n--- SEMUA SUBKATEGORI / TURUNAN KATEGORI ---" << endl;
    cout << "Kategori : [" << target->id_kategori << "] "
         << target->nama << endl;
    cout << "Level    : " << target->level << endl;
    cout << "---------------------------------------------" << endl;

    int jumlah_data = 0;

    tampilkanSubkategoriRekursif(target, "", jumlah_data);

    if (jumlah_data == 0)
    {
        cout << "[INFO] Kategori ini belum memiliki subkategori." << endl;
    }

    cout << "---------------------------------------------" << endl;
    cout << "[INFO] Total subkategori/turunan ditemukan: "
         << jumlah_data << endl;
}

void tampilkanDataLeafRekursif(Kategori* kat, int& jumlah_data)
{
    if (kat == nullptr)
    {
        return;
    }

    // Kalau tidak punya anak, berarti ini data paling bawah / data produk
    if (kat->sub_kategori.empty())
    {
        cout << jumlah_data + 1 << ". "
             << "[" << kat->id_kategori << "] "
             << kat->nama
             << " - "
             << ((kat->status == 1) ? "Aktif" : "Nonaktif")
             << endl;

        jumlah_data++;
        return;
    }

    // Kalau masih punya anak, lanjut telusuri anak-anaknya
    for (Kategori* anak : kat->sub_kategori)
    {
        tampilkanDataLeafRekursif(anak, jumlah_data);
    }
}

void tampilkanDataLeafBerdasarkanSubkategori(int id_subkategori)
{
    Kategori* target = cariDenganHash(id_subkategori);

    if (target == nullptr)
    {
        cout << "[!] ID subkategori tidak ditemukan." << endl;
        return;
    }

    cout << "\n--- DATA PADA SUBKATEGORI TERPILIH ---" << endl;
    cout << "Subkategori : [" << target->id_kategori << "] "
         << target->nama << endl;
    cout << "Level       : " << target->level << endl;
    cout << "----------------------------------------" << endl;

    int jumlah_data = 0;

    tampilkanDataLeafRekursif(target, jumlah_data);

    cout << "----------------------------------------" << endl;
    cout << "[INFO] Total data ditemukan: "
         << jumlah_data << endl;
}
