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

Pengujian dilakukan dengan menjalankan pencarian ID pada beberapa level hierarki, yaitu level 2 sampai level 5. Setiap ID pada level tersebut dicari beberapa kali menggunakan dua metode:

1. DFS pada tree.
2. Hash map berdasarkan ID.

Hasil pengujian diekspor ke file `hasil_benchmark.csv` melalui menu:

```text
8. Export Benchmark ke CSV
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

## Grafik yang Disarankan

Grafik 1: Perbandingan waktu pencarian DFS vs Hash Map

- Sumbu X: level
- Sumbu Y: waktu eksekusi dalam mikrodetik
- Seri data: `waktu_dfs_us` dan `waktu_hash_us`

Grafik 2: Rata-rata waktu pencarian per operasi

- Sumbu X: level
- Sumbu Y: rata-rata waktu dalam nanodetik
- Seri data: `rata_dfs_ns` dan `rata_hash_ns`

Grafik 3: Estimasi penggunaan memori

- Sumbu X: struktur data
- Sumbu Y: memori dalam KB
- Seri data utama: `estimasi_memori_tree_kb` dan `estimasi_memori_tree_hash_kb`
- `estimasi_memori_hash_index_kb` dapat dijelaskan sebagai memori tambahan untuk indeks hash map

## Tabel dan Grafik yang Dimasukkan ke Laporan

### Penempatan pada Struktur Laporan

Gunakan pembagian berikut agar tabel dan grafik tidak menumpuk di satu bagian.

Masukkan ke Bab 7 `Eksperimen & Pengujian`:

| Item | Letak | Tujuan |
|---|---|---|
| Skenario pengujian manual berdasarkan posisi data | Bab 7 | Menjelaskan target data paling atas, tengah, dan paling bawah |
| Skenario benchmark otomatis berdasarkan level | Bab 7 | Menjelaskan level 2 sampai level 5 dan jumlah pengulangan |
| Metode pengukuran waktu dan memori | Bab 7 | Menjelaskan penggunaan `chrono`, jumlah iterasi, dan estimasi memori |
| Replikasi eksperimen | Bab 7 | Menjelaskan bahwa pencarian dilakukan berulang agar hasil lebih stabil |

Masukkan ke Bab 8 `Hasil & Analisis`:

| Item | Letak | Keterangan |
|---|---|---|
| Tabel 1. Hasil pengujian manual berdasarkan posisi data | Bab 8 | Tabel hasil waktu DFS pada Tree vs Hash Map untuk posisi atas, tengah, bawah |
| Grafik 1. Grafik dari Tabel 1 | Bab 8 | Visualisasi pengaruh posisi data terhadap waktu pencarian |
| Tabel 2. Hasil benchmark otomatis dari CSV berdasarkan level | Bab 8 | Tabel hasil ekspor `hasil_benchmark.csv` |
| Grafik 2. Grafik waktu DFS vs Hash Map dari CSV | Bab 8 | Visualisasi total waktu pencarian per level |
| Grafik 3. Grafik rata-rata waktu dari CSV | Bab 8 | Visualisasi rata-rata waktu pencarian per operasi |
| Grafik 4. Grafik memori dari CSV | Bab 8 | Visualisasi penggunaan memori Tree saja vs Tree + Hash Map |
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
| Potongan kode benchmark | Lampiran | Fungsi export benchmark atau fungsi pencarian |
| Screenshot demo aplikasi | Lampiran | Bukti fitur berjalan |

### Tabel 1: Perbandingan Waktu Pencarian Berdasarkan Posisi Data

Sumber data: tabel manual yang sudah dibuat di spreadsheet.

Judul tabel:

```text
Tabel 1. Perbandingan Waktu Eksekusi Pencarian Berdasarkan Posisi Data dalam Hierarki
```

Nama kolom:

```text
Skenario
ID Target
Waktu DFS pada Tree (mks)
Waktu Hash Map (mks)
RAM Tree Saja (KB)
RAM Tree + Hash Map (KB)
```

Isi skenario:

```text
Paling atas
Tengah
Paling bawah
```

Catatan penyesuaian:

- Jika data RAM lama masih memakai kolom `RAM Tree (KB)` dan `RAM Hash (KB)`, sebaiknya ubah labelnya.
- Untuk laporan final, gunakan istilah `RAM Tree Saja (KB)` dan `RAM Tree + Hash Map (KB)`.
- Jika nilai `RAM Hash (KB)` hanya menunjukkan memori indeks hash map, jelaskan sebagai `RAM Hash Map Index (KB)`, bukan sebagai pengganti tree.

Kesimpulan Tabel 1:

```text
Hasil pengujian berdasarkan posisi data menunjukkan bahwa waktu pencarian DFS pada Tree dipengaruhi oleh letak data dalam hierarki. Data yang berada semakin jauh dari root membutuhkan waktu pencarian lebih besar karena DFS harus menelusuri lebih banyak node. Sebaliknya, pencarian menggunakan Hash Map relatif stabil karena akses dilakukan langsung berdasarkan ID.
```

### Grafik 1: Perbandingan Waktu Pencarian Berdasarkan Posisi Data

Sumber data: Tabel 1 dari spreadsheet manual.

Judul grafik:

```text
Grafik 1. Perbandingan Waktu Pencarian DFS pada Tree dan Hash Map Berdasarkan Posisi Data
```

Sumbu dan seri data:

```text
Sumbu X: Skenario / ID Target
Sumbu Y: Waktu Eksekusi (mks)
Seri 1: Waktu DFS pada Tree (mks)
Seri 2: Waktu Hash Map (mks)
```

Kesimpulan Grafik 1:

```text
Grafik menunjukkan bahwa waktu pencarian DFS pada Tree meningkat ketika target berada pada posisi tengah dan paling bawah. Hash Map memiliki waktu pencarian yang jauh lebih kecil dan lebih stabil karena tidak bergantung pada posisi node dalam hierarki.
```

### Tabel 2: Perbandingan Waktu Pencarian Berdasarkan Kedalaman Level

Sumber data: tabel manual yang sudah dibuat atau hasil `hasil_benchmark.csv`.

Judul tabel:

```text
Tabel 2. Perbandingan Waktu Eksekusi Pencarian Berdasarkan Kedalaman Level
```

Nama kolom versi manual:

```text
Skenario Kedalaman
ID Target
Waktu DFS pada Tree (mks)
Waktu Hash Map (mks)
```

Nama kolom jika memakai CSV:

```text
level
jumlah_data
total_pencarian
waktu_dfs_us
waktu_hash_us
rata_dfs_ns
rata_hash_ns
rasio_dfs_vs_hash
```

Catatan satuan:

```text
CSV memakai satuan mikrodetik pada kolom waktu_dfs_us dan waktu_hash_us.
Dalam laporan, boleh ditulis sebagai mks/mikrodetik agar konsisten dengan tabel manual.
```

Kesimpulan Tabel 2:

```text
Pengujian berdasarkan level menunjukkan bahwa DFS pada Tree membutuhkan waktu lebih besar dibanding Hash Map pada setiap level. Hal ini sesuai dengan kompleksitas DFS sebesar O(n), sedangkan Hash Map memiliki kompleksitas rata-rata O(1). Hasil ini memperkuat bahwa Hash Map lebih efisien untuk pencarian berdasarkan ID.
```

### Grafik 2: Perbandingan Waktu Pencarian Berdasarkan Level

Sumber data: `hasil_benchmark.csv`.

Judul grafik:

```text
Grafik 2. Perbandingan Waktu Pencarian DFS pada Tree dan Hash Map Berdasarkan Level
```

Sumbu dan seri data:

```text
Sumbu X: level
Sumbu Y: Waktu Eksekusi (mikrodetik)
Seri 1: waktu_dfs_us
Seri 2: waktu_hash_us
```

Kesimpulan Grafik 2:

```text
Grafik memperlihatkan bahwa waktu pencarian menggunakan DFS pada Tree lebih tinggi dibandingkan Hash Map pada berbagai level kategori. Hash Map tetap lebih cepat karena pencarian dilakukan melalui indeks ID, sedangkan DFS perlu melakukan traversal pada struktur tree.
```

### Grafik 3: Rata-rata Waktu Pencarian per Operasi

Sumber data: `hasil_benchmark.csv`.

Judul grafik:

```text
Grafik 3. Rata-rata Waktu Pencarian per Operasi DFS pada Tree dan Hash Map
```

Sumbu dan seri data:

```text
Sumbu X: level
Sumbu Y: Rata-rata Waktu (nanodetik)
Seri 1: rata_dfs_ns
Seri 2: rata_hash_ns
```

Kesimpulan Grafik 3:

```text
Rata-rata waktu pencarian per operasi menunjukkan perbedaan performa yang lebih adil karena memperhitungkan jumlah pencarian. DFS pada Tree tetap memiliki rata-rata waktu lebih besar dibanding Hash Map, sehingga Hash Map lebih sesuai untuk operasi pencarian ID yang sering dilakukan.
```

### Grafik 4: Perbandingan Penggunaan Memori

Sumber data: `hasil_benchmark.csv` atau menu monitoring memori.

Judul grafik:

```text
Grafik 4. Perbandingan Penggunaan Memori Tree Saja dan Tree + Hash Map
```

Sumbu dan seri data:

```text
Sumbu X: Struktur Data
Sumbu Y: Memori (KB)
Seri 1: estimasi_memori_tree_kb
Seri 2: estimasi_memori_tree_hash_kb
```

Kolom pendukung:

```text
estimasi_memori_hash_index_kb
```

Kolom `estimasi_memori_hash_index_kb` digunakan untuk menjelaskan tambahan memori dari indeks hash map.

Kesimpulan Grafik 4:

```text
Grafik memori menunjukkan bahwa penggunaan struktur hybrid Tree + Hash Map membutuhkan memori lebih besar dibanding Tree saja. Tambahan memori tersebut berasal dari indeks Hash Map. Namun, tambahan memori ini sebanding dengan peningkatan kecepatan pencarian ID yang diperoleh.
```

## Kesimpulan Akhir untuk Bagian Hasil dan Analisis

Kesimpulan yang dapat dimasukkan:

```text
Berdasarkan hasil pengujian, pencarian menggunakan Hash Map lebih cepat dibandingkan pencarian DFS pada Tree. Pada pencarian DFS, waktu eksekusi dipengaruhi oleh posisi dan kedalaman data karena proses pencarian dilakukan dengan menelusuri node satu per satu. Sebaliknya, Hash Map dapat mengakses data secara langsung berdasarkan ID sehingga waktu pencarian relatif lebih stabil.

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
