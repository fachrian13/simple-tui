﻿#include "simple.h"
#include <iostream>

void Render(Simple::Buffer& buffer, std::shared_ptr<Simple::Base::Renderable>& object) {
	object->Init();
	object->Set({ 0, 0, object->Width, object->Height });
	object->Render(buffer);
	std::cout << buffer.ToString() << "\x1b[H" << std::flush;
	buffer.Clear();
}

int main() {
	SetConsoleOutputCP(CP_UTF8);

	bool loop = true;

	auto iNamaDepan = Input("Nama Depan");
	iNamaDepan->Width = 24;
	auto iNamaBelakang = Input("Nama Belakang");
	iNamaBelakang->Width = 23;
	auto rLakiLaki = Radio("Laki-laki");
	auto rPerempuan = Radio("Perempuan");
	auto grJenisKelamin = MakeGroup(rLakiLaki, rPerempuan);
	auto iAlamat = Input("Jalan ...");
	iAlamat->Width = 48;
	iAlamat->Height = 3;
	auto rIslam = Radio("Islam");
	auto rKristen1 = Radio("Kristen Protestan");
	auto rKristen2 = Radio("Kristen Katolik");
	auto rHindu = Radio("Hindu");
	auto rBuddha = Radio("Buddha");
	auto rKonghuchu = Radio("Konghuchu");
	auto grAgama = MakeGroup(rIslam, rKristen1, rKristen2, rHindu, rBuddha, rKonghuchu);
	auto dJurusan = Dropdown({
			"Teknologi Pendidikan",
			"Administrasi Pendidikan",
			"Manajemen Pendidikan",
			"Psikologi Pendidikan dan Bimbingan",
			"Pendidikan Masyarakat",
			"Pendidikan Khusus",
			"Bimbingan dan Konseling",
			"Perpustakaan& Sains Informasi",
			"Pendidikan Guru Sekolah Dasar(PGSD)",
			"Pendidikan Guru Anak Usia Dini(PAUD)",
			"Pendidikan Luar Sekolah(PLS)",
			"Pendidikan Luar Biasa",
			"Pendidikan Bahasa Indonesia",
			"Pendidikan Bahasa Daerah",
			"Pendidikan Bahasa Inggris",
			"Pendidikan Bahasa Arab",
			"Pendidikan Bahasa Jepang",
			"Pendidikan Bahasa Jerman",
			"Pendidikan Bahasa Prancis",
			"Pendidikan Bahasa Korea",
			"Pendidikan Pancasila dan Kewarganegaraan",
			"Pendidikan Sejarah",
			"Pendidikan Geografi",
			"Pendidikan Sosiologi",
			"Pendidikan IPS",
			"Pendidikan Agama Islam",
			"Manajemen Pemasaran Pariwisata",
			"Pendidikan Matematika",
			"Pendidikan Fisika",
			"Pendidikan Biologi",
			"Pendidikan Kimia",
			"Pendidikan IPA",
			"Pendidikan Ilmu Komputer",
			"Pendidikan Seni Rupa",
			"Pendidikan Seni Tari",
			"Pendidikan Seni Musik",
			"Pendidikan Kepelatihan Olahraga",
			"Pendidikan Jasmani, Kesehatan, dan Rekreasi",
			"Pendidikan Teknik Otomotif",
			"Seni Rupa Murni",
			"Seni Kriya",
			"Seni Tari",
			"Seni Musik",
			"Desain dan Komunikasi Visual",
			"Desain Interior",
			"Desain Produk",
			"Tata Kelola Seni",
			"Film dan Televisi",
			"Film dan Animasi",
			"Musik",
			"Tata Rias",
			"Tata Busana",
			"Tata Boga"
		}, "Silakan Pilih");
	dJurusan->Width = 48;
	auto cTerm = CheckBox("Saya telah membaca peraturan");
	auto cTerm1 = CheckBox("Saya setuju dengan peraturan Univertsitas");
	auto bDaftar = Button("Daftar");
	auto bKeluar = Button("Keluar", [&loop]() { loop = false; });

	auto vLayout = VLayout(
		Text("       PENDAFTARAN CALON MAHASISWA BARU       ") | Border,
		Text("Nama Lengkap"),
		HLayout(iNamaDepan, Text(" "), iNamaBelakang),
		Text("Jenis Kelamin"),
		HLayout(rLakiLaki, rPerempuan),
		Text("Alamat Domisili"),
		iAlamat,
		Text("Agama"),
		VLayout(
			HLayout(rIslam, Text(" "), rKristen1, Text(" "), rKristen2),
			HLayout(rHindu, Text(" "), rBuddha, Text(" "), rKonghuchu)
		),
		Text("Jurusan Kuliah"),
		dJurusan,
		cTerm,
		cTerm1,
		bDaftar,
		bKeluar
	) | Border;

	auto vContainer = VContainer(
		HContainer(iNamaDepan, iNamaBelakang),
		HContainer(rLakiLaki, rPerempuan),
		iAlamat,
		HContainer(rIslam, rKristen1, rKristen2),
		HContainer(rHindu, rBuddha, rKonghuchu),
		dJurusan,
		cTerm,
		cTerm1,
		bDaftar,
		bKeluar
	);
	vContainer->Focused(true);

	auto buffer = Simple::Buffer(120, 30);

	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD record[128];
	DWORD size;

	std::cout << "\x1b[?25l";
	while (loop) {
		Render(buffer, vLayout);

		ReadConsoleInput(hIn, record, 128, &size);
		for (DWORD i = 0; i < size; ++i)
			if (record[i].EventType == KEY_EVENT && record[i].Event.KeyEvent.bKeyDown)
				vContainer->OnKey(record[i].Event.KeyEvent);
	}

	return 0;
}