#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <stdbool.h>

char inusername[50], inpassword[50], username[50], password[50], key;
char pilihUlang;

enum SortType {
	None = 0,
	HiPrice,
	LoPrice,
	HiStock,
	LoStock
};

const char* sortType_to_string(enum SortType Type) {
	switch (Type) {
	case None: return "Tidak Terurut";
	case HiPrice: return "Harga Tertinggi";
	case LoPrice: return "Harga Terendah";
	case HiStock: return "Stok Terbanyak";
	case LoStock: return "Stok Paling Sedikit";
	}
	return "Invalid";
}

typedef struct {
	const char* nama;
	int stock;
	float harga;
}Item;


typedef struct {
	int jumlahbarang;
	Item item[20];
	float totalharga;
}Pembeli;

typedef struct {
	void* next;
	Item data;
}Node;

void gotoxy(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void panah(int pospanah, int posmenu)
{
	if (pospanah == posmenu) {
		printf("==>");
	}
	else {
		printf("  ");
	}
}

int MenuUtama()
{
	system("cls");
	static int posisi = 1;
	panah(posisi, 1); printf(" Tambah Barang\n");
	panah(posisi, 2); printf(" List Barang\n");
	panah(posisi, 3); printf(" Beli Barang\n");
	panah(posisi, 4); printf(" hapus Barang\n");
	panah(posisi, 5); printf(" pilih urutan Barang\n");
	panah(posisi, 6); printf(" Kalkulasi\n");
	panah(posisi, 7); printf(" Exit\n");
	char tekan = _getch();
	if (tekan ==(char) 80) {
		system("cls");
		posisi++;
	}
	else if (tekan ==(char) 72) {
		system("cls");
		posisi--;
	}
	else {
		system("cls");
		posisi = posisi;
	}
	if (posisi > 7)
		posisi = 1;
	else if (posisi < 1)
		posisi = 7;
	if (tekan == 13)
		return posisi;
}


Node* head = NULL;
int TotalNodes = 0;
enum SortType sorttype = None;


Node* TambahData() {
	char* inputnama=NULL;
	int inputstok;
	float inputharga;
	char pilihan;

	label_menambahbarang:
	inputnama = malloc(sizeof(char) * 32);
	printf("nama : ");
	scanf(" %[^\n]s", inputnama);
	printf("stok : ");
	scanf("%d", &inputstok);
	printf("harga : ");
	scanf("%f", &inputharga);

	Item data;
	data.nama = inputnama;
	data.stock = inputstok;
	data.harga = inputharga;

	Node* new = NULL;
	if (head == NULL) {
		new = malloc(sizeof(Node));
		new->data = data;
		head = new;
		new->next = NULL;
		TotalNodes++;
	}
	else {
		new = malloc(sizeof(Node));
		new->data = data;
		new->next = head;
		head = new;
		TotalNodes++;
	}

	tanyaUlang:
	printf("mau menambahkan barang lagi?[y/n] : ");
	scanf(" %c", &pilihan);
	if (pilihan == 'Y' || pilihan == 'y') {
		system("cls");
		goto label_menambahbarang;
	}
	else if (pilihan == 'N' || pilihan == 'n') {
		system("cls");
		return;
	}
	else {
		system("cls");
		goto tanyaUlang;
	}
	return new;

}



void Kalkulasi(Node* current, Pembeli* pembeli)
{
	char* inputnama = NULL;
	int inputstok;
	char pilihan;

	label_listbarang:
	inputnama = malloc(sizeof(char) * 32);
	printf("list barang\n");
	
	current = head;
	while (current != NULL) {
		printf("nama  : %s\n", current->data.nama);
		printf("stok  : %d\n", current->data.stock);
		printf("harga : %.3f\n", current->data.harga);
		current = current->next;
	}

	printf("masukkan nama barang : ");
	scanf(" %[^\n]s", inputnama);
	current = head;
	bool berhasil = false;
	while (current != NULL) {
		if (strcmp(current->data.nama, inputnama)==0) {
			printf("masukkan stok : ");
			scanf("%d", &inputstok);
			if (inputstok > current->data.stock) {
				printf("stok kurang!!\n");
				tanyaUlang:
				printf("pilih barang lain?[y/n] : ");
				scanf(" %c", &pilihan);
				if (pilihan == 'Y' || pilihan == 'y') {
					system("cls");
					goto label_listbarang;
				}
				else if (pilihan == 'N' || pilihan == 'n') {
					system("cls");
					
					return;
				}
				else {
					system("cls");
					goto tanyaUlang;
				}
				
			}
			pembeli->item[pembeli->jumlahbarang].nama = inputnama;
			pembeli->item[pembeli->jumlahbarang].stock = inputstok;
			current->data.stock -= inputstok;
			pembeli->item[pembeli->jumlahbarang].harga = current->data.harga;
			pembeli->totalharga += pembeli->item[pembeli->jumlahbarang].harga * pembeli->item[pembeli->jumlahbarang].stock;
			pembeli->jumlahbarang++;
			berhasil = true;
		}
		current = current->next;

	}
	
	if(berhasil== false)
		printf("barang tidak ditemukan\n");
    tanyaUlang2:
	printf("pilih barang lain?[y/n] : ");
	scanf(" %c", &pilihan);
	if (pilihan == 'Y' || pilihan == 'y') {
		system("cls");
		goto label_listbarang;
	}
	else if (pilihan == 'N' || pilihan == 'n') {
		system("cls");
		return;
	}
	else {
		system("cls");
		goto tanyaUlang2;
	}
}




void printlist(Node* current)
{
	printf("Jenis Sort : %s\n", sortType_to_string(sorttype));
	
	Node** nodes = malloc(sizeof(Node*) * TotalNodes);
	int count = 0;
	
	for (current = head; current != NULL; current = current->next)
		nodes[count++] = current;

	if (sorttype != None) {
		for (int i = 0; i < TotalNodes; i++) {
			for (int j = 0; j < TotalNodes - i - 1; j++) {
				Node* temp = NULL;
				if (sorttype == HiPrice) {
					if (nodes[j]->data.harga < nodes[j + 1]->data.harga) {
						temp = nodes[j];
						nodes[j] = nodes[j + 1];
						nodes[j + 1] = temp;
					}
				}
				if (sorttype == LoPrice) {
					if (nodes[j]->data.harga > nodes[j + 1]->data.harga) {
						temp = nodes[j];
						nodes[j] = nodes[j + 1];
						nodes[j + 1] = temp;
					}
				}
				if (sorttype == HiStock) {
					if (nodes[j]->data.stock < nodes[j + 1]->data.stock) {
						temp = nodes[j];
						nodes[j] = nodes[j + 1];
						nodes[j + 1] = temp;
					}
				}
				if (sorttype == LoStock) {
					if (nodes[j]->data.stock > nodes[j + 1]->data.stock) {
						temp = nodes[j];
						nodes[j] = nodes[j + 1];
						nodes[j + 1] = temp;
					}
				}
			}
		}
	}
	

	printf("List Barang : \n");
	printf("======================================================================================\n");
	printf("| No. |          Nama                           |     stok     |      Harga          |\n");
	printf("======================================================================================\n");
	for (int i = 0; i < TotalNodes; i++) {
	
		gotoxy(0, 5+i); printf("|  %d.  ", i + 1);
		gotoxy(6, 5+i);  printf("|    %s", nodes[i]->data.nama);
		gotoxy(48, 5+i); printf("|       %d", nodes[i]->data.stock);
		gotoxy(63, 5+i); printf("|       %.3f", nodes[i]->data.harga);
		gotoxy(85, 5+i); printf("|\n");
		
	}
	printf("======================================================================================\n");
}


void HapusList(Node* current)
{	
	Node* prev = NULL;
	char* inputnama = NULL;
	char input;

Label_ulang:
	current = head;
	prev = head;
	inputnama = malloc(sizeof(char) * 32);

	printlist(current);

	printf("Barang Yang Mau Dihapus : ");
	scanf("  %[^\n]s", inputnama);
	while (current != NULL) {
		if (strcmp(current->data.nama, inputnama) == 0) {
			if (current == head)
				head = current->next;
			else {
				prev->next = current->next;
				free(current);
				current = NULL;
			}
			printf("%s Berhasil Dihapus\n", inputnama);
			printf("Press Any Key To Go Back\n");
			_getch();
			TotalNodes--;
			return;
		}
		prev = current;
		current = current->next;
	}
	labelTanyalagi:
	printf("Barang %s Tidak Ditemnukan\n", inputnama);
	printf("Apakah Mau Cari Yang Lain?[y/n] : ");
	scanf(" %c", &input);
	if (input == 'Y' || input == 'y') {
		system("cls");
		goto Label_ulang;
	}
	else if (input == 'N' || input == 'n') {
		system("cls");
		return;
	}
	else {
		system("cls");
		goto labelTanyalagi; 
	}
}

void printPembelian(Pembeli* pembeli)
{
	printf("list pembelian\n");
	for (int i = 0; i < pembeli->jumlahbarang; i++) {
		printf("%s\n", pembeli->item[i].nama);
		printf("%d\n", pembeli->item[i].stock);
		printf("%.3f\n", pembeli->item[i].harga);
	}
	printf("Total Harga : %.3f\n", pembeli->totalharga);
	printf("Press Any Key To Go Back!\n");
	_getch();
}

void UrutanBarang() {
	char input = '\0';
	int posisi = 1;

	while (true) {
		printf("Pilih Urutan Barang\n");
		panah(posisi, 1); printf(" Tidak Terurut\n");
		panah(posisi, 2); printf(" Harga Tertinggi\n");
		panah(posisi, 3); printf(" Harga Terendah\n");
		panah(posisi, 4); printf(" Stok Terbanyak\n");
		panah(posisi, 5); printf(" Stok Paling sedikit\n");
		input = _getch();
		if (input == 80) {
			system("cls");
			posisi++;
		}
		else if (input == 72) {
			system("cls");
			posisi--;
		}
		else {
			posisi = posisi;
		}
		if (posisi < 1)
			posisi = 5;
		else if (posisi > 5)
			posisi = 1;
		if (input == '\r')
			break;
		
	}

	switch (posisi) {
	case 1:
		sorttype = None;
		break;
	case 2:
		sorttype = HiPrice;
		break;
	case 3:
		sorttype = LoPrice;
		break;
	case 4:
		sorttype = HiStock;
		break;
	case 5:
		sorttype = LoStock;
		break;

	}

	printf("Jenis Sort Diubah Menjadi : %s\n", sortType_to_string(sorttype));
	printf("Press Any Key To Go Back");
	_getch();

}

void menuUtama()
{

	Node* list = NULL;
	Pembeli pembeli;
	pembeli.jumlahbarang = 0;
	pembeli.totalharga = 0.0f;
	int pilihan = 0;
	while (pilihan != 7) {
		pilihan = MenuUtama();
		switch (pilihan) {
		case 1:
			list = TambahData();
			break;
		case 2:
			printlist(list);
			_getch();
			break;
		case 3:
			Kalkulasi(list, &pembeli);
			break;
		case 4:
			HapusList(list);
			break;
		case 5:
			UrutanBarang();
			break;
		case 6:
			printPembelian(&pembeli);
			break;
		case 7:
			return 0;
		}

	}


	return 0;
}

void loading()
{ 
	system("color 0a");
	gotoxy(50, 15);
	printf("Loading");
	gotoxy(50, 16);
	for (int i = 0; i < 30; i++) {

		printf("%c", 220);
		Sleep(100);

	}

	menuUtama();

}



//Rheyna Atalya S (672022168)
void daftar()
{
	printf("MENU DAFTAR\n");
	printf("masukkan username = ");
	scanf(" %[^\n]s", &inusername);
	printf("masukkan password = ");
	scanf(" %[^\n]s", &inpassword);
	strcpy(username, inusername);
	strcpy(password, inpassword);
	printf("\nANDA TELAH REGIS\n\n");
	system("pause");
	system("cls");
	main();

}

//Rheyna Atalya S (672022168)
void login()
{

	int i = 0;
	system("cls");
labelLogin:
	if (inusername[0] == '\0' || inpassword[0] == '\0') {
		printf("anda Belum Register\n");
		return;
	}
	printf("masukkan username : ");
	scanf(" %[^\n]s", &username);


	printf("masukkan password : ");
	while ((key = _getch()) != 13)
	{
		if (i < 0)
			i = 0;
		if (key == 8)
		{
			if (i == 0)
				continue;
			else
			{
				printf("\b \b");
				i--;
				continue;
			}
		}
		if (i == 8)
		{
			continue;
		}
		printf("*");
		password[i] = key;
		i++;
	}
	password[i] = '\0';
	if (strcmp(inusername, username) == 0 && strcmp(inpassword, password) == 0) {
		printf("\nLOGIN SUKSES\n");
		system("pause");
		system("cls");
		loading();
	}
	else if (strcmp(inusername, username) != 0 && strcmp(inpassword, password) != 0) {
		printf("\nPASSWORD ATAU USERNAME ANDA SALAH\n");
		system("pause");
		system("cls");
		login();
	}
	else {
		printf("\npassword atau username anda salah\n");
		system("pause");
		system("cls");
		login();
	}

	system("cls");
	main();


}

void animasi()
{
	int x; double y;
	int key;
	char Pembuat[] = "\t\t\t   Pembuat :";
	char NIM1[] = "\t\t\t   Angelina Sekar Ayu (672022240)";
	char NIM2[] = "\t\t\t   Florentino Lama Tokan (672022245)";
	char NIM3[] = "\t\t\t   Cheful Mandiri Djanning (672022256)";
	char NIM4[] = "\t\t\t   Rheyna Atalya S (672022168)";
	char NIM5[] = "\t\t\t   Gwen Theresia Grandis A (672022250)";

	printf("\n\n\n");
	for (x = 0; Pembuat[x] != NULL; x++)
	{
		printf(" %c", Pembuat[x]);
		for (y = 0; y <= 9999999; y++)
		{
		}

	}
	printf("\n");
	for (x = 0; NIM1[x] != NULL; x++)
	{
		printf(" %c", NIM1[x]);
		for (y = 0; y <= 9999999; y++)
		{
		}

	}
	printf("\n");
	for (x = 0; NIM2[x] != NULL; x++)
	{
		printf(" %c", NIM2[x]);
		for (y = 0; y <= 9999999; y++)
		{
		}

	}
	printf("\n");
	for (x = 0; NIM3[x] != NULL; x++)
	{
		printf(" %c", NIM3[x]);
		for (y = 0; y <= 9999999; y++)
		{
		}

	}
	printf("\n");
	for (x = 0; NIM4[x] != NULL; x++)
	{
		printf(" %c", NIM4[x]);
		for (y = 0; y <= 9999999; y++)
		{
		}

	}
	printf("\n");
	for (x = 0; NIM5[x] != NULL; x++)
	{
		printf(" %c", NIM5[x]);
		for (y = 0; y <= 9999999; y++)
		{
		}

	}
	printf("\n");
	_getch();


}

int main() {
	system("cls");
	system("color 0a");
	gotoxy(2, 2);
	printf("================");
	gotoxy(2, 3);
	printf("     WELCOME    ");
	gotoxy(2, 4);
	printf("================");
	gotoxy(2, 10);
	printf("================");
	int posisi = 1;
	int tekan = 0;

	while (tekan != 13) {
		gotoxy(2, 5);
		panah(1, posisi); printf(" 1. Register");
		gotoxy(2, 6);
		panah(2, posisi); printf(" 2. Login");
		gotoxy(2, 7);
		panah(3, posisi); printf(" 3. EXIT");
		tekan = _getch();
		if (tekan == 80 && posisi != 3) {
			posisi++;
		}
		else if (tekan == 72 && posisi != 1) {
			posisi--;
		}

		if (posisi < 1) {
			posisi = 3;
		}
		else if (posisi > 3) {
			posisi = 1;
		}
		else {
			posisi = posisi;
		}
	}int x = posisi;

	switch (x) {
	case 1:
		system("cls");
		system("COLOR F8");

		daftar();

		system("pause");
		main();
		break;

	case 2:
		system("cls");
		system("COLOR F8");

		login();

		system("pause");
		main();
		break;

	case 3:
		system("cls");
		system("COLOR F8");

		animasi();

		system("pause");
		exit(0);
	}
}

