
#include "Engine.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to call CoInitialize. ");
		return -1;
	}
	Engine engine;
	if (engine.Initialize(hInstance, "Title", "MyWindowClass", 800, 600))
	{
		while (engine.ProcessMessages() == true)
		{
			engine.Update();
			engine.RenderFrame();
		}
	}
	return 0;
}

//Graphics Pipeline
//Input Assembler has to be set up (Completed)
//Vertex Shader has to be programmed (Completed)
//Rasterizer has to be set up (Completed)
//Pixel Shader has to be programmed (Completed)
//Output Merger has to be set up (Completed)

//Create our Vertex Buffer (Completed)
//Draw (Completed)
//Create our indices Buffer (Completed)
//Create Vertex Buffer Template (Completed)
//Constant Buffers Setup for vertex shader pipeline (Completed)
//Constant Buffer Template (Completed)
//Setup Matrix Transformation (Completed)
//Setup View/Projection Matrix(Completed)
//Setup Camera Class