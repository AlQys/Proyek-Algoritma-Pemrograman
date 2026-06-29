#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <limits>

using namespace std;

const int MAX_USER = 20;
const int MAX_MENU = 50;
const int MAX_ITEM = 15;
const int MAX_TRANSAKSI = 50;
const int MAX_PESANAN = 50;
const int MAX_SUPPLIER = 20;
const int MAX_STOK = 80;
const int MAX_PEMBELIAN = 30;
const int MAX_PROMO = 10;

struct Tanggal {
    int hari, bulan, tahun;
    Tanggal() { hari = 1; bulan = 1; tahun = 2026; }
    void isiSekarang() {
        time_t t = time(0);
        tm *now = localtime(&t);
        hari = now->tm_mday; bulan = now->tm_mon + 1; tahun = now->tm_year + 1900;
    }
    void tampil() {
        cout << setfill('0') << setw(2) << hari << "-"
             << setw(2) << bulan << "-" << setw(4) << tahun << setfill(' ');
    }
};

struct Waktu {
    int jam, menit, detik;
    Waktu() { jam = 0; menit = 0; detik = 0; }
    void isiSekarang() {
        time_t t = time(0);
        tm *now = localtime(&t);
        jam = now->tm_hour; menit = now->tm_min; detik = now->tm_sec;
    }
    void tampil() {
        cout << setfill('0') << setw(2) << jam << ":"
             << setw(2) << menit << ":" << setw(2) << detik << setfill(' ');
    }
};

class User {
public:
    string username, password, role, namaLengkap;
    bool aktif;
    User() { aktif = true; }
};

class MenuItem {
public:
    int id, harga, stok;
    string tipe, kategori, nama;
    bool tersedia;
    MenuItem() { id = 0; harga = 0; stok = 0; tersedia = true; }
};

class ItemTransaksi {
public:
    int idMenu, harga, jumlah, subtotal;
    string namaMenu;
    ItemTransaksi() { idMenu = 0; harga = 0; jumlah = 0; subtotal = 0; }
};

class Transaksi {
public:
    int idTransaksi, jumlahItem, totalBayar, uangBayar, kembalian;
    Tanggal tanggal;
    Waktu waktu;
    string kasirUsername, customerUsername;
    ItemTransaksi daftarItem[MAX_ITEM];
    Transaksi() {
        idTransaksi = 0; jumlahItem = 0; totalBayar = 0;
        uangBayar = 0; kembalian = 0;
    }
};

class Pesanan {
public:
    int idPesanan, jumlahItem, totalHarga;
    Tanggal tanggal;
    Waktu waktu;
    string customerUsername, status;
    ItemTransaksi daftarItem[MAX_ITEM];
    Pesanan() { idPesanan = 0; jumlahItem = 0; totalHarga = 0; status = "Pending"; }
};

class Supplier {
public:
    int idSupplier;
    string namaSupplier, alamat;
    bool aktif;
    Supplier() { idSupplier = 0; aktif = true; }
};

class CatatanStok {
public:
    int idCatatan, stokMasuk, stokKeluar;
    Tanggal tanggal;
    Waktu waktu;
    string kodeBarang, namaBarang, keterangan;
    CatatanStok() { idCatatan = 0; stokMasuk = 0; stokKeluar = 0; }
};

class ItemPembelian {
public:
    string namaBarang;
    int hargaSatuan, jumlah, subtotal;
    ItemPembelian() { hargaSatuan = 0; jumlah = 0; subtotal = 0; }
};

class TransaksiPembelian {
public:
    int idPembelian, idSupplier, jumlahBarang, totalBeli;
    Tanggal tanggal;
    Waktu waktu;
    string namaSupplier, adminUsername;
    ItemPembelian daftarBarang[MAX_ITEM];
    TransaksiPembelian() {
        idPembelian = 0; idSupplier = 0; jumlahBarang = 0; totalBeli = 0;
    }
};

class Promo {
public:
    string kodePromo, namaPromo;
    int besarDiskonPersen;
    bool aktif;
    Promo() { besarDiskonPersen = 0; aktif = true; }
};

User daftarUser[MAX_USER];
MenuItem daftarMenu[MAX_MENU];
Transaksi daftarTransaksi[MAX_TRANSAKSI];
Pesanan daftarPesanan[MAX_PESANAN];
Supplier daftarSupplier[MAX_SUPPLIER];
CatatanStok daftarStok[MAX_STOK];
TransaksiPembelian daftarPembelian[MAX_PEMBELIAN];
Promo daftarPromo[MAX_PROMO];

int jumlahUser = 0, jumlahMenu = 0, jumlahTransaksi = 0, jumlahPesanan = 0;
int jumlahSupplier = 0, jumlahStok = 0, jumlahPembelian = 0, jumlahPromo = 0;
int idMenu = 1, idTransaksi = 1, idPesanan = 1, idSupplier = 1, idStok = 1, idPembelian = 1;

void garis() { cout << "==================================================\n"; }

int inputInt(string pesan) {
    int nilai;
    while (true) {
        cout << pesan;
        cin >> nilai;
        if (!cin.fail()) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return nilai;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Input harus angka.\n";
    }
}

string inputString(string pesan) {
    string teks;
    cout << pesan;
    getline(cin, teks);
    return teks;
}

int cariMenu(int id) {
    for (int i = 0; i < jumlahMenu; i++) if (daftarMenu[i].id == id) return i;
    return -1;
}

int cariSupplier(int id) {
    for (int i = 0; i < jumlahSupplier; i++) if (daftarSupplier[i].idSupplier == id) return i;
    return -1;
}

void dataAwal() {
    daftarUser[0].username = "admin";
    daftarUser[0].password = "admin";
    daftarUser[0].role = "Admin";
    daftarUser[0].namaLengkap = "Admin Kafe";
    jumlahUser = 1;

    daftarMenu[0].id = idMenu++;
    daftarMenu[0].tipe = "Minuman";
    daftarMenu[0].kategori = "Kopi";
    daftarMenu[0].nama = "Kopi Susu";
    daftarMenu[0].harga = 15000;
    daftarMenu[0].stok = 20;

    daftarMenu[1].id = idMenu++;
    daftarMenu[1].tipe = "Makanan";
    daftarMenu[1].kategori = "Snack";
    daftarMenu[1].nama = "Roti Bakar";
    daftarMenu[1].harga = 18000;
    daftarMenu[1].stok = 15;
    jumlahMenu = 2;

    daftarSupplier[0].idSupplier = idSupplier++;
    daftarSupplier[0].namaSupplier = "Supplier Utama";
    daftarSupplier[0].alamat = "Jakarta";
    jumlahSupplier = 1;

    daftarPromo[0].kodePromo = "HEMAT10";
    daftarPromo[0].namaPromo = "Diskon Pembukaan";
    daftarPromo[0].besarDiskonPersen = 10;
    jumlahPromo = 1;
}

void tampilMenu() {
    garis();
    cout << "DAFTAR MENU KOBESSAH KOPI\n";
    garis();
    cout << left << setw(5) << "ID" << setw(12) << "Tipe"
         << setw(18) << "Nama" << setw(10) << "Harga" << "Stok\n";
    for (int i = 0; i < jumlahMenu; i++) {
        cout << left << setw(5) << daftarMenu[i].id
             << setw(12) << daftarMenu[i].tipe
             << setw(18) << daftarMenu[i].nama
             << setw(10) << daftarMenu[i].harga
             << daftarMenu[i].stok << "\n";
    }
}

void tambahMenu() {
    if (jumlahMenu >= MAX_MENU) { cout << "Data menu penuh.\n"; return; }
    MenuItem m;
    m.id = idMenu++;
    m.tipe = inputString("Tipe       : ");
    m.kategori = inputString("Kategori   : ");
    m.nama = inputString("Nama menu  : ");
    m.harga = inputInt("Harga      : ");
    m.stok = inputInt("Stok       : ");
    daftarMenu[jumlahMenu++] = m;
    cout << "Menu ditambahkan.\n";
}

void tampilSupplier() {
    garis();
    cout << "DAFTAR SUPPLIER\n";
    garis();
    for (int i = 0; i < jumlahSupplier; i++) {
        cout << daftarSupplier[i].idSupplier << ". "
             << daftarSupplier[i].namaSupplier << " - "
             << daftarSupplier[i].alamat << "\n";
    }
}

void tambahSupplier() {
    if (jumlahSupplier >= MAX_SUPPLIER) { cout << "Data supplier penuh.\n"; return; }
    Supplier s;
    s.idSupplier = idSupplier++;
    s.namaSupplier = inputString("Nama supplier : ");
    s.alamat = inputString("Alamat        : ");
    daftarSupplier[jumlahSupplier++] = s;
    cout << "Supplier ditambahkan.\n";
}

void catatStok(string nama, int masuk, int keluar, string ket) {
    if (jumlahStok >= MAX_STOK) return;
    daftarStok[jumlahStok].idCatatan = idStok++;
    daftarStok[jumlahStok].tanggal.isiSekarang();
    daftarStok[jumlahStok].waktu.isiSekarang();
    daftarStok[jumlahStok].namaBarang = nama;
    daftarStok[jumlahStok].stokMasuk = masuk;
    daftarStok[jumlahStok].stokKeluar = keluar;
    daftarStok[jumlahStok].keterangan = ket;
    jumlahStok++;
}

void pembelianStok() {
    if (jumlahPembelian >= MAX_PEMBELIAN) { cout << "Data pembelian penuh.\n"; return; }
    tampilSupplier();
    int id = inputInt("ID supplier : ");
    int idx = cariSupplier(id);
    if (idx == -1) { cout << "Supplier tidak ditemukan.\n"; return; }

    TransaksiPembelian p;
    p.idPembelian = idPembelian++;
    p.tanggal.isiSekarang();
    p.waktu.isiSekarang();
    p.idSupplier = id;
    p.namaSupplier = daftarSupplier[idx].namaSupplier;
    p.adminUsername = "admin";
    p.jumlahBarang = inputInt("Jumlah jenis barang : ");
    if (p.jumlahBarang > MAX_ITEM) p.jumlahBarang = MAX_ITEM;

    for (int i = 0; i < p.jumlahBarang; i++) {
        cout << "Barang ke-" << i + 1 << "\n";
        p.daftarBarang[i].namaBarang = inputString("Nama   : ");
        p.daftarBarang[i].hargaSatuan = inputInt("Harga  : ");
        p.daftarBarang[i].jumlah = inputInt("Jumlah : ");
        p.daftarBarang[i].subtotal = p.daftarBarang[i].hargaSatuan * p.daftarBarang[i].jumlah;
        p.totalBeli += p.daftarBarang[i].subtotal;
        catatStok(p.daftarBarang[i].namaBarang, p.daftarBarang[i].jumlah, 0, "Pembelian");
    }
    daftarPembelian[jumlahPembelian++] = p;
    cout << "Total pembelian Rp" << p.totalBeli << "\n";
}

void buatPesanan() {
    if (jumlahPesanan >= MAX_PESANAN) { cout << "Data pesanan penuh.\n"; return; }
    Pesanan p;
    p.idPesanan = idPesanan++;
    p.tanggal.isiSekarang();
    p.waktu.isiSekarang();
    p.customerUsername = inputString("Nama customer : ");
    tampilMenu();
    int banyak = inputInt("Jumlah item : ");
    if (banyak > MAX_ITEM) banyak = MAX_ITEM;

    for (int i = 0; i < banyak; i++) {
        int kode = inputInt("ID menu : ");
        int idx = cariMenu(kode);
        if (idx == -1) { cout << "Menu tidak ada.\n"; continue; }
        int jumlah = inputInt("Jumlah  : ");
        if (jumlah > daftarMenu[idx].stok) { cout << "Stok kurang.\n"; continue; }

        p.daftarItem[p.jumlahItem].idMenu = daftarMenu[idx].id;
        p.daftarItem[p.jumlahItem].namaMenu = daftarMenu[idx].nama;
        p.daftarItem[p.jumlahItem].harga = daftarMenu[idx].harga;
        p.daftarItem[p.jumlahItem].jumlah = jumlah;
        p.daftarItem[p.jumlahItem].subtotal = daftarMenu[idx].harga * jumlah;
        p.totalHarga += p.daftarItem[p.jumlahItem].subtotal;
        p.jumlahItem++;
    }
    daftarPesanan[jumlahPesanan++] = p;
    cout << "Pesanan dibuat. Total Rp" << p.totalHarga << "\n";
}

void tampilPesanan() {
    garis();
    cout << "PESANAN PENDING\n";
    garis();
    for (int i = 0; i < jumlahPesanan; i++) {
        if (daftarPesanan[i].status == "Pending") {
            cout << daftarPesanan[i].idPesanan << ". "
                 << daftarPesanan[i].customerUsername
                 << " | Rp" << daftarPesanan[i].totalHarga << "\n";
        }
    }
}

void bayarPesanan() {
    if (jumlahTransaksi >= MAX_TRANSAKSI) { cout << "Data transaksi penuh.\n"; return; }
    tampilPesanan();
    int id = inputInt("ID pesanan : ");
    int idxPesanan = -1;
    for (int i = 0; i < jumlahPesanan; i++) if (daftarPesanan[i].idPesanan == id) idxPesanan = i;
    if (idxPesanan == -1 || daftarPesanan[idxPesanan].status != "Pending") {
        cout << "Pesanan tidak ditemukan.\n";
        return;
    }

    Transaksi t;
    t.idTransaksi = idTransaksi++;
    t.tanggal.isiSekarang();
    t.waktu.isiSekarang();
    t.kasirUsername = "kasir";
    t.customerUsername = daftarPesanan[idxPesanan].customerUsername;
    t.jumlahItem = daftarPesanan[idxPesanan].jumlahItem;
    t.totalBayar = daftarPesanan[idxPesanan].totalHarga;

    for (int i = 0; i < t.jumlahItem; i++) {
        t.daftarItem[i] = daftarPesanan[idxPesanan].daftarItem[i];
        int idxMenu = cariMenu(t.daftarItem[i].idMenu);
        if (idxMenu != -1) {
            daftarMenu[idxMenu].stok -= t.daftarItem[i].jumlah;
            catatStok(t.daftarItem[i].namaMenu, 0, t.daftarItem[i].jumlah, "Penjualan");
        }
    }
    t.uangBayar = inputInt("Uang bayar : ");
    t.kembalian = t.uangBayar - t.totalBayar;
    daftarPesanan[idxPesanan].status = "Selesai";
    daftarTransaksi[jumlahTransaksi++] = t;

    garis();
    cout << "STRUK KOBESSAH KOPI\n";
    cout << "Tanggal : "; t.tanggal.tampil(); cout << "\n";
    cout << "Waktu   : "; t.waktu.tampil(); cout << "\n";
    cout << "Total   : Rp" << t.totalBayar << "\n";
    cout << "Kembali : Rp" << t.kembalian << "\n";
}

void tampilPromo() {
    garis();
    cout << "PROMO AKTIF\n";
    garis();
    for (int i = 0; i < jumlahPromo; i++) {
        cout << daftarPromo[i].kodePromo << " - "
             << daftarPromo[i].namaPromo << " "
             << daftarPromo[i].besarDiskonPersen << "%\n";
    }
}

void tambahPromo() {
    if (jumlahPromo >= MAX_PROMO) {
        cout << "Data promo penuh.\n";
        return;
    }

    Promo p;
    p.kodePromo = inputString("Kode promo : ");
    p.namaPromo = inputString("Nama promo : ");
    p.besarDiskonPersen = inputInt("Diskon (%) : ");
    p.aktif = true;
    daftarPromo[jumlahPromo++] = p;
    cout << "Promo ditambahkan.\n";
}

void menuPromo() {
    int pilih;
    do {
        garis();
        cout << "MENU PROMO\n";
        garis();
        cout << "1. Lihat Promo\n";
        cout << "2. Tambah Promo\n";
        cout << "0. Kembali\n";
        pilih = inputInt("Pilih : ");

        if (pilih == 1) tampilPromo();
        else if (pilih == 2) tambahPromo();
    } while (pilih != 0);
}

void riwayatStok() {
    garis();
    cout << "RIWAYAT STOK\n";
    garis();

    if (jumlahStok == 0) {
        cout << "Belum ada catatan stok.\n";
        return;
    }

    cout << left << setw(5) << "ID"
         << setw(18) << "Barang"
         << setw(8) << "Masuk"
         << setw(8) << "Keluar"
         << "Keterangan\n";

    for (int i = 0; i < jumlahStok; i++) {
        cout << left << setw(5) << daftarStok[i].idCatatan
             << setw(18) << daftarStok[i].namaBarang
             << setw(8) << daftarStok[i].stokMasuk
             << setw(8) << daftarStok[i].stokKeluar
             << daftarStok[i].keterangan << "\n";
    }
}

void riwayatPembelian() {
    garis();
    cout << "RIWAYAT PEMBELIAN\n";
    garis();

    if (jumlahPembelian == 0) {
        cout << "Belum ada transaksi pembelian.\n";
        return;
    }

    for (int i = 0; i < jumlahPembelian; i++) {
        cout << "PB-" << daftarPembelian[i].idPembelian << " | "
             << daftarPembelian[i].namaSupplier << " | Rp"
             << daftarPembelian[i].totalBeli << "\n";

        for (int j = 0; j < daftarPembelian[i].jumlahBarang; j++) {
            cout << "  - " << daftarPembelian[i].daftarBarang[j].namaBarang
                 << " x" << daftarPembelian[i].daftarBarang[j].jumlah
                 << " = Rp" << daftarPembelian[i].daftarBarang[j].subtotal << "\n";
        }
    }
}

void laporan() {
    int total = 0;
    garis();
    cout << "LAPORAN PENJUALAN\n";
    garis();
    for (int i = 0; i < jumlahTransaksi; i++) {
        cout << "TRX-" << daftarTransaksi[i].idTransaksi << " | ";
        daftarTransaksi[i].tanggal.tampil();
        cout << " | Rp" << daftarTransaksi[i].totalBayar << "\n";
        total += daftarTransaksi[i].totalBayar;
    }
    cout << "Grand total Rp" << total << "\n";
}

void menuUtama() {
    int pilih;
    do {
        garis();
        cout << "KOBESSAH KOPI - PROGRAM AWAL\n";
        garis();
        cout << "1. Lihat Menu\n2. Tambah Menu\n3. Buat Pesanan\n";
        cout << "4. Bayar Pesanan\n5. Supplier\n6. Pembelian Stok\n";
        cout << "7. Promo\n8. Riwayat Stok\n9. Riwayat Pembelian\n";
        cout << "10. Laporan\n0. Keluar\n";
        pilih = inputInt("Pilih : ");

        if (pilih == 1) tampilMenu();
        else if (pilih == 2) tambahMenu();
        else if (pilih == 3) buatPesanan();
        else if (pilih == 4) bayarPesanan();
        else if (pilih == 5) {
            tampilSupplier();
            if (inputInt("Tambah supplier? 1=Ya : ") == 1) tambahSupplier();
        }
        else if (pilih == 6) pembelianStok();
        else if (pilih == 7) menuPromo();
        else if (pilih == 8) riwayatStok();
        else if (pilih == 9) riwayatPembelian();
        else if (pilih == 10) laporan();
    } while (pilih != 0);
}

int main() {
    dataAwal();
    menuUtama();
    cout << "Program selesai.\n";
    return 0;
}
