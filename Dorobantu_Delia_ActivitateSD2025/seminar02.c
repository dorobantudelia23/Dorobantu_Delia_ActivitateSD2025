#include<stdio.h>
#include<malloc.h>

//struct Sablon {
//	int id;
//	//int ;
//	//char* ;
//	//float ;
//	//char ;
//};
//struct Sablon initializare(/*parametri necesari*/) {
//	struct Sablon s;
//	s.id = 1;
//	//initializare structura 
//	return s;
//}
//
//void afisare(struct Sablon s) {
//	//afisarea tuturor atributelor.
//}

struct Masina {
	int id;
	int nrKm;
	char* model;
	float pret;
	char tipMotor;
};

struct Masina initializare(int id, int nrKm, const char* model, float pret, char tipMotor) {
	struct Masina m;
	m.id = id;
	m.nrKm = nrKm;
	m.model = (char*)malloc(strlen(model) + 1);
	strcpy(m.model, model);
	m.pret = pret;
	m.tipMotor = tipMotor;
	return m;
}

void afisare(struct Masina m) {
	printf("Id: %d, nr km: %d, model: %s, pret: %f, tip motor: %c", m.id, m.nrKm, m.model, m.pret, m.tipMotor);

}

void afisareVector(struct Masina* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++) {
		afisare(vector[i]);
		printf("\n");
	}
}

struct Masina* copiazaPrimeleNElemente(struct Masina* vector, int nrElemente, int* nrElementeCopiate) {
	struct Masina* vectorNou; 
	vectorNou = NULL;
	if (vector != NULL && (*nrElementeCopiate) > 0) {

		if (nrElemente < *nrElementeCopiate) {
			*nrElementeCopiate = nrElemente;
		}

		vectorNou = malloc(sizeof(struct Masina) * (*nrElementeCopiate)); 


		for (int i = 0; i < *nrElementeCopiate; i++) {
			vectorNou[i] = initializare(vector[i].id, vector[i].nrKm, vector[i].model, vector[i].pret, vector[i].tipMotor); // -> dereferentiere si accesare
		}
	}
	return vectorNou;
}

void dezalocare(struct Masina** vector, int* nrElemente) {
	for (int i = 0; i < (*nrElemente); i++) {
		if ((*vector)[i].model != NULL) {
			free((*vector)[i].model); 

		}
	}
	free(*vector);
	*vector = NULL;
	*nrElemente = 0;
}

void copiazaMasiniIeftine(struct Masina* vector, char nrElemente, float prag, struct Masina** vectorNou, int* dimensiune) {

	*dimensiune = 0;

	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].pret < prag) {
			(*dimensiune) += 1;
		}
	}

	int k = 0;
	*vectorNou = malloc(sizeof(struct Masina) * (*dimensiune));
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].pret < prag) {
			(*vectorNou)[k] = vector[i];
			(*vectorNou)[k].model = malloc(sizeof(char) * strlen(vector[i].model) + 1);
			strcpy_s((*vectorNou)[k].model, (strlen(vector[i].model) + 1), vector[i].model);
			k += 1;
		}
	}

}

struct Masina getPrimulElementConditionat(struct Masina* vector, int nrElemente, const char* conditie) {

	struct Masina s;
	s.id = 1;

	return s;
}



int main() {

	int n = 3;
	struct Masina* vec;
	vec = malloc(sizeof(struct Masina) * 3);
	vec[0] = initializare(10, 100000, "Logan", 1000.4, 'D');
	vec[1] = initializare(11, 1000, "Passat", 2500.4, 'B');
	vec[2] = initializare(12, 25464, "CyberTruck", 1000.0, 'E');
	afisareVector(vec, n);

	int nrElemCopiat = 5;
	struct Masina* vecNou = copiazaPrimeleNElemente(vec, n, &nrElemCopiat);
	printf("Elementele copiate:\n");
	afisareVector(vec, nrElemCopiat);

	printf("Masini ieftine: \n");
	dezalocare(&vecNou, &nrElemCopiat);
	copiazaMasiniIeftine(vec, n, 1500, &vecNou, &nrElemCopiat);
	afisareVector(vecNou, nrElemCopiat);

	dezalocare(&vecNou, &nrElemCopiat);
	dezalocare(&vec, &n);
	return 0;

}