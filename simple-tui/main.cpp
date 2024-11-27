#include "simple.h"
#include <iostream>

using simple::COLOR;
using simple::pixel;
using simple::buffer;
using simple::buttons_group;

HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

COORD fullscreen() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hOut, &csbi);
	return csbi.dwSize;
}

void render(buffer& b, std::shared_ptr<simple::base::node> node) {
	node->init();
	int x = (b.getWidth() - node->width) / 2;
	int y = (b.getHeight() - node->height) / 2;
	node->set({ x, y, x + node->width, y + node->height });
	b.clear();
	node->render(b);
	std::cout << b.toString() << "\x1b[H" << std::flush;
}

int main() {
	COORD size = fullscreen();
	buffer b = buffer(size.X, size.Y);
	bool loop = true;

	auto iNamaDepan = input("Nama Depan");
	iNamaDepan->width = 24;
	auto iNamaBelakang = input("Nama Belakang");
	iNamaBelakang->width = 25;
	auto rLakiLaki = radio("Laki-laki");
	auto rPerempuan = radio("Perempuan");
	auto bgJenisKelamin = buttons_group({ rLakiLaki, rPerempuan });
	auto iEmail = input("your@mail.id");
	iEmail->width = 50;
	auto iNomorTelepon = input("08***");
	iNomorTelepon->width = 50;
	iNomorTelepon->pattern = isdigit;
	iNomorTelepon->limit = 13;
	auto iAlamat = input();
	iAlamat->width = 50;
	iAlamat->height = 3;
	auto rIslam = radio("Islam");
	auto rKristen1 = radio("Kristen Katolik");
	auto rKristen2 = radio("Kristen Protestan");
	auto rHindu = radio("Hindu");
	auto rBuddha = radio("Buddha");
	auto rKonghuchu = radio("Konghuchu");
	auto gAgama = buttons_group({ rIslam, rKristen1, rKristen2, rHindu, rBuddha, rKonghuchu });
	auto dProvinsi = dropdown(
		{
			"Nanggroe Aceh Darussalam(Ibu Kota Banda Aceh)",
			"Sumatera Utara(Ibu Kota Medan)",
			"Sumatera Selatan(Ibu Kota Palembang)",
			"Sumatera Barat(Ibu Kota Padang)",
			"Bengkulu(Ibu Kota Bengkulu)",
			"Riau(Ibu Kota Pekanbaru)",
			"Kepulauan Riau(Ibu Kota Tanjung Pinang)",
			"Jambi(Ibu Kota Jambi)",
			"Lampung(Ibu Kota Bandar Lampung)",
			"Bangka Belitung(Ibu Kota Pangkal Pinang)",
			"Kalimantan Barat(Ibu Kota Pontianak)",
			"Kalimantan Timur(Ibu Kota Samarinda)",
			"Kalimantan Selatan(Ibu Kota Banjarbaru)",
			"Kalimantan Tengah(Ibu Kota Palangkaraya)",
			"Kalimantan Utara(Ibu Kota Tanjung Selor)",
			"Banten(Ibu Kota Serang)",
			"DKI Jakarta(Ibu Kota Jakarta)",
			"Jawa Barat(Ibu Kota Bandung)",
			"Jawa Tengah(Ibu Kota Semarang)",
			"Daerah Istimewa Yogyakarta(Ibu Kota Yogyakarta)",
			"Jawa Timur(Ibu Kota Surabaya)",
			"Bali(Ibu Kota Denpasar)",
			"Nusa Tenggara Timur(Ibu Kota Kupang)",
			"Nusa Tenggara Barat(Ibu Kota Mataram)",
			"Gorontalo(Ibu Kota Gorontalo)",
			"Sulawesi Barat(Ibu Kota Mamuju)",
			"Sulawesi Tengah(Ibu Kota Palu)",
			"Sulawesi Utara(Ibu Kota Manado)",
			"Sulawesi Tenggara(Ibu Kota Kendari)",
			"Sulawesi Selatan(Ibu Kota Makassar)",
			"Maluku Utara(Ibu Kota Sofifi)",
			"Maluku(Ibu Kota Ambon)",
			"Papua Barat(Ibu Kota Manokwari)",
			"Papua(Ibu Kota Jayapura)",
			"Papua Tengah(Ibu Kota Nabire)",
			"Papua Pegunungan(Ibu Kota Jayawijaya)",
			"Papua Selatan(Ibu Kota Merauke)",
			"Papua Barat Daya(Ibu Kota Sorong)"
		},
		"Silakan Pilih"
	);
	dProvinsi->width = 50;
	auto iTanggal = input("Tanggal");
	iTanggal->width = 16;
	iTanggal->limit = 2;
	iTanggal->pattern = isdigit;
	auto iBulan = input("Bulan");
	iBulan->width = 16;
	iBulan->limit = 2;
	iBulan->pattern = isdigit;
	auto iTahun = input("Tahun");
	iTahun->width = 16;
	iTahun->limit = 4;
	iTahun->pattern = isdigit;
	auto bDaftar = button("Daftar", [&loop]() {	loop = false; });
	auto cConfirm = checkbox("Bagikan kepada teman");
	auto cCheck = checkbox("Saya menyetujui persyaratan");
	auto bKeluar = button("Keluar", [&loop]() { loop = false; });

	auto vc = vcontainer(
		hcontainer(iNamaDepan, iNamaBelakang),
		hcontainer(rLakiLaki, rPerempuan),
		iEmail,
		iNomorTelepon,
		iAlamat,
		hcontainer(rIslam, rKristen1, rKristen2),
		hcontainer(rHindu, rBuddha, rKonghuchu),
		dProvinsi,
		hcontainer(iTanggal, iBulan, iTahun),
		cCheck,
		cConfirm,
		bDaftar,
		bKeluar
	);
	vc->focused(true);

	std::cout << "\x1b[?25l" << std::flush;
	while (loop) {
		render(b, vlayout(
			text("=============================="),
			text(" PENDAFTARAN KEANGGOTAAN KLUB"),
			text("=============================="),
			text(""),
			text("Nama"),
			hlayout(iNamaDepan, text(" "), iNamaBelakang),
			text("Jenis Kelamin"),
			hlayout(rLakiLaki, text(" "), rPerempuan),
			text("Email"),
			iEmail,
			text("Nomor Telepon"),
			iNomorTelepon,
			text("Alamat"),
			iAlamat,
			text("Agama"),
			hlayout(rIslam, text(" "), rKristen1, text(" "), rKristen2),
			hlayout(rHindu, text(" "), rBuddha, text(" "), rKonghuchu),
			text("Provinsi"),
			dProvinsi,
			text("Tanggal Lahir"),
			hlayout(iTanggal, text(" "), iBulan, text(" "), iTahun),
			cCheck,
			cConfirm,
			bDaftar,
			bKeluar
		) | background(COLOR::BLUE) | border);

		INPUT_RECORD record[128];
		DWORD eventsRead;
		ReadConsoleInput(hIn, record, 128, &eventsRead);
		for (DWORD i = 0; i < eventsRead; ++i)
			if (record[i].EventType == KEY_EVENT && record[i].Event.KeyEvent.bKeyDown)
				vc->onkey(record[i].Event.KeyEvent);
	}
}