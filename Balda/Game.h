//поле для игры

#pragma once

#include <vector>
#include "Dictionary.h"
#include "resource.h"

class CCell {
	
public:
	CCell() {};
	CCell(int str, int row, char letter[2], COLORREF bkColor);
	~CCell() {};

	void Draw(HDC &hdc);
	void InitCoord(int xLeftOfTable, int yTopOfTable, int sizeOfGap, int sizeOfCell);
	void SetBkColorOfCell(COLORREF newBkColor);
	void SetLetter(char letter[2]);
	bool IsContained(int xPos, int yPos);
	void ChangeCell(HDC &hdc, COLORREF newBkColor, char newLetter);
	int GetStr() { return str; }
	int GetRow() { return row; }
	RECT GetRectFromCell();
	char GetLetter() { return letter[0]; }
	COLORREF GetBkColor() { return bkColor; }

private:
	int str;
	int row;
	int xLeft;
	int yTop;
	int sizeOfCell;
	char letter[2];
	COLORREF bkColor;
};

//--------------------------------------------------------

class CTable {

public:
	CTable(int nStr, int nRow);
	~CTable() {}

	void Draw(HDC &hdc);
	void InitCoord(int _xLeft, int _yTop, int _sizeOfGap, int _sizeOfCell);
	int GetCountOfStrings();
	int GetCountOfRows();
	void SetLetter(int str, int row, char letter[2]);
	bool FindCell(int xPos, int yPos, CCell &cell);
	void ChangeOneCell(HDC &hdc, int str, int row, COLORREF newBkColor, char newLetter);
	void ClearBkColor();

private:
	int xLeft;
	int yTop;
	int sizeOfCell;
	int sizeOfGap;
	std::vector <std::vector <CCell> > table;
};

//--------------------------------------------------------

class CPlayer {

public:
	CPlayer() {}
	~CPlayer() {}
	void SetName(const std::string &name);
	void AddWord(const std::string &newWord);
	void InitCoord(int _xLeft, int _yTop, int _xRight, int _yBottom);
	void Draw(HDC &hdc);
	void SetScore(int _score) { score = _score; }
	std::string GetName() { return name; }
	void Clear();

private:
	std::string scoreToString();
	std::vector <std::string> words;
	std::string name;
	int score;
	int xLeft;
	int yTop;
	int xRight;
	int yBottom;
};

//--------------------------------------------------------

class CGame {

public:
	CGame();
	~CGame() {};

	void Draw(HDC &hdc);
	void InitCoord(RECT rect);
	bool FindCell(int xPos, int yPos, int &type, CCell &cell);
	void ChangeOneCell(HDC &hdc, int type, int str, int row, COLORREF newBkColor, char newLetter);
	void PrintInt(HWND hWnd, LPCSTR title, int n);
	void ChooseCell(int str, int row, char letter);
	void AddCell(int str, int row);
	bool GetIsItTimeToChoose() { return isItTimeToChoose; }
	void SetIsItTimeToChoose(bool value) { isItTimeToChoose = value; }
	bool GetIsStarted() { return isStarted; }
	void SetIsStarted(bool value) { isStarted = value; }
	bool GetIsCellChosen() { return isCellChosen; };
	void SetIsCellChosen(bool value) { isCellChosen = value; }
	int GetStrOfChosenCell() { return strOfChosenCell; }
	int GetRowOfChosenCell() { return rowOfChosenCell; }
	bool GetIsCellAdded() { return isCellAdded; }
	char GetLetterOfChosenCell() { return letterOfChosenCell; }
	int GetStrOfAddedCell() { return strOfAddedCell; }
	int GetRowOfAddedCell() { return rowOfAddedCell; }
	bool IsPathGood(int str, int row);
	void AddLetterToPath(int str, int row, char letter);
	void SetIsCellAdded(bool value) { isCellAdded = value; }
	bool AddWord(std::string &message);
	void NextPlayer(int add);
	void AddMainWord(const std::string &mainWord);
	void NewGame(HDC &hdc);

private:
	CTable field;
	CTable alphabet;
	CDictionary dictionary;
	CPlayer players[2];
	int currentPlayer;
	bool isCellChosen;
	int strOfChosenCell;
	int rowOfChosenCell;
	char letterOfChosenCell;
	bool isCellAdded;
	int strOfAddedCell;
	int rowOfAddedCell;
	bool isItTimeToChoose;
	bool isStarted;
	std::vector <std::pair <int, int> > currentPath;
	std::string currentWord;
	int xLeftOfCurrentPlayer;
	int xRightOfCurrentPlayer;
	int yTopOfCurrentPlayer;;
	int yBottomOfCurrentPlayer;
	std::string getNameOfCurrentPlayer();
};