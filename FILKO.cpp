#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Osoba {
    char ime[30];
    char prezime[30];
    int godina_rodenja;
    struct Osoba* next;
} Osoba;

Osoba* kreirajOsobu(const char* ime, const char* prezime, int godina_rodenja) {
    Osoba* novaOsoba = (Osoba*)malloc(sizeof(Osoba));
    if (novaOsoba == NULL) {
        printf("Greška pri alokaciji memorije!\n");
        exit(EXIT_FAILURE);
    }
    strncpy(novaOsoba->ime, ime, sizeof(novaOsoba->ime));
    strncpy(novaOsoba->prezime, prezime, sizeof(novaOsoba->prezime));
    novaOsoba->godina_rodenja = godina_rodenja;
    novaOsoba->next = NULL;
    return novaOsoba;
}

Osoba* dodajNaPocetak(Osoba* lista, const char* ime, const char* prezime, int godina_rodenja) {
    Osoba* novaOsoba = kreirajOsobu(ime, prezime, godina_rodenja);
    novaOsoba->next = lista;
    return novaOsoba;
}

void ispisiListu(Osoba* lista) {
    Osoba* trenutna = lista;
    while (trenutna != NULL) {
        printf("Ime: %s, Prezime: %s, Godina rođenja: %d\n", trenutna->ime, trenutna->prezime, trenutna->godina_rodenja);
        trenutna = trenutna->next;
    }
}

Osoba* dodajNaKraj(Osoba* lista, const char* ime, const char* prezime, int godina_rodenja) {
    Osoba* novaOsoba = kreirajOsobu(ime, prezime, godina_rodenja);
    if (lista == NULL) {
        return novaOsoba;
    }

    Osoba* trenutna = lista;
    while (trenutna->next != NULL) {
        trenutna = trenutna->next;
    }
    trenutna->next = novaOsoba;
    return lista;
}

Osoba* pronadjiPoPrezimenu(Osoba* lista, const char* prezime) {
    Osoba* trenutna = lista;
    while (trenutna != NULL) {
        if (strcmp(trenutna->prezime, prezime) == 0) {
            return trenutna;
        }
        trenutna = trenutna->next;
    }
    return NULL;
}

Osoba* obrisiElement(Osoba* lista, const char* prezime) {
    Osoba* trenutna = lista;
    Osoba* prethodna = NULL;

    while (trenutna != NULL) {
        if (strcmp(trenutna->prezime, prezime) == 0) {
            if (prethodna == NULL) {
                Osoba* noviPocetak = trenutna->next;
                free(trenutna);
                return noviPocetak;
            }
            else {
                prethodna->next = trenutna->next;
                free(trenutna);
                return lista;
            }
        }
        prethodna = trenutna;
        trenutna = trenutna->next;
    }
    return lista;
}

void oslobodiListu(Osoba* lista) {
    Osoba* trenutna = lista;
    while (trenutna != NULL) {
        Osoba* sledeca = trenutna->next;
        free(trenutna);
        trenutna = sledeca;
    }
}

int main() {
    Osoba* lista = NULL;

    lista = dodajNaPocetak(lista, "Marko", "Markovic", 2000);
    lista = dodajNaKraj(lista, "Jana", "Jankovic", 1998);
    lista = dodajNaKraj(lista, "Ana", "Anic", 2001);

    printf("Lista osoba:\n");
    ispisiListu(lista);

    const char* prezimeZaPretragu = "Jankovic";
    Osoba* pronadjen = pronadjiPoPrezimenu(lista, prezimeZaPretragu);
    if (pronadjen) {
        printf("Pronađena osoba: %s %s, Godina rođenja: %d\n", pronadjen->ime, pronadjen->prezime, pronadjen->godina_rodenja);
    }
    else {
        printf("Osoba sa prezimenom '%s' nije pronađena.\n", prezimeZaPretragu);
    }

    lista = obrisiElement(lista, "Markovic");
    printf("Lista nakon brisanja:\n");
    ispisiListu(lista);

    oslobodiListu(lista);

    return 0;
}