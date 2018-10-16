
#include "Engine.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	Engine engine;
 	engine.Initialize(hInstance, "Title", "MyWindowClass", 800, 600);
		
	while (engine.ProcessMessages() == true)
	{
		engine.Update();
		engine.RenderFrame();
	}
	return 0;
}

//Graphics Pipeline
//Input Assembler has to be set up
//Vertex Shader has to be programmed
//Rasterizer has to be set up
//Pixel Shader has to be programmed
//Output Merger has to be set up