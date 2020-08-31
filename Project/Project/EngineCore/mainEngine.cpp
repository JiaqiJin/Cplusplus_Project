#include <windows.h>
#include <stdint.h>
#include <xinput.h>
#include <dsound.h>
#include<stdio.h>
#include <math.h>
#include<malloc.h>
#include "handmade.h"
#include "handmade.cpp"
#include "win32_handmade.h"

// XInputGetState
#define XINPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE* pState)
typedef XINPUT_GET_STATE(x_input_get_state);
XINPUT_GET_STATE(XInputGetStateStub) {
    return ERROR_DEVICE_NOT_CONNECTED;
}
global_variable x_input_get_state* XInputGetState_ = XInputGetStateStub;

// XInputSetState
#define XINPUT_SET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration)
typedef XINPUT_SET_STATE(x_input_set_state);
XINPUT_SET_STATE(XInputSetStateStub) {
    return ERROR_DEVICE_NOT_CONNECTED;
}
global_variable x_input_set_state* XInputSetState_ = XInputSetStateStub;

#define XInputGetState XInputGetState_
#define XInputSetState XInputSetState_

#define DIRECT_SOUND_CREATE(name) HRESULT WINAPI name(LPCGUID pcGuidDevice, LPDIRECTSOUND *ppDS, LPUNKNOWN pUnkOuter);
typedef DIRECT_SOUND_CREATE(direct_sound_create);

global_variable bool GlobalRunning = true;
global_variable win32_offscreen_buffer GlobalBackBuffer;
global_variable LPDIRECTSOUNDBUFFER GlobalSecondaryBuffer;

debug_read_file_result
DebugPlatformReadFile(const char* FileName)
{
    debug_read_file_result Result = {};
    HANDLE FileHandle = CreateFileA(
        FileName,
        GENERIC_READ,
        0,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        0
    );

    LARGE_INTEGER FileSize;

    if (FileHandle != INVALID_HANDLE_VALUE)
    {
        if (GetFileSizeEx(FileHandle, &FileSize))
        {
            uint32 FileSize32 = SafeTruncateUInt64(FileSize.QuadPart);
            void* Memory = VirtualAlloc(0, FileSize32, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
            if (Memory) 
            {
                DWORD BytesRead;
                if (ReadFile(FileHandle, Memory, FileSize32, &BytesRead, 0) && BytesRead == FileSize32) 
                {
                    Result.Size = FileSize32;
                    Result.Memory = Memory;
                }
                else 
                {
                    // TODO: logging
                    DebugPlatformFreeFileMemory(Result.Memory);
                }
            }
            else 
            {
                // TODO: logging
            }
        }
        else 
        {
            // TODO: logging
        }

        CloseHandle(FileHandle);
    }
    else 
    {
        // TODO: logging
    }

    return Result;
}


bool32 DebugPlatformWriteFile(const char* FileName, void* Memory, uint32 Size)
{
    bool32 Result = false;
    HANDLE File = CreateFileA(
        FileName,
        GENERIC_WRITE,
        0,
        0,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        0);

    if (File != INVALID_HANDLE_VALUE) 
    {
        DWORD BytesWritten;
        if (WriteFile(File, Memory, Size, &BytesWritten, 0)) {
            Result = (BytesWritten == Size);
        }
        else 
        {
            // TODO: logging
        }

        CloseHandle(File);
    }
    else 
    {
        // TODO: logging
    }

    return Result;
}


void DebugPlatformFreeFileMemory(void* Memory)
{
    if(Memory)
    {
        VirtualFree(Memory, 0, MEM_RELEASE);
    }
}

internal void
Win32InitDSound(HWND Window, int32 SamplesPerSecond, int32 BufferSize)
{
    HMODULE Library = LoadLibraryA("dsound.dll");
    if (Library)
    {
        direct_sound_create* DirectSoundCreate = (direct_sound_create*)GetProcAddress(Library, "DirectSoundCreate");

        LPDIRECTSOUND DirectSound;

        if (SUCCEEDED(DirectSoundCreate(0, &DirectSound, 0)))
        {
            if (SUCCEEDED(DirectSound->SetCooperativeLevel(Window, DSSCL_PRIORITY)))
            {
                OutputDebugStringA("Set cooperative level ok\n");
            }
            else
            {
                // TODO: logging
            }

            WAVEFORMATEX WaveFormat = {};
            WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
            WaveFormat.nChannels = 2;
            WaveFormat.nSamplesPerSec = SamplesPerSecond;
            WaveFormat.wBitsPerSample = 16;
            WaveFormat.nBlockAlign = WaveFormat.nChannels * WaveFormat.wBitsPerSample / 8;
            WaveFormat.nAvgBytesPerSec = WaveFormat.nSamplesPerSec * WaveFormat.nBlockAlign;

            {
                DSBUFFERDESC BufferDesc = {};
                BufferDesc.dwSize = sizeof(BufferDesc);
                BufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
                LPDIRECTSOUNDBUFFER  PrimaryBuffer;
                if (SUCCEEDED(DirectSound->CreateSoundBuffer(&BufferDesc, &PrimaryBuffer, 0))) {
                    OutputDebugStringA("Create primary buffer ok\n");
                    if (SUCCEEDED(PrimaryBuffer->SetFormat(&WaveFormat))) {
                        OutputDebugStringA("Primary buffer set format ok\n");
                    }
                    else {
                        // TDOO: logging
                    }
                }
            }

            {
                DSBUFFERDESC BufferDesc = {};
                BufferDesc.dwSize = sizeof(BufferDesc);
                BufferDesc.dwFlags = 0;
                BufferDesc.dwBufferBytes = BufferSize;
                BufferDesc.lpwfxFormat = &WaveFormat;
                if (SUCCEEDED(DirectSound->CreateSoundBuffer(&BufferDesc, &GlobalSecondaryBuffer, 0))) {
                    OutputDebugStringA("Secondary buffer created\n");
                }
                else {
                    // TODO: logging
                }
            }

        }
        else {
            // TODO: logging
        }
    }
    else {
        // TODO: logging
    }
}

internal void
Win32ClearSoundBuffer(win32_sound_output* SoundOutput)
{
    VOID* Region1;
    DWORD Region1Size;
    VOID* Region2;
    DWORD Region2Size;

    if (SUCCEEDED(GlobalSecondaryBuffer->Lock(0,
        SoundOutput->SecondaryBufferSize,
        &Region1, &Region1Size,
        &Region2, &Region2Size,
        0))) 
    {
        DWORD Region1SampleCount = Region1Size / SoundOutput->BytesPerSample;
        int16_t* SampleOut = (int16_t*)Region1;
        for (DWORD SampleIndex = 0; SampleIndex < Region1SampleCount; SampleIndex++) 
        {
            *SampleOut++ = 0;
            *SampleOut++ = 0;
            ++SoundOutput->RunningSampleIndex;
        }
        DWORD Region2SampleCount = Region2Size / SoundOutput->BytesPerSample;
        SampleOut = (int16_t*)Region2;
        for (DWORD SampleIndex = 0; SampleIndex < Region2SampleCount; SampleIndex++) 
        {
            *SampleOut++ = 0;
            *SampleOut++ = 0;
            ++SoundOutput->RunningSampleIndex;
        }
        GlobalSecondaryBuffer->Unlock(Region1, Region1Size, Region2, Region2Size);
    }
}

internal void
Win32FillSoundBuffer(win32_sound_output* SoundOutput, DWORD LockOffset, DWORD BytesToLock,
                       game_sound_output_buffer* SourceBuffer)
{
    void* Region1;
    DWORD Region1Size;
    void* Region2;
    DWORD Region2Size;

    if (SUCCEEDED(GlobalSecondaryBuffer->Lock(
        LockOffset,
        BytesToLock,
        &Region1, &Region1Size,
        &Region2, &Region2Size,
        0
    )))
    {
        DWORD Region1SampleCount = Region1Size / SoundOutput->BytesPerSample;
        int16_t* SampleOut = (int16_t*)Region1;
        int16_t* SourceSample = SourceBuffer->Samples;
        for (DWORD SampleIndex = 0; SampleIndex < Region1SampleCount; SampleIndex++) {
            *SampleOut++ = *SourceSample++;
            ++SoundOutput->RunningSampleIndex;
        }

        DWORD Region2SampleCount = Region2Size / SoundOutput->BytesPerSample;
        SampleOut = (int16_t*)Region2;
        for (DWORD SampleIndex = 0; SampleIndex < Region2SampleCount; SampleIndex++) {
            *SampleOut++ = *SourceSample++;
            ++SoundOutput->RunningSampleIndex;
        }

        GlobalSecondaryBuffer->Unlock(Region1, Region1Size, Region2, Region2Size);
    }
}

internal void
Win32ProcessButtonState(
    game_button_state* NewButtonState,
    game_button_state* OldButtonState,
    DWORD Buttons,
    DWORD ButtonBit
) {
    NewButtonState->HalfTransitionCount = OldButtonState->IsEndedDown == NewButtonState->IsEndedDown ? 0 : 1;
    NewButtonState->IsEndedDown = Buttons & ButtonBit;
}

internal void
Win32LoadXInput()
{
    HMODULE Library = LoadLibraryA("xinput1_4.dll");
    if (Library) {
        XInputGetState = (x_input_get_state*)GetProcAddress(Library, "XInputGetState");
        XInputSetState = (x_input_set_state*)GetProcAddress(Library, "XInputSetState");
    }
}

internal win32_window_dimension
Win32GetWindowDimension(HWND Window)
{
    win32_window_dimension Result;
    RECT ClientRect;
    GetClientRect(Window, &ClientRect);
    Result.Width = ClientRect.right - ClientRect.left;
    Result.Height = ClientRect.bottom - ClientRect.top;
    return Result;
}

internal void
Win32ResizeDIBSection(win32_offscreen_buffer* Buffer, int Width, int Height)
{
    if (Buffer->Memory) 
    {
        VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
    }
    Buffer->Width = Width;
    Buffer->Height = Height;

    Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
    Buffer->Info.bmiHeader.biWidth = Width;
    Buffer->Info.bmiHeader.biHeight = Height;
    Buffer->Info.bmiHeader.biPlanes = 1;
    Buffer->Info.bmiHeader.biBitCount = 32;
    Buffer->Info.bmiHeader.biCompression = BI_RGB;

    int BytesPerPixel = 4;
    int BitmapMemorysize = Width * Height * BytesPerPixel;
    Buffer->Memory = VirtualAlloc(0, BitmapMemorysize, MEM_COMMIT, PAGE_READWRITE);
    Buffer->Pitch = BytesPerPixel * Width;
}

internal void
Win32UpdateWindow(
    HDC DeviceContext,
    int WindowWidth,
    int WindowHeight,
    win32_offscreen_buffer* Buffer)
{
    StretchDIBits(
        DeviceContext,
        0, 0, WindowWidth, WindowHeight, // dest
        0, 0, Buffer->Width, Buffer->Height, // src
        Buffer->Memory,
        &Buffer->Info,
        DIB_RGB_COLORS,
        SRCCOPY
    );
}

internal void
Win32ProcessKeyboardMessage(game_button_state* ButtonState, bool IsDown)
{
    ButtonState->IsEndedDown = IsDown;
    ButtonState->HalfTransitionCount++;
}

internal void
Win32ProcessPendingMessages(game_controller_input* KeyboardController)
{
    MSG Message;
    while (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
    {
        switch (Message.message)
        {
        case WM_QUIT:
        {
            GlobalRunning = false;
        } break;

        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            uint32_t VKCode = (uint32_t)Message.wParam;
            bool WasDown = ((Message.lParam & (1 << 30)) != 0);
            bool IsDown = ((Message.lParam & (1 << 31)) == 0);
            if (WasDown != IsDown)
            {
                switch (VKCode) {
                case 'W':
                {
                } break;
                case 'A':
                {
                } break;
                case 'S':
                {
                } break;

                case 'D':
                {
                } break;
                case 'Q':
                {
                    Win32ProcessKeyboardMessage(&KeyboardController->LeftShoulder, IsDown);
                } break;
                case 'E':
                {
                    Win32ProcessKeyboardMessage(&KeyboardController->RightShoulder, IsDown);
                } break;
                case VK_UP:
                {
                    Win32ProcessKeyboardMessage(&KeyboardController->Up, IsDown);
                } break;
                case VK_DOWN:
                {
                    Win32ProcessKeyboardMessage(&KeyboardController->Down, IsDown);
                } break;
                case VK_LEFT:
                {
                    Win32ProcessKeyboardMessage(&KeyboardController->Left, IsDown);
                } break;
                case VK_RIGHT:
                {
                    Win32ProcessKeyboardMessage(&KeyboardController->Right, IsDown);
                } break;
                case VK_ESCAPE:
                {
                    GlobalRunning = false;
                } break;
                case VK_SPACE:
                {
                } break;
                }
            }
        } break;

        default:
        {
            TranslateMessage(&Message);
            DispatchMessage(&Message);
        } break;
        }
    }
}


internal LRESULT CALLBACK
Win32MainWindowCallback(
    HWND Window,
    UINT Message,
    WPARAM WParam,
    LPARAM LParam)
{
    LRESULT Result = 0;
    switch (Message)
    {
    case WM_DESTROY:
    {
        GlobalRunning = false;
    } break;

    case WM_CLOSE:
    {
        GlobalRunning = false;
    } break;

    case WM_ACTIVATEAPP:
    {
        //OutputDebugStringA("ACTIVATEAPP\n");
    } break;

    case WM_PAINT:
    {
        PAINTSTRUCT PaintStruct = {};
        HDC DeviceContext = BeginPaint(Window, &PaintStruct);
        win32_window_dimension Dimension = Win32GetWindowDimension(Window);
        Win32UpdateWindow(
            DeviceContext,
            Dimension.Width,
            Dimension.Height,
            &GlobalBackBuffer
        );
        EndPaint(Window, &PaintStruct);
    } break;
    default:
    {
        Result = DefWindowProcA(Window, Message, WParam, LParam);
    } break;
    }

    return Result;
}

int CALLBACK
WinMain(
    HINSTANCE Instance,
    HINSTANCE PrevInstance,
    LPSTR     CmdLine,
    int       ShowCmd)
{
    Win32LoadXInput();

    LARGE_INTEGER CounterPerSecond;
    QueryPerformanceFrequency(&CounterPerSecond);

    WNDCLASS WindowClass = {};

    // TODO: Check if we need these
    WindowClass.style = CS_HREDRAW | CS_VREDRAW;

    WindowClass.lpfnWndProc = Win32MainWindowCallback;
    WindowClass.hInstance = Instance;
    WindowClass.lpszClassName = L"KawaiiDesuneWindowClass";

    Win32ResizeDIBSection(&GlobalBackBuffer, 1280, 720);

    if (RegisterClass(&WindowClass))
    {
        HWND Window = CreateWindowEx(
            0,
            L"KawaiiDesuneWindowClass",
            L"Kawaii Desune",
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

        if (Window)
        {
            win32_sound_output SoundOutput = {};
            SoundOutput.SamplesPerSecond = 48000;
            SoundOutput.LatencySampleCount = SoundOutput.SamplesPerSecond / 20;
            SoundOutput.BytesPerSample = 4;
            SoundOutput.SecondaryBufferSize = SoundOutput.SamplesPerSecond * SoundOutput.BytesPerSample;
            SoundOutput.RunningSampleIndex = 0;

            //Poor with Virtual Alloc
            int16* Samples = (int16*)VirtualAlloc(0, SoundOutput.SecondaryBufferSize, 
                                                   MEM_RESERVE| MEM_COMMIT, PAGE_READWRITE);

            game_memory GameMemory = {};
            GameMemory.PermanentStorageSize = Megabytes(64);
            GameMemory.TransientStorageSize = Gigabytes((uint64)2);
#ifdef HANDMADE_INTERNAL
            LPVOID BaseAddress = (LPVOID)Terabytes(2);
#else
            LPVOID BaseAddress = 0;
#endif
            uint64 TotalSize = GameMemory.PermanentStorageSize + GameMemory.TransientStorageSize;
            GameMemory.PermanentStorage = VirtualAlloc(
                BaseAddress,
                (size_t)TotalSize,
                MEM_COMMIT | MEM_RESERVE,
                PAGE_READWRITE
            );
            GameMemory.TransientStorage = (uint8*)GameMemory.PermanentStorage + GameMemory.PermanentStorageSize;

            if (Samples && GameMemory.PermanentStorage && GameMemory.TransientStorage)
            {

                Win32InitDSound(Window, SoundOutput.SamplesPerSecond, SoundOutput.SecondaryBufferSize);
                Win32ClearSoundBuffer(&SoundOutput);
                GlobalSecondaryBuffer->Play(0, 0, DSBPLAY_LOOPING);

                LARGE_INTEGER LastCounter;
                QueryPerformanceCounter(&LastCounter);
                uint64 LastCycleCounter = __rdtsc();

                game_input Input[2] = {};
                game_input* OldInput = &Input[0];
                game_input* NewInput = &Input[1];

                while (GlobalRunning)
                {
                    //MSG Message = {};
                    game_controller_input* KeyboardController = &NewInput->Controllers[0];
                    *KeyboardController = {};

                    Win32ProcessPendingMessages(KeyboardController);

                    int MaxControllerCount = XUSER_MAX_COUNT;
                    if (MaxControllerCount > ArrayCount(NewInput->Controllers))
                    {
                        MaxControllerCount = ArrayCount(NewInput->Controllers);
                    }

                    for (DWORD ControllerIndex = 0; ControllerIndex < MaxControllerCount; ControllerIndex++)
                    {
                        game_controller_input* OldController = &OldInput->Controllers[ControllerIndex];
                        game_controller_input* NewController = &NewInput->Controllers[ControllerIndex];

                        XINPUT_STATE state;
                        ZeroMemory(&state, sizeof(XINPUT_STATE));
                        if (XInputGetState(ControllerIndex, &state) == ERROR_SUCCESS)
                        {
                            XINPUT_GAMEPAD* Pad = &state.Gamepad;

                            /*bool32 Up = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
                             bool32 Down = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
                             bool32 Left = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
                             bool32 Right = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
                             bool32 Start = (Pad->wButtons & XINPUT_GAMEPAD_START);
                             bool32 Back = (Pad->wButtons & XINPUT_GAMEPAD_BACK);
                             bool32 LeftShoulder = (Pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
                             bool32 RightShoulder = (Pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
                             bool32 AButton = (Pad->wButtons & XINPUT_GAMEPAD_A);
                             bool32 BButton = (Pad->wButtons & XINPUT_GAMEPAD_B);
                             bool32 XButton = (Pad->wButtons & XINPUT_GAMEPAD_X);
                             bool32 YButton = (Pad->wButtons & XINPUT_GAMEPAD_Y);*/
                            Win32ProcessButtonState(
                                &NewController->Down, &OldController->Down,
                                Pad->wButtons, XINPUT_GAMEPAD_A
                            );
                            Win32ProcessButtonState(
                                &NewController->Right, &OldController->Right,
                                Pad->wButtons, XINPUT_GAMEPAD_B
                            );
                            Win32ProcessButtonState(
                                &NewController->Left, &OldController->Left,
                                Pad->wButtons, XINPUT_GAMEPAD_X
                            );
                            Win32ProcessButtonState(
                                &NewController->Up, &OldController->Up,
                                Pad->wButtons, XINPUT_GAMEPAD_Y
                            );
                            Win32ProcessButtonState(
                                &NewController->LeftShoulder, &OldController->LeftShoulder,
                                Pad->wButtons, XINPUT_GAMEPAD_LEFT_SHOULDER
                            );
                            Win32ProcessButtonState(
                                &NewController->RightShoulder, &OldController->RightShoulder,
                                Pad->wButtons, XINPUT_GAMEPAD_RIGHT_SHOULDER
                            );
                            int16 StickX = Pad->sThumbLX;
                            int16 StickY = Pad->sThumbLY;
                            real32 X;
                            if (StickX > 0) {
                                X = StickX / 32767;
                            }
                            else {
                                X = StickX / 32768;
                            }

                            NewController->StartX = OldController->EndX;
                            NewController->EndX = NewController->MinX = NewController->MaxX = X;

                            real32 Y;
                            if (StickY > 0) {
                                Y = StickY / 32767;
                            }
                            else {
                                Y = StickY / 32768;
                            }
                            NewController->StartY = OldController->EndY;
                            NewController->EndY = NewController->MinY = NewController->MaxY = Y;

                        }
                        else
                        {
                            // Controller is not connected
                        }
                    }

                    DWORD PlayCursor;
                    DWORD WriteCursor;
                    DWORD BytesToWrite;
                    DWORD ByteToLock;
                    DWORD TargetCursor;
                    bool32 SoundIsValid = false;
                    if (SUCCEEDED(GlobalSecondaryBuffer->GetCurrentPosition(&PlayCursor, &WriteCursor)))
                    {
                        ByteToLock = ((SoundOutput.RunningSampleIndex * SoundOutput.BytesPerSample)
                            % SoundOutput.SecondaryBufferSize);
                        TargetCursor = ((PlayCursor + (SoundOutput.LatencySampleCount * SoundOutput.BytesPerSample)) %
                            SoundOutput.SecondaryBufferSize);
                        if (ByteToLock > TargetCursor)
                        {
                            BytesToWrite = (SoundOutput.SecondaryBufferSize - ByteToLock);
                            BytesToWrite += TargetCursor;
                        }
                        else
                        {
                            BytesToWrite = TargetCursor - ByteToLock;
                        }
                        SoundIsValid = true;
                    }

                    game_offscreen_buffer Buffer = {};
                    Buffer.Memory = GlobalBackBuffer.Memory;
                    Buffer.Pitch = GlobalBackBuffer.Pitch;
                    Buffer.Width = GlobalBackBuffer.Width;
                    Buffer.Height = GlobalBackBuffer.Height;

                    game_sound_output_buffer SoundBuffer = {};
                    SoundBuffer.SamplePerSecond = SoundOutput.SamplesPerSecond;
                    SoundBuffer.SampleCount = BytesToWrite / SoundOutput.BytesPerSample;
                    SoundBuffer.Samples = Samples;

                    GameUpdateAndRender(&GameMemory, NewInput, &Buffer, &SoundBuffer);

                    if (SoundIsValid)
                    {
                        Win32FillSoundBuffer(&SoundOutput, ByteToLock, BytesToWrite, &SoundBuffer);
                    }

                    HDC DeviceContext = GetDC(Window);
                    win32_window_dimension Dimension = Win32GetWindowDimension(Window);
                    Win32UpdateWindow(DeviceContext, Dimension.Width, Dimension.Height, &GlobalBackBuffer);
                    ReleaseDC(Window, DeviceContext);

                    uint64 CurrentCycleCounter = __rdtsc();
                    LARGE_INTEGER CurrentCounter;
                    QueryPerformanceCounter(&CurrentCounter);

                    int64 CounterElapsed = CurrentCounter.QuadPart - LastCounter.QuadPart;
                    uint64 CycleElapsed = CurrentCycleCounter - LastCycleCounter;

                    real32 MSPerFrame = 1000.0f * (real32)CounterElapsed / (real32)CounterPerSecond.QuadPart;
                    real32 FPS = (real32)CounterPerSecond.QuadPart / (real32)CounterElapsed;
                    real32 MCPF = (real32)CycleElapsed / (1000.0f * 1000.0f);

                    char OutputBuffer[256];
                    sprintf_s(OutputBuffer, "ms/f: %.2f,  fps: %.2f,  mc/f: %.2f\n", MSPerFrame, FPS, MCPF);
                    OutputDebugStringA(OutputBuffer);

                    LastCounter = CurrentCounter;
                    LastCycleCounter = CurrentCycleCounter;
                    //swap
                    game_input* Tmp = OldInput;
                    OldInput = NewInput;
                    NewInput = OldInput;

                }
            }
            else
            {

            }
        }
        else
        {
            // TODO: Logging
        }
    }
    else
    {
        // TDOO: Logging
    }

    return 0;
}