/*
program generuje losowo labirynt o rozmiarze podanym przez uzytkownika a nastepnie szuka drogi z punktu a do b, rowniez podanych przez uzytkownika 
jakub gredzicki 01.05.2022 asd lab2
*/
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <queue>

const bool fieldTypeGenerated = 0; // typ pola generowany przez algorytm dfs, 0 to korytarz, 1 to sciana

struct graph
{
    unsigned int edgesCount, vortexCount;
    std::vector<std::vector<int>> lists;
};

struct pole
{
    int row;
    int col;
};

void generateLabirynth(graph &graph, int size)
{
    graph.vortexCount = size * size;
    graph.edgesCount = 2 * size * (size - 1);

    graph.lists.resize(graph.vortexCount);

    graph.lists[0].push_back(1);
    graph.lists[0].push_back(size);

    graph.lists[size - 1].push_back(size - 2);
    graph.lists[size - 1].push_back(2 * size - 1);

    graph.lists[(size - 1) * size].push_back((size - 1) * size + 1);
    graph.lists[(size - 1) * size].push_back((size - 1) * size);

    graph.lists[graph.vortexCount - 1].push_back(graph.vortexCount - 2);
    graph.lists[graph.vortexCount - 1].push_back(graph.vortexCount - 1 - size);

    for (int i = 1; i <= size - 2; ++i)
    {
        graph.lists[i].push_back(i - 1);
        graph.lists[i].push_back(i + 1);
        graph.lists[i].push_back(i + size);
    }

    for (int i = ((size - 1) * size + 1); i < graph.vortexCount - 2; ++i)
    {
        graph.lists[i].push_back(i - 1);
        graph.lists[i].push_back(i + 1);
        graph.lists[i].push_back(i - size);
    }

    for (int j = 1; j <= size - 2; ++j)
    {
        graph.lists[j * size].push_back(j * size - size);
        graph.lists[j * size].push_back(j * size + size);
        graph.lists[j * size].push_back(j * size + 1);

        graph.lists[j * size + size - 1].push_back(j * size - 1);
        graph.lists[j * size + size - 1].push_back(j * size + 2 * size - 1);
        graph.lists[j * size + size - 1].push_back(j * size + size - 2);
    }

    for (int i = 1; i <= size - 2; ++i)
    {
        for (int j = 1; j <= size - 2; ++j)
        {
            graph.lists[i * size + j].push_back(i * size + j - 1);
            graph.lists[i * size + j].push_back(i * size + j + 1);
            graph.lists[i * size + j].push_back(i * size + j - size);
            graph.lists[i * size + j].push_back(i * size + j + size);
        }
    }
}

void DFS(const graph &graph, int vortex, std::vector<bool> &been, std::vector<std::vector<int>>& labirynth, unsigned int size)
{
    been[vortex] = 1;
    int x = vortex % size, y = vortex / size;
    labirynth[2 * x][2 * y] = fieldTypeGenerated;  

    std::vector<int> random = graph.lists[vortex];
    while (random.size() > 0)
    {
        int i = rand() % random.size();
        int u = random[i];
        random.erase(random.begin() + i);

        if (been[u] == 0)    
        {
            int nx = u % size, ny = u / size;
            if (nx == x - 1)
                labirynth[2 * x - 1][2 * y] = fieldTypeGenerated;   
            else if (nx == x + 1)
                labirynth[2 * x + 1][2 * y] = fieldTypeGenerated;
            else if (ny == y - 1)
                labirynth[2 * x][2 * y - 1] = fieldTypeGenerated;
            else
                labirynth[2 * x][2 * y + 1] = fieldTypeGenerated;

            DFS(graph, u, been, labirynth, size);
        }
    }
}

void printLabirynth(std::vector<std::vector<int>>& labirynth)
{
    std::cout << "  ";
    for (int i = 0; i < labirynth.size(); i++)
        std::cout << std::setw(3) << i;
    std::cout << std::endl;

    for (int i = 0; i < labirynth.size(); ++i)
    {
        std::cout << std::setw(3) << i;
        for (int j = 0; j < labirynth.size(); ++j)
            if (labirynth[i][j] == 1)
                std::cout << (char)219 << (char)219 << (char)219;
            else if (labirynth[i][j] == -2)
                std::cout << " S ";
            else if (labirynth[i][j] == -3)
                std::cout << " W ";
            else if (labirynth[i][j] != -1)
                std::cout << "   ";
            else 
                std::cout << " + ";
        std::cout << std::endl;
    }
}

bool isPathToExit(std::vector<std::vector<int>>& labirynth, int rowStart, int colStart, int& rowExit, int& colExit)
{
    bool exitFound = false;
    std::queue<pole> listOfFields;

    pole start;
    start.row = rowStart;
    start.col = colStart;

    pole exit;          
    exit.row = rowExit; 
    exit.col = colExit; 

    listOfFields.push(start);
    labirynth[rowStart][colStart] = 2;

    while (!exitFound && !listOfFields.empty())
    {
        
        pole currentField = listOfFields.front();
        listOfFields.pop();
        int currentRow = currentField.row, currentCol = currentField.col;
        //std::cout << "col = " << currentCol << ", row = " << currentRow << ", pozostalo " << listOfFields.size() << " pol w kolejce" << std::endl;
        if (currentRow == exit.row && currentCol == exit.col)
        {
            exitFound = true;
        }

        if (!exitFound)
        {
            
            if (currentRow > 0 && labirynth[currentRow - 1][currentCol] == 0)
            {
                labirynth[currentRow - 1][currentCol] = labirynth[currentRow][currentCol] + 1;
                pole nextField;
                nextField.row = currentRow - 1;
                nextField.col = currentCol;
                listOfFields.push(nextField);
            }

            if (currentRow < labirynth.size() - 2 && labirynth[currentRow + 1][currentCol] == 0)
            {
                labirynth[currentRow + 1][currentCol] = labirynth[currentRow][currentCol] + 1;
                pole nextField;
                nextField.row = currentRow + 1;
                nextField.col = currentCol;
                listOfFields.push(nextField);
            }

            if (currentCol > 0 && labirynth[currentRow][currentCol - 1] == 0)
            {
                labirynth[currentRow][currentCol - 1] = labirynth[currentRow][currentCol] + 1;
                pole nextField;
                nextField.row = currentRow;
                nextField.col = currentCol - 1;
                listOfFields.push(nextField);
            }

            if (currentCol < labirynth.size() - 2 && labirynth[currentRow][currentCol + 1] == 0)
            {
                labirynth[currentRow][currentCol + 1] = labirynth[currentRow][currentCol] + 1;
                pole nextField;
                nextField.row = currentRow;
                nextField.col = currentCol + 1;
                listOfFields.push(nextField);
            }
        }
    }
    return exitFound;
}

void markPath(std::vector<std::vector<int>>& labirynth, int rowExit, int colExit)
{
    int currentFieldValue = labirynth[rowExit][colExit];
    int currentRow = rowExit;
    int currentCol = colExit;

    while(currentFieldValue > 2)
    {
        labirynth[currentRow][currentCol] = -1;

        if(currentRow > 0 && labirynth[currentRow-1][currentCol] == currentFieldValue-1)
            currentRow -= 1;
        else if(currentRow < labirynth.size() && labirynth[currentRow+1][currentCol] == currentFieldValue-1)
            currentRow += 1;
        else if(currentCol > 0 && labirynth[currentRow][currentCol-1] == currentFieldValue-1)
            currentCol -= 1;
        else
            currentCol += 1;

        currentFieldValue--;
    }

    labirynth[currentRow][currentCol] = -2;
}

void generateWalls(std::vector<std::vector<int> >& labirynth)
{
    for(int i = 0; i < labirynth.size(); ++i)
    {
        labirynth[0][i] = 1;
        labirynth[i][0] = 1;
        labirynth[labirynth.size() - 1][i] = 1;
        labirynth[i][labirynth.size() - 1] = 1;
    }
}

int main()
{
    srand ( time(NULL) );
    graph graph;
    unsigned int size;
    std::cout << "Podaj rozmiar labiryntu: ";
    std::cin >> size;

    int rowStart, colStart, rowExit, colExit;

    generateLabirynth(graph, size);

    std::vector<std::vector<int>> labirynth;
    labirynth.resize(2 * size - 1);
    for (int i = 0; i < 2 * size - 1; ++i)
       labirynth[i].resize(2 * size - 1);
    
    for (int i = 0; i < labirynth.size(); ++i)
        for (int j = 0; j < labirynth.size(); ++j)
            labirynth[i][j] = !fieldTypeGenerated;   // 1 to sciana, 0 to sciezka, na poczatku pliku do zmiany

    std::vector<bool> been;
    been.resize(graph.vortexCount); 

    DFS(graph, rand() % graph.vortexCount, been, labirynth, size);
    //generateWalls(labirynth);
    printLabirynth(labirynth);

     //tablica labirynth zawiera teraz 0 - korytarze i 1 - sciany

    std::cout << std::endl << "Podaj pole startowe(x y): ";
    std::cin >> colStart >> rowStart;

    while(labirynth[rowStart][colStart] == 1)
    {
        std::cout << "Nie mozna zaczac w scianie, podaj jeszcze raz: ";
        std::cin >> colStart >> rowStart;
    }

    std::cout << "Podaj pole wyjscia(x y): ";
    std::cin >> colExit >> rowExit;

    while(labirynth[rowExit][colExit] == 1)
    {
        std::cout << "Nie mozna wyjsc  w scianie, podaj jeszcze raz: ";
        std::cin >> colExit >> rowExit;
    }

    if (isPathToExit(labirynth, rowStart, colStart, rowExit, colExit))
    {
        std::cout << "Pomyslnie odnaleziono droge do wyjscia z pola x=" << colStart << ", y=" << rowStart << " do wyjscia na polu x=" << colExit << ", y=" << rowExit << std::endl << std::endl;
        markPath(labirynth, rowExit, colExit);
        labirynth[rowExit][colExit] = -3;
        printLabirynth(labirynth);
    }
    else 
        std::cout << "Nie udalo sie znalezc drogi do wyjscia." << std::endl; 
}