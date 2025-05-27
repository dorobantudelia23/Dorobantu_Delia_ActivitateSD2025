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

struct Heap {
	int lungime;
	int nrElemente;
	Masina* masini;
};
typedef struct Heap Heap;

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
	printf("Nr. usi: %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.masini = (Masina*)malloc(sizeof(Masina) * lungime);
	heap.nrElemente = 0;
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int pozFiuSt = pozitieNod * 2 + 1;
	int pozFiuDr = pozitieNod * 2 + 2;
	int pozMax = pozitieNod;
	if (pozFiuSt < heap.nrElemente && heap.masini[pozMax].pret < heap.masini[pozFiuSt].pret) {
		pozMax = pozFiuSt;
	}
	if (pozFiuDr < heap.nrElemente && heap.masini[pozMax].pret < heap.masini[pozFiuDr].pret) {
		pozMax = pozFiuDr;
	}
	if (pozMax != pozitieNod) {
		Masina aux = heap.masini[pozMax];
		heap.masini[pozMax] = heap.masini[pozitieNod];
		heap.masini[pozitieNod] = aux;
		if (pozMax <= (heap.nrElemente - 2) / 2) {
			filtreazaHeap(heap, pozMax);
		}
	}

}

Heap citireHeapDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Heap heap = initializareHeap(10);
	while (!feof(file)) {
		heap.masini[heap.nrElemente++] = citireMasinaDinFisier(file);
	}
	fclose(file);
	for (int i = (heap.nrElemente - 2) / 2; i >= 0;i--) {
		filtreazaHeap(heap, i);
	}
	return heap;
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrElemente;i++) {
		afisareMasina(heap.masini[i]);
	}
}

void afisareHeapAscuns(Heap heap) {
	for (int i = heap.nrElemente;i < heap.lungime;i++) {
		afisareMasina(heap.masini[i]);
	}
}

Masina extrageMasina(Heap* heap) {
	if (heap->nrElemente > 0) {
		Masina aux = heap->masini[0];
		heap->masini[0] = heap->masini[heap->nrElemente - 1];
		heap->masini[heap->nrElemente - 1] = aux;
		heap->nrElemente--;
		for (int i = (heap->nrElemente - 2) / 2;i >= 0;i--) {
			filtreazaHeap(*heap, i);
		}
		return aux;
	}
}

void dezalocare(Heap* heap) {
	for (int i = 0; i < heap->lungime;i++) {
		free(heap->masini[i].model);
		free(heap->masini[i].numeSofer);
	}
	free(heap->masini);
	heap->masini = NULL;
	heap->nrElemente = 0;
	heap->lungime = 0;
}

int main() {
	Heap heap = citireHeapDinFisier("masini.txt");
	//afisareHeap(heap);
	printf("Masini extrase: ");
	for (int i = 0; i < heap.lungime;i++) {
		afisareMasina(extrageMasina(&heap));
	}
}