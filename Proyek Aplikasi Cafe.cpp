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
#include <iomanip>
#include <string>
#include <sstream>
#include <ctime>
#include <limits>

using namespace std;

/* ============================================================
   KONSTANTA GLOBAL
   ============================================================ */

const string FILE_USER          = "user.txt";
const string FILE_MENU          = "menu.txt";
const string FILE_TRANSAKSI     = "transaksi.txt";
const string FILE_PESANAN       = "pesanan.txt";
const string FILE_SUPPLIER      = "supplier.txt";
const string FILE_STOK          = "stok_log.txt";
const string FILE_PEMBELIAN     = "pembelian.txt";
const string FILE_PROMO         = "promo.txt";
const string NAMA_KAFE          = "KOBESSAH KOPI";
const int    LEBAR_LAYAR        = 50;

/* ============================================================
   LIST DINAMIS MANUAL
   ============================================================ */

template <class T>
class ManualList {
private:
    T* data;
    int banyakData;
    int kapasitas;

    void perbesarKapasitas() {
        int kapasitasBaru = (kapasitas == 0) ? 4 : kapasitas * 2;
        T* dataBaru = new T[kapasitasBaru];

        for (int i = 0; i < banyakData; i++) {
            dataBaru[i] = data[i];
        }

        delete[] data;
        data = dataBaru;
        kapasitas = kapasitasBaru;
    }

public:
    ManualList() {
        data = NULL;
        banyakData = 0;
        kapasitas = 0;
    }

    ManualList(const ManualList<T>& lain) {
        banyakData = lain.banyakData;
        kapasitas = lain.kapasitas;
        data = NULL;

        if (kapasitas > 0) {
            data = new T[kapasitas];
            for (int i = 0; i < banyakData; i++) {
                data[i] = lain.data[i];
            }
        }
    }

    ~ManualList() {
        delete[] data;
    }

    ManualList<T>& operator=(const ManualList<T>& lain) {
        if (this != &lain) {
            delete[] data;

            banyakData = lain.banyakData;
            kapasitas = lain.kapasitas;
            data = NULL;

            if (kapasitas > 0) {
                data = new T[kapasitas];
                for (int i = 0; i < banyakData; i++) {
                    data[i] = lain.data[i];
                }
            }
        }

        return *this;
    }

    void push_back(const T& nilai) {
        if (banyakData >= kapasitas) {
            perbesarKapasitas();
        }

        data[banyakData] = nilai;
        banyakData++;
    }

    void clear() {
        banyakData = 0;
    }

    bool empty() const {
        return banyakData == 0;
    }

    size_t size() const {
        return banyakData;
    }

    void eraseAt(int index) {
        if (index < 0 || index >= banyakData) {
            return;
        }

        for (int i = index; i < banyakData - 1; i++) {
            data[i] = data[i + 1];
        }

        banyakData--;
    }

    T& operator[](int index) {
        return data[index];
    }

    const T& operator[](int index) const {
        return data[index];
    }
};

/* ============================================================
   STRUCT / CLASS DATA
   ============================================================ */

class User {
public:
    string username;
    string password;
    string role;       
    string namaLengkap;
    bool   aktif;

    User() {
        aktif = true;
        namaLengkap = "-";
    }
};


class MenuItem {
public:
    int    id;
    string tipe;        // "Makanan" / "Minuman"
    string kategori;
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

struct Tanggal {
    int hari;
    int bulan;
    int tahun;

    Tanggal() {
        hari = 0;
        bulan = 0;
        tahun = 0;
    }

    Tanggal(int h, int b, int t) {
        hari = h;
        bulan = b;
        tahun = t;
    }

    Tanggal(string teks) {
        isiDariString(teks);
    }

    void isiDariString(string teks) {
        hari = 0;
        bulan = 0;
        tahun = 0;

        if (teks.size() >= 10) {
            hari  = atoi(teks.substr(0, 2).c_str());
            bulan = atoi(teks.substr(3, 2).c_str());
            tahun = atoi(teks.substr(6, 4).c_str());
        }
    }

    string keString() const {
        ostringstream oss;
        oss << setfill('0') << setw(2) << hari << "-"
            << setfill('0') << setw(2) << bulan << "-"
            << setw(4) << tahun;
        return oss.str();
    }

    Tanggal& operator=(string teks) {
        isiDariString(teks);
        return *this;
    }

    operator string() const {
        return keString();
    }
};

struct Waktu {
    int jam;
    int menit;
    int detik;

    Waktu() {
        jam = 0;
        menit = 0;
        detik = 0;
    }

    Waktu(int j, int m, int d) {
        jam = j;
        menit = m;
        detik = d;
    }

    Waktu(string teks) {
        isiDariString(teks);
    }

    void isiDariString(string teks) {
        jam = 0;
        menit = 0;
        detik = 0;

        if (teks.size() >= 8) {
            jam   = atoi(teks.substr(0, 2).c_str());
            menit = atoi(teks.substr(3, 2).c_str());
            detik = atoi(teks.substr(6, 2).c_str());
        }
    }

    string keString() const {
        ostringstream oss;
        oss << setfill('0') << setw(2) << jam << ":"
            << setfill('0') << setw(2) << menit << ":"
            << setfill('0') << setw(2) << detik;
        return oss.str();
    }

    Waktu& operator=(string teks) {
        isiDariString(teks);
        return *this;
    }

    operator string() const {
        return keString();
    }
};

ostream& operator<<(ostream& out, const Tanggal& tanggal) {
    out << tanggal.keString();
    return out;
}

ostream& operator<<(ostream& out, const Waktu& waktu) {
    out << waktu.keString();
    return out;
}

bool operator!=(const Tanggal& tanggal, const string& teks) {
    return tanggal.keString() != teks;
}

bool operator==(const Tanggal& tanggal, const string& teks) {
    return tanggal.keString() == teks;
}

// Satu transaksi penuh (bisa berisi banyak item)
class Transaksi {
public:
    int    idTransaksi;
    Tanggal tanggal;
    Waktu  waktu;
    string kasirUsername;
    string customerUsername;
    ManualList<ItemTransaksi> daftarItem;
    int    subtotalTransaksi;
    string kodePromo;
    int    diskonPromo;
    int    totalBayar;
    int    uangBayar;
    int    kembalian;

    Transaksi() {
        idTransaksi = 0;
        subtotalTransaksi = 0;
        kodePromo = "-";
        diskonPromo = 0;
        totalBayar = 0;
        uangBayar = 0;
        kembalian = 0;
    }
};

// Satu pesanan dari customer yang menunggu diproses pembayarannya oleh Kasir
class Pesanan {
public:
    int    idPesanan;
    Tanggal tanggal;
    Waktu  waktu;
    string customerUsername;
    ManualList<ItemTransaksi> daftarItem;
    int    totalHarga;
    string status;   // "Pending" / "Selesai" / "Dibatalkan"

    Pesanan() {
        idPesanan = 0;
        totalHarga = 0;
        status = "Pending";
    }
};

// Data supplier/pemasok bahan baku atau menu untuk kafe
class Supplier {
public:
    int    idSupplier;
    string namaSupplier;
    string alamat;
    bool   aktif;

    Supplier() {
        idSupplier = 0;
        aktif = true;
    }
};


// Catatan pergerakan stok (masuk/keluar) untuk satu barang/menu,
class CatatanStok {
public:
    int    idCatatan;
    Tanggal tanggal;
    Waktu  waktu;
    string kodeBarang;     // bisa merujuk ke id MenuItem, atau nama bahan baku
    string namaBarang;
    int    stokMasuk;
    int    stokKeluar;
    string keterangan;     // contoh: "Pembelian dari supplier", "Penjualan", "Koreksi stok"

    CatatanStok() {
        idCatatan  = 0;
        stokMasuk  = 0;
        stokKeluar = 0;
    }
};

// Satu baris barang yang dibeli dalam satu transaksi pembelian dari supplier
class ItemPembelian {
public:
    string namaBarang;
    int    hargaSatuan;
    int    jumlah;
    int    subtotal;

    ItemPembelian() {
        hargaSatuan = 0;
        jumlah = 0;
        subtotal = 0;
    }
};

// Transaksi pembelian stok/bahan dari supplier
class TransaksiPembelian {
public:
    int    idPembelian;
    Tanggal tanggal;
    Waktu  waktu;
    int    idSupplier;
    string namaSupplier;
    ManualList<ItemPembelian> daftarBarang;
    int    totalBeli;
    string adminUsername;   // admin/pegawai yang mencatat pembelian

    TransaksiPembelian() {
        idPembelian = 0;
        idSupplier  = 0;
        totalBeli   = 0;
    }
};

// Data diskon/promo yang bisa diterapkan kasir saat pembayaran
class Promo {
public:
    string kodePromo;
    string namaPromo;
    int    besarDiskonPersen;   // diskon dalam persen, contoh: 10 = potong 10%
    bool   aktif;

    Promo() {
        besarDiskonPersen = 0;
        aktif = true;
    }
};

/* ============================================================
   VARIABEL GLOBAL
   ============================================================ */

ManualList<User>               daftarUser;
ManualList<MenuItem>           daftarMenu;
ManualList<Transaksi>          daftarTransaksi;
ManualList<Pesanan>            daftarPesanan;
ManualList<ItemTransaksi>      keranjang;       // keranjang sementara saat input pesanan
ManualList<Supplier>           daftarSupplier;
ManualList<CatatanStok>        daftarCatatanStok;
ManualList<TransaksiPembelian> daftarPembelian;
ManualList<Promo>              daftarPromo;

string roleLogin    = "";
string userLogin    = "";
string namaLogin    = "";

int counterIdMenu       = 1;
int counterIdTransaksi  = 1;
int counterIdPesanan    = 1;
int counterIdSupplier   = 1;
int counterIdCatatanStok = 1;
int counterIdPembelian  = 1;

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

Tanggal getTanggalSekarang() {
    time_t t = time(0);
    tm *now = localtime(&t);

    return Tanggal(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
}

Waktu getWaktuSekarang() {
    time_t t = time(0);
    tm *now = localtime(&t);

    return Waktu(now->tm_hour, now->tm_min, now->tm_sec);
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

bool mengandungKata(string teks, string kata) {
    if (kata.size() == 0) {
        return true;
    }

    if (kata.size() > teks.size()) {
        return false;
    }

    for (size_t i = 0; i <= teks.size() - kata.size(); i++) {
        bool sama = true;

        for (size_t j = 0; j < kata.size(); j++) {
            if (teks[i + j] != kata[j]) {
                sama = false;
                break;
            }
        }

        if (sama) {
            return true;
        }
    }

    return false;
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
        ManualList<string> kolom;

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

ManualList<int> getIndexMenuByTipe(string tipe) {
    ManualList<int> hasil;
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
        ManualList<string> kolom;

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

    daftarMenu.eraseAt(idx);
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

        if (mengandungKata(namaLower, kataKunci)) {

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
   PENYIMPANAN & PEMUATAN DATA SUPPLIER (FILE)
   ============================================================ */

void simpanSemuaSupplier() {

    ofstream file(FILE_SUPPLIER.c_str());

    for (size_t i = 0; i < daftarSupplier.size(); i++) {

        file << daftarSupplier[i].idSupplier << "|"
             << daftarSupplier[i].namaSupplier << "|"
             << daftarSupplier[i].alamat << "|"
             << (daftarSupplier[i].aktif ? 1 : 0)
             << "\n";
    }

    file.close();
}

void loadSupplierDariFile() {

    ifstream file(FILE_SUPPLIER.c_str());

    if (!file.is_open()) return;

    string baris;

    while (getline(file, baris)) {

        if (baris.empty()) continue;

        stringstream ss(baris);
        string token;
        ManualList<string> kolom;

        while (getline(ss, token, '|')) {
            kolom.push_back(token);
        }

        if (kolom.size() < 4) continue;

        Supplier s;
        s.idSupplier   = stoi(kolom[0]);
        s.namaSupplier = kolom[1];
        s.alamat       = kolom[2];
        s.aktif        = (kolom[3] == "1");

        daftarSupplier.push_back(s);

        if (s.idSupplier >= counterIdSupplier) {
            counterIdSupplier = s.idSupplier + 1;
        }
    }

    file.close();
}

int cariIndexSupplierById(int id) {
    for (size_t i = 0; i < daftarSupplier.size(); i++) {
        if (daftarSupplier[i].idSupplier == id) {
            return (int)i;
        }
    }
    return -1;
}

/* ============================================================
   FITUR: KELOLA SUPPLIER (ADMIN) - CRUD
   ============================================================ */

void tampilDaftarSupplier() {

    clearScreen();

    garis();
    cout << "            DAFTAR SUPPLIER\n";
    garis();

    cout << left << setw(5)  << "ID"
         << setw(25) << "Nama Supplier"
         << setw(20) << "Alamat"
         << setw(10) << "Status" << "\n";

    garis('-');

    if (daftarSupplier.empty()) {
        cout << "Belum ada data supplier.\n";
    }

    for (size_t i = 0; i < daftarSupplier.size(); i++) {

        cout << left << setw(5)  << daftarSupplier[i].idSupplier
             << setw(25) << daftarSupplier[i].namaSupplier
             << setw(20) << daftarSupplier[i].alamat
             << setw(10) << (daftarSupplier[i].aktif ? "Aktif" : "Nonaktif")
             << "\n";
    }

    cout << "\n";
    jedaLayar();
}

void tambahSupplier() {

    clearScreen();

    garis();
    cout << "            TAMBAH SUPPLIER\n";
    garis();

    string nama = inputString("Nama Supplier : ");

    if (nama.empty()) {
        cout << "\nNama supplier tidak boleh kosong!\n";
        jedaLayar();
        return;
    }

    string alamat = inputString("Alamat : ");

    Supplier s;
    s.idSupplier   = counterIdSupplier++;
    s.namaSupplier = nama;
    s.alamat       = alamat.empty() ? "-" : alamat;
    s.aktif        = true;

    daftarSupplier.push_back(s);
    simpanSemuaSupplier();

    cout << "\nSupplier '" << nama << "' berhasil ditambahkan dengan ID " << s.idSupplier << "!\n";
    jedaLayar();
}

void editSupplier() {

    clearScreen();

    garis();
    cout << "             EDIT SUPPLIER\n";
    garis();

    tampilDaftarSupplier();

    clearScreen();

    int id = inputInt("Masukkan ID supplier yang ingin diedit (0 untuk batal) : ");

    if (id == 0) return;

    int idx = cariIndexSupplierById(id);

    if (idx == -1) {
        cout << "\nID supplier tidak ditemukan!\n";
        jedaLayar();
        return;
    }

    cout << "\nSupplier saat ini: " << daftarSupplier[idx].namaSupplier
         << " - " << daftarSupplier[idx].alamat << "\n\n";

    string namaBaru = inputString("Nama Baru (kosongkan jika tidak diubah) : ");

    if (!namaBaru.empty()) {
        daftarSupplier[idx].namaSupplier = namaBaru;
    }

    string alamatBaru = inputString("Alamat Baru (kosongkan jika tidak diubah) : ");

    if (!alamatBaru.empty()) {
        daftarSupplier[idx].alamat = alamatBaru;
    }

    simpanSemuaSupplier();

    cout << "\nData supplier berhasil diperbarui!\n";
    jedaLayar();
}

void hapusSupplier() {

    clearScreen();

    garis();
    cout << "             HAPUS SUPPLIER\n";
    garis();

    tampilDaftarSupplier();

    clearScreen();

    int id = inputInt("Masukkan ID supplier yang ingin dihapus (0 untuk batal) : ");

    if (id == 0) return;

    int idx = cariIndexSupplierById(id);

    if (idx == -1) {
        cout << "\nID supplier tidak ditemukan!\n";
        jedaLayar();
        return;
    }

    string namaSupplier = daftarSupplier[idx].namaSupplier;

    daftarSupplier.eraseAt(idx);
    simpanSemuaSupplier();

    cout << "\nSupplier '" << namaSupplier << "' berhasil dihapus!\n";
    jedaLayar();
}

void ubahStatusSupplier() {

    clearScreen();

    garis();
    cout << "        UBAH STATUS SUPPLIER\n";
    garis();

    tampilDaftarSupplier();

    clearScreen();

    int id = inputInt("Masukkan ID supplier (0 untuk batal) : ");

    if (id == 0) return;

    int idx = cariIndexSupplierById(id);

    if (idx == -1) {
        cout << "\nID supplier tidak ditemukan!\n";
        jedaLayar();
        return;
    }

    daftarSupplier[idx].aktif = !daftarSupplier[idx].aktif;
    simpanSemuaSupplier();

    cout << "\nStatus supplier '" << daftarSupplier[idx].namaSupplier << "' sekarang: "
         << (daftarSupplier[idx].aktif ? "Aktif" : "Nonaktif") << "\n";

    jedaLayar();
}

void menuKelolaSupplier() {

    int pilihan;

    do {
        clearScreen();

        garis();
        cout << "           KELOLA SUPPLIER\n";
        garis();
        cout << "1. Lihat Semua Supplier\n";
        cout << "2. Tambah Supplier\n";
        cout << "3. Edit Supplier\n";
        cout << "4. Hapus Supplier\n";
        cout << "5. Ubah Status Aktif/Nonaktif\n";
        cout << "0. Kembali\n";
        garis();

        pilihan = inputIntRange("Pilih Menu : ", 0, 5);

        switch (pilihan) {
            case 1: tampilDaftarSupplier();  break;
            case 2: tambahSupplier();        break;
            case 3: editSupplier();          break;
            case 4: hapusSupplier();         break;
            case 5: ubahStatusSupplier();    break;
        }

    } while (pilihan != 0);
}

/* ============================================================
   PENYIMPANAN & PEMUATAN DATA CATATAN STOK (FILE)
   ============================================================ */

void simpanSatuCatatanStok(CatatanStok c) {

    ofstream file(FILE_STOK.c_str(), ios::app);

    file << c.idCatatan << "|"
         << c.tanggal << "|"
         << c.waktu << "|"
         << c.kodeBarang << "|"
         << c.namaBarang << "|"
         << c.stokMasuk << "|"
         << c.stokKeluar << "|"
         << c.keterangan
         << "\n";

    file.close();
}

void loadCatatanStokDariFile() {

    ifstream file(FILE_STOK.c_str());

    if (!file.is_open()) return;

    string baris;

    while (getline(file, baris)) {

        if (baris.empty()) continue;

        stringstream ss(baris);
        string token;
        ManualList<string> kolom;

        while (getline(ss, token, '|')) {
            kolom.push_back(token);
        }

        if (kolom.size() < 8) continue;

        CatatanStok c;
        c.idCatatan  = stoi(kolom[0]);
        c.tanggal    = kolom[1];
        c.waktu      = kolom[2];
        c.kodeBarang = kolom[3];
        c.namaBarang = kolom[4];
        c.stokMasuk  = stoi(kolom[5]);
        c.stokKeluar = stoi(kolom[6]);
        c.keterangan = kolom[7];

        daftarCatatanStok.push_back(c);

        if (c.idCatatan >= counterIdCatatanStok) {
            counterIdCatatanStok = c.idCatatan + 1;
        }
    }

    file.close();
}

void catatPergerakanStok(string kodeBarang, string namaBarang, int masuk, int keluar, string keterangan) {

    CatatanStok c;
    c.idCatatan  = counterIdCatatanStok++;
    c.tanggal    = getTanggalSekarang();
    c.waktu      = getWaktuSekarang();
    c.kodeBarang = kodeBarang;
    c.namaBarang = namaBarang;
    c.stokMasuk  = masuk;
    c.stokKeluar = keluar;
    c.keterangan = keterangan;

    daftarCatatanStok.push_back(c);
    simpanSatuCatatanStok(c);
}

/* ============================================================
   FITUR: LIHAT RIWAYAT STOK (ADMIN)
   ============================================================ */

void lihatRiwayatStok() {

    clearScreen();

    garis();
    cout << "           RIWAYAT PERGERAKAN STOK\n";
    garis();

    if (daftarCatatanStok.empty()) {
        cout << "Belum ada catatan pergerakan stok.\n\n";
        jedaLayar();
        return;
    }

    cout << left << setw(12) << "Tanggal"
         << setw(22) << "Barang"
         << setw(8)  << "Masuk"
         << setw(8)  << "Keluar"
         << "Keterangan" << "\n";

    garis('-');

    for (size_t i = 0; i < daftarCatatanStok.size(); i++) {

        cout << left << setw(12) << daftarCatatanStok[i].tanggal
             << setw(22) << daftarCatatanStok[i].namaBarang
             << setw(8)  << daftarCatatanStok[i].stokMasuk
             << setw(8)  << daftarCatatanStok[i].stokKeluar
             << daftarCatatanStok[i].keterangan
             << "\n";
    }

    cout << "\n";
    jedaLayar();
}

void lihatStokMenuSaatIni() {

    clearScreen();

    garis();
    cout << "            STOK MENU SAAT INI\n";
    garis();

    cout << left << setw(5)  << "ID"
         << setw(30) << "Nama Menu"
         << setw(10) << "Stok"
         << "Status" << "\n";

    garis('-');

    for (size_t i = 0; i < daftarMenu.size(); i++) {

        string statusStok = "Aman";

        if (daftarMenu[i].stok <= 0) {
            statusStok = "HABIS";
        } else if (daftarMenu[i].stok <= 10) {
            statusStok = "Menipis";
        }

        cout << left << setw(5)  << daftarMenu[i].id
             << setw(30) << daftarMenu[i].nama
             << setw(10) << daftarMenu[i].stok
             << statusStok
             << "\n";
    }

    cout << "\n";
    jedaLayar();
}

/* ============================================================
   PENYIMPANAN & PEMUATAN DATA TRANSAKSI PEMBELIAN (FILE)
   ============================================================ */

void simpanPembelianKeFile(TransaksiPembelian tb) {

    ofstream file(FILE_PEMBELIAN.c_str(), ios::app);

    // Format: ID|TANGGAL|WAKTU|ID_SUPPLIER|NAMA_SUPPLIER|TOTAL|ADMIN|JML_ITEM|nama:harga:jumlah;...
    file << tb.idPembelian << "|"
         << tb.tanggal << "|"
         << tb.waktu << "|"
         << tb.idSupplier << "|"
         << tb.namaSupplier << "|"
         << tb.totalBeli << "|"
         << tb.adminUsername << "|"
         << tb.daftarBarang.size() << "|";

    for (size_t i = 0; i < tb.daftarBarang.size(); i++) {
        file << tb.daftarBarang[i].namaBarang << ":"
             << tb.daftarBarang[i].hargaSatuan << ":"
             << tb.daftarBarang[i].jumlah;

        if (i != tb.daftarBarang.size() - 1) {
            file << ";";
        }
    }

    file << "\n";
    file.close();
}

void loadPembelianDariFile() {

    ifstream file(FILE_PEMBELIAN.c_str());

    if (!file.is_open()) return;

    string baris;

    while (getline(file, baris)) {

        if (baris.empty()) continue;

        stringstream ss(baris);
        string token;
        ManualList<string> kolom;

        while (getline(ss, token, '|')) {
            kolom.push_back(token);
        }

        if (kolom.size() < 8) continue;

        TransaksiPembelian tb;
        tb.idPembelian   = stoi(kolom[0]);
        tb.tanggal       = kolom[1];
        tb.waktu         = kolom[2];
        tb.idSupplier    = stoi(kolom[3]);
        tb.namaSupplier  = kolom[4];
        tb.totalBeli     = stoi(kolom[5]);
        tb.adminUsername = kolom[6];

        int jumlahItem = stoi(kolom[7]);

        if (kolom.size() > 8 && jumlahItem > 0) {

            stringstream itemStream(kolom[8]);
            string itemToken;

            while (getline(itemStream, itemToken, ';')) {

                stringstream itemSS(itemToken);
                string namaI, hargaI, jumlahI;

                getline(itemSS, namaI, ':');
                getline(itemSS, hargaI, ':');
                getline(itemSS, jumlahI, ':');

                if (namaI.empty()) continue;

                ItemPembelian ib;
                ib.namaBarang   = namaI;
                ib.hargaSatuan  = stoi(hargaI);
                ib.jumlah       = stoi(jumlahI);
                ib.subtotal     = ib.hargaSatuan * ib.jumlah;

                tb.daftarBarang.push_back(ib);
            }
        }

        daftarPembelian.push_back(tb);

        if (tb.idPembelian >= counterIdPembelian) {
            counterIdPembelian = tb.idPembelian + 1;
        }
    }

    file.close();
}

/* ============================================================
   FITUR: TRANSAKSI PEMBELIAN STOK DARI SUPPLIER (ADMIN)
   ============================================================ */

void buatTransaksiPembelian() {

    clearScreen();

    garis();
    cout << "        PEMBELIAN STOK DARI SUPPLIER\n";
    garis();

    if (daftarSupplier.empty()) {
        cout << "Belum ada data supplier. Tambahkan supplier terlebih dahulu!\n\n";
        jedaLayar();
        return;
    }

    cout << left << setw(5) << "ID" << "Nama Supplier\n";
    garis('-');

    for (size_t i = 0; i < daftarSupplier.size(); i++) {
        if (!daftarSupplier[i].aktif) continue;
        cout << left << setw(5) << daftarSupplier[i].idSupplier
             << daftarSupplier[i].namaSupplier << "\n";
    }

    int idSupplier = inputInt("\nPilih ID Supplier (0 untuk batal) : ");

    if (idSupplier == 0) return;

    int idxSupplier = cariIndexSupplierById(idSupplier);

    if (idxSupplier == -1) {
        cout << "\nSupplier tidak ditemukan!\n";
        jedaLayar();
        return;
    }

    ManualList<ItemPembelian> daftarBeli;
    int pilihanLanjut;

    do {
        clearScreen();

        garis();
        cout << "   PEMBELIAN DARI: " << daftarSupplier[idxSupplier].namaSupplier << "\n";
        garis();

        if (!daftarBeli.empty()) {
            cout << "\nBarang yang sudah diinput:\n";
            for (size_t i = 0; i < daftarBeli.size(); i++) {
                cout << "  - " << daftarBeli[i].namaBarang
                     << " x" << daftarBeli[i].jumlah
                     << " = " << formatRupiah(daftarBeli[i].subtotal) << "\n";
            }
        }

        string namaBarang = inputString("\nNama Barang/Bahan (kosongkan untuk selesai) : ");

        if (namaBarang.empty()) break;

        int hargaSatuan = inputInt("Harga Satuan : ");
        int jumlah      = inputInt("Jumlah : ");

        if (hargaSatuan <= 0 || jumlah <= 0) {
            cout << "\nHarga dan jumlah harus lebih dari 0!\n";
            jedaLayar();
            continue;
        }

        ItemPembelian ib;
        ib.namaBarang  = namaBarang;
        ib.hargaSatuan = hargaSatuan;
        ib.jumlah      = jumlah;
        ib.subtotal    = hargaSatuan * jumlah;

        daftarBeli.push_back(ib);

        pilihanLanjut = inputIntRange("Tambah barang lain? (1=Ya, 0=Selesai) : ", 0, 1);

        if (pilihanLanjut == 0) break;

    } while (true);

    if (daftarBeli.empty()) {
        cout << "\nTidak ada barang yang dibeli.\n";
        jedaLayar();
        return;
    }

    int totalBeli = 0;
    for (size_t i = 0; i < daftarBeli.size(); i++) {
        totalBeli += daftarBeli[i].subtotal;
    }

    TransaksiPembelian tb;
    tb.idPembelian   = counterIdPembelian++;
    tb.tanggal       = getTanggalSekarang();
    tb.waktu         = getWaktuSekarang();
    tb.idSupplier    = daftarSupplier[idxSupplier].idSupplier;
    tb.namaSupplier  = daftarSupplier[idxSupplier].namaSupplier;
    tb.daftarBarang  = daftarBeli;
    tb.totalBeli     = totalBeli;
    tb.adminUsername = userLogin;

    daftarPembelian.push_back(tb);
    simpanPembelianKeFile(tb);

    // Setiap barang yang dibeli dicatat sebagai stok masuk di riwayat stok.
    // Jika nama barang cocok dengan nama menu yang ada, stok menu itu juga ditambahkan otomatis.
    for (size_t i = 0; i < daftarBeli.size(); i++) {

        bool cocokMenu = false;

        for (size_t j = 0; j < daftarMenu.size(); j++) {
            if (toLowerStr(daftarMenu[j].nama) == toLowerStr(daftarBeli[i].namaBarang)) {
                daftarMenu[j].stok += daftarBeli[i].jumlah;
                cocokMenu = true;
            }
        }

        catatPergerakanStok(
            cocokMenu ? "MENU" : "BAHAN",
            daftarBeli[i].namaBarang,
            daftarBeli[i].jumlah,
            0,
            "Pembelian dari supplier " + daftarSupplier[idxSupplier].namaSupplier
        );
    }

    simpanSemuaMenu();

    clearScreen();
    garis();
    cout << "        PEMBELIAN BERHASIL DICATAT\n";
    garis();
    cout << "ID Pembelian  : " << tb.idPembelian << "\n";
    cout << "Supplier      : " << tb.namaSupplier << "\n";
    cout << "Tanggal       : " << tb.tanggal << " " << tb.waktu << "\n\n";

    for (size_t i = 0; i < daftarBeli.size(); i++) {
        cout << "  - " << daftarBeli[i].namaBarang
             << " x" << daftarBeli[i].jumlah
             << " = " << formatRupiah(daftarBeli[i].subtotal) << "\n";
    }

    garis('-');
    cout << "TOTAL PEMBELIAN : " << formatRupiah(totalBeli) << "\n\n";

    jedaLayar();
}

void lihatRiwayatPembelian() {

    clearScreen();

    garis();
    cout << "          RIWAYAT PEMBELIAN STOK\n";
    garis();

    if (daftarPembelian.empty()) {
        cout << "Belum ada riwayat pembelian.\n\n";
        jedaLayar();
        return;
    }

    int grandTotal = 0;

    for (size_t i = 0; i < daftarPembelian.size(); i++) {

        cout << "\nPembelian #" << daftarPembelian[i].idPembelian
             << " | " << daftarPembelian[i].tanggal
             << " " << daftarPembelian[i].waktu << "\n";

        cout << "Supplier : " << daftarPembelian[i].namaSupplier
             << " | Dicatat oleh: " << daftarPembelian[i].adminUsername << "\n";

        for (size_t j = 0; j < daftarPembelian[i].daftarBarang.size(); j++) {
            cout << "  - " << daftarPembelian[i].daftarBarang[j].namaBarang
                 << " x" << daftarPembelian[i].daftarBarang[j].jumlah
                 << " = " << formatRupiah(daftarPembelian[i].daftarBarang[j].subtotal)
                 << "\n";
        }

        cout << "  Total: " << formatRupiah(daftarPembelian[i].totalBeli) << "\n";
        garis('-');

        grandTotal += daftarPembelian[i].totalBeli;
    }

    cout << "\nGRAND TOTAL SELURUH PEMBELIAN : " << formatRupiah(grandTotal) << "\n\n";

    jedaLayar();
}

void menuKelolaStokPembelian() {

    int pilihan;

    do {
        clearScreen();

        garis();
        cout << "        STOK & PEMBELIAN BARANG\n";
        garis();
        cout << "1. Lihat Stok Menu Saat Ini\n";
        cout << "2. Riwayat Pergerakan Stok\n";
        cout << "3. Catat Pembelian dari Supplier\n";
        cout << "4. Riwayat Pembelian\n";
        cout << "0. Kembali\n";
        garis();

        pilihan = inputIntRange("Pilih Menu : ", 0, 4);

        switch (pilihan) {
            case 1: lihatStokMenuSaatIni();     break;
            case 2: lihatRiwayatStok();         break;
            case 3: buatTransaksiPembelian();   break;
            case 4: lihatRiwayatPembelian();    break;
        }

    } while (pilihan != 0);
}

/* ============================================================
   FITUR: PROMO / DISKON
   ============================================================ */

int cariIndexPromoByKode(string kodePromo) {
    string kodeDicari = toUpperStr(kodePromo);

    for (size_t i = 0; i < daftarPromo.size(); i++) {
        if (toUpperStr(daftarPromo[i].kodePromo) == kodeDicari) {
            return (int)i;
        }
    }

    return -1;
}

void simpanSemuaPromo() {
    ofstream file(FILE_PROMO.c_str());

    for (size_t i = 0; i < daftarPromo.size(); i++) {
        file << daftarPromo[i].kodePromo << "|"
             << daftarPromo[i].namaPromo << "|"
             << daftarPromo[i].besarDiskonPersen << "|"
             << (daftarPromo[i].aktif ? 1 : 0)
             << "\n";
    }

    file.close();
}

void loadPromoDariFile() {
    ifstream file(FILE_PROMO.c_str());

    if (!file.is_open()) {
        Promo p;
        p.kodePromo = "HEMAT10";
        p.namaPromo = "Diskon Pembukaan";
        p.besarDiskonPersen = 10;
        p.aktif = true;
        daftarPromo.push_back(p);
        simpanSemuaPromo();
        return;
    }

    string baris;

    while (getline(file, baris)) {
        if (baris.empty()) continue;

        stringstream ss(baris);
        string token;
        ManualList<string> kolom;

        while (getline(ss, token, '|')) {
            kolom.push_back(token);
        }

        if (kolom.size() < 4) continue;

        Promo p;
        p.kodePromo = toUpperStr(kolom[0]);
        p.namaPromo = kolom[1];
        p.besarDiskonPersen = stoi(kolom[2]);
        p.aktif = (kolom[3] == "1");

        if (cariIndexPromoByKode(p.kodePromo) == -1) {
            daftarPromo.push_back(p);
        }
    }

    file.close();
}

void tampilDaftarPromo(bool hanyaAktif = false) {
    clearScreen();

    garis();
    cout << "              DAFTAR PROMO\n";
    garis();

    cout << left << setw(14) << "Kode"
         << setw(25) << "Nama Promo"
         << setw(10) << "Diskon"
         << "Status\n";
    garis('-');

    bool ada = false;

    for (size_t i = 0; i < daftarPromo.size(); i++) {
        if (hanyaAktif && !daftarPromo[i].aktif) continue;

        ada = true;

        cout << left << setw(14) << daftarPromo[i].kodePromo
             << setw(25) << daftarPromo[i].namaPromo
             << setw(10) << (to_string(daftarPromo[i].besarDiskonPersen) + "%")
             << (daftarPromo[i].aktif ? "Aktif" : "Nonaktif")
             << "\n";
    }

    if (!ada) {
        cout << "Belum ada promo yang dapat ditampilkan.\n";
    }

    garis('-');
}

void tambahPromo() {
    clearScreen();

    garis();
    cout << "              TAMBAH PROMO\n";
    garis();

    Promo p;
    p.kodePromo = toUpperStr(inputString("Kode promo        : "));

    if (p.kodePromo.empty()) {
        cout << "\nKode promo tidak boleh kosong.\n";
        jedaLayar();
        return;
    }

    if (cariIndexPromoByKode(p.kodePromo) != -1) {
        cout << "\nKode promo sudah terdaftar.\n";
        jedaLayar();
        return;
    }

    p.namaPromo = inputString("Nama promo        : ");
    p.besarDiskonPersen = inputIntRange("Besar diskon (%)  : ", 1, 90);
    p.aktif = true;

    daftarPromo.push_back(p);
    simpanSemuaPromo();

    cout << "\nPromo berhasil ditambahkan.\n";
    jedaLayar();
}

void editPromo() {
    clearScreen();

    tampilDaftarPromo();

    string kode = toUpperStr(inputString("\nMasukkan kode promo yang ingin diedit (0 untuk batal) : "));

    if (kode == "0") return;

    int idx = cariIndexPromoByKode(kode);

    if (idx == -1) {
        cout << "\nPromo tidak ditemukan.\n";
        jedaLayar();
        return;
    }

    cout << "\nKosongkan input teks jika tidak ingin mengubah data.\n";

    string namaBaru = inputString("Nama promo baru        : ");
    if (!namaBaru.empty()) {
        daftarPromo[idx].namaPromo = namaBaru;
    }

    int ubahDiskon = inputIntRange("Ubah diskon? (1=Ya, 0=Tidak) : ", 0, 1);
    if (ubahDiskon == 1) {
        daftarPromo[idx].besarDiskonPersen = inputIntRange("Diskon baru (%)        : ", 1, 90);
    }

    simpanSemuaPromo();

    cout << "\nPromo berhasil diperbarui.\n";
    jedaLayar();
}

void aktifNonaktifPromo() {
    clearScreen();

    tampilDaftarPromo();

    string kode = toUpperStr(inputString("\nMasukkan kode promo yang ingin diubah statusnya (0 untuk batal) : "));

    if (kode == "0") return;

    int idx = cariIndexPromoByKode(kode);

    if (idx == -1) {
        cout << "\nPromo tidak ditemukan.\n";
        jedaLayar();
        return;
    }

    daftarPromo[idx].aktif = !daftarPromo[idx].aktif;
    simpanSemuaPromo();

    cout << "\nStatus promo '" << daftarPromo[idx].kodePromo << "' sekarang: "
         << (daftarPromo[idx].aktif ? "Aktif" : "Nonaktif") << "\n";

    jedaLayar();
}

int hitungDiskonPromo(int total, Promo p) {
    if (!p.aktif) return 0;
    return total * p.besarDiskonPersen / 100;
}

Promo pilihPromoUntukTransaksi(int totalBelanja, int &diskon) {
    Promo promoKosong;
    promoKosong.kodePromo = "-";
    promoKosong.namaPromo = "Tanpa Promo";
    promoKosong.besarDiskonPersen = 0;
    promoKosong.aktif = false;
    diskon = 0;

    if (daftarPromo.empty()) {
        return promoKosong;
    }

    cout << "\n";
    garis('-');
    cout << "PROMO AKTIF\n";
    garis('-');

    bool adaAktif = false;

    for (size_t i = 0; i < daftarPromo.size(); i++) {
        if (!daftarPromo[i].aktif) continue;

        adaAktif = true;

        cout << left << setw(14) << daftarPromo[i].kodePromo
             << setw(25) << daftarPromo[i].namaPromo
             << daftarPromo[i].besarDiskonPersen << "%\n";
    }

    if (!adaAktif) {
        cout << "Tidak ada promo aktif.\n";
        garis('-');
        return promoKosong;
    }

    garis('-');

    int pakaiPromo = inputIntRange("Gunakan promo? (1=Ya, 0=Tidak) : ", 0, 1);

    if (pakaiPromo == 0) {
        return promoKosong;
    }

    string kode = toUpperStr(inputString("Masukkan kode promo : "));
    int idx = cariIndexPromoByKode(kode);

    if (idx == -1 || !daftarPromo[idx].aktif) {
        cout << "Kode promo tidak valid. Transaksi dilanjutkan tanpa promo.\n";
        return promoKosong;
    }

    diskon = hitungDiskonPromo(totalBelanja, daftarPromo[idx]);

    cout << "Promo diterapkan: " << daftarPromo[idx].namaPromo
         << " (-" << formatRupiah(diskon) << ")\n";

    return daftarPromo[idx];
}

void menuKelolaPromo() {
    int pilihan;

    do {
        clearScreen();

        garis();
        cout << "              KELOLA PROMO\n";
        garis();
        cout << "1. Lihat Semua Promo\n";
        cout << "2. Tambah Promo\n";
        cout << "3. Edit Promo\n";
        cout << "4. Aktifkan/Nonaktifkan Promo\n";
        cout << "0. Kembali\n";
        garis();

        pilihan = inputIntRange("Pilih Menu : ", 0, 4);

        switch (pilihan) {
            case 1:
                tampilDaftarPromo();
                jedaLayar();
                break;
            case 2:
                tambahPromo();
                break;
            case 3:
                editPromo();
                break;
            case 4:
                aktifNonaktifPromo();
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
        ManualList<string> kolom;

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

    if (trx.diskonPromo > 0) {
        cout << right << setw(38) << "SUBTOTAL : " << formatRupiah(trx.subtotalTransaksi) << "\n";
        cout << right << setw(38) << ("PROMO " + trx.kodePromo + " : ") << "-" << formatRupiah(trx.diskonPromo) << "\n";
    }

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

    // Format: ID|TANGGAL|WAKTU|KASIR|CUSTOMER|TOTAL|BAYAR|KEMBALIAN|JUMLAH_ITEM|item1_nama:harga:jumlah;...|KODE_PROMO|DISKON|SUBTOTAL
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

    file << "|"
         << trx.kodePromo << "|"
         << trx.diskonPromo << "|"
         << trx.subtotalTransaksi
         << "\n";
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

    int diskonPromo = 0;
    Promo promoDipakai = pilihPromoUntukTransaksi(p.totalHarga, diskonPromo);
    int totalSetelahPromo = p.totalHarga - diskonPromo;

    if (totalSetelahPromo < 0) {
        totalSetelahPromo = 0;
    }

    if (diskonPromo > 0) {
        cout << "DISKON PROMO  : -" << formatRupiah(diskonPromo) << "\n";
        cout << "TOTAL AKHIR   : " << formatRupiah(totalSetelahPromo) << "\n";
    }

    int uangBayar;

    while (true) {
        uangBayar = inputInt("\nMasukkan jumlah uang dibayar customer : ");

        if (uangBayar < totalSetelahPromo) {
            cout << "Uang tidak cukup! Total tagihan: " << formatRupiah(totalSetelahPromo) << "\n";
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
    trx.subtotalTransaksi = p.totalHarga;
    trx.kodePromo        = promoDipakai.kodePromo;
    trx.diskonPromo      = diskonPromo;
    trx.totalBayar       = totalSetelahPromo;
    trx.uangBayar        = uangBayar;
    trx.kembalian        = uangBayar - totalSetelahPromo;

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
        ManualList<string> kolom;

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
        trx.subtotalTransaksi = trx.totalBayar;

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

        if (kolom.size() > 10) {
            trx.kodePromo = kolom[10];
        }

        if (kolom.size() > 11) {
            trx.diskonPromo = stoi(kolom[11]);
        }

        if (kolom.size() > 12) {
            trx.subtotalTransaksi = stoi(kolom[12]);
        } else if (trx.subtotalTransaksi == 0) {
            trx.subtotalTransaksi = trx.totalBayar + trx.diskonPromo;
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

        if (daftarTransaksi[i].diskonPromo > 0) {
            cout << "  Subtotal: " << formatRupiah(daftarTransaksi[i].subtotalTransaksi) << "\n";
            cout << "  Promo   : " << daftarTransaksi[i].kodePromo
                 << " (-" << formatRupiah(daftarTransaksi[i].diskonPromo) << ")\n";
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

    Tanggal tanggalDicari = inputString("Masukkan tanggal (format DD-MM-YYYY) : ");

    bool ketemu = false;
    int totalTanggalIni = 0;

    for (size_t i = 0; i < daftarTransaksi.size(); i++) {

        if (daftarTransaksi[i].tanggal != tanggalDicari.keString()) continue;

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
        cout << "TOTAL PENJUALAN TANGGAL " << tanggalDicari << " : "
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
    ManualList<string> namaUnik;
    ManualList<int>    jumlahTerjual;
    ManualList<int>    pendapatanMenu;

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

    ManualList<string> tanggalUnik;
    ManualList<int>    totalPerTanggal;
    ManualList<int>    jumlahTrxPerTanggal;

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

/* ============================================================
   FITUR TAMBAHAN: DASHBOARD, FILTER, DAN KONTROL OPERASIONAL
   ============================================================ */

int hitungJumlahPesananByStatus(string status) {
    int jumlah = 0;

    for (size_t i = 0; i < daftarPesanan.size(); i++) {
        if (daftarPesanan[i].status == status) {
            jumlah++;
        }
    }

    return jumlah;
}

int hitungTotalPenjualanSemua() {
    int total = 0;

    for (size_t i = 0; i < daftarTransaksi.size(); i++) {
        total += daftarTransaksi[i].totalBayar;
    }

    return total;
}

int hitungTotalDiskonPromo() {
    int total = 0;

    for (size_t i = 0; i < daftarTransaksi.size(); i++) {
        total += daftarTransaksi[i].diskonPromo;
    }

    return total;
}

int hitungTotalPembelianSemua() {
    int total = 0;

    for (size_t i = 0; i < daftarPembelian.size(); i++) {
        total += daftarPembelian[i].totalBeli;
    }

    return total;
}

int hitungMenuTersedia() {
    int jumlah = 0;

    for (size_t i = 0; i < daftarMenu.size(); i++) {
        if (daftarMenu[i].tersedia && daftarMenu[i].stok > 0) {
            jumlah++;
        }
    }

    return jumlah;
}

void dashboardAdmin() {
    clearScreen();

    garis();
    cout << "              DASHBOARD ADMIN\n";
    garis();

    cout << left << setw(28) << "Total User"
         << ": " << daftarUser.size() << "\n";
    cout << left << setw(28) << "Total Menu"
         << ": " << daftarMenu.size() << "\n";
    cout << left << setw(28) << "Menu Tersedia"
         << ": " << hitungMenuTersedia() << "\n";
    cout << left << setw(28) << "Pesanan Pending"
         << ": " << hitungJumlahPesananByStatus("Pending") << "\n";
    cout << left << setw(28) << "Pesanan Selesai"
         << ": " << hitungJumlahPesananByStatus("Selesai") << "\n";
    cout << left << setw(28) << "Jumlah Transaksi"
         << ": " << daftarTransaksi.size() << "\n";
    cout << left << setw(28) << "Total Penjualan"
         << ": " << formatRupiah(hitungTotalPenjualanSemua()) << "\n";
    cout << left << setw(28) << "Total Pembelian Stok"
         << ": " << formatRupiah(hitungTotalPembelianSemua()) << "\n";
    cout << left << setw(28) << "Total Diskon Promo"
         << ": " << formatRupiah(hitungTotalDiskonPromo()) << "\n";
    cout << left << setw(28) << "Promo Terdaftar"
         << ": " << daftarPromo.size() << "\n";

    garis();

    cout << "\nMenu stok rendah:\n";

    bool adaStokRendah = false;

    for (size_t i = 0; i < daftarMenu.size(); i++) {
        if (daftarMenu[i].stok <= 5) {
            adaStokRendah = true;
            cout << "  - " << daftarMenu[i].nama
                 << " tersisa " << daftarMenu[i].stok << "\n";
        }
    }

    if (!adaStokRendah) {
        cout << "  Tidak ada menu dengan stok rendah.\n";
    }

    cout << "\n";
    jedaLayar();
}

void tampilMenuStokMenipis() {
    clearScreen();

    garis();
    cout << "              MENU STOK MENIPIS\n";
    garis();

    int batas = inputIntRange("Tampilkan stok <= berapa? : ", 0, 9999);

    cout << "\n";
    cout << left << setw(5) << "ID"
         << setw(28) << "Nama Menu"
         << setw(12) << "Tipe"
         << setw(8) << "Stok"
         << "Status\n";
    garis('-');

    bool ada = false;

    for (size_t i = 0; i < daftarMenu.size(); i++) {
        if (daftarMenu[i].stok <= batas) {
            ada = true;
            cout << left << setw(5) << daftarMenu[i].id
                 << setw(28) << daftarMenu[i].nama
                 << setw(12) << daftarMenu[i].tipe
                 << setw(8) << daftarMenu[i].stok
                 << (daftarMenu[i].tersedia ? "Tersedia" : "Nonaktif")
                 << "\n";
        }
    }

    if (!ada) {
        cout << "Tidak ada menu dengan stok di bawah batas tersebut.\n";
    }

    cout << "\n";
    jedaLayar();
}

void cariPegawaiByNamaAtauUsername() {
    clearScreen();

    garis();
    cout << "              CARI PEGAWAI\n";
    garis();

    string kata = toLowerStr(inputString("Masukkan nama/username/role : "));

    cout << "\n";
    cout << left << setw(16) << "Username"
         << setw(18) << "Nama"
         << setw(12) << "Role"
         << "Status\n";
    garis('-');

    bool ketemu = false;

    for (size_t i = 0; i < daftarUser.size(); i++) {
        if (daftarUser[i].role == "Customer") continue;

        string gabungan = toLowerStr(daftarUser[i].username + " " +
                                     daftarUser[i].namaLengkap + " " +
                                     daftarUser[i].role);

        if (mengandungKata(gabungan, kata)) {
            ketemu = true;
            cout << left << setw(16) << daftarUser[i].username
                 << setw(18) << daftarUser[i].namaLengkap
                 << setw(12) << daftarUser[i].role
                 << (daftarUser[i].aktif ? "Aktif" : "Nonaktif")
                 << "\n";
        }
    }

    if (!ketemu) {
        cout << "Pegawai tidak ditemukan.\n";
    }

    cout << "\n";
    jedaLayar();
}

void resetPasswordPegawai() {
    clearScreen();

    garis();
    cout << "              RESET PASSWORD PEGAWAI\n";
    garis();

    string username = inputString("Username pegawai : ");
    int idx = cariIndexUserByUsername(username);

    if (idx == -1 || daftarUser[idx].role == "Customer") {
        cout << "\nPegawai tidak ditemukan.\n";
        jedaLayar();
        return;
    }

    string passwordBaru = inputString("Password baru    : ");

    if (passwordBaru.empty()) {
        cout << "\nPassword tidak boleh kosong.\n";
        jedaLayar();
        return;
    }

    daftarUser[idx].password = passwordBaru;
    simpanUlangSemuaUser();

    cout << "\nPassword pegawai berhasil diubah.\n";
    jedaLayar();
}

void menuKelolaPegawai() {
    int pilihan;

    do {
        clearScreen();

        garis();
        cout << "              KELOLA PEGAWAI\n";
        garis();
        cout << "1. Tambah Pegawai\n";
        cout << "2. Daftar Pegawai\n";
        cout << "3. Cari Pegawai\n";
        cout << "4. Nonaktifkan/Aktifkan Pegawai\n";
        cout << "5. Reset Password Pegawai\n";
        cout << "0. Kembali\n";
        garis();

        pilihan = inputIntRange("Pilih Menu : ", 0, 5);

        switch (pilihan) {
            case 1: tambahPegawai();                  break;
            case 2: lihatDaftarPegawai();             break;
            case 3: cariPegawaiByNamaAtauUsername();  break;
            case 4: nonaktifkanPegawai();             break;
            case 5: resetPasswordPegawai();           break;
        }

    } while (pilihan != 0);
}

void tampilPesananByStatus() {
    clearScreen();

    garis();
    cout << "              FILTER PESANAN\n";
    garis();
    cout << "1. Pending\n";
    cout << "2. Selesai\n";
    cout << "3. Dibatalkan\n";
    garis();

    int pilihan = inputIntRange("Pilih status : ", 1, 3);
    string status = "Pending";

    if (pilihan == 2) status = "Selesai";
    else if (pilihan == 3) status = "Dibatalkan";

    clearScreen();

    garis();
    cout << "          PESANAN STATUS: " << status << "\n";
    garis();

    bool ada = false;

    for (size_t i = 0; i < daftarPesanan.size(); i++) {
        if (daftarPesanan[i].status != status) continue;

        ada = true;

        cout << "\nPesanan #" << daftarPesanan[i].idPesanan
             << " | " << daftarPesanan[i].tanggal
             << " " << daftarPesanan[i].waktu
             << " | Customer: " << daftarPesanan[i].customerUsername
             << "\n";

        for (size_t j = 0; j < daftarPesanan[i].daftarItem.size(); j++) {
            cout << "  - " << daftarPesanan[i].daftarItem[j].namaMenu
                 << " x" << daftarPesanan[i].daftarItem[j].jumlah
                 << " = " << formatRupiah(daftarPesanan[i].daftarItem[j].subtotal)
                 << "\n";
        }

        cout << "  Total: " << formatRupiah(daftarPesanan[i].totalHarga) << "\n";
        garis('-');
    }

    if (!ada) {
        cout << "Tidak ada pesanan dengan status tersebut.\n";
    }

    cout << "\n";
    jedaLayar();
}

void kembalikanStokPesanan(Pesanan p) {
    for (size_t i = 0; i < p.daftarItem.size(); i++) {
        int idxMenu = cariIndexMenuById(p.daftarItem[i].idMenu);

        if (idxMenu != -1) {
            daftarMenu[idxMenu].stok += p.daftarItem[i].jumlah;
        }
    }
}

void batalkanPesananPending() {
    clearScreen();

    garis();
    cout << "              BATALKAN PESANAN PENDING\n";
    garis();

    tampilAntrianPesananPending();

    int idPesanan = inputInt("\nMasukkan ID pesanan yang dibatalkan (0 untuk batal) : ");

    if (idPesanan == 0) return;

    int idx = cariIndexPesananById(idPesanan);

    if (idx == -1 || daftarPesanan[idx].status != "Pending") {
        cout << "\nPesanan tidak ditemukan atau bukan status Pending.\n";
        jedaLayar();
        return;
    }

    int yakin = inputIntRange("Yakin batalkan pesanan ini? (1=Ya, 0=Tidak) : ", 0, 1);

    if (yakin == 0) return;

    kembalikanStokPesanan(daftarPesanan[idx]);
    daftarPesanan[idx].status = "Dibatalkan";

    simpanSemuaMenu();
    simpanUlangSemuaPesanan();

    cout << "\nPesanan dibatalkan dan stok menu sudah dikembalikan.\n";
    jedaLayar();
}

void batalkanPesananSaya() {
    clearScreen();

    garis();
    cout << "              BATALKAN PESANAN SAYA\n";
    garis();

    bool adaPending = false;

    for (size_t i = 0; i < daftarPesanan.size(); i++) {
        if (daftarPesanan[i].customerUsername != userLogin) continue;
        if (daftarPesanan[i].status != "Pending") continue;

        adaPending = true;

        cout << "ID " << daftarPesanan[i].idPesanan
             << " | " << daftarPesanan[i].tanggal
             << " " << daftarPesanan[i].waktu
             << " | Total " << formatRupiah(daftarPesanan[i].totalHarga)
             << "\n";
    }

    if (!adaPending) {
        cout << "Tidak ada pesanan Pending yang bisa dibatalkan.\n";
        jedaLayar();
        return;
    }

    int idPesanan = inputInt("\nMasukkan ID pesanan yang dibatalkan (0 untuk batal) : ");

    if (idPesanan == 0) return;

    int idx = cariIndexPesananById(idPesanan);

    if (idx == -1 ||
        daftarPesanan[idx].customerUsername != userLogin ||
        daftarPesanan[idx].status != "Pending") {
        cout << "\nPesanan tidak valid.\n";
        jedaLayar();
        return;
    }

    kembalikanStokPesanan(daftarPesanan[idx]);
    daftarPesanan[idx].status = "Dibatalkan";

    simpanSemuaMenu();
    simpanUlangSemuaPesanan();

    cout << "\nPesanan berhasil dibatalkan.\n";
    jedaLayar();
}

void laporanPenjualanPerKasir() {
    clearScreen();

    garis();
    cout << "              LAPORAN PENJUALAN PER KASIR\n";
    garis();

    if (daftarTransaksi.empty()) {
        cout << "Belum ada transaksi.\n\n";
        jedaLayar();
        return;
    }

    ManualList<string> namaKasir;
    ManualList<int> jumlahTransaksiKasir;
    ManualList<int> totalPenjualanKasir;

    for (size_t i = 0; i < daftarTransaksi.size(); i++) {
        string kasir = daftarTransaksi[i].kasirUsername;
        bool ditemukan = false;

        for (size_t j = 0; j < namaKasir.size(); j++) {
            if (namaKasir[j] == kasir) {
                jumlahTransaksiKasir[j]++;
                totalPenjualanKasir[j] += daftarTransaksi[i].totalBayar;
                ditemukan = true;
                break;
            }
        }

        if (!ditemukan) {
            namaKasir.push_back(kasir);
            jumlahTransaksiKasir.push_back(1);
            totalPenjualanKasir.push_back(daftarTransaksi[i].totalBayar);
        }
    }

    cout << left << setw(18) << "Kasir"
         << setw(15) << "Jml Transaksi"
         << "Total Penjualan\n";
    garis('-');

    for (size_t i = 0; i < namaKasir.size(); i++) {
        cout << left << setw(18) << namaKasir[i]
             << setw(15) << jumlahTransaksiKasir[i]
             << formatRupiah(totalPenjualanKasir[i])
             << "\n";
    }

    cout << "\n";
    jedaLayar();
}

void laporanPenjualanPerCustomer() {
    clearScreen();

    garis();
    cout << "              LAPORAN PENJUALAN PER CUSTOMER\n";
    garis();

    if (daftarTransaksi.empty()) {
        cout << "Belum ada transaksi.\n\n";
        jedaLayar();
        return;
    }

    ManualList<string> namaCustomer;
    ManualList<int> jumlahTransaksiCustomer;
    ManualList<int> totalBelanjaCustomer;

    for (size_t i = 0; i < daftarTransaksi.size(); i++) {
        string customer = daftarTransaksi[i].customerUsername;
        bool ditemukan = false;

        for (size_t j = 0; j < namaCustomer.size(); j++) {
            if (namaCustomer[j] == customer) {
                jumlahTransaksiCustomer[j]++;
                totalBelanjaCustomer[j] += daftarTransaksi[i].totalBayar;
                ditemukan = true;
                break;
            }
        }

        if (!ditemukan) {
            namaCustomer.push_back(customer);
            jumlahTransaksiCustomer.push_back(1);
            totalBelanjaCustomer.push_back(daftarTransaksi[i].totalBayar);
        }
    }

    for (size_t i = 0; i < namaCustomer.size(); i++) {
        for (size_t j = i + 1; j < namaCustomer.size(); j++) {
            if (totalBelanjaCustomer[j] > totalBelanjaCustomer[i]) {
                string tempNama = namaCustomer[i];
                int tempJumlah = jumlahTransaksiCustomer[i];
                int tempTotal = totalBelanjaCustomer[i];

                namaCustomer[i] = namaCustomer[j];
                jumlahTransaksiCustomer[i] = jumlahTransaksiCustomer[j];
                totalBelanjaCustomer[i] = totalBelanjaCustomer[j];

                namaCustomer[j] = tempNama;
                jumlahTransaksiCustomer[j] = tempJumlah;
                totalBelanjaCustomer[j] = tempTotal;
            }
        }
    }

    cout << left << setw(18) << "Customer"
         << setw(15) << "Jml Transaksi"
         << "Total Belanja\n";
    garis('-');

    for (size_t i = 0; i < namaCustomer.size(); i++) {
        cout << left << setw(18) << namaCustomer[i]
             << setw(15) << jumlahTransaksiCustomer[i]
             << formatRupiah(totalBelanjaCustomer[i])
             << "\n";
    }

    cout << "\n";
    jedaLayar();
}

void laporanPenggunaanPromo() {
    clearScreen();

    garis();
    cout << "              LAPORAN PENGGUNAAN PROMO\n";
    garis();

    if (daftarTransaksi.empty()) {
        cout << "Belum ada transaksi.\n\n";
        jedaLayar();
        return;
    }

    ManualList<string> kodePromoUnik;
    ManualList<int> jumlahPemakaian;
    ManualList<int> totalDiskon;

    for (size_t i = 0; i < daftarTransaksi.size(); i++) {
        if (daftarTransaksi[i].diskonPromo <= 0) continue;

        string kode = daftarTransaksi[i].kodePromo;
        bool ditemukan = false;

        for (size_t j = 0; j < kodePromoUnik.size(); j++) {
            if (kodePromoUnik[j] == kode) {
                jumlahPemakaian[j]++;
                totalDiskon[j] += daftarTransaksi[i].diskonPromo;
                ditemukan = true;
                break;
            }
        }

        if (!ditemukan) {
            kodePromoUnik.push_back(kode);
            jumlahPemakaian.push_back(1);
            totalDiskon.push_back(daftarTransaksi[i].diskonPromo);
        }
    }

    if (kodePromoUnik.empty()) {
        cout << "Belum ada transaksi yang memakai promo.\n\n";
        jedaLayar();
        return;
    }

    cout << left << setw(15) << "Kode Promo"
         << setw(15) << "Pemakaian"
         << "Total Diskon\n";
    garis('-');

    int grandDiskon = 0;

    for (size_t i = 0; i < kodePromoUnik.size(); i++) {
        cout << left << setw(15) << kodePromoUnik[i]
             << setw(15) << jumlahPemakaian[i]
             << formatRupiah(totalDiskon[i])
             << "\n";

        grandDiskon += totalDiskon[i];
    }

    garis('-');
    cout << "GRAND TOTAL DISKON : " << formatRupiah(grandDiskon) << "\n\n";

    jedaLayar();
}

void laporanPenjualanPerKategori() {
    clearScreen();

    garis();
    cout << "              LAPORAN PENJUALAN PER KATEGORI\n";
    garis();

    if (daftarTransaksi.empty()) {
        cout << "Belum ada transaksi.\n\n";
        jedaLayar();
        return;
    }

    ManualList<string> kategoriUnik;
    ManualList<int> jumlahTerjual;
    ManualList<int> totalPendapatan;

    for (size_t i = 0; i < daftarTransaksi.size(); i++) {
        for (size_t j = 0; j < daftarTransaksi[i].daftarItem.size(); j++) {
            string kategori = "Tidak Diketahui";
            int idxMenu = cariIndexMenuById(daftarTransaksi[i].daftarItem[j].idMenu);

            if (idxMenu != -1) {
                kategori = daftarMenu[idxMenu].kategori;
            }

            bool ditemukan = false;

            for (size_t k = 0; k < kategoriUnik.size(); k++) {
                if (kategoriUnik[k] == kategori) {
                    jumlahTerjual[k] += daftarTransaksi[i].daftarItem[j].jumlah;
                    totalPendapatan[k] += daftarTransaksi[i].daftarItem[j].subtotal;
                    ditemukan = true;
                    break;
                }
            }

            if (!ditemukan) {
                kategoriUnik.push_back(kategori);
                jumlahTerjual.push_back(daftarTransaksi[i].daftarItem[j].jumlah);
                totalPendapatan.push_back(daftarTransaksi[i].daftarItem[j].subtotal);
            }
        }
    }

    cout << left << setw(20) << "Kategori"
         << setw(15) << "Qty Terjual"
         << "Pendapatan\n";
    garis('-');

    for (size_t i = 0; i < kategoriUnik.size(); i++) {
        cout << left << setw(20) << kategoriUnik[i]
             << setw(15) << jumlahTerjual[i]
             << formatRupiah(totalPendapatan[i])
             << "\n";
    }

    cout << "\n";
    jedaLayar();
}

void cariTransaksiByCustomer() {
    clearScreen();

    garis();
    cout << "              CARI TRANSAKSI CUSTOMER\n";
    garis();

    string kata = toLowerStr(inputString("Masukkan username/nama customer : "));

    bool ketemu = false;
    int total = 0;

    for (size_t i = 0; i < daftarTransaksi.size(); i++) {
        string customerLower = toLowerStr(daftarTransaksi[i].customerUsername);

        if (!mengandungKata(customerLower, kata)) continue;

        ketemu = true;

        cout << "\nTransaksi #" << daftarTransaksi[i].idTransaksi
             << " | " << daftarTransaksi[i].tanggal
             << " " << daftarTransaksi[i].waktu
             << " | Customer: " << daftarTransaksi[i].customerUsername
             << " | Total: " << formatRupiah(daftarTransaksi[i].totalBayar)
             << "\n";

        total += daftarTransaksi[i].totalBayar;
    }

    if (!ketemu) {
        cout << "\nTransaksi customer tidak ditemukan.\n";
    } else {
        cout << "\nTotal transaksi yang cocok: " << formatRupiah(total) << "\n";
    }

    cout << "\n";
    jedaLayar();
}

void cetakRingkasanKasirLogin() {
    clearScreen();

    garis();
    cout << "              RINGKASAN KASIR HARI INI\n";
    garis();

    string tanggalHariIni = getTanggalSekarang();
    int jumlah = 0;
    int total = 0;
    int diskon = 0;

    for (size_t i = 0; i < daftarTransaksi.size(); i++) {
        string tgl = daftarTransaksi[i].tanggal;

        if (daftarTransaksi[i].kasirUsername == userLogin && tgl == tanggalHariIni) {
            jumlah++;
            total += daftarTransaksi[i].totalBayar;
            diskon += daftarTransaksi[i].diskonPromo;
        }
    }

    cout << "Kasir             : " << userLogin << "\n";
    cout << "Tanggal           : " << tanggalHariIni << "\n";
    cout << "Jumlah Transaksi  : " << jumlah << "\n";
    cout << "Total Penjualan   : " << formatRupiah(total) << "\n";
    cout << "Total Diskon      : " << formatRupiah(diskon) << "\n\n";

    jedaLayar();
}

void lihatTransaksiSayaSebagaiCustomer() {
    clearScreen();

    garis();
    cout << "              RIWAYAT TRANSAKSI SAYA\n";
    garis();

    bool ada = false;
    int total = 0;

    for (size_t i = 0; i < daftarTransaksi.size(); i++) {
        if (daftarTransaksi[i].customerUsername != userLogin) continue;

        ada = true;

        cout << "\nTransaksi #" << daftarTransaksi[i].idTransaksi
             << " | " << daftarTransaksi[i].tanggal
             << " " << daftarTransaksi[i].waktu
             << " | Total " << formatRupiah(daftarTransaksi[i].totalBayar)
             << "\n";

        for (size_t j = 0; j < daftarTransaksi[i].daftarItem.size(); j++) {
            cout << "  - " << daftarTransaksi[i].daftarItem[j].namaMenu
                 << " x" << daftarTransaksi[i].daftarItem[j].jumlah
                 << " = " << formatRupiah(daftarTransaksi[i].daftarItem[j].subtotal)
                 << "\n";
        }

        if (daftarTransaksi[i].diskonPromo > 0) {
            cout << "  Promo: " << daftarTransaksi[i].kodePromo
                 << " (-" << formatRupiah(daftarTransaksi[i].diskonPromo) << ")\n";
        }

        total += daftarTransaksi[i].totalBayar;
    }

    if (!ada) {
        cout << "Belum ada transaksi selesai untuk akun ini.\n";
    } else {
        cout << "\nTotal belanja selesai: " << formatRupiah(total) << "\n";
    }

    cout << "\n";
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
        cout << "5. Laporan Penjualan Per Kasir\n";
        cout << "6. Laporan Penjualan Per Customer\n";
        cout << "7. Laporan Penggunaan Promo\n";
        cout << "8. Laporan Penjualan Per Kategori\n";
        cout << "9. Cari Transaksi by Customer\n";
        cout << "0. Kembali\n";
        garis();

        pilihan = inputIntRange("Pilih Menu : ", 0, 9);

        switch (pilihan) {
            case 1: lihatSemuaTransaksi();          break;
            case 2: cariTransaksiByTanggal();       break;
            case 3: laporanMenuTerlaris();          break;
            case 4: laporanRekapHarian();           break;
            case 5: laporanPenjualanPerKasir();     break;
            case 6: laporanPenjualanPerCustomer();  break;
            case 7: laporanPenggunaanPromo();       break;
            case 8: laporanPenjualanPerKategori();  break;
            case 9: cariTransaksiByCustomer();      break;
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
    cout << "1. Dashboard Admin\n";
    cout << "2. Lihat Menu (Makanan & Minuman)\n";
    cout << "3. Kelola Menu (Tambah/Edit/Hapus)\n";
    cout << "4. Kelola Pegawai\n";
    cout << "5. Supplier, Stok & Pembelian\n";
    cout << "6. Kelola Promo\n";
    cout << "7. Laporan & Transaksi\n";
    cout << "8. Menu Stok Menipis\n";
    cout << "9. Logout\n";
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
    cout << "3. Batalkan Pesanan Pending\n";
    cout << "4. Riwayat Transaksi\n";
    cout << "5. Ringkasan Kasir Hari Ini\n";
    cout << "6. Logout\n";
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
    cout << "4. Riwayat Transaksi Selesai\n";
    cout << "5. Batalkan Pesanan Pending\n";
    cout << "6. Logout\n";
    cout << "0. Keluar\n";
    garis();
}

/* ============================================================
   HANDLER PER ROLE
   ============================================================ */

void jalankanMenuAdmin(int pilihan, bool &sudahLogin) {

    switch (pilihan) {

        case 1:
            dashboardAdmin();
            break;

        case 2:
            tampilMenuMakanan();
            tampilMenuMinuman();
            break;

        case 3:
            menuKelolaMenu();
            break;

        case 4:
            menuKelolaPegawai();
            break;

        case 5:
            menuKelolaStokPembelian();
            break;

        case 6:
            menuKelolaPromo();
            break;

        case 7:
            menuLaporanTransaksi();
            break;

        case 8:
            tampilMenuStokMenipis();
            break;

        case 9:
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
            batalkanPesananPending();
            break;

        case 4:
            lihatSemuaTransaksi();
            break;

        case 5:
            cetakRingkasanKasirLogin();
            break;

        case 6:
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
            lihatTransaksiSayaSebagaiCustomer();
            break;

        case 5:
            batalkanPesananSaya();
            break;

        case 6:
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
    loadSupplierDariFile();
    loadCatatanStokDariFile();
    loadPembelianDariFile();
    loadPromoDariFile();
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
                pilihan = inputIntRange("Pilih Menu : ", 0, 9);
                jalankanMenuAdmin(pilihan, sudahLogin);
            }
            else if (roleLogin == "Kasir") {
                tampilMenuKasir();
                pilihan = inputIntRange("Pilih Menu : ", 0, 6);
                jalankanMenuKasir(pilihan, sudahLogin);
            }
            else if (roleLogin == "Customer") {
                tampilMenuCustomer();
                pilihan = inputIntRange("Pilih Menu : ", 0, 6);
                jalankanMenuCustomer(pilihan, sudahLogin);
            }
        }

    } while (pilihan != 0);

    return 0;
}
