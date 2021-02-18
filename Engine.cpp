#include"framework.h"
#include "Engine.h"

using namespace Mydx;

Engine::Engine(HWND hwnd, HINSTANCE hInstance) : mHardware(HW::GetInstance(hwnd, hInstance))
{
	scene = new Scene("Main Scene");
}

Engine::~Engine()
{
	delete mHardware;
}

void Engine::PreInit()
{
	Renderer2D& r2d = Renderer2D::GetInstance();

	auto scDesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R8G8B8A8_UNORM, mHardware->GetWidth(), mHardware->GetHeight(), 1U, 0U, D3D11_BIND_RENDER_TARGET);
	auto depthDesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R24G8_TYPELESS, mHardware->GetWidth(), mHardware->GetHeight(), 1U, 0U, D3D11_BIND_DEPTH_STENCIL);

	Tex2D* scTex = new Tex2D(scDesc, true);
	Tex2D* depthTex = new Tex2D(depthDesc);

	r2d.AddTexture2D(scTex);
	r2d.AddTexture2D(depthTex);

	scene->Init();
}

void Engine::Init()
{
	AllocConsole();

	mSwapChain = HW::GetSwapChain();

	Pass* p = new Pass("C:/Users/Leopard/source/repos/Mydx/StandardForwardShader.hlsl", "Sample", VERTEX | PIXEL, FORWARD);
	Mesh* mesh = new Mesh(*PrimitiveGenerator::GenerateSphere(1.0f, 32, 32));
	Camera* mainCam = scene->GetCamera();

	Instance* inst = new Instance();
	inst->AddComponent<MeshRenderer>();
	Transform* transform = inst->GetTransform();


	scene->AddInstance(inst);
	scene->Init();
	
	MeshRenderer* mr = inst->GetComponent<MeshRenderer>();
	
	mr->SetMesh(&*mesh);
	mr->SetPass(p);
	RenderState& rs = mr->GetState();

	XMVECTOR lightDir = XMVectorSet(-1, 0, 0, 1) - XMVectorSet(0, 0, 0, 0);
	XMFLOAT4 lightDirf4;

	XMStoreFloat4(&lightDirf4, lightDir);

	scene->AddDirectionalLight(new DirectionalLight(lightDirf4, Colors::White, 1.0f));
	DirectionalLight* light = scene->GetDirectionalLight(0);
	mainCam->Update();
	rs.AddResource(mainCam->GetBuffer(), VERTEX, 0);
	rs.AddResource(transform->GetBuffer(), VERTEX, 1);

	rs.AddResource(mainCam->GetBuffer(), PIXEL, 0);
	rs.AddResource(transform->GetBuffer(), PIXEL, 1);
	rs.AddResource(light->AsBuffer(), PIXEL, 2);
	// Instance->GetBuffer();
			

	// Instance 버퍼도 붙여야함
	// 렌더 방식을 바꾸는 순간 리소스 다시 바인드 해야함.
	// 우버는 아니어도 최소한의 렌더 상태를 유지할 패스가 필요함.

	// Attempt 1. 렌더 큐에 넣는 방식을 사용하자.
	// 1. 렌더할 인스턴스를 렌더큐에 넣는다.
	// 2. 큐에서 빠지는 인스턴스마다 버퍼를 가져옴
	// 3. 해당 큐에서 사용하는 카메라 버퍼를 뽑아온 인스턴스 버퍼와 함께 파이프라인에 바인드
	// Problems
	//  - Deferred, Forward에 대한 다른 렌더 큐 구조가 필요하다.
	//  - 그래서 렌더는 누가할래
	//     - 씬이 해보는건 어떨까
	//     - 씬에 렌더큐 배치

}

void Engine::Update(float delta)
{
	Renderer2D& r2d = Renderer2D::GetInstance();

	static Tex2D* scTex = r2d.GetTexture2D(0);
	static Tex2D* scDepth = r2d.GetTexture2D(1);
	scTex->ClearRenderTarget(DirectX::Colors::OrangeRed);
	scDepth->ClearDepthStencil();

 	scene->Update(delta);
	//auto context = HW::GetContext();
	
}

void Engine::Render(float delta)
{
	mSwapChain->Present(0, 0);

}

void Engine::Release()
{
	
}
