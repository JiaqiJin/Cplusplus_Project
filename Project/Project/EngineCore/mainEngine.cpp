#include <windows.h>
#include <stdint.h>

typedef int8_t int8; // 
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8; // unsigned char
typedef uint16_t uint16; //unsigned short
typedef uint32_t uint32; //unsigned int
typedef uint64_t uint64; //long long

#define global_variable static
#define interval static

global_variable bool Running = true;

global_variable void* BitmapMemory;
global_variable BITMAPINFO BitmapInfo;
global_variable int BytesPerPixel = 4;
global_variable int BitmapWidth;
global_variable int BitmapHeight;

void RenderWeirdGradeint(int XOffset, int YOffset) {
    uint8* Row = (uint8*)BitmapMemory;

    for (int Y = 0; Y < BitmapHeight; Y++) {
        uint32* Pixel = (uint32*)Row;

        for (int X = 0; X < BitmapWidth; X++) {
            uint8 Blue = X + XOffset;
            uint8 Green = Y + YOffset;
            // 0xXXRRGGBB
            *Pixel++ = (Green << 8) | Blue;
        }

        Row = Row + BitmapWidth * BytesPerPixel;
    }
}

void Win32ResizeDIBSection(int Width, int Height) {
    if (BitmapMemory) {
        VirtualFree(BitmapMemory, 0, MEM_RELEASE);
    }

    BitmapWidth = Width;
    BitmapHeight = Height;

    BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
    BitmapInfo.bmiHeader.biWidth = Width;
    BitmapInfo.bmiHeader.biHeight = -Height;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = BytesPerPixel * 8;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;

    int BitmapSize = Width * Height * BytesPerPixel;

    BitmapMemory = VirtualAlloc(0, BitmapSize, MEM_COMMIT, PAGE_READWRITE);
}

void Win32UpdateWindow(HDC DeviceContext,RECT* WindowRect, int X, int Y, int Width, int Height) {
    int WindowWidth = WindowRect->right - WindowRect->left;
    int WindowHeight = WindowRect->bottom - WindowRect->top;
    StretchDIBits(
        DeviceContext,
        0, 0, BitmapWidth, BitmapHeight,
        0, 0, WindowWidth, WindowHeight,
        BitmapMemory,
        &BitmapInfo,
        DIB_RGB_COLORS,
        SRCCOPY
    );
}

LRESULT CALLBACK Win32MainWindowCallback(
    HWND Window,
    UINT Message,
    WPARAM WParam,
    LPARAM LParam
) {
    LRESULT Result = 0;
    switch (Message) {
    case WM_ACTIVATEAPP:
    {
        OutputDebugStringA("WM_ACTIVATEAPP\n");
    } break;

    case WM_CLOSE:
    {
        Running = false;
        OutputDebugStringA("WM_CLOSE\n");
    } break;

    case WM_DESTROY:
    {
        OutputDebugStringA("WM_DESTROY\n");
    } break;

    case WM_PAINT:
    {
        PAINTSTRUCT PaintStruct = {};
        HDC DeviceContext = BeginPaint(Window, &PaintStruct);
        int X = PaintStruct.rcPaint.left;
        int Y = PaintStruct.rcPaint.top;
        int Width = PaintStruct.rcPaint.right - PaintStruct.rcPaint.left;
        int Height = PaintStruct.rcPaint.bottom - PaintStruct.rcPaint.top;
        RECT ClientRect;
        GetClientRect(Window, &ClientRect);
        Win32UpdateWindow(DeviceContext, &ClientRect, X, Y, Width, Height);
        EndPaint(Window, &PaintStruct);
    } break;

    case WM_SIZE:
    {
        RECT ClientRect;
        GetClientRect(Window, &ClientRect);
        int Width = ClientRect.right - ClientRect.left;
        int Height = ClientRect.bottom - ClientRect.top;
        Win32ResizeDIBSection(Width, Height);
        //OutputDebugStringA("WM_SIZE\n");
    } break;

    default:
    {
        Result = DefWindowProc(Window, Message, WParam, LParam);
    } break;
    }

    return Result;
}

int CALLBACK WinMain(
    HINSTANCE Instance,
    HINSTANCE PrevInstance,
    LPSTR     CmdLine,
    int       ShowCmd
) {
    WNDCLASS WindowClass = {};

    // TODO: Check if we need these
    WindowClass.style = CS_HREDRAW | CS_VREDRAW;

    WindowClass.lpfnWndProc = Win32MainWindowCallback;
    WindowClass.hInstance = Instance;
    WindowClass.lpszClassName = (LPCWSTR)"KawaiiWindowClass";

    if (RegisterClass(&WindowClass)) {
        HWND Window = CreateWindowEx(
            0,
            (LPCWSTR)"KawaiiWindowClass",
            (LPCWSTR)"Kawaii Desune",
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            Instance,
            0
        );

        if (Window) {
            int XOffset = 0;
            int YOffset = 0;

            while (Running) {
                MSG Message = {};

                while (PeekMessage(&Message, 0, 0, 0, PM_REMOVE)) {
                    TranslateMessage(&Message);
                    DispatchMessage(&Message);
                }

                RenderWeirdGradeint(XOffset, YOffset);
                HDC DeviceContext = GetDC(Window);
                RECT ClientRect;
                GetClientRect(Window, &ClientRect);
                int WindowWidth = ClientRect.right - ClientRect.left;
                int WindowHeight = ClientRect.bottom - ClientRect.top;
                Win32UpdateWindow(DeviceContext, &ClientRect, 0, 0, WindowWidth, WindowHeight);
                ReleaseDC(Window, DeviceContext);

                XOffset++;
                YOffset += 2;
            }
        }
        else {
            // TODO: Logging
        }
    }
    else {
        // TDOO: Logging
    }

    return 0;
}