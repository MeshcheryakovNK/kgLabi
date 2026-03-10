#include "App.h"
#include <windows.h>

int WINAPI wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int cmdShow)
{
    Application application;

    if (!application.Init(hInst, cmdShow))
        return 0;

    return application.MainLoop();
}