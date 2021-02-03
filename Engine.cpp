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

	auto sctexDesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R8G8B8A8_UNORM, mHardware->GetWidth(), mHardware->GetHeight(), 1U, 0U, D3D11_BIND_RENDER_TARGET);
	scTex = new SCTex2D(sctexDesc);
	
	texResult = scTex->Generate();
	Pass p = Pass("SampleShader.hlsl", "Sample", eVertex);
	p.Generate();
	
	
	
}

void Engine::Update(float Delta)
{
	scTex->ClearTexture(DirectX::Colors::OrangeRed);
	

}

void Engine::Render(float Delta)
{
	mSwapChain->Present(0, 0);

}

void Engine::Release()
{
	
}
