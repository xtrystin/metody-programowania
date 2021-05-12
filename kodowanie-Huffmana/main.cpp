#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <vector>
#include <functional>

using namespace std;


class Node
{
public:
    Node * right;
    Node * left;
    int value;          //ilosc wystapien litery
    char character;     //litera z alfabetu
    int HuffmannCode;   //0 lub 1


    //konstruktor
    Node(int v, Node*l = nullptr, Node*r = nullptr, char c = NULL)
        : value(v), left(l), right(r), character(c), HuffmannCode(-1) {}

    //funkcje skladowe
    bool isLeaf() const
    {
        if (this->left == nullptr && this->right == nullptr)
            return true;
        return false;
    }
    bool isRoot() const
    {
        if(HuffmannCode == -1)
            return true;
        return false;
    }

};

class CompareNodes  //komparator do kolejki piorytetowej
{
public:
    bool operator() (const Node* n1, const Node* n2)
    {
        if(n1->value > n2->value)   //najmniejsze na poczatku kolejki priorytetowej
            return true;
        else
            return false;
    }
};

//wyswietla drzewo huffmanna
void DisplayTree(const Node * node, int depth = 0)
{
    if(node == nullptr)
        return;
    if(node->isLeaf())
    {
        std::cout << node->character << " ";
        std::cout << node->value << " " << node->HuffmannCode << '\n';
        return;
    }
    else if(node->isRoot())
        std::cout << node->value << '\n';
    else
        std::cout << node->value << " " << node->HuffmannCode << '\n';

    for(int i = 0; i < depth; i++)
        std::cout << '\t';
    DisplayTree(node->left, depth+1);
    for(int i = 0; i < depth; i++)
        std::cout << '\t';
    DisplayTree(node->right, depth+1);
}

void Encode(const Node* node, std::map<char, string>& encodedChars, string code = "")
{
    if(node == nullptr)
        return;
    if(node->isLeaf())
    {
        encodedChars[node->character] = code;
        std::cout /*<< node->value << " " */<< node->character << ' ';
        std::cout << code << '\n';
        return;
    }


    Encode(node->left, encodedChars, code+'0');
    Encode(node->right, encodedChars, code+'1');
}



void Decode(string path, const Node * root)
{
    std::ifstream file;
    file.open(path);
    if(file.fail())
    {
        std::cerr << "FILE ERROR \n";
        exit(-1);
    }

    char tmp;
    char decodedChar;
    const Node * node = root;
    std::vector<char> decoded;
    while(file >> std::noskipws >> tmp)
    {
        cout << tmp;

        if(tmp == '0')
        {
            node = node->left;
        }
        else if(tmp == '1')
        {
            node = node->right;
        }
        else
            std::cerr << "eRROR: Decode \n";
        if(node->isLeaf())
        {
            cout << node->character << '\n';
            decoded.push_back(node->character);
            node = root;    //powroc do korzenia
            continue;
        }

    }
    std::cout << '\n';
    for (int i=0; i < decoded.size(); i++)
        std::cout << decoded[i];

    file.close();

    //zapisanie zdekowanego tekstu w pliku
    std::ofstream of;
    of.open("plik_zdekodowany.txt");
    if(of.fail())
    {
        std::cerr << "error \n";
        exit(-2);
    }
    for (int i=0; i < decoded.size(); i++)
        of << decoded[i];

    of.close();
}
void Huffmann(string path)
{
    //otworzenie pliku
    std::fstream file;
    file.open(path);
    if(file.fail())
    {
        std::cout << "file error \n";
        return;
    }

    //mapa przechowuje ilosc wystapien danego znaku
    std::map<char, int> counter;
    char c;
    while(file >> std::noskipws >> c)
    {
        if(counter.find(c) == counter.end())  //jesli nie jest jeszcze w mapie
            counter[c] = 1;
        else
             counter[c]++;           //jesli juz istnieje to zwieksz ilosc
    }
    file.close();


    //budowanie drzewa huffmanna

    //kolejka priorytetowa
    std::priority_queue<Node*, std::vector<Node*>, CompareNodes> Q;
    int n{0};   //ilosc wszystkich znakow
    for(auto entry : counter) //umieszczenie wszystkich elementow w kolejce jako osobne wezly
    {
        //std::cout << entry.first << ' ' << entry.second << '\n';
        n++;
        Node * z = new Node(entry.second, nullptr, nullptr, entry.first);
        std::cout << z->value << " " << z->character << '\n';
        Q.push(z);
    }
    //std::cout << Q.top()->value << '\n';
    //Q.pop();
    //budowanie drzewa Huffmana
    for(int i = 1; i <= n-1; i++)
    {
        Node * left = Q.top();
        int x = left->value;
        left->HuffmannCode = 0;     //dla lewych potomkow 0, dla prawych 1
        Q.pop();
        Node * right = Q.top();
        int y = right->value;
        right->HuffmannCode = 1;
        Q.pop();
        Node * z = new Node((x+y), left, right);
        Q.push(z);
        DisplayTree(z);
        std::cout << "\n\n\n";
    }
    Node * root = Q.top();  //zapisanie uzskanego drzewa Huffmana
    Q.pop();
    //std::cout << root->value;
    std::cout << "-----------------\n";
    DisplayTree(root);

    //zakodowanie znakow
    std::map<char, string> encodedChars;    //tablica ze znakami i ich kodami
    std::cout << "-----------------\n";
    Encode(root, encodedChars);

    //zamien znaki z ich utworzonymi kodami, zapisz w nowym pliku
    std::ofstream file2; //nowy plik
    file2.open("plik_zakodowany.txt");
    if(file2.fail())
    {
        std::cout << "file2 error: FILE2 NOT FOUND \n";
        return;
    }
    //plik kodowany
    file.open(path);
    if(file.fail())
    {
        std::cout << "file error: FILE NOT FOUND \n";
        return;
    }


    while(file>> std::noskipws  >> c)
    {
        //cout << c << " " << encodedChars[c] << '\n';
        file2 << encodedChars[c];
    }

    file.close();
    file2.close();

    //dekodowanie
    std::cout << "-----------------\n";
    Decode("plik_zakodowany.txt", root);








}



int main()
{
    std::cout << "Program koduje podany plik kodowaniem Huffmana, obslugiwane znaki: alfabet lacinski, znaki biale, znaki diakrytyczne, cyfry \n";
    std::cout << "Podaj nazwe (wraz z rozszerzeniem) pliku do zakodowania \n";
    std::string path = "plik_kodowany5.txt";    //wartosc domyslna
    std::cin >> path;
    Huffmann(path);


    return 0;
}
