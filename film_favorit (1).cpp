/*
============================================================
  APLIKASI DATA FILM FAVORIT
  Menggunakan: Singly Linked List, Doubly Linked List, Class
============================================================
  Anggota 1 : Tambah Film, Tampilkan Film
  Anggota 2 : Hapus Film, Sorting
  Anggota 3 : Pencarian, Update Film
============================================================
*/

#include <iostream>
#include <string>
#include <algorithm>
#include <iomanip>
using namespace std;

// ============================================================
// CLASS Film  (menyimpan data satu film)
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
             << setw(25) << "Judul"
             << setw(20) << "Sutradara"
             << setw(12) << "Genre"
             << setw(7)  << "Tahun"
             << setw(7)  << "Rating"
             << endl;
        cout << string(75, '-') << endl;
    }

    void tampilkanBaris(int no) const {
        cout << left
             << setw(4)  << no
             << setw(25) << judul
             << setw(20) << sutradara
             << setw(12) << genre
             << setw(7)  << tahun
             << fixed << setprecision(1) << setw(7) << rating
             << endl;
    }
};

// ============================================================
// SINGLY LINKED LIST  — Riwayat Pencarian (Anggota 3)
// Setiap kali user mencari film, kata kunci disimpan di sini
// ============================================================
struct NodeSingly {
    string keyword;
    NodeSingly* next;
    NodeSingly(string k) : keyword(k), next(nullptr) {}
};

class RiwayatPencarian {
private:
    NodeSingly* head;
    int jumlah;

public:
    RiwayatPencarian() : head(nullptr), jumlah(0) {}

    ~RiwayatPencarian() {
        NodeSingly* cur = head;
        while (cur) {
            NodeSingly* tmp = cur;
            cur = cur->next;
            delete tmp;
        }
    }

    // Tambah keyword ke depan (push front)
    void tambah(const string& keyword) {
        NodeSingly* node = new NodeSingly(keyword);
        node->next = head;
        head = node;
        jumlah++;
    }

    void tampilkan() const {
        if (!head) {
            cout << "  (Belum ada riwayat pencarian)\n";
            return;
        }
        NodeSingly* cur = head;
        int i = 1;
        while (cur) {
            cout << "  " << i++ << ". " << cur->keyword << endl;
            cur = cur->next;
        }
    }

    void hapusSemua() {
        NodeSingly* cur = head;
        while (cur) {
            NodeSingly* tmp = cur;
            cur = cur->next;
            delete tmp;
        }
        head = nullptr;
        jumlah = 0;
        cout << "  Riwayat pencarian dihapus.\n";
    }
};

// ============================================================
// DOUBLY LINKED LIST  — Daftar Film Utama
// ============================================================
struct NodeDoubly {
    Film      data;
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

    // --- helper: salin semua film ke array, urutkan, rebuild list ---
    void sortBy(bool (*cmp)(const Film&, const Film&)) {
        if (!head || !head->next) return;

        // Kumpulkan ke vector sementara
        Film arr[200];
        int n = 0;
        NodeDoubly* cur = head;
        while (cur) { arr[n++] = cur->data; cur = cur->next; }

        // Bubble sort sederhana
        for (int i = 0; i < n - 1; i++)
            for (int j = 0; j < n - i - 1; j++)
                if (cmp(arr[j + 1], arr[j]))
                    swap(arr[j], arr[j + 1]);

        // Tulis kembali ke node
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

    // ----------------------------------------------------------
    // ANGGOTA 1 — Tambah Film
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
        cout << "\n  [OK] Film \"" << judul << "\" berhasil ditambahkan!\n";
    }

    // ----------------------------------------------------------
    // ANGGOTA 1 — Tampilkan Semua Film (traversal maju)
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
        cout << string(75, '-') << endl;
    }

    // Traversal mundur (fitur bonus doubly linked list)
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
        cout << string(75, '-') << endl;
    }

    // ----------------------------------------------------------
    // ANGGOTA 2 — Hapus Film berdasarkan ID
    // ----------------------------------------------------------
    void hapusFilm(int id) {
        NodeDoubly* cur = head;
        while (cur) {
            if (cur->data.id == id) {
                string judul = cur->data.judul;

                if (cur->prev) cur->prev->next = cur->next;
                else           head = cur->next;

                if (cur->next) cur->next->prev = cur->prev;
                else           tail = cur->prev;

                delete cur;
                jumlah--;
                cout << "\n  [OK] Film \"" << judul << "\" berhasil dihapus!\n";
                return;
            }
            cur = cur->next;
        }
        cout << "\n  [!] Film dengan ID " << id << " tidak ditemukan.\n";
    }

    // ----------------------------------------------------------
    // ANGGOTA 2 — Sorting
    // ----------------------------------------------------------
    void sortJudulAZ() {
        sortBy([](const Film& a, const Film& b){
            return a.judul < b.judul;
        });
        cout << "\n  [OK] Film diurutkan berdasarkan Judul (A-Z).\n";
    }

    void sortJudulZA() {
        sortBy([](const Film& a, const Film& b){
            return a.judul > b.judul;
        });
        cout << "\n  [OK] Film diurutkan berdasarkan Judul (Z-A).\n";
    }

    void sortRatingTertinggi() {
        sortBy([](const Film& a, const Film& b){
            return a.rating > b.rating;
        });
        cout << "\n  [OK] Film diurutkan berdasarkan Rating (Tertinggi).\n";
    }

    void sortRatingTerendah() {
        sortBy([](const Film& a, const Film& b){
            return a.rating < b.rating;
        });
        cout << "\n  [OK] Film diurutkan berdasarkan Rating (Terendah).\n";
    }

    void sortTahunTerbaru() {
        sortBy([](const Film& a, const Film& b){
            return a.tahun > b.tahun;
        });
        cout << "\n  [OK] Film diurutkan berdasarkan Tahun (Terbaru).\n";
    }

    void sortTahunTerlama() {
        sortBy([](const Film& a, const Film& b){
            return a.tahun < b.tahun;
        });
        cout << "\n  [OK] Film diurutkan berdasarkan Tahun (Terlama).\n";
    }

    // ----------------------------------------------------------
    // ANGGOTA 3 — Pencarian Film
    // ----------------------------------------------------------
    void cariFilm(const string& keyword, RiwayatPencarian& riwayat) const {
        // Simpan ke riwayat (singly linked list)
        riwayat.tambah(keyword);

        string kw = keyword;
        transform(kw.begin(), kw.end(), kw.begin(), ::tolower);

        cout << "\n====== HASIL PENCARIAN: \"" << keyword << "\" ======\n";
        Film dummy;
        dummy.tampilkanHeader();

        int no = 1, ditemukan = 0;
        NodeDoubly* cur = head;
        while (cur) {
            string judul = cur->data.judul;
            string sutradara = cur->data.sutradara;
            string genre = cur->data.genre;

            transform(judul.begin(), judul.end(), judul.begin(), ::tolower);
            transform(sutradara.begin(), sutradara.end(), sutradara.begin(), ::tolower);
            transform(genre.begin(), genre.end(), genre.begin(), ::tolower);

            if (judul.find(kw) != string::npos ||
                sutradara.find(kw) != string::npos ||
                genre.find(kw) != string::npos) {
                cur->data.tampilkanBaris(no++);
                ditemukan++;
            }
            cur = cur->next;
        }

        if (ditemukan == 0)
            cout << "  Tidak ada film yang cocok dengan \"" << keyword << "\".\n";
        else
            cout << string(75, '-') << "\n  " << ditemukan << " film ditemukan.\n";
    }

    // ----------------------------------------------------------
    // ANGGOTA 3 — Update Film berdasarkan ID
    // ----------------------------------------------------------
    void updateFilm(int id) {
        NodeDoubly* cur = head;
        while (cur) {
            if (cur->data.id == id) {
                Film& f = cur->data;
                cout << "\n  Update film: \"" << f.judul << "\"\n";
                cout << "  (Tekan Enter untuk mempertahankan nilai lama)\n\n";

                string input;

                cout << "  Judul baru [" << f.judul << "]: ";
                cin.ignore();
                getline(cin, input);
                if (!input.empty()) f.judul = input;

                cout << "  Sutradara baru [" << f.sutradara << "]: ";
                getline(cin, input);
                if (!input.empty()) f.sutradara = input;

                cout << "  Genre baru [" << f.genre << "]: ";
                getline(cin, input);
                if (!input.empty()) f.genre = input;

                cout << "  Tahun baru [" << f.tahun << "]: ";
                getline(cin, input);
                if (!input.empty()) f.tahun = stoi(input);

                cout << "  Rating baru [" << f.rating << "]: ";
                getline(cin, input);
                if (!input.empty()) f.rating = stof(input);

                cout << "\n  [OK] Film berhasil diupdate!\n";
                return;
            }
            cur = cur->next;
        }
        cout << "\n  [!] Film dengan ID " << id << " tidak ditemukan.\n";
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
// FUNGSI MENU
// ============================================================
void cetakMenu() {
    cout << "\n";
    cout << "+==========================================+\n";
    cout << "|      APLIKASI DATA FILM FAVORIT          |\n";
    cout << "+==========================================+\n";
    cout << "|  -- ANGGOTA 1 --                         |\n";
    cout << "|  1. Tambah Film                          |\n";
    cout << "|  2. Tampilkan Semua Film                 |\n";
    cout << "|  3. Tampilkan Film (Urutan Terbalik)     |\n";
    cout << "+------------------------------------------+\n";
    cout << "|  -- ANGGOTA 2 --                         |\n";
    cout << "|  4. Hapus Film                           |\n";
    cout << "|  5. Sorting Film                         |\n";
    cout << "+------------------------------------------+\n";
    cout << "|  -- ANGGOTA 3 --                         |\n";
    cout << "|  6. Cari Film                            |\n";
    cout << "|  7. Update Film                          |\n";
    cout << "|  8. Riwayat Pencarian                    |\n";
    cout << "+------------------------------------------+\n";
    cout << "|  0. Keluar                               |\n";
    cout << "+==========================================+\n";
    cout << "  Pilih menu: ";
}

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
        default: cout << "\n  Pilihan tidak valid.\n";
    }
}

// ============================================================
// MAIN
// ============================================================
int main() {
    DaftarFilm     daftar;
    RiwayatPencarian riwayat;

    // Data awal
    daftar.tambahFilm("Parasite",             "Bong Joon-ho",      "Drama",     2019, 8.6f);
    daftar.tambahFilm("Interstellar",          "Christopher Nolan", "Sci-Fi",    2014, 8.7f);
    daftar.tambahFilm("The Dark Knight",       "Christopher Nolan", "Action",    2008, 9.0f);
    daftar.tambahFilm("Spirited Away",         "Hayao Miyazaki",    "Animation", 2001, 8.6f);
    daftar.tambahFilm("Dune: Part Two",        "Denis Villeneuve",  "Sci-Fi",    2024, 8.5f);

    int pilih;
    do {
        cetakMenu();
        cin >> pilih;

        switch (pilih) {

            // ---- ANGGOTA 1 ----
            case 1: {
                cout << "\n  ===== TAMBAH FILM =====\n";
                string judul     = inputString("  Judul      : ");
                string sutradara = inputString("  Sutradara  : ");
                string genre     = inputString("  Genre      : ");
                int    tahun     = inputInt   ("  Tahun      : ");
                float  rating    = inputFloat ("  Rating(1-10): ");
                if (rating < 1) rating = 1;
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

            // ---- ANGGOTA 2 ----
            case 4: {
                daftar.tampilkanSemuaFilm();
                int id = inputInt("  Masukkan ID film yang dihapus: ");
                daftar.hapusFilm(id);
                break;
            }
            case 5:
                menuSorting(daftar);
                daftar.tampilkanSemuaFilm();
                break;

            // ---- ANGGOTA 3 ----
            case 6: {
                string kw = inputString("  Masukkan kata kunci pencarian: ");
                daftar.cariFilm(kw, riwayat);
                break;
            }
            case 7: {
                daftar.tampilkanSemuaFilm();
                int id = inputInt("  Masukkan ID film yang diupdate: ");
                daftar.updateFilm(id);
                break;
            }
            case 8: {
                cout << "\n  ===== RIWAYAT PENCARIAN (Singly Linked List) =====\n";
                riwayat.tampilkan();
                cout << "\n  Hapus riwayat? (1=Ya / 0=Tidak): ";
                int h; cin >> h;
                if (h == 1) riwayat.hapusSemua();
                break;
            }
            case 0:
                cout << "\n  Terima kasih! Program selesai.\n\n";
                break;
            default:
                cout << "\n  [!] Pilihan tidak valid.\n";
        }

    } while (pilih != 0);

    return 0;
}