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

struct Stiva {
	Masina info;
	struct Stiva* next;
};
typedef struct Stiva Stiva;

//STACK
//Alegeti prin ce veti reprezenta stiva si creati structura necesara acestei stive
//putem reprezenta o stiva prin LSI, LDI sau vector
void pushStack(Stiva** nod, Masina masina) {
	
	Stiva* aux = (Stiva*)malloc(sizeof(Stiva));
	aux->info = masina;	
	aux->next = (*nod);
	(*nod) = aux;

}

Masina popStack(Stiva** nod) {
	Masina masina;
	if ((*nod) != NULL) 
	{
		Stiva* aux = (*nod);
		masina = (*nod)->info;
		(*nod) = (*nod)->next;
		free(*nod);
	}
	else {
		masina.id = -1;
	}
	return masina;
}

unsigned char isEmptyStack(Stiva* cap) 
{
	return cap == NULL;

}

Stiva* citireStackMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");
	Stiva* nod = NULL;
	while (!feof(f)) 
	{
		Masina m = citireMasinaDinFisier(f);
		pushStack(&nod, m);
	}
	fclose(f);
	return nod;
}

void dezalocareStivaDeMasini(Stiva** cap) {
	//sunt dezalocate toate masinile si stiva de elemente

	while (!isEmptyStack) {
		Masina m = popStack(cap);
		free(m.model);
		free(m.numeSofer);

	}
}

int size(Stiva* cap) {
	//returneaza numarul de elemente din stiva
	int marime = 0;
	while (cap != NULL) {
		marime++;
		cap = cap->next;
	}
	return marime;

}

//QUEUE
//Alegeti prin ce veti reprezenta coada si creati structura necesara acestei cozi
//putem reprezenta o coada prin LSI, LDI sau vector
typedef struct nod nod;
struct nod {
	Masina info;
	nod* next;
	nod* prev;
};

typedef struct Coada Coada;
struct Coada {
	nod* first;
	nod* last;
};


void enqueue( Coada* coada, Masina masina) {
	//adauga o masina in coada la inceput
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = masina;
	nou->next = (*coada).first;
	nou->prev = NULL;
	if ((*coada).first == NULL) {
		(*coada).first = nou;
		(*coada).last = nou;
	}
	else {
		(*coada).first->prev = nou;
		(*coada).first = nou;
	}
	
}

Masina dequeue(Coada* coada) {
	//extrage o masina din coada de la sfarsit

	if (coada->last == NULL) {
		Masina m;
		m.id = -1;
		return m;
	}
	else {
		Masina m = coada->last->info;
		if (coada->last->prev == NULL) {
			coada->first = NULL;
			coada->last = NULL;
		}
		else {
			nod* ultim = coada->last;
			coada->last = ultim->prev;
			ultim->prev->next = NULL;

			free(ultim);
		}
		return m;
	}
}

Coada citireCoadaDeMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul

	FILE* f = fopen(numeFisier, "r");
	Coada cap;
	cap.first = NULL;
	cap.last = NULL;
	while (!feof(f))
	{
		Masina m = citireMasinaDinFisier(f);
		enqueue(&cap, m);
	}
	fclose(f);
	return cap;
}

void dezalocareCoadaDeMasini(/*coada*/) {
	//sunt dezalocate toate masinile si coada de elemente
}


//metode de procesare
Masina getMasinaByID(/*stiva sau coada de masini*/int id);

float calculeazaPretTotal(Stiva** cap) {
	int suma = 0;
	Stiva* temp = NULL;

	while (!isEmptyStack(*cap)) {
		Masina m = popStack(cap);
		suma = +m.pret;
		pushStack(&temp, m);
	}

	while (!isEmptyStack(temp)) {
		pushStack(cap, popStack(temp));
	}
	return suma;
}

int main() {

	Stiva* cap;
	cap = citireStackMasiniDinFisier("masini.txt");
	//afisareMasina(popStack(&cap));

	printf("==================================================\n");

	Coada capCoada = citireCoadaDeMasiniDinFisier("masini.txt");
	afisareMasina(dequeue(&capCoada));

	printf("==================================================\n");

	float s = calculeazaPretTotal(&cap);
	printf("Pretul total este %.2f\n", s);

	return 0;
}