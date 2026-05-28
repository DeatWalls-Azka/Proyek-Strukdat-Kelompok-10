#include <iostream>
#include <chrono>
#include <string>
#include <iomanip>
#include <vector>
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

void kumpulkanIdBerdasarkanLevel(
    const vector<Kategori*>& list_kat,
    int level_target,
    vector<int>& daftar_id
) {
    for (Kategori* kat : list_kat) {
        if (kat->level == level_target) {
            daftar_id.push_back(kat->id_kategori);
        }

        kumpulkanIdBerdasarkanLevel(kat->sub_kategori, level_target, daftar_id);
    }
}

void benchmarkPencarianPerLevel(int level_target) {
    vector<int> daftar_id;
    kumpulkanIdBerdasarkanLevel(root_kategori, level_target, daftar_id);

    if (daftar_id.empty()) {
        cout << "\n[!] Tidak ada data pada level " << level_target << endl;
        return;
    }

    int pengulangan = 10;
    long long total_pencarian = (long long) daftar_id.size() * pengulangan;

    int jumlah_ketemu_dfs = 0;
    int jumlah_ketemu_hash = 0;

    auto mulai_dfs = chrono::high_resolution_clock::now();

    for (int ulang = 0; ulang < pengulangan; ulang++) {
        for (int id : daftar_id) {
            if (cariKategoriDFS(root_kategori, id) != nullptr) {
                jumlah_ketemu_dfs++;
            }
        }
    }

    auto selesai_dfs = chrono::high_resolution_clock::now();

    auto mulai_hash = chrono::high_resolution_clock::now();

    for (int ulang = 0; ulang < pengulangan; ulang++) {
        for (int id : daftar_id) {
            if (cariDenganHash(id) != nullptr) {
                jumlah_ketemu_hash++;
            }
        }
    }

    auto selesai_hash = chrono::high_resolution_clock::now();

    auto durasi_dfs_ns = chrono::duration_cast<chrono::nanoseconds>(
        selesai_dfs - mulai_dfs
    ).count();

    auto durasi_hash_ns = chrono::duration_cast<chrono::nanoseconds>(
        selesai_hash - mulai_hash
    ).count();

    double rata_dfs = (double) durasi_dfs_ns / total_pencarian;
    double rata_hash = (double) durasi_hash_ns / total_pencarian;

    cout << "\n=== BENCHMARK PENCARIAN LEVEL " << level_target << " ===" << endl;
    cout << "Jumlah data pada level " << level_target << " : " << daftar_id.size() << endl;
    cout << "Total pengulangan pencarian     : " << total_pencarian << " kali" << endl;

    cout << fixed << setprecision(3);

    cout << "\n[DFS / Tree Search]" << endl;
    cout << "Total waktu : " << durasi_dfs_ns / 1000.0 << " mikrodetik" << endl;
    cout << "Rata-rata   : " << rata_dfs << " nanodetik per pencarian" << endl;
    cout << "Data ketemu : " << jumlah_ketemu_dfs << endl;

    cout << "\n[Hash Map Search]" << endl;
    cout << "Total waktu : " << durasi_hash_ns / 1000.0 << " mikrodetik" << endl;
    cout << "Rata-rata   : " << rata_hash << " nanodetik per pencarian" << endl;
    cout << "Data ketemu : " << jumlah_ketemu_hash << endl;

    if (durasi_hash_ns > 0) {
        cout << "\nKesimpulan: Hash Map sekitar "
             << (double) durasi_dfs_ns / durasi_hash_ns
             << "x lebih cepat dibanding DFS pada level "
             << level_target << "." << endl;
    }
}

void benchmarkLevel2Sampai5() {
    benchmarkPencarianPerLevel(2);
    benchmarkPencarianPerLevel(3);
    benchmarkPencarianPerLevel(4);
    benchmarkPencarianPerLevel(5);
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
        cout << "\n7. Monitoring Penggunaan Memori (RAM)";
        cout << "\n0. Simpan & Keluar";
        cout << "\n--------------------------------------------";
        cout << "\nPilih Menu: "; cin >> pilihan;

        switch(pilihan) {
            case 1: {
                int pilihan_tampil;

                cout << "\n--- MENU TAMPILKAN DATA KATEGORI ---";
                cout << "\n1. Tampilkan Semua Hierarki";
                cout << "\n2. Tampilkan 50 Data Hierarki Teratas";
                cout << "\n3. Tampilkan Data Berdasarkan Subkategori";
                cout << "\nPilih Tampilan: ";
                cin >> pilihan_tampil;

                if (pilihan_tampil == 1)
                {
                    cout << "\n--- STRUKTUR HIERARKI KATEGORI LENGKAP ---\n";
                    tampilkanHierarki(root_kategori);
                }
                else if (pilihan_tampil == 2)
                {
                    cout << "\n--- STRUKTUR HIERARKI KATEGORI (50 DATA TERATAS) ---\n";
                    tampilkanHierarkiTerbatas(root_kategori, 50);
                }
                else if (pilihan_tampil == 3)
                {
                    int id_subkategori;

                    cout << "\n--- DAFTAR SUBKATEGORI YANG TERSEDIA ---\n";
                    tampilkanDaftarSubkategoriNonLeaf(root_kategori);

                    cout << "\nMasukkan ID subkategori yang ingin ditampilkan datanya: ";
                    cin >> id_subkategori;

                    tampilkanDataLeafBerdasarkanSubkategori(id_subkategori);
                }
                else
                {
                    cout << "[!] Pilihan tampilan tidak tersedia." << endl;
                }

                break;
            }

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

            case 6: { // Di-bunder/dibungkus kurung kurawal agar terhindar dari compiler error initialization
                cout << "\nPilih Metode Pencarian:\n1. Cari berdasarkan ID (Benchmark Tree vs Hash Map)\n2. Cari berdasarkan Nama\nPilihan: ";
                int pil_cari;
                cin >> pil_cari;

                if (pil_cari == 1) {
                    int id_target;
                    cout << "Masukkan ID Kategori: ";
                    cin >> id_target;
                    
                    // Validasi eksistensi data ID sebelum di-benchmark
                    if (map_kategori.count(id_target)) {
                        cout << "\n[DATA DITEMUKAN]" << endl;
                        cout << ">> Nama Kategori : " << map_kategori[id_target]->nama << endl;
                        cout << ">> Level/Depth    : " << map_kategori[id_target]->level << endl;
                        jalankanBenchmark(id_target);
                    } else {
                        cout << "[!] ID Kategori tidak ditemukan di dalam dataset.\n";
                    }
                } else {
                    string nama_target;
                    cout << "Masukkan Nama Kategori: ";
                    cin.ignore();
                    getline(cin, nama_target);
                    
                    // Uji performa pencarian nama (DFS) menggunakan tracker waktu chrono
                    auto mulai_nama = chrono::high_resolution_clock::now();
                    Kategori* hasil = cariDenganNama(root_kategori, nama_target);
                    auto selesai_nama = chrono::high_resolution_clock::now();
                    chrono::duration<double, micro> durasi_nama = selesai_nama - mulai_nama;

                    if (hasil) {
                        cout << "\n[DATA DITEMUKAN]" << endl;
                        cout << ">> ID Kategori   : " << hasil->id_kategori << endl;
                        cout << ">> Level/Depth   : " << hasil->level << endl;
                        cout << ">> Status        : " << ((hasil->status == 1) ? "Aktif" : "Nonaktif") << endl;
                        cout << "--------------------------------------" << endl;
                        cout << ">> Waktu Cari Nama (DFS) : " << durasi_nama.count() << " mikrodetik" << endl;
                    } else {
                        cout << "\n[!] Kategori dengan nama '" << nama_target << "' tidak ditemukan.\n";
                    }
                }
                break; // Menghindari bug fall-through ke case 7
            }

            case 7:
                hitungEstimasiMemori(); 
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