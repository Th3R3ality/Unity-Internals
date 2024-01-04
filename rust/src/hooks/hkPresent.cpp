#include "hkPresent.h"
#include "global.h"

#include <iostream>
#include <format>
#include <d3d11.h>

#include "Lapis/engine/LapisEngine.h"

#include "cache.hpp"

HWND g_hwnd{};

bool ShowMenu = false;
bool doDebugPrint = false;

uintptr_t origPresent{ 0 };
HRESULT hkPresent(IDXGISwapChain* _this, UINT SyncInterval, UINT Flags)
{
	static int presentCalls = -1;
	presentCalls += 1;

	//doDebugPrint = (presentCalls % 60 == 0);

	if (ejecting) {
		Lapis::CleanLapis();
		if (origWndProcHandler != nullptr)
			(WNDPROC)SetWindowLongPtr(g_hwnd, GWLP_WNDPROC, (LONG_PTR)origWndProcHandler);

		presentReset = true;
		return ((HRESULT(*)(IDXGISwapChain*, UINT, UINT))origPresent)(_this, SyncInterval, Flags);
	}

	static bool init = false;
	if (!init) {
		std::cout << "initting from hkPresent\n";

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		_this->GetDesc(&swapChainDesc);
		g_hwnd = swapChainDesc.OutputWindow; // the g_hwnd
		std::cout << "g_hwnd : " << g_hwnd << "\n";

		Lapis::InitLapisInternal(_this);

		//Set OriginalWndProcHandler to the Address of the Original WndProc function
		origWndProcHandler = (WNDPROC)SetWindowLongPtr(g_hwnd, GWLP_WNDPROC, (LONG_PTR)hWndProc);

		init = true;
		std::cout << "initted\n";
	}


	// lapis render
	{
		using namespace Lapis;
		Lapis::NewFrame();

		BasePlayer* lp = cache::local();
		UnityEngine::Camera* mainCam = cache::cameraMain();

		if (lp && mainCam) {
			Vec3 lpPos = cache::local()->transform()->position();

			std::cout << "Camera Angles : " << mainCam->transform()->eulerAngles() << "\n";

			auto viewMat = mainCam->worldToCameraMatrix();
			auto projMat = mainCam->nonJitteredProjectionMatrix();

			/*
				TRY THIS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

				Camera cam = Camera.main;
				Matrix4x4 P = GL.GetGPUProjectionMatrix(cam.projectionMatrix, false);
				Matrix4x4 V = cam.worldToCameraMatrix;
				Matrix4x4 M = renderer.localToWorldMatrix;
 
				Matrix4x4 MVP = P * V * M;
 
			*/



			//std::cout << "viewMat : " << static_cast<mat4x4>(viewMat) << "\n";
			//std::cout << "projMat : " << static_cast<mat4x4>(projMat) << "\n";

			//Lapis::PushViewMatrix(viewMat);
			//Lapis::PushProjectionMatrix(projMat);

			//std::cout << "LocalPlayer Pos : " << lpPos << "\n";
			//std::cout << "Main Camera Pos : " << mainCam->transform()->position() << "\n";

			//Lapis::mainCamera.pos = mainCam->transform()->position();
			//Lapis::mainCamera.rot = mainCam->transform()->eulerAngles();

			Lapis::Draw::D3::Cube(Transform( 0, 0, 1), "#FEE75C99");
		}

		//Lapis::mainCamera.pos = lpPos + -Vec3::forward * 3;


		Draw::D2::Triangle(0, { 50,0 }, {0,50}, "#ED424599");

		Lapis::RenderFrame();
		Lapis::FlushFrame();
	}

	return ((HRESULT(*)(IDXGISwapChain*, UINT, UINT))origPresent)(_this, SyncInterval, Flags);
}

WNDPROC origWndProcHandler{};
LRESULT CALLBACK hWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Lapis::WndProcHandler(hWnd, uMsg, wParam, lParam);

	return CallWindowProc(origWndProcHandler, hWnd, uMsg, wParam, lParam);
}