#include <stdio.h>
#include <stdlib.h>
#include "funkcje.h"
#define Smax 10 //maskymalne wymiary macierzy to Smax x Smax

double M[Smax][Smax],A[Smax][Smax],C[Smax][Smax];//M i A pobierane macierze poddawane dzialaniu C-wynik
int i,j,n,m;

void pobieranie(double T[Smax][Smax],int k);//pobiera macierz, w zaleznosci od k pyta o wymiary
void wyswietlanie(double T[Smax][Smax]);
void dodawanie(double T[Smax][Smax],double W[Smax][Smax],int k);//w pozostalych k potrzebne przy dzialaniach wykorzystujacych zapamietany wynik
void odejmowanie(double T[Smax][Smax],double W[Smax][Smax],int k);
void mnozenie(double T[Smax][Smax],double W[Smax][Smax],int k);
void transpozycja(double T[Smax][Smax],int k);
void skalar(double T[Smax][Smax], int k);
void wynik();
void bufor();

int main()
{
    puts("Michal Zadrozny Projekt 3.1\n");
    puts("Kalkulator macierzy\n");
    puts("\nUwaga: wspolczynniki podawaj czytajac wierszami. Jesli podasz za duzo wspolczynnikow, to nadmiar zostanie odrzucony.\n");

    int g=0;
    while(1)
    {
    puts("\nWybierz numer dzialania i wcisnij enter: \n");
    puts("[1] Dodawanie macierzy");
    puts("[2] Odejmowanie macierzy");
    puts("[3] Mnozenie dwoch macierzy");
    puts("[4] Mnozenie macierzy przez skalar");
    puts("[5] Transpozycja macierzy");
    puts("[6] Pobierz macierze z pliku macierze.txt\n    i wykonaj na nich dzialanie z pliku dzialanie.txt ");
    if(g>0)puts("[7] Wykorzysaj wynik poprzedniego dzialania");//opcja dostepna dopiero po wykonaniu co najmniej jednego dzialania
    puts("[inny znak] Zamknij");
    puts("");
    char c;
    switch(c=getchar())
    {
        case '1': dodawanie(M,A,2);break;
        case '2': odejmowanie(M,A,2);break;
        case '3': mnozenie(M,A,2);break;
        case '4': skalar(M,2);break;
        case '5': transpozycja(M,2);break;
        case '6': projekt3();g=-1;bufor();break;
        case '7': if(g==0)exit(0);else wynik();break;
        default : exit(0);
    }
    g++;
    }
    return 0;
}

void pobieranie(double T[Smax][Smax],int k)// k=0 nie pobiera liczby wierszy ani kolumn(dodawanie), k=2 pobiera wszystko, k=1 pobiera tylko kolumny(mnozenie)
{
if(k==2) {
    while(1)//petle konczace sie gdy dane bed1 poprawne
    {
        puts("Podaj liczbe wierszy: ");
        if(scanf("%d",&n)==1 && n<Smax && n>0) {bufor();break;}
        else {puts("Niepoprawne dane, sproboj jeszcze raz:");bufor();}
    }
    }
    if(k==1 || k==2){
    while(1)
    {
        puts("Podaj liczbe kolumn: ");if(k==1)puts("(Aby pomnozyc macierz przez wektor, wpisz 1)");
        if(scanf("%d",&m)==1 && m<Smax && m>0) {bufor();break;}
        else {puts("Niepoprawne dane, sproboj jeszcze raz:");bufor();}
    }
    }

    printf("\nPodaj wspolczynniki macierzy %d x %d oddzielajac je spacjami lub enterami: \n", n,m);

    int blad=0;
    while(1)
    {for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            blad+=scanf("%lf",&T[i][j]);
        }
    }
    if(blad==n*m) {break;bufor();}
    else {puts("Podczas wpisywania wpolczynnikow wystapil blad, podaj wspolczynniki jeszcze raz:");bufor();blad=0;}
    }
    printf("\n");
    bufor();
}

void wyswietlanie(double T[Smax][Smax])
{
    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            printf("%g ",T[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void dodawanie(double T[Smax][Smax],double W[Smax][Smax],int k)//k=2 pobiera 2 macierze, inne k - pobiera jedn1(przy wykorzystaniu wyniku drugiej nie potrzeba)
{
    puts("\nDodawanie\nObie macierze maja te same rozmiary");
    if(k==2){puts("\nPierwsza macierz:");
    pobieranie(T,2);}
    puts("\nDruga macierz:");
    pobieranie(W,0);
    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            C[i][j]=T[i][j]+W[i][j];
        }
    }
    puts("\nWynik dodawania: \n");
    wyswietlanie(C);
}

void odejmowanie(double T[Smax][Smax],double W[Smax][Smax], int k)//k=2 pobiera 2 macierze, inne k - pobiera jedn1(przy wykorzystaniu wyniku drugiej nie potrzeba)
{
    puts("\nOdejmowanie\nObie macierze maja te same rozmiary\n");
    if(k==2){puts("\nPierwsza macierz:");
    pobieranie(T,2);}
    puts("\nDruga macierz:");
    pobieranie(W,0);
    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            C[i][j]=T[i][j]-W[i][j];
        }
    }
    puts("Wynik odejmowania: \n");
    wyswietlanie(C);
}

void mnozenie(double T[Smax][Smax],double W[Smax][Smax], int k)//k=2 pobiera 2 macierze, inne k - pobiera jedn1(przy wykorzystaniu wyniku drugiej nie potrzeba)
{
    puts("\nMnozenie\nIlosc wierszy drugiej macierzy jest rowna liczbie kolumn pierwszej macierzy (aby mnozenie bylo wykonalne).");
    if(k==2){puts("\nPierwsza macierz:");
    pobieranie(T,2);}
    int tn=n,tm=m;
    n=m;//z definicji w drugiej macierzy musi byc tyle wierszy co w pierwszej kolumn
    printf("\nDruga macierz: \n(Ilosc wierszy przydzielona automatycznie: %d )\n\n",m);
    pobieranie(W,1);//pobieramy macierz, dostajemy ilosc kolumn wyniku
    n=tn;//mnozenie macierzy o wymiarach n x m i m x z daje macierz n x z, wracamy do n wierszy
    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            int suma=0;
            int a=0;
            for(a=0;a<tm;a++)//zapamietana liczba kolumn 1 macierzy(no i wierszy drugiej) bo oznacza ilosc mnozen przy obliczeniu jednego wspolczynnika
            {
                suma+=T[i][a]*W[a][j];
            }
            C[i][j]=suma;
        }
    }
    puts("Wynik mnozenia: ");
    wyswietlanie(C);
}

void transpozycja(double T[Smax][Smax],int k)
{
    puts("\nTranspozycja\n");
    if(k==2) pobieranie(T,2);
    puts("Macierz transponowana: ");
    if(k==1)//gdy transponujemy wynik poprzedniego dzialania
    {
        for(i=0; i<n; i++)
        {
            for(j=0; j<m; j++)
            {
                T[i][j]=C[i][j];
            }
        }
    }
    int z=0;
    if(n>m)z=n; else z=m;//wpisujemy macierz w kwadratow1 macierz i zamieniamy wpolczynniki
    for(i=0;i<z;i++)
    {
        for(j=0;j<z;j++)
        {
            C[i][j]=T[j][i];
        }
    }
    int tn=n;
    n=m;
    m=tn;//transponowana macierz ma zamienione kolumny i wiersze
    wyswietlanie(C);
    printf("\n");
}

void skalar(double T[Smax][Smax],int k)
{
    puts("\nMnozenie macierzy przez skalar\n");
    if(k==2) pobieranie(T,2);
    double s=0.0;
    while(1)
    {
        puts("Podaj skalar: ");
        if(scanf("%lf",&s)==1) {bufor();break;}
        else {puts("Niepoprawne dane, sproboj jeszcze raz:");bufor();}
    }
    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            C[i][j]=T[i][j]*s;
        }
    }
    puts("Wynik mnozenia: \n");
    wyswietlanie(C);
}

void wynik()
{
    puts("\nWybierz dzialanie: \n");
    puts("[1] Dodaj macierz do wyniku");
    puts("[2] Odejmij macierz od wyniku");
    puts("[3] Pomnoz wynik przez macierz");
    puts("[4] Pomnoz wynik przez skalar");
    puts("[5] Transpozycja macierzy");
    puts("[inny znak] Zamknij");
    char c;
    bufor();
    switch(c=getchar())
    {
        case '1': dodawanie(C,A,1);break;
        case '2': odejmowanie(C,A,1);break;
        case '3': mnozenie(C,A,1);break;
        case '4': skalar(C,1);break;
        case '5': transpozycja(M,1);break;
        default : exit(0);
    }
    bufor();
}

void bufor()
{
    char c;
    while(scanf("%c",&c) && c!='\n');
}
