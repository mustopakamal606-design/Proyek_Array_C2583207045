/*
 * Sistem Monitoring Stok dan Penjualan Kantin Kampus
 * Mata Kuliah : Algoritma dan Pemrograman Lanjutan
 * Bahasa      : C (aplikasi console)
 *
 * Struktur data utama (semua berupa array):
 *   nama[MAKS][PANJANG_NAMA] -> array karakter 2 dimensi (nama produk)
 *   harga[MAKS]              -> array 1 dimensi (harga satuan)
 *   stok[MAKS]               -> array 1 dimensi (stok tersedia)
 *   terjual[MAKS]            -> array 1 dimensi (jumlah unit terjual)
 *   pendapatan[MAKS]         -> array 1 dimensi (akumulasi pendapatan per produk)
 *   jumlah                   -> jumlah data aktif (indeks 0 .. jumlah-1)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAKS          50   /* kapasitas maksimal produk */
#define PANJANG_NAMA  30
#define BATAS_MENIPIS 5    /* stok < 5 dianggap menipis */

char nama[MAKS][PANJANG_NAMA];
int  harga[MAKS];
int  stok[MAKS];
int  terjual[MAKS];
int  pendapatan[MAKS];
int  jumlah = 0;

/* ---------- Fungsi bantu input ---------- */

/* Membaca satu baris lalu mengubahnya menjadi bilangan bulat.
   Mengembalikan 1 jika valid, 0 jika bukan angka. */
int bacaInt(const char *prompt, int *hasil) {
    char buf[64], *akhir;
    long v;
    printf("%s", prompt);
    if (fgets(buf, sizeof(buf), stdin) == NULL) {
        printf("\nInput berakhir. Program ditutup.\n");
        exit(0);
    }
    v = strtol(buf, &akhir, 10);
    if (akhir == buf) return 0;
    while (*akhir == ' ' || *akhir == '\n' || *akhir == '\r') akhir++;
    if (*akhir != '\0') return 0;
    if (v > 1000000000L || v < -1000000000L) return 0;
    *hasil = (int)v;
    return 1;
}

/* Membaca teks satu baris (boleh mengandung spasi). */
void bacaTeks(const char *prompt, char *tujuan, int panjang) {
    printf("%s", prompt);
    if (fgets(tujuan, panjang, stdin) == NULL) {
        printf("\nInput berakhir. Program ditutup.\n");
        exit(0);
    }
    tujuan[strcspn(tujuan, "\r\n")] = '\0';
}

void keHurufKecil(const char *sumber, char *tujuan) {
    int i;
    for (i = 0; sumber[i] != '\0'; i++)
        tujuan[i] = (char)tolower((unsigned char)sumber[i]);
    tujuan[i] = '\0';
}

/* Meminta nomor produk (1..jumlah). Mengembalikan indeks, atau -1 jika tidak valid. */
int bacaNomorProduk(const char *prompt) {
    int no;
    if (!bacaInt(prompt, &no)) {
        printf("[!] Input harus berupa angka.\n");
        return -1;
    }
    if (no < 1 || no > jumlah) {
        printf("[!] Nomor produk %d tidak ditemukan.\n", no);
        return -1;
    }
    return no - 1;
}

/* ---------- Data awal ---------- */

void isiDataAwal(void) {
    const char *n[] = {"Nasi Goreng", "Mie Ayam", "Es Teh", "Gorengan", "Air Mineral", "Kopi Susu"};
    int h[] = {12000, 10000, 4000, 2000, 3000, 8000};
    int s[] = {20, 15, 30, 40, 50, 4};
    int i;
    for (i = 0; i < 6; i++) {
        strcpy(nama[i], n[i]);
        harga[i] = h[i];
        stok[i] = s[i];
        terjual[i] = 0;
        pendapatan[i] = 0;
    }
    jumlah = 6;
}

/* ---------- Fitur ---------- */

void tampilDaftar(void) {
    int i;
    printf("\n===== DAFTAR PRODUK =====\n");
    if (jumlah == 0) {
        printf("Belum ada produk.\n");
        return;
    }
    printf("%-4s %-30s %10s %6s\n", "No", "Nama Produk", "Harga", "Stok");
    for (i = 0; i < jumlah; i++)
        printf("%-4d %-30s %10d %6d\n", i + 1, nama[i], harga[i], stok[i]);
}

void tambahProduk(void) {
    char buf[PANJANG_NAMA];
    char a[PANJANG_NAMA], b[PANJANG_NAMA];
    int h, s, i;

    printf("\n===== TAMBAH PRODUK =====\n");
    if (jumlah >= MAKS) {
        printf("[!] Kapasitas penuh (maksimal %d produk).\n", MAKS);
        return;
    }
    bacaTeks("Nama produk : ", buf, PANJANG_NAMA);
    if (strlen(buf) == 0) {
        printf("[!] Nama produk tidak boleh kosong.\n");
        return;
    }
    keHurufKecil(buf, a);
    for (i = 0; i < jumlah; i++) {
        keHurufKecil(nama[i], b);
        if (strcmp(a, b) == 0) {
            printf("[!] Produk \"%s\" sudah ada.\n", nama[i]);
            return;
        }
    }
    if (!bacaInt("Harga       : ", &h)) { printf("[!] Harga harus berupa angka.\n"); return; }
    if (h <= 0)                         { printf("[!] Harga harus lebih dari 0 (tidak boleh negatif).\n"); return; }
    if (!bacaInt("Stok awal   : ", &s)) { printf("[!] Stok harus berupa angka.\n"); return; }
    if (s < 0)                          { printf("[!] Stok tidak boleh negatif.\n"); return; }

    strcpy(nama[jumlah], buf);
    harga[jumlah] = h;
    stok[jumlah] = s;
    terjual[jumlah] = 0;
    pendapatan[jumlah] = 0;
    jumlah++;
    printf("Produk \"%s\" berhasil ditambahkan.\n", buf);
}

void ubahProduk(void) {
    int idx, h, s;
    tampilDaftar();
    if (jumlah == 0) return;
    idx = bacaNomorProduk("Nomor produk yang diubah : ");
    if (idx < 0) return;
    if (!bacaInt("Harga baru : ", &h)) { printf("[!] Harga harus berupa angka.\n"); return; }
    if (h <= 0)                         { printf("[!] Harga harus lebih dari 0 (tidak boleh negatif).\n"); return; }
    if (!bacaInt("Stok baru  : ", &s)) { printf("[!] Stok harus berupa angka.\n"); return; }
    if (s < 0)                          { printf("[!] Stok tidak boleh negatif.\n"); return; }
    harga[idx] = h;
    stok[idx] = s;
    printf("Produk \"%s\" berhasil diubah.\n", nama[idx]);
}

void transaksiPenjualan(void) {
    int beli[MAKS] = {0};   /* keranjang: jumlah beli per produk pada transaksi ini */
    int idx, qty, i, total = 0, adaItem = 0;

    tampilDaftar();
    if (jumlah == 0) return;
    printf("\n===== TRANSAKSI PENJUALAN =====\n");
    printf("Masukkan nomor produk (0 = selesai).\n");

    while (1) {
        int no;
        if (!bacaInt("\nNomor produk : ", &no)) {
            printf("[!] Input harus berupa angka.\n");
            continue;
        }
        if (no == 0) break;
        if (no < 1 || no > jumlah) {
            printf("[!] Nomor produk %d tidak ditemukan.\n", no);
            continue;
        }
        idx = no - 1;
        if (!bacaInt("Jumlah beli   : ", &qty)) {
            printf("[!] Jumlah harus berupa angka.\n");
            continue;
        }
        if (qty <= 0) {
            printf("[!] Jumlah pembelian harus lebih dari 0.\n");
            continue;
        }
        if (qty > stok[idx] - beli[idx]) {
            printf("[!] Stok tidak cukup. Sisa stok %s: %d (sudah di keranjang: %d).\n",
                   nama[idx], stok[idx], beli[idx]);
            continue;
        }
        beli[idx] += qty;
        adaItem = 1;
        printf("Ditambahkan: %s x %d\n", nama[idx], qty);
    }

    if (!adaItem) {
        printf("Transaksi dibatalkan (tidak ada produk dibeli).\n");
        return;
    }

    printf("\n------------- STRUK -------------\n");
    printf("%-22s %4s %10s\n", "Produk", "Qty", "Subtotal");
    for (i = 0; i < jumlah; i++) {
        if (beli[i] > 0) {
            int subtotal = beli[i] * harga[i];
            printf("%-22s %4d %10d\n", nama[i], beli[i], subtotal);
            total += subtotal;
            /* transaksi berhasil: update stok, jumlah terjual, pendapatan */
            stok[i] -= beli[i];
            terjual[i] += beli[i];
            pendapatan[i] += subtotal;
        }
    }
    printf("---------------------------------\n");
    printf("%-27s %10d\n", "TOTAL PEMBAYARAN", total);
    printf("Transaksi berhasil.\n");
}

void tambahStok(void) {
    int idx, qty;
    tampilDaftar();
    if (jumlah == 0) return;
    idx = bacaNomorProduk("Nomor produk : ");
    if (idx < 0) return;
    if (!bacaInt("Jumlah stok yang ditambah : ", &qty)) { printf("[!] Jumlah harus berupa angka.\n"); return; }
    if (qty <= 0) { printf("[!] Jumlah tambahan stok harus lebih dari 0.\n"); return; }
    stok[idx] += qty;
    printf("Stok %s sekarang: %d\n", nama[idx], stok[idx]);
}

void cariProduk(void) {
    char kata[PANJANG_NAMA], kataKecil[PANJANG_NAMA], namaKecil[PANJANG_NAMA];
    int i, ketemu = 0;
    printf("\n===== CARI PRODUK =====\n");
    bacaTeks("Nama produk (sebagian juga boleh) : ", kata, PANJANG_NAMA);
    if (strlen(kata) == 0) {
        printf("[!] Kata kunci tidak boleh kosong.\n");
        return;
    }
    keHurufKecil(kata, kataKecil);
    for (i = 0; i < jumlah; i++) {
        keHurufKecil(nama[i], namaKecil);
        if (strstr(namaKecil, kataKecil) != NULL) {
            if (!ketemu)
                printf("%-4s %-30s %10s %6s\n", "No", "Nama Produk", "Harga", "Stok");
            printf("%-4d %-30s %10d %6d\n", i + 1, nama[i], harga[i], stok[i]);
            ketemu++;
        }
    }
    if (!ketemu) printf("Produk \"%s\" tidak ditemukan.\n", kata);
}

/* Menukar seluruh data produk pada indeks a dan b. */
void tukar(int a, int b) {
    char tmpNama[PANJANG_NAMA];
    int t;
    strcpy(tmpNama, nama[a]); strcpy(nama[a], nama[b]); strcpy(nama[b], tmpNama);
    t = harga[a];      harga[a] = harga[b];           harga[b] = t;
    t = stok[a];       stok[a] = stok[b];             stok[b] = t;
    t = terjual[a];    terjual[a] = terjual[b];       terjual[b] = t;
    t = pendapatan[a]; pendapatan[a] = pendapatan[b]; pendapatan[b] = t;
}

/* Bubble sort. mode 1: harga termurah; mode 2: terjual terbanyak. */
void urutkanProduk(void) {
    int pilih, i, j;
    printf("\n===== URUTKAN PRODUK =====\n");
    printf("1. Harga termurah\n2. Jumlah terjual terbanyak\n");
    if (!bacaInt("Pilih : ", &pilih) || (pilih != 1 && pilih != 2)) {
        printf("[!] Pilihan urutan tidak tersedia.\n");
        return;
    }
    for (i = 0; i < jumlah - 1; i++) {
        for (j = 0; j < jumlah - 1 - i; j++) {
            int tukarSekarang;
            if (pilih == 1) tukarSekarang = harga[j] > harga[j + 1];
            else            tukarSekarang = terjual[j] < terjual[j + 1];
            if (tukarSekarang) tukar(j, j + 1);
        }
    }
    printf("Produk berhasil diurutkan berdasarkan %s.\n",
           pilih == 1 ? "harga termurah" : "jumlah terjual terbanyak");
    printf("\n%-4s %-30s %10s %6s %8s\n", "No", "Nama Produk", "Harga", "Stok", "Terjual");
    for (i = 0; i < jumlah; i++)
        printf("%-4d %-30s %10d %6d %8d\n", i + 1, nama[i], harga[i], stok[i], terjual[i]);
}

void laporanPenjualan(void) {
    int i, totalUnit = 0, totalPendapatan = 0, maks = 0;
    printf("\n===== LAPORAN PENJUALAN =====\n");
    if (jumlah == 0) { printf("Belum ada produk.\n"); return; }
    printf("%-4s %-30s %8s %12s\n", "No", "Nama Produk", "Terjual", "Pendapatan");
    for (i = 0; i < jumlah; i++) {
        printf("%-4d %-30s %8d %12d\n", i + 1, nama[i], terjual[i], pendapatan[i]);
        totalUnit += terjual[i];
        totalPendapatan += pendapatan[i];
        if (terjual[i] > maks) maks = terjual[i];
    }
    printf("\nTotal unit terjual : %d\n", totalUnit);
    printf("Total pendapatan   : Rp %d\n", totalPendapatan);
    if (maks == 0) {
        printf("Produk terlaris    : belum ada penjualan\n");
    } else {
        printf("Produk terlaris    : ");
        int pertama = 1;
        for (i = 0; i < jumlah; i++) {
            if (terjual[i] == maks) {
                printf("%s%s", pertama ? "" : ", ", nama[i]);
                pertama = 0;
            }
        }
        printf(" (%d unit)\n", maks);
    }
}

void cekStokMenipis(void) {
    int i, ada = 0;
    printf("\n===== STOK MENIPIS (< %d) =====\n", BATAS_MENIPIS);
    for (i = 0; i < jumlah; i++) {
        if (stok[i] < BATAS_MENIPIS) {
            printf("[PERINGATAN] No %d - %s : sisa %d\n", i + 1, nama[i], stok[i]);
            ada = 1;
        }
    }
    if (!ada) printf("Semua produk memiliki stok mencukupi.\n");
}

void tampilMenu(void) {
    printf("\n=========================================\n");
    printf("   MONITORING STOK & PENJUALAN KANTIN\n");
    printf("=========================================\n");
    printf("1. Lihat Daftar Produk    6. Cari Produk\n");
    printf("2. Tambah Produk          7. Urutkan Produk\n");
    printf("3. Ubah Produk            8. Laporan Penjualan\n");
    printf("4. Transaksi Penjualan    9. Cek Stok Menipis\n");
    printf("5. Tambah Stok            0. Keluar\n");
}

int main(void) {
    int pilih;
    isiDataAwal();
    do {
        tampilMenu();
        if (!bacaInt("Pilih menu : ", &pilih)) {
            printf("[!] Pilihan menu harus berupa angka.\n");
            pilih = -1;
            continue;
        }
        switch (pilih) {
            case 1: tampilDaftar();       break;
            case 2: tambahProduk();       break;
            case 3: ubahProduk();         break;
            case 4: transaksiPenjualan(); break;
            case 5: tambahStok();         break;
            case 6: cariProduk();         break;
            case 7: urutkanProduk();      break;
            case 8: laporanPenjualan();   break;
            case 9: cekStokMenipis();     break;
            case 0: printf("Terima kasih. Program selesai.\n"); break;
            default: printf("[!] Pilihan menu %d tidak tersedia.\n", pilih);
        }
    } while (pilih != 0);
    return 0;
}
