# Ringkasan Proyek Struktur Data

## Identitas Proyek

Nama proyek: Sistem Manajemen Kategori dan Monitoring Performa

Topik: Topik 10 - Sistem Manajemen Kategori dan Hierarki Data

Bahasa pemrograman: C++

Dataset: Data kategori produk yang disusun ke dalam format hierarki kategori dan subkategori.

## Gambaran Umum

Proyek ini membuat program berbasis terminal untuk mengelola kategori produk dalam bentuk hierarki. Sistem menggunakan struktur data hybrid:

1. N-ary Tree / Linked Tree
   - Menyimpan relasi parent-child antara kategori dan subkategori.
   - Cocok untuk menampilkan struktur hierarki, subkategori, dan proses delete rekursif.

2. Hash Map / `unordered_map`
   - Menyimpan pointer kategori berdasarkan ID.
   - Digunakan untuk pencarian ID, validasi parent, update, dan delete dengan akses cepat.

Perbandingan performa pencarian dilakukan antara DFS pada Tree dan pencarian langsung menggunakan Hash Map. Untuk memori, perbandingan dilakukan antara Tree saja dan Tree + Hash Map karena hash map berfungsi sebagai indeks tambahan.

## File Proyek

| File | Fungsi |
|---|---|
| `Kategori.h` | Berisi struct `Kategori`, deklarasi storage global, dan prototype fungsi. |
| `ManagerKategori.cpp` | Berisi implementasi load/save, insert, update parent, delete rekursif, search, sorting, tampilan hierarki, subkategori, dan estimasi memori. |
| `main.cpp` | Berisi menu utama program, input pengguna, benchmark pencarian, export CSV, dan pemanggilan fitur. |
| `dataset_kategori.txt` | Dataset utama yang dibaca dan disimpan program. |
| `dataset_kategori_backup.txt` | Cadangan dataset. |
| `hasil_benchmark.csv` | Hasil export benchmark otomatis untuk bahan tabel dan grafik laporan. |
| `BAHAN_LAPORAN.md` | Panduan isi laporan akhir, tabel, grafik, kesimpulan, dan penempatan bab. |
| `README.md` | Dokumentasi penggunaan program. |
| `ProgramFinal.exe` | File executable hasil kompilasi. |

## Domain Data

Struct utama yang digunakan adalah `Kategori`.

| Atribut | Tipe Data | Keterangan |
|---|---|---|
| `id_kategori` | `int` | ID unik setiap kategori. |
| `nama` | `string` | Nama kategori atau produk. |
| `parent_id` | `int` | ID induk kategori. Nilai 0 berarti root. |
| `level` | `int` | Kedalaman kategori dalam tree. |
| `status` | `int` | Status data, 1 untuk aktif dan 0 untuk nonaktif. |
| `sub_kategori` | `vector<Kategori*>` | Daftar anak/subkategori dari kategori tersebut. |

Storage global:

| Storage | Fungsi |
|---|---|
| `vector<Kategori*> root_kategori` | Menyimpan kategori root. |
| `unordered_map<int, Kategori*> map_kategori` | Indeks ID kategori untuk akses cepat. |

## Struktur Data dan Kompleksitas

| Operasi | DFS pada Tree | Hash Map | Keterangan |
|---|---:|---:|---|
| Insert kategori | O(1) setelah parent ditemukan | O(1) rata-rata | Parent dicari melalui hash map. |
| Search berdasarkan ID | O(n) | O(1) rata-rata | DFS menelusuri node, hash map akses langsung ID. |
| Search berdasarkan nama | O(n) | - | Nama dicari dengan DFS. |
| Tampilkan hierarki | O(n) | - | Semua node dikunjungi. |
| Tampilkan subkategori/turunan | O(k) | O(1) untuk menemukan target | k adalah jumlah turunan kategori terpilih. |
| Delete kategori beserta turunan | O(k) | O(k) | Semua turunan dihapus dari tree dan hash map. |
| Update parent kategori | O(k) | O(1) untuk akses node | Validasi siklus dan update level turunan. |

## Fitur Program

### 1. Load dan Save Dataset

Program membaca dan menyimpan data ke `dataset_kategori.txt`.

Format dataset:

```text
id_kategori;nama;parent_id;level;status
```

Data otomatis disimpan setelah operasi tambah, update, delete, dan sorting.

### 2. Tambah Kategori Baru

Input yang diminta:

- ID kategori
- Nama kategori
- Parent ID
- Status aktif/nonaktif

Level tidak lagi diinput manual. Level dihitung otomatis:

```text
Parent ID = 0  -> level 1
Parent level x -> kategori baru level x + 1
```

Validasi yang sudah ada:

- ID tidak boleh duplikat.
- Parent ID harus ada, kecuali 0 untuk root.
- Status hanya boleh 0 atau 1.

### 3. Update Data Kategori

Menu update berisi:

```text
1. Ubah nama
2. Ubah status aktif/nonaktif
3. Pindah parent kategori
```

Pada pindah parent, program mencegah:

- Parent ID tidak ditemukan.
- Kategori menjadi parent dirinya sendiri.
- Kategori dipindahkan ke turunannya sendiri.

Level kategori dan seluruh turunannya akan disesuaikan otomatis setelah pindah parent.

### 4. Hapus Kategori dan Subkategori

Program menghapus kategori beserta seluruh turunannya secara rekursif. Data juga dihapus dari hash map agar tidak ada data yatim.

### 5. Tampilkan Hierarki Kategori

Mode tampilan:

```text
1. Tampilkan semua hierarki
2. Tampilkan 50 data hierarki teratas
3. Tampilkan subkategori dari kategori tertentu
```

Pada menu subkategori, tersedia:

```text
1. Tampilkan semua subkategori/turunan
2. Tampilkan data paling bawah/leaf
```

### 6. Urutkan Kategori A-Z

Sorting dilakukan secara rekursif pada setiap level tree berdasarkan nama kategori.

### 7. Search Kategori

Pencarian berdasarkan ID:

- DFS pada Tree
- Hash Map

Pencarian berdasarkan nama:

- DFS pada Tree

### 8. Benchmark Performa

Program membandingkan waktu pencarian ID antara:

```text
DFS pada Tree
Hash Map
```

Hasil benchmark dapat ditampilkan di terminal dan diekspor ke CSV.

### 9. Monitoring Memori

Menu memori menampilkan:

```text
Estimasi RAM Tree saja
Estimasi RAM Hash Map index
Estimasi RAM Tree + Hash Map
```

Kesimpulan memori: Hash Map membutuhkan memori tambahan, tetapi mempercepat pencarian ID.

### 10. Export Benchmark ke CSV

Menu:

```text
8. Export Benchmark ke CSV
```

Output:

```text
hasil_benchmark.csv
```

Kolom CSV:

```text
level
jumlah_data
total_pencarian
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

CSV ini digunakan untuk membuat tabel dan grafik pada laporan.

## Menu Program Final

```text
1. Tampilkan Hierarki Kategori
2. Tambah Kategori Baru (+ Status)
3. Update Data Kategori
4. Hapus Kategori & Sub-Kategori
5. Urutkan Kategori (A-Z)
6. Uji Performa Pencarian (Benchmarking)
7. Monitoring Penggunaan Memori (RAM)
8. Export Benchmark ke CSV
0. Simpan & Keluar
```

## Bahan Laporan

Bagian laporan yang sudah disiapkan di `BAHAN_LAPORAN.md`:

- Struktur data yang dibandingkan.
- Kompleksitas Big-O.
- Skenario eksperimen.
- Tabel dan grafik yang dimasukkan ke laporan.
- Penempatan tabel/grafik pada bab laporan.
- Analisis trade-off.
- Kesimpulan dan rekomendasi.

Tabel/grafik yang disarankan:

```text
Tabel 1: Hasil pengujian manual berdasarkan posisi data
Grafik 1: Grafik dari Tabel 1
Tabel 2: Hasil benchmark otomatis dari CSV berdasarkan level
Grafik 2: Grafik waktu DFS vs Hash Map dari CSV
Grafik 3: Grafik rata-rata waktu dari CSV
Grafik 4: Grafik memori dari CSV
```

## Cara Kompilasi

```bash
g++ main.cpp ManagerKategori.cpp -o ProgramFinal.exe
```

## Cara Menjalankan

```bash
./ProgramFinal.exe
```

Pada PowerShell Windows:

```powershell
.\ProgramFinal.exe
```

## Kesimpulan Proyek

Sistem sudah memenuhi kebutuhan proyek Topik 10 karena mampu mengelola data kategori bertingkat menggunakan minimal dua struktur data, yaitu N-ary Tree dan Hash Map. Tree digunakan untuk menyimpan dan menampilkan hierarki, sedangkan Hash Map digunakan untuk mempercepat pencarian ID.

Hasil analisis menunjukkan bahwa Hash Map lebih cepat untuk pencarian ID karena memiliki kompleksitas rata-rata O(1), sedangkan DFS pada Tree memiliki kompleksitas O(n). Namun, penggunaan Hash Map membutuhkan memori tambahan karena menjadi indeks di atas struktur Tree. Oleh karena itu, struktur hybrid N-ary Tree + Hash Map direkomendasikan untuk sistem manajemen kategori dan hierarki data.
