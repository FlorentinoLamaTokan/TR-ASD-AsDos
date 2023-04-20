#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <stdbool.h>

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

int main()
{
	Node* list = NULL;
	Pembeli pembeli;
	pembeli.jumlahbarang = 0;
	pembeli.totalharga = 0.0f;
	int pilihan = 0;
	while (pilihan!=7) {
		pilihan = MenuUtama();
		switch (pilihan) {
		case 1 :
			list = TambahData();
			break;
		case 2 :
			printlist(list);
			_getch();
			break;
		case 3 :
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