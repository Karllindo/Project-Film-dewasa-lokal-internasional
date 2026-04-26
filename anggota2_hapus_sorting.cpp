/*
============================================================
  APLIKASI DATA FILM FAVORIT
  ANGGOTA 2 : Hapus Film, Sorting
  Struktur  : Doubly Linked List, Class
============================================================
*/

#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
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

    // Helper: kumpulkan ke array, sort, tulis kembali ke node
    void sortBy(bool (*cmp)(const Film&, const Film&)) {
        if (!head || !head->next) return;

        Film arr[200];
        int n = 0;
        NodeDoubly* cur = head;
        while (cur) { arr[n++] = cur->data; cur = cur->next; }

        // Bubble sort
        for (int i = 0; i < n - 1; i++)
            for (int j = 0; j < n - i - 1; j++)
                if (cmp(arr[j + 1], arr[j]))
                    swap(arr[j], arr[j + 1]);

        cur = head;
        for (int i = 0; i < n; i++) {
            cur->data = arr[i];
            cur = cur->next;
        }
    }

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

    // Tambah film (digunakan untuk inisialisasi data awal)
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
    }

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
    // HAPUS FILM berdasarkan ID
    // (Keunggulan Doubly: hapus node lebih mudah karena
    //  langsung bisa akses prev tanpa traversal ulang)
    // ----------------------------------------------------------
    void hapusFilm(int id) {
        NodeDoubly* cur = head;
        while (cur) {
            if (cur->data.id == id) {
                string judul = cur->data.judul;

                if (cur->prev) cur->prev->next = cur->next;
                else           head = cur->next;   // node pertama

                if (cur->next) cur->next->prev = cur->prev;
                else           tail = cur->prev;   // node terakhir

                delete cur;
                jumlah--;
                cout << "\n  [OK] Film \"" << judul << "\" (ID=" << id << ") berhasil dihapus!\n";
                return;
            }
            cur = cur->next;
        }
        cout << "\n  [!] Film dengan ID " << id << " tidak ditemukan.\n";
    }

    // ----------------------------------------------------------
    // SORTING — 6 pilihan urutan
    // ----------------------------------------------------------
    void sortJudulAZ() {
        sortBy([](const Film& a, const Film& b){ return a.judul < b.judul; });
        cout << "\n  [OK] Film diurutkan: Judul A-Z.\n";
    }
    void sortJudulZA() {
        sortBy([](const Film& a, const Film& b){ return a.judul > b.judul; });
        cout << "\n  [OK] Film diurutkan: Judul Z-A.\n";
    }
    void sortRatingTertinggi() {
        sortBy([](const Film& a, const Film& b){ return a.rating > b.rating; });
        cout << "\n  [OK] Film diurutkan: Rating Tertinggi.\n";
    }
    void sortRatingTerendah() {
        sortBy([](const Film& a, const Film& b){ return a.rating < b.rating; });
        cout << "\n  [OK] Film diurutkan: Rating Terendah.\n";
    }
    void sortTahunTerbaru() {
        sortBy([](const Film& a, const Film& b){ return a.tahun > b.tahun; });
        cout << "\n  [OK] Film diurutkan: Tahun Terbaru.\n";
    }
    void sortTahunTerlama() {
        sortBy([](const Film& a, const Film& b){ return a.tahun < b.tahun; });
        cout << "\n  [OK] Film diurutkan: Tahun Terlama.\n";
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

// ============================================================
// SUB-MENU SORTING
// ============================================================
void menuSorting(DaftarFilm& daftar) {
    cout << "\n  ===== MENU SORTING =====\n";
    cout << "  1. Judul A-Z\n";
    cout << "  2. Judul Z-A\n";
    cout << "  3. Rating Tertinggi\n";
    cout << "  4. Rating Terendah\n";
    cout << "  5. Tahun Terbaru\n";
    cout << "  6. Tahun Terlama\n";
    cout << "  Pilih sorting: ";
    int pilih; cin >> pilih;
    switch (pilih) {
        case 1: daftar.sortJudulAZ();         break;
        case 2: daftar.sortJudulZA();         break;
        case 3: daftar.sortRatingTertinggi(); break;
        case 4: daftar.sortRatingTerendah();  break;
        case 5: daftar.sortTahunTerbaru();    break;
        case 6: daftar.sortTahunTerlama();    break;
        default: cout << "\n  Pilihan sorting tidak valid.\n";
    }
}

// ============================================================
// MENU UTAMA
// ============================================================
void cetakMenu() {
    cout << "\n";
    cout << "+==========================================+\n";
    cout << "|   APLIKASI DATA FILM FAVORIT             |\n";
    cout << "|   Anggota 2: Hapus Film & Sorting        |\n";
    cout << "+==========================================+\n";
    cout << "|  1. Tampilkan Semua Film                 |\n";
    cout << "|  2. Hapus Film (berdasarkan ID)          |\n";
    cout << "|  3. Sorting Film                         |\n";
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

    cout << "\n  Data awal berhasil dimuat (" << daftar.getJumlah() << " film).\n";

    int pilih;
    do {
        cetakMenu();
        cin >> pilih;

        switch (pilih) {
            case 1:
                daftar.tampilkanSemuaFilm();
                break;
            case 2: {
                daftar.tampilkanSemuaFilm();
                if (daftar.getJumlah() == 0) break;
                int id = inputInt("  Masukkan ID film yang akan dihapus: ");
                daftar.hapusFilm(id);
                break;
            }
            case 3:
                menuSorting(daftar);
                daftar.tampilkanSemuaFilm();
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
