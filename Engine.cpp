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
	mSwapChain = HW::GetSwapChain();

	Pass* p = new Pass("SampleShader.hlsl", "Sample", VERTEX | PIXEL, FORWARD);
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
	
	rs.AddResource(mainCam->GetBuffer(), VERTEX, 0);
	rs.AddResource(transform->GetBuffer(), VERTEX, 1);
	// Instance->GetBuffer();
	

	// Instance ���۵� �ٿ�����
	// ���� ����� �ٲٴ� ���� ���ҽ� �ٽ� ���ε� �ؾ���.
	// ����� �ƴϾ �ּ����� ���� ���¸� ������ �н��� �ʿ���.

	// Attempt 1. ���� ť�� �ִ� ����� �������.
	// 1. ������ �ν��Ͻ��� ����ť�� �ִ´�.
	// 2. ť���� ������ �ν��Ͻ����� ���۸� ������
	// 3. �ش� ť���� ����ϴ� ī�޶� ���۸� �̾ƿ� �ν��Ͻ� ���ۿ� �Բ� ���������ο� ���ε�
	// Problems
	//  - Deferred, Forward�� ���� �ٸ� ���� ť ������ �ʿ��ϴ�.
	//  - �׷��� ������ �����ҷ�
	//     - ���� �غ��°� ���
	//     - ���� ����ť ��ġ

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
