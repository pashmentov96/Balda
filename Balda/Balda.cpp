// Balda.cpp: Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Balda.h"
#include <windowsx.h>

#define MAX_LOADSTRING 100

// Глобальные переменные:
CGame game;
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Rules(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Control(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BALDA, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BALDA));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BALDA));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_BALDA);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

   HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

/*void PrintInt(HWND hWnd, LPCSTR title, int n) {
	char buffer[15];
	_itoa_s(n, buffer, 10);
	MessageBoxA(hWnd, buffer, title, MB_OK | MB_ICONINFORMATION);
}*/

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND — обработать меню приложения
//  WM_PAINT — отрисовать главное окно
//  WM_DESTROY — отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
				{
					int userReply = MessageBox(hWnd, L"Вы действительно хотите выйти?", L"Выход", MB_YESNO | MB_ICONQUESTION);
					if (userReply == IDYES) {
						DestroyWindow(hWnd);
					}
				}
                break;
			case IDM_RULES:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_RULESBOX), hWnd, Rules);
				break;
			case IDM_CONTROL:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_CONTROL), hWnd, Control);
				break;
			case IDM_NEWGAME:
				{
					PAINTSTRUCT ps;
					HDC hdc = BeginPaint(hWnd, &ps);

					game.NewGame(hdc);

					std::string mainWord = "ПАЛКА";
					game.SetIsStarted(true);
					game.AddMainWord(mainWord);

					for (int i = 0; i < 5; ++i) {
						game.ChangeOneCell(hdc, CELLFROMFIELD, 2, i, COMMON_CELL, mainWord[i]);
					}

					InvalidateRect(hWnd, NULL, FALSE);
					UpdateWindow(hWnd);

					EndPaint(hWnd, &ps);
				}
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			RECT rect;
			GetClientRect(hWnd, &rect);
			game.InitCoord(rect);
			game.Draw(hdc);

			// TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_CLOSE:
		{
			int userReply = MessageBox(hWnd, L"Вы действительно хотите выйти?", L"Выход", MB_YESNO | MB_ICONQUESTION);
			if (userReply == IDYES) {
				DestroyWindow(hWnd);
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);
			int type = NOTCELL;
			CCell cell;
			if (game.GetIsStarted() && game.FindCell(xPos, yPos, type, cell)) {
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);

				if (game.GetIsCellChosen()) {
					if (type == CELLFROMALPHABET) {
						if (!game.GetIsCellAdded() && cell.GetStr() == game.GetStrOfChosenCell() && cell.GetRow() == game.GetRowOfChosenCell()) {
							game.SetIsCellChosen(false);
							game.ChangeOneCell(hdc, type, cell.GetStr(), cell.GetRow(), COMMON_CELL, -1);
							RECT rect = cell.GetRectFromCell();
							InvalidateRect(hWnd, &rect, FALSE);
							UpdateWindow(hWnd);
						}
					}
					else {
						if (game.GetIsCellAdded()) {
							if (cell.GetLetter() != 0 && game.IsPathGood(cell.GetStr(), cell.GetRow())) {
								game.SetIsItTimeToChoose(true);
								game.AddLetterToPath(cell.GetStr(), cell.GetRow(), cell.GetLetter());
								game.ChangeOneCell(hdc, type, cell.GetStr(), cell.GetRow(), CLICKED_CELL, -1);
								RECT rect = cell.GetRectFromCell();
								InvalidateRect(hWnd, &rect, FALSE);
								UpdateWindow(hWnd);
							}
						}
						else {
							if (cell.GetBkColor() == COMMON_CELL) {
								game.AddCell(cell.GetStr(), cell.GetRow());
								game.ChangeOneCell(hdc, type, cell.GetStr(), cell.GetRow(), ADDED_CELL, game.GetLetterOfChosenCell());
								game.ChangeOneCell(hdc, CELLFROMALPHABET, game.GetStrOfChosenCell(), game.GetRowOfChosenCell(), COMMON_CELL, -1);
								InvalidateRect(hWnd, NULL, FALSE); 
								UpdateWindow(hWnd);
							}
						}
					}
				}
				else {
					if (type == CELLFROMALPHABET) {
						game.ChooseCell(cell.GetStr(), cell.GetRow(), cell.GetLetter());
						game.ChangeOneCell(hdc, type, cell.GetStr(), cell.GetRow(), CLICKED_CELL, -1);
						RECT rect = cell.GetRectFromCell();
						InvalidateRect(hWnd, &rect, FALSE);
						UpdateWindow(hWnd);
					}
				}

				EndPaint(hWnd, &ps);
			}
		}
		break;
	case WM_KEYDOWN:
		if (wParam == VK_RETURN) {
			if (game.GetIsItTimeToChoose()) {
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);

				std::string message;
				bool ok = game.AddWord(message);
				if (ok == false) {
					MessageBoxA(hWnd, message.c_str(), "Ошибка", MB_OK | MB_ICONERROR);
					game.ChangeOneCell(hdc, CELLFROMFIELD, game.GetStrOfAddedCell(), game.GetRowOfAddedCell(), COMMON_CELL, 0);
					game.NextPlayer(0);
				}
				else {
					game.NextPlayer(1);
				}

				game.Draw(hdc);
				InvalidateRect(hWnd, NULL, FALSE);
				UpdateWindow(hWnd);

				EndPaint(hWnd, &ps);
			}
		}
		else if (wParam == VK_BACK) {
			if (game.GetIsCellAdded()) {
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);

				game.ChangeOneCell(hdc, CELLFROMFIELD, game.GetStrOfAddedCell(), game.GetRowOfAddedCell(), COMMON_CELL, 0);
				game.NextPlayer(0);

				game.Draw(hdc);
				InvalidateRect(hWnd, NULL, FALSE);
				UpdateWindow(hWnd);

				EndPaint(hWnd, &ps);
			}
		}
		break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK Rules(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;

	case WM_PAINT:
		{
			HANDLE hf;
			hf = CreateFile(L"Rules.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
			if (hf == INVALID_HANDLE_VALUE) {
				MessageBox(hDlg, L"Open failed", L"Error", MB_ICONHAND | MB_OK);
				break;
			}
			DWORD lenOfFile = GetFileSize(hf, NULL);
			char* buf = (char*)malloc(sizeof(char) * (lenOfFile + 1));
			if (buf == NULL) {
				MessageBox(hDlg, L"Memory alloc failed", L"Error", MB_ICONHAND | MB_OK);
				break;
			}
			DWORD len;
			ReadFile(hf, buf, lenOfFile, &len, NULL);
			buf[len] = 0;
			CloseHandle(hf);

			PAINTSTRUCT ps;
			RECT rect;
			HDC hdc = BeginPaint(hDlg, &ps);
			SetTextColor(hdc, RGB(0, 0, 255));
			GetClientRect(hDlg, &rect);
			DrawTextA(hdc, buf, -1, &rect, DT_TOP); //Noooo
													// TODO: Добавьте сюда любой код прорисовки, использующий HDC...
			EndPaint(hDlg, &ps);
			free(buf);
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// Обработчик сообщений для окна "Управление".
INT_PTR CALLBACK Control(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}