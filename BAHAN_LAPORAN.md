# Bahan Laporan Akhir

## Judul

Sistem Manajemen Kategori dan Hierarki Data Menggunakan Tree dan Hash Map

## Ringkasan Masalah

Data kategori pada sistem e-commerce, arsip digital, dan manajemen konten biasanya tersusun secara bertingkat. Jika data hanya disimpan dalam daftar datar, proses pencarian, penelusuran subkategori, dan penghapusan kategori beserta turunannya dapat menjadi lambat dan rawan kesalahan. Proyek ini membangun sistem manajemen kategori sederhana dengan fokus pada pemilihan struktur data dan analisis performanya.

## Struktur Data yang Dibandingkan

| Struktur Data | Peran dalam Sistem | Kelebihan | Kekurangan |
|---|---|---|---|
| N-ary Tree | Menyimpan relasi kategori dan subkategori | Alami untuk data hierarki, mudah menampilkan struktur bertingkat | Pencarian perlu traversal DFS sehingga dapat lambat pada data besar |
| Hash Map / unordered_map | Menyimpan pointer kategori berdasarkan ID | Pencarian ID sangat cepat dengan kompleksitas rata-rata O(1) | Membutuhkan memori tambahan |

## Kompleksitas Teoretis

| Operasi | Tree / DFS | Hash Map | Keterangan |
|---|---:|---:|---|
| Insert kategori | O(1) jika parent ditemukan lewat hash map | O(1) rata-rata | Sistem memakai hash map untuk menemukan parent |
| Search berdasarkan ID | O(n) | O(1) rata-rata | DFS harus menelusuri node satu per satu |
| Search berdasarkan nama | O(n) | - | Nama tidak dijadikan key hash map |
| Tampilkan hierarki | O(n) | - | Semua node harus dikunjungi |
| Tampilkan subkategori/turunan | O(k) | O(1) untuk menemukan root subkategori | k adalah jumlah turunan kategori terpilih |
| Delete kategori beserta turunan | O(k) | O(k) | Semua turunan harus dihapus dari tree dan hash map |
| Update parent | O(k) | O(1) untuk akses node | k dipakai untuk validasi turunan dan update level |

Tabel average case dan worst case yang dapat dimasukkan pada bagian teori:

| Operasi | Average Case | Worst Case | Keterangan |
|---|---:|---:|---|
| Search DFS pada Tree | O(n) | O(n) | Terburuk saat data berada di node terakhir atau tidak ditemukan |
| Search Hash Map | O(1) | O(n) | Terburuk saat banyak collision |
| Delete Subtree | O(k) | O(n) | Jika subtree yang dihapus mencakup hampir semua data |
| Tampilkan Hierarki | O(n) | O(n) | Semua node harus dikunjungi |

## Fitur Sistem

- Insert kategori baru dengan level otomatis.
- Menambahkan subkategori ke kategori tertentu.
- Search kategori berdasarkan ID menggunakan DFS dan hash map.
- Search kategori berdasarkan nama menggunakan DFS.
- Menampilkan seluruh struktur hierarki kategori.
- Menampilkan semua subkategori/turunan dari kategori tertentu.
- Menampilkan data paling bawah atau leaf dari kategori tertentu.
- Update nama, status, dan parent kategori.
- Delete kategori beserta seluruh subkategorinya.
- Sorting kategori A-Z secara rekursif.
- Monitoring estimasi penggunaan memori.
- Export hasil benchmark ke `hasil_benchmark.csv`.

## Skenario Pengujian

Pengujian dilakukan dengan menjalankan pencarian ID pada beberapa level hierarki, yaitu level 2 sampai level 5. Setiap ID pada level tersebut dicari 1.000 kali menggunakan dua metode:

1. DFS pada tree.
2. Hash map berdasarkan ID.

Hasil pengujian berdasarkan level diekspor ke file `hasil_benchmark.csv` melalui menu:

```text
8. Export Benchmark ke CSV
```

Hasil pengujian pertumbuhan jumlah data diekspor ke file `hasil_benchmark_pertumbuhan_data.csv` melalui menu:

```text
9. Export Benchmark Pertumbuhan Data
```

Kolom CSV:

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

## Tabel dan Grafik Final yang Disarankan

Tabel 1: Rata-rata waktu pencarian berdasarkan pertumbuhan jumlah data

- Kolom: `jumlah_data`, `id_target`, `rata_dfs_us`, `rata_hash_us`

Grafik 1: Rata-rata waktu pencarian DFS pada Tree dan Hash Map berdasarkan pertumbuhan jumlah data

- Sumbu X: jumlah_data
- Sumbu Y: rata-rata waktu pencarian dalam mikrodetik per pencarian
- Seri data: `rata_dfs_us` dan `rata_hash_us`

Tabel 2: Rata-rata waktu pencarian berdasarkan kedalaman level

- Kolom: `level`, `jumlah_data`, `total_pencarian`, `rata_dfs_us`, `rata_hash_us`

Grafik 2: Rata-rata waktu pencarian DFS pada Tree dan Hash Map berdasarkan level

- Sumbu X: level
- Sumbu Y: rata-rata waktu pencarian dalam mikrodetik per pencarian
- Seri data: `rata_dfs_us` dan `rata_hash_us`

Tabel 3: Penggunaan memori berdasarkan total data 5.694

- Kolom: Total Data, RAM Tree Saja (KB), RAM Tree + Hash Map (KB)

Grafik 3: Perbandingan penggunaan memori Tree Saja dan Tree + Hash Map

- Sumbu X: struktur data
- Sumbu Y: memori dalam KB
- Data: Tree Saja dan Tree + Hash Map

## Tabel dan Grafik yang Dimasukkan ke Laporan

### Penempatan pada Struktur Laporan

Gunakan pembagian berikut agar tabel dan grafik tidak menumpuk di satu bagian.

Masukkan ke Bab 7 `Eksperimen & Pengujian`:

| Item | Letak | Tujuan |
|---|---|---|
| Skenario benchmark pertumbuhan jumlah data | Bab 7 | Menjelaskan variasi ukuran data yang diuji |
| Skenario benchmark otomatis berdasarkan level | Bab 7 | Menjelaskan level 2 sampai level 5 dan jumlah pengulangan |
| Metode pengukuran waktu dan memori | Bab 7 | Menjelaskan penggunaan `chrono`, jumlah iterasi, dan estimasi memori |
| Replikasi eksperimen | Bab 7 | Menjelaskan bahwa pencarian dilakukan berulang agar hasil lebih stabil |

Masukkan ke Bab 8 `Hasil & Analisis`:

| Item | Letak | Keterangan |
|---|---|---|
| Tabel 1. Hasil benchmark pertumbuhan jumlah data | Bab 8 | Tabel rata-rata waktu DFS pada Tree vs Hash Map untuk beberapa ukuran data |
| Grafik 1. Grafik rata-rata waktu pencarian berdasarkan pertumbuhan jumlah data | Bab 8 | Visualisasi dampak pertumbuhan jumlah data terhadap rata-rata waktu pencarian |
| Tabel 2. Hasil benchmark otomatis dari CSV berdasarkan level | Bab 8 | Tabel rata-rata waktu pencarian berdasarkan level |
| Grafik 2. Grafik rata-rata waktu DFS vs Hash Map berdasarkan level | Bab 8 | Visualisasi rata-rata waktu pencarian per level |
| Tabel 3. Penggunaan memori berdasarkan total data 5.694 | Bab 8 | Tabel ringkas penggunaan memori Tree saja dan Tree + Hash Map |
| Grafik 3. Grafik memori Tree saja vs Tree + Hash Map | Bab 8 | Visualisasi penggunaan memori pada total data akhir |
| Analisis insert/search/delete | Bab 8 | Jelaskan hasil operasi sistem dan kompleksitasnya |
| Analisis penggunaan memori | Bab 8 | Jelaskan tambahan memori dari Hash Map index |
| Diskusi trade-off | Bab 8 | Jelaskan pertukaran antara kecepatan pencarian dan penggunaan memori |

Masukkan ke Bab 9 `Kesimpulan & Rekomendasi`:

| Item | Letak | Keterangan |
|---|---|---|
| Ringkasan hasil utama | Bab 9 | Hash Map lebih cepat untuk search ID, Tree tetap cocok untuk hierarki |
| Struktur data paling optimal | Bab 9 | Rekomendasi hybrid N-ary Tree + Hash Map |
| Saran pengembangan | Bab 9 | Misalnya pencarian nama case-insensitive, GUI, atau integrasi database sederhana |

Masukkan ke Bab 12 `Lampiran` jika diperlukan:

| Item | Letak | Keterangan |
|---|---|---|
| File `hasil_benchmark.csv` | Lampiran | Data mentah hasil benchmark otomatis |
| File `hasil_benchmark_pertumbuhan_data.csv` | Lampiran | Data mentah benchmark pertumbuhan jumlah data |
| Potongan kode benchmark | Lampiran | Fungsi export benchmark atau fungsi pencarian |
| Screenshot demo aplikasi | Lampiran | Bukti fitur berjalan |

### Tabel 1: Rata-rata Waktu Pencarian Berdasarkan Pertumbuhan Jumlah Data

Sumber data: `hasil_benchmark_pertumbuhan_data.csv` dari menu 9.

Judul tabel:

```text
Tabel 1. Rata-rata Waktu Pencarian Berdasarkan Pertumbuhan Jumlah Data
```

Nama kolom:

```text
jumlah_data
id_target
rata_dfs_us
rata_hash_us
```

Kolom `rata_dfs_us` dan `rata_hash_us` dapat dihitung dari CSV:

```text
rata_dfs_us = waktu_dfs_us / pengulangan
rata_hash_us = waktu_hash_us / pengulangan
```

Kolom lain seperti `pengulangan`, `total_pencarian`, `estimasi_node_dikunjungi_dfs`, `waktu_dfs_us`, `waktu_hash_us`, `rata_dfs_ns`, `rata_hash_ns`, dan `rasio_dfs_vs_hash` boleh disimpan di lampiran jika laporan utama perlu dibuat lebih ringkas.

Ukuran data yang digunakan menyesuaikan jumlah data tersedia, misalnya:

```text
1.000
2.000
3.000
4.000
5.000
total data
```

Benchmark pertumbuhan jumlah data menggunakan 10.000 pengulangan pencarian target untuk setiap ukuran data.
Karena Hash Map sangat cepat, waktu Hash Map diukur dengan pengulangan internal yang lebih besar lalu dinormalisasi kembali agar setara dengan 10.000 pencarian.

Kesimpulan Tabel 1:

```text
Hasil pengujian berdasarkan pertumbuhan jumlah data menunjukkan bahwa rata-rata waktu pencarian DFS pada Tree meningkat ketika jumlah data yang diuji semakin besar. Hal ini terjadi karena DFS harus menelusuri node satu per satu. Sebaliknya, rata-rata waktu pencarian Hash Map relatif lebih stabil karena pencarian dilakukan langsung berdasarkan ID.
```

### Grafik 1: Rata-rata Waktu Pencarian Berdasarkan Pertumbuhan Jumlah Data

Sumber data: `hasil_benchmark_pertumbuhan_data.csv`.

Judul grafik:

```text
Grafik 1. Rata-rata Waktu Pencarian DFS pada Tree dan Hash Map Berdasarkan Pertumbuhan Jumlah Data
```

Sumbu dan seri data:

```text
Sumbu X: jumlah_data
Sumbu Y: Rata-rata Waktu Pencarian (mks/pencarian)
Seri 1: rata_dfs_us
Seri 2: rata_hash_us
```

Kesimpulan Grafik 1:

```text
Grafik menunjukkan bahwa rata-rata waktu pencarian DFS pada Tree meningkat seiring bertambahnya jumlah data. Hash Map memiliki rata-rata waktu pencarian yang lebih kecil dan relatif stabil karena akses data dilakukan melalui indeks ID.
```

### Tabel 2: Rata-rata Waktu Pencarian Berdasarkan Kedalaman Level

Sumber data: tabel manual yang sudah dibuat atau hasil `hasil_benchmark.csv`.

Judul tabel:

```text
Tabel 2. Rata-rata Waktu Pencarian Berdasarkan Kedalaman Level
```

Nama kolom versi manual:

```text
Skenario Kedalaman
ID Target
Rata-rata DFS pada Tree (mks/pencarian)
Rata-rata Hash Map (mks/pencarian)
```

Nama kolom jika memakai CSV:

```text
level
jumlah_data
total_pencarian
rata_dfs_us
rata_hash_us
```

Kolom `rata_dfs_us` dan `rata_hash_us` dapat dihitung dari CSV:

```text
rata_dfs_us = waktu_dfs_us / total_pencarian
rata_hash_us = waktu_hash_us / total_pencarian
```

Kolom `waktu_dfs_us`, `waktu_hash_us`, `rata_dfs_ns`, `rata_hash_ns`, dan `rasio_dfs_vs_hash` boleh disimpan di lampiran jika tidak digunakan di tabel utama.

Catatan satuan:

```text
CSV memakai satuan mikrodetik pada kolom waktu_dfs_us dan waktu_hash_us.
Untuk tabel utama, gunakan rata-rata waktu dalam mks/pencarian agar perbandingan antarlevel lebih adil.
Benchmark otomatis memakai 1.000 pengulangan untuk setiap data pada level yang diuji.
Total pencarian berbeda pada tiap level karena jumlah data pada tiap level berbeda.
```

Kesimpulan Tabel 2:

```text
Pengujian berdasarkan level menunjukkan bahwa rata-rata waktu pencarian DFS pada Tree lebih besar dibanding Hash Map pada setiap level. Hal ini sesuai dengan kompleksitas DFS sebesar O(n), sedangkan Hash Map memiliki kompleksitas rata-rata O(1). Hasil ini memperkuat bahwa Hash Map lebih efisien untuk pencarian berdasarkan ID.
```

### Grafik 2: Rata-rata Waktu Pencarian Berdasarkan Level

Sumber data: `hasil_benchmark.csv`.

Judul grafik:

```text
Grafik 2. Rata-rata Waktu Pencarian DFS pada Tree dan Hash Map Berdasarkan Level
```

Sumbu dan seri data:

```text
Sumbu X: level
Sumbu Y: Rata-rata Waktu Pencarian (mks/pencarian)
Seri 1: rata_dfs_us
Seri 2: rata_hash_us
```

Kesimpulan Grafik 2:

```text
Grafik memperlihatkan bahwa rata-rata waktu pencarian menggunakan DFS pada Tree lebih tinggi dibandingkan Hash Map pada berbagai level kategori. Hash Map tetap lebih cepat karena pencarian dilakukan melalui indeks ID, sedangkan DFS perlu melakukan traversal pada struktur tree.
```

### Tabel 3: Penggunaan Memori Berdasarkan Total Data 5.694

Judul tabel:

```text
Tabel 3. Penggunaan Memori Berdasarkan Total Data 5.694
```

Nama kolom:

```text
Total Data
RAM Tree Saja (KB)
RAM Tree + Hash Map (KB)
```

Contoh isi tabel:

```text
5.694 | 444.766 | 556.055
```

### Grafik 3: Perbandingan Penggunaan Memori Tree Saja dan Tree + Hash Map

Judul grafik:

```text
Grafik 3. Perbandingan Penggunaan Memori Tree Saja dan Tree + Hash Map
```

Sumbu dan seri data:

```text
Sumbu X: Struktur Data
Sumbu Y: Memori (KB)
Data 1: Tree Saja
Data 2: Tree + Hash Map
```

Kalimat analisis:

```text
Berdasarkan grafik, penggunaan memori pada struktur Tree + Hash Map lebih besar dibanding Tree saja. Hal ini terjadi karena Hash Map digunakan sebagai indeks tambahan untuk mempercepat pencarian ID kategori. Meskipun membutuhkan tambahan memori, struktur hybrid memberikan keuntungan berupa pencarian yang lebih cepat.
```

## Kesimpulan Akhir untuk Bagian Hasil dan Analisis

Kesimpulan yang dapat dimasukkan:

```text
Berdasarkan hasil pengujian, pencarian menggunakan Hash Map lebih cepat dibandingkan pencarian DFS pada Tree. Pada pencarian DFS, waktu eksekusi dipengaruhi oleh jumlah data dan kedalaman data karena proses pencarian dilakukan dengan menelusuri node satu per satu. Sebaliknya, Hash Map dapat mengakses data secara langsung berdasarkan ID sehingga waktu pencarian relatif lebih stabil.

Dari sisi memori, struktur hybrid Tree + Hash Map membutuhkan memori lebih besar dibanding Tree saja karena Hash Map digunakan sebagai indeks tambahan. Namun, tambahan memori tersebut memberikan keuntungan berupa pencarian ID yang jauh lebih cepat. Oleh karena itu, kombinasi N-ary Tree dan Hash Map menjadi struktur data yang paling sesuai untuk sistem manajemen kategori dan hierarki data ini.
```

## Analisis yang Bisa Ditulis

Berdasarkan teori kompleksitas, pencarian menggunakan hash map lebih efisien dibanding DFS karena hash map memiliki kompleksitas rata-rata O(1), sedangkan DFS memiliki kompleksitas O(n). Pada DFS, pencarian harus menelusuri node dari root hingga data ditemukan. Semakin besar jumlah kategori dan semakin dalam hierarki, waktu pencarian DFS dapat meningkat.

Tree tetap diperlukan karena struktur ini merepresentasikan hubungan parent-child secara alami. Operasi seperti menampilkan hierarki, menampilkan subkategori, dan menghapus kategori beserta turunannya lebih mudah dijelaskan dan diimplementasikan menggunakan tree.

Hash map memberikan keuntungan besar untuk pencarian berdasarkan ID dan validasi parent saat insert/update. Namun, penggunaan hash map membutuhkan memori tambahan karena sistem tetap menyimpan tree sebagai struktur utama, lalu menambahkan indeks ID ke pointer kategori.

Dengan demikian, struktur hybrid tree dan hash map menjadi pilihan yang paling sesuai untuk sistem ini. Tree digunakan untuk manajemen hierarki, sedangkan hash map digunakan untuk optimasi pencarian.

## Rekomendasi

Struktur data yang direkomendasikan adalah kombinasi N-ary Tree dan Hash Map. Tree cocok untuk menyimpan data bertingkat, sedangkan hash map cocok untuk mempercepat pencarian ID dan validasi relasi parent-child. Pendekatan hybrid ini lebih seimbang dibanding hanya menggunakan tree atau hanya menggunakan daftar datar.

## Cara Mengambil Data untuk Laporan

1. Tutup `ProgramFinal.exe` jika masih berjalan.
2. Compile program:

```bash
g++ main.cpp ManagerKategori.cpp -o ProgramFinal.exe
```

3. Jalankan program:

```bash
./ProgramFinal.exe
```

4. Pilih menu:

```text
8. Export Benchmark ke CSV
```

5. Buka `hasil_benchmark.csv` di Excel atau Google Sheets.
6. Buat grafik dari kolom waktu DFS, waktu Hash Map, dan estimasi memori.
