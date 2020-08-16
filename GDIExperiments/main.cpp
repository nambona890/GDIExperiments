#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <stdlib.h>
#include <random>
#include <wincodec.h>
#include <wincodecsdk.h>

HCRYPTPROV prov;



int random();

DWORD WINAPI visualEffect1(LPVOID lpParam);
DWORD WINAPI visualEffect2(LPVOID lpParam);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	const wchar_t CLASS_NAME[] = L"GDI Experiments";

	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"plz get out",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    int numproc = 12;
    for(int i = 1; i < numproc;i++)
        if (CreateThread(NULL, NULL, visualEffect2, NULL, 0, NULL) == NULL)
        {
            MessageBoxA(hwnd, "uh some shit happened oops", "fuck", MB_OK);
        }

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int random() {
    if (prov == NULL)
        if (!CryptAcquireContext(&prov, NULL, NULL, PROV_RSA_FULL, CRYPT_SILENT | CRYPT_VERIFYCONTEXT))
            ExitProcess(1);

    int out;
    CryptGenRandom(prov, sizeof(out), (BYTE*)(&out));
    return out & 0x7fffffff;
}

DWORD WINAPI visualEffect1(LPVOID lpParam)
{
    HWND hwnd = GetDesktopWindow();
    HDC hdc = GetWindowDC(hwnd);
    RECT rect;

    GetWindowRect(hwnd, &rect);
    int w = rect.right - rect.left;
    int h = rect.bottom - rect.top;
    while (true)
    {
        int x = random() % w;
        int y = random() % h;
        RECT tmprectangle = { (LONG)x,(LONG)y,(LONG)x + 10,(LONG)y + 10 };
        const RECT* rectangle = &tmprectangle;
        HBRUSH rcol = CreateSolidBrush(random() % 0x00FFFFFF);
        FillRect(hdc, rectangle, rcol);
    }
}

DWORD WINAPI visualEffect2(LPVOID lpParam)
{
    HWND hwnd = GetDesktopWindow();
    HDC hdc = GetWindowDC(hwnd);
    RECT rect;

    GetWindowRect(hwnd, &rect);
    int w = rect.right - rect.left;
    int h = rect.bottom - rect.top;
    while (true)
    {
        int x1 = (random()*64) % w;
        int y1 = (random()*64) % h;
        int x2 = (random()*64) % w;
        int y2 = (random()*64) % h;
        BitBlt(hdc, x1, y1, 64, 64, hdc, x2, y2, SRCCOPY);
    }
}