#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <SFML/Graphics.hpp>
#include <random>
#include <ctime>
#include "Header.h"
using namespace std;

mt19937 RandomNumber::randomNumber(time(0));


void Board::Add(Tile* tile)
{
	tiles.push_back(tile);
}

void Board::SetBoardValues(fstream& file, string& fileData) {

    getline(file, fileData);
    int spacePos = 0;
    int prevCommaPos = 0;
    const char* spacePosPtr = fileData.c_str();
    string numberOfRows;
    string numberOfColums;
    string numberOfBombs;
    int counter = 1;
    int numberOfC = 0;
    int numberOfR = 0;
    int mines = 0;

    for (; spacePosPtr != nullptr; spacePos++, spacePosPtr++) {
        if (*spacePosPtr == '\0') {
            if (counter == 1) {
                numberOfColums = fileData.substr(prevCommaPos);
                numberOfC = stoi(numberOfColums);
                getline(file, fileData);
                counter++;
                spacePos = 0;
            }
            else if (counter == 2) {
                numberOfRows = fileData.substr(prevCommaPos);
                numberOfR = stoi(numberOfRows);
                counter++;
                spacePos = 0;
                getline(file, fileData);


            }
            else if (counter == 3) {
                numberOfBombs = fileData.substr(prevCommaPos);
                mines = stoi(numberOfBombs);
                break;
            }
        }
        else {
            continue;
        }
    }
    boardValues.push_back(numberOfC);
    boardValues.push_back(numberOfR);
    boardValues.push_back(mines);    
}

int Board::GetWidth()
{
    return width;
}

int Board::GetHeight()
{
    return height;
}

void Board::InitBoard()
{
    fstream configFile;
    configFile.open("./boards/config.cfg", ios::in);
    string configData;
    SetBoardValues(configFile, configData);
    configFile.close();
    columnMax = boardValues[0];
    rowMax = boardValues[1];
    width = boardValues[0] * 32;
    height = (boardValues[1] * 32) + 88;
    mineCount = boardValues[2];
    flagCounter = mineCount;
    tileCount = boardValues[0] * boardValues[1];   
}

void Board::BuildBoard()
{
    sf::RectangleShape rectangle(sf::Vector2f(width, height));
    rectangle.setFillColor(sf::Color::White);
    float xPosition = 0;
    float yPosition = 0;
    int totalTileHeight = 600 - 88;
    int xBegBounds = 0;
    int yBegBounds = 0;
    int xEndBounds = 32;
    int yEndBounds = 32;
    int column = 1;
    int row = 1;
    int tileCounter = 1;


    for (int count = 1; count <= tileCount; count++) {
        Tile* tile = new Tile();
        tile->sprite.setPosition(xPosition, yPosition);
        tile->xPosition = xPosition;
        tile->yPosition = yPosition;
        tile->xBeginBounds = xBegBounds;
        tile->yBeginBounds = yBegBounds;
        tile->xEndBounds = xEndBounds;
        tile->yEndBounds = yEndBounds;
        tile->column = column;
        tile->row = row;      
        tile->tileNumber = tileCounter;

        SetAdjacentTiles(tile);
        Add(tile);

         if (xEndBounds < width) {
            column++;
            xPosition += 32.f;
            xBegBounds += 32;
            xEndBounds += 32;
            tileCounter++;

        }
        else if (xEndBounds >= width) {
            yPosition += 32.f;
            yBegBounds += 32;
            yEndBounds += 32;
            xPosition = 0;
            xBegBounds = 0;
            xEndBounds = 32;
            tileCounter++;
            row++;
            column = 1;
        }

    }

    int counts = 0;
    while (counts < mineCount) {
        for (int count = 0; count < tiles.size(); count++) {
            Tile* tileMine = tiles[count];
            if (tileMine->isMine) {
                continue;
            }
            bool isFound = isMine(tileMine);
            if (isFound) {
                counts++;
                tileMine->isMine = true;
            }
            if (counts == mineCount) {
                break;
            }
        }
        int gg = 0;
    }
    for (int count = 0; count < tiles.size(); count++) {
        Tile* tile = tiles[count];
        int adjMineCount = AdjMineCount(tile);
        tile->adjMineCount = adjMineCount;
    }

    string mineCountStr = to_string(mineCount);
    if (mineCountStr.length() == 1) {
        mineCountStr = "00" + mineCountStr;
    }
    if (mineCountStr.length() == 2) {
        mineCountStr = "0" + mineCountStr;

    }
    char firstDigit = mineCountStr[0];
    char secDigit = mineCountStr[1];
    char thirdDigit = mineCountStr[2];

    string firstDigitStr;
    firstDigitStr += firstDigit;
    int firstDigitInt = stoi(firstDigitStr);

    string secDigitStr;
    secDigitStr += secDigit;
    int secDigitInt = stoi(secDigitStr);

    string thirdDigitStr;
    thirdDigitStr += thirdDigit;
    int thirdDigitInt = stoi(thirdDigitStr);

    int menuNumberCount = 0;
    Tile* tile = new Tile();
    tile->xBeginBounds = 21;
    tile->xEndBounds = 42;
    tile->yBeginBounds = height - 88;
    tile->yEndBounds = (tile->yBeginBounds + 32);
    tile->texture_type = DIGITS;
    tile->texture.loadFromFile(DIGITS_PATH);
    tile->sprite.setTexture(tile->texture);
    tile->isMenu = true;
    menuNumberCount++;
    tile->menuNumer = menuNumberCount;
    Digit(tile, firstDigitInt);
    tile->sprite.setPosition(21, height - 88);
    AddMenu(tile);

    int tile2BeginBound = width / 2;
    Tile* tile2 = new Tile();
    tile2->xBeginBounds = 42;
    tile2->xEndBounds = 63;
    tile2->yBeginBounds = height - 88;
    tile2->yEndBounds = (tile->yBeginBounds + 32);
    tile2->texture_type = DIGITS;
    tile2->texture.loadFromFile(DIGITS_PATH);
    tile2->sprite.setTexture(tile2->texture);
    tile2->sprite.setTextureRect(sf::IntRect(105, 0, 21, 32));
    tile2->sprite.setPosition(42, height - 88);
    tile2->isMenu = true;
    menuNumberCount++; 
    tile2->menuNumer = menuNumberCount;
    Digit(tile2, secDigitInt);
    AddMenu(tile2);

    Tile* tile3 = new Tile();
    tile3->xBeginBounds = 63;
    tile3->xEndBounds = 84;
    tile3->yBeginBounds = height - 88;
    tile3->yEndBounds = (tile->yBeginBounds + 32);
    tile3->texture_type = DIGITS;
    tile3->texture.loadFromFile(DIGITS_PATH);
    tile3->sprite.setTexture(tile3->texture);
    tile3->sprite.setTextureRect(sf::IntRect(1, 0, 21, 32));
    tile3->sprite.setPosition(63, height - 88);
    tile3->isMenu = true;
    menuNumberCount++;
    tile3->menuNumer = menuNumberCount;
    Digit(tile3, thirdDigitInt);
    AddMenu(tile3);

    Tile* tile4 = new Tile();
    tile4->xBeginBounds = 84;
    tile4->xEndBounds = 105;
    tile4->yBeginBounds = height - 88;
    tile4->yEndBounds = (tile->yBeginBounds + 32);
    tile4->texture_type = DIGITS;
    tile4->texture.loadFromFile(DIGITS_PATH);
    tile4->sprite.setTexture(tile4->texture);
    tile4->sprite.setTextureRect(sf::IntRect(210, 0, 21, 32));
    tile4->sprite.setPosition(0, height - 88);
    tile4->isMenu = true;
    tile4->isVisible = false;
    menuNumberCount++;
    tile4->menuNumer = menuNumberCount;
    AddMenu(tile4);

    int position = width / 2;
    Tile* tile5 = new Tile();
    tile5->xBeginBounds = tile2BeginBound;
    tile5->xEndBounds = tile2BeginBound + 64;
    tile5->yBeginBounds = height - 88;
    tile5->yEndBounds = (tile->yBeginBounds + 64);
    tile5->texture_type = HAPPY_FACE;
    tile5->texture.loadFromFile(HAPPY_FACE_PATH);
    tile5->sprite.setTexture(tile5->texture);
    tile5->sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
    tile5->sprite.setPosition(position, height - 88);
    tile5->isMenu = true;
    menuNumberCount++;
    tile5->menuNumer = menuNumberCount;
    AddMenu(tile5);

    Tile* tile6 = new Tile();
    tile6->xBeginBounds = tile2BeginBound;
    tile6->xEndBounds = tile2BeginBound;
    tile6->yBeginBounds = height - 88;
    tile6->yEndBounds = (tile->yBeginBounds + 64);
    tile6->texture_type = WIN_FACE;
    tile6->texture.loadFromFile(WIN_FACE_PATH);
    tile6->sprite.setTexture(tile6->texture);
    tile6->sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
    tile6->sprite.setPosition(position, height - 88);
    tile6->isMenu = true;
    tile6->isVisible = false;
    menuNumberCount++;
    tile6->menuNumer = menuNumberCount;
    AddMenu(tile6);

    Tile* tile7 = new Tile();
    tile7->xBeginBounds = tile2BeginBound;
    tile7->xEndBounds = tile2BeginBound;
    tile7->yBeginBounds = height - 88;
    tile7->yEndBounds = (tile->yBeginBounds + 64);
    tile7->texture_type = LOSE_FACE;
    tile7->texture.loadFromFile(LOSE_FACE_PATH);
    tile7->sprite.setTexture(tile7->texture);
    tile7->sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
    tile7->sprite.setPosition(position, height - 88);
    tile7->isMenu = true;
    tile7->isVisible = false;
    menuNumberCount++;
    tile7->menuNumer = menuNumberCount;
    AddMenu(tile7);

    Tile* tile8 = new Tile();
    tile->xBeginBounds = tile2BeginBound + 128;
    tile8->xEndBounds = tile2BeginBound + 192;
    tile8->yBeginBounds = height - 88;        
    tile8->yEndBounds = (tile->yBeginBounds + 64);;
    tile8->texture_type = DEBUG;
    tile8->texture.loadFromFile(DEBUG_PATH);
    tile8->sprite.setTexture(tile8->texture);
    tile8->sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
    tile8->sprite.setPosition(position += 128, height - 88);
    tile8->isMenu = true;
    menuNumberCount++;
    tile8->menuNumer = menuNumberCount;
    AddMenu(tile8);

    Tile* tile9 = new Tile();
    tile9->xBeginBounds = tile2BeginBound + 192;
    tile9->xEndBounds = tile2BeginBound + 256;
    tile9->yBeginBounds = height - 88;
    tile9->yEndBounds = (tile->yBeginBounds + 64);
    tile9->texture_type = TEST_1;
    tile9->texture.loadFromFile(TEST_1_PATH);
    tile9->sprite.setTexture(tile9->texture);
    tile9->sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
    tile9->sprite.setPosition(position += 64, height - 88);
    tile9->isMenu = true;
    menuNumberCount++;
    tile9->menuNumer = menuNumberCount;
    AddMenu(tile9);


    Tile* tile10 = new Tile();
    tile10->xBeginBounds = tile2BeginBound + 256;
    tile10->xEndBounds = tile2BeginBound + 320;
    tile10->yBeginBounds = height - 88;
    tile10->yEndBounds = (tile->yBeginBounds + 64);
    tile10->texture_type = TEST_2;
    tile10->texture.loadFromFile(TEST_2_PATH);
    tile10->sprite.setTexture(tile10->texture);
    tile10->sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
    tile10->sprite.setPosition(position += 64, height - 88);
    tile10->isMenu = true;
    menuNumberCount++;
    tile10->menuNumer = menuNumberCount;
    AddMenu(tile10);

    Tile* tile11 = new Tile();
    tile11->xBeginBounds = tile2BeginBound + 320;
    tile11->xEndBounds = tile2BeginBound + 384;
    tile11->yBeginBounds = height - 88;
    tile11->yEndBounds = (tile->yBeginBounds + 64);
    tile11->texture_type = TEST_3;
    tile11->texture.loadFromFile(TEST_3_PATH);
    tile11->sprite.setTexture(tile11->texture);
    tile11->sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
    tile11->sprite.setPosition(position += 64, height - 88);
    tile11->isMenu = true;
    menuNumberCount++;
    tile11->menuNumer = menuNumberCount;
    AddMenu(tile11);       
}

void Board::BuildBoardForTests(Texture_Type texture_type)
{
    string testFileStr;
    int tileCounter = 1;
    int mineCounter = 0;
    revealedTileCount = 0; // reset global revealed tile counter
    if (texture_type == TEST_1) {
        testFileStr = "./boards/testboard1.brd";
    }
    if (texture_type == TEST_2) {
        testFileStr = "./boards/testboard2.brd";
    }
    if (texture_type == TEST_3) {
        testFileStr = "./boards/testboard3.brd";
    }
    fstream testFile;
    testFile.open(testFileStr, ios::in);
    string testFileData;
    
    columnMax = 25;
    width = columnMax*32;

    rowMax = 16;
    height = (rowMax*32)+88;

    sf::RectangleShape rectangle(sf::Vector2f(width, height));
    rectangle.setFillColor(sf::Color::White);

    float xPosition = 0;
    float yPosition = 0;
    int xBegBounds = 0;
    int yBegBounds = 0;
    int xEndBounds = 32;
    int yEndBounds = 32;
    int column = 1;
    int row = 1;

    while (getline(testFile, testFileData)) {
        int spacePos = 0;
        int prevCommaPos = 0;
        const char* spacePosPtr = testFileData.c_str();
        string numberOfRows;
        string numberOfColums;
        string numberOfBombs;
      
        for (; spacePosPtr != nullptr; spacePos++, spacePosPtr++) {
            if (*spacePosPtr == '\0') {
                numberOfColums = testFileData.substr(prevCommaPos);
                spacePos = 0;
                const char* cptr = numberOfColums.c_str();  

                for (int count = 0; count < columnMax; count++, cptr++) {                    
                    Tile* tile = new Tile();                     
                    char numberOfColumsChr = *cptr;
                    if (numberOfColumsChr == '0') {
                        tile->isMine = false;
                    }
                    else if (numberOfColumsChr == '1') {
                        mineCounter++;
                        tile->isMine = true;
                    }
                   
                    tile->sprite.setPosition(xPosition, yPosition);
                    tile->xPosition = xPosition;
                    tile->yPosition = yPosition;
                    tile->xBeginBounds = xBegBounds;
                    tile->yBeginBounds = yBegBounds;
                    tile->xEndBounds = xEndBounds;
                    tile->yEndBounds = yEndBounds;
                    tile->column = column;
                    tile->row = row;
                    tile->tileNumber = tileCounter;             

                    if (tileCounter == 400) {
                        int skippy = 1;
                    }
                    tile->adjMineCount = 0;
                    SetAdjacentTiles(tile);             
                    Add(tile);                    
                    
                    if (xEndBounds < width) {
                        column++;
                        xPosition += 32.f;
                        xBegBounds += 32;
                        xEndBounds += 32;
                        tileCounter++;
                    }
                    else if (xEndBounds >= width) {
                        yPosition += 32.f;
                        yBegBounds += 32;
                        yEndBounds += 32;
                        xPosition = 0;
                        xBegBounds = 0;
                        xEndBounds = 32;
                        row++;
                        tileCounter++;
                        column = 1;
                    }                    
                }                
            }            
            else {
                continue;
            }
            break;
        }
    }
    tileCount = tiles.size();
    mineCount = mineCounter;
    flagCounter = mineCount;
    Tile* menuTileDigit1 = GetMenuTile(1);
    Tile* menuTileDigit2 = GetMenuTile(2);
    Tile* menuTileDigit3 = GetMenuTile(3);
    Tile* menuTileDigit4 = GetMenuTile(4);
    UpdateDigit(menuTileDigit1, menuTileDigit2, menuTileDigit3, menuTileDigit4);

    int adjMineCount = 0;
    for (int count = 0; count < tiles.size(); count++) {
        Tile* tile = tiles[count];
        adjMineCount = AdjMineCount(tile);

        tile->adjMineCount = adjMineCount;
    }

    testFile.close();
}

vector <Tile*> Board::GetTileVector()
{
    return tiles;
}

void Board::ProcessedTileLeft(int x, int y)
{    
    if (!isLose) {
        Tile* tile = FindTile(x, y);
        if (!(tile->isFlag)) {
            if (tile->texture_type != REVEALED_TILE) {
                revealedTileCount++;
            }
            tile->texture_type = REVEALED_TILE;
            tile->texture.loadFromFile(REVEALED_TILE_PATH);
            tile->sprite.setTexture(tile->texture);
            isMineClicked(tile);
            if (!tile->isMine) {
                if (tile->texture_type == REVEALED_TILE) {
                    NumberTiles(tile->adjMineCount, tile);
                }

            }
            if (tile->isMine) {
                for (const auto aTile : tiles) {
                    if (aTile->isMine) {
                        if (tile->texture_type != REVEALED_TILE) {
                            revealedTileCount++;
                        }
                        aTile->texture_type = REVEALED_TILE;
                        aTile->texture.loadFromFile(REVEALED_TILE_PATH);
                        aTile->sprite.setTexture(aTile->texture);
                        isMineClicked(aTile);
                        isLose = true;
                    }
                }
            }
        }

        if ((!tile->isFlag) && (!tile->isMine) && (tile->texture_type == REVEALED_TILE)) {
            initRecursion = true;
            RevealAdjTiles(tile);
            int tilesLeft = tileCount - mineCount;
            if (tilesLeft == revealedTileCount) {
                Tile* menuTile = GetMenuTile(6);
                isWin = true;
                for (const auto tile : tiles) {
                    if (tile->isMine) {
                        isFlagClicked(tile);
                    }
                }
                menuTile->isVisible = true;
                menuTile->texture_type = WIN_FACE;
                menuTile->texture.loadFromFile(WIN_FACE_PATH);
                menuTile->sprite.setTexture(menuTile->texture);
            }
        }
    }
}

void Board::ProcessedTileRight(int x, int y)
{
    if (!isLose) {
        Tile* tile = FindTile(x, y);
        tile->texture_type = HIDDEN_TILE;
        tile->texture.loadFromFile(HIDDEN_TILE_PATH);
        tile->sprite.setTexture(tile->texture);
        isFlagClicked(tile);
    }
    
}

void Board::ProcessedMenu(int x, int y)
{
    Tile* menuTile = FindMenu(x, y);
    if (isLose) {
        if (menuTile->texture_type != DEBUG) {
            if ((menuTile->texture_type == TEST_1) ||(menuTile->texture_type == TEST_2)||(menuTile->texture_type == TEST_3) ) {
                if (menuTile->texture_type == TEST_1) {
                    ProcessTests(TEST_1);
                }
                else if (menuTile->texture_type == TEST_2) {
                    ProcessTests(TEST_2);
                }
                else if (menuTile->texture_type == TEST_3) {
                    ProcessTests(TEST_3);
                }
            }
            else {
                menuTile->texture_type = HAPPY_FACE;
                menuTile->texture.loadFromFile(HAPPY_FACE_PATH);
                menuTile->sprite.setTexture(menuTile->texture);
                ResetBoard();
                menuTile->isVisible = false;
            }  
        }

    }
    if (!isLose  /*&& !isWin*/) {
        if (menuTile->texture_type == DEBUG) {
            if (isDebug) {
                isDebug = false;
                for (const auto tile : tiles) {
                    ReverseMineDebug(tile);
                }
            }
            else {
                isDebug = true;
                for (const auto tile : tiles) {
                    isMineClicked(tile);
                }

            }
        }
        else if (menuTile->texture_type == HAPPY_FACE) {
            ResetBoard();
        }
        else if (menuTile->texture_type == WIN_FACE) {
            ResetBoard();
        }
        else if (menuTile->texture_type == LOSE_FACE) {
            ResetBoard();
        }
        else if (menuTile->texture_type == TEST_1) {
            ProcessTests(TEST_1);
        }
        else if (menuTile->texture_type == TEST_2) {
            ProcessTests(TEST_2);
        }
        else if (menuTile->texture_type == TEST_3) {
            ProcessTests(TEST_3);
        }
    }
}



    void Board::ProcessTests(Texture_Type texture_type)
    {
        tiles.clear();
        BuildBoardForTests(texture_type);
        isLose = false;
        isDebug = true;
        if (isDebug) {
            isDebug = false;
            for (const auto tile : tiles) {
                isMineClicked(tile);
            }
        }
    }

    Tile* Board::FindTile(int x, int y)
{
    for (int count = 0; count < tiles.size(); count++) {
        Tile* tile = tiles[count];
        if ((tile->xBeginBounds <= x && x <= tile->xEndBounds) && (tile->yBeginBounds <= y && y <= tile->yEndBounds)){
            return tile;
        }
    }
   
    return nullptr;
}

Tile* Board::FindMenu(int x, int y)
{
    for (int counter = 0; counter < menuTiles.size(); counter++) {
        Tile* menuTile = menuTiles[counter];
        if ((menuTile->xBeginBounds <= x && x <= menuTile->xEndBounds) && (menuTile->yBeginBounds <= y && y <= menuTile->yEndBounds)) {
            
            return menuTile;
        }
    }
    return nullptr;
}

void Board::SetAdjacentTiles(Tile* tile)
{
    int row = tile->row;
    int tileNumber = tile->tileNumber;
    int column = tile->column;
    int columnMax = this->columnMax;
    int rowMax = this->rowMax;
    
    if (row == 1) {
        if (column == 1) {
            SetAdjTileAttributes(tile, 0, tileNumber + columnMax, 0,
                tileNumber + 1, tileNumber + (columnMax + 1), 0, 0, 0);
        }
        else if (column > 1 && column < columnMax) {
            SetAdjTileAttributes(tile, 0, tileNumber + columnMax, 0,
                tileNumber + 1, tileNumber + (columnMax + 1), 0, tileNumber - 1, tileNumber + (columnMax - 1));
        }
        else if (column == columnMax) {
            SetAdjTileAttributes(tile, 0, tileNumber + columnMax, 0,
                0, 0, 0, tileNumber - 1, tileNumber + (columnMax - 1));
        }
    }
    else if (row > 1 && row < rowMax) {
        if (column == 1) {
            SetAdjTileAttributes(tile, tileNumber - columnMax, tileNumber + columnMax, tileNumber - (columnMax - 1),
                tileNumber + 1, tileNumber + (columnMax + 1), 0, 0, 0);
        }
        else if (column > 1 && column < columnMax) {
            SetAdjTileAttributes(tile, tileNumber - columnMax, tileNumber + columnMax, tileNumber - (columnMax - 1),
                tileNumber + 1, tileNumber + (columnMax + 1), tileNumber - (columnMax + 1), tileNumber - 1, tileNumber + (columnMax - 1));
        }
        else if (column == columnMax) {
            SetAdjTileAttributes(tile, tileNumber - columnMax, tileNumber + columnMax, 0,
                0, 0, ((tileNumber - columnMax) - 1), tileNumber - 1, tileNumber + (columnMax - 1));
        }
    }
    else if (row == rowMax) {
        if (column == 1) {
            SetAdjTileAttributes(tile, tileNumber - columnMax, 0, tileNumber - (columnMax - 1),
                tileNumber + 1, 0, 0, 0, 0);
        }
        else if (column > 1 && column < columnMax) {
            SetAdjTileAttributes(tile, tileNumber - columnMax, tileNumber + columnMax, tileNumber - (columnMax - 1),
                tileNumber + 1, 0, tileNumber - (columnMax + 1), tileNumber - 1, 0);
        }
        else if (column == columnMax) {
            SetAdjTileAttributes(tile, tileNumber - columnMax, 0,
                0, 0, 0, ((tileNumber - columnMax) - 1), tileNumber - 1 , 0);
        }
    }
}



void Board::SetAdjTileAttributes(Tile* tile, int aboveAdj, int belowAdj, int rightAdjAngleTop, int rightAdjMid, int rightAdjAngleBot, int leftAdjAngleTop, int leftAdjMid, int leftAdjAngleBot)
{

    tile->aboveAdj = aboveAdj;
    tile->belowAdj = belowAdj;
    tile->rightAdjAngleTop = rightAdjAngleTop;
    tile->rightAdjMid = rightAdjMid;
    tile->rightAdjAngleBot = rightAdjAngleBot;
    tile->leftAdjAngleTop = leftAdjAngleTop;
    tile->leftAdjMid = leftAdjMid;
    tile->leftAdjAngleBot = leftAdjAngleBot;
}

vector<Tile*> Board::GetMenuVector()
{
    return menuTiles;
}

void Board::AddMenu(Tile* tile)
{
    menuTiles.push_back(tile);
}

bool Board::isMine(Tile* tile)
{
    int randomX = RandomX();
    int randomY = RandomY();
    bool isFoundx = false;
    bool isFoundy = false;

    if ((tile->xBeginBounds <= randomX) && (tile->xEndBounds >= randomX)) {
        isFoundx = true;
    }
    if ((tile->yBeginBounds <= randomY) && (tile->yEndBounds >= randomY)){
        isFoundy = true;
    }
   
    if(isFoundx == true && isFoundy == true){
            return true;
    }

    return false;
}

int Board::AdjMineCount(Tile* tile)
{
   
    int adjMineCount = 0;
    Tile* aboveAdjtile = GetTile(tile->aboveAdj);
    Tile* belowAdjtile = GetTile(tile->belowAdj);
    Tile* rightAdjAngleToptile = GetTile(tile->rightAdjAngleTop);
    Tile* rightAdjMidtile = GetTile(tile->rightAdjMid);
    Tile* rightAdjAngleBottile = GetTile(tile->rightAdjAngleBot);
    Tile* leftAdjAngleToptile = GetTile(tile->leftAdjAngleTop);
    Tile* leftAdjMidtile = GetTile(tile->leftAdjMid);
    Tile* leftAdjAngleBottile = GetTile(tile->leftAdjAngleBot);
    if (!(aboveAdjtile == nullptr)) {
        if (aboveAdjtile->isMine) {
            adjMineCount++;
        }       
    }
    if (!(belowAdjtile == nullptr)) {
        if (belowAdjtile->isMine) {
            adjMineCount++;
        }
    }
    if (!(rightAdjAngleToptile == nullptr)) {
        if (rightAdjAngleToptile->isMine) {
            adjMineCount++;
        }
    }
    if (!(rightAdjMidtile == nullptr)) {
        if (rightAdjMidtile->isMine) {
            adjMineCount++;
        }
    }
    if (!(rightAdjAngleBottile == nullptr)) {
        if (rightAdjAngleBottile->isMine) {
            adjMineCount++;
        }
    }
    if (!(leftAdjAngleToptile == nullptr)) {
        if (leftAdjAngleToptile->isMine) {
            adjMineCount++;
        }
    }
    if (!(leftAdjMidtile == nullptr)) {
        if (leftAdjMidtile->isMine) {
            adjMineCount++;
        }
    }
    if (!(leftAdjAngleBottile == nullptr)) {
        if (leftAdjAngleBottile->isMine) {
            adjMineCount++;
        }
    }
    tile->adjMineCount = adjMineCount;
    return adjMineCount;
}

int Board::RandomX()
{    
    int randomX = RandomNumber::Number(0, width);    
    return randomX;
}

int Board::RandomY()
{
    int height = this->height - 88;
    int randomY = RandomNumber::Number(0, height);
    return randomY;
}

bool Board::isMineClicked(Tile* tile)
{
    if (tile->isMine) {
       // if (tile->isFlag) {
            tile->overlayBombSprite.setPosition(tile->xPosition, tile->yPosition);
            tile->bomb_overlay_type = MINE;
            tile->overlayBombTexture->loadFromFile(MINE_PATH);
            tile->overlayBombSprite.setTexture(*tile->overlayBombTexture);
            return true;
        //}
    }
    return false;
}
bool Board::isFlagClicked(Tile* tile)
{
    Tile* menuTileDigit1 = GetMenuTile(1);
    Tile* menuTileDigit2 = GetMenuTile(2);
    Tile* menuTileDigit3 = GetMenuTile(3);
    Tile* menuTileDigit4 = GetMenuTile(4);
    if (isWin) {
        flagCounter = 0;
        if (tile->isMine) {
            tile->isFlag = true;
            tile->overlayBombSprite.setPosition(tile->xPosition, tile->yPosition);
            tile->bomb_overlay_type = HIDDEN_TILE;
            tile->overlayBombTexture->loadFromFile(HIDDEN_TILE_PATH);
            tile->overlayBombSprite.setTexture(*tile->overlayBombTexture);

            tile->overlayFlagSprite.setPosition(tile->xPosition, tile->yPosition);
            tile->flag_overlay_type = FLAG;
            tile->overlayFlagTexture->loadFromFile(FLAG_PATH);
            tile->overlayFlagSprite.setTexture(*tile->overlayFlagTexture);
        }
    }
    else if (!tile->isFlag) {
        tile->isFlag = true;
        tile->overlayFlagSprite.setPosition(tile->xPosition, tile->yPosition);
        tile->flag_overlay_type = FLAG;
        tile->overlayFlagTexture->loadFromFile(FLAG_PATH);
        tile->overlayFlagSprite.setTexture(*tile->overlayFlagTexture);
        flagCounter--;
    }
    else {
        tile->isFlag = false;
        ReverseFlag(tile);
        flagCounter++;

    }
    UpdateDigit(menuTileDigit1, menuTileDigit2, menuTileDigit3, menuTileDigit4);
    return true;
}
void Board::ReverseMineDebug(Tile* tile)
{
    if (tile->isMine) {
        tile->overlayBombSprite.setPosition(tile->xPosition, tile->yPosition);
        tile->bomb_overlay_type = HIDDEN_TILE;
        tile->overlayBombTexture->loadFromFile(HIDDEN_TILE_PATH);
        tile->overlayBombSprite.setTexture(*tile->overlayBombTexture);
    }
}

void Board::ReverseFlag(Tile* tile)
{
    tile->overlayFlagSprite.setPosition(tile->xPosition, tile->yPosition);
    tile->flag_overlay_type == HIDDEN_TILE;
    tile->overlayFlagTexture->loadFromFile(HIDDEN_TILE_PATH);
    tile->overlayFlagSprite.setTexture(*tile->overlayFlagTexture);

}

void Board::NumberTiles(int adjMineCount, Tile* tile)
{
    if (adjMineCount == 1) {
        tile->overlayNumberSprite.setPosition(tile->xPosition, tile->yPosition);
        tile->prev_texture_type = tile->number_overlay_type;
        tile->number_overlay_type = ONE;
        tile->overlayNumberTexture->loadFromFile(ONE_PATH);
        tile->overlayNumberSprite.setTexture(*tile->overlayNumberTexture);
    }
    if (adjMineCount == 2) {
        tile->overlayNumberSprite.setPosition(tile->xPosition, tile->yPosition);
        tile->prev_texture_type = tile->number_overlay_type;
        tile->number_overlay_type = TWO;
        tile->overlayNumberTexture->loadFromFile(TWO_PATH);
        tile->overlayNumberSprite.setTexture(*tile->overlayNumberTexture);

    }
    if (adjMineCount == 3) {
        tile->overlayNumberSprite.setPosition(tile->xPosition, tile->yPosition);
        tile->prev_texture_type = tile->number_overlay_type;
        tile->number_overlay_type = THREE;
        tile->overlayNumberTexture->loadFromFile(THREE_PATH);
        tile->overlayNumberSprite.setTexture(*tile->overlayNumberTexture);

    }
    if (adjMineCount == 4) {
        tile->overlayNumberSprite.setPosition(tile->xPosition, tile->yPosition);
        tile->prev_texture_type = tile->number_overlay_type;
        tile->number_overlay_type = FOUR;
        tile->overlayNumberTexture->loadFromFile(FOUR_PATH);
        tile->overlayNumberSprite.setTexture(*tile->overlayNumberTexture);
    }
    if (adjMineCount == 5) {
        tile->overlayNumberSprite.setPosition(tile->xPosition, tile->yPosition);
        tile->prev_texture_type = tile->number_overlay_type;
        tile->number_overlay_type = FIVE;
        tile->overlayNumberTexture->loadFromFile(FIVE_PATH);
        tile->overlayNumberSprite.setTexture(*tile->overlayNumberTexture);
    }
    if (adjMineCount == 6) {
        tile->overlayNumberSprite.setPosition(tile->xPosition, tile->yPosition);
        tile->prev_texture_type = tile->number_overlay_type;
        tile->number_overlay_type = SIX;
        tile->overlayNumberTexture->loadFromFile(SIX_PATH);
        tile->overlayNumberSprite.setTexture(*tile->overlayNumberTexture);
    }
    if (adjMineCount == 7) {
        tile->overlayNumberSprite.setPosition(tile->xPosition, tile->yPosition);
        tile->prev_texture_type = tile->number_overlay_type;
        tile->number_overlay_type = SEVEN;
        tile->overlayNumberTexture->loadFromFile(SEVEN_PATH);
        tile->overlayNumberSprite.setTexture(*tile->overlayNumberTexture);
    }
    if (adjMineCount == 8) {
        tile->overlayNumberSprite.setPosition(tile->xPosition, tile->yPosition);
        tile->prev_texture_type = tile->number_overlay_type;
        tile->number_overlay_type = EIGHT;
        tile->overlayNumberTexture->loadFromFile(EIGHT_PATH);
        tile->overlayNumberSprite.setTexture(*tile->overlayNumberTexture);
    }
}

Tile* Board::GetTile(int tileNumber)
{
    for (int count = 0; count < tiles.size();count++) {
        Tile* tile = tiles[count];
        if (tile->tileNumber == tileNumber) {
            return tile;
        }
    }
    return nullptr;
}

Tile* Board::GetMenuTile(int menuTileNumber)
{
    for (int count = 0; count < menuTiles.size(); count++) {
        Tile* menuTile = menuTiles[count];
        if (menuTile->menuNumer == menuTileNumber) {
            return menuTile;
        }
    }
    return nullptr;
}

void Board::Digit(Tile* tile, int digit)
{  
    if (digit == 0) {
        tile->sprite.setTextureRect(sf::IntRect(0, 0, 21, 32));
    }
    else if (digit == 1) {
        tile->sprite.setTextureRect(sf::IntRect(21, 0, 21, 32));
    }
    else if (digit == 2) {
        tile->sprite.setTextureRect(sf::IntRect(42, 0, 21, 32));
    }
    else if (digit == 3) {
        tile->sprite.setTextureRect(sf::IntRect(63, 0, 21, 32));
    }
    else if (digit == 4) {
        tile->sprite.setTextureRect(sf::IntRect(84, 0, 21, 32));
    }
    else if (digit == 5) {
        tile->sprite.setTextureRect(sf::IntRect(105, 0, 21, 32));
    }
    else if (digit == 6) {
        tile->sprite.setTextureRect(sf::IntRect(126, 0, 21, 32));
    }
    else if (digit == 7) {
        tile->sprite.setTextureRect(sf::IntRect(147, 0, 21, 32));
    }
    else if (digit == 8) {
        tile->sprite.setTextureRect(sf::IntRect(168, 0, 21, 32));
    }
    else if (digit == 9) {
        tile->sprite.setTextureRect(sf::IntRect(189, 0, 21, 32));
    }
}

void Board::UpdateDigit(Tile* tile, Tile* tile2, Tile* tile3, Tile* tile4)
{
    bool isNegative = false;
    if (flagCounter < 0) {
        isNegative = true;
    }
    if (flagCounter >= 0) {
        isNegative = false;
    }
    string flagCounterStr = to_string(abs(flagCounter));
    if (flagCounterStr.length() == 1) {
        flagCounterStr = "00" + flagCounterStr;
    }
    if (flagCounterStr.length() == 2) {
        flagCounterStr = "0" + flagCounterStr;

    }
    char firstDigit = flagCounterStr[0];
    char secDigit = flagCounterStr[1];
    char thirdDigit = flagCounterStr[2];

    string firstDigitStr;
    firstDigitStr += firstDigit;
    int firstDigitInt = stoi(firstDigitStr);

    string secDigitStr;
    secDigitStr += secDigit;
    int secDigitInt = stoi(secDigitStr);

    string thirdDigitStr;
    thirdDigitStr += thirdDigit;
    int thirdDigitInt = stoi(thirdDigitStr);

    Digit(tile, firstDigitInt);
    Digit(tile2, secDigitInt);
    Digit(tile3, thirdDigitInt);
    if (isNegative) {
        tile4->isVisible = true;
    }
}

void Board::RevealAdjTiles(Tile* tile)
{
    if (initRecursion) {
        initRecursion = false;
        if (tile->adjMineCount > 0) { 
            return; 
        }        
        RevealAdjTiles(GetTile(tile->aboveAdj));
        RevealAdjTiles(GetTile(tile->belowAdj));
        RevealAdjTiles(GetTile(tile->rightAdjAngleTop));
        RevealAdjTiles(GetTile(tile->rightAdjMid));
        RevealAdjTiles(GetTile(tile->rightAdjAngleBot));
        RevealAdjTiles(GetTile(tile->leftAdjAngleTop));
        RevealAdjTiles(GetTile(tile->leftAdjMid));
        RevealAdjTiles(GetTile(tile->leftAdjAngleBot));
        return;
    }
    if (tile == nullptr) {
        return;
    }

    if (tile->adjMineCount > 0) {
        if (tile->texture_type == REVEALED_TILE) {
            return;
        }
        revealedTileCount++;
        tile->texture_type = REVEALED_TILE;
        tile->texture.loadFromFile(REVEALED_TILE_PATH);
        tile->sprite.setTexture(tile->texture);
        NumberTiles(tile->adjMineCount, tile);
        return;
    }
    else {
        if (tile->texture_type == REVEALED_TILE) {
            return;
        }
        revealedTileCount++;
        tile->texture_type = REVEALED_TILE;
        tile->texture.loadFromFile(REVEALED_TILE_PATH);
        tile->sprite.setTexture(tile->texture);        
        RevealAdjTiles(GetTile(tile->aboveAdj));
        RevealAdjTiles(GetTile(tile->belowAdj));
        RevealAdjTiles(GetTile(tile->rightAdjAngleTop));
        RevealAdjTiles(GetTile(tile->rightAdjMid));
        RevealAdjTiles(GetTile(tile->rightAdjAngleBot));
        RevealAdjTiles(GetTile(tile->leftAdjAngleTop));
        RevealAdjTiles(GetTile(tile->leftAdjMid));
        RevealAdjTiles(GetTile(tile->leftAdjAngleBot));
    }
    
}

void Board::ResetBoard()
{
    tiles.clear();
    menuTiles.clear();
    InitBoard(); // reset to normal game mode not tests
    BuildBoard();
    isWin = false;
    isLose = false;
    isDebug = false;
    revealedTileCount = 0;
}


void Tile::SetTexture()
{

}

int RandomNumber::Number(int min, int max)
{
    uniform_int_distribution<int> dis(min, max);

    return dis(randomNumber);
}
