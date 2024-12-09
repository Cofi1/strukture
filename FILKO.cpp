#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struktura za artikl
typedef struct Artikl {
    char naziv[50];
    int kolicina;
    float cijena;
    struct Artikl* sljedeci;
} Artikl;

// Struktura za račun
typedef struct Racun {
    char datum[11]; // Format YYYY-MM-DD
    Artikl* artikli;
    struct Racun* sljedeci;
} Racun;

// Funkcija za kreiranje novog artikla
Artikl* noviArtikl(char* naziv, int kolicina, float cijena) {
    Artikl* artikl = (Artikl*)malloc(sizeof(Artikl));
    strcpy(artikl->naziv, naziv);
    artikl->kolicina = kolicina;
    artikl->cijena = cijena;
    artikl->sljedeci = NULL;
    return artikl;
}

// Funkcija za dodavanje artikla u sortiranu listu
Artikl* dodajArtiklSortirano(Artikl* glava, Artikl* novi) {
    if (!glava || strcmp(novi->naziv, glava->naziv) < 0) {
        novi->sljedeci = glava;
        return novi;
    }
    Artikl* trenutni = glava;
    while (trenutni->sljedeci && strcmp(novi->naziv, trenutni->sljedeci->naziv) > 0) {
        trenutni = trenutni->sljedeci;
    }
    novi->sljedeci = trenutni->sljedeci;
    trenutni->sljedeci = novi;
    return glava;
}

// Funkcija za spajanje duplikata artikala
Artikl* spojiDuplikate(Artikl* glava) {
    Artikl* trenutni = glava;
    while (trenutni && trenutni->sljedeci) {
        if (strcmp(trenutni->naziv, trenutni->sljedeci->naziv) == 0) {
            Artikl* duplikat = trenutni->sljedeci;
            trenutni->kolicina += duplikat->kolicina;
            trenutni->cijena += duplikat->cijena;
            trenutni->sljedeci = duplikat->sljedeci;
            free(duplikat);
        }
        else {
            trenutni = trenutni->sljedeci;
        }
    }
    return glava;
}

// Funkcija za kreiranje novog računa
Racun* noviRacun(char* datum) {
    Racun* racun = (Racun*)malloc(sizeof(Racun));
    strcpy(racun->datum, datum);
    racun->artikli = NULL;
    racun->sljedeci = NULL;
    return racun;
}

// Funkcija za dodavanje računa u sortiranu listu
Racun* dodajRacunSortirano(Racun* glava, Racun* novi) {
    if (!glava || strcmp(novi->datum, glava->datum) < 0) {
        novi->sljedeci = glava;
        return novi;
    }
    Racun* trenutni = glava;
    while (trenutni->sljedeci && strcmp(novi->datum, trenutni->sljedeci->datum) > 0) {
        trenutni = trenutni->sljedeci;
    }
    novi->sljedeci = trenutni->sljedeci;
    trenutni->sljedeci = novi;
    return glava;
}

// Funkcija za učitavanje računa iz datoteke
Racun* ucitajRacune(const char* nazivDatoteke) {
    FILE* file = fopen(nazivDatoteke, "r");
    if (!file) {
        printf("Greska pri otvaranju datoteke %s!\n", nazivDatoteke);
        return NULL;
    }

    Racun* racuni = NULL;
    char nazivRacuna[50];
    while (fscanf(file, "%s", nazivRacuna) == 1) {
        FILE* racunFile = fopen(nazivRacuna, "r");
        if (!racunFile) {
            printf("Greska pri otvaranju datoteke %s!\n", nazivRacuna);
            continue;
        }

        char datum[11];
        fscanf(racunFile, "%10s", datum);

        Racun* novi = noviRacun(datum);
        char nazivArtikla[50];
        int kolicina;
        float cijena;
        while (fscanf(racunFile, "%[^,],%d,%f\n", nazivArtikla, &kolicina, &cijena) == 3) {
            Artikl* artikl = noviArtikl(nazivArtikla, kolicina, cijena);
            novi->artikli = dodajArtiklSortirano(novi->artikli, artikl);
        }

        fclose(racunFile);

        // Spajanje duplikata artikala
        novi->artikli = spojiDuplikate(novi->artikli);
        racuni = dodajRacunSortirano(racuni, novi);
    }

    fclose(file);
    return racuni;
}

// Funkcija za izračun ukupne potrošnje i količine za artikl
void izracunajPotrosnju(Racun* racuni, const char* nazivArtikla, const char* datumOd, const char* datumDo) {
    float ukupnaCijena = 0;
    int ukupnaKolicina = 0;

    while (racuni) {
        if (strcmp(racuni->datum, datumOd) >= 0 && strcmp(racuni->datum, datumDo) <= 0) {
            Artikl* artikli = racuni->artikli;
            while (artikli) {
                if (strcmp(artikli->naziv, nazivArtikla) == 0) {
                    ukupnaCijena += artikli->kolicina * artikli->cijena;
                    ukupnaKolicina += artikli->kolicina;
                }
                artikli = artikli->sljedeci;
            }
        }
        racuni = racuni->sljedeci;
    }

    printf("Artikl: %s\nUkupno potroseno: %.2f\nUkupno kupljeno: %d\n",
        nazivArtikla, ukupnaCijena, ukupnaKolicina);
}

// Glavna funkcija
int main() {
    Racun* racuni = ucitajRacune("racuni.txt");
    if (!racuni) {
        return 1;
    }

    char artikl[50], datumOd[11], datumDo[11];
    printf("Unesite naziv artikla: ");
    scanf("%s", artikl);
    printf("Unesite pocetni datum (YYYY-MM-DD): ");
    scanf("%s", datumOd);
    printf("Unesite krajnji datum (YYYY-MM-DD): ");
    scanf("%s", datumDo);

    izracunajPotrosnju(racuni, artikl, datumOd, datumDo);

    return 0;
}
