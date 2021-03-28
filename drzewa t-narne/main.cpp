#include <iostream>
#include <windows.h>
#include "menu.hpp"
#include <fstream>

using namespace std;

__int64 Factorial(int a)    //oblicza a!
{
    int score = 0;
    if(a == 0)
        return 1;
    else
    return a * Factorial(a-1);  //rekursja
}

// oblicza ze wzoru ilosc t-narnych drzew z n wewnetrznymi wezlami
int CalculateTrees(int n, int t)
{
    return ( Factorial(t*n) / (Factorial(n) * Factorial(t*n - n + 1)) );
}

//funkcja wypelnia tablice wartosciami z argumentu ONE
//od indexu LEFT do RIGHT
//SET to wybrana tablica
void ONE2SUBSET(int ONE,int * SET, int LEFT, int RIGHT)
{
    for(int i = LEFT; i <= RIGHT; i++)
        SET[i] = ONE;
}

//funckja wypisuje zawartosc tablicy T o dlugosci n
//dokonujac odpowiednich obliczen aby
//powstala Z-sekwencja
void Output(int n, int * T)
{
    for(int i = 1; i <=n; i++)
        std::cout << T[i]+i-1;
    std::cout << std::endl;
}

//funckja wypisuje zawartosc tablicy X o dlugosci n*t
void Output(int n, int t, int * X)
{
    for(int i = 1; i <=n*t; i++)
        std::cout << X[i];
    std::cout << std::endl;
}

//n - ilosc wezlow wewnetrznych, t - ilosc dzieci dla kazdego wezla
void ZtreeGenerator(int n, int t)
{
     //obsluga pliku
    std::ofstream file; //uchwyt do pliku
    char saveIntoFile;
    std::cout << "Czy zapisac wyniki do pliku 'wyniki.txt'? [y/n]: ";
    std::cin >> saveIntoFile;
    if(saveIntoFile == 'y')
    {
        file.open("wyniki.txt");
        if(file.fail())
        {
            std::cerr << "ERROR:: NIE MOZNA ZAPiSAC WYNIKOW:: Podzialy2() \n";
            exit(0);
        }
    }


    int counter = 1;    //liczy ilosc drzew
    __int64 expectedCounter = CalculateTrees(n, t);
    int * MAX = new int[n+1];   //tab pomocnicza zawiera maksymalne
                                //wartosci odpowiadajacemu elementowi
                                //w tablicy T
    int * T = new int[n+1];   //tab z wynikami
    int * TAG = new int[n+1]; //tab pomocnicza

    int ind = 1;   //wskazuje kolejne elementy w tablicy T ktore
                   // zostana zmodyfikowane
    int s = 1;  //zmienna pomocnicza, przechowuje przyszle wartosci
                // zmienianego elementu w tablicy T
    for(int i = 1; i <= n; i++)
        MAX[i] = (i-1) * (t-1) + 1;

    ONE2SUBSET(s, T, ind, n);  //ustawia elementy w tablicy na s
    ONE2SUBSET(0, TAG, ind, n); //ustawia elementy w tablicy na 0

    Output(n, T);   //wypisz pierwsza sekwencje
    //zapis do pliku
    if(saveIntoFile == 'y')
    {
        for (int i = 1; i <=n; i++)     //zapis do pliku kolejnych sekwencji
                    file << T[i] + i - 1;
                file << '\n';
    }

    ind = n;

    while(ind > 1)  //dopoki nie wygeneruje wszystkich sekwencji
    {
        if(T[ind] < MAX[ind])
        {
            s = T[ind] + 1;
            ONE2SUBSET(s, T, ind, n);
            if(s==MAX[ind])
                ONE2SUBSET(1, TAG, ind, ind);
            if(ind < n)
                ONE2SUBSET(0, TAG, ind+1, n);
            Output(n, T);     //wypisz kolejne sekwencje
            //zapis do pliku
            if(saveIntoFile == 'y')
            {
                for (int i = 1; i <=n; i++)     //zapis do pliku kolejnych sekwencji
                    file << T[i] + i - 1;
                file << '\n';
            }

            ind = n;
            counter++;

        }
        else
            ind = ind-1;
    }

    //sprawdzenie poprawnosci
    std::cout << "Ilosc drzew: " << counter << std::endl;
    std::cout << "Oczekiwana ilosc drzew: " << expectedCounter << std::endl;
    if(counter == expectedCounter)
        std::cout << "Wynik zgadza sie z oczekiwanym wynikiem \n";
    else
        std::cout << "Wynik prawdopodobnie NIE zgadza sie z oczekiwanym wynikiem"
        << "(mozliwy blad: jezeli wynik Factorial(n*t) jest wiekszy od "
        << "maksymalnej wartosci zmiennej int64 a sam algorytm moze byc poprawny) \n";


    //zwolnienie zaalokowanej pamieci
    delete MAX;
    delete T;
    delete TAG;
    //zamkniecie pliku
    if(saveIntoFile == 'y')
        file.close();
}

void XtreeGenerator(int n, int t)
{
     //obsluga pliku
    std::ofstream file; //utworzenie uchwytu do pliku
    char saveIntoFile;
    std::cout << "Czy zapisac wyniki do pliku 'wyniki.txt'? [y/n]: ";
    std::cin >> saveIntoFile;
    if(saveIntoFile == 'y')
    {
        file.open("wyniki.txt");
        if(file.fail())
        {
            std::cerr << "ERROR:: NIE MOZNA ZAPiSAC WYNIKOW:: Podzialy2() \n";
            exit(0);
        }
    }


    int counter = 1;    //liczy ilosc drzew
    __int64 expectedCounter = CalculateTrees(n, t);
    int * MAX = new int[n+1];   //tab pomocnicza zawiera maksymalne
                                //wartosci odpowiadajacemu elementowi
                                //w tablicy T
    int * T = new int[n+1];   //tab z poœrednimi wynikami
    int * TAG = new int[n+1]; //tab pomocnicza
    int * X = new int[n*t + 1];    //tab z wynikami

    int ind = 1;   //wskazuje kolejne elementy w tablicy T ktore
                   // zostana zmodyfikowane
    int s = 1;  //zmienna pomocnicza, przechowuje przysz³e wartoœci
                // zmienianego elementu w tablicy T
    for(int i = 1; i <= n; i++)
        MAX[i] = (i-1) * (t-1) + 1;

    ONE2SUBSET(s, T, ind, n);  //ustawia elementy w tablcy na s
    ONE2SUBSET(0, TAG, ind, n); //ustawia elementy w tablcy na 0

    ONE2SUBSET(1, X, ind, n);   //ustawia elementy w tablcy na 1 od idexu 1 do n
    ONE2SUBSET(0, X, n+1, n*t); //ustawia elementy w tablcy na 0 od idexu n+1 do n*t


    Output(n, t, X);   //wypisz pierwsza sekwencje
    //zapis do pliku
    if(saveIntoFile == 'y')
    {
        for (int i = 1; i <=n; i++)     //zapis do pliku kolejnych sekwencji
                    file << T[i] + i - 1;
                file << '\n';
    }
    ind = n;

    while(ind > 1)
    {
        if(T[ind] < MAX[ind])
        {
            s = T[ind] + 1;
            ONE2SUBSET(s, T, ind, n);

            ONE2SUBSET(0, X, T[ind-1]+ind-1, n*t);
            ONE2SUBSET(1, X, T[ind]+ind-1, T[ind]+n-1);

            if(s==MAX[ind])
                ONE2SUBSET(1, TAG, ind, ind);
            if(ind < n)
                ONE2SUBSET(0, TAG, ind+1, n);
            Output(n,t, X);     //wypisz kolejne sekwencje
            //zapis do pliku
            if(saveIntoFile == 'y')
            {
                for (int i = 1; i <=n; i++)     //zapis do pliku kolejnych sekwencji
                    file << T[i] + i - 1;
                file << '\n';
            }

            ind = n;
            counter++;

        }
        else
            ind = ind-1;
    }
    //sprawdzenie poprawnosci
    std::cout << "Ilosc drzew: " << counter << std::endl;
    std::cout << "Oczekiwana ilosc drzew: " << expectedCounter << std::endl;
    if(counter == expectedCounter)
        std::cout << "Wynik zgadza sie z oczekiwanym wynikiem \n";
    else
        std::cout << "Wynik prawdopodobnie NIE zgadza sie z oczekiwanym wynikiem"
        << "(mozliwy blad: jezeli wynik Factorial(n*t) jest wiekszy od "
        << "maksymalnej wartosci zmiennej int64 a sam algorytm moze byc poprawny) \n";


    //zwolnienie zaalokowanej pamieci
    delete MAX;
    delete T;
    delete TAG;
    delete X;
    //zamkniecie pliku
    if(saveIntoFile == 'y')
        file.close();
}



int main(int argc, char** argv)
{
    if (argc == 2)
    {
        string a = argv[1];
        if (a == "-v")
        {
            cout << "Version: " << __DATE__ << " | " << __TIME__;
            return 0;
        }

    }
    int choice;
    int n;  //ilosc wewnetrznych wezlow
    int k;  //ilosc dzieci

    while(1)
    {
        system("cls");
        info();
        menu();
        std::cin >> choice;
        system("cls");

        switch(choice)
        {
        case 1:
            std::cout << "Podaj n: ";
            std::cin >> n;
            std::cout << "Podaj t: ";
            std::cin >> k;
            ZtreeGenerator(n,k);
            cout << "Click anything to continue \n";
            getchar(); getchar();
            break;
        case 2:
            std::cout << "Podaj n: ";
            int n;
            std::cin >> n;
            std::cout << "Podaj t: ";
            std::cin >> k;
            XtreeGenerator(n,k);
            cout << "Click anything to continue \n";
            getchar(); getchar();
            break;
        case 3:
            Credits();
            getchar(); getchar();
            break;
        case 4:
            Exit();
            break;
        default:
            std::cerr << "ERROR podana opcja nie istnieje \a \n";
            Sleep(1000);
            break;
        }

    }

    return 0;
}
