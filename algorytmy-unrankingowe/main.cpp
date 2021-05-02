#include <iostream>
#include <fstream>
#include <stdio.h>
#include<windows.h>
#include <chrono>  //mierzenie czasu wykonania

#define DEBUG 0
#define ZESTAWIENIE 0
using namespace std;


int Factorial(int a)    //oblicza a!
{
    if(a == 0)
        return 1;
    else
    return a * Factorial(a-1);  //rekursja
}

//oblicza n! / ( k! * (n-k)! )
int C(const int n, const int k)
{
    return ( Factorial(n) / (Factorial(k) * Factorial(n-k)) );
}

void UnrankCombA(int n, int k, int N, int ** PTA)
{
    #if DEBUG == 0
     //otworzenie pliku
    std::ofstream file; //utworzenie uchwytu do pliku
    char saveIntoFile;
    std::cout << "Czy zapisac wyniki do pliku 'wyniki.txt'? [y/n]: ";
    std::cin >> saveIntoFile;
    if(saveIntoFile == 'y')
    {
        file.open("wyniki.txt");
        if(file.fail())
        {
            std::cerr << "ERROR:: NIE MOZNA ZAPiSAC WYNIKOW:: \n";
            exit(0);
        }
    }
    #endif // DEBUG

    int * K = new int[k+1];

    int t = n-k+1;
    int m = k;
    int N2 = PTA[t][m] + PTA[t+1][m-1] - 1 - N;

    do
    {
        if(PTA[t][m] <= N2)
        {
            K[k-m+1] = n-t-m+2;
            N2 = N2 - PTA[t][m];
            m--;
        }
        else
            t--;



    }while(m != 0);

    // wyswietl wynik
    for(int i = 1; i <=k; i++)
    {
        std::cout << K[i];
        #if DEBUG == 0
        file << K[i];
        #endif // DEBUG
    }

    //std::cout << std::endl;


    delete K;
    #if DEBUG == 0
    file.close();
    #endif // DEBUG
}

//funkcja pomocnicza binary search
void BS(int N2, int ** PTA, int m, int w, int u, int& t)
{
    t = (w+u) / 2;                                                  //printf("t: %i m: %i N2: %i w: %i u: %i PTA[t][m]: %i PTA[t+1][m]: %i\n", t, m, N2, w, u, PTA[t][m], PTA[t+1][m]);
    if((w+u) % 2 != 0)
        t++;
    if(N2 < PTA[t][m])
        BS(N2, PTA, m, w, t-1, t);
    else if(N2 >= PTA[t+1][m])
        BS(N2, PTA, m, t, u, t);
    return;
}

void UnrankCombC(int n, int k, int N, int ** PTA)
{
     //otworzenie pliku
    std::ofstream file; //utworzenie uchwytu do pliku
    char saveIntoFile;
    std::cout << "Czy zapisac wyniki do pliku 'wyniki.txt'? [y/n]: ";
    std::cin >> saveIntoFile;
    if(saveIntoFile == 'y')
    {
        file.open("wyniki.txt");
        if(file.fail())
        {
            std::cerr << "ERROR:: NIE MOZNA ZAPiSAC WYNIKOW:: \n";
            exit(0);
        }
    }



    int * K = new int[k+1];


    int t = n-k+1;
    int N2 = PTA[t][k] + PTA[t+1][k-1] - 1 - N;

    for(int m = k; m >= 1; m--)
    {
        BS(N2, PTA, m, 1, t, t);
        //std::cout << t << '\n';
        K[k-m+1] = n-t-m+2;
        N2 = N2 - PTA[t][m];
    }
    // wyswietl wynik
    for(int i = 1; i <=k; i++)
    {
        std::cout << K[i];
        file << K[i];

    }
    std::cout << std::endl;


    file.close();
    delete K;
}

void UnrankCombD(int n, int k, int N)
{
     //otworzenie pliku
    std::ofstream file; //utworzenie uchwytu do pliku
    char saveIntoFile;
    std::cout << "Czy zapisac wyniki do pliku 'wyniki.txt'? [y/n]: ";
    std::cin >> saveIntoFile;
    if(saveIntoFile == 'y')
    {
        file.open("wyniki.txt");
        if(file.fail())
        {
            std::cerr << "ERROR:: NIE MOZNA ZAPiSAC WYNIKOW:: \n";
            exit(0);
        }
    }


    int * K = new int[k+1];

    int e = C(n,k);
    int N2 = e-1-N;
    e = (n-k) * e / n;
    int t = n-k+1;
    int m = k;
    int p = n-1;

    do
    {
        if(e <= N2)
        {
            K[k-m+1] = n-t-m+2;
            if(e > 0)
            {
                N2 = N2-e;
                e = m*e/p;
            }
            m--;
            p--;
        }
        else
        {
            e = (p-m)*e / p;
            t--;
            p--;
        }

    }while(m != 0);

        // wyswietl wynik
    for(int i = 1; i <=k; i++)
    {
        std::cout << K[i];
        file << K[i];
    }

    std::cout << std::endl;

    file.close();
    delete K;
}



// tworzy trojkat Paskala ze wspolczynnikami
void CalculatePTA(int ** PTA, int n)
{
    for(int i = 1; i <=n; i++)
        PTA[1][i] = 0;
    for(int i = 2; i <=n; i++)
        PTA[2][i] = 1;
    for(int i = 1; i <=n; i++)
        PTA[i][1] = i-1;
    for(int i = 3; i <=n; i++)
    {
        for(int j = 2; j <=n; j++)
        {
            PTA[i][j] = PTA[i-1][j] + PTA[i][j-1];
        }
    }

    /* debug
    for(int i = 1; i <=n; i++)
    {
        for(int j = 1; j <=n; j++)
            std::cout << PTA[i][j];
        std::cout << std::endl;
    }*/




}


void UnrankSemba(int ** T, int n, int N)
{
      //otworzenie pliku
    std::ofstream file; //utworzenie uchwytu do pliku
    char saveIntoFile;
    std::cout << "Czy zapisac wyniki do pliku 'wyniki.txt'? [y/n]: ";
    std::cin >> saveIntoFile;
    if(saveIntoFile == 'y')
    {
        file.open("wyniki.txt");
        if(file.fail())
        {
            std::cerr << "ERROR:: NIE MOZNA ZAPiSAC WYNIKOW:: \n";
            exit(0);
        }
    }


    int * digits = new int[n+1];
    int Nnext = N;
    int firstDigit =0;
    int border = 0;
        //pierwsza petlamoze wpisacc tylko cyfry od 1 do n, kolejna 2 do n, kolejna 3 do n, ...

    for(int a = n-1+1; a>= 1; a--)  //sprawdza po kolei kolejne obszary
    {
        border += T[a][2];
        //std::cout << "    "<< T[a][2]<<" ";
        if(N <= border)
        {
            printf("%i ", n-a+1);
            file << (n-a+1);
            firstDigit = n-a+1;
            digits[1] = firstDigit;

            border = border - T[a][2];  //ustawienie na liczbe pominietych elementow
            //printf("border1: %i \n", border);
            if(firstDigit != 1)
                Nnext = N - border; // nastepna lokalna pozycja
            else
                Nnext = N;
            break;
        }
    }

    int lastPos = 1000000;
    int digID = 1;
    for(int j = firstDigit; j< n; j++)
    {
        //printf("digits[%i]: %i \t j: %i \t Nnext: %i \n", digID, digits[digID], j, Nnext);
       // if(digits[digID] == j)
       // {
            // T[n-j+1][2] to jest ilosc z tablicy
            border = T[n-j+1][1];     //to co jest po lewej: liczba rzeczy z zerem czyli jedynka
            //printf("Nnxt: %i \n", Nnext);
            if(Nnext <= border)     // obszar 0
            {
                break;
                 //printf("Obszar: %i \n", 0);
                 //digits[digID+1] = 0;
                 //digID++;
                 //break;
            }
            int nextDigit = digits[digID]+1;       //kolejna cyfra
            for(int pos = n-j+1-1; pos >= 1; pos--) //inne obszary
            {
                //wyrownanie wywolan tablicy T -> ma schodzic coraz nizej
                if(lastPos == pos)  // po kazdej cyfrze idz wiersz w T do gory, dopasowuje do tabeli T
                    pos--;
                else if(lastPos < pos)
                    pos = lastPos - 1;


                //printf("pos: %i \n", pos);
               //printf("border: %i \t T[%i][2]: %i \n", border, pos, T[pos][2]);
                border += T[pos][2];
               //printf("border: %i \t T[%i][2]: %i \n", border, pos, T[pos][2]);
                if(Nnext <= border)
                    {
                        //printf("IF: border: %i \t T[%i][2]: %i \n", border, pos, T[pos][2]);
                        lastPos = pos;
                        //printf("lastPos: %i \n", lastPos);

                        printf("%i ", nextDigit);
                        file << nextDigit;

                        digits[digID+1] = nextDigit;

                        Nnext = Nnext - border + T[pos][2]; // wyliczenie kolejnej lokalnej pozycji


                        digID++;
                        nextDigit++;
                        break;
                    }
                nextDigit++;    //jak ma wejsc w kolejna granice to zwieksz oczekiwana liczbe



            }


    }


    file << '\n';
    file.close();
}


void CreateT(int ** T, int n)
{

    for(int i = 1; i <= n; i++)
        T[i][1] = 1;
    T[1][1] = 1;
    T[1][2] = 1;

    int tmp = 0;
    for(int i = 2; i <= n; i++)
    {
        tmp = T[i][1];
        for(int j = i-1; j>=1; j--)
            tmp += T[j][2];

        T[i][2] = tmp;
    }

}


void menu()
{

    cout << "*************************************************" << endl;
    cout << "*                    MENU                       *" << endl;
    cout << "*                                               *" << endl;
    cout << "*                                               *" << endl;
    cout << "*        1. UnrankComb-A                        *" << endl;
    cout << "*        2. UnrankComb-C                        *" << endl;
    cout << "*        3. UnrankComb-D                        *" << endl;
    cout << "*        4. UnrankSemba                         *" << endl;
    cout << "*        5. Exit                                *" << endl;
    cout << "*                                               *" << endl;
    cout << "*                                               *" << endl;
    cout << "*************************************************" << endl;
    //cout << endl;
    cout << "Wybierz opcje: ";

}
void Exit()
{
    std::cout << "Leaving..." << std::endl;
    Sleep(1000);
    exit(0);
}

int main(int argc, char ** argv)
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



    std::cout << "Program realizuje funkcje UnrankCombA, C, D UnrankSemba dla danych liczb calkowitych n,k,N, gdzie N>0, 1<=k<=n \n";
    int choice;
    int n;
    int k;
    int N;

    while(1)
    {
        menu();
        std::cin >> choice;
        system("cls");

        switch(choice)
        {
        case 1:
            {
            std::cout << "Podaj n: ";
            std::cin >> n;
            std::cout << "Podaj k: ";
            std::cin >> k;
            std::cout << "Podaj N: ";
            std::cin >> N;
            int ** PTA = new int*[n+1];
            for(int i = 1; i <= n; i++)
                PTA[i] = new int[n+1];
            CalculatePTA(PTA, n);
            UnrankCombA(n,k,N, PTA);
            cout << "\nClick anything to continue \n";
            getchar(); getchar();

                //zwolnienie pamieci
            for(int i = 1; i <= n; i++)
                delete PTA[i];
            delete PTA;
            }
            break;
        case 2:
            {
            std::cout << "Podaj n: ";
            std::cin >> n;
            std::cout << "Podaj k: ";
            std::cin >> k;
            std::cout << "Podaj N: ";
            std::cin >> N;
            int ** PTA = new int*[n+1];
            for(int i = 1; i <= n; i++)
                PTA[i] = new int[n+1];
            CalculatePTA(PTA, n);
            UnrankCombC(n,k,N, PTA);
            cout << "\nClick anything to continue \n";
            getchar(); getchar();

                //zwolnienie pamieci
            for(int i = 1; i <= n; i++)
                delete PTA[i];
            delete PTA;
            }
            break;
        case 3:
            {
            std::cout << "Podaj n: ";
            std::cin >> n;
            std::cout << "Podaj k: ";
            std::cin >> k;
            std::cout << "Podaj N: ";
            std::cin >> N;
            UnrankCombD(n,k,N);
            cout << "\nClick anything to continue \n";
            getchar(); getchar();
            }
            break;
        case 4:
            {
            std::cout << "Podaj n: ";
            std::cin >> n;
            std::cout << "Podaj N: ";
            std::cin >> N;
            int ** T = new int*[n+1];
            for(int i = 1; i <= n; i++)
                T[i] = new int[n+1];
            CreateT(T, n);
            UnrankSemba(T, n, N);
            cout << "\nClick anything to continue \n";
            getchar(); getchar();

            //zwolnienie pamieci
            for(int i = 1; i <= n; i++)
                delete T[i];
            delete T;
            getchar();
            }
            break;
        case 5:
            Exit();
            break;
        default:
            std::cerr << "ERROR podana opcja nie istnieje \n";
            Sleep(1000);
            break;
        }
        system("cls");

    }




    //debug
    /*int n = 15;
    int k = 10;
    int N = 40;
    int ** PTA = new int*[n+1];
    for(int i = 1; i <= n; i++)
        PTA[i] = new int[n+1];

    #if ZESTAWIENIE == 0
    CalculatePTA(PTA, n);
    UnrankCombC(n,k,N, PTA);
    #endif // ZESTAWIENIE
    */







    #if ZESTAWIENIE == 1
     //obsluga pliku
    std::ofstream file; //utworzenie uchwytu do pliku
    char saveIntoFile;
    std::cout << "Czy zapisac wyniki do pliku 'wyniki.txt'? [y/n]: ";
    std::cin >> saveIntoFile;
    if(saveIntoFile == 'y')
    {
        file.open("zestawienie.txt");
        if(file.fail())
        {
            std::cerr << "ERROR:: NIE MOZNA ZAPiSAC WYNIKOW:: Podzialy2() \n";
            exit(0);
        }
    }
    double mean = 0;
    for(n = 4; n <=20; n++) //testowanie
    {
        k = n/2;
        N = n/2 - 1;
        mean = 0;
        //alokwacja niezbednej pamieci
        int ** PTA = new int*[n+1];
        for(int i = 1; i <= n; i++)
            PTA[i] = new int[n+1];

        for(int i = 0; i< 100; i++)
        {
            auto t1 = std::chrono::high_resolution_clock::now();
            CalculatePTA(PTA, n);
            Sleep(1);   //potrzebne do mierzenia czasu
            UnrankCombC(n,k,N, PTA);
            auto t2 = std::chrono::high_resolution_clock::now();

            auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
            //std::cout << "Czas wykonania: " << (ms_int.count() - 1) << '\n';

            mean += (double)(ms_int.count() - 1);
            //printf("Mean: %f", mean);
        }
        mean = mean / 100.f;
        printf("n: %i k: %i srednia: %f N: %i\n",n,k,mean, N);
        file << mean << ' ';

    }
    file.close();
    #endif // ZESTAWIENIE




    return 0;
}
