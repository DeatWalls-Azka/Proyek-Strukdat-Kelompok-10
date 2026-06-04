#include <iostream>
#include <chrono>
#include <string>
#include <iomanip>
#include <vector>
#include <fstream>
#include "Kategori.h"


using namespace std;

// Fungsi Benchmark untuk Laporan Analisis Performa
void jalankanBenchmark(int id_target) {
    int n = 100000;
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

    int pengulangan = 1000;
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

void kumpulkanSemuaKategori(
    const vector<Kategori*>& list_kat,
    vector<Kategori*>& daftar_kategori
) {
    for (Kategori* kat : list_kat) {
        daftar_kategori.push_back(kat);
        kumpulkanSemuaKategori(kat->sub_kategori, daftar_kategori);
    }
}

Kategori* cariKategoriDFSBatas(
    const vector<Kategori*>& list_kat,
    int id_target,
    int& jumlah_dikunjungi,
    int batas_data
) {
    for (Kategori* kat : list_kat) {
        if (jumlah_dikunjungi >= batas_data) return nullptr;

        jumlah_dikunjungi++;

        if (kat->id_kategori == id_target) return kat;

        Kategori* hasil = cariKategoriDFSBatas(
            kat->sub_kategori,
            id_target,
            jumlah_dikunjungi,
            batas_data
        );

        if (hasil) return hasil;
    }

    return nullptr;
}

void exportBenchmarkPertumbuhanDataKeCSV() {
    vector<Kategori*> semua_kategori;
    kumpulkanSemuaKategori(root_kategori, semua_kategori);

    if (semua_kategori.empty()) {
        cout << "[!] Dataset kosong. Benchmark pertumbuhan data tidak dapat dijalankan." << endl;
        return;
    }

    ofstream file("hasil_benchmark_pertumbuhan_data.csv");

    if (!file.is_open()) {
        cout << "[!] Gagal membuat file hasil_benchmark_pertumbuhan_data.csv" << endl;
        return;
    }

    vector<int> daftar_ukuran = {1000, 2000, 3000, 4000, 5000};
    int total_data = (int) semua_kategori.size();

    if (total_data < 1000) {
        daftar_ukuran.clear();
    }

    if (daftar_ukuran.empty() || daftar_ukuran.back() != total_data) {
        daftar_ukuran.push_back(total_data);
    }

    int pengulangan = 10000;

    file << "jumlah_data,id_target,pengulangan,total_pencarian,"
         << "estimasi_node_dikunjungi_dfs,"
         << "waktu_dfs_us,waktu_hash_us,rata_dfs_ns,rata_hash_ns,"
         << "rasio_dfs_vs_hash,estimasi_memori_tree_kb,"
         << "estimasi_memori_hash_index_kb,estimasi_memori_tree_hash_kb\n";

    cout << "\n=== EXPORT BENCHMARK PERTUMBUHAN DATA ===" << endl;

    for (int ukuran : daftar_ukuran) {
        if (ukuran > total_data) continue;

        int id_target = semua_kategori[ukuran - 1]->id_kategori;
        long long total_pencarian = pengulangan;
        long long estimasi_node_dikunjungi_dfs = (long long) ukuran * pengulangan;
        int jumlah_ketemu_dfs = 0;
        int jumlah_ketemu_hash = 0;
        int faktor_pengukuran_hash = 100;
        int pengulangan_hash = pengulangan * faktor_pengukuran_hash;

        unordered_map<int, Kategori*> map_sementara;
        map_sementara.reserve(ukuran);

        for (int i = 0; i < ukuran; i++) {
            map_sementara[semua_kategori[i]->id_kategori] = semua_kategori[i];
        }

        auto mulai_dfs = chrono::high_resolution_clock::now();

        for (int ulang = 0; ulang < pengulangan; ulang++) {
            int jumlah_dikunjungi = 0;
            if (cariKategoriDFSBatas(root_kategori, id_target, jumlah_dikunjungi, ukuran) != nullptr) {
                jumlah_ketemu_dfs++;
            }
        }

        auto selesai_dfs = chrono::high_resolution_clock::now();

        auto mulai_hash = chrono::high_resolution_clock::now();

        for (int ulang = 0; ulang < pengulangan_hash; ulang++) {
            if (map_sementara.find(id_target) != map_sementara.end()) {
                jumlah_ketemu_hash++;
            }
        }

        auto selesai_hash = chrono::high_resolution_clock::now();

        auto durasi_dfs_ns = chrono::duration_cast<chrono::nanoseconds>(
            selesai_dfs - mulai_dfs
        ).count();

        auto durasi_hash_ns_terukur = chrono::duration_cast<chrono::nanoseconds>(
            selesai_hash - mulai_hash
        ).count();
        double durasi_hash_ns = (double) durasi_hash_ns_terukur / faktor_pengukuran_hash;

        double waktu_dfs_us = durasi_dfs_ns / 1000.0;
        double waktu_hash_us = durasi_hash_ns / 1000.0;
        double rata_dfs_ns = (double) durasi_dfs_ns / pengulangan;
        double rata_hash_ns = durasi_hash_ns / pengulangan;
        double rasio = (durasi_hash_ns > 0) ? (double) durasi_dfs_ns / durasi_hash_ns : 0;
        double memori_tree_kb = (double)(ukuran * sizeof(Kategori)) / 1024;
        double memori_hash_index_kb = (double)(ukuran * (sizeof(int) + sizeof(Kategori*) + 8)) / 1024;
        double memori_hybrid_kb = memori_tree_kb + memori_hash_index_kb;

        if (jumlah_ketemu_hash == 0) {
            cout << "[!] Target tidak ditemukan pada Hash Map sementara." << endl;
        }

        file << ukuran << ","
             << id_target << ","
             << pengulangan << ","
             << total_pencarian << ","
             << estimasi_node_dikunjungi_dfs << ","
             << fixed << setprecision(3)
             << waktu_dfs_us << ","
             << waktu_hash_us << ","
             << rata_dfs_ns << ","
             << rata_hash_ns << ","
             << rasio << ","
             << memori_tree_kb << ","
             << memori_hash_index_kb << ","
             << memori_hybrid_kb << "\n";

        cout << fixed << setprecision(3)
             << "Data " << ukuran
             << " | DFS: " << waktu_dfs_us
             << " us | Hash: " << waktu_hash_us
             << " us | Ketemu: " << jumlah_ketemu_dfs
             << " | Hash dinormalisasi"
             << endl;
    }

    file.close();

    cout << "\n[OK] Benchmark pertumbuhan data berhasil diekspor ke hasil_benchmark_pertumbuhan_data.csv" << endl;
    cout << "[INFO] Gunakan CSV ini untuk tabel/grafik dampak pertumbuhan jumlah data." << endl;
}

void exportBenchmarkKeCSV() {
    ofstream file("hasil_benchmark.csv");

    if (!file.is_open()) {
        cout << "[!] Gagal membuat file hasil_benchmark.csv" << endl;
        return;
    }

    size_t jumlah_node = map_kategori.size();
    double memori_tree_kb = (double)(jumlah_node * sizeof(Kategori)) / 1024;
    double memori_hash_index_kb = (double)(jumlah_node * (sizeof(int) + sizeof(Kategori*) + 8)) / 1024;
    double memori_hybrid_kb = memori_tree_kb + memori_hash_index_kb;

    file << "level,jumlah_data,total_pencarian,waktu_dfs_us,waktu_hash_us,"
         << "rata_dfs_ns,rata_hash_ns,rasio_dfs_vs_hash,total_node,"
         << "estimasi_memori_tree_kb,estimasi_memori_hash_index_kb,"
         << "estimasi_memori_tree_hash_kb\n";

    for (int level_target = 2; level_target <= 5; level_target++) {
        vector<int> daftar_id;
        kumpulkanIdBerdasarkanLevel(root_kategori, level_target, daftar_id);

        if (daftar_id.empty()) {
            file << level_target << ",0,0,0,0,0,0,0,"
                 << jumlah_node << "," << memori_tree_kb << ","
                 << memori_hash_index_kb << "," << memori_hybrid_kb << "\n";
            continue;
        }

        int pengulangan = 1000;
        long long total_pencarian = (long long)daftar_id.size() * pengulangan;

        auto mulai_dfs = chrono::high_resolution_clock::now();

        for (int ulang = 0; ulang < pengulangan; ulang++) {
            for (int id : daftar_id) {
                cariKategoriDFS(root_kategori, id);
            }
        }

        auto selesai_dfs = chrono::high_resolution_clock::now();

        auto mulai_hash = chrono::high_resolution_clock::now();

        for (int ulang = 0; ulang < pengulangan; ulang++) {
            for (int id : daftar_id) {
                cariDenganHash(id);
            }
        }

        auto selesai_hash = chrono::high_resolution_clock::now();

        auto durasi_dfs_ns = chrono::duration_cast<chrono::nanoseconds>(
            selesai_dfs - mulai_dfs
        ).count();

        auto durasi_hash_ns = chrono::duration_cast<chrono::nanoseconds>(
            selesai_hash - mulai_hash
        ).count();

        double waktu_dfs_us = durasi_dfs_ns / 1000.0;
        double waktu_hash_us = durasi_hash_ns / 1000.0;
        double rata_dfs_ns = (double)durasi_dfs_ns / total_pencarian;
        double rata_hash_ns = (double)durasi_hash_ns / total_pencarian;
        double rasio = (durasi_hash_ns > 0) ? (double)durasi_dfs_ns / durasi_hash_ns : 0;

        file << level_target << ","
             << daftar_id.size() << ","
             << total_pencarian << ","
             << fixed << setprecision(3)
             << waktu_dfs_us << ","
             << waktu_hash_us << ","
             << rata_dfs_ns << ","
             << rata_hash_ns << ","
             << rasio << ","
             << jumlah_node << ","
             << memori_tree_kb << ","
             << memori_hash_index_kb << ","
             << memori_hybrid_kb << "\n";
    }

    file.close();

    cout << "\n[OK] Benchmark berhasil diekspor ke hasil_benchmark.csv" << endl;
    cout << "[INFO] Buka file CSV tersebut di Excel/Google Sheets untuk membuat grafik." << endl;
}

int main() {
    // Memuat data dari file saat program dijalankan
    muatData();
    
    int pilihan, id, p_id, status_input;
    string nama;

    do {
        cout << "\n============================================";
        cout << "\n FINAL PROJECT: SISTEM MONITORING KATEGORI";
        cout << "\n============================================";
        cout << "\n1. Tampilkan Hierarki Kategori";
        cout << "\n2. Tambah Kategori Baru ";
        cout << "\n3. Update Data Kategori";
        cout << "\n4. Hapus Kategori & Sub-Kategori";
        cout << "\n5. Urutkan Kategori";
        cout << "\n6. Pencarian Kategori";
        cout << "\n7. Monitoring Penggunaan Memori (RAM)";
        cout << "\n8. Export Benchmark Kedalaman level (CSV)";
        cout << "\n9. Export Benchmark Pertumbuhan Data (CSV)";
        cout << "\n0. Simpan & Keluar";
        cout << "\n--------------------------------------------";
        cout << "\nPilih Menu: "; cin >> pilihan;

        switch(pilihan) {
            case 1: {
                int pilihan_tampil;

                cout << "\n--- MENU TAMPILKAN DATA KATEGORI ---";
                cout << "\n1. Tampilkan Semua Hierarki";
                cout << "\n2. Tampilkan 50 Data Hierarki Teratas";
                cout << "\n3. Tampilkan Subkategori dari Kategori Tertentu";
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
                    int pilihan_subkategori;
                    int id_subkategori;

                    cout << "\n--- DAFTAR SUBKATEGORI YANG TERSEDIA ---\n";
                    tampilkanDaftarSubkategoriNonLeaf(root_kategori);

                    cout << "\n\nPilih jenis tampilan:";
                    cout << "\n1. Tampilkan semua subkategori/turunan";
                    cout << "\n2. Tampilkan data paling bawah/leaf";
                    cout << "\nPilihan: ";
                    cin >> pilihan_subkategori;

                    cout << "\nMasukkan ID kategori/subkategori: ";
                    cin >> id_subkategori;

                    if (pilihan_subkategori == 1)
                    {
                        tampilkanSemuaSubkategoriBerdasarkanKategori(id_subkategori);
                    }
                    else if (pilihan_subkategori == 2)
                    {
                        tampilkanDataLeafBerdasarkanSubkategori(id_subkategori);
                    }
                    else
                    {
                        cout << "[!] Pilihan subkategori tidak tersedia." << endl;
                    }
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
                cout << "Status (1:Aktif, 0:Nonaktif): "; cin >> status_input; // <-- Syarat Asdos

                if (status_input != 0 && status_input != 1) {
                    cout << "[!] Status tidak valid. Gunakan 1 untuk aktif atau 0 untuk nonaktif." << endl;
                    break;
                }

                if (tambahKategori(id, nama, p_id, status_input)) {
                    simpanData();
                    cout << "[OK] Kategori berhasil ditambahkan dan disimpan." << endl;
                    cout << "[INFO] Level otomatis: " << map_kategori[id]->level << endl;
                } else {
                    cout << "[!] Gagal menambahkan kategori. ID sudah ada atau Parent ID tidak ditemukan." << endl;
                }
                break;

            case 3: {
                int pilihan_update;

                cout << "\n--- UPDATE DATA KATEGORI ---";
                cout << "\nMasukkan ID yang diupdate: ";
                cin >> id;

                if (!map_kategori.count(id)) {
                    cout << "[!] ID tidak ditemukan." << endl;
                    break;
                }

                cout << "\nData saat ini:";
                cout << "\nID        : " << map_kategori[id]->id_kategori;
                cout << "\nNama      : " << map_kategori[id]->nama;
                cout << "\nParent ID : " << map_kategori[id]->parent_id;
                cout << "\nLevel     : " << map_kategori[id]->level;
                cout << "\nStatus    : " << ((map_kategori[id]->status == 1) ? "Aktif" : "Nonaktif");

                cout << "\n\nPilih data yang ingin diubah:";
                cout << "\n1. Ubah nama";
                cout << "\n2. Ubah status aktif/nonaktif";
                cout << "\n3. Pindah parent kategori";
                cout << "\nPilihan: ";
                cin >> pilihan_update;

                if (pilihan_update == 1) {
                    cout << "Nama Baru: ";
                    cin.ignore();
                    getline(cin, nama);

                    map_kategori[id]->nama = nama;
                    simpanData();

                    cout << "[OK] Nama kategori berhasil diperbarui." << endl;
                } else if (pilihan_update == 2) {
                    cout << "Status Baru (1:Aktif, 0:Nonaktif): ";
                    cin >> status_input;

                    if (status_input == 0 || status_input == 1) {
                        map_kategori[id]->status = status_input;
                        simpanData();

                        cout << "[OK] Status kategori berhasil diperbarui." << endl;
                    } else {
                        cout << "[!] Status tidak valid. Gunakan 1 untuk aktif atau 0 untuk nonaktif." << endl;
                    }
                } else if (pilihan_update == 3) {
                    int parent_baru;

                    cout << "Parent ID Baru (0 untuk root/kategori utama): ";
                    cin >> parent_baru;

                    if (pindahParentKategori(id, parent_baru)) {
                        simpanData();

                        cout << "[OK] Parent kategori berhasil diperbarui." << endl;
                        cout << "[INFO] Level kategori dan turunannya sudah disesuaikan otomatis." << endl;
                    } else {
                        cout << "[!] Gagal memindahkan parent. Parent tidak ada, sama dengan ID sendiri, atau menyebabkan siklus hierarki." << endl;
                    }
                } else {
                    cout << "[!] Pilihan update tidak tersedia." << endl;
                }

                break;
            }

            case 4:
                cout << "Masukkan ID yang akan dihapus: "; cin >> id;
                if(map_kategori.count(id)) {
                    hapusKategoriRecursive(id);
                    simpanData();
                    cout << "[OK] Kategori dan turunannya berhasil dihapus." << endl;
                } else cout << "[!] ID tidak ditemukan." << endl;
                break;

            case 5: {
                int pilihan_urut;
                cout << "\n--- MENU URUTKAN KATEGORI ---";
                cout << "\n1. Urutkan A-Z";
                cout << "\n2. Urutkan berdasarkan ID";
                cout << "\nPilih Urutan: ";
                cin >> pilihan_urut;

                if (pilihan_urut == 1) {
                    urutkanKategori(root_kategori);
                    simpanData();
                    cout << "[OK] Data diurutkan berdasarkan abjad." << endl;
                } else if (pilihan_urut == 2) {
                    urutkanKategoriById(root_kategori);
                    simpanData();
                    cout << "[OK] Data diurutkan berdasarkan ID." << endl;
                } else {
                    cout << "[!] Pilihan urutan tidak tersedia." << endl;
                }
                break;
            }

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
                    
                    Kategori* hasil = cariDenganNama(root_kategori, nama_target);

                    if (hasil) {
                        string nama_parent = "-";
                        if (hasil->parent_id == 0) {
                            nama_parent = "Root";
                        } else if (map_kategori.count(hasil->parent_id)) {
                            nama_parent = map_kategori[hasil->parent_id]->nama;
                        }

                        cout << "\n[DATA DITEMUKAN]" << endl;
                        cout << ">> ID Kategori   : " << hasil->id_kategori << endl;
                        cout << ">> Nama          : " << hasil->nama << endl;
                        cout << ">> Parent ID     : " << hasil->parent_id << endl;
                        cout << ">> Parent Nama   : " << nama_parent << endl;
                        cout << ">> Level/Depth   : " << hasil->level << endl;
                        cout << ">> Status        : " << ((hasil->status == 1) ? "Aktif" : "Nonaktif") << endl;
                    } else {
                        cout << "\n[!] Kategori dengan nama '" << nama_target << "' tidak ditemukan.\n";
                    }
                }
                break; // Menghindari bug fall-through ke case 7
            }

            case 7:
                hitungEstimasiMemori(); 
                break;    

            case 8:
                exportBenchmarkKeCSV();
                break;

            case 9:
                exportBenchmarkPertumbuhanDataKeCSV();
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
