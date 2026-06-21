/*
==============================================================
   SISTEM INFORMASI KAFE - "KOBESSAH KOPI"
   Proyek Tugas Pemrograman C++
==============================================================
   Fitur:
   - Login & Registrasi (Admin, Kasir, Customer)
   - Manajemen Menu (CRUD) oleh Admin
   - Pemesanan berbasis pilih-dari-daftar-menu
   - Cetak Struk / Nota Transaksi
   - Riwayat & Laporan Transaksi
   - Manajemen Pegawai oleh Admin
   - Validasi input di seluruh sistem
==============================================================
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <limits>

using namespace std;

/* ============================================================
   KONSTANTA GLOBAL
   ============================================================ */

const string FILE_USER        = "user.txt";
const string FILE_MENU        = "menu.txt";
const string FILE_TRANSAKSI   = "transaksi.txt";
const string FILE_PESANAN     = "pesanan.txt";
const string NAMA_KAFE        = "KOBESSAH KOPI";
const int    LEBAR_LAYAR      = 50;

/* ============================================================
   STRUCT / CLASS DATA
   ============================================================ */

class User {
public:
    string username;
    string password;
    string role;       // "Admin", "Kasir", "Customer"
    string namaLengkap;
    bool   aktif;

    User() {
        aktif = true;
        namaLengkap = "-";
    }
};

// Tipe menu: "Makanan" atau "Minuman"
class MenuItem {
public:
    int    id;
    string tipe;        // "Makanan" / "Minuman"
    string kategori;    // contoh: "Nasgor", "Mie", "Kopi Robusta", dll
    string nama;
    int    harga;
    int    stok;
    bool   tersedia;

    MenuItem() {
        id = 0;
        harga = 0;
        stok = 0;
        tersedia = true;
    }
};

// Satu baris item di dalam sebuah transaksi
class ItemTransaksi {
public:
    int    idMenu;
    string namaMenu;
    int    harga;
    int    jumlah;
    int    subtotal;

    ItemTransaksi() {
        idMenu = 0;
        harga = 0;
        jumlah = 0;
        subtotal = 0;
    }
};

// Satu transaksi penuh (bisa berisi banyak item)
class Transaksi {
public:
    int    idTransaksi;
    string tanggal;
    string waktu;
    string kasirUsername;
    string customerUsername;
    vector<ItemTransaksi> daftarItem;
    int    totalBayar;
    int    uangBayar;
    int    kembalian;

    Transaksi() {
        idTransaksi = 0;
        totalBayar = 0;
        uangBayar = 0;
        kembalian = 0;
    }
};

// Satu pesanan dari customer yang menunggu diproses pembayarannya oleh Kasir
class Pesanan {
public:
    int    idPesanan;
    string tanggal;
    string waktu;
    string customerUsername;
    vector<ItemTransaksi> daftarItem;
    int    totalHarga;
    string status;   // "Pending" / "Selesai" / "Dibatalkan"

    Pesanan() {
        idPesanan = 0;
        totalHarga = 0;
        status = "Pending";
    }
};

/* ============================================================
   VARIABEL GLOBAL
   ============================================================ */

vector<User>          daftarUser;
vector<MenuItem>       daftarMenu;
vector<Transaksi>      daftarTransaksi;
vector<Pesanan>        daftarPesanan;
vector<ItemTransaksi>  keranjang;       // keranjang sementara saat input pesanan

string roleLogin    = "";
string userLogin    = "";
string namaLogin    = "";

int counterIdMenu       = 1;
int counterIdTransaksi  = 1;
int counterIdPesanan    = 1;

/* ============================================================
   FUNGSI UTILITAS UMUM
   ============================================================ */

void clearScreen() {
    system("cls");
}

void jedaLayar() {
    system("pause");
}

void garis(char c = '=', int panjang = LEBAR_LAYAR) {
    cout << string(panjang, c) << "\n";
}

string getTanggalSekarang() {
    time_t t = time(0);
    tm *now = localtime(&t);

    ostringstream oss;
    oss << setfill('0') << setw(2) << now->tm_mday << "-"
        << setfill('0') << setw(2) << (now->tm_mon + 1) << "-"
        << (now->tm_year + 1900);

    return oss.str();
}

string getWaktuSekarang() {
    time_t t = time(0);
    tm *now = localtime(&t);

    ostringstream oss;
    oss << setfill('0') << setw(2) << now->tm_hour << ":"
        << setfill('0') << setw(2) << now->tm_min   << ":"
        << setfill('0') << setw(2) << now->tm_sec;

    return oss.str();
}

string formatRupiah(int angka) {
    string s = to_string(angka);
    string hasil = "";
    int hitung = 0;

    for (int i = (int)s.size() - 1; i >= 0; i--) {
        hasil = s[i] + hasil;
        hitung++;
        if (hitung % 3 == 0 && i != 0) {
            hasil = "." + hasil;
        }
    }

    return "Rp" + hasil;
}

// Membaca input integer dengan validasi (anti crash kalau user input huruf)
int inputInt(string label) {
    int nilai;

    while (true) {
        cout << label;
        cin >> nilai;

        if (cin.eof()) {
            // Input stream berakhir (mis. Ctrl+D/Ctrl+Z) -> hentikan program dengan aman
            cout << "\nInput berakhir. Program ditutup.\n";
            exit(0);
        }

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input tidak valid! Masukkan angka.\n";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return nilai;
        }
    }
}

// Membaca input integer dengan validasi rentang minimum-maksimum
int inputIntRange(string label, int minVal, int maxVal) {
    int nilai;

    while (true) {
        nilai = inputInt(label);

        if (nilai < minVal || nilai > maxVal) {
            cout << "Input harus di antara " << minVal << " - " << maxVal << "!\n";
        } else {
            return nilai;
        }
    }
}

string inputString(string label) {
    string nilai;
    cout << label;
    getline(cin, nilai);
    return nilai;
}

string toUpperStr(string s) {
    for (size_t i = 0; i < s.size(); i++) {
        s[i] = toupper(s[i]);
    }
    return s;
}

string toLowerStr(string s) {
    for (size_t i = 0; i < s.size(); i++) {
        s[i] = tolower(s[i]);
    }
    return s;
}

/* ============================================================
   INISIALISASI DATA MENU AWAL
   ============================================================ */

void tambahMenuKeDaftar(string tipe, string kategori, string nama, int harga, int stok = 100) {
    MenuItem m;
    m.id        = counterIdMenu++;
    m.tipe      = tipe;
    m.kategori  = kategori;
    m.nama      = nama;
    m.harga     = harga;
    m.stok      = stok;
    m.tersedia  = true;

    daftarMenu.push_back(m);
}

void inisialisasiMenuMakanan() {

    tambahMenuKeDaftar("Makanan", "Main Dish", "Nasi Telur", 11000);
    tambahMenuKeDaftar("Makanan", "Main Dish", "Nasi Telur + Tempe", 13000);
    tambahMenuKeDaftar("Makanan", "Main Dish", "Nasi Telur + Jamur", 13000);
    tambahMenuKeDaftar("Makanan", "Main Dish", "Nasi Telur + Nugget", 14000);
    tambahMenuKeDaftar("Makanan", "Main Dish", "Nasi Ayam", 17000);
    tambahMenuKeDaftar("Makanan", "Main Dish", "Nasi Ayam + Jamur", 18000);
    tambahMenuKeDaftar("Makanan", "Main Dish", "Nasi Ayam + Nugget", 19000);

    tambahMenuKeDaftar("Makanan", "Nasgor", "Nasi Goreng Kobe", 12000);
    tambahMenuKeDaftar("Makanan", "Nasgor", "Nasi Goreng Oriental", 15000);
    tambahMenuKeDaftar("Makanan", "Nasgor", "Nasi Goreng Jawa", 15000);
    tambahMenuKeDaftar("Makanan", "Nasgor", "Nasi Goreng Arab", 15000);
    tambahMenuKeDaftar("Makanan", "Nasgor", "Magelangan", 17000);

    tambahMenuKeDaftar("Makanan", "Mie", "Mie Goreng Telur", 10000);
    tambahMenuKeDaftar("Makanan", "Mie", "Mie Rebus Telur", 10000);
    tambahMenuKeDaftar("Makanan", "Mie", "Mie Goreng Telur + Sosis", 12000);
    tambahMenuKeDaftar("Makanan", "Mie", "Mie Rebus Telur + Sosis", 12000);
    tambahMenuKeDaftar("Makanan", "Mie", "Mie Goreng Katsu", 15000);
    tambahMenuKeDaftar("Makanan", "Mie", "Mie Goreng Spesial", 15000);
    tambahMenuKeDaftar("Makanan", "Mie", "Mie Rebus Spesial", 15000);
}

void inisialisasiMenuMinuman() {

    tambahMenuKeDaftar("Minuman", "Kopi Robusta", "Kopi Hitam", 15000);
    tambahMenuKeDaftar("Minuman", "Kopi Robusta", "Kopi Susu", 18000);

    tambahMenuKeDaftar("Minuman", "Ice Coffee", "Ice Coffee Hitam", 18000);
    tambahMenuKeDaftar("Minuman", "Ice Coffee", "Ice Coffee Kobe", 22000);
    tambahMenuKeDaftar("Minuman", "Ice Coffee", "Ice Coffee Aren", 24000);

    tambahMenuKeDaftar("Minuman", "Non Coffee", "Matcha", 23000);
    tambahMenuKeDaftar("Minuman", "Non Coffee", "Red Velvet", 22000);

    tambahMenuKeDaftar("Minuman", "Cokelat", "Cokelat Original", 20000);
    tambahMenuKeDaftar("Minuman", "Cokelat", "Cokelat Almond", 22000);

    tambahMenuKeDaftar("Minuman", "Blend", "Cappuccino", 25000);
    tambahMenuKeDaftar("Minuman", "Blend", "Oreo", 24000);

    tambahMenuKeDaftar("Minuman", "Milk Series", "Milk Tea", 18000);
    tambahMenuKeDaftar("Minuman", "Milk Series", "Milk Strawberry", 19000);

    tambahMenuKeDaftar("Minuman", "Squash", "Lemon Squash", 17000);
    tambahMenuKeDaftar("Minuman", "Squash", "Lychee Squash", 17000);

    tambahMenuKeDaftar("Minuman", "Juice", "Alpukat", 20000);
    tambahMenuKeDaftar("Minuman", "Juice", "Jambu", 19000);

    tambahMenuKeDaftar("Minuman", "Tea Series", "Original Tea", 18000);
    tambahMenuKeDaftar("Minuman", "Tea Series", "Lemon Tea", 19000);

    tambahMenuKeDaftar("Minuman", "Wedangan", "Teh Jahe", 15000);
    tambahMenuKeDaftar("Minuman", "Wedangan", "Wedang Uwuh", 17000);
}

/* ============================================================
   PENYIMPANAN & PEMUATAN DATA MENU (FILE)
   ============================================================ */

void simpanSemuaMenu() {

    ofstream file(FILE_MENU.c_str());

    for (size_t i = 0; i < daftarMenu.size(); i++) {

        file << daftarMenu[i].id << "|"
             << daftarMenu[i].tipe << "|"
             << daftarMenu[i].kategori << "|"
             << daftarMenu[i].nama << "|"
             << daftarMenu[i].harga << "|"
             << daftarMenu[i].stok << "|"
             << (daftarMenu[i].tersedia ? 1 : 0)
             << "\n";
    }

    file.close();
}

bool loadMenuDariFile() {

    ifstream file(FILE_MENU.c_str());

    if (!file.is_open()) {
        return false;
    }

    string baris;
    bool adaData = false;

    while (getline(file, baris)) {

        if (baris.empty()) continue;

        stringstream ss(baris);
        string token;
        vector<string> kolom;

        while (getline(ss, token, '|')) {
            kolom.push_back(token);
        }

        if (kolom.size() < 7) continue;

        MenuItem m;
        m.id        = stoi(kolom[0]);
        m.tipe      = kolom[1];
        m.kategori  = kolom[2];
        m.nama      = kolom[3];
        m.harga     = stoi(kolom[4]);
        m.stok      = stoi(kolom[5]);
        m.tersedia  = (kolom[6] == "1");

        daftarMenu.push_back(m);

        if (m.id >= counterIdMenu) {
            counterIdMenu = m.id + 1;
        }

        adaData = true;
    }

    file.close();
    return adaData;
}

void siapkanDataMenu() {

    // Jika file menu.txt sudah ada isinya, pakai itu.
    // Jika belum ada (pertama kali jalan), buat data awal lalu simpan ke file.
    bool berhasilLoad = loadMenuDariFile();

    if (!berhasilLoad) {
        inisialisasiMenuMakanan();
        inisialisasiMenuMinuman();
        simpanSemuaMenu();
    }
}

/* ============================================================
   PENCARIAN & UTILITAS MENU
   ============================================================ */

int cariIndexMenuById(int id) {
    for (size_t i = 0; i < daftarMenu.size(); i++) {
        if (daftarMenu[i].id == id) {
            return (int)i;
        }
    }
    return -1;
}

vector<int> getIndexMenuByTipe(string tipe) {
    vector<int> hasil;
    for (size_t i = 0; i < daftarMenu.size(); i++) {
        if (daftarMenu[i].tipe == tipe) {
            hasil.push_back((int)i);
        }
    }
    return hasil;
}

bool adaMenuDenganId(int id) {
    return cariIndexMenuById(id) != -1;
}

/* ============================================================
   PENYIMPANAN & PEMUATAN DATA USER (FILE)
   ============================================================ */

void simpanSatuUser(User u) {

    ofstream file(FILE_USER.c_str(), ios::app);

    file << u.username << "|"
         << u.password << "|"
         << u.role << "|"
         << u.namaLengkap << "|"
         << (u.aktif ? 1 : 0)
         << "\n";

    file.close();
}

void simpanUlangSemuaUser() {

    ofstream file(FILE_USER.c_str());

    for (size_t i = 0; i < daftarUser.size(); i++) {

        file << daftarUser[i].username << "|"
             << daftarUser[i].password << "|"
             << daftarUser[i].role << "|"
             << daftarUser[i].namaLengkap << "|"
             << (daftarUser[i].aktif ? 1 : 0)
             << "\n";
    }

    file.close();
}

void loadUserDariFile() {

    ifstream file(FILE_USER.c_str());

    if (!file.is_open()) {
        return;
    }

    string baris;

    while (getline(file, baris)) {

        if (baris.empty()) continue;

        stringstream ss(baris);
        string token;
        vector<string> kolom;

        while (getline(ss, token, '|')) {
            kolom.push_back(token);
        }

        if (kolom.size() < 5) continue;

        User u;
        u.username    = kolom[0];
        u.password    = kolom[1];
        u.role        = kolom[2];
        u.namaLengkap = kolom[3];
        u.aktif       = (kolom[4] == "1");

        daftarUser.push_back(u);
    }

    file.close();
}

/* ============================================================
   PENCARIAN & VALIDASI USER
   ============================================================ */

int cariIndexUserByUsername(string username) {
    for (size_t i = 0; i < daftarUser.size(); i++) {
        if (daftarUser[i].username == username) {
            return (int)i;
        }
    }
    return -1;
}

bool usernameSudahDipakai(string username) {
    return cariIndexUserByUsername(username) != -1;
}

bool cekLogin(string username, string password) {

    int idx = cariIndexUserByUsername(username);

    if (idx == -1) {
        return false;
    }

    if (daftarUser[idx].password != password) {
        return false;
    }

    if (!daftarUser[idx].aktif) {
        cout << "\nAkun ini sudah dinonaktifkan. Hubungi admin.\n";
        return false;
    }

    roleLogin = daftarUser[idx].role;
    userLogin = daftarUser[idx].username;
    namaLogin = daftarUser[idx].namaLengkap;

    return true;
}

/* ============================================================
   FITUR: REGISTRASI, LOGIN, LOGOUT
   ============================================================ */

void registrasi() {

    clearScreen();

    garis();
    cout << "          REGISTRASI AKUN CUSTOMER\n";
    garis();

    User user;

    string username = inputString("Masukkan Username : ");

    if (username.empty()) {
        cout << "\nUsername tidak boleh kosong!\n";
        jedaLayar();
        return;
    }

    if (usernameSudahDipakai(username)) {
        cout << "\nUsername sudah digunakan, silakan pilih yang lain!\n";
        jedaLayar();
        return;
    }

    string password = inputString("Masukkan Password : ");

    if (password.empty()) {
        cout << "\nPassword tidak boleh kosong!\n";
        jedaLayar();
        return;
    }

    string nama = inputString("Masukkan Nama Lengkap : ");

    user.username    = username;
    user.password    = password;
    user.role        = "Customer";
    user.namaLengkap = nama.empty() ? "-" : nama;
    user.aktif       = true;

    daftarUser.push_back(user);
    simpanSatuUser(user);

    cout << "\nRegistrasi berhasil! Silakan login.\n";
    jedaLayar();
}

bool login() {

    clearScreen();

    garis();
    cout << "             LOGIN SISTEM\n";
    garis();

    string username = inputString("Username : ");
    string password = inputString("Password : ");

    bool status = cekLogin(username, password);

    if (status) {

        cout << "\nLogin berhasil! Selamat datang, "
             << (namaLogin == "-" ? username : namaLogin) << "!\n";

        jedaLayar();
        return true;
    }
    else {

        cout << "\nUsername atau password salah!\n";
        jedaLayar();
        return false;
    }
}

void logout() {

    clearScreen();

    garis();
    cout << "                LOGOUT\n";
    garis();
    cout << "Anda berhasil logout dari sistem.\n";

    roleLogin = "";
    userLogin = "";
    namaLogin = "";

    jedaLayar();
}

/* ============================================================
   FITUR: MANAJEMEN PEGAWAI (ADMIN)
   ============================================================ */

void tambahPegawai() {

    clearScreen();

    garis();
    cout << "            TAMBAH PEGAWAI\n";
    garis();

    User user;

    string username = inputString("Username : ");

    if (usernameSudahDipakai(username)) {
        cout << "\nUsername sudah digunakan!\n";
        jedaLayar();
        return;
    }

    string password = inputString("Password : ");
    string nama     = inputString("Nama Lengkap : ");

    cout << "\n1. Admin\n";
    cout << "2. Kasir\n";

    int pilihan = inputIntRange("Pilih Role : ", 1, 2);

    user.username    = username;
    user.password    = password;
    user.namaLengkap = nama.empty() ? "-" : nama;
    user.role        = (pilihan == 1) ? "Admin" : "Kasir";
    user.aktif       = true;

    daftarUser.push_back(user);
    simpanSatuUser(user);

    cout << "\nPegawai berhasil ditambahkan!\n";
    jedaLayar();
}

void lihatDaftarPegawai() {

    clearScreen();

    garis();
    cout << "            DAFTAR PEGAWAI\n";
    garis();

    cout << left << setw(15) << "Username"
         << setw(20) << "Nama"
         << setw(10) << "Role"
         << setw(10) << "Status" << "\n";

    garis('-');

    int jumlahPegawai = 0;

    for (size_t i = 0; i < daftarUser.size(); i++) {

        if (daftarUser[i].role == "Admin" || daftarUser[i].role == "Kasir") {

            cout << left << setw(15) << daftarUser[i].username
                 << setw(20) << daftarUser[i].namaLengkap
                 << setw(10) << daftarUser[i].role
                 << setw(10) << (daftarUser[i].aktif ? "Aktif" : "Nonaktif")
                 << "\n";

            jumlahPegawai++;
        }
    }

    if (jumlahPegawai == 0) {
        cout << "Belum ada data pegawai.\n";
    }

    garis();
    cout << "\n";
    jedaLayar();
}

void nonaktifkanPegawai() {

    clearScreen();

    garis();
    cout << "          NONAKTIFKAN PEGAWAI\n";
    garis();

    lihatDaftarPegawai();

    clearScreen();

    string username = inputString("Masukkan username pegawai yang ingin dinonaktifkan : ");

    int idx = cariIndexUserByUsername(username);

    if (idx == -1) {
        cout << "\nUsername tidak ditemukan!\n";
        jedaLayar();
        return;
    }

    if (daftarUser[idx].role == "Customer") {
        cout << "\nUsername tersebut adalah Customer, bukan pegawai!\n";
        jedaLayar();
        return;
    }

    if (daftarUser[idx].username == userLogin) {
        cout << "\nAnda tidak bisa menonaktifkan akun Anda sendiri!\n";
        jedaLayar();
        return;
    }

    daftarUser[idx].aktif = !daftarUser[idx].aktif;
    simpanUlangSemuaUser();

    cout << "\nStatus pegawai " << username << " sekarang: "
         << (daftarUser[idx].aktif ? "Aktif" : "Nonaktif") << "\n";

    jedaLayar();
}

/* ============================================================
   FITUR: TAMPILKAN MENU (MAKANAN / MINUMAN TERPISAH)
   ============================================================ */

void cetakHeaderMenu(string judul) {
    garis();
    cout << "          " << NAMA_KAFE << "\n";
    garis('-');
    cout << judul << "\n";
    garis();
}

void tampilMenuMakanan() {

    clearScreen();

    cetakHeaderMenu("DAFTAR MENU MAKANAN");

    string kategoriSebelumnya = "";
    bool adaData = false;

    for (size_t i = 0; i < daftarMenu.size(); i++) {

        if (daftarMenu[i].tipe != "Makanan") continue;

        adaData = true;

        if (daftarMenu[i].kategori != kategoriSebelumnya) {
            kategoriSebelumnya = daftarMenu[i].kategori;
            cout << "\n[" << kategoriSebelumnya << "]\n";
        }

        cout << "  " << setw(4) << left << daftarMenu[i].id
             << setw(30) << left << daftarMenu[i].nama
             << setw(12) << left << formatRupiah(daftarMenu[i].harga)
             << (daftarMenu[i].tersedia ? "" : "(Habis)")
             << "\n";
    }

    if (!adaData) {
        cout << "\nBelum ada menu makanan.\n";
    }

    cout << "\n";
    jedaLayar();
}

void tampilMenuMinuman() {

    clearScreen();

    cetakHeaderMenu("DAFTAR MENU MINUMAN");

    string kategoriSebelumnya = "";
    bool adaData = false;

    for (size_t i = 0; i < daftarMenu.size(); i++) {

        if (daftarMenu[i].tipe != "Minuman") continue;

        adaData = true;

        if (daftarMenu[i].kategori != kategoriSebelumnya) {
            kategoriSebelumnya = daftarMenu[i].kategori;
            cout << "\n[" << kategoriSebelumnya << "]\n";
        }

        cout << "  " << setw(4) << left << daftarMenu[i].id
             << setw(30) << left << daftarMenu[i].nama
             << setw(12) << left << formatRupiah(daftarMenu[i].harga)
             << (daftarMenu[i].tersedia ? "" : "(Habis)")
             << "\n";
    }

    if (!adaData) {
        cout << "\nBelum ada menu minuman.\n";
    }

    cout << "\n";
    jedaLayar();
}

void tampilSemuaMenuRingkas() {

    // Versi ringkas dipakai saat proses pemesanan, supaya user bisa
    // langsung lihat ID lalu input ID itu untuk memesan.

    string kategoriSebelumnya = "";

    cout << "\n-- MAKANAN --\n";

    for (size_t i = 0; i < daftarMenu.size(); i++) {

        if (daftarMenu[i].tipe != "Makanan") continue;
        if (!daftarMenu[i].tersedia) continue;

        if (daftarMenu[i].kategori != kategoriSebelumnya) {
            kategoriSebelumnya = daftarMenu[i].kategori;
            cout << "\n[" << kategoriSebelumnya << "]\n";
        }

        cout << "  " << setw(4) << left << daftarMenu[i].id
             << setw(30) << left << daftarMenu[i].nama
             << formatRupiah(daftarMenu[i].harga) << "\n";
    }

    kategoriSebelumnya = "";
    cout << "\n-- MINUMAN --\n";

    for (size_t i = 0; i < daftarMenu.size(); i++) {

        if (daftarMenu[i].tipe != "Minuman") continue;
        if (!daftarMenu[i].tersedia) continue;

        if (daftarMenu[i].kategori != kategoriSebelumnya) {
            kategoriSebelumnya = daftarMenu[i].kategori;
            cout << "\n[" << kategoriSebelumnya << "]\n";
        }

        cout << "  " << setw(4) << left << daftarMenu[i].id
             << setw(30) << left << daftarMenu[i].nama
             << formatRupiah(daftarMenu[i].harga) << "\n";
    }

    cout << "\n";
}

/* ============================================================
   FITUR: KELOLA MENU (ADMIN) - CRUD
   ============================================================ */

void tambahMenuBaru() {

    clearScreen();

    garis();
    cout << "            TAMBAH MENU BARU\n";
    garis();

    int tipePilihan = inputIntRange("1. Makanan\n2. Minuman\nPilih Tipe : ", 1, 2);

    string tipe = (tipePilihan == 1) ? "Makanan" : "Minuman";
    string kategori = inputString("Kategori (contoh: Nasgor, Mie, Kopi, dll) : ");
    string nama     = inputString("Nama Menu : ");

    if (nama.empty()) {
        cout << "\nNama menu tidak boleh kosong!\n";
        jedaLayar();
        return;
    }

    int harga = inputInt("Harga : ");

    if (harga <= 0) {
        cout << "\nHarga harus lebih dari 0!\n";
        jedaLayar();
        return;
    }

    int stok = inputInt("Stok Awal : ");

    if (stok < 0) stok = 0;

    tambahMenuKeDaftar(tipe, kategori, nama, harga, stok);
    simpanSemuaMenu();

    cout << "\nMenu '" << nama << "' berhasil ditambahkan!\n";
    jedaLayar();
}

void editMenu() {

    clearScreen();

    garis();
    cout << "              EDIT MENU\n";
    garis();

    tampilSemuaMenuRingkas();

    int id = inputInt("\nMasukkan ID menu yang ingin diedit (0 untuk batal) : ");

    if (id == 0) return;

    int idx = cariIndexMenuById(id);

    if (idx == -1) {
        cout << "\nID menu tidak ditemukan!\n";
        jedaLayar();
        return;
    }

    cout << "\nMenu saat ini: " << daftarMenu[idx].nama
         << " - " << formatRupiah(daftarMenu[idx].harga)
         << " (Stok: " << daftarMenu[idx].stok << ")\n\n";

    string namaBaru = inputString("Nama Baru (kosongkan jika tidak diubah) : ");

    if (!namaBaru.empty()) {
        daftarMenu[idx].nama = namaBaru;
    }

    int hargaBaru = inputInt("Harga Baru (0 jika tidak diubah) : ");

    if (hargaBaru > 0) {
        daftarMenu[idx].harga = hargaBaru;
    }

    int stokBaru = inputInt("Stok Baru (-1 jika tidak diubah) : ");

    if (stokBaru >= 0) {
        daftarMenu[idx].stok = stokBaru;
    }

    simpanSemuaMenu();

    cout << "\nMenu berhasil diperbarui!\n";
    jedaLayar();
}

void hapusMenu() {

    clearScreen();

    garis();
    cout << "             HAPUS MENU\n";
    garis();

    tampilSemuaMenuRingkas();

    int id = inputInt("\nMasukkan ID menu yang ingin dihapus (0 untuk batal) : ");

    if (id == 0) return;

    int idx = cariIndexMenuById(id);

    if (idx == -1) {
        cout << "\nID menu tidak ditemukan!\n";
        jedaLayar();
        return;
    }

    string namaMenu = daftarMenu[idx].nama;

    daftarMenu.erase(daftarMenu.begin() + idx);
    simpanSemuaMenu();

    cout << "\nMenu '" << namaMenu << "' berhasil dihapus!\n";
    jedaLayar();
}

void ubahStatusKetersediaanMenu() {

    clearScreen();

    garis();
    cout << "       UBAH STATUS KETERSEDIAAN MENU\n";
    garis();

    tampilSemuaMenuRingkas();

    int id = inputInt("\nMasukkan ID menu (0 untuk batal) : ");

    if (id == 0) return;

    int idx = cariIndexMenuById(id);

    if (idx == -1) {
        cout << "\nID menu tidak ditemukan!\n";
        jedaLayar();
        return;
    }

    daftarMenu[idx].tersedia = !daftarMenu[idx].tersedia;
    simpanSemuaMenu();

    cout << "\nStatus menu '" << daftarMenu[idx].nama << "' sekarang: "
         << (daftarMenu[idx].tersedia ? "Tersedia" : "Habis/Nonaktif") << "\n";

    jedaLayar();
}

void cariMenu() {

    clearScreen();

    garis();
    cout << "              CARI MENU\n";
    garis();

    string kataKunci = toLowerStr(inputString("Masukkan kata kunci nama menu : "));

    bool ketemu = false;

    cout << "\n";

    for (size_t i = 0; i < daftarMenu.size(); i++) {

        string namaLower = toLowerStr(daftarMenu[i].nama);

        if (namaLower.find(kataKunci) != string::npos) {

            cout << "  " << setw(4) << left << daftarMenu[i].id
                 << setw(30) << left << daftarMenu[i].nama
                 << setw(12) << left << formatRupiah(daftarMenu[i].harga)
                 << "[" << daftarMenu[i].tipe << "]"
                 << "\n";

            ketemu = true;
        }
    }

    if (!ketemu) {
        cout << "Tidak ada menu yang cocok dengan kata kunci '" << kataKunci << "'.\n";
    }

    cout << "\n";
    jedaLayar();
}

void menuKelolaMenu() {

    int pilihan;

    do {
        clearScreen();

        garis();
        cout << "            KELOLA MENU\n";
        garis();
        cout << "1. Lihat Semua Menu\n";
        cout << "2. Tambah Menu Baru\n";
        cout << "3. Edit Menu\n";
        cout << "4. Hapus Menu\n";
        cout << "5. Ubah Status Ketersediaan\n";
        cout << "6. Cari Menu\n";
        cout << "0. Kembali\n";
        garis();

        pilihan = inputIntRange("Pilih Menu : ", 0, 6);

        switch (pilihan) {
            case 1:
                tampilMenuMakanan();
                tampilMenuMinuman();
                break;
            case 2:
                tambahMenuBaru();
                break;
            case 3:
                editMenu();
                break;
            case 4:
                hapusMenu();
                break;
            case 5:
                ubahStatusKetersediaanMenu();
                break;
            case 6:
                cariMenu();
                break;
        }

    } while (pilihan != 0);
}

/* ============================================================
   FITUR: PEMESANAN (PILIH DARI DAFTAR MENU)
   ============================================================ */

void tambahItemKeKeranjang(int idMenu, int jumlah) {

    int idxMenu = cariIndexMenuById(idMenu);

    if (idxMenu == -1) return;

    // Jika menu sudah ada di keranjang, tambahkan jumlahnya saja
    for (size_t i = 0; i < keranjang.size(); i++) {
        if (keranjang[i].idMenu == idMenu) {
            keranjang[i].jumlah   += jumlah;
            keranjang[i].subtotal  = keranjang[i].harga * keranjang[i].jumlah;
            return;
        }
    }

    ItemTransaksi it;
    it.idMenu   = daftarMenu[idxMenu].id;
    it.namaMenu = daftarMenu[idxMenu].nama;
    it.harga    = daftarMenu[idxMenu].harga;
    it.jumlah   = jumlah;
    it.subtotal = it.harga * jumlah;

    keranjang.push_back(it);
}

int hitungTotalKeranjang() {

    int total = 0;

    for (size_t i = 0; i < keranjang.size(); i++) {
        total += keranjang[i].subtotal;
    }

    return total;
}

void tampilIsiKeranjang() {

    if (keranjang.empty()) {
        cout << "\nKeranjang masih kosong.\n";
        return;
    }

    cout << "\n";
    garis('-');
    cout << left << setw(25) << "Menu"
         << setw(8)  << "Jml"
         << setw(12) << "Harga"
         << setw(12) << "Subtotal" << "\n";
    garis('-');

    for (size_t i = 0; i < keranjang.size(); i++) {

        cout << left << setw(25) << keranjang[i].namaMenu
             << setw(8)  << keranjang[i].jumlah
             << setw(12) << formatRupiah(keranjang[i].harga)
             << setw(12) << formatRupiah(keranjang[i].subtotal)
             << "\n";
    }

    garis('-');
    cout << "TOTAL SEMENTARA : " << formatRupiah(hitungTotalKeranjang()) << "\n";
}

void simpanSatuPesananKeFile(Pesanan p) {

    ofstream file(FILE_PESANAN.c_str(), ios::app);

    // Format: ID|TANGGAL|WAKTU|CUSTOMER|TOTAL|STATUS|JUMLAH_ITEM|nama:harga:jumlah;...
    file << p.idPesanan << "|"
         << p.tanggal << "|"
         << p.waktu << "|"
         << p.customerUsername << "|"
         << p.totalHarga << "|"
         << p.status << "|"
         << p.daftarItem.size() << "|";

    for (size_t i = 0; i < p.daftarItem.size(); i++) {
        file << p.daftarItem[i].namaMenu << ":"
             << p.daftarItem[i].harga << ":"
             << p.daftarItem[i].jumlah;

        if (i != p.daftarItem.size() - 1) {
            file << ";";
        }
    }

    file << "\n";
    file.close();
}

void simpanUlangSemuaPesanan() {

    ofstream file(FILE_PESANAN.c_str());

    for (size_t i = 0; i < daftarPesanan.size(); i++) {

        Pesanan p = daftarPesanan[i];

        file << p.idPesanan << "|"
             << p.tanggal << "|"
             << p.waktu << "|"
             << p.customerUsername << "|"
             << p.totalHarga << "|"
             << p.status << "|"
             << p.daftarItem.size() << "|";

        for (size_t j = 0; j < p.daftarItem.size(); j++) {
            file << p.daftarItem[j].namaMenu << ":"
                 << p.daftarItem[j].harga << ":"
                 << p.daftarItem[j].jumlah;

            if (j != p.daftarItem.size() - 1) {
                file << ";";
            }
        }

        file << "\n";
    }

    file.close();
}

void loadPesananDariFile() {

    ifstream file(FILE_PESANAN.c_str());

    if (!file.is_open()) return;

    string baris;

    while (getline(file, baris)) {

        if (baris.empty()) continue;

        stringstream ss(baris);
        string token;
        vector<string> kolom;

        while (getline(ss, token, '|')) {
            kolom.push_back(token);
        }

        if (kolom.size() < 7) continue;

        Pesanan p;
        p.idPesanan         = stoi(kolom[0]);
        p.tanggal           = kolom[1];
        p.waktu             = kolom[2];
        p.customerUsername  = kolom[3];
        p.totalHarga        = stoi(kolom[4]);
        p.status            = kolom[5];

        int jumlahItem = stoi(kolom[6]);

        if (kolom.size() > 7 && jumlahItem > 0) {

            stringstream itemStream(kolom[7]);
            string itemToken;

            while (getline(itemStream, itemToken, ';')) {

                stringstream itemSS(itemToken);
                string namaI, hargaI, jumlahI;

                getline(itemSS, namaI, ':');
                getline(itemSS, hargaI, ':');
                getline(itemSS, jumlahI, ':');

                if (namaI.empty()) continue;

                ItemTransaksi it;
                it.namaMenu = namaI;
                it.harga    = stoi(hargaI);
                it.jumlah   = stoi(jumlahI);
                it.subtotal = it.harga * it.jumlah;

                p.daftarItem.push_back(it);
            }
        }

        daftarPesanan.push_back(p);

        if (p.idPesanan >= counterIdPesanan) {
            counterIdPesanan = p.idPesanan + 1;
        }
    }

    file.close();
}

/* ============================================================
   FITUR: CUSTOMER MEMBUAT PESANAN (STATUS PENDING)
   ============================================================ */

void buatPesananBaru() {

    keranjang.clear();

    int pilihanLanjut;

    do {
        clearScreen();

        garis();
        cout << "             PESAN MENU\n";
        garis();

        tampilSemuaMenuRingkas();
        tampilIsiKeranjang();

        cout << "\n";

        int idMenu = inputInt("\nMasukkan ID Menu yang ingin dipesan (0 untuk selesai) : ");

        if (idMenu == 0) {
            break;
        }

        int idxMenu = cariIndexMenuById(idMenu);

        if (idxMenu == -1) {
            cout << "\nID Menu tidak ditemukan!\n";
            jedaLayar();
            continue;
        }

        if (!daftarMenu[idxMenu].tersedia) {
            cout << "\nMaaf, menu '" << daftarMenu[idxMenu].nama << "' sedang tidak tersedia!\n";
            jedaLayar();
            continue;
        }

        if (daftarMenu[idxMenu].stok <= 0) {
            cout << "\nMaaf, stok '" << daftarMenu[idxMenu].nama << "' habis!\n";
            jedaLayar();
            continue;
        }

        int jumlah = inputInt("Jumlah pesan untuk '" + daftarMenu[idxMenu].nama + "' : ");

        if (jumlah <= 0) {
            cout << "\nJumlah harus lebih dari 0!\n";
            jedaLayar();
            continue;
        }

        if (jumlah > daftarMenu[idxMenu].stok) {
            cout << "\nStok tidak mencukupi! Sisa stok: " << daftarMenu[idxMenu].stok << "\n";
            jedaLayar();
            continue;
        }

        tambahItemKeKeranjang(idMenu, jumlah);

        cout << "\n'" << daftarMenu[idxMenu].nama << "' x" << jumlah << " ditambahkan ke keranjang!\n";

        pilihanLanjut = inputIntRange("\nTambah pesanan lain? (1=Ya, 0=Selesai) : ", 0, 1);

        if (pilihanLanjut == 0) break;

    } while (true);

    if (keranjang.empty()) {
        cout << "\nTidak ada pesanan yang dibuat.\n";
        jedaLayar();
        return;
    }

    // Konfirmasi sebelum disimpan jadi pesanan Pending
    clearScreen();

    garis();
    cout << "          KONFIRMASI PESANAN\n";
    garis();

    tampilIsiKeranjang();

    int konfirmasi = inputIntRange("\nKirim pesanan ini ke kasir? (1=Ya, 0=Batal) : ", 0, 1);

    if (konfirmasi == 0) {
        keranjang.clear();
        cout << "\nPesanan dibatalkan.\n";
        jedaLayar();
        return;
    }

    // Kurangi stok begitu pesanan dikirim ke kasir (stok di-reserve)
    for (size_t i = 0; i < keranjang.size(); i++) {
        int idxMenu = cariIndexMenuById(keranjang[i].idMenu);
        if (idxMenu != -1) {
            daftarMenu[idxMenu].stok -= keranjang[i].jumlah;
        }
    }
    simpanSemuaMenu();

    Pesanan p;
    p.idPesanan        = counterIdPesanan++;
    p.tanggal          = getTanggalSekarang();
    p.waktu            = getWaktuSekarang();
    p.customerUsername = userLogin;
    p.daftarItem       = keranjang;
    p.totalHarga       = hitungTotalKeranjang();
    p.status           = "Pending";

    daftarPesanan.push_back(p);
    simpanSatuPesananKeFile(p);

    cout << "\nPesanan #" << p.idPesanan << " berhasil dikirim ke kasir!\n";
    cout << "Silakan tunggu dipanggil dan lakukan pembayaran di kasir.\n";

    keranjang.clear();
    jedaLayar();
}

/* ============================================================
   FITUR: PEMBAYARAN & CETAK STRUK
   ============================================================ */

void cetakStruk(Transaksi trx) {

    clearScreen();

    garis('*');
    cout << "          " << NAMA_KAFE << "\n";
    cout << "         Jl. Contoh No. 123\n";
    garis('*');

    cout << "No. Transaksi : " << trx.idTransaksi << "\n";
    cout << "Tanggal       : " << trx.tanggal << "\n";
    cout << "Waktu         : " << trx.waktu << "\n";
    cout << "Kasir         : " << trx.kasirUsername << "\n";
    cout << "Pelanggan     : " << trx.customerUsername << "\n";

    garis('-');

    cout << left << setw(20) << "Item"
         << setw(6)  << "Qty"
         << setw(12) << "Harga"
         << setw(12) << "Subtotal" << "\n";

    garis('-');

    for (size_t i = 0; i < trx.daftarItem.size(); i++) {

        cout << left << setw(20) << trx.daftarItem[i].namaMenu
             << setw(6)  << trx.daftarItem[i].jumlah
             << setw(12) << formatRupiah(trx.daftarItem[i].harga)
             << setw(12) << formatRupiah(trx.daftarItem[i].subtotal)
             << "\n";
    }

    garis('-');

    cout << right << setw(38) << "TOTAL    : " << formatRupiah(trx.totalBayar) << "\n";
    cout << right << setw(38) << "BAYAR    : " << formatRupiah(trx.uangBayar)  << "\n";
    cout << right << setw(38) << "KEMBALI  : " << formatRupiah(trx.kembalian)  << "\n";

    garis('*');
    cout << "       Terima kasih telah berkunjung!\n";
    garis('*');

    cout << "\n";
    jedaLayar();
}

void simpanTransaksiKeFile(Transaksi trx) {

    ofstream file(FILE_TRANSAKSI.c_str(), ios::app);

    // Format: ID|TANGGAL|WAKTU|KASIR|CUSTOMER|TOTAL|BAYAR|KEMBALIAN|JUMLAH_ITEM|item1_nama:harga:jumlah;item2...
    file << trx.idTransaksi << "|"
         << trx.tanggal << "|"
         << trx.waktu << "|"
         << trx.kasirUsername << "|"
         << trx.customerUsername << "|"
         << trx.totalBayar << "|"
         << trx.uangBayar << "|"
         << trx.kembalian << "|"
         << trx.daftarItem.size() << "|";

    for (size_t i = 0; i < trx.daftarItem.size(); i++) {
        file << trx.daftarItem[i].namaMenu << ":"
             << trx.daftarItem[i].harga << ":"
             << trx.daftarItem[i].jumlah;

        if (i != trx.daftarItem.size() - 1) {
            file << ";";
        }
    }

    file << "\n";
    file.close();
}

void tampilAntrianPesananPending() {

    cout << "\n";
    garis('-');
    cout << left << setw(6) << "ID"
         << setw(14) << "Customer"
         << setw(12) << "Waktu"
         << setw(12) << "Total" << "\n";
    garis('-');

    bool adaPending = false;

    for (size_t i = 0; i < daftarPesanan.size(); i++) {

        if (daftarPesanan[i].status != "Pending") continue;

        adaPending = true;

        cout << left << setw(6) << daftarPesanan[i].idPesanan
             << setw(14) << daftarPesanan[i].customerUsername
             << setw(12) << daftarPesanan[i].waktu
             << setw(12) << formatRupiah(daftarPesanan[i].totalHarga)
             << "\n";
    }

    if (!adaPending) {
        cout << "Tidak ada pesanan yang menunggu pembayaran.\n";
    }

    garis('-');
}

int cariIndexPesananById(int id) {
    for (size_t i = 0; i < daftarPesanan.size(); i++) {
        if (daftarPesanan[i].idPesanan == id) {
            return (int)i;
        }
    }
    return -1;
}

void prosesPembayaranKasir() {

    clearScreen();

    garis();
    cout << "         ANTRIAN PEMBAYARAN (KASIR)\n";
    garis();

    tampilAntrianPesananPending();

    int idPesanan = inputInt("\nMasukkan ID Pesanan yang ingin dibayar (0 untuk batal) : ");

    if (idPesanan == 0) {
        return;
    }

    int idx = cariIndexPesananById(idPesanan);

    if (idx == -1 || daftarPesanan[idx].status != "Pending") {
        cout << "\nPesanan tidak ditemukan atau sudah diproses!\n";
        jedaLayar();
        return;
    }

    Pesanan p = daftarPesanan[idx];

    clearScreen();

    garis();
    cout << "             PEMBAYARAN\n";
    garis();

    cout << "Pesanan #" << p.idPesanan << " | Customer: " << p.customerUsername << "\n\n";

    cout << left << setw(25) << "Menu"
         << setw(8)  << "Jml"
         << setw(12) << "Harga"
         << setw(12) << "Subtotal" << "\n";
    garis('-');

    for (size_t i = 0; i < p.daftarItem.size(); i++) {
        cout << left << setw(25) << p.daftarItem[i].namaMenu
             << setw(8)  << p.daftarItem[i].jumlah
             << setw(12) << formatRupiah(p.daftarItem[i].harga)
             << setw(12) << formatRupiah(p.daftarItem[i].subtotal)
             << "\n";
    }

    garis('-');
    cout << "TOTAL TAGIHAN : " << formatRupiah(p.totalHarga) << "\n";

    int uangBayar;

    while (true) {
        uangBayar = inputInt("\nMasukkan jumlah uang dibayar customer : ");

        if (uangBayar < p.totalHarga) {
            cout << "Uang tidak cukup! Total tagihan: " << formatRupiah(p.totalHarga) << "\n";
        } else {
            break;
        }
    }

    Transaksi trx;
    trx.idTransaksi      = counterIdTransaksi++;
    trx.tanggal          = getTanggalSekarang();
    trx.waktu            = getWaktuSekarang();
    trx.kasirUsername    = userLogin;
    trx.customerUsername = p.customerUsername;
    trx.daftarItem       = p.daftarItem;
    trx.totalBayar       = p.totalHarga;
    trx.uangBayar        = uangBayar;
    trx.kembalian        = uangBayar - p.totalHarga;

    daftarTransaksi.push_back(trx);
    simpanTransaksiKeFile(trx);

    // Tandai pesanan sebagai Selesai
    daftarPesanan[idx].status = "Selesai";
    simpanUlangSemuaPesanan();

    cetakStruk(trx);
}

/* ============================================================
   PEMUATAN DATA TRANSAKSI DARI FILE (untuk laporan/riwayat)
   ============================================================ */

void loadTransaksiDariFile() {

    ifstream file(FILE_TRANSAKSI.c_str());

    if (!file.is_open()) return;

    string baris;

    while (getline(file, baris)) {

        if (baris.empty()) continue;

        stringstream ss(baris);
        string token;
        vector<string> kolom;

        while (getline(ss, token, '|')) {
            kolom.push_back(token);
        }

        if (kolom.size() < 9) continue;

        Transaksi trx;
        trx.idTransaksi      = stoi(kolom[0]);
        trx.tanggal          = kolom[1];
        trx.waktu            = kolom[2];
        trx.kasirUsername    = kolom[3];
        trx.customerUsername = kolom[4];
        trx.totalBayar       = stoi(kolom[5]);
        trx.uangBayar        = stoi(kolom[6]);
        trx.kembalian        = stoi(kolom[7]);

        int jumlahItem = stoi(kolom[8]);

        if (kolom.size() > 9 && jumlahItem > 0) {

            stringstream itemStream(kolom[9]);
            string itemToken;

            while (getline(itemStream, itemToken, ';')) {

                stringstream itemSS(itemToken);
                string namaI, hargaI, jumlahI;

                getline(itemSS, namaI, ':');
                getline(itemSS, hargaI, ':');
                getline(itemSS, jumlahI, ':');

                if (namaI.empty()) continue;

                ItemTransaksi it;
                it.namaMenu = namaI;
                it.harga    = stoi(hargaI);
                it.jumlah   = stoi(jumlahI);
                it.subtotal = it.harga * it.jumlah;

                trx.daftarItem.push_back(it);
            }
        }

        daftarTransaksi.push_back(trx);

        if (trx.idTransaksi >= counterIdTransaksi) {
            counterIdTransaksi = trx.idTransaksi + 1;
        }
    }

    file.close();
}

/* ============================================================
   FITUR: RIWAYAT TRANSAKSI
   ============================================================ */

void lihatSemuaTransaksi() {

    clearScreen();

    garis();
    cout << "          RIWAYAT SEMUA TRANSAKSI\n";
    garis();

    if (daftarTransaksi.empty()) {
        cout << "Belum ada transaksi.\n\n";
        jedaLayar();
        return;
    }

    int grandTotalSemua = 0;

    for (size_t i = 0; i < daftarTransaksi.size(); i++) {

        cout << "\nTransaksi #" << daftarTransaksi[i].idTransaksi
             << " | " << daftarTransaksi[i].tanggal
             << " " << daftarTransaksi[i].waktu << "\n";

        cout << "Kasir: " << daftarTransaksi[i].kasirUsername
             << " | Pelanggan: " << daftarTransaksi[i].customerUsername << "\n";

        for (size_t j = 0; j < daftarTransaksi[i].daftarItem.size(); j++) {
            cout << "  - " << daftarTransaksi[i].daftarItem[j].namaMenu
                 << " x" << daftarTransaksi[i].daftarItem[j].jumlah
                 << " = " << formatRupiah(daftarTransaksi[i].daftarItem[j].subtotal)
                 << "\n";
        }

        cout << "  Total: " << formatRupiah(daftarTransaksi[i].totalBayar) << "\n";
        garis('-');

        grandTotalSemua += daftarTransaksi[i].totalBayar;
    }

    cout << "\nGRAND TOTAL SELURUH TRANSAKSI : " << formatRupiah(grandTotalSemua) << "\n\n";

    jedaLayar();
}

void lihatPesananSayaSendiri() {

    // Untuk customer: lihat pesanan Pending (belum dibayar) dan riwayat yang sudah Selesai

    clearScreen();

    garis();
    cout << "           RIWAYAT PESANAN SAYA\n";
    garis();

    cout << "\n-- MENUNGGU PEMBAYARAN DI KASIR --\n";

    bool adaPending = false;

    for (size_t i = 0; i < daftarPesanan.size(); i++) {

        if (daftarPesanan[i].customerUsername != userLogin) continue;
        if (daftarPesanan[i].status != "Pending") continue;

        adaPending = true;

        cout << "\nPesanan #" << daftarPesanan[i].idPesanan
             << " | " << daftarPesanan[i].tanggal
             << " " << daftarPesanan[i].waktu
             << " | Status: " << daftarPesanan[i].status << "\n";

        for (size_t j = 0; j < daftarPesanan[i].daftarItem.size(); j++) {
            cout << "  - " << daftarPesanan[i].daftarItem[j].namaMenu
                 << " x" << daftarPesanan[i].daftarItem[j].jumlah
                 << " = " << formatRupiah(daftarPesanan[i].daftarItem[j].subtotal)
                 << "\n";
        }

        cout << "  Total: " << formatRupiah(daftarPesanan[i].totalHarga) << "\n";
    }

    if (!adaPending) {
        cout << "\nTidak ada pesanan yang menunggu pembayaran.\n";
    }

    cout << "\n-- RIWAYAT SUDAH DIBAYAR --\n";

    bool adaData = false;
    int totalSaya = 0;

    for (size_t i = 0; i < daftarTransaksi.size(); i++) {

        if (daftarTransaksi[i].customerUsername != userLogin) continue;

        adaData = true;

        cout << "\nTransaksi #" << daftarTransaksi[i].idTransaksi
             << " | " << daftarTransaksi[i].tanggal
             << " " << daftarTransaksi[i].waktu << "\n";

        for (size_t j = 0; j < daftarTransaksi[i].daftarItem.size(); j++) {
            cout << "  - " << daftarTransaksi[i].daftarItem[j].namaMenu
                 << " x" << daftarTransaksi[i].daftarItem[j].jumlah
                 << " = " << formatRupiah(daftarTransaksi[i].daftarItem[j].subtotal)
                 << "\n";
        }

        cout << "  Total: " << formatRupiah(daftarTransaksi[i].totalBayar) << "\n";
        garis('-');

        totalSaya += daftarTransaksi[i].totalBayar;
    }

    if (!adaData) {
        cout << "\nBelum ada riwayat pembayaran.\n";
    } else {
        cout << "\nTOTAL PENGELUARAN ANDA : " << formatRupiah(totalSaya) << "\n";
    }

    cout << "\n";
    jedaLayar();
}

void cariTransaksiByTanggal() {

    clearScreen();

    garis();
    cout << "        CARI TRANSAKSI BERDASARKAN TANGGAL\n";
    garis();

    string tanggal = inputString("Masukkan tanggal (format DD-MM-YYYY) : ");

    bool ketemu = false;
    int totalTanggalIni = 0;

    for (size_t i = 0; i < daftarTransaksi.size(); i++) {

        if (daftarTransaksi[i].tanggal != tanggal) continue;

        ketemu = true;

        cout << "\nTransaksi #" << daftarTransaksi[i].idTransaksi
             << " | " << daftarTransaksi[i].waktu
             << " | Kasir: " << daftarTransaksi[i].kasirUsername << "\n";

        cout << "  Total: " << formatRupiah(daftarTransaksi[i].totalBayar) << "\n";

        totalTanggalIni += daftarTransaksi[i].totalBayar;
    }

    if (!ketemu) {
        cout << "\nTidak ada transaksi pada tanggal tersebut.\n";
    } else {
        garis('-');
        cout << "TOTAL PENJUALAN TANGGAL " << tanggal << " : "
             << formatRupiah(totalTanggalIni) << "\n";
    }

    cout << "\n";
    jedaLayar();
}

/* ============================================================
   FITUR: LAPORAN PENJUALAN (ADMIN)
   ============================================================ */

void laporanMenuTerlaris() {

    clearScreen();

    garis();
    cout << "          LAPORAN MENU TERLARIS\n";
    garis();

    if (daftarTransaksi.empty()) {
        cout << "Belum ada data transaksi.\n\n";
        jedaLayar();
        return;
    }

    // Hitung total terjual per nama menu
    vector<string> namaUnik;
    vector<int>    jumlahTerjual;
    vector<int>    pendapatanMenu;

    for (size_t i = 0; i < daftarTransaksi.size(); i++) {

        for (size_t j = 0; j < daftarTransaksi[i].daftarItem.size(); j++) {

            string nama = daftarTransaksi[i].daftarItem[j].namaMenu;
            int    jml  = daftarTransaksi[i].daftarItem[j].jumlah;
            int    sub  = daftarTransaksi[i].daftarItem[j].subtotal;

            bool ditemukan = false;

            for (size_t k = 0; k < namaUnik.size(); k++) {
                if (namaUnik[k] == nama) {
                    jumlahTerjual[k]  += jml;
                    pendapatanMenu[k] += sub;
                    ditemukan = true;
                    break;
                }
            }

            if (!ditemukan) {
                namaUnik.push_back(nama);
                jumlahTerjual.push_back(jml);
                pendapatanMenu.push_back(sub);
            }
        }
    }

    // Urutkan menurun berdasarkan jumlah terjual (selection sort sederhana)
    for (size_t i = 0; i < namaUnik.size(); i++) {
        size_t idxMax = i;

        for (size_t j = i + 1; j < namaUnik.size(); j++) {
            if (jumlahTerjual[j] > jumlahTerjual[idxMax]) {
                idxMax = j;
            }
        }

        if (idxMax != i) {
            swap(namaUnik[i], namaUnik[idxMax]);
            swap(jumlahTerjual[i], jumlahTerjual[idxMax]);
            swap(pendapatanMenu[i], pendapatanMenu[idxMax]);
        }
    }

    cout << left << setw(5) << "No"
         << setw(28) << "Nama Menu"
         << setw(10) << "Terjual"
         << setw(15) << "Pendapatan" << "\n";

    garis('-');

    for (size_t i = 0; i < namaUnik.size(); i++) {

        cout << left << setw(5) << (i + 1)
             << setw(28) << namaUnik[i]
             << setw(10) << jumlahTerjual[i]
             << setw(15) << formatRupiah(pendapatanMenu[i])
             << "\n";
    }

    cout << "\n";
    jedaLayar();
}

void laporanRekapHarian() {

    clearScreen();

    garis();
    cout << "          REKAP PENJUALAN PER HARI\n";
    garis();

    if (daftarTransaksi.empty()) {
        cout << "Belum ada data transaksi.\n\n";
        jedaLayar();
        return;
    }

    vector<string> tanggalUnik;
    vector<int>    totalPerTanggal;
    vector<int>    jumlahTrxPerTanggal;

    for (size_t i = 0; i < daftarTransaksi.size(); i++) {

        string tgl = daftarTransaksi[i].tanggal;
        bool ditemukan = false;

        for (size_t k = 0; k < tanggalUnik.size(); k++) {
            if (tanggalUnik[k] == tgl) {
                totalPerTanggal[k]     += daftarTransaksi[i].totalBayar;
                jumlahTrxPerTanggal[k] += 1;
                ditemukan = true;
                break;
            }
        }

        if (!ditemukan) {
            tanggalUnik.push_back(tgl);
            totalPerTanggal.push_back(daftarTransaksi[i].totalBayar);
            jumlahTrxPerTanggal.push_back(1);
        }
    }

    cout << left << setw(15) << "Tanggal"
         << setw(15) << "Jml Transaksi"
         << setw(15) << "Total" << "\n";

    garis('-');

    int grandTotal = 0;

    for (size_t i = 0; i < tanggalUnik.size(); i++) {

        cout << left << setw(15) << tanggalUnik[i]
             << setw(15) << jumlahTrxPerTanggal[i]
             << setw(15) << formatRupiah(totalPerTanggal[i])
             << "\n";

        grandTotal += totalPerTanggal[i];
    }

    garis('-');
    cout << "GRAND TOTAL : " << formatRupiah(grandTotal) << "\n\n";

    jedaLayar();
}

void menuLaporanTransaksi() {

    int pilihan;

    do {
        clearScreen();

        garis();
        cout << "         LAPORAN & TRANSAKSI\n";
        garis();
        cout << "1. Lihat Semua Transaksi\n";
        cout << "2. Cari Transaksi by Tanggal\n";
        cout << "3. Laporan Menu Terlaris\n";
        cout << "4. Rekap Penjualan Per Hari\n";
        cout << "0. Kembali\n";
        garis();

        pilihan = inputIntRange("Pilih Menu : ", 0, 4);

        switch (pilihan) {
            case 1: lihatSemuaTransaksi();    break;
            case 2: cariTransaksiByTanggal(); break;
            case 3: laporanMenuTerlaris();    break;
            case 4: laporanRekapHarian();     break;
        }

    } while (pilihan != 0);
}

/* ============================================================
   TAMPILAN MENU NAVIGASI
   ============================================================ */

void tampilMenuAwal() {

    garis();
    cout << "      SISTEM INFORMASI KAFE - " << NAMA_KAFE << "\n";
    garis();
    cout << "1. Registrasi Akun\n";
    cout << "2. Login\n";
    cout << "0. Keluar\n";
    garis();
}

void tampilMenuAdmin() {

    garis();
    cout << "      MENU ADMIN - Selamat datang, "
         << (namaLogin == "-" ? userLogin : namaLogin) << "\n";
    garis();
    cout << "1. Lihat Menu (Makanan & Minuman)\n";
    cout << "2. Kelola Menu (Tambah/Edit/Hapus)\n";
    cout << "3. Tambah Pegawai\n";
    cout << "4. Daftar Pegawai\n";
    cout << "5. Nonaktifkan/Aktifkan Pegawai\n";
    cout << "6. Laporan & Transaksi\n";
    cout << "7. Logout\n";
    cout << "0. Keluar\n";
    garis();
}

void tampilMenuKasir() {

    garis();
    cout << "      MENU KASIR - Selamat datang, "
         << (namaLogin == "-" ? userLogin : namaLogin) << "\n";
    garis();
    cout << "1. Lihat Menu (Makanan & Minuman)\n";
    cout << "2. Proses Pembayaran (Antrian Pesanan)\n";
    cout << "3. Riwayat Transaksi\n";
    cout << "4. Logout\n";
    cout << "0. Keluar\n";
    garis();
}

void tampilMenuCustomer() {

    garis();
    cout << "      MENU CUSTOMER - Selamat datang, "
         << (namaLogin == "-" ? userLogin : namaLogin) << "\n";
    garis();
    cout << "1. Lihat Menu (Makanan & Minuman)\n";
    cout << "2. Pesan Menu\n";
    cout << "3. Riwayat Pesanan Saya\n";
    cout << "4. Logout\n";
    cout << "0. Keluar\n";
    garis();
}

/* ============================================================
   HANDLER PER ROLE
   ============================================================ */

void jalankanMenuAdmin(int pilihan, bool &sudahLogin) {

    switch (pilihan) {

        case 1:
            tampilMenuMakanan();
            tampilMenuMinuman();
            break;

        case 2:
            menuKelolaMenu();
            break;

        case 3:
            tambahPegawai();
            break;

        case 4:
            lihatDaftarPegawai();
            break;

        case 5:
            nonaktifkanPegawai();
            break;

        case 6:
            menuLaporanTransaksi();
            break;

        case 7:
            logout();
            sudahLogin = false;
            break;

        case 0:
            clearScreen();
            cout << "Terima kasih telah menggunakan sistem!\n";
            break;

        default:
            cout << "\nPilihan tidak tersedia!\n";
            jedaLayar();
    }
}

void jalankanMenuKasir(int pilihan, bool &sudahLogin) {

    switch (pilihan) {

        case 1:
            tampilMenuMakanan();
            tampilMenuMinuman();
            break;

        case 2:
            prosesPembayaranKasir();
            break;

        case 3:
            lihatSemuaTransaksi();
            break;

        case 4:
            logout();
            sudahLogin = false;
            break;

        case 0:
            clearScreen();
            cout << "Terima kasih telah menggunakan sistem!\n";
            break;

        default:
            cout << "\nPilihan tidak tersedia!\n";
            jedaLayar();
    }
}

void jalankanMenuCustomer(int pilihan, bool &sudahLogin) {

    switch (pilihan) {

        case 1:
            tampilMenuMakanan();
            tampilMenuMinuman();
            break;

        case 2:
            buatPesananBaru();
            break;

        case 3:
            lihatPesananSayaSendiri();
            break;

        case 4:
            logout();
            sudahLogin = false;
            break;

        case 0:
            clearScreen();
            cout << "Terima kasih telah menggunakan sistem!\n";
            break;

        default:
            cout << "\nPilihan tidak tersedia!\n";
            jedaLayar();
    }
}

void jalankanMenuAwal(int pilihan, bool &sudahLogin) {

    switch (pilihan) {

        case 1:
            registrasi();
            break;

        case 2:
            sudahLogin = login();
            break;

        case 0:
            clearScreen();
            cout << "Terima kasih telah menggunakan sistem!\n";
            break;

        default:
            cout << "\nPilihan tidak tersedia!\n";
            jedaLayar();
    }
}

/* ============================================================
   INISIALISASI AKUN ADMIN DEFAULT
   ============================================================ */

void siapkanAdminDefault() {

    // Pastikan selalu ada minimal 1 akun admin bawaan sistem
    if (cariIndexUserByUsername("admin") == -1) {

        User admin;
        admin.username    = "admin";
        admin.password    = "123";
        admin.role        = "Admin";
        admin.namaLengkap = "Administrator";
        admin.aktif       = true;

        daftarUser.push_back(admin);
        simpanSatuUser(admin);
    }
}

/* ============================================================
   FUNGSI MAIN
   ============================================================ */

int main() {

    int pilihan;
    bool sudahLogin = false;

    // Muat seluruh data dari file (persist antar sesi program)
    loadUserDariFile();
    siapkanAdminDefault();
    siapkanDataMenu();
    loadTransaksiDariFile();
    loadPesananDariFile();

    do {
        clearScreen();

        if (!sudahLogin) {
            tampilMenuAwal();
            pilihan = inputIntRange("Pilih Menu : ", 0, 2);
            jalankanMenuAwal(pilihan, sudahLogin);
        }
        else {

            if (roleLogin == "Admin") {
                tampilMenuAdmin();
                pilihan = inputIntRange("Pilih Menu : ", 0, 7);
                jalankanMenuAdmin(pilihan, sudahLogin);
            }
            else if (roleLogin == "Kasir") {
                tampilMenuKasir();
                pilihan = inputIntRange("Pilih Menu : ", 0, 4);
                jalankanMenuKasir(pilihan, sudahLogin);
            }
            else if (roleLogin == "Customer") {
                tampilMenuCustomer();
                pilihan = inputIntRange("Pilih Menu : ", 0, 4);
                jalankanMenuCustomer(pilihan, sudahLogin);
            }
        }

    } while (pilihan != 0);

    return 0;
}
