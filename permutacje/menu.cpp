#include <iostream>
#include "menu.hpp"
#include <windows.h>

using namespace std;

void menu()
{

    cout << "*************************************************" << endl;
    cout << "*                    MENU                       *" << endl;
    cout << "*                                               *" << endl;
    cout << "*                                               *" << endl;
    cout << "*        1. HeapAlgorithm                       *" << endl;
    cout << "*        2. Credits                             *" << endl;
    cout << "*        3. Exit                                *" << endl;
    cout << "*                                               *" << endl;
    cout << "*                                               *" << endl;
    cout << "*************************************************" << endl;
    //cout << endl;
    cout << "Select option: ";

}


void Credits()
{
    cout << "*************************************************" << endl;
    cout << "*               CREDITS                         *" << endl;
    cout << "*                                               *" << endl;
    cout << "*                                               *" << endl;
    cout << "*           PROGRAM MADE BY                     *" << endl;
    cout << "*             xtrystin                          *" << endl;
    cout << "*          All rights reserved                  *" << endl;
    cout << "*                2021                           *" << endl;
    cout << "*                                               *" << endl;
    cout << "*                                               *" << endl;
    cout << "*************************************************" << endl;
    cout << "Click anything to continue \n";
}

void info()
{
    cout << "Program sluzy do generowania permutacji wykorzustujac algorytm Heap'a ";
    cout << endl;
    cout << endl;
}

void Exit()
{
    std::cout << "Leaving..." << std::endl;
    Sleep(1000);
    exit(0);
}
