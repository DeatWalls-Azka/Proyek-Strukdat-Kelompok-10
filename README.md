# Sistem Manajemen Kategori dan Monitoring Performa

Proyek akhir Praktikum Struktur Data - Topik 10: Sistem Manajemen Kategori dan Hierarki Data.

Program ini mengelola kategori dan subkategori dalam bentuk hierarki. Sistem menggunakan kombinasi N-ary Tree dan Hash Map untuk menampilkan data bertingkat, mencari kategori, menghapus kategori beserta turunannya, serta menganalisis performa pencarian dan penggunaan memori.

Proyek ini bertujuan untuk mensimulasikan sistem manajemen kategori pada skala industri menggunakan dataset dari UCI Online Retail. Dataset tersebut diolah menjadi data kategori hierarkis dengan lebih dari 5.700 data unik. Sistem membandingkan efisiensi waktu eksekusi antara pencarian DFS pada Tree dengan pencarian langsung menggunakan Hash Map.

## Anggota Kelompok

| Nama | NIM |
|---|---|
| Azka Julian Putra Wahyudi | M0403241029 |
| Aaliyah Nofarizki | M0403241011 |
| Asty Athetha Loethan | M0403241089 |
| Muhamad Raihan Pratama Putra Setyatmoko | M04032241064 |

## Struktur Data

| Struktur Data | Fungsi |
|---|---|
| N-ary Tree / Linked Tree | Menyimpan relasi parent-child kategori dan subkategori. |
| Hash Map / `unordered_map` | Mempercepat pencarian kategori berdasarkan ID. |

Perbandingan waktu pencarian dilakukan antara DFS pada Tree dan Hash Map. Perbandingan memori dilakukan antara Tree saja dan Tree + Hash Map karena Hash Map digunakan sebagai indeks tambahan.

## Domain Data

Setiap kategori memiliki atribut:

| Atribut | Tipe | Keterangan |
|---|---|---|
| ID Kategori | `int` | ID unik kategori. |
| Nama Kategori | `string` | Nama kategori atau produk. |
| Parent ID | `int` | ID induk kategori. Nilai 0 berarti root. |
| Level | `int` | Kedalaman kategori dalam tree. |
| Status | `int` | 1 untuk aktif, 0 untuk nonaktif. |
| Subkategori | `vector<Kategori*>` | Daftar child dari kategori. |

## Fitur Utama

- Load dan save data dari `dataset_kategori.txt`.
- Insert kategori baru dengan level otomatis.
- Validasi ID unik, Parent ID, dan status aktif/nonaktif.
- Update data kategori:
  - Ubah nama.
  - Ubah status.
  - Pindah parent kategori.
- Delete kategori beserta seluruh subkategori secara rekursif.
- Tampilkan seluruh hierarki kategori.
- Tampilkan 50 data hierarki teratas.
- Tampilkan semua subkategori/turunan dari kategori tertentu.
- Tampilkan data paling bawah atau leaf dari kategori tertentu.
- Search berdasarkan ID menggunakan DFS pada Tree dan Hash Map.
- Search berdasarkan nama menggunakan DFS.
- Sorting kategori A-Z secara rekursif.
- Benchmark waktu pencarian.
- Monitoring estimasi penggunaan memori.
- Export hasil benchmark ke `hasil_benchmark.csv`.
- Export benchmark pertumbuhan jumlah data ke `hasil_benchmark_pertumbuhan_data.csv`.

## File Penting

| File | Keterangan |
|---|---|
| `main.cpp` | Menu utama, benchmark, export CSV, dan input pengguna. |
| `ManagerKategori.cpp` | Implementasi struktur data dan operasi kategori. |
| `Kategori.h` | Struct `Kategori` dan deklarasi fungsi. |
| `dataset_kategori.txt` | Dataset utama yang digunakan program. |
| `dataset_kategori_backup.txt` | Dataset cadangan. |
| `hasil_benchmark.csv` | Hasil export benchmark untuk bahan grafik laporan. |
| `hasil_benchmark_pertumbuhan_data.csv` | Hasil benchmark dampak pertumbuhan jumlah data. |
| `BAHAN_LAPORAN.md` | Panduan tabel, grafik, analisis, dan kesimpulan laporan. |
| `RINGKASAN_PROYEK.md` | Ringkasan lengkap kondisi akhir proyek. |

## Menu Program

```text
1. Tampilkan Hierarki Kategori
2. Tambah Kategori Baru (+ Status)
3. Update Data Kategori
4. Hapus Kategori & Sub-Kategori
5. Urutkan Kategori (A-Z)
6. Uji Performa Pencarian (Benchmarking)
7. Monitoring Penggunaan Memori (RAM)
8. Export Benchmark ke CSV
9. Export Benchmark Pertumbuhan Data
0. Simpan & Keluar
```

## Cara Kompilasi

Pastikan compiler C++ seperti MinGW/GCC sudah tersedia.

```bash
g++ main.cpp ManagerKategori.cpp -o ProgramFinal.exe
```

## Cara Menjalankan

PowerShell Windows:

```powershell
.\ProgramFinal.exe
```

Terminal bash:

```bash
./ProgramFinal.exe
```

## Format Dataset

Dataset proyek bersumber dari UCI Online Retail yang sudah diolah menjadi format kategori dan subkategori.

Dataset disimpan dalam format:

```text
id_kategori;nama;parent_id;level;status
```

Contoh:

```text
1;Semua Produk;0;1;1
10;Home & Garden;1;2;1
100;Decor;10;3;1
101;Home Accessories;100;4;1
```

## Export Benchmark

Untuk membuat bahan tabel dan grafik berdasarkan level:

1. Jalankan program.
2. Pilih menu `8. Export Benchmark ke CSV`.
3. Buka `hasil_benchmark.csv` di Excel atau Google Sheets.
4. Buat grafik waktu pencarian dan penggunaan memori.

Benchmark CSV menggunakan 1.000 pengulangan untuk setiap data pada level yang diuji. Total pencarian berbeda pada tiap level karena jumlah data pada setiap level berbeda.

Kolom utama CSV:

```text
level
jumlah_data
total_pencarian
estimasi_node_dikunjungi_dfs
waktu_dfs_us
waktu_hash_us
rata_dfs_ns
rata_hash_ns
rasio_dfs_vs_hash
total_node
estimasi_memori_tree_kb
estimasi_memori_hash_index_kb
estimasi_memori_tree_hash_kb
```

Untuk membuat bahan tabel dan grafik pertumbuhan jumlah data:

1. Jalankan program.
2. Pilih menu `9. Export Benchmark Pertumbuhan Data`.
3. Buka `hasil_benchmark_pertumbuhan_data.csv` di Excel atau Google Sheets.

Benchmark pertumbuhan data menggunakan 10.000 pengulangan untuk target pada setiap ukuran data agar perbedaan waktu DFS pada Tree dan Hash Map terlihat lebih jelas.
Karena Hash Map sangat cepat, waktu Hash Map diukur dengan pengulangan internal yang lebih besar lalu dinormalisasi kembali agar setara dengan 10.000 pencarian.

Kolom utama CSV pertumbuhan data:

```text
jumlah_data
id_target
pengulangan
total_pencarian
waktu_dfs_us
waktu_hash_us
rata_dfs_ns
rata_hash_ns
rasio_dfs_vs_hash
estimasi_memori_tree_kb
estimasi_memori_hash_index_kb
estimasi_memori_tree_hash_kb
```

## Analisis Singkat

DFS pada Tree cocok untuk menelusuri hierarki, tetapi pencarian ID memiliki kompleksitas O(n). Hash Map memiliki kompleksitas rata-rata O(1), sehingga lebih cepat untuk pencarian berdasarkan ID.

Trade-off utama sistem adalah penggunaan memori. Tree saja membutuhkan memori lebih kecil, sedangkan Tree + Hash Map membutuhkan memori tambahan untuk indeks. Namun, tambahan memori tersebut memberi keuntungan berupa pencarian ID yang jauh lebih cepat.

## Rekomendasi

Struktur data yang direkomendasikan adalah hybrid N-ary Tree + Hash Map:

- N-ary Tree digunakan untuk menyimpan dan menampilkan hierarki kategori.
- Hash Map digunakan untuk mempercepat pencarian ID dan validasi parent.

Pendekatan ini paling sesuai untuk sistem manajemen kategori dan hierarki data sederhana.
