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

//1. 
// structuri necesare
//dorim stocarea unui graf intr-o lista de liste
//astfel avem nod ListaPrincipala si NodListaSecundara

typedef struct NodLS NodLS;
typedef struct NodLP NodLP;


struct NodLS {
	NodLP* nodPrincipal;
	NodLS* next;
};


struct NodLP {
	Masina info;
	NodLS* vecini;
	NodLP* next;
};


//2.
//functii de inserare in liste
//si in principala si in secundara

void inserareInLPLaSfarsit(NodLP** cap, Masina masinaNoua) {
	NodLP* nou = (NodLP*)malloc(sizeof(NodLP));

	nou->info = masinaNoua;
	nou->next = NULL;
	nou->vecini = NULL;

	if ((*cap) == NULL) {
		(*cap) = nou;
	}
	else {
		NodLP* temp = (*cap);

		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
	}

}

void inserareInLSLaInceput(NodLS** cap, NodLP* nodPrincipal) {
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	nou->nodPrincipal = nodPrincipal;
	nou->next = (*cap);
	(*cap) = nou;
}

//3.
//functie de cautarea in lista principala dupa ID
NodLP* cautaNodDupaID(NodLP* cap, int id) {
	while (cap != NULL && cap->info.id != NULL ) {
		cap = cap->next;
	}
	return cap;


}

//4.
//inserare muchie
void inserareMuchie(NodLP* listaPrincipala, int idStart, int idStop) {
	NodLP* nodStart = cautaNodDupaID(listaPrincipala, idStart);
	NodLP* nodStop = cautaNodDupaID(listaPrincipala, idStop);
	if (nodStart != NULL && nodStop != NULL) {
		inserareInLSLaInceput(&(nodStart->vecini), nodStop);

		//urmatoarea linie este doar pt graf NEORIENTAT
		inserareInLSLaInceput(&(nodStop->vecini), nodStart);
	}

}


NodLP* citireNoduriMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul

	FILE* f = fopen(numeFisier, "r");
	NodLP* cap = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		inserareInLPLaSfarsit(&cap, m);
	}
	fclose(f);

	return cap;
}

void citireMuchiiDinFisier(NodLP* cap, const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste 
	//toate id-urile de start si stop pentru fiecare muchie
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");
	int idStart, idStop;
	while (!feof(f)) {
		fscanf(f, "&d %d", &idStart, &idStop);
		inserareMuchie(cap, idStart, idStop);
	}
	fclose(f);

}

void dezalocareListaAdiacenta(NodLS* cap) {
	while (cap != NULL) {
		NodLS* temp = cap->next;
		free(cap);
		cap = temp;
	}
}

void dezalocareNoduriGraf(NodLP** cap) {
	//sunt dezalocate toate masinile din graf 
	//si toate nodurile celor doua liste
	while ((*cap) != NULL) {
		NodLP* temp = (*cap)->next;
		dezalocareListaAdiacenta((*cap)->vecini);
		free((*cap)->info.model);
		free((*cap)->info.numeSofer);
		(*cap)->vecini = NULL;
		free((*cap));
		(*cap) = temp;
	}

}

NodLP* citesteGrafMasini(const char* fisierMasini, const char* fisierMuchii) {
	NodLP* graf = citireNoduriMasiniDinFisier(fisierMasini); 
	citireMuchiiDinFisier(graf, fisierMuchii);
	return graf;
}

int main() {
	NodLP* graf = citesteGrafMasini("masini.txt", "muchii.txt");

	dezalocareNoduriGraf(&graf);

	return 0;
}