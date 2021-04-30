#include <iostream>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <fstream>

using namespace std;

//sprawdza czy sekwencja juz byla
bool IsDuplicated(int * score, int ** allScores, int allScoresID, int regLen)
{
    int counter = 0;
    for(int i = 0; i <= allScoresID; i++)
    {
        counter = 0;
        for(int j = 0; j < regLen; j++)
            if(score[j] == allScores[i][j])
            {
                //printf("j: %i score: %i, allScores: %i\n", j, score[j], allScores[i][j]);
                counter++;
            }
        if(counter == regLen) //jezeli wszystkie cyfry sa takie same to jest to duplikat
            return true;

    }
    return false;
}


void LFSRGenerator(const int regLen, int * initValues, bool * feedbackConfig, const int feedbackType, int scoreLen)
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

    int maxSize = pow(2, regLen) - 1;
    int ** allScores = new int*[maxSize];
    for(int i = 0; i < maxSize; i++)
        allScores[i] = new int[regLen];

    int * score = new int[regLen];
    score = initValues;
    int allScoresID = 0;
    for(int i = 0; i < regLen; i++) //gleboka kopia
            allScores[allScoresID][i] = score[i];
    //poczatkowa wartosc
    /*for(int i = 0; i < regLen; i++)
         std::cout << score[i];
    std::cout << '\n';
    */

    while(1)
    {
        // realizacja odpowiedniego sprzezenia zwrotnego
        switch(feedbackType)
        {
        case 0:
            {
                int outputBit = score[regLen-1];
                if(outputBit == 0)
                {
                    for(int i = regLen-1; i > 0; i--)  //aktualizowanie wartosci przez kolejny cykl zegara
                        score[i] = score[i-1];
                    score[0] = 0;
                }
                else if(outputBit == 1)
                {
                    for(int i = regLen-1; i > 0; i--)   //od tylu
                    {
                        if(feedbackConfig[i-1] == 1)    //jak 1 to xoruj i przesun
                            score[i] = score[i-1] ^ outputBit;
                        else if(feedbackConfig[i-1] == 0)   //jak 0 to tylko przesun
                            score[i] = score[i-1];
                        else
                            std::cerr << "ERROR\n";
                    }
                    score[0] = 1;

                }
                else
                    std::cerr << "ERROR\n";

            }
            break;
        case 1:
            {
                int nextValue = 0;
                for(int i = 0; i < regLen; i++)
                    if(feedbackConfig[i] == 1)
                    {
                        //printf("score[%i]: %i \n", i, score[i]);
                        nextValue = nextValue ^ score[i];
                    }

                for(int i = regLen-1; i > 0; i--)  //aktualizowanie wartosci przez kolejny cykl zegara
                    score[i] = score[i-1];      //przesuniecie
                score[0] = nextValue;   // przypisanie wyniku sprzezenia zwrotnego do 0 elementu
            }
            break;
        default:
            std::cerr << "switch error \n";
            break;
        }





        //wypisanie zawartosci rejestru
        /*for(int i = 0; i < regLen; i++)
             std::cout << score[i];
        std::cout << '\n'; */
        if(IsDuplicated(score, allScores, allScoresID, regLen)) //jesli sekwencja sie powtorzy to zakoncz bo wartosci zaczna sie powtarzac
            {
                //printf("BREAK");
                break;
            }


        //zapisanie aktualnej zawartosci rejestru do tablicy 2d allScores
        allScoresID++;
        for(int i = 0; i < regLen; i++)
            allScores[allScoresID][i] = score[i];


    }


    for(int i = 0; i <= allScoresID; i++)
    {
        for(int j = 0; j < regLen; j++)
        {
            std::cout << allScores[i][j];
            file << allScores[i][j];
        }

        std::cout << '\n';
        file << '\n';
    }

    //DEBUG
    /*for(int i = 0; i <= allScoresID; i++)
    {
        for(int j = 0; j < regLen; j++)
            printf("i: %i, j: %i allScores: %i \n", i, j, allScores[i][j]);
        std::cout << '\n';
    }*/

    std::cout << "-------Wygenerowana sekwencja------ \n";
    file << "-------Wygenerowana sekwencja------ \n";
    //wybranie sekwencji wyjsciowej
    if(scoreLen > allScoresID)
    {
        std::cout << "scoreLen za dlugie \n";
        return;
    }
    int * finalScore = new int[scoreLen];
    for(int i = 0; i < scoreLen; i++)
    {
        //std::cout << allScores[i][regLen-1];
        finalScore[i] = allScores[i][regLen-1];
        std::cout << finalScore[i];
        file << finalScore[i];
    }

    std::cout << '\n';
    file << '\n';


    file.close();
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


    //debug
    /*int regLen = 4;
    int * initValues = new int[regLen]{1,0,1,1};
    std::vector<bool> feedbackConfig = {0,0,1,1};
    int feedbackType = 0;   // wewnetrzne = 0; zewnetrzne = 1,
    int scoreLen = 10;
    */
    int regLen;
    int * initValues;
    bool * feedbackConfig;
    int feedbackType;
    int scoreLen;

    printf("Program symuluje dzialanie generatora liczb losowych zbudowanego na rejestrze LFSR \n");
    printf("Podaj dlugosc rejestru: ");
    std::cin >> regLen;
    initValues = new int[regLen];
    feedbackConfig = new bool [regLen];
    printf("Podaj wartosci poczatkowe rejestru rejestru: ");
    for(int i = 0; i < regLen; i++)
        std::cin >> initValues[i];
    printf("Podaj wektor charakterystyczny opisujacy sprzezenie zwrotne: ");
    for(int i = 0; i < regLen; i++)
        std::cin >> feedbackConfig[i];

    printf("Podaj rodzaj sprzezenia zwrotnego: ");
    std::cin >> feedbackType;
    printf("Podaj dlugosc generowanej sekwencji losowej: ");
    std::cin >> scoreLen;

    LFSRGenerator(regLen, initValues, feedbackConfig, feedbackType, scoreLen);



    return 0;
}
