#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <memory>

#include "Win32Project1.h"

#include "backbuffer.h"
#include "raytracer.h"
#include "world.h"
#include "sphere.h"
#include "vec3.h"
#include "camera.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

std::unique_ptr< Backbuffer > backBuffer;
std::unique_ptr< RayTracer > rayTracer;
std::unique_ptr< World > world;
std::unique_ptr< Camera > camera;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    backBuffer = std::make_unique< Backbuffer >( 200, 100 );

    std::vector< std::shared_ptr< Hitable > > hitables = 
    {
        std::make_shared< Sphere >( Vec3( 0.0f, 0.0f, -1.0f ), 0.5f, std::make_shared< MaterialLambert >( Vec3( 0.8f, 0.3f, 0.3f ) ) ),
        std::make_shared< Sphere >( Vec3( 0.0f, -100.5f, -1.0f ), 100.0f, std::make_shared< MaterialLambert >( Vec3( 0.8f, 0.8f, 0.0f ) ) ),
        std::make_shared< Sphere >( Vec3( 1.0f, 0.0f, -1.0f ), 0.5f, std::make_shared< MaterialMetal>( Vec3( 0.8f, 0.6f, 0.2f ) ) ),
        std::make_shared< Sphere >( Vec3( -1.0f, 0.0f, -1.0f ), 0.5f, std::make_shared< MaterialMetal >( Vec3( 0.8f, 0.8f, 0.8f ) ) )
    };

    world = std::make_unique< World >( hitables );
    rayTracer = std::make_unique< RayTracer >();
    camera = std::make_unique< Camera >( 100 );

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32PROJECT1);
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
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
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
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            rayTracer->Process( *backBuffer.get(), *world.get(), *camera.get() );

            PAINTSTRUCT ps;
            RECT rect;
            HDC hdc = BeginPaint( hWnd, &ps );
            GetClientRect( hWnd, &rect );

            backBuffer->Draw( hdc, rect.right, rect.bottom );

            RECT textRect;
            textRect.left = 5;
            textRect.top = 5;
            textRect.right = 500;
            textRect.bottom = 30;
            SetTextColor( hdc, 0x00000080 );
            SetBkMode( hdc, TRANSPARENT );
            //DrawTextA( hdc, s_Buffer, ( int ) strlen( s_Buffer ), &textRect, DT_NOCLIP | DT_LEFT | DT_TOP );
            EndPaint( hWnd, &ps );
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