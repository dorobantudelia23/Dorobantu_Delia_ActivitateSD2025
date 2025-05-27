#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include<string.h>

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
	struct Nod* dr;
	struct Nod* st;
};
typedef struct Nod Nod;

Masina citireMasinaDinfisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n" ;
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

void afisareMasina(Masina m) {
	printf("Id: %d\n", m.id);
	printf("Nr usi: %d\n", m.nrUsi);
	printf("Pret: %.2f\n", m.pret);
	printf("Model: %s\n", m.model);
	printf("Nume sofer: %s\n", m.numeSofer);
	printf("Serie: %c\n\n", m.serie);
}

void adaugaMasinaInArbore(Nod** nod, Masina masinaNoua) {
	if ((*nod) != 0) {
		if (masinaNoua.id > (*nod)->info.id) {
			adaugaMasinaInArbore(&(*nod)->dr, masinaNoua);
		}
		else if (masinaNoua.id < (*nod)->info.id) {
			adaugaMasinaInArbore(&(*nod)->st, masinaNoua);
		}
	}
	else {
		(*nod) = (Nod*)malloc(sizeof(Nod));
		(*nod)->info = masinaNoua;
		(*nod)->st = NULL;
		(*nod)->dr = NULL;
	}
}

void* citireArboreDinFisier(const char* numefisier) {
	FILE* f = fopen(numefisier, "r");
	Nod* nod = NULL;
	while (!feof(f)) {
		adaugaMasinaInArbore(&nod, citireMasinaDinfisier(f));
	}
	fclose(f);
	return nod;
}

void afisareArborePreOrdineRSD(Nod* nod) {
	if (nod != NULL) {
		afisareMasina(nod->info);
		afisareArborePreOrdineRSD(nod->st);
		afisareArborePreOrdineRSD(nod->dr);
	}
}

void afisareArboreInOrdineSRD(Nod* nod) {
	if (nod != NULL) {
		afisareArboreInOrdineSRD(nod->st);
		afisareMasina(nod->info);
		afisareArboreInOrdineSRD(nod->dr);
	}
}

void afisareArboreInOrdineSDR(Nod* nod) {
	if(nod != NULL){
		afisareArboreInOrdineSDR(nod->st);
		afisareArboreInOrdineSDR(nod->dr);
		afisareMasina(nod->info);
	}
}

void dezalocareArbore(Nod** nod) {
	if ((*nod) != NULL) {
		dezalocareArbore(&(*nod)->st);
		dezalocareArbore(&(*nod)->dr);

		free((*nod)->info.model);
		free((*nod)->info.numeSofer);

		free(*nod);
		(*nod) = NULL;
	}
}

Masina getMasinaById(Nod* radacina, int id) {
	Masina m;
	m.id = -1;
	if (radacina != 0) {
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

int determinaNumarNoduri(Nod* radacina) {
	int nrNoduri = 0;
	if (radacina != NULL) {
		nrNoduri += determinaNumarNoduri(radacina->st);
		nrNoduri += determinaNumarNoduri(radacina->dr);
		nrNoduri += 1;
	}
	return nrNoduri;
}

int calculeazaInaltimeArbore(Nod* radacina) {
	
	if (radacina != NULL) {
		int inaltimeSt = calculeazaInaltimeArbore(radacina->st);
		int inaltimeDr = calculeazaInaltimeArbore(radacina->dr);
		if (inaltimeDr > inaltimeSt) {
			return inaltimeDr + 1;
		}
		else {
			return inaltimeSt + 1;
		}
	}
	else {
		return 0;
	}
}

float calculeazaPretTotal(Nod* radacina) {
	if (radacina == NULL)
		return 0;
	//return radacina->info.pret + calculeazaPretTotal(radacina->st) + calculeazaPretTotal(radacina->dr);

	float totalSt = calculeazaPretTotal(radacina->st);
	float totalDr = calculeazaPretTotal(radacina->dr);
	return radacina->info.pret + totalDr + totalSt;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* radacina, const char* numeSofer) {
	if (radacina == NULL) {
		return 0;
	}
	float suma = 0;
	if (strcmp(radacina->info.numeSofer, numeSofer) == 0) {
		suma = +radacina->info.pret;
	}
	suma += calculeazaPretulMasinilorUnuiSofer(radacina->st, numeSofer);
	suma += calculeazaPretulMasinilorUnuiSofer(radacina->dr, numeSofer);

	return suma;
}

int main() {
	Nod* radacina = citireArboreDinFisier("masini_arbore.txt");
	afisareArborePreOrdineRSD(radacina);
	printf("\n=============================================================================\n");
	afisareArboreInOrdineSRD(radacina);
	printf("\n=============================================================================\n");
	afisareMasina( getMasinaById(radacina, 6));

	printf("nr noduri: %d\n", determinaNumarNoduri(radacina));
	printf("inaltime arbore: %d\n", calculeazaInaltimeArbore(radacina));
	printf("pret total: %.2f\n", calculeazaPretTotal(radacina));
	printf("pret total al soferului Ionescu: %.2f\n", calculeazaPretulMasinilorUnuiSofer(radacina,"Ionescu"));
	
	dezalocareArbore(&radacina);


}