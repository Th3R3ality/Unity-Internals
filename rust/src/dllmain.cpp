// dllmain.cpp : Defines the entry point for the DLL application.
#include <windows.h>
#include <iostream>
#include <chrono>

#include "Il2cppLib.h"
#pragma comment( lib, "Il2cppLib" )

#include "cheat.hpp"


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

void mainThread(HMODULE hModule);

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        AllocConsole();
        FILE* f;
        freopen_s(&f, "CONOUT$", "w", stdout);
        std::cout << "[+] hello!" << std::endl;

        std::cout << "init | Il2cppLib\n";
        if (!Il2cppLib::initialize()) {
            std::cout << "\n---FAILED---\n\n";
            break;
        }
        init_Il2cppLib = true;

        /*if (!cheat::load_assets()) {
            break;
        }
        cheat_load_assets = true;
        */

        std::cout << "init | hooks \n";
        if (!cheat::init_hooks()) {
            std::cout << "\n---FAILED---\n\n";
            break;
        }
        cheat_init_hooks = true;

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
    bool loaded = false;
    while (!GetAsyncKeyState(VK_DELETE)) {
        /*
        auto names = cheat::get_asset_names();
            
        for (int idx = 0; idx < names->length(); idx++) {
            auto name = names->data()[idx];
            std::wcout << L"[=] Asset Name: " << name->buffer << std::endl;
        }
        */
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    cheat::unload();
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
}
