// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include "Il2cppLib.h"

#pragma comment( lib, "Il2cppLib" )

//#ifdef NDEBUG
//#pragma comment( lib, "Il2cppLib" )
//#elif _DEBUG
//#pragma comment( lib, "Il2cppLib_Debug" )
//#endif

#include "UnityEngine/UnityEngine.hpp"
#include "rust.hpp"

void mainThread(HMODULE hModule);

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        Il2cppLib::initialize();

        if (auto handle = CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(mainThread), hModule, 0, 0))
            CloseHandle(handle);
        break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}

void mainThread(HMODULE hModule)
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    std::cout << "[+] hello!" << std::endl;

    bool loaded = false;
    while (!GetAsyncKeyState(VK_DELETE)) {
        if (!loaded) {
            auto bundle = UnityEngine::AssetBundle::LoadFromFile(Il2cppLib::api::new_string<System::String*>("C:\\Users\\reality\\Desktop\\monke.bundle"));
            std::cout << "[+] Loaded AssetBundle" << std::endl;
            loaded = true;
        }
        if (GetAsyncKeyState(VK_F8) & 0x0001)
        {
            std::cout << " -> F8" << std::endl;
            loaded = false;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    FreeLibraryAndExitThread(hModule, 0);
    FreeConsole();
}
