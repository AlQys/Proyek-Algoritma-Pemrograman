☕ KOBESSAH KOPI
Sistem Informasi Kafe Berbasis C++

* ✨ Login
* 🍽 CRUD Menu
* 👨‍💼 Manajemen Pegawai
* 🛒 Transaksi
* 📦 Manajemen Stok
* 🎁 Promo
* 📊 Laporan
* 💾 File Handling


Made with ❤️ using C++










# ☕ KOBESSAH KOPI

### Sistem Informasi Kafe Berbasis C++

![Language](https://img.shields.io/badge/Language-C%2B%2B-blue)
![Platform](https://img.shields.io/badge/Platform-Console-green)
![Status](https://img.shields.io/badge/Status-Completed-success)

## 📖 Deskripsi

KOBESSAH KOPI merupakan aplikasi manajemen kafe berbasis **C++ Console** yang dikembangkan sebagai proyek akhir mata kuliah Algoritma Pemrograman.

Aplikasi ini dirancang untuk membantu operasional kafe mulai dari pengelolaan akun, menu, transaksi, stok, supplier, promo hingga laporan penjualan.

---

# ✨ Fitur

## 🔐 Authentication

* Login
* Registrasi Customer
* Logout
* Hak akses berdasarkan role (Admin, Kasir, Customer)

## 👨‍💼 Manajemen Pegawai

* Tambah Pegawai
* Lihat Pegawai
* Aktif / Nonaktif Pegawai

## 🍽️ Manajemen Menu

* Tambah Menu
* Edit Menu
* Hapus Menu
* Cari Menu
* Ubah Status Ketersediaan

## 🛒 Transaksi

* Pemesanan Menu
* Keranjang Pesanan
* Pembayaran
* Cetak Struk
* Riwayat Transaksi

## 📦 Manajemen Stok

* Supplier
* Pembelian Barang
* Riwayat Stok
* Rekomendasi Restock
* Nilai Persediaan

## 🎁 Promo

* Tambah Promo
* Edit Promo
* Hapus Promo
* Aktivasi Promo

## 💾 File Handling

Seluruh data disimpan menggunakan file `.txt`, sehingga data tetap tersedia ketika program dijalankan kembali.

---

# 🛠️ Konsep Pemrograman yang Digunakan

* Object Oriented Programming (OOP)
* Class
* Object
* Struct
* Template Class
* Dynamic Array (ManualList)
* CRUD
* File Handling
* Searching
* Input Validation
* Modular Programming

---

# 📂 Struktur Data

Program menggunakan beberapa class utama seperti:

* User
* MenuItem
* Transaksi
* ItemTransaksi
* Supplier
* Promo
* Pesanan
* CatatanStok
* TransaksiPembelian

---

# 💾 File Database

| File          | Fungsi            |
| ------------- | ----------------- |
| user.txt      | Data pengguna     |
| menu.txt      | Data menu         |
| transaksi.txt | Riwayat transaksi |
| pesanan.txt   | Data pesanan      |
| supplier.txt  | Data supplier     |
| stok_log.txt  | Riwayat stok      |
| pembelian.txt | Data pembelian    |
| promo.txt     | Data promo        |

---

# 👥 Role Pengguna

### Admin

* Mengelola pegawai
* Mengelola menu
* Mengelola supplier
* Mengelola stok
* Mengelola promo
* Melihat laporan

### Kasir

* Memproses transaksi
* Melakukan pembayaran
* Mencetak struk

### Customer

* Registrasi akun
* Login
* Melakukan pemesanan

---

# 🚀 Cara Menjalankan Program

1. Clone repository

```
git clone <repository-url>
```

2. Buka project menggunakan Dev C++ atau Visual Studio Code.

3. Compile program.

4. Jalankan file executable.

---

# 👨‍💻 Tim Pengembang

* Ahmad Imam Al Qysthy Patria
* ABD. Rayhan M. Kota
* Zakky Auliya Fadlan

---

# 📌 Catatan

Project ini dibuat sebagai media pembelajaran mata kuliah Algoritma Pemrograman dan masih dapat dikembangkan lebih lanjut dengan penggunaan database seperti MySQL serta GUI menggunakan Qt atau framework lainnya.
