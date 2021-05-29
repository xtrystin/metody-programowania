#include <iostream>
#include <math.h>
#include <fstream>
#include <cmath>
#include <time.h>

#define DEBUG 0
#define PI 3.14159265

using namespace std;


class Point
{
public:
    double x;
    double y;

    Point(double x, double y) : x(x), y(y) {}
    Point() {}

    void Display()
    {
        printf("Punkt(%f, %f)\n", x, y);

    }

    /*bool isEqual(Point b)
    {
        if(this->x == b-> && this->y == b->y)
            return true;
        return false;
    }*/

    bool isInHull = false;
};


int Orientation(Point * a, Point * b, Point * c)
{
    //obliczanie iloczynu wektorowego
    //double vectorProduct = (b->y - a->y) * (c->x - b->x) - (b->x - a->x) * (c->y - b->y);
    double vectorProduct = (b->x - a->x) * (c->y - b->y) - (b->y - a->y) * (c->x - b->x);
    int epsilon = 0.001;   //blad
    if(abs(vectorProduct - 0.0) <= epsilon)
        return 0;           // wspolliniowe
    return (vectorProduct > 0) ? 1 : -1;      // 1 - orientacja zgodna z ruchem zegara   -1 - przeciwna
}

//oblicza kat miedzy wektorami w stopniach
double Angle(Point * a, Point * b, Point * c)
{
    double scalarProduct = (b->x - a->x) * (c->x - a->x) + (b->y - a->y) * (c->y - a->y);
    double w1_Lenght = sqrt( (b->x - a->x)*(b->x - a->x) + (b->y - a->y)*(b->y - a->y) );
    double w2_Lenght = sqrt( (c->x - a->x)*(c->x - a->x) + (c->y - a->y)*(c->y - a->y) );

    double cos_alfa = (double)scalarProduct / (double)( w1_Lenght * w2_Lenght );
    //std::cout << cos_alfa << '\n';
    double score = acos(cos_alfa) * 180.0 / PI;
    return score;

}


void Jarvis(Point ** points, int n)
{
    //czy zapisac do pliku
    ofstream file;
    char choice;
    std::cout << "Zapisac wynik do pliku wynik.txt?[y/n]: ";
    std::cin >> choice;
    if(choice == 'y')
    {
        file.open("wynik.txt");
        if(file.fail())
        {
            std::cerr << "Eile error \n";
            exit(-1);
        }
    }

    Point ** P = new Point*[n]; //tablica z punktami na prawej otoczce
    Point ** Q = new Point*[n]; //tablica z punktami na lewej otoczce
    //wyznacz punkt o najmniejszej wspolrzednej y oraz najwiekszej y
    P[0]= new Point();
    Q[0] = new Point();
    double minY = 100000.0;
    double maxY = -100000.0;
    for(int i = 0; i < n; i++)
    {
        if(points[i]->y <= minY)
        {
            //jesli rowne to wybierz punkt o mniejszym x
            if(points[i]->y == minY)
            {
                if(points[i]->x < P[0]->x)
                {
                    minY = points[i]->y;
                    P[0] = points[i];
                }
                else
                    continue;
            }
            else
            {
                minY = points[i]->y;
                P[0] = points[i];
            }

        }
        if(points[i]->y >= maxY)
        {
            //jesli rowne to wybierz punkt o wiekszym x
            if(points[i]->y == maxY)
            {
                if(points[i]->x > Q[0]->x)
                {
                    maxY = points[i]->y;
                    Q[0] = points[i];
                }
                else
                    continue;
            }
            else
            {
                maxY = points[i]->y;
                Q[0] = points[i];
            }
        }
    }
    printf("Najmniejszy punkt: (%f, %f) \n", P[0]->x, P[0]->y);
    printf("Najwiekszy punkt: (%f, %f) \n", Q[0]->x, Q[0]->y);



    //wyznaczenie prawego lancucha otocznki
    int i = 0;      //ilosc punktow na prawej otoczce, numeracja od 0
    Point * S = new Point();
    Point * N = new Point();   //sprawdzane po kolei punkty
    double minAngle;
    while(1)
    {
       S = P[i];
       S->isInHull = true;      // jest w otoczce

       //wybierz najmniejszy kat
       minAngle = 1000;
       for(int j = 0; j < n; j++)   //sprawdz wszystkie punkty nienalezace do otoczki
       {
           N = points[j];
           if(N->isInHull)
           {
               if(N == Q[0])
                    break;
                else
                    continue;
           }
           N->Display();
           int orientation = Orientation(S, N, new Point(S->x+1, S->y+0));  //ostatni wektor [1,0]
           if( orientation == -1 || orientation == 0)    //orientacja przeciwna do zegara lub wspolliniowa, S jest nowym punktem odniesienia
           {
               double angle = Angle(S, N, new Point(S->x+1, S->y+0));
               printf("Angle: %f, minAngle: %f\n",angle, minAngle );
               if(angle <= minAngle)
               {
                   minAngle = angle;
                   P[i+1] = N;
               }
           }

            //jesli ostatni faktycznie wpisany punkt to Q[0] to przerwij
           if(j == n-1)
           {
               if(Q[i+1] == Q[0])
                    break;
           }

       }
       N = P[i+1];  //wybrany do otoczki punkt
        //N->Display();

        if(N == Q[0])
            break;
        i++;

    }
    Point ** score = new Point*[n];
    int scoreID=0;
    std::cout << "\nPrawa otoczka: \n";
    for(int j=0; j <=i; j++)
    {
        P[j]->Display();
        score[scoreID] = P[j];
        scoreID++;
        if(choice == 'y')
            file << P[j]->x << ' ' << P[j]->y << '\n';
    }

   // P[i+1]->Display();  tu znajduje sie punkt Q[0]

    std::cout << "\n\n\n";
    //wyznaczenie lewego lancucha otocznki
    i = 0;
    while(1)
    {
       S = Q[i];
       S->isInHull = true;      // jest w otoczce

       //wybierz najmniejszy kat
       minAngle = 1000;
       for(int j = 0; j < n; j++)   //sprawdz wszystkie punkty nienalezace do otoczki
       {
           N = points[j];
           if(N->isInHull)
           {
               //printf("Continue: ");
               //N->Display();
               //continue;
           }

           //N->Display();
           int orientation = Orientation(S, N, new Point(S->x-1, S->y+0));  //ostatni wektor [-1,0] (odwrocenie)
           if( orientation == -1 || orientation == 0)    //orientacja przeciwna do zegara lub wspolliniowa, S jest nowym punktem odniesienia
           {
               double angle = Angle(S, N, new Point(S->x-1, S->y+0));
               N->Display(); printf("Angle: %f, minAngle: %f\n",angle, minAngle );
               if(angle <= minAngle)
               {
                   minAngle = angle;
                   Q[i+1] = N;
                   printf("j: %i ", j); N->Display();
               }
           }
            //jesli ostatni faktycznie wpisany punkt toP[0] to przerwij
           if(j == n-1)
           {
               if(Q[i+1] == P[0])
                    break;
           }

       }
       N = Q[i+1];  //wybrany do otoczki punkt
       //N->Display();


        //N->Display();
        //P[0]->Display();
        //std::cout << "----------------\n";
        if(N == P[0])
            break;
        i++;

    }

    std::cout << "\Lewa otoczka: \n";
    for(int j=0; j <=i; j++)
    {
        Q[j]->Display();
        score[scoreID] = Q[j];
        scoreID++;
        if(choice == 'y')
            file << Q[j]->x << ' ' << Q[j]->y << '\n';
    }

   // Q[i+1]->Display();  tu znajduje sie punkt P[0]

    //wypisanie
    std::cout << "\nPunkty na otoczce: \n";
    for(int j=0; j < scoreID; j++)
        score[j]->Display();



    delete score;
    file.close();
}

void PointsGenerator(Point ** points, int n)
{
    srand((time(NULL)));
    int randX, randY;

    for(int i=0; i<n; i++)
    {
        randX = rand() % 1000; //od 0 do 999
        randY = rand() % 1000; //od 0 do 999
        Point * randPoint = new Point(randX, randY);
        points[i] = randPoint;
    }
}

int main()
{
    #if DEBUG == 1
    Point * a = new Point(0,0);
    Point * b = new Point(-3,0);
    Point * c = new Point(1,0);

    std::cout << Angle(a, b ,c ) << '\n';
    std::cout << Orientation(a, b ,c );
    return 0;
    #endif // DEBUG

    std::cout << "Program realizuje algorytm Jarvisa \n";
    char choice;
    std::cout << "Wczytac dane z pliku dane.txt?[y/n]: ";
    std::cin >> choice;

    int n;     // ilosc punktow
    double tmpX;
    double tmpY;
    Point ** points = nullptr;  //tablica punktow
    if(choice == 'y')
    {
        fstream file("dane.txt");
        if(file.fail())
        {
            std::cerr << "File error \n";
            exit(-1);
        }
        file >> n;
        points = new Point*[n];
        for(int i=0; i < n; i++)
        {
            file >> tmpX;
            file >> tmpY;
            Point * tmp = new Point(tmpX, tmpY);
            points[i] = tmp;
        }
    }
    else
    {
        char choice2;
        std::cout << "Wygenerowac punkty losowo?[y/n]: ";
        std::cin >> choice2;
        if(choice2 == 'y')
        {
            std::cout << "Podaj ilosc punktow: ";
            std::cin >> n;
            points = new Point*[n];
            PointsGenerator(points, n);
        }
        else
        {
            std::cout << "Podaj ilosc punktow: ";
            std::cin >> n;
            points = new Point*[n];
            std::cout << "Podaj wspolrzedne punktow \n";
            for(int i=0; i < n; i++)
            {
                std::cout << "Punkt " << i << '\n';
                std::cin >> tmpX;
                std::cin >> tmpY;
                Point * tmp = new Point(tmpX, tmpY);
                points[i] = tmp;
            }
        }




    }
    if(points == nullptr)
    {
        std::cerr << "points ERROR\n";
        exit(-1);
    }

    for(int i=0; i < n; i++)
        std::cout << "Punkt " << i << ": "<< points[i]->x << " " << points[i]->y << '\n';


    Jarvis(points, n);



    delete points;
    return 0;
}
