#ifndef KATEGORI_H
#define KATEGORI_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;
// Struktur data untuk kategori
struct Kategori {
    int id_kategori;
    string nama;
    int parent_id;
    int level;
    int status; 

    vector<Kategori*> sub_kategori; 
    Kategori(int id, string n, int pid, int lvl, int st) 
        : id_kategori(id), nama(n), parent_id(pid), level(lvl), status(st) {}
};

// Variabel global untuk menyimpan kategori
extern vector<Kategori*> root_kategori;
extern unordered_map<int, Kategori*> map_kategori;

// Fungsi untuk mengelola kategori
void muatData();
void simpanData();

bool tambahKategori(int id, string nama, int parent_id, int status);
bool tambahKategoriDenganLevel(int id, string nama, int parent_id, int level, int status);
bool pindahParentKategori(int id, int parent_id_baru);
void hapusKategoriRecursive(int id);
void urutkanKategori(vector<Kategori*>& list_kat);
void urutkanKategoriById(vector<Kategori*>& list_kat);
Kategori* cariDenganHash(int id_target);
Kategori* cariKategoriDFS(const vector<Kategori*>& list_kat, int id_target);
Kategori* cariDenganNama(const vector<Kategori*>& list_kat, string nama_target);
void tampilkanHierarki(const vector<Kategori*>& list_kat, string indent = "");
void tampilkanHierarkiTerbatas(const vector<Kategori*>& list_kat, int batas);
void hitungEstimasiMemori();
void tampilkanDaftarSubkategoriNonLeaf(const vector<Kategori*>& list_kat, string indent = "");
void tampilkanSemuaSubkategoriBerdasarkanKategori(int id_kategori);
void tampilkanDataLeafBerdasarkanSubkategori(int id_subkategori);
void tampilkanDataLeafRekursif(Kategori* kat, int& jumlah_data);


#endif
