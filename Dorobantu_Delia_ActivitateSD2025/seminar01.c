#include <stdio.h>
#include <stdlib.h>

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
	m.model = (char*)malloc(sizeof(model) + 1);
	strcpy_s(m.model, sizeof(model) + 1, model);
	m.pret = pret;
	m.tipMotor = tipMotor;

	return m;
}

void afisare(struct Masina m) {
	printf("\nId: %d \nNr. Km: %d \nModel: %s \nPret: %2.4f \nTip Motor: %c\n", m.id, m.nrKm, m.model, m.pret, m.tipMotor);
}

void modificaPret(struct Masina* m, float pretNou) {
	if (pretNou > 0)
		m->pret = pretNou;
}

void dezalocare(struct Masina* m) {
	free(m->model);
	m->model = NULL;
}

int main() {
	struct Masina masina;
	masina = initializare(1, 120000, "Ford", 10000, 'B');
	afisare(masina);
	modificaPret(&masina, 9000);
	afisare(masina);
	dezalocare(&masina);
	afisare(masina);

	return 0;
}