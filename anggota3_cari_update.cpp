/*
============================================================
  APLIKASI DATA FILM FAVORIT
  ANGGOTA 3 : Pencarian Film, Update Film, Riwayat Pencarian
  Struktur  : Singly Linked List (riwayat) +
              Doubly Linked List (daftar film), Class
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
// SINGLY LINKED LIST — Riwayat Pencarian
// Setiap keyword yang dicari disimpan di sini (push ke depan)
// Singly dipilih karena riwayat hanya perlu traversal satu arah
// ============================================================
struct NodeSingly {
    string      keyword;
    NodeSingly* next;
    NodeSingly(string k) : keyword(k), next(nullptr) {}
};

class RiwayatPencarian {
private:
    NodeSingly* head;
    int         jumlah;

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

    // Push depan: pencarian terbaru tampil di atas
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
        cout << "  Total: " << jumlah << " pencarian\n";
        cout << string(40, '-') << "\n";
        NodeSingly* cur = head;
        int i = 1;
        while (cur) {
            cout << "  " << i++ << ". " << cur->keyword << "\n";
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
        cout << "  [OK] Semua riwayat pencarian dihapus.\n";
    }

    int getJumlah() const { return jumlah; }
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

    // Tambah film (untuk inisialisasi data awal)
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
    // PENCARIAN FILM
    // Mencari berdasarkan judul, sutradara, atau genre (case-insensitive)
    // Keyword disimpan ke riwayat (Singly Linked List)
    // ----------------------------------------------------------
    void cariFilm(const string& keyword, RiwayatPencarian& riwayat) const {
        // Simpan keyword ke riwayat pencarian
        riwayat.tambah(keyword);

        // Konversi keyword ke lowercase untuk perbandingan
        string kw = keyword;
        transform(kw.begin(), kw.end(), kw.begin(), ::tolower);

        cout << "\n====== HASIL PENCARIAN: \"" << keyword << "\" ======\n";
        Film dummy;
        dummy.tampilkanHeader();

        int no = 1, ditemukan = 0;
        NodeDoubly* cur = head;
        while (cur) {
            string judul     = cur->data.judul;
            string sutradara = cur->data.sutradara;
            string genre     = cur->data.genre;

            transform(judul.begin(),     judul.end(),     judul.begin(),     ::tolower);
            transform(sutradara.begin(), sutradara.end(), sutradara.begin(), ::tolower);
            transform(genre.begin(),     genre.end(),     genre.begin(),     ::tolower);

            if (judul.find(kw)     != string::npos ||
                sutradara.find(kw) != string::npos ||
                genre.find(kw)     != string::npos) {
                cur->data.tampilkanBaris(no++);
                ditemukan++;
            }
            cur = cur->next;
        }

        if (ditemukan == 0)
            cout << "  Tidak ada film yang cocok dengan \"" << keyword << "\".\n";
        else
            cout << string(80, '-') << "\n  " << ditemukan << " film ditemukan.\n";
    }

    // ----------------------------------------------------------
    // UPDATE FILM berdasarkan ID
    // Tekan Enter untuk mempertahankan nilai lama
    // ----------------------------------------------------------
    void updateFilm(int id) {
        NodeDoubly* cur = head;
        while (cur) {
            if (cur->data.id == id) {
                Film& f = cur->data;
                cout << "\n  ===== UPDATE FILM: \"" << f.judul << "\" (ID=" << id << ") =====\n";
                cout << "  (Tekan Enter untuk mempertahankan nilai lama)\n\n";

                string input;

                cout << "  Judul baru      [" << f.judul << "]: ";
                cin.ignore();
                getline(cin, input);
                if (!input.empty()) f.judul = input;

                cout << "  Sutradara baru  [" << f.sutradara << "]: ";
                getline(cin, input);
                if (!input.empty()) f.sutradara = input;

                cout << "  Genre baru      [" << f.genre << "]: ";
                getline(cin, input);
                if (!input.empty()) f.genre = input;

                cout << "  Tahun baru      [" << f.tahun << "]: ";
                getline(cin, input);
                if (!input.empty()) {
                    try { f.tahun = stoi(input); }
                    catch (...) { cout << "  Input tidak valid, tahun tidak diubah.\n"; }
                }

                cout << "  Rating baru     [" << fixed << setprecision(1) << f.rating << "]: ";
                getline(cin, input);
                if (!input.empty()) {
                    try {
                        float r = stof(input);
                        if (r < 1) r = 1;
                        if (r > 10) r = 10;
                        f.rating = r;
                    } catch (...) { cout << "  Input tidak valid, rating tidak diubah.\n"; }
                }

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
    cout << "|   Anggota 3: Cari, Update, Riwayat       |\n";
    cout << "+==========================================+\n";
    cout << "|  1. Tampilkan Semua Film                 |\n";
    cout << "|  2. Cari Film                            |\n";
    cout << "|  3. Update Film                          |\n";
    cout << "|  4. Lihat Riwayat Pencarian              |\n";
    cout << "|  0. Keluar                               |\n";
    cout << "+==========================================+\n";
    cout << "  Pilih menu: ";
}

// ============================================================
// MAIN
// ============================================================
int main() {
    DaftarFilm       daftar;
    RiwayatPencarian riwayat;

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
                string kw = inputString("  Masukkan kata kunci (judul/sutradara/genre): ");
                daftar.cariFilm(kw, riwayat);
                break;
            }
            case 3: {
                daftar.tampilkanSemuaFilm();
                if (daftar.getJumlah() == 0) break;
                int id = inputInt("  Masukkan ID film yang akan diupdate: ");
                daftar.updateFilm(id);
                break;
            }
            case 4: {
                cout << "\n  ===== RIWAYAT PENCARIAN (Singly Linked List) =====\n";
                riwayat.tampilkan();
                if (riwayat.getJumlah() > 0) {
                    cout << "\n  Hapus semua riwayat? (1=Ya / 0=Tidak): ";
                    int h; cin >> h;
                    if (h == 1) riwayat.hapusSemua();
                }
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
