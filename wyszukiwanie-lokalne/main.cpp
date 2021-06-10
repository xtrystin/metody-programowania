#include <iostream>
#include <stdlib.h>
#include <vector>
#include <bitset>
#include <time.h>
#include <fstream>
#include <windows.h>
#include <chrono>

#define DEBUG 0
#define WYPISZ 0    //czy wypisac wyniki posrednie kazdej iteracji
#define ZESTAWIENIE 0   //lepszy pomiar czasu

using namespace std;
using namespace std::chrono;

//sprawdza czy wszystkie implicenty maja wartosc 1
bool IsStatisfied(int * implicentScore, int N)
{
    for(int i=0; i < N; i++)
    {
        if(implicentScore[i] == 0)
            return false;
    }
    return true;
}


void WalkSat(int n, int N, int l, int ** implicentsVector)
{
    char loadToFile;
    #if ZESTAWIENIE == 0
    std::cout << "Zapisac wynik do pliku wynik.txt?[y/n]: ";
    std::cin >> loadToFile;
    #endif // ZESTAWIENIE
    #if ZESTAWIENIE == 1
    loadToFile = 'n';
    #endif // ZESTAWIENIE
    ofstream of;
    if(loadToFile == 'y')
    {
        of.open("wynik.txt");
        if(of.fail())
        {
            std::cerr << "File error \n";
            exit(-1);
        }
    }


    srand(time(NULL));
    int * logicalVars = new int[n];

    //ustawienie poczatkowych wartosci zmiennych logicznych x na wartosci losowe
     std::cout << "Poczatkowa (losowa) wartosc zmiennych: ";
    for(int i = 0; i < n; i++)
    {
        int randVal = rand() % 2;
        logicalVars[i] = randVal;
        std::cout << logicalVars[i];
    }
    std::cout << '\n';

    int * implicentScore = new int[N];   //wektor wartosci kolejnych implicentow

    int iterationsCouter = 0;      //licznik wykonanych iteracji

    while(1)
    {
         //sprawdz wartosci implicentow
       for(int i = 0; i < N; i++)
       {
           if(implicentsVector[i][0] == 0)
           {
               implicentScore[i] = ~logicalVars[0] + 2;    //negacja
               #if WYPISZ == 1
                std::cout << implicentScore[i] << '\n';
               #endif // WYPISZ
           }

            else if(implicentsVector[i][0] != -1)       //-1 oznacza brak literalu
            {
                implicentScore[i] = logicalVars[0];
                #if WYPISZ == 1
                std::cout << implicentScore[i] << '\n';
                #endif // WYPISZ
            }


            for(int j = 1; j < l; j++)
            {
                if(implicentsVector[i][j] == 0)
                    implicentScore[i] = implicentScore[i] | (~logicalVars[j] + 2);    //negacja
                else if(implicentsVector[i][j] != -1)
                {
                    implicentScore[i] = implicentScore[i] | logicalVars[j];
                }
                #if WYPISZ == 1
                std::cout << implicentScore[i] << '\n';
                #endif // WYPISZ
            }
            #if WYPISZ == 1
            std::cout << "Implicent nr " << i+1 << ": " << implicentScore[i] << '\n';
            #endif // WYPISZ
       }
       //jesli warunek spelnialnosci jest spelniony to przerwij
        if(IsStatisfied(implicentScore, N))
        {
            std::cout << "Wykonano iteracji: " << iterationsCouter << '\n';
            std::cout << "Wynik: ";
            for(int i=0; i < n; i++)
            {

                std::cout << 'x'<< i << '=' << logicalVars[i] << ' ';
                if(loadToFile == 'y')
                    of << 'x'<< i << '=' << logicalVars[i] << ' ';
            }

            cout << "\n satisfied \n";
            return;
        }

        //jesli nie spelniony to wybierz losowy implicent o wartosci 0
        //policz ilosc implicentow o wartosci 0
        int counter=0;
        while(1)
        {
            int randId = rand() % N;
            if(implicentScore[randId] == 0) //jesli wybrano implicent o wartosci 0 zakoncz petle
                break;
        }
        //wybierz losowo zmienna i zaneguj ja
        int randVarId = rand() % n;
        logicalVars[randVarId] = (~logicalVars[randVarId] + 2);


        #if WYPISZ == 1
        std::cout << "Kolejna wartosc zmiennych: ";
        for(int i = 0; i < n; i++)
            std::cout << logicalVars[i];
        std::cout << '\n';
        #endif // DEBUG


        //jesli liczba iteracji przkroczy 1000000 to przerwij program
        if(iterationsCouter > 100000)
        {
            std::cout << "stop_bez_rozwiazania, wykonano iteracji: " << iterationsCouter << '\n';
            if(loadToFile == 'y')
                of << "stop_bez_rozwiazania";
            return;
        }
        iterationsCouter++;
    }


/*
    for(int i = 0; i < N; i++)
    {
        bool score = implicentsVector[i][0];    //pierwsza zmienna
        for(int j = 0+1; j < l; j++)
        {
            score = score & implicentsVector[i][j];
           // std::cout << score << '\n';
        }
    }
*/

}

bool IsInTabuList(int * tabu, int id, int t)
{
    for(int i=0; i < t; i++)
    {
        if(tabu[i] == id)
            return true;
    }
    return false;
}

void AddToTabu(int * tabu, int id, int t)
{
    //jesli jest juz w liscie tabu to nie dodawaj
    if(IsInTabuList(tabu, id, t))
       return;
    //przesun o 1 do przodu i dodaj nowy element id na zerowa pozycje
    for(int i = t-1; i > 0; i--)
        std::swap(tabu[i], tabu[i-1]);
    tabu[0] = id;
}


void GSATtabu(int n, int N, int l, int t, int ** implicentsVector)
{
    char loadToFile;
    #if ZESTAWIENIE == 0
    std::cout << "Zapisac wynik do pliku wynik.txt?[y/n]: ";
    std::cin >> loadToFile;
    #endif // ZESTAWIENIE
    #if ZESTAWIENIE == 1
    loadToFile = 'n';
    #endif // ZESTAWIENIE
    ofstream of;
    if(loadToFile == 'y')
    {
        of.open("wynik.txt");
        if(of.fail())
        {
            std::cerr << "File error \n";
            exit(-1);
        }
    }

    srand(time(NULL));
    int * logicalVars = new int[n];


    //ustawienie poczatkowych wartosci zmiennych logicznych x na wartosci losowe
    std::cout << "Poczatkowa (losowa) wartosc zmiennych: ";
    for(int i = 0; i < n; i++)
    {
        int randVal = rand() % 2;
        logicalVars[i] = randVal;
        std::cout << logicalVars[i];
    }
    std::cout << '\n';


    int * implicentScore = new int[N];   //wektor wartosci kolejnych implicentow

    int iterationsCouter = 1;      //licznik wykonanych iteracji

    //lista tabu
    int * tabu = new int[t];
    for(int i = 0; i < t; i++)
        tabu[i] = 99999;        //na poczatku lista tabu 'jest pusta'

    while(1)
    {
         //sprawdz wartosci implicentow
       for(int i = 0; i < N; i++)
       {
           if(implicentsVector[i][0] == 0)
           {
                implicentScore[i] = ~logicalVars[0] + 2;    //negacja
                #if WYPISZ == 1
                std::cout << implicentScore[i] << '\n';
                #endif // WYPISZ
           }
            else if(implicentsVector[i][0] != -1)
            {
                implicentScore[i] = logicalVars[0];
                #if WYPISZ == 1
                std::cout << implicentScore[i] << '\n';
                #endif // WYPISZ
            }

            for(int j = 1; j < l; j++)
            {
                if(implicentsVector[i][j] == 0)
                    implicentScore[i] = implicentScore[i] | (~logicalVars[j] + 2);    //negacja
                else if(implicentsVector[i][j] != -1)
                    implicentScore[i] = implicentScore[i] | logicalVars[j];
                    #if WYPISZ == 1
                std::cout << implicentScore[i] << '\n';
                #endif // WYPISZ
            }
            #if WYPISZ == 1
            std::cout << "Implicent nr " << i+1 << ": " << implicentScore[i] << '\n';
            #endif // WYPISZ
       }
       //jesli warunek spelnialnosci jest spelniony to przerwij
        if(IsStatisfied(implicentScore, N))
        {
            std::cout << "Wykonano iteracji: " << iterationsCouter << '\n';
            std::cout << "Wynik: ";
            for(int i=0; i < n; i++)
            {

                std::cout << 'x'<< i << '=' << logicalVars[i] << ' ';
                if(loadToFile == 'y')
                    of << 'x'<< i << '=' << logicalVars[i] << ' ';
            }

            cout << "\n satisfied \n";
            return;
        }

        //jesli nie spelniony to wybierz zmienna ktorej
        //  negacja minimalizuje ilosc implicentow o wartosci 0

        //po kolei dla kazdej zmiennej 1.zaneguj zmienna 2.sprawdz ilosc implicentow o wartosci 0
        //    3. wybierz najoptymalniejsza zmienna i ja zaneguj

        int * numberOf0Implicents = new int[n]; //licznik ilosci implicentow o wartosci 0 dla kazdej zmiennej
        for(int i=0; i < n; i++)
            numberOf0Implicents[i] = 0;

        for(int id = 0; id < n; id++)
        {
            logicalVars[id] = (~logicalVars[id] + 2); //negacja zmiennej

            //sprawdz wartosci implicentow dla wyzej zanegowanej zmiennej
           for(int i = 0; i < N; i++)
           {
               if(implicentsVector[i][0] == 0)
                    implicentScore[i] = ~logicalVars[0] + 2;    //negacja
                else if(implicentsVector[i][0] != -1)
                    implicentScore[i] = logicalVars[0];
                for(int j = 1; j < l; j++)
                {
                    if(implicentsVector[i][j] == 0)
                        implicentScore[i] = implicentScore[i] | (~logicalVars[j] + 2);    //negacja
                    else if(implicentsVector[i][j] != -1)
                        implicentScore[i] = implicentScore[i] | logicalVars[j];

                }

               if(implicentScore[i] == 0)
               {
                   numberOf0Implicents[id]++;
               }

           }
           #if WYPISZ == 1
           printf("Koszt %i: %i \n", id, numberOf0Implicents[id]);
           #endif // WYPISZ
           //cofnij zanegowanie
            logicalVars[id] = (~logicalVars[id] + 2);

        }
        // wybierz najoptymalniejsza zmienna

        int * cost = new int[n];    //koszt dla wyboru danej zmiennej
        int aspirationLvl = 100000;          //pozniej przyjmuje wartosc /*drugiego*/ najmniejszego kosztu
            //znajdz wartosc najmniejsza zer
        int min0 = 100000;      //ilosc implicentow
        int min0id = 10000;     //id zmiennej
        for(int id=0; id < n; id++)
        {
            if(numberOf0Implicents[id] <= min0)
            {
                cost[id] = numberOf0Implicents[id];     //koszt wyoru danej zmiennej jest rowny ilosci implicentow o wartosci 0 -> im mniiej tym lepszy koszt
                min0 = numberOf0Implicents[id];
                min0id = id;
            }
        }

        //znajdz poziom aspiracji -> czyli najlepszy koszt zmiennej ktora nie znajduje sie na liscie tabu
        for(int id=0; id < n; id++)
        {
            if(cost[id] <= aspirationLvl && IsInTabuList(tabu, id, t) == false)
            {
                aspirationLvl = cost[id];     // plus1?
            }
        }

        //sprawdzenie czy zmienna o danym indexie znajduje sie na liscie tabu
        while(IsInTabuList(tabu, min0id, t))
        {
            int newMin0id;
            #if WYPISZ == 1
            printf("min0id: %i\n", min0id);
            #endif // WYPISZ
            if(cost[min0id] < aspirationLvl )
            {
                #if WYPISZ == 1
                printf("Koszt o wartosci %i jest mniejszy niz poziom aspiracji o wartosci %i \n", cost[min0id], aspirationLvl);
                #endif // WYPISZ
                break;
            }
            else
            {
                int newMin0 = 10000;
                newMin0id = 10000;
                //wybierz inna zmienna
                for(int id=0; id < n; id++)
                {

                    if(cost[id] <= newMin0 && id != min0id)
                    {
                        newMin0 = cost[id];
                        newMin0id = id;

                    }
                }


            }
            #if WYPISZ == 1
            printf("newMin0id: %i\n", newMin0id);
            #endif // WYPISZ
            min0id = newMin0id;
        }

            //zaneguj wybrana zmienna
        logicalVars[min0id] = (~logicalVars[min0id] + 2);
        #if WYPISZ == 1
        std::cout << "Neguje zmianna o indexie: " << min0id << " i koszcie: " << cost[min0id] << '\n';
        #endif // WYPISZ
        // umiesc wybrana zmienna na liscie tabu
        AddToTabu(tabu, min0id, t);

        #if WYPISZ == 1
        //wypisanie zawartosci listy tabu
        std::cout << "Lista tabu: ";
        for(int i=0; i < t; i++)
        {
            if(tabu[i] == 99999)    //umowna wartosc pustej pozycji w liscie tabu
                std::cout << "  ";
            else
                std::cout << tabu[i] << ' ';
        }
        std::cout << '\n';

        std::cout << "Kolejna wartosc zmiennych: ";
        for(int i = 0; i < n; i++)
            std::cout << logicalVars[i];
        std::cout << '\n';

        #endif // WYPISZ




        //jesli liczba iteracji przkroczy 1000000 to przerwij program
        if(iterationsCouter > 100000)
        {
            std::cout << "stop_bez_rozwiazania, wykonano iteracji: " << iterationsCouter << '\n';
            if(loadToFile == 'y')
                of << "stop_bez_rozwiazania";
            return;
        }
        iterationsCouter++;
    }

}



void menu()
{

    cout << "*************************************************" << endl;
    cout << "*                    MENU                       *" << endl;
    cout << "*                                               *" << endl;
    cout << "*                                               *" << endl;
    cout << "*        1. WalkSAT                             *" << endl;
    cout << "*        2. GSAT z lista tabu                   *" << endl;
    cout << "*        3. Exit                                *" << endl;
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



    std::cout << "Program realizujacy algorytm wyszukiwania lokalnego (local search) WALKSAT oraz GSAT z lista tabu dla problemu spelnialnosci formul boolowskich SAT. \n";


    int n = 3;  //ilosc zmiennych
    int N = 5;  //ilosc (roznych) implicentow  < 2^n
    int t = 2;      //rozmiar listy tabu
    int l = n;  //ilosc literalow (zmiennych) w implicencie   - stala wartosc
    int ** implicentsVector; //wektor implicentow

    #if DEBUG == 1
    implicentsVector = new int*[N];
    for(int i = 0; i < N; i++)
        implicentsVector[i] = new int[l];

    char c;
    std::cout << "Wczytac dane z pliku dane.txt?[y/n]: ";
    std::cin >> c;
    bool loadFromFile = (c == 'y')? 1: 0;

    if(loadFromFile)
    {
        fstream file;
        file.open("dane.txt");
        if(file.fail())
        {
            std::cerr << "File does not exist \n";
            exit(-1);
        }

        for(int i = 0; i < N; i++)
            for(int j = 0; j < l; j++)
                file >> implicentsVector[i][j];

        file.close();
    }
    else
    {
         std::cout << "Podaj zapis implicentow w postaci wektorowej (np x1+x2+~x3 podaj w postaci: 110) \n";
        for(int i = 0; i < N; i++)
        {
            std::cout << "Implicent " << i+1 << ": \n";
            for(int j = 0; j < l; j++)
            {
                std::cin >> implicentsVector[i][j];
                //std::cout << ~implicentsVector[i][j] + 2;   //negacja
            }
        }
    }

    //wypisanie implicentow
    for(int i = 0; i < N; i++)
    {
        std::cout << "Implicent nr " << i << ' ';
        for(int j = 0; j < l; j++)
        {
            std::cout << implicentsVector[i][j];
        }
        std::cout << '\n';
    }

    WalkSat(n, N, l, implicentsVector);
    GSATtabu(n, N, l, t, implicentsVector);


    #endif // DEBUG

    #if DEBUG == 0
    int choice;
    char c;
    while(1)
    {
        menu();
        std::cin >> choice;
        system("cls");

        switch(choice)
        {
        case 1:
            {
                std::cout << "Wczytac dane z pliku dane.txt?[y/n]: ";
                std::cin >> c;
                bool loadFromFile = (c == 'y')? 1: 0;
                if(loadFromFile)
                {
                    fstream file;
                    file.open("dane.txt");
                    if(file.fail())
                    {
                        std::cerr << "File does not exist \n";
                        exit(-1);
                    }
                    file >> n;
                    file >> N;
                    l = n;
                    implicentsVector = new int*[N];
                    for(int i = 0; i < N; i++)
                        implicentsVector[i] = new int[l];



                    //ustawienie wszystkich zmiennych na nieistniejace
                    for(int i = 0; i < N; i++)
                        for(int j = 0; j < l; j++)
                            implicentsVector[i][j] = -1;

                    //wczytanie z pliku w postaci x0x2-x3 ..
                    string line;
                    getline(file, line);      //pusta linia po uzyciu file >>
                    for(int i = 0; i < N; i++)      //po implicentach
                    {
                        getline(file, line);
                        bool isNeg = 0;
                        for(int j = 0; j < line.size(); j++)  //po literalach
                        {
                            switch(line[j])
                            {
                            case '-':
                                {
                                    isNeg = 1;
                                }
                                break;
                                case 'x':
                                {
                                    //pobranie nastepnego znaku ktorym jest dolny indeks zmiennej
                                    j++;
                                    int id = line[j] - '0';   //zamiana char na int
                                    if(isNeg)
                                    {

                                        implicentsVector[i][id] = 0;       //umieszczenie w wektorze
                                        isNeg = 0;
                                    }
                                    else
                                        implicentsVector[i][id] = 1;

                                }
                                break;
                                default:
                                    std::cerr << "ERROR SWITCH: Mozliwy zly zapis w pliku(lista tabu?) \n";
                                    exit(-1);
                            }
                        }
                    }

                    //wczytanie w postaci wektorowej
                    //for(int i = 0; i < N; i++)
                     //   for(int j = 0; j < l; j++)
                      //      file >> implicentsVector[i][j];

                    file.close();
                }
                else
                {
                    std::cout << "Podaj ilosc zmiennych: ";
                    std::cin >> n;
                    std::cout << "Podaj ilosc roznych implicentow: ";
                    std::cin >> N;
                    l = n;

                    implicentsVector = new int*[N];
                    for(int i = 0; i < N; i++)
                        implicentsVector[i] = new int[l];


                    char tmp;
                    std::cout << "Wygenerowac losowo?[y/n]";
                    std::cin >> tmp;
                    bool generateRand = (tmp=='y')? 1: 0;
                    if(generateRand)
                    {
                        srand(time(NULL));
                        int randInt;
                        bool con;
                        for(int i = 0; i < N; i++)
                        {
                            con = 0;
                            for(int j = 0; j < l; j++)
                            {
                                randInt = rand() % 3 -1;           //zakres -1 do 1
                                implicentsVector[i][j] = randInt;
                            }
                            //sprawdz czy wylosowano implicent ktory ma nieistniec
                            for(int j = 0; j < l; j++)
                                if(implicentsVector[i][j] != -1)
                                {
                                    con = true;
                                    continue;
                                }
                            if(con == true)
                                continue;
                            //wylosuj jeszcze raz
                            i--;
                        }


                    }
                    else
                    {
                        //ustawienie wszystkich zmiennych na nieistniejace
                        for(int i = 0; i < N; i++)
                            for(int j = 0; j < l; j++)
                                implicentsVector[i][j] = -1;

                        std::cout << "Podaj implicenty w postaci np. x0-x1x2 \n";
                        //wczytanie ze strumienia wejsciowego w postaci x0x2-x3 ..
                        //zakodowanie postaci matematycznej na wektorowa
                        string line;
                        getline(std::cin, line);      //pusta linia po uzyciu std::cin >>
                        for(int i = 0; i < N; i++)      //po implicentach
                        {
                            std::cout << "Podaj implicent nr " << i << " :";
                            getline(std::cin, line);
                            bool isNeg = 0;
                            for(int j = 0; j < line.size(); j++)  //po literalach
                            {
                                switch(line[j])
                                {
                                case '-':
                                    {
                                        isNeg = 1;
                                    }
                                    break;
                                    case 'x':
                                    {
                                        //pobranie nastepnego znaku ktorym jest dolny indeks zmiennej
                                        j++;
                                        int id = line[j] - '0';   //zamiana char na int
                                        if(isNeg)
                                        {

                                            implicentsVector[i][id] = 0;       //umieszczenie w wektorze
                                            isNeg = 0;
                                        }
                                        else
                                            implicentsVector[i][id] = 1;

                                    }
                                    break;
                                    default:
                                        std::cerr << "ERROR SWITCH \n";
                                        exit(-1);
                                }
                            }
                        }

                        /*std::cout << "Podaj zapis implicentow w postaci wektorowej (np x1+x2+~x4 podaj w postaci: 11-10) \n";
                        for(int i = 0; i < N; i++)
                        {
                            std::cout << "Implicent " << i+1 << ": \n";
                            for(int j = 0; j < l; j++)
                            {
                                std::cin >> implicentsVector[i][j];
                                //std::cout << ~implicentsVector[i][j] + 2;   //negacja
                            }
                        }*/
                    }

                }

                //wypisanie implicentow
                for(int i = 0; i < N; i++)
                {
                    //std::cout << "Implicent nr " << i << ' ';
                    for(int j = 0; j < l; j++)
                    {
                        //std::cout << implicentsVector[i][j];  //postac wektorowa
                        if(implicentsVector[i][j] == 1)
                            printf("x%i", j);
                        else if(implicentsVector[i][j] == 0)
                            printf("-x%i", j);
                        else
                            continue;
                    }
                    std::cout << '\n';
                }

                #if ZESTAWIENIE == 1
                char s;
                std::cin >> s;
                #endif // ZESTAWIENIE
                auto start2 = high_resolution_clock::now();
                WalkSat(n, N, l, implicentsVector);
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(stop - start2);
                cout << "Czas wykonania: " << (duration.count() ) << '\n';    //wynik w ms
                cout << "\nClick anything to continue \n";
                getchar(); getchar();
                system("cls");
            }
            break;
        case 2:
            {
                std::cout << "Wczytac dane z pliku dane.txt?[y/n]: ";
                std::cin >> c;
                bool loadFromFile = (c == 'y')? 1: 0;
                if(loadFromFile)
                {
                    fstream file;
                    file.open("dane.txt");
                    if(file.fail())
                    {
                        std::cerr << "File does not exist \n";
                        exit(-1);
                    }
                    file >> n;
                    file >> N;
                    file >> t;
                    l = n;
                    implicentsVector = new int*[N];
                    for(int i = 0; i < N; i++)
                        implicentsVector[i] = new int[l];


                    //ustawienie wszystkich zmiennych na nieistniejace
                    for(int i = 0; i < N; i++)
                        for(int j = 0; j < l; j++)
                            implicentsVector[i][j] = -1;

                    //wczytanie z pliku w postaci x0x2-x3 ..
                    string line;
                    getline(file, line);      //pusta linia po uzyciu file >>
                    for(int i = 0; i < N; i++)      //po implicentach
                    {
                        getline(file, line);
                        bool isNeg = 0;
                        for(int j = 0; j < line.size(); j++)  //po literalach
                        {
                            switch(line[j])
                            {
                            case '-':
                                {
                                    isNeg = 1;
                                }
                                break;
                                case 'x':
                                {
                                    //pobranie nastepnego znaku ktorym jest dolny indeks zmiennej
                                    j++;
                                    int id = line[j] - '0';   //zamiana char na int
                                    if(isNeg)
                                    {

                                        implicentsVector[i][id] = 0;       //umieszczenie w wektorze
                                        isNeg = 0;
                                    }
                                    else
                                        implicentsVector[i][id] = 1;

                                }
                                break;
                                default:
                                    std::cerr << "ERROR SWITCH: Mozliwy zly zapis w pliku(lista tabu?)  \n";
                                    exit(-1);
                            }
                        }
                    }



                    //wczytanie postaci wektorowej
                    //for(int i = 0; i < N; i++)
                      //  for(int j = 0; j < l; j++)
                        //    file >> implicentsVector[i][j];

                    file.close();
                }
                else
                {
                    std::cout << "Podaj ilosc zmiennych: ";
                    std::cin >> n;
                    std::cout << "Podaj ilosc roznych implicentow: ";
                    std::cin >> N;
                    std::cout << "Podaj rozmiar tablicy tabu: ";
                    std::cin >> t;
                    l = n;
                    implicentsVector = new int*[N];
                    for(int i = 0; i < N; i++)
                        implicentsVector[i] = new int[l];

                    char tmp;
                    std::cout << "Wygenerowac losowo?[y/n]";
                    std::cin >> tmp;
                    bool generateRand = (tmp=='y')? 1: 0;
                    if(generateRand)
                    {
                        srand(time(NULL));
                        int randInt;
                        bool con = 0;
                        for(int i = 0; i < N; i++)
                        {
                            con = 0;
                            for(int j = 0; j < l; j++)
                            {
                                randInt = rand() % 3 -1;           //zakres -1 do 1
                                implicentsVector[i][j] = randInt;
                            }
                            //sprawdz czy wylosowano implicent ktory ma nieistniec
                            for(int j = 0; j < l; j++)
                                if(implicentsVector[i][j] != -1)
                                {
                                    con = true;
                                    continue;
                                }
                            if(con == true)
                                continue;
                            //wylosuj jeszcze raz
                            i--;
                        }

                    }
                    else
                    {
                            //ustawienie wszystkich zmiennych na nieistniejace
                        for(int i = 0; i < N; i++)
                            for(int j = 0; j < l; j++)
                                implicentsVector[i][j] = -1;

                        std::cout << "Podaj implicenty w postaci np. x0-x1x2 \n";
                        //wczytanie ze strumienia wejsciowego w postaci x0x2-x3 ..
                        string line;
                        getline(std::cin, line);      //pusta linia po uzyciu std::cin >>
                        for(int i = 0; i < N; i++)      //po implicentach
                        {
                            std::cout << "Podaj implicent nr " << i << " :";
                            getline(std::cin, line);
                            bool isNeg = 0;
                            for(int j = 0; j < line.size(); j++)  //po literalach
                            {
                                switch(line[j])
                                {
                                case '-':
                                    {
                                        isNeg = 1;
                                    }
                                    break;
                                    case 'x':
                                    {
                                        //pobranie nastepnego znaku ktorym jest dolny indeks zmiennej
                                        j++;
                                        int id = line[j] - '0';   //zamiana char na int
                                        if(isNeg)
                                        {

                                            implicentsVector[i][id] = 0;       //umieszczenie w wektorze
                                            isNeg = 0;
                                        }
                                        else
                                            implicentsVector[i][id] = 1;

                                    }
                                    break;
                                    default:
                                        std::cerr << "ERROR SWITCH \n";
                                        exit(-1);
                                }
                            }
                        }


                        /*std::cout << "Podaj zapis implicentow w postaci wektorowej (np x1+x2+~x4 podaj w postaci: 11-10) \n";
                        for(int i = 0; i < N; i++)
                        {
                            std::cout << "Implicent " << i+1 << ": \n";
                            for(int j = 0; j < l; j++)
                            {
                                std::cin >> implicentsVector[i][j];
                                //std::cout << ~implicentsVector[i][j] + 2;   //negacja
                            }
                        }*/
                    }



                }

                //wypisanie implicentow
                for(int i = 0; i < N; i++)
                {
                    //std::cout << "Implicent nr " << i << ' ';
                    for(int j = 0; j < l; j++)
                    {
                        //std::cout << implicentsVector[i][j];  //postac wektorowa
                        if(implicentsVector[i][j] == 1)
                            printf("x%i", j);
                        else if(implicentsVector[i][j] == 0)
                            printf("-x%i", j);
                        else
                            continue;
                    }
                    std::cout << '\n';
                }
                #if ZESTAWIENIE == 1
                char s;
                std::cin >> s;
                #endif // ZESTAWIENIE
                auto start2 = high_resolution_clock::now();
                GSATtabu(n, N, l, t, implicentsVector);
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(stop - start2);
                cout << "Czas wykonania: " << (duration.count() ) << '\n';    //wynik w ms
                cout << "\nClick anything to continue \n";
                getchar(); getchar();
                system("cls");
            }
            break;
        case 3:
            Exit();
            break;
        default:
            std::cerr << "ERROR \n";
            system("cls");
        }
    }
    #endif //DEBUG

    for(int i = 0; i < N; i++)
        delete implicentsVector[i];
    delete implicentsVector;

    return 0;
}
