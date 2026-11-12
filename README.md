
Nama: mustopa kamal

NIM:C2583207045

Kelas:PTI3A

Mata Kuliah:Algoritma dan Pemrograman Lanjutan

Dosen Pengampu:Taofik Muhammad, M.Kom

Nama Proyek	Sistem:Monitoring Stok dan Penjualan Kantin Kampus

Deskripsi Singkat
Aplikasi console berbahasa C untuk mengelola produk, stok, transaksi penjualan, pencarian, pengurutan, dan laporan penjualan kantin kampus. Data disimpan dalam array selama program berjalan (tanpa database/file eksternal). Kapasitas maksimal 50 produk, dengan 6 produk sebagai data awal.

Daftar Fitur
Lihat daftar produk (nama, harga, stok)
Tambah produk baru
Ubah harga dan stok produk
Transaksi penjualan beberapa produk dalam satu transaksi (subtotal, total, stok berkurang, jumlah terjual bertambah)
Tambah stok
Cari produk berdasarkan nama
Urutkan produk (harga termurah / jumlah terjual terbanyak)
Laporan penjualan (terjual per produk, total unit, total pendapatan, produk terlaris)
Cek stok menipis (stok < 5)
Keluar

Validasi: menu/nomor produk tidak valid, jumlah beli ≤ 0, harga/stok negatif, jumlah beli melebihi stok, dan kapasitas array penuh.
<img width="668" height="523" alt="Screenshot 2026-11-12 104717" src="https://github.com/user-attachments/assets/2ec12173-8807-41c1-898d-d1f79cb10f30" />
Cara Kompilasi dan Menjalankan
bash
gcc kantin.c -o kantin
./kantin          # Windows: kantin.exe
Skenario Pengujian
Letakkan screenshot di folder screenshots/ lalu ganti nama file di bawah.
1. Tambah produk
Input: menu 2, isi produk baru. Hasil: produk muncul di daftar. Tambah produk
2. Transaksi
Input: menu 4, beli beberapa produk. Hasil: subtotal dan total benar. Transaksi
3. Pembaruan stok
Input: selesaikan transaksi, lihat menu 1. Hasil: stok berkurang, jumlah terjual bertambah. Pembaruan stok
4. Stok tidak cukup
Input: beli melebihi stok. Hasil: ditolak, stok tetap. Stok tidak cukup
5. Laporan
Input: menu 8. Hasil: unit, pendapatan, dan terlaris benar. Laporan
6. Pengurutan
Input: menu 7 (harga dan penjualan). Hasil: urutan sesuai ketentuan. Pengurutan
7. Pencarian dan stok

Input: menu 6 cari produk, menu 9 cek stok < 5. Hasil: hasil dan peringatan tepat. Pencarian dan stok

tangkapan layar hasil pengujian

<img width="1536" height="864" alt="image" src="https://github.com/user-attachments/assets/ceb2648f-3b48-4051-b43d-657c14723d22" />
<img width="1536" height="864" alt="image" src="https://github.com/user-attachments/assets/bf95d10e-2e8e-44cd-a938-38d67e0963d5" />
<img width="1536" height="864" alt="image" src="https://github.com/user-attachments/assets/8b136552-1d12-4bc2-bc96-507842d3f3e5" />
<img width="1536" height="864" alt="image" src="https://github.com/user-attachments/assets/6ae09c78-8134-4ef7-907c-c404611b8914" />
<img width="1536" height="864" alt="image" src="https://github.com/user-attachments/assets/2dec2c54-24bb-4e94-a7f0-48375fa718bc" />
<img width="1536" height="864" alt="image" src="https://github.com/user-attachments/assets/c123713c-f6a2-411e-8e46-7db7bce2ea22" />
<img width="1536" height="864" alt="image" src="https://github.com/user-attachments/assets/281c6ad3-fe92-4efb-9a36-aa4601e0fae0" />
<img width="1536" height="864" alt="image" src="https://github.com/user-attachments/assets/060cf277-e312-4d2a-9090-ba2636bcba60" />







