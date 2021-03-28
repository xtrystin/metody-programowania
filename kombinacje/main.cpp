#include <iostream>
#include <windows.h>
#include "menu.hpp"
#include <fstream>

using namespace std;

int Factorial(int a)    //oblicza a!
{
    int score = 0;
    if(a == 0)
        return 1;
    else
    return a * Factorial(a-1);  //rekursja
}

//oblicza n! / ( k! * (n-k)! )
int BinomialTheorem(const int n, const int k)
{
    return ( Factorial(n) / (Factorial(k) * Factorial(n-k)) );
}

//funkcja pomocnicza
int Power2(int a)   // 2^a
{
    if(a == 0)
        return 1;
    else
    {
    int score = 1;
        for(int i = 1; i <=a; i++)
            score *= 2;
    return score;
    }
}

int CalculateCombinations(int n, int k_min, int k_max)
{
    int sum = 0;
    for(int i = k_min; i <= k_max; i++)
        sum += BinomialTheorem(n, i);
    return sum;
}

void CombinationGenerator(int n, int k)
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


    int counter = 1;    // licznik kombinacji
    //oblicza ilosc kombinacji ze wzoru:
    int expectedCounter = BinomialTheorem(n, k);
    int * K = new int [k+1];    //tablica z wynikami
    int * L = new int [k+1];    //tablica pomocnicza

    int i;
    for(i = 1; i <= k; i++)
    {
        K[i] = i;
        L[i] = n - k + i;
    }

    //wyswietl pierwxza sekwencje
    for(int t = 1; t <= k; t++)
        std::cout << K[t] <<" ";
    std::cout << std::endl;
    //zapis do pliku
    if(saveIntoFile == 'y')
    {
        for (int id = 1; id <=k; id++)     //zapis do pliku kolejnych sekwencji
                    file << K[id];
                file << '\n';
    }
    do
    {
        counter++;
        i = k;
        if(K[i] < L[i])
            K[i] = K[i] + 1;
        else
        {
            do
            {
                i = i - 1;
            }while(K[i] >= L[i]);

            K[i] = K[i] + 1;
            for(int j = i + 1; j <= k; j++)
                K[j] = K[j-1] + 1;
        }
        //wyswietl kolejne sekwencje
        for(int t = 1; t <= k; t++)
            std::cout << K[t] <<" ";
        std::cout << std::endl;
        //zapis do pliku
        if(saveIntoFile == 'y')
        {
        for (int id = 1; id <=k; id++)     //zapis do pliku kolejnych sekwencji
                    file << K[id];
                file << '\n';
        }

    }while(K[1] != L[1]);

    //sprawdzenie poprawnosci
    std::cout << "counter: "<< counter << "\nExpected value: " << expectedCounter <<std::endl;
    if(counter == expectedCounter)
        std::cout << "Wynik zgadza sie z oczekiwanym wynikiem \n";
    else
        std::cout << "Wynik NIE zgadza sie z oczekiwanym wynikiem \n";


    //zwolnienie zaalokowanej pamieci
    delete L;
    delete K;
    //zamkniecie pliku
    file.close();
}

void Semba(int n, int k)
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

     int * K = new int [k+1];    //tablica z wynikami

    int counter = 0;    // zmienna kontrolna
    //wylizcenie ze wzoru liczby sekwencji
    int expectedCounter = Power2(n) - 1;

    k = 0;
    K[k] = 0;
    do
    {
        counter++;
        if(K[k] < n)
        {
            //extend
            K[k+1] = K[k] + 1;
            k++;
        }
        else
        {
            //reduce
            k--;
            K[k] = K[k] + 1;
        }
        //wypisanie
        for(int i = 1; i <=k; i++)
            std::cout << K[i] << " ";
        std::cout << std::endl;

        //zapis do pliku
        if(saveIntoFile == 'y')
        {
        for (int id = 1; id <=k; id++)     //zapis do pliku kolejnych sekwencji
                    file << K[id];
                file << '\n';
        }

    }   while(K[1] != n);

    //sprawdzenie poprawnosci
    std::cout << "counter: "<< counter << "\nExpected value: " << expectedCounter <<std::endl;
    if(counter == expectedCounter)
        std::cout << "Wynik zgadza sie z oczekiwanym wynikiem \n";
    else
        std::cout << "Wynik NIE zgadza sie z oczekiwanym wynikiem \n";

    //zwolnienie zaalokowanej pamieci
    delete K;
    //zamkniecie pliku
    file.close();
}

void SembaGeneral(int n, int k, int k_min, int k_max)
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


    int * K = new int [k+1];    //tablica z wynikami
    int counter = 1;            //zmienna kontrolna
    int expectedCounter = CalculateCombinations(n, k_min, k_max);

    for(int i = 1; i <= k_min; i++)
        K[i] = i;

    k = k_min;

    //wypisanie pierwszej sekwencji
    for(int i = 1; i <= k; i++)
        std::cout << K[i] << " ";
    std::cout << std::endl;

    //zapis do pliku
    if(saveIntoFile == 'y')
    {
    for (int id = 1; id <=k; id++)     //zapis do pliku kolejnych sekwencji
                file << K[id];
            file << '\n';
    }

    do
    {
        if(K[k] < n && k < k_max)
        {
            //extend
            K[k+1] = K[k] + 1;
            k++;
        }
        else
        {
            //cut
            if(K[k] < n )
                K[k] = K[k] + 1;
            else if(k >= k_min)
            {
                //reduce    K[--k]++;
                k--;
                K[k] = K[k] + 1;

                if(k < k_min)
                {
                    for(int j = k+1; j <= k_min; j++)
                    {
                        K[j] = K[j-1] + 1;
                        k = k_min;
                    }
                }
            }
        }
        counter++;
        //wypisanie kolejnych sekwencji
        for(int i = 1; i <= k; i++)
            std::cout << K[i] << " ";
        std::cout << std::endl;

        //zapis do pliku
        if(saveIntoFile == 'y')
        {
        for (int id = 1; id <=k; id++)     //zapis do pliku kolejnych sekwencji
                    file << K[id];
                file << '\n';
        }

    }   while(K[1] != n - k_min + 1);


    //sprawdzenie poprawnosci
    std::cout << "counter: "<< counter << "\nExpected value: " << expectedCounter <<std::endl;
    if(counter == expectedCounter)
        std::cout << "Wynik zgadza sie z oczekiwanym wynikiem \n";
    else
        std::cout << "Wynik NIE zgadza sie z oczekiwanym wynikiem \n";

    //zwolnienie zaalokowanej pamieci
    delete K;
    //zamkniecie pliku
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
    int n;
    int k;
    int k_min, k_max;

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
            std::cout << "Podaj k: ";
            std::cin >> k;
            CombinationGenerator(n,k);
            cout << "Click anything to continue \n";
            getchar(); getchar();
            break;
        case 2:
            std::cout << "Podaj n: ";
            std::cin >> n;
            std::cout << "Podaj k: ";
            std::cin >> k;
            Semba(n,k);
            cout << "Click anything to continue \n";
            getchar(); getchar();
            break;
        case 3:
            std::cout << "Podaj n: ";
            std::cin >> n;
            std::cout << "Podaj k: ";
            std::cin >> k;
            std::cout << "Podaj k_min: ";
            std::cin >> k_min;
            std::cout << "Podaj k_max: ";
            std::cin >> k_max;
            SembaGeneral(n,k, k_min, k_max);
            cout << "Click anything to continue \n";
            getchar(); getchar();
            break;
        case 4:
            Credits();
            getchar(); getchar();
            break;
        case 5:
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
