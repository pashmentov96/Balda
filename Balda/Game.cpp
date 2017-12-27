#include "stdafx.h"
#include "Game.h"

const char ALPHABET[33] = {'А', 'Б', 'В', 'Г', 'Д', 'Е', 'Ё', 'Ж', 'З', 'И', 'Й', 
	'К', 'Л', 'М', 'Н', 'О', 'П', 'Р', 'С', 'Т', 'У', 'Ф', 'Х', 'Ц', 'Ч', 'Ш', 'Щ', 'Ъ',
	'Ы', 'Ь', 'Э', 'Ю', 'Я'};

CCell::CCell(int str, int row, char _letter[2], COLORREF bkColor):
	str(str),
	row(row),
	bkColor(bkColor)
{
	strcpy_s(letter, _letter);
}

void CCell::Draw(HDC &hdc)
{
	HBRUSH hBrush = CreateSolidBrush(bkColor);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	HFONT hFont = CreateFont(sizeOfCell - 2, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Arial");
	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	COLORREF oldBkColor = SetBkColor(hdc, bkColor);

	Rectangle(hdc, xLeft, yTop, xLeft + sizeOfCell, yTop + sizeOfCell);
	RECT currentRect;
	SetRect(&currentRect, xLeft, yTop, xLeft + sizeOfCell, yTop + sizeOfCell);
	DrawTextA(hdc, letter, -1, &currentRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	DeleteObject(hBrush);
	SelectObject(hdc, hOldBrush);

	DeleteObject(hFont);
	SelectObject(hdc, oldFont);

	SetBkColor(hdc, oldBkColor);
}

void CCell::InitCoord(int xLeftOfTable, int yTopOfTable, int sizeOfGap, int _sizeOfCell) 
{
	sizeOfCell = _sizeOfCell;
	xLeft = xLeftOfTable + row * (sizeOfCell + sizeOfGap);
	yTop = yTopOfTable + str * (sizeOfCell + sizeOfGap);
}

void CCell::SetBkColorOfCell(COLORREF newBkColor)
{
	bkColor = newBkColor;
}

void CCell::SetLetter(char _letter[2])
{
	strcpy_s(letter, _letter);
}

bool CCell::IsContained(int xPos, int yPos)
{
	int xRight = xLeft + sizeOfCell;
	int yBottom = yTop + sizeOfCell;
	return xLeft < xPos && xPos < xRight && yTop < yPos && yPos < yBottom;
}

void CCell::ChangeCell(HDC &hdc, COLORREF newBkColor, char newLetter)
{
	SetBkColorOfCell(newBkColor);
	if (newLetter != -1) {
		letter[0] = newLetter;
		letter[1] = 0;
	}
	Draw(hdc);
}

RECT CCell::GetRectFromCell()
{
	RECT rect;
	int xRight = xLeft + sizeOfCell;
	int yBottom = yTop + sizeOfCell;
	SetRect(&rect, xLeft, yTop, xRight, yBottom);
	return rect;
}

//--------------------------------------------------------

CTable::CTable(int nStr, int nRow)
{
	table.resize(nStr);
	for (int i = 0; i < nStr; ++i) {
		table[i].resize(nRow);
		for (int j = 0; j < nRow; ++j) {
			table[i][j] = CCell(i, j, "", COMMON_CELL);
		}
	}
}

void CTable::InitCoord(int _xLeft, int _yTop, int _sizeOfGap, int _sizeOfCell)
{
	xLeft = _xLeft;
	yTop = _yTop;
	sizeOfGap = _sizeOfGap;
	sizeOfCell = _sizeOfCell;
	for (int i = 0; i < (int)table.size(); ++i) {
		for (int j = 0; j < (int)table[i].size(); ++j) {
			table[i][j].InitCoord(xLeft, yTop, sizeOfGap, sizeOfCell);
		}
	}
}

void CTable::Draw(HDC &hdc)
{
	for (int i = 0; i < (int)table.size(); ++i) {
		for (int j = 0; j < (int)table[i].size(); ++j) {
			table[i][j].Draw(hdc);
		}
	}
}

int CTable::GetCountOfStrings()
{
	return (int)table.size();
}

int CTable::GetCountOfRows()
{
	return (int)table[0].size();
}

void CTable::SetLetter(int str, int row, char letter[2])
{
	table[str][row].SetLetter(letter);
}

bool CTable::FindCell(int xPos, int yPos, CCell &cell)
{
	for (int i = 0; i < (int)table.size(); ++i) {
		for (int j = 0; j < (int)table[i].size(); ++j) {
			if (table[i][j].IsContained(xPos, yPos)) {
				//str = i;
				//row = j;
				cell = table[i][j];
				return true;
			}
		}
	}
	return false;
}

void CTable::ChangeOneCell(HDC &hdc, int str, int row, COLORREF newBkColor, char newLetter)
{
	table[str][row].ChangeCell(hdc, newBkColor, newLetter);
}

void CTable::ClearBkColor()
{
	for (int i = 0; i < (int)table.size(); ++i) {
		for (int j = 0; j < (int)table[i].size(); ++j) {
			table[i][j].SetBkColorOfCell(COMMON_CELL);
		}
	}
}

//-------------------------------------------------------

void CPlayer::SetName(const std::string &_name)
{
	name = _name;
}

void CPlayer::AddWord(const std::string &newWord)
{
	words.push_back(newWord);
	score += (int)newWord.size();
}

void CPlayer::InitCoord(int _xLeft, int _yTop, int _xRight, int _yBottom)
{
	xLeft = _xLeft;
	yTop = _yTop;
	xRight = _xRight;
	yBottom = _yBottom;

}

void CPlayer::Draw(HDC &hdc)
{
	Rectangle(hdc, xLeft, yTop, xRight, yBottom);

	int size = max((yBottom - yTop) / 15, 10);
	int distToTop = 2;
	int sizeOfGap = 2;

	HFONT hFont = CreateFont(size, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Arial");
	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	RECT currentRect;
	SetRect(&currentRect, xLeft, yTop + distToTop, xRight, yTop + distToTop + size);
	DrawTextA(hdc, name.c_str(), -1, &currentRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	currentRect.bottom += sizeOfGap + size;
	currentRect.top += sizeOfGap + size;

	DrawTextA(hdc, scoreToString().c_str(), -1, &currentRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	for (int i = 0; i < (int)words.size(); ++i) {
		currentRect.bottom += sizeOfGap + size;
		currentRect.top += sizeOfGap + size;
		DrawTextA(hdc, words[i].c_str(), -1, &currentRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	DeleteObject(hFont);
	SelectObject(hdc, oldFont);
}

std::string CPlayer::scoreToString()
{
	char buffer[15];
	_itoa_s(score, buffer, 10);
	return "Очки: " + std::string(buffer);
}

void CPlayer::Clear()
{
	score = 0;
	words.clear();
}

//--------------------------------------------------------

CGame::CGame():
	dictionary(CDictionary()),
	field(CTable(5, 5)),
	alphabet(CTable(3, 11))
{
	int pos = 0;
	char letter[2] = {0, 0};
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 11; ++j) {
			letter[0] = ALPHABET[pos];
			pos++;
			alphabet.SetLetter(i, j, letter);
		}
	}
	players[0].SetName("Игрок 1");
	players[0].SetScore(0);

	players[1].SetName("Игрок 2");
	players[1].SetScore(0);

	currentPlayer = 0;
	isCellChosen = false;
	isCellAdded = false;
	isItTimeToChoose = false;
	isStarted = false;
}

void CGame::Draw(HDC &hdc)
{
	field.Draw(hdc);
	alphabet.Draw(hdc);
	players[0].Draw(hdc);
	players[1].Draw(hdc);

	int size = (yBottomOfCurrentPlayer - yTopOfCurrentPlayer) / 2;

	HFONT hFont = CreateFont(size, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Arial");
	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	RECT rect;
	SetRect(&rect, xLeftOfCurrentPlayer, yTopOfCurrentPlayer, xRightOfCurrentPlayer, yBottomOfCurrentPlayer);
	DrawTextA(hdc, getNameOfCurrentPlayer().c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	DeleteObject(hFont);
	SelectObject(hdc, oldFont);
}

void CGame::InitCoord(RECT rect)
{
	int xSize = rect.right - rect.left;
	int ySize = rect.bottom - rect.top;

	int sizeOfCellOfField = min(xSize / 15, ySize / 10);
	int sizeOfGapOfField = min(2, sizeOfCellOfField / field.GetCountOfStrings());
	int distToMenu = min(10, ySize / 30);

	int xLeftOfField = (int)((rect.left + rect.right) / 2.0 - ((int)field.GetCountOfRows()) * sizeOfCellOfField / 2.0 - (field.GetCountOfRows() / 2) * sizeOfGapOfField);
	int yTopOfField = rect.top + distToMenu;

	field.InitCoord(xLeftOfField, yTopOfField, sizeOfGapOfField, sizeOfCellOfField);

	int sizeOfCellOfAlphabet = min(3 * xSize / (4 * alphabet.GetCountOfRows()), ySize / (3 * alphabet.GetCountOfStrings()));
	int sizeOfGapOfAlphabet = min(2, sizeOfCellOfAlphabet / alphabet.GetCountOfStrings());
	int distToBottom = min(10, ySize / 10);

	int xLeftOfAlphabet = (int)((rect.left + rect.right) / 2.0 - ((int)alphabet.GetCountOfRows()) * sizeOfCellOfAlphabet / 2.0 - (alphabet.GetCountOfRows() / 2) * sizeOfGapOfAlphabet);
	int yTopOfAlphabet = rect.bottom - distToBottom - sizeOfCellOfAlphabet * alphabet.GetCountOfStrings() - sizeOfGapOfAlphabet * (alphabet.GetCountOfStrings() - 1);

	alphabet.InitCoord(xLeftOfAlphabet, yTopOfAlphabet, sizeOfGapOfAlphabet, sizeOfCellOfAlphabet);

	int distToSide = 3;
	int distToField = 10;
	int distToAlphabet = 10;

	players[0].InitCoord(rect.left + distToSide, rect.top + distToMenu, xLeftOfAlphabet - distToField, yTopOfAlphabet - distToAlphabet);
	players[1].InitCoord((rect.right - distToField) - ((xLeftOfAlphabet - distToField) - (rect.left + distToSide)), rect.top + distToMenu, rect.right - distToField, yTopOfAlphabet - distToAlphabet);

	xLeftOfCurrentPlayer = xLeftOfField;
	xRightOfCurrentPlayer = xLeftOfField + field.GetCountOfRows() * (sizeOfCellOfField + sizeOfGapOfField);
	yTopOfCurrentPlayer = yTopOfField + field.GetCountOfStrings() * (sizeOfCellOfField + sizeOfGapOfField);
	yBottomOfCurrentPlayer = yTopOfAlphabet;
}

bool CGame::FindCell(int xPos, int yPos, int &type, CCell &cell)
{
	if (field.FindCell(xPos, yPos, cell)) {
		type = CELLFROMFIELD;
		return true;
	}
	if (alphabet.FindCell(xPos, yPos, cell)) {
		type = CELLFROMALPHABET;
		return true;
	}
	type = NOTCELL;
	return false;
}

void CGame::ChangeOneCell(HDC &hdc, int type, int str, int row, COLORREF newBkColor, char newLetter)
{
	switch (type)
	{
	case CELLFROMFIELD:
		field.ChangeOneCell(hdc, str, row, newBkColor, newLetter);
		break;
	case CELLFROMALPHABET:
		alphabet.ChangeOneCell(hdc, str, row, newBkColor, newLetter);
		break;
	default:
		return ;
	}
}

void CGame::ChooseCell(int str, int row, char letter)
{
	isCellChosen = true;
	strOfChosenCell = str;
	rowOfChosenCell = row;
	letterOfChosenCell = letter;
}

void CGame::AddCell(int str, int row)
{
	isCellAdded = true;
	strOfAddedCell = str;
	rowOfAddedCell = row;
}

bool CGame::IsPathGood(int str, int row)
{
	if (currentPath.empty()) {
		return true;
	}
	if (std::abs(str - currentPath.back().first) + std::abs(row - currentPath.back().second) != 1) {
		return false;
	}
	for (int i = 0; i < (int)currentPath.size(); ++i) {
		if (currentPath[i] == std::make_pair(str, row)) {
			return false;
		}
	}
	return true;
}

void CGame::AddLetterToPath(int str, int row, char letter)
{
	currentPath.push_back(std::make_pair(str, row));
	currentWord += letter;
}

bool CGame::AddWord(std::string &message)
{
	if (dictionary.IsFound(currentWord)) {
		message = "Такое слово уже было! Попробуйте еще раз";
		return false;
	}
	bool isGood = false;
	for (int i = 0; i < (int)currentPath.size(); ++i) {
		if (currentPath[i] == std::make_pair(strOfAddedCell, rowOfAddedCell)) {
			isGood = true;
			break;
		}
	}
	if (!isGood) {
		message = "Выбранное слово должно содержать Вашу букву. Попробуйте еще раз";
		return false;
	}
	dictionary.Insert(currentWord);
	players[currentPlayer].AddWord(currentWord);
	return true;
}

void CGame::NextPlayer(int add)
{
	if (add == 1) {
		currentPlayer = 1 - currentPlayer;
	}
	currentPath.clear();
	currentWord.clear();
	isCellAdded = false;
	isCellChosen = false;
	isItTimeToChoose = false;
	field.ClearBkColor();
	alphabet.ClearBkColor();
}

std::string CGame::getNameOfCurrentPlayer()
{
	return "Сейчас ходит " + players[currentPlayer].GetName();
}

void CGame::AddMainWord(const std::string &mainWord)
{
	dictionary.Insert(mainWord);
}

void CGame::NewGame(HDC &hdc)
{
	field.ClearBkColor();
	for (int i = 0; i < field.GetCountOfStrings(); ++i) {
		for (int j = 0; j < field.GetCountOfRows(); ++j) {
			ChangeOneCell(hdc, CELLFROMFIELD, i, j, COMMON_CELL, 0);
		}
	}
	alphabet.ClearBkColor();
	dictionary.Clear();
	players[0].Clear();
	players[1].Clear();
	currentPlayer = 0;
	isCellChosen = false;
	isCellAdded = false;
	isItTimeToChoose = false;
	isStarted = false;
	currentPath.clear();
	currentWord.clear();
}

//------------------------

void CGame::PrintInt(HWND hWnd, LPCSTR title, int n) {
	char buffer[15];
	_itoa_s(n, buffer, 10);
	MessageBoxA(hWnd, buffer, title, MB_OK | MB_ICONINFORMATION);
}