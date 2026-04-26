/*
============================================================
  APLIKASI DATA FILM FAVORIT
  ANGGOTA 1 : Tambah Film, Tampilkan Film
  Struktur  : Doubly Linked List, Class
============================================================
*/

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

// ============================================================
// CLASS Film
// ============================================================
class Film {
public:
    int    id;
    string judul;
    string sutradara;
    string genre;
    int    tahun;
    float  rating;

    Film() : id(0), tahun(0), rating(0.0f) {}

    Film(int id, string judul, string sutradara,
         string genre, int tahun, float rating)
        : id(id), judul(judul), sutradara(sutradara),
          genre(genre), tahun(tahun), rating(rating) {}

    void tampilkanHeader() const {
        cout << left
             << setw(4)  << "No"
             << setw(5)  << "ID"
             << setw(25) << "Judul"
             << setw(20) << "Sutradara"
             << setw(12) << "Genre"
             << setw(7)  << "Tahun"
             << setw(7)  << "Rating"
             << endl;
        cout << string(80, '-') << endl;
    }

    void tampilkanBaris(int no) const {
        cout << left
             << setw(4)  << no
             << setw(5)  << id
             << setw(25) << judul
             << setw(20) << sutradara
             << setw(12) << genre
             << setw(7)  << tahun
             << fixed << setprecision(1) << setw(7) << rating
             << endl;
    }
};

// ============================================================
// DOUBLY LINKED LIST — Daftar Film
// ============================================================
struct NodeDoubly {
    Film        data;
    NodeDoubly* prev;
    NodeDoubly* next;
    NodeDoubly(Film f) : data(f), prev(nullptr), next(nullptr) {}
};

class DaftarFilm {
private:
    NodeDoubly* head;
    NodeDoubly* tail;
    int         jumlah;
    int         nextId;

public:
    DaftarFilm() : head(nullptr), tail(nullptr), jumlah(0), nextId(1) {}

    ~DaftarFilm() {
        NodeDoubly* cur = head;
        while (cur) {
            NodeDoubly* tmp = cur;
            cur = cur->next;
            delete tmp;
        }
    }

    // ----------------------------------------------------------
    // Tambah Film (sisip di akhir / tail)
    // ----------------------------------------------------------
    void tambahFilm(string judul, string sutradara,
                    string genre, int tahun, float rating) {
        Film f(nextId++, judul, sutradara, genre, tahun, rating);
        NodeDoubly* node = new NodeDoubly(f);

        if (!head) {
            head = tail = node;
        } else {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }
        jumlah++;
        cout << "\n  [OK] Film \"" << judul << "\" berhasil ditambahkan! (ID=" << f.id << ")\n";
    }

    // ----------------------------------------------------------
    // Tampilkan Semua Film (traversal maju head -> tail)
    // ----------------------------------------------------------
    void tampilkanSemuaFilm() const {
        if (!head) {
            cout << "\n  Daftar film kosong.\n";
            return;
        }
        cout << "\n====== DAFTAR FILM FAVORIT (" << jumlah << " film) ======\n";
        Film dummy;
        dummy.tampilkanHeader();

        NodeDoubly* cur = head;
        int no = 1;
        while (cur) {
            cur->data.tampilkanBaris(no++);
            cur = cur->next;
        }
        cout << string(80, '-') << "\n";
    }

    // ----------------------------------------------------------
    // Tampilkan Film Urutan Terbalik (traversal mundur tail -> head)
    // (Manfaat utama Doubly Linked List: bisa traversal dua arah)
    // ----------------------------------------------------------
    void tampilkanTerbalik() const {
        if (!tail) {
            cout << "\n  Daftar film kosong.\n";
            return;
        }
        cout << "\n====== DAFTAR FILM (URUTAN TERBALIK) ======\n";
        Film dummy;
        dummy.tampilkanHeader();

        NodeDoubly* cur = tail;
        int no = jumlah;
        while (cur) {
            cur->data.tampilkanBaris(no--);
            cur = cur->prev;
        }
        cout << string(80, '-') << "\n";
    }

    int getJumlah() const { return jumlah; }
};

// ============================================================
// HELPER — Input dengan validasi
// ============================================================
int inputInt(const string& prompt) {
    int val;
    cout << prompt;
    while (!(cin >> val)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "  Input tidak valid. Masukkan angka: ";
    }
    return val;
}

float inputFloat(const string& prompt) {
    float val;
    cout << prompt;
    while (!(cin >> val)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "  Input tidak valid. Masukkan angka desimal: ";
    }
    return val;
}

string inputString(const string& prompt) {
    string val;
    cout << prompt;
    cin.ignore();
    getline(cin, val);
    return val;
}

// ============================================================
// MENU
// ============================================================
void cetakMenu() {
    cout << "\n";
    cout << "+==========================================+\n";
    cout << "|   APLIKASI DATA FILM FAVORIT             |\n";
    cout << "|   Anggota 1: Tambah & Tampilkan Film     |\n";
    cout << "+==========================================+\n";
    cout << "|  1. Tambah Film                          |\n";
    cout << "|  2. Tampilkan Semua Film (Maju)          |\n";
    cout << "|  3. Tampilkan Film (Urutan Terbalik)     |\n";
    cout << "|  0. Keluar                               |\n";
    cout << "+==========================================+\n";
    cout << "  Pilih menu: ";
}

// ============================================================
// MAIN
// ============================================================
int main() {
    DaftarFilm daftar;

    // Data awal
    daftar.tambahFilm("Parasite",       "Bong Joon-ho",      "Drama",     2019, 8.6f);
    daftar.tambahFilm("Interstellar",   "Christopher Nolan", "Sci-Fi",    2014, 8.7f);
    daftar.tambahFilm("The Dark Knight","Christopher Nolan", "Action",    2008, 9.0f);
    daftar.tambahFilm("Spirited Away",  "Hayao Miyazaki",    "Animation", 2001, 8.6f);
    daftar.tambahFilm("Dune: Part Two", "Denis Villeneuve",  "Sci-Fi",    2024, 8.5f);

    int pilih;
    do {
        cetakMenu();
        cin >> pilih;

        switch (pilih) {
            case 1: {
                cout << "\n  ===== TAMBAH FILM =====\n";
                string judul     = inputString("  Judul       : ");
                string sutradara = inputString("  Sutradara   : ");
                string genre     = inputString("  Genre       : ");
                int    tahun     = inputInt   ("  Tahun       : ");
                float  rating    = inputFloat ("  Rating(1-10): ");
                if (rating < 1)  rating = 1;
                if (rating > 10) rating = 10;
                daftar.tambahFilm(judul, sutradara, genre, tahun, rating);
                break;
            }
            case 2:
                daftar.tampilkanSemuaFilm();
                break;
            case 3:
                daftar.tampilkanTerbalik();
                break;
            case 0:
                cout << "\n  Terima kasih! Program selesai.\n\n";
                break;
            default:
                cout << "\n  [!] Pilihan tidak valid.\n";
        }
    } while (pilih != 0);

    return 0;
}
