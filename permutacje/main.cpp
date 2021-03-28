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

void Generate(int n, char * A)
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


    int counter = 1;
    int expectedCounter = Factorial(n);
    int * c = new int [n];  //tab pomocnicza
    for(int i = 0; i < n; i++)  //wyzerowanie tab
        c[i] = 0;

    for(int t = 0; t < n; t++)  //wypisanie 1 sekwencji
        std::cout << A[t] << " ";
    std::cout << std::endl;

    //zapis do pliku
        if(saveIntoFile == 'y')
        {
        for (int id = 0; id < n; id++)     //zapis do pliku kolejnych sekwencji
                    file << A[id];
                file << '\n';
        }

    int i = 1;
    while(i < n)
    {
        if(c[i] < i)
        {

            if((i % 2) == 0)
                std::swap(A[0], A[i]);
            else
                std::swap(A[c[i]], A[i]);


            //wypisanie kolejnych sekwencji
            for(int t = 0; t < n; t++)
                std::cout << A[t] << " ";
            std::cout << std::endl;
                //zapis do pliku
            if(saveIntoFile == 'y')
            {
            for (int id = 0; id < n; id++)     //zapis do pliku kolejnych sekwencji
                        file << A[id];
                    file << '\n';
            }

            counter++;

            c[i]++;
            i = 1;

        }
        else
        {
            c[i] = 0;
            i++;
        }

    }


    //sprawdzenie poprawnosci
    std::cout << "counter: "<< counter << "\nExpected value: " << expectedCounter <<std::endl;
    if(counter == expectedCounter)
        std::cout << "Wynik zgadza sie z oczekiwanym wynikiem \n";
    else
        std::cout << "Wynik NIE zgadza sie z oczekiwanym wynikiem \n";


    //zwolnienie zaalokowanej pamieci
    delete c;
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
            {
            std::cout << "Podaj n: ";
            std::cin >> n;
            std::cout << "Podaj kolejne elementy tablicy: ";
            char * A = new char[n];
            for(int i = 0; i < n; i++)
                std::cin >> A[i];
            Generate(n, A);
            cout << "Click anything to continue \n";
            getchar(); getchar();

            delete A;
            break;
            }

        case 2:
            Credits();
            getchar(); getchar();
            break;
        case 3:
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
