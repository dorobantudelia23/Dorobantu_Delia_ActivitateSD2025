#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Stiva {
	Masina info;
	struct Stiva* next;
};
typedef struct Stiva Stiva;


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
	m1.model = (char*)malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = (char*)malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi: %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void pushStack(Stiva** stiva, Masina masina) {
	Stiva* aux = (Stiva*)malloc(sizeof(Stiva));
	aux->info = masina;
	aux->next = (*stiva);
	(*stiva) = aux;
}

Masina popStack(Stiva** stiva) {
	Masina m;
	m.id = -1;
	if ((*stiva) != NULL) {
		Stiva* aux = (*stiva);
		m = (*stiva)->info;
		(*stiva) = (*stiva)->next;
		free(aux);
	}
	return m;
}

Stiva* citireStiva(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Stiva* stiva = NULL;
	while (!feof(f)) {
		pushStack(&stiva, citireMasinaDinFisier(f));
	}
	fclose(f);
	return stiva;
}

unsigned int isEmptyStack(Stiva* stiva) {
	return stiva == NULL;
}

void afisareStiva(Stiva* stiva) {
	while (stiva != NULL) {
		afisareMasina(stiva->info);
		stiva = stiva->next;
	}
}

void dezalocareStiva(Stiva** stiva) {
	while (isEmptyStack(*stiva) == 0) {
		Masina m = popStack(stiva);
		free(m.model);
		free(m.numeSofer);
	}
}

struct Nod {
	Masina info;
	struct Nod* next;
	struct Nod* prev;
};
typedef struct Nod Nod;

struct Coada {
	Nod* first;
	Nod* last;
};
typedef struct Coada Coada;

void enqueue(Coada* coada, Masina masina) {
	
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masina;
	nou->next = coada->first;
	nou->prev = NULL;
	if (coada->first == NULL) {
		coada->first = nou;
		coada->last = nou;
	}
	else {
		coada->first->prev = nou;
		coada->first = nou;
	}
}

Masina dequeue(Coada* coada) {
	Masina m;
	if (coada->last == NULL) {
		m.id = -1;
		return m;
	}
	else {
		m = coada->last->info;
		if (coada->last->prev == NULL) {
			coada->first = NULL;
			coada->last = NULL;
		}
		else {
			Nod* ultim = coada->last;
			coada->last = ultim->prev;
			ultim->prev->next = NULL;
			free(ultim);
		}
		return m;
	}
}

Coada citireCoada(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Coada cap;
	cap.first = NULL;
	cap.last = NULL;

	while (!feof(f)) {
		enqueue(&cap, citireMasinaDinFisier(f));
	}
	fclose(f);
	return cap;
}

void afisareCoada(Coada coada) {
	while (coada.last != NULL) {
		afisareMasina(coada.last->info);
		coada.last = coada.last->prev;
	}
}

void dezalocareCoada(Coada* coada) {
	while (coada->first != NULL) {
		Masina m = dequeue(coada);
		free(m.model);
		free(m.numeSofer);
	}
	coada->first = NULL;
	coada->last = NULL;
}

int main() {
	//Stiva* stiva = citireStiva("masini.txt");
	//afisareStiva(stiva);
	//printf("%d\n", isEmptyStack(stiva));
	//dezalocareStiva(&stiva);
	//printf("%d\n", isEmptyStack(stiva));


	Coada coada = citireCoada("masini.txt");
	afisareCoada(coada);

	dezalocareCoada(&coada);
}