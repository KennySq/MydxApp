#include"framework.h"
#include "Engine.h"
#include"EnvironmentalVariables.h"

using namespace Mydx;
using namespace std;
Engine::Engine(HWND hwnd, HINSTANCE hInstance) : mHardware(HW::GetInstance(hwnd, hInstance))
{
	scene = new Scene("Main Scene");
	mSwapChain = HW::GetSwapChain();
}

Engine::~Engine()
{
	delete mHardware;
}

void Engine::PreInit()
{
	Renderer2D& r2d = Renderer2D::GetInstance();
	Hardware* hwInst = HW::GetInstance();

	D3D11_TEXTURE2D_DESC chainTextureDesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R8G8B8A8_UNORM, hwInst->GetWidth(), hwInst->GetHeight(), 1U, 0U, D3D11_BIND_RENDER_TARGET);
	D3D11_TEXTURE2D_DESC chainDepthDesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R24G8_TYPELESS, hwInst->GetWidth(), hwInst->GetHeight(), 1U, 0U, D3D11_BIND_DEPTH_STENCIL);
	
	Tex2D* chainTexture = new Tex2D(chainTextureDesc, true);
	Tex2D* chainDepth = new Tex2D(chainDepthDesc);

	r2d.AddTexture2D(chainTexture);
	r2d.AddTexture2D(chainDepth);

}

void Engine::Init()
{

}

void Engine::Update(float delta)
{
	Renderer2D& r2d = Renderer2D::GetInstance();
	
	Tex2D* chainTex = r2d.GetTexture2D(0);
	Tex2D* chainDepth = r2d.GetTexture2D(1);

	chainTex->ClearRenderTarget(Colors::OrangeRed);
	chainDepth->ClearDepthStencil();

	
	
}

void Engine::Render(float delta)
{
	mSwapChain->Present(0, 0);

}

void Engine::Release()
{
	
}
