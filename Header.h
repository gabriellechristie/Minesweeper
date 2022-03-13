#pragma once
#include <vector>
#include <random>
#include <SFML/Graphics.hpp>
using namespace std;

const static string HIDDEN_TILE_PATH = "./images/tile_hidden.png";
const static string REVEALED_TILE_PATH = "./images/tile_revealed.png";
const static string MINE_PATH = "./images/mine.png";
const static string ONE_PATH = "./images/number_1.png";
const static string TWO_PATH = "./images/number_2.png";
const static string THREE_PATH = "./images/number_3.png";
const static string FOUR_PATH = "./images/number_4.png";
const static string FIVE_PATH = "./images/number_5.png";
const static string SIX_PATH = "./images/number_6.png";
const static string SEVEN_PATH = "./images/number_7.png";
const static string EIGHT_PATH = "./images/number_8.png";
const static string FLAG_PATH = "./images/flag.png";
const static string TEST_1_PATH = "./images/test_1.png";
const static string TEST_2_PATH = "./images/test_2.png";
const static string TEST_3_PATH = "./images/test_3.png";
const static string DEBUG_PATH = "./images/debug.png";
const static string DIGITS_PATH = "./images/digits.png";
const static string HAPPY_FACE_PATH = "./images/face_happy.png";
const static string WIN_FACE_PATH = "./images/face_win.png";
const static string LOSE_FACE_PATH = "./images/face_lose.png";


static const enum Texture_Type {

	HIDDEN_TILE, REVEALED_TILE, MINE, ONE, TWO,
	THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, FLAG, TEST_1,
	TEST_2, TEST_3, DEBUG, DIGITS, HAPPY_FACE, WIN_FACE, LOSE_FACE, NONE 

};
static const char* Texture_Type_As_Str[] =
{ "HIDDEN_TILE", "REVEALED_TILE", "MINE", "ONE", "TWO", "THREE", "FOUR", 
"FIVE", "SIX", "SEVEN", "EIGHT", "FLAG","TEST_1","TEST_2", "TEST_3",
"DEBUG","DIGITS","HAPPY_FACE","WIN_FACE","LOSE_FACE","NONE"};


class RandomNumber {
	static mt19937 randomNumber;
public:
	static int Number(int min, int max);
};

class Tile {
public:
	Tile() {
		xPosition = 0;
		yPosition = 0;
		adjMineCount = 0;
		texture_type = HIDDEN_TILE;
		bomb_overlay_type = NONE;
		flag_overlay_type = NONE;
		number_overlay_type = NONE;
		overlayBombTexture = new sf::Texture;
		overlayFlagTexture = new sf::Texture;
		overlayNumberTexture = new sf::Texture;
		texture.loadFromFile(HIDDEN_TILE_PATH);
		sprite.setTexture(texture);
		isMenu = false;
		isMine = false;
		isVisible = true;
		isFlag = false;
	};
	~Tile() {
		delete overlayBombTexture;
		delete overlayFlagTexture;
		delete overlayNumberTexture;
	};
	
	void SetTexture();

	bool isMenu;
	bool isMine;
	bool isVisible;
	bool isFlag;

	//getters and setters for x and y

	Texture_Type texture_type;
	Texture_Type bomb_overlay_type;
	Texture_Type flag_overlay_type;
	Texture_Type number_overlay_type;
	Texture_Type prev_texture_type;

	int xBeginBounds;
	int yBeginBounds;
	int xEndBounds;
	int yEndBounds;

	int xPosition;
	int yPosition;

	int aboveAdj;
	int belowAdj;
	int rightAdjAngleTop;
	int rightAdjMid;
	int rightAdjAngleBot;
	int leftAdjAngleTop;
	int leftAdjMid;
	int leftAdjAngleBot;

	int tileNumber;
	int menuNumer;
	int adjMineCount;
	int column;
	int row;

	sf::Sprite sprite;
	sf::Sprite overlayBombSprite;
	sf::Sprite overlayFlagSprite;	
	sf::Sprite overlayNumberSprite;


	sf::Texture texture;
	sf::Texture* overlayBombTexture;
	sf::Texture* overlayFlagTexture;
	sf::Texture* overlayNumberTexture;

};

class Board {
public:
	Board() {};	
	void Add(Tile* tile);
	void InitBoard();
	void SetBoardValues(fstream& file, string& fileData);
	int GetWidth();
	int GetHeight();
	void BuildBoard();
	void BuildBoardForTests(Texture_Type texture_type);
	vector <Tile*> GetTileVector();
	void ProcessedTileLeft(int x, int y);
	void ProcessedTileRight(int x, int y);
	void ProcessedMenu(int x, int y);
	void ProcessTests(Texture_Type texture_type);
	Tile* FindTile(int x, int y);
	Tile* FindMenu(int x, int y);
	void SetAdjacentTiles(Tile* tile);
	void SetAdjTileAttributes(Tile* tile,int aboveAdj, int belowAdj,int rightAdjAngleTop,int rightAdjMid,int rightAdjAngleBot,	int leftAdjAngleTop,int leftAdjMid,	int leftAdjAngleBot);
	vector <Tile*> GetMenuVector();
	void AddMenu(Tile* tile);
	bool isMine(Tile* tile);
	int AdjMineCount(Tile* tile);
	int RandomX();
	int RandomY();
	bool isMineClicked(Tile* tile);
	bool isFlagClicked(Tile* tile);
	void NumberTiles(int value, Tile* tile);
	void ReverseMineDebug(Tile* tile);
	void ReverseFlag(Tile* tile);
	Tile* GetTile(int value);
	Tile* GetMenuTile(int value);
	void Digit(Tile* tile, int digit);
	void UpdateDigit(Tile* tile, Tile* tile2, Tile* tile3, Tile* tile4);
	void RevealAdjTiles(Tile* tile);
	void ResetBoard();
	bool isLose = false;
	bool isWin = false;
	bool isDebug = false;
	
private:
	vector <Tile*> tiles;
	vector <Tile*> menuTiles;
	vector <int> boardValues;

	int rowMax;
	int columnMax;
	int mineCount = 0;
	int tileCount = 0;
	int width = 0;
	int height = 0;
	int flagCounter = 0;
	int revealedTileCount = 0;
	int initRecursion = true;

	//bool isTest = false; 
	//int adjMineCount;
	

};

//helper function ideas:///////////////////////////////////////////////////
//Restarting the board
//Setting or clearing tiles of flags
//Setting or clearing mines(singly or in large quantities)
//Recalculating the number of adjacent mines Etc…
