// dllmain.cpp : Defines the entry point for the DLL application.
#include <windows.h>
#include <iostream>
#include <chrono>
#include <thread>

#include "Il2cppLib.h"
#pragma comment( lib, "Il2cppLib" )

#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <d3dcompiler.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include "cheat.hpp"


#include "global.h"
bool ejecting;
bool presentReset;
//bool doDebugPrint;


//#ifdef NDEBUG
//#pragma comment( lib, "Il2cppLib" )
//#elif _DEBUG
//#pragma comment( lib, "Il2cppLib_Debug" )
//#endif

#include "UnityEngine/UnityEngine.hpp"
#include "mscorlib/mscorlib.hpp"
#include "rust/rust.hpp"

bool init_Il2cppLib = false;
bool cheat_load_assets = false;
bool cheat_init_hooks = false;
bool console_present = false;
void mainThread(HMODULE hModule);
void eject(HMODULE hModule);

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        if (auto handle = CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(mainThread), hModule, 0, 0))
            CloseHandle(handle);
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}

void mainThread(HMODULE hModule)
{

    if (GetConsoleWindow()) {
        console_present = true;
    }
    if (!console_present) {
        AllocConsole();
        FILE* f;
        freopen_s(&f, "CONOUT$", "w", stdout);
    }
    std::cout << "[+] hello!" << std::endl;

    cheat::state(cheat::status::loading);

    std::cout << "init | Il2cppLib\n";
    if (!Il2cppLib::initialize()) {
        std::cout << "---FAILED---\n\n";
        return eject(hModule);
    } init_Il2cppLib = true;

    std::cout << "init | hooks \n";
    if (!cheat::init_hooks()) {
        std::cout << "---FAILED---\n\n";
        return eject(hModule);
    } cheat_init_hooks = true;

    cheat::state(cheat::status::running);

    while (!GetAsyncKeyState(VK_DELETE)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    cheat::unload();
    
    eject(hModule);
}

void eject(HMODULE hModule)
{
    if (!console_present) {
        FreeConsole();
    }
    FreeLibraryAndExitThread(hModule, 0);
}