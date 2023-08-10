// DAT154-Oblig1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "DAT154-Oblig1.h"
#include <vector>
#include <windows.h>
#include <random>
#include <iostream>
using namespace std;

#define MAX_LOADSTRING 100
#define ID_TIMER 1
#define ID_MOVE 3
#define ID_SPAWN 4

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DAT154OBLIG1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DAT154OBLIG1));

    MSG msg;

    // Main message loop:
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DAT154OBLIG1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DAT154OBLIG1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
    175, 80, 1600, 900, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }


   SetTimer(hWnd, ID_TIMER, 5000, NULL);
   SetTimer(hWnd, ID_MOVE, 5, NULL);
   SetTimer(hWnd, ID_SPAWN, 500, NULL);



   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);


   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

const int LIGHT_SIZE = 40;
const int BORDER_WIDTH = 2;



void DrawRoads(HDC hdc, int x, int y, int width, int height)
{
    HBRUSH hBrush = CreateSolidBrush(RGB(128, 128, 128));
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HGDIOBJ hOldBrush = SelectObject(hdc, hBrush);
    HGDIOBJ hOldPen = SelectObject(hdc, hPen);
    Rectangle(hdc, x, y, x + width, y + height);
    SelectObject(hdc, hOldBrush);
    SelectObject(hdc, hOldPen);
    DeleteObject(hBrush);
    DeleteObject(hPen);
}

void DrawTrafficLight(HDC hdc, int x, int y, int color)
{
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
    HPEN hPen = CreatePen(PS_SOLID, BORDER_WIDTH, RGB(0, 0, 0));
    HGDIOBJ hOldBrush = SelectObject(hdc, hBrush);
    HGDIOBJ hOldPen = SelectObject(hdc, hPen);

    Rectangle(hdc, x - BORDER_WIDTH, y - BORDER_WIDTH, x + LIGHT_SIZE + BORDER_WIDTH, y + LIGHT_SIZE * 3 + BORDER_WIDTH);
    DeleteObject(hBrush);
    DeleteObject(hPen);

    hBrush = CreateSolidBrush(RGB(255, 0, 0));
    hOldBrush = SelectObject(hdc, hBrush);
    if (color == 0 || color == 1)
    {
        Ellipse(hdc, x, y, x + LIGHT_SIZE, y + LIGHT_SIZE);
    }
    DeleteObject(hBrush);

    hBrush = CreateSolidBrush(RGB(255, 255, 0));
    hOldBrush = SelectObject(hdc, hBrush);
    if (color == 1 || color == 3)
    {
        Ellipse(hdc, x, y + LIGHT_SIZE, x + LIGHT_SIZE, y + LIGHT_SIZE * 2);
    }
    DeleteObject(hBrush);


    hBrush = CreateSolidBrush(RGB(0, 255, 0));
    hOldBrush = SelectObject(hdc, hBrush);
    if (color == 2)
    {
        Ellipse(hdc, x, y + LIGHT_SIZE * 2, x + LIGHT_SIZE, y + LIGHT_SIZE * 3);
    }
    DeleteObject(hBrush);

    SelectObject(hdc, hOldBrush);
    DeleteObject(hBrush);
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}



struct CarWest {
    int x;
    int y;
    int size;
};

CarWest createCarWest(int x, int y, int size) {
    CarWest carWest;
    carWest.x = x;
    carWest.y = y;
    carWest.size = size;
    return carWest;
}

void drawCarWest(HDC hdc, CarWest carWest)
{
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255));
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HGDIOBJ hOldBrush = SelectObject(hdc, hBrush);
    HGDIOBJ hOldPen = SelectObject(hdc, hPen);

    Rectangle(hdc, carWest.x, carWest.y, carWest.x + carWest.size * 2, carWest.y + carWest.size);

    SelectObject(hdc, hOldBrush);
    DeleteObject(hBrush);
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}

vector<CarWest> carsWest;


struct CarNorth {
    int x;
    int y;
    int size;
};

CarNorth createCarNorth(int x, int y, int size) {
    CarNorth carNorth;
    carNorth.x = x;
    carNorth.y = y;
    carNorth.size = size;
    return carNorth;
}

void drawCarNorth(HDC hdc, CarNorth carNorth)
{
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255));
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HGDIOBJ hOldBrush = SelectObject(hdc, hBrush);
    HGDIOBJ hOldPen = SelectObject(hdc, hPen);

    Rectangle(hdc, carNorth.x, carNorth.y, carNorth.x + carNorth.size, carNorth.y + carNorth.size *2);

    SelectObject(hdc, hOldBrush);
    DeleteObject(hBrush);
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}

vector<CarNorth> carsNorth;


int color1 = 0;
int color2 = 2;

double pn = 0.05;
double pw = 0.1;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_LBUTTONDOWN:
    {
        int x = -100;
        int y = 375;
        int spacing = 20;
        if (!carsWest.empty()) {
            x = carsWest.back().x - carsWest.back().size * 2 - spacing;
        }
        CarWest carWest = createCarWest(x, y, 50);
        carsWest.push_back(carWest);
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }
   
    case WM_RBUTTONDOWN:
    {
        int x = 750;
        int y = -100;
        int spacing = 20;
        if (!carsNorth.empty()) {
            y = -100;
            if (carsNorth.back().y - y < carsNorth.back().size * 2 + 5) {
                y = carsNorth.back().y - carsNorth.back().size * 2 - spacing;
            }
        }
        CarNorth carNorth = createCarNorth(x, y, 50);
        carsNorth.push_back(carNorth);
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }

    case WM_KEYDOWN:
    {
        switch (wParam)
        {
            case VK_RIGHT:
            {
                pw = pw + 0.1;
                break;
            }
            case VK_LEFT:
            {
                pw = pw - 0.1;
                break;
            }
            case VK_UP:
            {
                pn = pn + 0.1;
                break;
            }
            case VK_DOWN:
            {
                pn = pn - 0.1;
                break;
            }
        }
    }
    break;

    case WM_TIMER:
    {
        switch (wParam)
        {
        case ID_TIMER:
            {
                color1 = (color1 + 1) % 4;
                color2 = (color2 + 1) % 4;

                InvalidateRect(hWnd, NULL, FALSE);
            }
            break;
        case ID_MOVE:
            {
                for (int i = 0; i < carsWest.size(); i++)
                {
                    CarWest& carWest = carsWest[i];
                    bool carInFront = false;
                    for (int j = 0; j < carsWest.size(); j++)
                    {
                        if (i != j)
                        {
                            CarWest& otherCar = carsWest[j];
                            if (carWest.x < otherCar.x && otherCar.x - carWest.x < carWest.size * 2 + 5)
                            {
                                carInFront = true;
                                break;
                            }
                        }
                    }
                    if (carWest.x == 595 && (color2 == 0 || color2 == 3 || color2 == 1))
                    {
                        carWest.x += 0;
                    }
                    else if (carInFront)
                    {
                        carWest.x += 0;
                    }
                    else {
                        carWest.x += 1;
                    }
                }   

                for (int i = 0; i < carsNorth.size(); i++)
                {
                    CarNorth& carNorth = carsNorth[i];
                    bool carInFront = false;
                    for (int j = 0; j < carsNorth.size(); j++)
                    {
                        if (i != j)
                        {
                            CarNorth& otherCar = carsNorth[j];
                            if (carNorth.y < otherCar.y && otherCar.y - carNorth.y < carNorth.size * 2 + 5)
                            {
                                carInFront = true;
                                break;
                            }
                        }
                    }
                    if (carNorth.y == 220 && (color1 == 0 || color1 == 3 || color1 == 1))
                    {
                        carNorth.y += 0;
                    }
                    else if (carInFront)
                    {
                        carNorth.y += 0;
                    }
                    else {
                        carNorth.y += 1;
                    }
                }


                InvalidateRect(hWnd, NULL, FALSE);
                break;
            }
        case ID_SPAWN:
            int carCount = 0;
            int maxCars = 50;
            while (carCount < maxCars)
            {
                double randomNumber = (double)rand() / RAND_MAX;
                if (randomNumber < pw)
                {
                    int x = -100;
                    int y = 375;
                    int spacing = 20;
                    if (!carsWest.empty()) {
                        x = -100;
                        if (carsWest.back().x - x < carsWest.back().size * 2 + 5) {
                            x = carsWest.back().x - carsWest.back().size * 2 - spacing;
                        }
                    }
                    CarWest carWest = createCarWest(x, y, 50);
                    carsWest.push_back(carWest);
                    carCount++;
                }
                if (randomNumber < pn)
                {
                    int x = 750;
                    int y = -100;
                    int spacing = 20;
                    if (!carsNorth.empty()) {
                        y = -100;
                        if (carsNorth.back().y - y < carsNorth.back().size * 2 + 5) {
                            y = carsNorth.back().y - carsNorth.back().size * 2 - spacing;
                        }
                    }
                    CarNorth carNorth = createCarNorth(x, y, 50);
                    carsNorth.push_back(carNorth);
                    carCount++;
                }
                for (auto it = carsNorth.begin(); it != carsNorth.end();) {
                    if (it->y > 900) {
                        it = carsNorth.erase(it);
                        carCount--;
                    }
                    else {
                        ++it;
                    }
                }
                for (auto it = carsWest.begin(); it != carsWest.end();) {
                    if (it->x > 1600) {
                        it = carsWest.erase(it);
                        carCount--;
                    }
                    else {
                        ++it;
                    }
                }
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            }
        }
    }
    
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            WCHAR buffer[50];
            swprintf(buffer, 50, L"pn = %lf, pw = %lf", pn, pw);
            TextOutW(hdc, 10, 10, buffer, (int)wcslen(buffer));

            DrawRoads(hdc, 700, 0, 150, 900);
            DrawRoads(hdc,0,325,1600,150);

            DrawTrafficLight(hdc, 860, 195, color1);
            DrawTrafficLight(hdc, 650, 485, color2);


            //draws west cars
            for (CarWest carWest : carsWest) {
                drawCarWest(hdc, carWest);
            }

            //draws north cars
            for (CarNorth carNorth : carsNorth) {
                drawCarNorth(hdc, carNorth);
            }

            EndPaint(hWnd, &ps);
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

// Message handler for about box.
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
