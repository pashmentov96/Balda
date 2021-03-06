// stdafx.h: включаемый файл для стандартных системных включаемых файлов
// или включаемых файлов для конкретного проекта, которые часто используются, но
// не часто изменяются
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Исключите редко используемые компоненты из заголовков Windows
// Файлы заголовков Windows:
#include <windows.h>

// Файлы заголовков C RunTime
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: Установите здесь ссылки на дополнительные заголовки, требующиеся для программы
#define COMMON_CELL  RGB(150, 100, 250)
#define CLICKED_CELL RGB(0, 255, 0)
#define ADDED_CELL   RGB(0, 0, 255)