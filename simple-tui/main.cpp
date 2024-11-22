#include "simple.h"
#include <iostream>

using simple::COLOR;
using simple::pixel;
using simple::buffer;

int main() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hOut, &csbi);

	buffer b = buffer(csbi.dwSize.X, csbi.dwSize.Y);

	auto iNamaLengkap = input("Ucup Mirin");
	auto iAlamat = input("JL...");
	iAlamat->height = 3;
	auto iTempatLahir = input();
	auto dJurusan = dropdown({
		"Teknik Mesin",
		"Teknik Otomotif",
		"Teknik Listrik",
		"Teknik Elektronika",
		"Teknik Komputer dan Jaringan",
		"Tata Boga",
		"Tata Busana",
		"Tata Rias",
		"Multimedia",
		"Desain Grafis",
		"Akuntansi",
		"Perhotelan dan Pariwisata",
		"Kesehatan",
		"Pemasaran",
		"Teknik Bangunan",
		"Pariwisata",
		"Perikanan",
		"Peternakan",
		"Agribisnis",
		"Kimia Industri",
		"Teknologi Pangan",
		"Tata Air",
		"Konstruksi Kapal",
		"Keamanan Jaringan",
		"Logistik",
		"Teknik Audio dan Video",
		"Teknik Pesawat Udara",
		"Kebidanan",
		"Akomodasi Perhotelan",
		"Farmasi"
		}, "Silakan Pilih");
	dJurusan->width = 30;
	auto bLogin = button("Login");
	auto bExit = button("Exit", []() { exit(0); });

	auto vc = vcontainer(
		iNamaLengkap,
		iAlamat,
		iTempatLahir,
		dJurusan,
		bLogin,
		bExit
	);
	vc->focused(true);

	INPUT_RECORD rec[128];
	DWORD numberOfEventsRead;
	std::cout << "\x1b[?25l" << std::flush;
	while (true) {
		auto vl = vlayout(
			hlayout(text("Nama Lengkap :"), iNamaLengkap),
			hlayout(text("Alamat       :"), iAlamat),
			hlayout(text("Tempat Lahir :"), iTempatLahir),
			hlayout(text("Jurusan      :"), dJurusan),
			bLogin,
			bExit
		) | border;
		b.clear();
		vl->init();
		int x = (b.getWidth() - vl->width) / 2;
		int y = (b.getHeight() - vl->height) / 2;
		vl->set({ x, y, x + vl->width, y + vl->height });
		vl->render(b);
		std::cout << "\x1b[H" << b.toString() << std::flush;

		ReadConsoleInput(hInput, rec, 128, &numberOfEventsRead);

		for (DWORD i = 0; i < numberOfEventsRead; ++i)
			if (rec[i].EventType == KEY_EVENT && rec[i].Event.KeyEvent.bKeyDown)
				vc->onkey(rec[i].Event.KeyEvent);
	}
}