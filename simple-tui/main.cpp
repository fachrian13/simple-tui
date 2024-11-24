#include "simple.h"
#include <iostream>

using simple::COLOR;
using simple::pixel;
using simple::buffer;

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
	iNamaDepan->width = 14;
	auto iNamaBelakang = input("Nama Belakang");
	iNamaBelakang->width = 15;
	auto iEmail = input("your@mail.id");
	auto iNomorTelepon = input("08***");
	iNomorTelepon->pattern = isdigit;
	iNomorTelepon->limit = 13;
	auto iAlamat = input();
	iAlamat->height = 3;
	auto iTanggal = input("Tanggal");
	iTanggal->width = 9;
	iTanggal->limit = 2;
	iTanggal->pattern = isdigit;
	auto iBulan = input("Bulan");
	iBulan->width = 9;
	iBulan->limit = 2;
	iBulan->pattern = isdigit;
	auto iTahun = input("Tahun");
	iTahun->width = 10;
	iTahun->limit = 4;
	iTahun->pattern = isdigit;
	auto bDaftar = button("Daftar", [&]() {
		loop = false;

		auto bOk = button("Ok");
		bOk->focused(true);

		render(b, vlayout(
			text("  PENDAFTARAN BERHASIL  "),
			text(""),
			bOk
		) | border);
		});
	auto bKeluar = button("Keluar", [&loop]() { loop = false; });

	auto vc = vcontainer(
		hcontainer(iNamaDepan, iNamaBelakang),
		iEmail,
		iNomorTelepon,
		iAlamat,
		hcontainer(iTanggal, iBulan, iTahun),
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
			text("Email"),
			iEmail,
			text("Nomor Telepon"),
			iNomorTelepon,
			text("Alamat"),
			iAlamat,
			text("Tanggal Lahir"),
			hlayout(iTanggal, text(" "), iBulan, text(" "), iTahun),
			bDaftar,
			bKeluar
		));

		INPUT_RECORD record[128];
		DWORD  eventsRead;
		ReadConsoleInput(hIn, record, 128, &eventsRead);
		for (DWORD i = 0; i < eventsRead; ++i)
			if (record[i].EventType == KEY_EVENT && record[i].Event.KeyEvent.bKeyDown)
				vc->onkey(record[i].Event.KeyEvent);
	}

	std::cin.get();
}