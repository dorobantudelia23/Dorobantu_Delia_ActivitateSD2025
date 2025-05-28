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

struct Nod {
	Masina info;
	struct Nod* st;
	struct Nod* dr;
};
typedef struct Nod Nod;

Masina citesteMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	Masina m;
	char* aux = strtok(buffer, sep);
	m.id = atoi(aux);
	m.nrUsi = atoi(strtok(NULL, sep));
	m.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m.model = malloc(strlen(aux) + 1);
	strcpy_s(m.model, strlen(aux) + 1, aux);
	aux = strtok(NULL, sep);
	m.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m.numeSofer, strlen(aux) + 1, aux);
	m.serie = *(strtok(NULL, sep));

	return m;
}


void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

int calculeazaInaltime(Nod* nod) {
	if (nod != NULL) {
		int inaltimeSt = calculeazaInaltime(nod->st);
		int inaltimeDr = calculeazaInaltime(nod->dr);

		if (inaltimeDr > inaltimeSt) {
			return 1 + inaltimeDr;
		}
		else {
			return 1 + inaltimeSt;
		}
	}else{
		return 0;
	}
}

int calculeazaGE(Nod* nod) {
	return calculeazaInaltime(nod->st) - calculeazaInaltime(nod->dr);
}

void rotireDreapta(Nod** nod) {
	Nod* aux = (*nod)->st;
	(*nod)->st = aux->dr;
	aux->dr = (*nod);
	(*nod) = aux;
}

void rotireStanga(Nod** nod) {
	Nod* aux = (*nod)->dr;
	(*nod)->dr = aux->st;
	aux->st = (*nod);
	(*nod) = aux;
}

void inserareInArbore(Nod** nod, Masina masinaNoua) {
	if ((*nod) != NULL) {
		if (masinaNoua.id < (*nod)->info.id) {
			inserareInArbore(&(*nod)->st, masinaNoua);
		}
		else if (masinaNoua.id > (*nod)->info.id) {
			inserareInArbore(&(*nod)->dr, masinaNoua);
		}
		int echilibru = calculeazaGE(*nod);
		if (echilibru == 2) {
			if (calculeazaGE((*nod)->st) == -1) {
				rotireStanga(&(*nod)->st);
			}
			rotireDreapta(nod);
		}
		if (echilibru == -2) {
			if (calculeazaGE((*nod)->dr) == 1) {
				rotireDreapta(&(*nod)->dr);
			}
			rotireStanga(nod);
		}
	}
	else {
		(*nod) = (Nod*)malloc(sizeof(Nod));
		(*nod)->info = masinaNoua;
		(*nod)->st = NULL;
		(*nod)->dr = NULL;
	}
}

Nod* citireArboreDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* nod = NULL;
	while (!feof(f)) {
		inserareInArbore(&nod, citesteMasinaDinFisier(f));
	}
	fclose(f);
	return nod;
}

void afisareArboreInPreordineRSD(Nod* nod) {
	if (nod != NULL) {

	afisareMasina(nod->info);
	afisareArboreInPreordineRSD(nod->st);
	afisareArboreInPreordineRSD(nod->dr);
	}

}

void dezalocare(Nod** nod) {
	if ((*nod) != NULL) {
		dezalocare(&(*nod)->st);
		dezalocare(&(*nod)->dr);
		free((*nod)->info.model);
		free((*nod)->info.numeSofer);
		free(*nod);
	}
	(*nod) = NULL;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* nod, const char* numeSofer) {
	if (nod == NULL) {
		return 0;
	}
	float s = 0;
	if (strcmp(nod->info.numeSofer, numeSofer) == 0) {
		s += nod->info.pret;
		}
	s += calculeazaPretulMasinilorUnuiSofer(nod->st, numeSofer);
	s += calculeazaPretulMasinilorUnuiSofer(nod->dr, numeSofer);

	return s;
}

int main() {
	Nod* arbore = citireArboreDinFisier("masini_arbore.txt");
	afisareArboreInPreordineRSD(arbore);

	printf("%d %d\n\n", calculeazaInaltime(arbore->st), calculeazaInaltime(arbore->dr));

	printf("%.2f", calculeazaPretulMasinilorUnuiSofer(arbore, "Gheorghe"));

	dezalocare(&arbore);
}