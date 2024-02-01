#include "hkPresent.h"
#include "global.h"

#include <iostream>
#include <format>
#include <d3d11.h>

#include "Lapis/engine/LapisEngine.h"
#include "Lapis/engine/Helpers.h"

#include "cache.hpp"

#include "UnityEngine/GL/GL.hpp"

#include "rust/Render/Render.h"

#include "rust/classes/BuildingManager/BuildingManager.hpp"
#include "rust/classes/BuildingBlock/BuildingBlock.hpp"

#include "sex.h"
bool sexbool = false;



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
		NewFrame();
		Draw::D2::Point(0, 0); // fixes weird error with shaders
		Draw::D2::Triangle(0, { 50,0 }, {0,50}, "FF005099");


		UnityEngine::Camera* mainCam = UnityEngine::Camera::main();

		if (mainCam) {


			// Get and Fix Matrices
			{
				auto viewMat = mainCam->worldToCameraMatrix();
				Lapis::PushViewMatrix(viewMat);

				constexpr bool renderIntoTexture = false;
				UnityEngine::Matrix4x4 projMat = UnityEngine::GL::GetGPUProjectionMatrix(mainCam->nonJitteredProjectionMatrix(), renderIntoTexture);
				//UnityEngine::Matrix4x4 projMat = UnityEngine::GL::GetGPUProjectionMatrix(mainCam->projectionMatrix(), renderIntoTexture);
				if (renderIntoTexture) {
					for (int i = 0; i < 4; i++) {
						projMat.m[i][1] = -projMat.m[i][1];
					}
				}

				// Invert, Scale and Bias from OpenGL -> D3D depth range (zF,zN)(0,1 -> 1,-1)
				for (int i = 0; i < 4; i++) {
					projMat.m[i][2] = projMat.m[i][2] * -1;
					projMat.m[i][2] = (projMat.m[i][2] * 0.5f + projMat.m[i][3] * 0.5f);
				}

				Lapis::PushProjectionMatrix(projMat);
			}


			Render::Players();


			auto drawCalls = cache::debugDrawables();
			for (auto& drawCall : drawCalls) {
				switch (drawCall.second.shape) {
                case Lapis::Shape::Icosahedron:
					Lapis::Draw::D3::Icosahedron(drawCall.second.transform, drawCall.second.color);
					break;
				case Lapis::Shape::Cube:
					Lapis::Draw::D3::Cube(drawCall.second.transform, drawCall.second.color);
					break;
				case Lapis::Shape::Line3d:
					Lapis::Draw::D3::Line(drawCall.second.from, drawCall.second.to, drawCall.second.color);
					break;
				default:
					std::cout << "no shape\n";
					break;
				}
			}

			Render::Building();

		}


		if (GetAsyncKeyState('O') & 0x1) {
			//sexbool = !sexbool;
		}
		if (sexbool) {
			Draw::D2::Circle(Vec2(30,30), 10, "fff33399");
		}

		RenderFrame();
		FlushFrame();
	}

	return ((HRESULT(*)(IDXGISwapChain*, UINT, UINT))origPresent)(_this, SyncInterval, Flags);
}

WNDPROC origWndProcHandler{};
LRESULT CALLBACK hWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Lapis::WndProcHandler(hWnd, uMsg, wParam, lParam);

	return CallWindowProc(origWndProcHandler, hWnd, uMsg, wParam, lParam);
}