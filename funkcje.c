#include <stdio.h>
#include <stdlib.h>
#include "funkcje.h"

int ile=0;  //ile macierzy pobrano
int in=0;             // index w tablicy DZ, po pobraniu dzialania przechowuje jego dlugosc

void projekt3()
{
    zamien_na_ONP();
    pobierz_macierze();

    if(ile>0)
    {
        malloc_ans();
        if(dzialanie_ok())
        {
            oblicz_ONP();
            wyswietl_html();
            puts("Wynik dzialania zostal pomyslnie zapisany do pliku wynik.html");
        }
        else
        {
            puts("Bledne dzialanie w pliku.");
            exit(0);
        }
        zwolnij();
    }
    else
    {
        puts("Brak macierzy do pobrania.");
        exit(0);
    }
}

void pobierz_macierze()
{
    FILE *plik;
    if((plik=fopen("macierze.txt","r"))==NULL)
    {
        puts("Blad otwarcia pliku z macierzami");
        exit(-1);
    }

    int i=0,j=0;

    while(ile<6)
    {
        macierz mx;
        char c;
        while((c=getc(plik))=='\n' || c==' ' || c=='\r');
        if((mx.nazwa=c)==EOF)break; //nazwa

        if(fscanf(plik,"%d %d",&mx.n,&mx.m)!=2){puts("Bledna definicja macierzy.");exit(0);}//wiersze i kolumny

        mx.tb=(double**)malloc(mx.n*sizeof(double*));

        for(i=0; i<mx.n; i++)
            mx.tb[i]=(double*)malloc(mx.m*sizeof(double));//alokacja pamieci

        int blad=0;
        for(i=0; i<mx.n; i++)
            for(j=0; j<mx.m; j++)
                blad+=fscanf(plik,"%lf",&mx.tb[i][j]); //odcytanie wspolczynnikow

        if(blad!=mx.m*mx.n){puts("Bledna definicja macierzy.");exit(0);}
        MAC[ile]=mx; //zapisanie wczytanej macierzy do tablicy
        ile++;
    }



    fclose(plik);
}

void malloc_ans()//alokuje pamiec dla macierzy przechowujacej wyniki
{
    ans.n=MAC[0].n;
    ans.m=MAC[0].m;
    ans.tb=(double**)malloc(ans.n*sizeof(double*));

    int i=0;

    for(i=0; i<ans.n; i++)
        ans.tb[i]=(double*)malloc(ans.m*sizeof(double));
}


void dod(macierz a, macierz b)
{
    if(a.n!=b.n || a.m!=b.m){puts("Wykonanie dzialania jest niemozliwe.");exit(0);}
    int i=0,j=0;
    for(i=0;i<a.n;i++)
    {
        for(j=0;j<a.m;j++)
        {
            ans.tb[i][j]=a.tb[i][j]+b.tb[i][j];
        }
    }
}

void odej(macierz a, macierz b)
{
    if(a.n!=b.n || a.m!=b.m){puts("Wykonanie dzialania jest niemozliwe.");exit(0);}
    int i=0,j=0;
    for(i=0;i<a.n;i++)
    {
        for(j=0;j<a.m;j++)
        {
            ans.tb[i][j]=a.tb[i][j]-b.tb[i][j];
        }
    }
}

void mnoz(macierz a, macierz b)
{
    if(a.n!=b.n || a.m!=b.m){puts("Wykonanie dzialania jest niemozliwe.");exit(0);}
    int i=0,j=0;
    for(i=0;i<a.n;i++)
    {
        for(j=0;j<a.m;j++)
        {
            ans.tb[i][j]=a.tb[i][j]*b.tb[i][j];
        }
    }
}

//zwraca priorytet operatora
int prio(char c)
{
  switch(c)
  {
    case '+' : ;
    case '-' : return 1;
    case '*' : return 2;
  }
  return 0;
}


void zamien_na_ONP()
{
    char S[STOS];            // stos operatorów
    int p=0;                // wskaznik stosu
    char c;                 // kolejny znak wyrazenia
    FILE *plik;
    if((plik=fopen("dzialanie.txt","r"))==NULL)
    {
        puts("Blad otwarcia pliku z dzialaniem");
        exit(-1);
    }

  while(1)
  {
    if((c=getc(plik))==EOF)                             // koniec wyrazenia?
    {
      while(p) DZ[in++]=S[--p];        // jesli tak, na wyjscie przesylamy
                                             // wszystkie operatory ze stosu
      break;                                 // i przerywamy petle
    }

    switch(c)                                // analizujemy odczytany znak
    {
      case ' ' : break;                      // spacje  i znak nowej linii ignorujemy
      case '\n': break;
      case '(' : S[p++] = '(';            // nawias otwierajacy zawsze na stos
                 break;
      case ')' : while(S[p-1] != '(')     // nawias zamykajacy
                 {
                     if(p==0){puts("Bledne dzialanie w pliku.");exit(0);}
                     else DZ[in++]=S[--p];         // ze stosu przesyaamy na wyjocie wszystkie operatory az do nawiasu otw.
                 }

                 p--;                     // usuwamy ze stosu nawias otwierajacy
      case '+' : ;                           // operator
      case '-' : ;
      case '*' : while(p)
                 {
                   if(prio(c) > prio(S[p-1])) break;
                   DZ[in++]=S[--p];       // na wyjscie przesylamy ze stosu wszystkie
                 }                           // operatory o wyzszych priorytetach
                 S[p++] = c;              // operator umieszczamy na stosie
                 break;
      default:   DZ[in++]=c;           // inaczej znak przesylamy na wyjscie
                 break;
    }
  }
    fclose(plik);
}

int dzialanie_ok()
{
    int na_stosie=0;
    int i=0;
    for(;i<in;i++) //przegladamy wyrazenie ONP
    {
        if(DZ[i]>='A' && DZ[i]<='F') na_stosie++; //macierze odkladamy na "stos"
        else if((DZ[i]=='+' || DZ[i]=='-' || DZ[i]=='*') && na_stosie>=2) na_stosie--;//operator moze wystapic po conajmniej dwoch macierzach
        else return 0; //inne znaki oznaczaja blad
    }
    return 1;
}

void oblicz_ONP()
{
    macierz S[STOS];      // stos
    int p=0;              // wskaznik stosu
    int i=0;              //index przy przegladniu dzialania
  while(i<in)           // w pêtli przetwarzamy wyra¿enie ONP
  {
      switch(DZ[i++])
      {
        case 'A' : S[p++]=MAC[0]; break;  //macierze odkladaj na stos
        case 'B' : S[p++]=MAC[1]; break;
        case 'C' : S[p++]=MAC[2]; break;
        case 'D' : S[p++]=MAC[3]; break;
        case 'E' : S[p++]=MAC[4]; break;
        case 'F' : S[p++]=MAC[5]; break;
        case '+' : dod(S[p-1],S[p-2]);S[p++]=ans; break;   //gdy trafisz na operator pobierz
        case '-' : odej(S[p-2],S[p-1]);S[p++]=ans; break;  //dwie macierze ze stosu i wykonaj dzialanie
        case '*' : mnoz(S[p-1],S[p-2]);S[p++]=ans; break;  //zgodne z operatorem, wynik odloz na stos
      }
  }
}

void wyswietl_html()
{
    int i=0,j=0;
    FILE *plik;
    if((plik=fopen("wynik.html","w"))==NULL)
    {
        puts("Blad utworzenia pliku z wynikiem");
        exit(-1);
    }
    fprintf(plik,"<html>");
        fprintf(plik,"<head>");
            fprintf(plik,"<meta charset=\"utf-8\">");
            fprintf(plik,"<title>Wynik</title>");
        fprintf(plik,"</head>");
        fprintf(plik,"<body>");
            fprintf(plik,"<table border=\"1\">");
                for(i=0; i<ans.n; i++)
                {
                    fprintf(plik,"<tr>");
                    for(j=0; j<ans.m; j++)
                    {
                        fprintf(plik,"<td><h1> %g </h1></td>",ans.tb[i][j]);
                    }
                    fprintf(plik,"</tr>");
                }
            fprintf(plik,"</table>");
        fprintf(plik,"</body>");
    fprintf(plik,"</html>");
    fclose(plik);
}

void zwolnij()
{
        int i=0,j=0;
        for(i=0; i<ans.n; i++)
            free(ans.tb[i]);
            free(ans.tb);
        for(j=0;j<ile;j++)
        {
            for(i=0; i<MAC[j].n; i++)
            free(MAC[j].tb[i]);
            free(MAC[j].tb);
        }
}
