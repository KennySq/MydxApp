#pragma once

#include<Mydx.h>

using namespace Mydx;

class Engine
{
	Hardware* mHardware;
	IDXGISwapChain* mSwapChain;
	Scene* scene;
	
public:
	Engine(HWND hwnd, HINSTANCE hInstance);
	~Engine();

	void PreInit();
	void Init();
	void Update(float delta);
	void Render(float delta);
	void Release();
};

