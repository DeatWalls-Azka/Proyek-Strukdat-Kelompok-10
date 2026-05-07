#include <iostream>
#include <chrono>
#include <string>
#include "Kategori.h"

using namespace std;

// Fungsi Benchmark untuk Laporan Analisis Performa
void jalankanBenchmark(int id_target) {
    int n = 10000;
    cout << "\n[Benchmarking] Mencari ID " << id_target << " sebanyak " << n << " kali..." << endl;
    
    // Uji Tree (DFS) - O(n)
    auto s1 = chrono::high_resolution_clock::now();
    for(int i=0; i<n; i++) cariKategoriDFS(root_kategori, id_target);
    auto e1 = chrono::high_resolution_clock::now();
    
    // Uji Hash Map - O(1)
    auto s2 = chrono::high_resolution_clock::now();
    for(int i=0; i<n; i++) cariDenganHash(id_target);
    auto e2 = chrono::high_resolution_clock::now();

    auto durasi_tree = chrono::duration_cast<chrono::microseconds>(e1-s1).count();
    auto durasi_hash = chrono::duration_cast<chrono::microseconds>(e2-s2).count();

    cout << ">> Total Waktu Tree (DFS) : " << durasi_tree << " mks" << endl;
    cout << ">> Total Waktu Hash Map  : " << durasi_hash << " mks" << endl;
    
    if (durasi_hash > 0) {
        cout << ">> Kesimpulan: Hash Map " << (double)durasi_tree/durasi_hash << "x lebih cepat!" << endl;
    }
}

int main() {
    // Memuat data dari file saat program dijalankan
    muatData();
    
    int pilihan, id, p_id, lvl, status_input;
    string nama;

    do {
        cout << "\n============================================";
        cout << "\n   FINAL PROJECT: SISTEM MONITORING KATEGORI";
        cout << "\n============================================";
        cout << "\n1. Tampilkan Hierarki Kategori";
        cout << "\n2. Tambah Kategori Baru (+ Status)";
        cout << "\n3. Update Nama Kategori";
        cout << "\n4. Hapus Kategori & Sub-Kategori";
        cout << "\n5. Urutkan Kategori (A-Z)";
        cout << "\n6. Uji Performa Pencarian (Benchmarking)";
        cout << "\n7. Monitoring Penggunaan Memori (RAM)"; // <-- Syarat PDF
        cout << "\n0. Simpan & Keluar";
        cout << "\n--------------------------------------------";
        cout << "\nPilih Menu: "; cin >> pilihan;

        switch(pilihan) {
            case 1:
                cout << "\n--- STRUKTUR HIERARKI KATEGORI ---\n";
                tampilkanHierarki(root_kategori);
                break;

            case 2:
                cout << "\n--- INPUT DATA BARU ---" << endl;
                cout << "ID Kategori  : "; cin >> id;
                cout << "Nama Kategori: "; cin.ignore(); getline(cin, nama);
                cout << "Parent ID    : "; cin >> p_id;
                cout << "Level        : "; cin >> lvl;
                cout << "Status (1:Aktif, 0:Nonaktif): "; cin >> status_input; // <-- Syarat Asdos
                tambahKategori(id, nama, p_id, lvl, status_input);
                cout << "[OK] Kategori berhasil ditambahkan." << endl;
                break;

            case 3:
                cout << "Masukkan ID yang diupdate: "; cin >> id;
                if(map_kategori.count(id)) {
                    cout << "Nama Baru: "; cin.ignore(); getline(cin, nama);
                    map_kategori[id]->nama = nama;
                    cout << "[OK] Nama diperbarui." << endl;
                } else cout << "[!] ID tidak ditemukan." << endl;
                break;

            case 4:
                cout << "Masukkan ID yang akan dihapus: "; cin >> id;
                if(map_kategori.count(id)) {
                    hapusKategoriRecursive(id);
                    cout << "[OK] Kategori dan turunannya berhasil dihapus." << endl;
                } else cout << "[!] ID tidak ditemukan." << endl;
                break;

            case 5:
                urutkanKategori(root_kategori);
                cout << "[OK] Data diurutkan berdasarkan abjad." << endl;
                break;

            case 6:
                cout << "Masukkan ID Target untuk Uji Kecepatan: "; cin >> id;
                jalankanBenchmark(id);
                break;

            case 7:
                hitungEstimasiMemori(); // <-- Memanggil fungsi baru kamu
                break;

            case 0:
                simpanData();
                cout << "[OK] Data tersimpan. Program berhenti." << endl;
                break;

            default:
                cout << "[!] Pilihan tidak tersedia." << endl;
        }

    } while (pilihan != 0);

    return 0;
}