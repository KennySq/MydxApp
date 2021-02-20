#include"framework.h"
#include "Engine.h"
#include"EnvironmentalVariables.h"

using namespace Mydx;
using namespace std;
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
	auto cubeDesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R32G32B32A32_FLOAT, 1024, 1024, 6U, 1U, D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT);
	
	
	Tex2D* scTex = new Tex2D(scDesc, true);
	Tex2D* depthTex = new Tex2D(depthDesc);
///	Tex2D* cubeMap = new Tex2D(cubeDesc, false, true);
	Tex2D* cubeMap = new Tex2D("resources/SampleCubemap.dds");
	
	r2d.AddTexture2D(scTex);
	r2d.AddTexture2D(depthTex);
	r2d.AddTexture2D(cubeMap);
	scene->Init();
}

void Engine::Init()
{
	AllocConsole();

	mSwapChain = HW::GetSwapChain();
	Renderer2D& r2d = Renderer2D::GetInstance();


	string resourcePath = MYDX_PATH;
	resourcePath += "StandardForwardShader.hlsl";
	Pass* p = new Pass(resourcePath.c_str(), "Sample", VERTEX | PIXEL, FORWARD);
	Mesh* mesh = new Mesh(*PrimitiveGenerator::GenerateSphere(1.0f, 32, 32));
	Camera* mainCam = scene->GetCamera();

	Instance* inst = new Instance();
	Instance* skybox = new Instance();

	inst->AddComponent<MeshRenderer>();
	skybox->AddComponent<MeshRenderer>();
	
	Transform* transform = inst->GetTransform();
	Transform* skyTransform = skybox->GetTransform();

	scene->AddInstance(inst);
	scene->AddInstance(skybox);

	scene->Init();
	
	

	MeshRenderer* mr = inst->GetComponent<MeshRenderer>();
	MeshRenderer* skyboxMr = skybox->GetComponent<MeshRenderer>();

	mr->SetMesh(&*mesh);
	mr->SetPass(p);

	skyboxMr->SetMesh(&*mesh);
	skyboxMr->SetPass(p);
	
	RenderState& rs = mr->GetState();
	RenderState& skyboxRs = skyboxMr->GetState();

	XMVECTOR lightDir = XMVectorSet(1,1, -1, 1) - XMVectorSet(0, 0, 0, 0);
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


	skyboxRs.ChangeCullMode(D3D11_CULL_NONE);
	skyboxRs.AddResource(mainCam->GetBuffer(), VERTEX, 0);
	skyboxRs.AddResource(skyTransform->GetBuffer(), VERTEX, 1);

	skyboxRs.AddResource(mainCam->GetBuffer(), PIXEL, 0);
	skyboxRs.AddResource(skyTransform->GetBuffer(), PIXEL, 1);

	transform->SetScale(1.5, 1.5, 1.5);
	skyTransform->SetScale(1000, 1000, 1000);
	Tex2D* cubeTex = r2d.GetTexture2D(2);

	skyboxRs.AddResource(cubeTex->GetShaderResource(), PIXEL, 0);
	rs.AddResource(cubeTex->GetShaderResource(), PIXEL, 0);
	// Instance->GetBuffer();
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
