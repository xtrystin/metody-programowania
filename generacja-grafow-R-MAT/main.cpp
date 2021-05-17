#include <iostream>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <cmath>
#include <fstream>

#define DEBUG 0
using namespace std;


void Generator(const int a, const int b, const int c, const int d, const int n, const int firstX, const int lastX, const int firstY, const int lastY, int ** A, const bool isDirected)
{
    printf("firstX: %d, lastX: %d, firstY: %d, lastY: %d, n: %i \n", firstX, lastX, firstY, lastY, n);
    //zakoncz gdy zostanie 1 komorka obszaru
    if(n == 1 && firstX == lastX && firstY == lastY)
    {
        std::cout << "WPISANIE \n\n";
        //sprawdzenie czy wybrana komorka lezy na glownej przekatnej macierzy zakldama ze wierzcholek sasiaduje z samym soba
        if(firstX == firstY)
        {
            A[firstX][firstY] = 0;
            return;
        }

        //sprawdzenie czy krawedz juz istnieje
        if(A[firstY][firstX] == 1)
            return;
        else
        {
            std::cout << "TAK \n";

            if(isDirected)
                A[firstY][firstX] = 1;  //na odwrot, bo w tablicach c++ x jest w dol a y w gore

            else
            {
                A[firstY][firstX] = 1;
                A[firstX][firstY] = 1;
            }
            printf("X: %i, Y: %i, A[x][y]: %i \n", firstX, firstY, A[firstX][firstY]);

        }



        //std::cout << "OSTATNIE: \n";
        //printf("firstX: %d, lastX: %d, firstY: %d, lastY: %d, n: %i \n", firstX, lastX, firstY, lastY, n);

        return;
    }

    //podzialy obszaru prawdopodobienstwa
    /*double poczatekA = 0.01;
    double koniecA = a;
    double poczatekB = koniecA + 0.01;
    double koniecB = koniecA + b;
    double poczatekC = koniecB + 0.01;
    double koniecC = koniecB + c;
    double poczatekD = koniecC + 0.01;
    double koniecD = koniecC + d;
*/




    int randInt = rand() % 100 + 1;     //losuj od 1 do 100
    //double p = (double)randInt / 100.0;
    int p = randInt;

    //podzialy obszaru prawdopodobienstwa
    int poczatekA = 1;
    int koniecA = a;
    int poczatekB = koniecA + 1;
    int koniecB = koniecA + b;
    int poczatekC = koniecB + 1;
    int koniecC = koniecB + c;
    int poczatekD = koniecC + 1;
    int koniecD = koniecC + d;
    printf("p: %i, poczatekA: %i, koniecA: %i, poczatekB: %i, koniecB: %i, poczatekC: %i, koniecC: %i, poczatekD: %i, koniecD: %i \n", p,poczatekA,koniecA,poczatekB,koniecB,poczatekC,koniecC, poczatekD,koniecD);

    //sprawdzenie poprawnosci
    if(a+b+c+d != 100)
    {
        std::cerr << "ERROR a+b+c+d != 100\n";
        printf("koniecD: %i a+b+c+d: %i", koniecD, a+b+c+d);
        exit(0);
    }

   // printf("p: %i, poczatekA: %i, koniecA: %i, poczatekB: %i, koniecB: %i, poczatekC: %i, koniecC: %i, poczatekD: %i, koniecD: %i \n", p,poczatekA,koniecA,poczatekB,poczatekC,koniecB,koniecC, poczatekD,koniecD);
    //wybierz obszar
    if(p >= poczatekA && p <= koniecA)
    {
        cout << "a\n";
        Generator(a,b,c,d,n/2,firstX, (firstX + lastX)/2, firstY, (firstY+lastY)/2, A, isDirected);
    }
    else if(p >= poczatekB && p <= koniecB)
    {
        cout << "b\n";
        Generator(a,b,c,d,n/2, (firstX + lastX)/2 + 1, lastX, firstY, (firstY+lastY)/2, A, isDirected);
    }
    else if(p >= poczatekC && p <= koniecC)
    {
        cout << "c\n";
        Generator(a,b,c,d,n/2, firstX, (firstX + lastX)/2, (firstY+lastY)/2 + 1, lastY, A, isDirected);
    }
    else if(p >= poczatekD && p <= koniecD)
    {
        cout << "d\n";
        Generator(a,b,c,d,n/2, (firstX + lastX)/2 + 1, lastX, (firstY+lastY)/2 + 1, lastY, A, isDirected);
    }
    else
        std::cerr << "ERROR IF \n\n\n\n" << koniecD << "\n";

}

void GenerateRMat(const int a, const int b, const int c, const int d, const int N, const double density, const bool isDirected)
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


    int n = pow(2,(int)log2((double)N)); //ilosc wezlow w R-Mat grafie
    //std::cout << "n: " << n << std::endl;

    //utworzenie tablicy z numeracja od 1 (pierszwa kolumna i wierz sa ignorowane)
    int ** A = new int*[n+1];
    for(int i = 1; i <= n; i++)
        A[i] = new int[n+1];

    int totalEdges = 0;     //ilosc wszystkich krawedzi
    int filledEdges = 0;    //ilosc zajetych krawedzi
    double currentDensity = 0;     // gestosc: ilosc krawedzi / liczba wszystkich krawedzi
    //wyzerowanie macerzy sasiedztwa
    for(int i = 1; i <= n; i++)
    {
        for(int j = 1; j <= n; j++)
        {
            A[i][j] = 0;
            totalEdges++;

            //wierzcholek sasiaduje sam ze soba
            //if(i == j)
            //    A[i][j] = 1;

        }
    }
    //std::cout << totalEdges <<std::endl;


    srand(time(NULL));

  /* REKURENCYJNIE dopoki n == 1  wtedy wypelnij A[][] jedynka
   rejony (obszary)sa te same ale szanse na to ze tam wpadnie sa rozne
   obszar w ktory sie rekurencyjnie weszlo jest traktowany jako nowy obszar z odpowiednia iloscia n
    wywoalnie rekurencyjnie z argumentami: poczatkowe n koncowe n w poziomie i w pionie (4args) + n + ...
*/
    while(1)
    {
        // wyzerowanie zmiennych pomocniczych
        filledEdges = 0;
        currentDensity = 0;

        Generator(a,b,c,d,n, 1, n, 1, n, A, isDirected); //generowanie 1 komórki

        //policz ilosc zapelnionych krawedzi
        for(int i = 1; i <= n; i++)
            for(int j = 1; j <= n; j++)
                if(A[i][j] == 1)
                    filledEdges++;

        currentDensity  = (double)filledEdges / (double)totalEdges;
        std::cout << "totalEdges: " << totalEdges << " filled: " << filledEdges << " currentDensity : " << currentDensity  << " density: " << density << std::endl;

        //jesli gestosc wystarczajaca to wyjdz
        if(currentDensity >= density)
            break;
    }
    // wyswietl macierz
    for(int i = 1; i <= n; i++)
    {
        if(i == n/2 + 1)
        {
            for(int j =1; j<=n; j++)
            {
                std::cout << " - ";
                file << " - ";
            }

            std::cout << '\n';
            file << '\n';
        }

        for(int j = 1; j <= n; j++)
        {
            if(j == n/2 + 1)
            {
                std::cout << " | ";
                file << " | ";
            }

            std::cout << A[i][j] << ' ';
            file << A[i][j] << ' ';
        }
        std::cout <<std::endl;
        file << '\n';
    }

    //zliczenie jedynek w poszczegolnych cwiartkach
    int aCounter=0;
    int bCounter=0;
    int cCounter=0;
    int dCounter=0;
    for(int i = 1; i <= n; i++)
    {
        for(int j = 1; j <= n; j++)
        {
            if(A[i][j] == 1)
            {
            if(i <= n/2 && j <= n/2)
                aCounter++;
            else if(i <= n/2 && j > n/2)
                bCounter++;
            else if(i > n/2 && j <= n/2)
                cCounter++;
            else if(i > n/2 && j > n/2)
                dCounter++;
            else
                std::cerr << "ERROR \n";
            }

        }
    }
    std::cout << "Ilosc jedynek w poszczegolnych cwiartkach \n";
    printf("A: %i B: %i C: %i D %i\n",aCounter, bCounter, cCounter, dCounter);


    //utworzenie wyzerowanej tablicy przechowujacej stopnie wierzcholkow
    int * degree = new int[n+1];
    for(int i = 1; i <= n; i++)
            degree[i] = 0;
    //obliczanie stopni wierzcholkow
    for(int i = 1; i <= n; i++)
    {
        for(int j = 1; j <= n; j++)
        {
            if(A[i][j] == 1)
                degree[i]++;
        }
    }
    //zapisanie stopni wierzholkow do pliku;
    ofstream file2;
    file2.open("stopnie.txt");


    double mean = 0.0;
    int minDegree = 10000000000;
    int maxDegree = 0;
    //wyswietlenie stopni wierzcholkow oraz liczenie sredniej
    for(int i = 1; i <= n; i++)
    {
        printf("wierzcholek nr %i ma stopnien: %i \n", i, degree[i]);
        file2 << degree[i] << '\n';
        mean += degree[i];
        if(degree[i] >= maxDegree)
            maxDegree = degree[i];
        if(degree[i] <= minDegree)
            minDegree= degree[i];
    }
    mean = mean / n;
    printf("Srednia wartosc stopnia: %f, minimalny stopnien: %i, maksymalny stopnien: %i \n", mean, minDegree, maxDegree);



    //zwolnienie pamieci
    for(int i = 1; i <= n; i++)
        delete A[i];
    delete A;

    delete degree;

    file.close();
    file2.close();
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
    std::cout << "Program sluzy do generowania grafow R-MAT \n";

    //prawdopodobienstwo
    int a = 5;
    int b = 20;
    int c = 15;
    int d = 60;
    //printf("a: %i b: %i c: %i d: %i\n", (int)(a*100), (int)b*100, (int)c*100, (int)d*100);
    //TODO: sprawdzic czy !=0 !=0 a+b+c+d == 1

    int N = 15; //ilosc wierzcholkow w prawdziwym grafie
    double density = 0.5;  //gestosc
    bool isDirected = 0;

    #if DEBUG == 0
    std::cout << "Podaj prawdopodobienstwa w skali 0 - 100, a+b+c+d musi = 100 \n";
    std::cout << "a: "; std::cin >> a;
    std::cout << "b: "; std::cin >> b;
    std::cout << "c: "; std::cin >> c;
    std::cout << "d: "; std::cin >> d;
    std::cout << "Podaj N: ";
    std::cin >> N;
    std::cout << "Podaj gestosc grafu: ";
    std::cin >> density;
    std::cout << "Podaj rodzaj grafu (0 nieskierowany, 1 skierowany): ";
    std::cin >> isDirected;
    #endif // DEBUG

    GenerateRMat(a,b,c,d,N, density, isDirected);


    return 0;
}
