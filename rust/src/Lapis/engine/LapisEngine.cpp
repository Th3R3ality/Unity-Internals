#include "LapisEngine.h"
#include <iostream>
#include "Backend.h"

namespace Lapis
{

	float deltaTime;
	float elapsedTime;
	Transform mainCamera;


	void InitLapisInternal(IDXGISwapChain* swapchain)
	{
		std::cout << "Initting via SwapChain : " << swapchain << "\n";
		Backend::InitBackendInternal(swapchain);
	}

	void WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Backend::WndProcHandler(hwnd, msg, wParam, lParam);
	}

	void NewFrame()
	{
		Backend::NewFrame();
	}
	void RenderFrame()
	{
		Backend::RenderFrame();
	}
	void FlushFrame()
	{
		Backend::FlushFrame();
	}
	void CleanLapis()
	{
		Backend::CleanD3D11();
	}

	void DestroyViews()
	{
		Backend::DestroyViews();
	}
	void CreateViews(IDXGISwapChain* swapchain)
	{
		Backend::CreateViews(swapchain);
	}

	void PushWorldMatrix(mat4x4 mat)
	{
		Backend::PushWorldMatrix(mat);
	}

	void PushViewMatrix(mat4x4 mat)
	{
		Backend::PushViewMatrix(mat);
	}

	void PushProjectionMatrix(mat4x4 mat)
	{
		Backend::PushProjectionMatrix(mat);
	}
}