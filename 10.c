#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256
#define MAX_NAME 128

// Struktura za čvor stabla gradova
typedef struct Grad {
    char naziv[MAX_NAME];
    int brojStanovnika;
    struct Grad* lijevo;
    struct Grad* desno;
} Grad;

// Struktura za čvor vezane liste država
typedef struct Drzava {
    char naziv[MAX_NAME];
    Grad* gradovi;
    struct Drzava* sljedeca;
} Drzava;

// Funkcija za kreiranje novog čvora za grad
Grad* noviGrad(char* naziv, int brojStanovnika) {
    Grad* novi = (Grad*)malloc(sizeof(Grad));
    if (!novi) {
        perror("Greska pri alokaciji memorije za grad.");
        return NULL;
    }
    strcpy(novi->naziv, naziv);
    novi->brojStanovnika = brojStanovnika;
    novi->lijevo = NULL;
    novi->desno = NULL;
    return novi;
}

// Funkcija za dodavanje grada u stablo gradova
Grad* dodajGrad(Grad* korijen, char* naziv, int brojStanovnika) {
    if (korijen == NULL)
        return noviGrad(naziv, brojStanovnika);

    if (brojStanovnika < korijen->brojStanovnika || (brojStanovnika == korijen->brojStanovnika && strcmp(naziv, korijen->naziv) < 0)) {
        korijen->lijevo = dodajGrad(korijen->lijevo, naziv, brojStanovnika);
    }
    else {
        korijen->desno = dodajGrad(korijen->desno, naziv, brojStanovnika);
    }

    return korijen;
}

// Funkcija za kreiranje novog čvora za državu
Drzava* novaDrzava(char* naziv) {
    Drzava* nova = (Drzava*)malloc(sizeof(Drzava));
    if (!nova) {
        perror("Greska pri alokaciji memorije za drzavu.");
        return NULL;
    }
    strcpy(nova->naziv, naziv);
    nova->gradovi = NULL;
    nova->sljedeca = NULL;
    return nova;
}

// Funkcija za dodavanje države u sortiranu vezanu listu
Drzava* dodajDrzavu(Drzava* glava, char* naziv) {
    Drzava* nova = novaDrzava(naziv);
    if (!nova)
        return glava;

    if (!glava || strcmp(naziv, glava->naziv) < 0) {
        nova->sljedeca = glava;
        return nova;
    }

    Drzava* trenutna = glava;
    while (trenutna->sljedeca && strcmp(naziv, trenutna->sljedeca->naziv) > 0) {
        trenutna = trenutna->sljedeca;
    }

    nova->sljedeca = trenutna->sljedeca;
    trenutna->sljedeca = nova;
    return glava;
}

// Funkcija za učitavanje gradova iz datoteke i dodavanje u stablo
Grad* ucitajGradove(char* imeDatoteke) {
    FILE* fp = fopen(imeDatoteke, "r");
    if (!fp) {
        perror("Greska pri otvaranju datoteke s gradovima.");
        return NULL;
    }

    Grad* korijen = NULL;
    char linija[MAX_LINE];
    char nazivGrada[MAX_NAME];
    int brojStanovnika;

    while (fgets(linija, MAX_LINE, fp)) {
        if (sscanf(linija, "%[^,], %d", nazivGrada, &brojStanovnika) == 2) {
            korijen = dodajGrad(korijen, nazivGrada, brojStanovnika);
        }
    }

    fclose(fp);
    return korijen;
}

// Funkcija za učitavanje država iz glavne datoteke
Drzava* ucitajDrzave(char* imeDatoteke) {
    FILE* fp = fopen(imeDatoteke, "r");
    if (!fp) {
        perror("Greska pri otvaranju glavne datoteke.");
        return NULL;
    }

    Drzava* glava = NULL;
    char linija[MAX_LINE];
    char nazivDrzave[MAX_NAME];
    char datotekaGradova[MAX_NAME];

    while (fgets(linija, MAX_LINE, fp)) {
        if (sscanf(linija, "%[^,], %s", nazivDrzave, datotekaGradova) == 2) {
            Drzava* drzava = novaDrzava(nazivDrzave);
            drzava->gradovi = ucitajGradove(datotekaGradova);
            glava = dodajDrzavu(glava, nazivDrzave);
        }
    }

    fclose(fp);
    return glava;
}

// Funkcija za ispis stabla gradova
void ispisiGradove(Grad* korijen) {
    if (!korijen)
        return;

    ispisiGradove(korijen->lijevo);
    printf("\t%s (%d)\n", korijen->naziv, korijen->brojStanovnika);
    ispisiGradove(korijen->desno);
}

// Funkcija za ispis vezane liste država
void ispisiDrzave(Drzava* glava) {
    while (glava) {
        printf("%s:\n", glava->naziv);
        ispisiGradove(glava->gradovi);
        glava = glava->sljedeca;
    }
}

// Funkcija za pretragu gradova s većim brojem stanovnika
void pretraziGradove(Grad* korijen, int prag) {
    if (!korijen)
        return;

    pretraziGradove(korijen->lijevo, prag);
    if (korijen->brojStanovnika > prag)
        printf("\t%s (%d)\n", korijen->naziv, korijen->brojStanovnika);
    pretraziGradove(korijen->desno, prag);
}

// Funkcija za pronalaženje države
Drzava* pronadjiDrzavu(Drzava* glava, char* naziv) {
    while (glava && strcmp(glava->naziv, naziv) != 0) {
        glava = glava->sljedeca;
    }
    return glava;
}

int main() {
    char imeDatoteke[MAX_NAME] = "drzave.txt";
    Drzava* drzave = ucitajDrzave(imeDatoteke);

    if (!drzave) {
        printf("Greska pri ucitavanju drzava.\n");
        return 1;
    }

    printf("Ucitane drzave i gradovi:\n");
    ispisiDrzave(drzave);

    char trazenaDrzava[MAX_NAME];
    int prag;

    printf("\nUnesite naziv drzave za pretragu: ");
    scanf("%s", trazenaDrzava);

    printf("Unesite minimalan broj stanovnika: ");
    scanf("%d", &prag);

    Drzava* drzava = pronadjiDrzavu(drzave, trazenaDrzava);
    if (drzava) {
        printf("Gradovi u drzavi %s s vise od %d stanovnika:\n", trazenaDrzava, prag);
        pretraziGradove(drzava->gradovi, prag);
    }
    else {
        printf("Drzava %s nije pronadjena.\n", trazenaDrzava);
    }

    return 0;
}