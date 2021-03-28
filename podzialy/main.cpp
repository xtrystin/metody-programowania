#include <iostream>
#include <windows.h>
#include "menu.hpp"
#include <fstream>

using namespace std;

//funkcja wypelnia tablice wartosciami z argumentu ONE
//od indexu LEFT do RIGHT
//SET to wybrana tablica
void ONE2SUBSET(int ONE,int * SET, int LEFT, int RIGHT)
{
    for(int i = LEFT; i <= RIGHT; i++)
        SET[i] = ONE;
}

int Factorial(int a)    //calculate a!
{
    int score = 0;
    if(a == 0)
        return 1;
    else
    return a * Factorial(a-1);  //recursion
}

int BinomialTheorem(const int n, const int k)
{
    return ( Factorial(n) / (Factorial(k) * Factorial(n-k)) );
}

int CalculateBellNumber(int n)
{
    if(n == 0)
        return 1;
    else if(n == 1)
        return 1;
    else if(n == 2)
        return 2;

    int sum = 0;
    for(int k = 0; k <= n-1; k++)
        sum += BinomialTheorem(n-1, k) * CalculateBellNumber(k);
    return sum;

}

void Podzialy1(int n)    //n - wartosc podana przez uzytkownika
{                       //    okresla wielkosc tablic
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



    int IND = 1;    //przechowuje numer tablicy ktorej zmieniamy wartosc
    int J = n - 1;
    int S = 1;      //przechowuje przyszle wartosci elementu R ktory ma sie zmienic

    int counter = 1;    //zmienna kontrolna
    int expectedCounter = CalculateBellNumber(n);    //oczekiwana ilosc podzialow

    // uzywane indexy tablicy: 1 do n
    int * R = new int [n+1];    //tab z wynikiem
    int * M = new int [n+1];    //tab pomocnicza

    ONE2SUBSET(S+1, M, IND, n);
    ONE2SUBSET(S, R, IND, n);

    S = R[IND] + 1; //wyznaczenie kolejnej wartosci S

    for (int i = 1; i <=n; i++)     //wyprowadzenie 1. podzialu
        std::cout << R[i];
    std::cout << std::endl;

    IND = n;

    while(IND > 1)     //dopoki nie bedzie zamieniany 1. element tablicy
    {
        ONE2SUBSET(S, R, IND, IND);
        if(IND < n)
        {
            ONE2SUBSET(1, R, IND+1, n);     //generowanie nowego podzialu
            ONE2SUBSET(S+1, M, IND+1, n);   //generowanie nowej tab pomocniczej
            IND = n;
            J = n -1;
        }
        counter++;
        for (int i = 1; i <=n; i++) //wyprowadzenie kolejnych podzialow
            std::cout << R[i];
        std::cout << std::endl;

         //zapis do pliku
        if(saveIntoFile == 'y')
        {
            for (int i = 1; i <=n; i++)     //zapis do pliku kolejnych sekwencji
            file << R[i];
            file << '\n';
        }


        if(R[n] < M[n])
        {
            S = R[n] + 1;       //ustawienie nowej wartosci S
            if(R[J] == M[J])
                J--;
        }
        else
            {
                IND = J;         //ustawienie nowej wartosci ind
                S = R[IND] + 1; //ustawienie nowej wartosci S
            }


    }
    std::cout << "Ilosc podzialow: " << counter << std::endl;
    std::cout << "Oczekiwana ilosc podzialow: " << expectedCounter << std::endl;
    if(counter == expectedCounter)
        std::cout << "Wynik zgadza sie z oczekiwanym wynikiem \n";
    else
        std::cout << "Wynik NIE zgadza sie z oczekiwanym wynikiem \n";


    //zwolnienie zaalokowanej pamieci
    delete R;
    delete M;
    //zamkniecie pliku
    if(saveIntoFile == 'y')
        file.close();
}

void Podzialy2(int n)
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


    int r =0;
    int * c = new int [n+1];    //tab z wynikami
    int n1 = n - 1;
    int * g = new int [n+1];    // tab pomocnicza
    c[0] = 0;
    g[0] = 0;

    int counter = 0;       //zmienna kontrolna
    int expectedCounter = CalculateBellNumber(n);    //oczekiwana ilosc podzialow

    bool firstTime = true; //zmienna kontrolna
    do
    {
        while(r < n1)
        {
            r++;        //ustawia r na n1 (po wykonaniu petli)
            c[r] = 1;   // od c[0] do c[n1] ustawia wartosci na 1
            g[r] = g[r-1];
        }
        for(int j = 1; j <= g[n1]+1; j++)
        {
            c[n] = j;
            if(firstTime == true)   // zeby 1. sekwencja nie powtarzala sie
            {
                firstTime = false;
                break;
            }
            for (int i = 1; i <=n; i++)     //wyprowadzenie kolejne sekwencje
                std::cout << c[i];
            std::cout << std::endl;

            //zapis do pliku
            if(saveIntoFile == 'y')
            {
                for (int i = 1; i <=n; i++)     //zapis do pliku kolejnych sekwencji
                    file << c[i];
                file << '\n';
            }

            counter++;
        }
        while(c[r] > g[r-1])    //backtracking
            r--;    //znajdz najwieksze r spelniajace warunek
        c[r]++;     //zwieszka element na pozycji r o 1
        if(c[r] > g[r])
            g[r] = c[r];

    }while(r != 1);

    // sprawdzenie poprawnoœci
    std::cout << "Ilosc podzialow: " << counter << std::endl;
    std::cout << "Oczekiwana ilosc podzialow: " << expectedCounter << std::endl;
    if(counter == expectedCounter)
        std::cout << "Wynik zgadza sie z oczekiwanym wynikiem \n";
    else
        std::cout << "Wynik NIE zgadza sie z oczekiwanym wynikiem \n";

    //zwolnienie zaalokowanej pamiêci
    delete c;
    delete g;
    //zamkniecie pliku
     if(saveIntoFile == 'y')
        file.close();
}


void Exit()
{
    std::cout << "Leaving..." << std::endl;
    Sleep(1000);
    exit(0);
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
    int n;

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
            Podzialy1(n);
            cout << "Click anything to continue \n";
            getchar(); getchar();
            break;
        case 2:
            std::cout << "Podaj n: ";
            int n;
            std::cin >> n;
            Podzialy2(n);
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
            std::cerr << "ERROR podana opcja nie istnieje \n";
            Sleep(1000);
            break;
        }

    }


    return 0;
}
