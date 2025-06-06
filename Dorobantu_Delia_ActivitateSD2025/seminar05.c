#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;
typedef struct nod nod;
struct nod {
	Masina info;
	struct nod* next;
	struct nod* prev;
};
struct listaDubla {
	nod* first;
	nod* last;
};
typedef struct listaDubla listaDubla;

//creare structura pentru un nod dintr-o lista dublu inlantuita

//creare structura pentru Lista Dubla 

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasiniDeLaInceput(listaDubla lista ) {
	nod* p = lista.first;
	while (p != NULL) {
		afisareMasina(p->info);
		p = p->next;
	}
	//afiseaza toate elemente de tip masina din lista dublu inlantuita
	//prin apelarea functiei afisareMasina()
}

void afisareListaMasiniDeLaSfarsit(listaDubla lista) {
	nod* aux = lista.last;

	while (aux != NULL) {
		afisareMasina(aux->info);
		aux = aux->prev;
	}
}

void adaugaMasinaInListaLaSfarsit(listaDubla* lista, Masina masinaNoua) {
	nod* p = (nod*)malloc(sizeof(nod));
	p->info = masinaNoua;
	p->next = NULL;
	p->prev = lista->last;
	if (lista->last != NULL) {
		lista->last->next = p;
	}
	else {
		lista->first = p;
	}
	lista->last = p;
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
}

void adaugaLaInceputInLista(listaDubla*lista, Masina masinaNoua) {
	nod* aux = (nod*)malloc(sizeof(nod));
	aux->info = masinaNoua;
	aux->next = lista->first;
	aux->prev = NULL;
	if (lista->first != NULL) {
		lista->first->prev = aux;
	}
	else {
		lista->last = aux;
	}
	lista->first = aux;


	//adauga la inceputul listei dublu inlantuite o noua masina pe care o primim ca parametru
}

listaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul

	FILE* f = fopen(numeFisier, "r");
	listaDubla lista;
	lista.first = NULL;
	lista.last = NULL;
	while (!feof(f)) {
		Masina aux = citireMasinaDinFisier(f);
		adaugaMasinaInListaLaSfarsit(&lista, aux); // shallow copy
	}
	fclose(f);
	return lista;
}

void dezalocareLDMasini(listaDubla* list) {
	//sunt dezalocate toate masinile si lista dublu inlantuita de elemente
	while (list->first != NULL) {
		nod* aux = list->first;
		free(aux->info.numeSofer);
		free(aux->info.model);
		list->first = list->first->next;
		free(aux);
	}
}

float calculeazaPretMediu(listaDubla lista) {
	//calculeaza pretul mediu al masinilor din lista.

	float suma = 0;
	int nrNoduri = 0;
	while(lista.first != NULL) {
		suma += lista.first->info.pret;
		nrNoduri++;
		lista.first = lista.first->next;
	}
	return (suma > 0) ? suma / nrNoduri : 0;
}

void stergeMasinaDupaID(/*lista masini*/ int id) {
	//sterge masina cu id-ul primit.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
}

char* getNumeSoferMasinaScumpa(listaDubla lista) {
	//cauta masina cea mai scumpa si 
	//returneaza numele soferului acestei maasini.
	if (lista.first == NULL) {
		return NULL;
	}
	else {
		nod* max = lista.first;
		lista.first = lista.first->next;
		while (lista.first != NULL) {
			if (lista.first->info.pret > max->info.pret) {
				max = lista.first;
			}
			lista.first = lista.first->next;

		}
		char* numeSofer = malloc(strlen(max->info.numeSofer) + 1);
		strcpy_s(numeSofer, strlen(max->info.numeSofer) + 1, max->info.numeSofer);
		return numeSofer;
	}
}

int main() {

	listaDubla LD = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasiniDeLaInceput(LD);

	dezalocareLDMasini(&LD);

	return 0;
}