#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class User {
public:
    string username;
    string password;
};

class Menu {
public:
    string kodeMi, kodeMa;
    string namaMi, namaMa;
    int hargaMi, hargaMa;
};

vector<User> daftarUser;
vector<Menu> daftarMenu;

void clearScreen() {
    system("cls");
}

void inisialisasiMenuMakan() {

    Menu ma;
    ma.kodeMa = "Main Dish";
    ma.namaMa = "Nasi Telur";
    ma.hargaMa = 11000;
    daftarMenu.push_back(ma);

	ma.namaMa = "Nasi Telur + Tempe";
	ma.hargaMa = 13000;
	daftarMenu.push_back(ma);
	
	ma.namaMa = "Nasi Telur + Jamur";
	ma.hargaMa = 13000;
	daftarMenu.push_back(ma);
	
	ma.namaMa = "Nasi Telur + Nugget";
	ma.hargaMa = 14000;
	daftarMenu.push_back(ma);
	
	
	ma.namaMa = "Nasi Ayam";
	ma.hargaMa = 17000;
	daftarMenu.push_back(ma);
	
	ma.namaMa = "Nasi Ayam + Jamur";
	ma.hargaMa = 18000;
	daftarMenu.push_back(ma);
	
	ma.namaMa = "Nasi Ayam + Nugget";
	ma.hargaMa = 19000;
	daftarMenu.push_back(ma);
	
	ma.kodeMa = "Nasgor";
	ma.namaMa = "Nasi Goreng Kobe";
	ma.hargaMa = 12000;
	daftarMenu.push_back(ma);
	
	ma.kodeMa = "Nasgor";
	ma.namaMa = "Nasi Goreng Oriental";
	ma.hargaMa = 15000;
	daftarMenu.push_back(ma);
	
	ma.kodeMa = "Nasgor";
	ma.namaMa = "Nasi Goreng Jawa";
	ma.hargaMa = 15000;
	daftarMenu.push_back(ma);
	
	ma.kodeMa = "Nasgor";
	ma.namaMa = "Nasi Goreng Arab";
	ma.hargaMa = 15000;
	daftarMenu.push_back(ma);
	
	ma.kodeMa = "Nasgor";
	ma.namaMa = "Magelangan";
	ma.hargaMa = 17000;
	daftarMenu.push_back(ma);
	
	ma.kodeMa = "Mie";
	ma.namaMa = "Mie Goreng Telur";
	ma.hargaMa = 10000;
	daftarMenu.push_back(ma);
	
	ma.kodeMa = "Mie";
	ma.namaMa = "Mie Rebus Telur";
	ma.hargaMa = 10000;
	daftarMenu.push_back(ma);
	
	ma.kodeMa = "Mie";
	ma.namaMa = "Mie Goreng Telur + Sosis";
	ma.hargaMa = 12000;
	daftarMenu.push_back(ma);
	
	ma.kodeMa = "Mie";
	ma.namaMa = "Mie Rebus Telur + Sosis";
	ma.hargaMa = 12000;
	daftarMenu.push_back(ma);
	
	ma.kodeMa = "Mie";
	ma.namaMa = "Mie Goreng Katsu";
	ma.hargaMa = 15000;
	daftarMenu.push_back(ma);
	
	ma.kodeMa = "Mie";
	ma.namaMa = "Mie Goreng Spesial";
	ma.hargaMa = 15000;
	daftarMenu.push_back(ma);
	
	ma.kodeMa = "Mie";
	ma.namaMa = "Mie Rebus Spesial";
	ma.hargaMa = 15000;
	daftarMenu.push_back(ma);
	
	ma.kodeMa = "Mie";
	ma.namaMa = "Mie Goreng Telur";
	ma.hargaMa = 10000;
	daftarMenu.push_back(ma);
}

void inisialisasiMenuMinum() {

    Menu mi;

    mi.kodeMi = "Minuman1";
    mi.namaMi = "Es Kopi Susu";
    mi.hargaMi = 18000;
    daftarMenu.push_back(mi);

    mi.kodeMi = "Minuman2";
    mi.namaMi = "Cappuccino";
    mi.hargaMi = 22000;
    daftarMenu.push_back(mi);

    mi.kodeMi = "Minuman3";
    mi.namaMi = "Americano";
    mi.hargaMi = 20000;
    daftarMenu.push_back(mi);
}

void simpanUser(string username, string password) {

    ofstream file("user.txt", ios::app);

    file << username << " " << password << endl;

    file.close();
}

void loadUser() {

    ifstream file("user.txt");

    User u;

    while (file >> u.username >> u.password) {

        daftarUser.push_back(u);
    }

    file.close();
}

bool cekLogin(string username, string password) {

    for (int i = 0; i < daftarUser.size(); i++) {

        if (daftarUser[i].username == username &&
            daftarUser[i].password == password) {

            return true;
        }
    }

    return false;
}

void registrasi() {

    User user;

    clearScreen();

    cout << "=== REGISTRASI AKUN ===\n";

    cout << "Masukkan Username : ";
    cin >> user.username;

    cout << "Masukkan Password : ";
    cin >> user.password;

    simpanUser(user.username, user.password);

    daftarUser.push_back(user);

    cout << "\nRegistrasi berhasil!\n";

    system("pause");
}

bool login() {

    string username;
    string password;

    clearScreen();

    cout << "=== LOGIN SISTEM ===\n";

    cout << "Username : ";
    cin >> username;

    cout << "Password : ";
    cin >> password;

    bool status = cekLogin(username, password);

    if (status == true) {

        cout << "\nLogin berhasil!\n";

        system("pause");

        return true;
    }
    else {

        cout << "\nUsername atau password salah!\n";

        system("pause");

        return false;
    }
}

void logout() {

    clearScreen();

    cout << "=== LOGOUT ===\n";
    cout << "Anda berhasil logout dari sistem.\n";

    system("pause");
}

void tampilMenuMiCafe() {

    clearScreen();

    cout << "=== MENU KOBESSAH KOPI ===\n\n";
    cout << "DRINK\n\n";

    for (int i = 0; i < daftarMenu.size(); i++) {

        if (daftarMenu[i].kodeMi != "") {

            cout << daftarMenu[i].kodeMi << " | ";
            cout << daftarMenu[i].namaMi << " | Rp.";
            cout << daftarMenu[i].hargaMi << endl;
        }
    }

    cout << "\n";

    system("pause");
}

void tampilMenuMaCafe() {

    clearScreen();

    cout << "=== MENU KOBESSAH KOPI ===\n\n";
    cout << "FOOD\n";

    string kategoriSebelumnya = "";

    for (int i = 0; i < daftarMenu.size(); i++) {

        if (daftarMenu[i].kodeMa != "") {

            if (daftarMenu[i].kodeMa != kategoriSebelumnya) {

                kategoriSebelumnya = daftarMenu[i].kodeMa;

                cout << "\n" << kategoriSebelumnya << "\n\n";
            }

            cout << "- "
                 << left << setw(30)
                 << daftarMenu[i].namaMa
                 << " | Rp." << daftarMenu[i].hargaMa
                 << endl;
        }
    }

    cout << "\n";

    system("pause");
}

void tampilMenuUtama() {

    cout << "===============================\n";
    cout << "      SISTEM INFORMASI KAFE    \n";
    cout << "===============================\n";
    cout << "1. Registrasi Akun\n";
    cout << "2. Login\n";
    cout << "3. Lihat Daftar Menu Minuman\n";
    cout << "4. Lihat Daftar Menu Makanan\n";
    cout << "5. Input Pesanan\n";
	cout << "6. Lihat Pesanan\n";
	cout << "7. Total Pembayaran\n";
	cout << "8. Logout\n";
	cout << "0. Keluar\n";
    cout << "===============================\n";
    cout << "Pilih Menu : ";
}

class Pesanan {
public:
    string namaMenu;
    int harga;
    int jumlah;
    int total;
};

vector<Pesanan> daftarPesanan;

void inputPesanan() {

    clearScreen();

    Pesanan p;

    cout << "=== INPUT PESANAN ===\n\n";

    cout << "Masukkan Nama Menu : ";
    cin.ignore();
    getline(cin, p.namaMenu);

    cout << "Masukkan Harga : ";
    cin >> p.harga;

    cout << "Masukkan Jumlah : ";
    cin >> p.jumlah;

    p.total = p.harga * p.jumlah;

    daftarPesanan.push_back(p);

    cout << "\nPesanan berhasil ditambahkan!\n";

    system("pause");
}

void tampilPesanan() {

    clearScreen();

    int grandTotal = 0;

    cout << "=== DAFTAR PESANAN ===\n\n";

    for (int i = 0; i < daftarPesanan.size(); i++) {

        cout << "Pesanan Ke-" << i + 1 << endl;
        cout << "Menu   : " << daftarPesanan[i].namaMenu << endl;
        cout << "Harga  : Rp." << daftarPesanan[i].harga << endl;
        cout << "Jumlah : " << daftarPesanan[i].jumlah << endl;
        cout << "Total  : Rp." << daftarPesanan[i].total << endl;
        cout << "========================\n";

        grandTotal += daftarPesanan[i].total;
    }

    cout << "\nGrand Total : Rp." << grandTotal << endl;

    system("pause");
}

int hitungTotalPembayaran() {

    int grandTotal = 0;

    for (int i = 0; i < daftarPesanan.size(); i++) {

        grandTotal += daftarPesanan[i].total;
    }

    return grandTotal;
}

void tampilTotalPembayaran() {

    clearScreen();

    int totalBayar = hitungTotalPembayaran();

    cout << "=== TOTAL PEMBAYARAN ===\n\n";

    if (daftarPesanan.size() == 0) {

        cout << "Belum ada pesanan!\n";
    }
    else {

        for (int i = 0; i < daftarPesanan.size(); i++) {

            cout << daftarPesanan[i].namaMenu;
            cout << " x " << daftarPesanan[i].jumlah;
            cout << " = Rp." << daftarPesanan[i].total << endl;
        }

        cout << "\n=========================\n";
        cout << "TOTAL BAYAR : Rp." << totalBayar << endl;
    }

    cout << endl;

    system("pause");
}


int main() {

    int pilihan;

    bool sudahLogin = false;

    inisialisasiMenuMakan();
    inisialisasiMenuMinum();

    loadUser();

    do {

        clearScreen();

        tampilMenuUtama();

        cin >> pilihan;

        switch (pilihan) {

        case 1:
            registrasi();
            break;

        case 2:
            sudahLogin = login();
            break;

        case 3:

            if (sudahLogin == true) {

                tampilMenuMiCafe();
            }
            else {

                cout << "\nSilakan login terlebih dahulu!\n";

                system("pause");
            }

            break;

        case 4:

            if (sudahLogin == true) {

                tampilMenuMaCafe();
            }
            else {

                cout << "\nSilakan login terlebih dahulu!\n";

                system("pause");
            }

            break;
        
        case 5:
		
		    if (sudahLogin == true) {
		
		        inputPesanan();
		    }
		    else {
		
		        cout << "\nSilakan login terlebih dahulu!\n";
		
		        system("pause");
		    }
		
		    break;
		
		case 6:
		
		    if (sudahLogin == true) {
		
		        tampilPesanan();
		    }
		    else {
		
		        cout << "\nSilakan login terlebih dahulu!\n";
		
		        system("pause");
		    }
		
		    break;
		
		case 7:

		    if (sudahLogin == true) {
		
		        tampilTotalPembayaran();
		    }
		    else {
		
		        cout << "\nSilakan login terlebih dahulu!\n";
		
		        system("pause");
		    }
		
		    break;

        case 8:

            if (sudahLogin == true) {

                logout();

                sudahLogin = false;
            }
            else {

                cout << "\nAnda belum login!\n";

                system("pause");
            }

            break;

        case 0:

            clearScreen();

            cout << "Terima kasih telah menggunakan sistem!\n";

            break;

        default:

            cout << "\nPilihan tidak tersedia!\n";

            system("pause");
        }

    } while (pilihan != 0);

    return 0;
}
