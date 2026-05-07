# Sistem Manajemen Kategori & Monitoring Performa (Topik 10)

**Proyek Akhir Praktikum Struktur Data - Departemen Ilmu Komputer IPB**

Program ini mengimplementasikan struktur data **Hybrid** antara **Linked Tree** (untuk manajemen hierarki) dan **Hash Map** (untuk optimasi pencarian) guna mengelola ribuan data kategori secara efisien.

---

## 👥 Kelompok 10 - Struktur Data

| Nama | NIM |
|------|-----|
| Azka Julian Putra Wahyudi | M0403241029 |
| Aaliyah Nofarizki | M0403241011 |
| Asty Athetha Loethan | M0403241089 |
| Muhamad Raihan Pratama Putra Setyatmoko | M04032241064 |

---

## 📝 Deskripsi Proyek

Proyek ini bertujuan untuk mensimulasikan sistem manajemen kategori pada skala industri menggunakan dataset dari **UCI Online Retail**. Dengan lebih dari **5.700 data unik**, sistem ini membandingkan efisiensi waktu eksekusi antara penelusuran pohon secara rekursif (O(n)) dengan pencarian langsung menggunakan tabel hash (O(1)).

---

## 🚀 Fitur Utama

### **Hybrid Data Structure**
- Menggabungkan **N-ary Tree** untuk visualisasi hierarki
- **Unordered Hash Map** untuk akses data instan

### **Manajemen CRUD Lengkap**
- **Insert**: Menambahkan kategori dan sub-kategori baru
- **Update**: Memperbarui informasi nama kategori secara dinamis
- **Delete (Recursive)**: Menghapus kategori beserta seluruh turunannya secara aman
- **Search**: Pencarian dengan DFS (Tree) dan Hash Map

### **Advanced Monitoring & Analysis**
- **Benchmarking**: Uji kecepatan pencarian (Tree vs Hash Map) dalam satuan mikrodetik
- **Memory Monitoring**: Estimasi penggunaan RAM untuk masing-masing struktur data
- **Data Persistence**: Otomatis menyimpan dan memuat data dari `dataset_kategori.txt`
- **Sorting**: Pengurutan kategori berdasarkan abjad (A-Z)

---

## 📊 Domain Data

Sesuai spesifikasi teknis, setiap entitas data memiliki atribut:

| Atribut | Tipe | Keterangan |
|---------|------|-----------|
| ID Kategori | `int` | Kunci unik (Primary Key) |
| Nama Kategori | `string` | Label deskripsi kategori |
| ID Induk (Parent) | `int` | Relasi untuk membentuk hierarki |
| Level | `int` | Kedalaman kategori dalam pohon |
| Status | `int` | Indikator Aktif (1) atau Nonaktif (0) |

---

## 💻 Cara Menjalankan

### Prasyarat
Pastikan Anda memiliki compiler C++ (seperti MinGW atau GCC) yang sudah terpasang.

### Kompilasi Program
```bash
g++ main.cpp ManagerKategori.cpp -o ProgramFinal
```

### Jalankan Aplikasi
```bash
./ProgramFinal
```

---

## 📈 Analisis Performa (Preview)

Berdasarkan pengujian pada **5.676 baris data**:

- **Search Hash Map**: Konstan (O(1)), rata-rata **jauh lebih cepat** untuk dataset besar
- **Search Tree (DFS)**: Linear (O(n)), waktu meningkat seiring bertambahnya jumlah data dan kedalaman level

Hasil benchmark menunjukkan Hash Map dapat mencapai **~10-15x lebih cepat** dibanding Tree traversal pada dataset besar.