#include "simple.h"
#include <iostream>

using simple::COLOR;
using simple::pixel;
using simple::buffer;

int main() {
	buffer b = buffer(120, 30);
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);

	auto iUsername = input("Masukkan karakter");
	iUsername->height = 3;
	auto iPassword = input("Masukkan karakter");
	auto bLogin = button("Login");
	auto bExit = button("Exit", []() { exit(0); });
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
	auto vc = vcontainer(
		iUsername,
		iPassword,
		dJurusan,
		hcontainer(
			bLogin,
			bExit
		)
	);
	vc->focused(true);

	INPUT_RECORD rec[128];
	DWORD numberOfEventsRead;
	auto vl = vlayout(
		hlayout(text("Username :"), iUsername),
		hlayout(text("Password :"), iPassword),
		hlayout(text("Jurusan  :"), dJurusan),
		hlayout(bLogin, bExit)
	);
	while (true) {
		b.clear();
		vl->init();
		vl->set({ 0, 0, vl->width, vl->height });
		vl->render(b);
		std::cout << "\x1b[H" << b.toString() << std::flush;

		ReadConsoleInput(hInput, rec, 128, &numberOfEventsRead);

		for (DWORD i = 0; i < numberOfEventsRead; ++i)
			if (rec[i].EventType == KEY_EVENT && rec[i].Event.KeyEvent.bKeyDown)
				vc->onkey(rec[i].Event.KeyEvent);
	}
}