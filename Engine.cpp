#include"framework.h"
#include "Engine.h"

using namespace Mydx;

Engine::Engine(HWND hwnd, HINSTANCE hInstance) : mHardware(HW::GetInstance(hwnd, hInstance))
{}

Engine::~Engine()
{
	delete mHardware;
}

void Engine::Init()
{
	mSwapChain = HW::GetSwapChain();


	auto scDesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R8G8B8A8_UNORM, mHardware->GetWidth(), mHardware->GetHeight(), 1U, 0U, D3D11_BIND_RENDER_TARGET);
	auto depthDesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R24G8_TYPELESS, mHardware->GetWidth(), mHardware->GetHeight(), 1U, 0U, D3D11_BIND_DEPTH_STENCIL);

	SCTex2D* scTex = new SCTex2D(scDesc);
	Tex2D* depthTex = new Tex2D(depthDesc);

	Pass p = Pass("SampleShader.hlsl", "Sample", VERTEX);
	
	shared_ptr<Mesh> mesh = PrimitiveGenerator::GenerateSphere(1.0f, 32, 32);
	
	Renderer3D& r3d = Renderer3D::GetInstance();
	Renderer2D& r2d = Renderer2D::GetInstance();

	r3d.DrawMesh(*mesh, p);

	r2d.AddSwapChainTexture2D(scTex);
	r2d.AddTexture2D(depthTex);

	Mydx::Camera c = Camera::GetMainCamera();
	
	Instance inst;
	inst.AddComponent<MeshRenderer>();
	
	MeshRenderer* mr = inst.GetComponent<MeshRenderer>();
	mr->SetMesh(&*mesh);
//	mr->DrawForward(scTex, depthTex, )

}

void Engine::Update(float delta)
{
	Renderer3D& r3d = Renderer3D::GetInstance();
	Renderer2D& r2d = Renderer2D::GetInstance();

	SCTex2D* scTex = r2d.GetSwapChainTexture2D(0);
	scTex->ClearTexture(DirectX::Colors::OrangeRed);
}

void Engine::Render(float delta)
{
	mSwapChain->Present(0, 0);

}

void Engine::Release()
{
	
}
