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

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m;
	aux = strtok(buffer, sep);
	m.id = atoi(aux);
	m.nrUsi = atoi(strtok(NULL, sep));
	m.pret = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	m.model = malloc(strlen(aux) + 1);
	strcpy_s(m.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m.numeSofer, strlen(aux) + 1, aux);

	m.serie = *strtok(NULL, sep);

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

int calculeazaInaltimeArbore(Nod* radacina) {
	if (radacina != NULL) {
		int inaltimeSt = calculeazaInaltimeArbore(radacina->st);
		int inaltimeDr = calculeazaInaltimeArbore(radacina->dr);

		if (inaltimeDr > inaltimeSt) {
			return 1 + inaltimeDr;
		}
		else {
			return 1 + inaltimeSt;
		}
	}
	else return 0;
}

int calculeazaGE(Nod* radacina) {
	return calculeazaInaltimeArbore(radacina->st) - calculeazaInaltimeArbore(radacina->dr);
}

void rotireStanga(Nod** radacina) {
	Nod* aux = (*radacina)->dr;
	(*radacina)->dr = aux->st;
	aux->st = (*radacina);
	(*radacina) = aux;
}

void rotireDreapta(Nod** radacina) {
	Nod* aux = (*radacina)->st;
	(*radacina)->st = aux->dr;
	aux->dr = (*radacina);
	(*radacina) = aux;
}

void adaugaMasinaInArboreEchilibrat(Nod** radacina, Masina masinaNoua) {
	if ((*radacina) != NULL) {
		if ((*radacina)->info.id < masinaNoua.id) {
			adaugaMasinaInArboreEchilibrat(&((*radacina)->dr), masinaNoua);
		}
		else {
			adaugaMasinaInArboreEchilibrat(&((*radacina)->st), masinaNoua);
		}
		int gradEchilibru = calculeazaGE(*radacina);
		if (gradEchilibru == 2) {
			if (calculeazaGE((*radacina)->st) == 1) {
				rotireDreapta(radacina);
			}
			else {
				rotireStanga(&((*radacina)->st));
				rotireDreapta(radacina);
			}
		}
		else if (gradEchilibru == -2) {
			if (calculeazaGE((*radacina)->dr) != -1) {
				rotireDreapta(&((*radacina)->dr));
			}
			rotireStanga(radacina);
		}
	}
	else {
		(*radacina) = (Nod*)malloc(sizeof(Nod));
		(*radacina)->info = masinaNoua;
		(*radacina)->st = NULL;
		(*radacina)->dr = NULL;
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* radacina = NULL;
	while (!feof(f)) {
		adaugaMasinaInArboreEchilibrat(&radacina, citireMasinaDinFisier(f));
	}
	fclose(f);
	return radacina;
}

void afisarePreOrdineRSD(Nod* radacina) {
	if (radacina != NULL) {
		afisareMasina(radacina->info);
		afisarePreOrdineRSD(radacina->st);
		afisarePreOrdineRSD(radacina->dr);
	}
}

void afisareInOrdineSRD(Nod* radacina) {
	if (radacina != NULL) {
		afisareInOrdineSRD(radacina->st);
		afisareMasina(radacina->info);
		afisareInOrdineSRD(radacina->dr);
	}
}

void afisareInOrdineSDR(Nod* radacina) {
	if (radacina != NULL) {
		afisareInOrdineSDR(radacina->st);
		afisareInOrdineSDR(radacina->dr);
		afisareMasina(radacina->info);
	}
}

void dezalocareArbore(Nod** radacina) {
	if ((*radacina) != NULL) {
		dezalocareArbore(&(*radacina)->st);
		dezalocareArbore(&(*radacina)->dr);
		free((*radacina)->info.model);
		free((*radacina)->info.numeSofer);
		free(*radacina);
		(*radacina) = NULL;
	}
}

Masina getMasinaById(Nod* radacina, int id) {
	
	Masina m;
	m.id = -1;
	if (radacina != NULL) {
		if (id < radacina->info.id) {
			return getMasinaById(radacina->st, id);
		}
		else {
			if (id > radacina->info.id) {
				return getMasinaById(radacina->dr, id);
			}
			else {
				return radacina->info;
			}
		}
	}
	return m;
}

int determinaNrNoduri(Nod* nod) {
	if (nod == NULL)
		return 0;
	return 1 + determinaNrNoduri(nod->st) + determinaNrNoduri(nod->dr);
}

float calculeazaPretTotal(Nod* nod){
	if (nod == NULL)
		return 0;

	return nod->info.pret + calculeazaPretTotal(nod->st) + calculeazaPretTotal(nod->dr);
}

float calculeazaPretulMasinilorUnuiSofer(Nod* nod, const char* numeSofer) {
	if (nod == NULL) {
		return 0;
	}
	float suma = 0;
	if (strcmp(nod->info.numeSofer, numeSofer) == 0) {
		suma = +nod->info.pret;
	}
	suma += calculeazaPretulMasinilorUnuiSofer(nod->st, numeSofer);
	suma += calculeazaPretulMasinilorUnuiSofer(nod->dr, numeSofer);

	return suma;
}

int main()
{
	Nod* nod = citireArboreDeMasiniDinFisier("masini.txt");
	//afisareMasiniDinArboreRSD(nod);

	Masina masina = getMasinaById(nod, 6);
	printf("\n===========Masina cautata: ===========\n");
	afisareMasina(masina);

	int nrNoduri = determinaNrNoduri(nod);
	printf("\n=========== Nr. noduri: %d ===========\n", nrNoduri);

	float pretTotal = calculeazaPretTotal(nod);
	printf("\n=========== Pret total: %.2f ===========\n", pretTotal);

	float pretTotalSofer = calculeazaPretulMasinilorUnuiSofer(nod, "Gigel");
	printf("\n=========== Pret total pentru Gigel: %.2f ===========\n", pretTotalSofer);

	dezalocareArbore(&nod);
	return 0;
}