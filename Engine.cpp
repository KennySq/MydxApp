#include"framework.h"
#include "Engine.h"

Engine::Engine(HWND hwnd, HINSTANCE hInstance) : mHardware(HW::GetInstance(hwnd, hInstance))
{}

Engine::~Engine()
{
	delete mHardware;
}

void Engine::Init()
{
	mSwapChain = HW::GetSwapChain();

	auto texDesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R32G32B32A32_FLOAT, mHardware->GetWidth(), mHardware->GetHeight(), 1U, 0U, D3D11_BIND_RENDER_TARGET);
	Tex2D tex(texDesc);

	bool texResult = tex.Generate();


}

void Engine::Update(float Delta)
{}

void Engine::Render(float Delta)
{


}

void Engine::Release()
{
	
}
