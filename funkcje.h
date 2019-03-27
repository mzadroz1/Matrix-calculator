#include <stdio.h>
#include <stdlib.h>

#define DZIALANIE 50 // maksymalna dlugosc dzialania
#define STOS 20 // rozmiar stosu

char DZ[DZIALANIE];  // tablica przechowuje dzialanie ONP

typedef struct
{
    char nazwa;
    int n;  //wiersze
    int m;  //kolumny
    double **tb;
} macierz;  //struktura przechowuj1ca macierze

macierz MAC[6];  //tablica na 6 macierzy wczytanych z pliku
macierz ans; //macierz przechowujaca wynik ostatnich obliczen

void zamien_na_ONP(); //pobiera dzialanie z pliku, zamienia na ONP i zapisuje do DZ[]
int prio(char c); //zwraca priorytet operatora (potrzebne do konwersji)
int dzialanie_ok(); //sprawdza poprawdnosc dzialania z pliku
void oblicz_ONP(); //oblicza wyrazenie ONP z DZ[], wynik w macierzy ans
void pobierz_macierze();
void malloc_ans();
void zwolnij();//free() dla kazdego malloca

void dod(macierz a, macierz b); //dodawanie, wynik w ans
void odej(macierz a, macierz b); //odejmowanie, wynik w ans
void mnoz(macierz a, macierz b); //mnozenie, wynik w ans

void wyswietl_html();//wyswietla ans w pliku

void projekt3();
