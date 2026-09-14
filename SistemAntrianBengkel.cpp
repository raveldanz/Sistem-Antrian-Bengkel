#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

// Identitas Mahasiswa
const string MHS1 = "Raveldo Andyka Ramadhan - 24081010112";

// ---------------------------
// Queue (single linked list)
// ---------------------------
struct QNode {
    int nomor;      // nomor antrian
    string nama;
    string motor;
    string keluhan;
    QNode* next;
    QNode(int n, const string& nm, const string& mo, const string& ke)
        : nomor(n), nama(nm), motor(mo), keluhan(ke), next(nullptr) {}
};

struct Queue {
    QNode* front;
    QNode* rear;
    Queue() : front(nullptr), rear(nullptr) {}
};

void InitializeQueue(Queue* q) {
    q->front = nullptr;
    q->rear = nullptr;
}

bool IsEmptyQueue(Queue* q) {
    return (q->front == nullptr);
}

void EnqueueQueue(Queue* q, int nomor, const string& nama, const string& motor, const string& keluhan) {
    QNode* newNode = new QNode(nomor, nama, motor, keluhan);
    if (IsEmptyQueue(q)) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

QNode* DequeueQueue(Queue* q) {
    if (IsEmptyQueue(q)) return nullptr;
    QNode* temp = q->front;
    q->front = q->front->next;
    if (q->front == nullptr) q->rear = nullptr;
    temp->next = nullptr;
    return temp;
}

void DisplayQueue(Queue* q) {
    if (IsEmptyQueue(q)) {
        cout << "  (kosong)\n";
        return;
    }
    QNode* cur = q->front;
    while (cur) {
        cout << cur->nomor << ". " << cur->nama << " (" << cur->motor << ") - " << cur->keluhan << "\n";
        cur = cur->next;
    }
}

struct Member {
    string nama;
    string memberID; // bisa NIM atau nomor member
    Member(const string& n="", const string& id="") : nama(n), memberID(id) {}
};

struct AVLNode {
    Member data;
    AVLNode* left;
    AVLNode* right;
    int height;
    AVLNode(const Member& m) : data(m), left(nullptr), right(nullptr), height(1) {}
};

int height(AVLNode* n) { return n ? n->height : 0; }
int bf(AVLNode* n) { return n ? height(n->left) - height(n->right) : 0; }
void updateHeight(AVLNode* n) { if(n) n->height = 1 + max(height(n->left), height(n->right)); }

AVLNode* rotateRight(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    updateHeight(y); updateHeight(x);
    return x;
}

AVLNode* rotateLeft(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    updateHeight(x); updateHeight(y);
    return y;
}

// compare members by name (case sensitive); you can change to ID or lowercase compare
int cmpName(const string& a, const string& b) {
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

AVLNode* insertAVL(AVLNode* node, const Member& m) {
    if (!node) return new AVLNode(m);
    if (cmpName(m.nama, node->data.nama) < 0)
        node->left = insertAVL(node->left, m);
    else if (cmpName(m.nama, node->data.nama) > 0)
        node->right = insertAVL(node->right, m);
    else
        return node; // duplicate name -> ignore

    updateHeight(node);
    int balance = bf(node);

    // LL
    if (balance > 1 && cmpName(m.nama, node->left->data.nama) < 0)
        return rotateRight(node);
    // RR
    if (balance < -1 && cmpName(m.nama, node->right->data.nama) > 0)
        return rotateLeft(node);
    // LR
    if (balance > 1 && cmpName(m.nama, node->left->data.nama) > 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    // RL
    if (balance < -1 && cmpName(m.nama, node->right->data.nama) < 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

AVLNode* searchAVL(AVLNode* node, const string& nama) {
    if (!node) return nullptr;
    int c = cmpName(nama, node->data.nama);
    if (c == 0) return node;
    if (c < 0) return searchAVL(node->left, nama);
    return searchAVL(node->right, nama);
}

void inorderAVL(AVLNode* node) {
    if (!node) return;
    inorderAVL(node->left);
    cout << "  - " << node->data.nama << " (" << node->data.memberID << ")\n";
    inorderAVL(node->right);
}

struct BookingNode {
    int bookingNo;      // key
    string namaMember;  // must be a member
    int tiedQueueNo;    // nomor antrian saat booking dibuat (untuk display)
    BookingNode* left;
    BookingNode* right;
    BookingNode(int bno, const string& n, int qno) : bookingNo(bno), namaMember(n), tiedQueueNo(qno), left(nullptr), right(nullptr) {}
};

BookingNode* insertBooking(BookingNode* root, BookingNode* node) {
    if (!root) return node;
    if (node->bookingNo < root->bookingNo) root->left = insertBooking(root->left, node);
    else if (node->bookingNo > root->bookingNo) root->right = insertBooking(root->right, node);
    else {
        // duplicate bookingNo shouldn't happen
    }
    return root;
}

BookingNode* searchBooking(BookingNode* root, int bno) {
    if (!root) return nullptr;
    if (bno == root->bookingNo) return root;
    if (bno < root->bookingNo) return searchBooking(root->left, bno);
    return searchBooking(root->right, bno);
}

BookingNode* searchBookingByName(BookingNode* root, const string& name) {
    if (!root) return nullptr;
    if (root->namaMember == name) return root;
    BookingNode* left = searchBookingByName(root->left, name);
    if (left) return left;
    return searchBookingByName(root->right, name);
}

// Helper to delete booking node from BST (standard BST deletion)
BookingNode* findMinBooking(BookingNode* node) {
    while (node && node->left) node = node->left;
    return node;
}

BookingNode* deleteBooking(BookingNode* root, int bno) {
    if (!root) return nullptr;
    if (bno < root->bookingNo) root->left = deleteBooking(root->left, bno);
    else if (bno > root->bookingNo) root->right = deleteBooking(root->right, bno);
    else {
        // found
        if (!root->left) {
            BookingNode* r = root->right;
            delete root;
            return r;
        } else if (!root->right) {
            BookingNode* l = root->left;
            delete root;
            return l;
        } else {
            BookingNode* succ = findMinBooking(root->right);
            root->bookingNo = succ->bookingNo;
            root->namaMember = succ->namaMember;
            root->tiedQueueNo = succ->tiedQueueNo;
            root->right = deleteBooking(root->right, succ->bookingNo);
        }
    }
    return root;
}

void inorderBooking(BookingNode* root) {
    if (!root) return;
    inorderBooking(root->left);
    cout << "  Booking#" << root->bookingNo << " - " << root->namaMember << " (tied queue no: " << root->tiedQueueNo << ")\n";
    inorderBooking(root->right);
}

// ---------------------------
// Program utama menggunakan:
// - AVL untuk members
// - BST untuk bookings
// - 2 queues: priorityQueue (booking) dan regularQueue (walk-in/member tanpa booking)
// ---------------------------

void showHeader() {
    cout << "  SISTEM ANTRIAN BENGKEL BORE UP (dengan Member & Booking)\n";
    cout << "  Dibuat oleh:\n";
    cout << "  " << MHS1 << "\n";
    cout << "==========================================\n";
}

int main() {
    AVLNode* membersRoot = nullptr;          // AVL tree root
    BookingNode* bookingsRoot = nullptr;     // BST root for bookings
    Queue priorityQueue; InitializeQueue(&priorityQueue);
    Queue regularQueue; InitializeQueue(&regularQueue);

    int globalNomor = 1;     // nomor antrian naik setiap enqueue berhasil
    int nextBookingNo = 100; // contoh start booking number (bisa mulai dari 1)

    showHeader();

    int pilihan;
    do {
        cout << "\nMenu:\n";
        cout << "1. Register Member\n";
        cout << "2. Make Booking (member only)\n";
        cout << "3. Tambah Antrian (walk-in / member tanpa booking)\n";
        cout << "4. Layani Antrian (priority dilayani dulu)\n";
        cout << "5. Lihat Antrian (priority & regular)\n";
        cout << "6. Lihat Member\n";
        cout << "7. Lihat Bookings\n";
        cout << "8. Cancel Booking (by booking number)\n";
        cout << "9. Exit\n";
        cout << "Pilih menu (1-9): ";
        cin >> pilihan;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Input tidak valid!\n";
            continue;
        }
        cin.ignore(); // flush newline

        if (pilihan == 1) {
            // Register Member
            string nama, id;
            cout << "Nama member: ";
            getline(cin, nama);
            cout << "Member ID (contoh NIM / kode): ";
            getline(cin, id);
            if (nama.empty() || id.empty()) {
                cout << "Data tidak boleh kosong!\n";
            } else {
                if (searchAVL(membersRoot, nama)) {
                    cout << "Member dengan nama \"" << nama << "\" sudah terdaftar.\n";
                } else {
                    membersRoot = insertAVL(membersRoot, Member(nama, id));
                    cout << "Member \"" << nama << "\" berhasil didaftarkan.\n";
                }
            }
        }
        else if (pilihan == 2) {
            // Make Booking (only for members)
            string nama;
            cout << "Masukkan nama member: ";
            getline(cin, nama);
            if (nama.empty()) { cout << "Nama kosong.\n"; continue; }
            AVLNode* found = searchAVL(membersRoot, nama);
            if (!found) {
                cout << "Nama tidak ditemukan di member. Booking hanya untuk member. Ingin mendaftar dulu? (y/n): ";
                char ch; cin >> ch; cin.ignore();
                if (ch == 'y' || ch == 'Y') {
                    string id;
                    cout << "Masukkan Member ID: ";
                    getline(cin, id);
                    membersRoot = insertAVL(membersRoot, Member(nama, id));
                    cout << "Member terdaftar. Lanjut buat booking.\n";
                    found = searchAVL(membersRoot, nama);
                } else {
                    cout << "Booking dibatalkan.\n";
                    continue;
                }
            }
            // Check existing booking by name
            BookingNode* existing = searchBookingByName(bookingsRoot, nama);
            if (existing) {
                cout << "Member ini sudah memiliki booking: Booking#" << existing->bookingNo << "\n";
                continue;
            }
            // Buat booking: otomatis juga menambahkan ke priority queue
            string motor, keluhan;
            cout << "Masukkan Merk, tipe & nopol motor: ";
            getline(cin, motor);
            cout << "Masukkan Keluhan: ";
            getline(cin, keluhan);
            int thisQueueNo = globalNomor++;
            // add to priority queue
            EnqueueQueue(&priorityQueue, thisQueueNo, nama, motor, keluhan);
            int bno = nextBookingNo++;
            BookingNode* bn = new BookingNode(bno, nama, thisQueueNo);
            bookingsRoot = insertBooking(bookingsRoot, bn);
            cout << "Booking sukses: Booking#" << bno << " dan menjadi PRIORITAS (antrian nomor " << thisQueueNo << ").\n";
        }
        else if (pilihan == 3) {
            // Tambah antrian walk-in or member without booking
            string nama, motor, keluhan;
            cout << "Masukkan Nama Pelanggan: ";
            getline(cin, nama);
            cout << "Masukkan Merk, Tipe, dan Nopol Motor: ";
            getline(cin, motor);
            cout << "Masukkan Keluhan: ";
            getline(cin, keluhan);
            if (nama.empty() || motor.empty() || keluhan.empty()) {
                cout << "Data tidak boleh kosong!\n";
                continue;
            }
            // If person is member but hasn't booked, they go to regularQueue (no effect on priorityQueue)
            EnqueueQueue(&regularQueue, globalNomor, nama, motor, keluhan);
            cout << "Pelanggan " << nama << " (Antrian #" << globalNomor << ") berhasil ditambahkan ke regular queue.\n";
            globalNomor++;
        }
        else if (pilihan == 4) {
            // Layani antrian: priority dulu
            QNode* served = nullptr;
            if (!IsEmptyQueue(&priorityQueue)) {
                served = DequeueQueue(&priorityQueue);
                // also remove booking record from BST since served
                BookingNode* bfound = searchBookingByName(bookingsRoot, served->nama);
                if (bfound) bookingsRoot = deleteBooking(bookingsRoot, bfound->bookingNo);
                cout << "\n--- Melayani (PRIORITAS / Booking) ---\n";
            } else if (!IsEmptyQueue(&regularQueue)) {
                served = DequeueQueue(&regularQueue);
                cout << "\n--- Melayani (Regular) ---\n";
            } else {
                cout << "Antrian kosong! Tidak ada pelanggan yang dilayani.\n";
            }

            if (served) {
                cout << "Nomor Antrian : " << served->nomor << "\n";
                cout << "Nama          : " << served->nama << "\n";
                cout << "Motor         : " << served->motor << "\n";
                cout << "Keluhan       : " << served->keluhan << "\n";
                cout << "Pelanggan telah selesai dilayani.\n";
                delete served;
            }
        }
        else if (pilihan == 5) {
            cout << "\n-- Daftar Antrian PRIORITAS (Booking) --\n";
            DisplayQueue(&priorityQueue);
            cout << "\n-- Daftar Antrian REGULAR (Walk-in / Member tanpa booking) --\n";
            DisplayQueue(&regularQueue);
        }
        else if (pilihan == 6) {
            cout << "\n-- Daftar Member --\n";
            if (!membersRoot) cout << "  (tidak ada member)\n";
            else inorderAVL(membersRoot);
        }
        else if (pilihan == 7) {
            cout << "\n-- Daftar Booking --\n";
            if (!bookingsRoot) cout << "  (tidak ada booking)\n";
            else inorderBooking(bookingsRoot);
        }
        else if (pilihan == 8) {
            cout << "Masukkan Booking# yang akan dibatalkan: ";
            int bno; cin >> bno; cin.ignore();
            BookingNode* found = searchBooking(bookingsRoot, bno);
            if (!found) cout << "Booking#" << bno << " tidak ditemukan.\n";
            else {
                // jika booking ada, kita harus juga menghapus entry dari priorityQueue
                string targetName = found->namaMember;
                // hapus booking dari BST
                bookingsRoot = deleteBooking(bookingsRoot, bno);
                // Hapus satu node di priorityQueue yang nama == targetName
                // (karena booking dijamin ada di priorityQueue)
                Queue tmp; InitializeQueue(&tmp);
                bool removed = false;
                while (!IsEmptyQueue(&priorityQueue)) {
                    QNode* cur = DequeueQueue(&priorityQueue);
                    if (!removed && cur->nama == targetName) {
                        // skip (hapus)
                        removed = true;
                        delete cur;
                    } else {
                        EnqueueQueue(&tmp, cur->nomor, cur->nama, cur->motor, cur->keluhan);
                        delete cur;
                    }
                }
                // restore
                priorityQueue = tmp;
                cout << "Booking#" << bno << " dibatalkan (jika ada di antrian prioritas, sudah dihapus).\n";
            }
        }
        else if (pilihan == 9) {
            cout << "Terima kasih telah menggunakan sistem antrian.\n";
        }
        else {
            cout << "Pilihan tidak valid! Silakan pilih 1-9.\n";
        }

    } while (pilihan != 9);

    return 0;
}

