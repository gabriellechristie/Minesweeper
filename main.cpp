#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <SFML/Graphics.hpp>
#include "Header.h"
using namespace std;


//char* loadBuffer(string filename);

//void GetBoardValues(Board& board, char* buffer);


int main()
{

    Board* board = new Board;
    board->InitBoard();
    board->BuildBoard();    
    

    sf::Texture revealedTile;
    sf::Sprite revealedSprite;
    if (!revealedTile.loadFromFile("./images/tile_revealed.png")) {
        cout << "Cannot load image";
    }
   // bool isFound = false;  
   

    sf::RenderWindow window(sf::VideoMode(board->GetWidth(), board->GetHeight()), "Minesweeper");
    
    /*int randomX = RandomNumber::Number(0,window.getSize().x);
    int randomY = RandomNumber::Number(0, window.getSize().y);*/

    bool isFound = false;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                // window closed
            case sf::Event::Closed: {
                window.close();
                break;
            }
                                  //left key pressed:
            case sf::Event::MouseButtonPressed: {

                if (event.type == sf::Event::MouseButtonPressed)
                {
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        int height = board->GetHeight() - 88;
                        if (event.mouseButton.y < height) {
                            board->ProcessedTileLeft(event.mouseButton.x, event.mouseButton.y);
                        }
                        else {
                            board->ProcessedMenu(event.mouseButton.x, event.mouseButton.y);
                        }

                    }
                    if (event.mouseButton.button == sf::Mouse::Right) {
                        board->ProcessedTileRight(event.mouseButton.x, event.mouseButton.y);
                    }
                    break;
                }
              }
            }
        }
    
                
        sf::RectangleShape rectangle;
        rectangle.setSize(sf::Vector2f(board->GetWidth(), board->GetHeight()));
        rectangle.setFillColor(sf::Color::White);
        window.draw(rectangle);

        vector <Tile*> tiles = board->GetTileVector();
        for (const auto tile : tiles) {
            window.draw(tile->sprite);            
            if (tile->isFlag && tile->isMine) {
                if (tile->bomb_overlay_type == HIDDEN_TILE) {
                    window.draw(tile->overlayBombSprite);
                    window.draw(tile->overlayFlagSprite);
                }
                else {
                    window.draw(tile->overlayFlagSprite);
                    window.draw(tile->overlayBombSprite);
                }
                
            }
            else if (tile->isMine) {
                window.draw(tile->overlayBombSprite);
            }
            else if (tile->isFlag) {
                window.draw(tile->overlayFlagSprite);
            }
            else if (tile->adjMineCount > 0 && (tile->texture_type == REVEALED_TILE)) {
                window.draw(tile->overlayNumberSprite);
            }           
           /* if (tile->isMine && tile->texture_type == HIDDEN_TILE) {
                board->isWin == true;
            }*/
            
        }        

        vector <Tile*> menuTiles = board->GetMenuVector();
        for (const auto menuTile : menuTiles) {
            if(menuTile->isVisible){
                window.draw(menuTile->sprite);
            }
            /*if (!board->isLose) {
                if (menuTile->texture_type == HAPPY_FACE) {
                    window.draw(menuTile->sprite);
                }
            }*/
            if (board->isLose) {
                if (menuTile->texture_type == LOSE_FACE) {
                    window.draw(menuTile->sprite);
                }
            }
        }      
        window.display();
    }
   

    return EXIT_SUCCESS;

}                  
    
   
    /*char* loadBuffer(string filename) {
        fstream file(filename, ios::in | ios::binary | ios::ate);
        long fileSize = file.tellg();
        file.seekg(0, ios::beg);
        char* buffer = new char[fileSize];
        file.read(buffer, fileSize);
        file.close();
        return buffer;
    }*/

    
        
    
    