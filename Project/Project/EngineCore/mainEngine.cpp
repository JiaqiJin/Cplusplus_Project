#include<Windows.h>

#define internal static
#define local_persist static
#define global_variable static 

global_variable bool Running;
global_variable BITMAPINFO BitmapInfo;
global_variable void* BitmapMemory;
global_variable HBITMAP BitmapHandle;
global_variable HDC BitmapDeviceContext;

/*
Bit map class inherit from Image class, procides methods for loading and saving vector images,
creating and manipulating raster images.
*/
internal void Win32ResizeDIBSection(int width ,int height)
{
   
    if (BitmapHandle)
    {
        DeleteObject(BitmapHandle);
    }
    if(!BitmapDeviceContext)
    {
        BitmapDeviceContext = CreateCompatibleDC(0);
    }

    BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
    BitmapInfo.bmiHeader.biWidth = width;
    BitmapInfo.bmiHeader.biHeight = height;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 32;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;

    BitmapHandle = CreateDIBSection(
        BitmapDeviceContext, &BitmapInfo,
        DIB_RGB_COLORS,
        &BitmapMemory,
        0, 0);
    //ReleaseDC(0, BitmapDeviceContext);
}

internal void Win32UpdateWindow(HDC DeveiceContex,int X, int Y, int Width, int Height)
{
    StretchDIBits(DeveiceContex,
        X,Y,Width,Height,
        X,Y,Width,Height,
        BitmapMemory,
        &BitmapInfo,
        DIB_RGB_COLORS, SRCCOPY);
}

LRESULT CALLBACK
Win32MainWindowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;
    switch (message) {

    case WM_SIZE: {
        RECT ClientRect;
        GetClientRect(window, &ClientRect);
        int Height = ClientRect.bottom - ClientRect.top;
        int Width = ClientRect.right - ClientRect.left;
        Win32ResizeDIBSection(Width,Height);
        //OutputDebugStringA("WM_SIZE\n");
    } break;

    case WM_SETCURSOR: {
        
    } break;

    case WM_ACTIVATEAPP: {
        //OutputDebugStringA("WM_ACTIVATEAPP\n");
    } break;

    case WM_CLOSE: {
        Running = false;
        //OutputDebugStringA("WM_CLOSE\n");
    } break;

    case WM_DESTROY: {
        Running = false;
        //DestroyWindow(window);
        //OutputDebugStringA("WM_DESTROY\n");
    } break;

    case WM_SYSKEYUP:
    case WM_SYSKEYDOWN:
    case WM_KEYUP:
    case WM_KEYDOWN: {
        /*Assert(!"no keyboard message here")*/
    } break;

    case WM_PAINT: {
        PAINTSTRUCT Paint = {};
          HDC deviceContext = BeginPaint(window, &Paint);
          int x = Paint.rcPaint.left;
          int y = Paint.rcPaint.top;
          int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
          int Width = Paint.rcPaint.right - Paint.rcPaint.left;
          //global_variable DWORD Operation = WHITENESS;
          //PatBlt(deviceContext, x, y, Width, Height, WHITENESS);
          Win32UpdateWindow(deviceContext, x, y, Width, Height);
          EndPaint(window, &Paint);
    } break;

    default: {
        result = DefWindowProcA(window, message, wParam, lParam);
    } break;
    }

    return result;
}

int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nShowCmd
)
{
    WNDCLASS WindowClass = {};
    WindowClass.style = CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpfnWndProc = Win32MainWindowCallback;
    WindowClass.hInstance = hInstance;
    WindowClass.hCursor = LoadCursor(0, IDC_ARROW);
    WindowClass.lpszClassName = (LPCWSTR)"KawaiiWindowClass";

    if (RegisterClass(&WindowClass)) {
        HWND window = CreateWindowEx(
            0,
            (LPCWSTR)WindowClass.lpszClassName,
            (LPCWSTR)"Kawaii desune",
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            hInstance,
            0);

        if (window)
        {
            Running = true;
            while(Running)
            {
                MSG Message;
                BOOL MessageResult = GetMessage(&Message, 0, 0, 0);
                if (MessageResult > 0)
                {
                    TranslateMessage(&Message);
                    DispatchMessageA(&Message);
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            //TODO Logging
        }
    }
    else
    {
        //logging
    }
    return 0;
}
