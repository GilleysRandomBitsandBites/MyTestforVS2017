#pragma once
#include <DirectXMath.h>

struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float r, float g, float b)
		:pos(x, y), color(r, g, b) {} //Initialize struct values

	DirectX::XMFLOAT2 pos;
	DirectX::XMFLOAT3 color; //Struct now also contains color value

};