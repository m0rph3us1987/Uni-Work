#include "renderer.h"
using namespace Solent;



// Program Entry Point
int main()
{
	// Renderer Class
	Renderer renderer;

	// Initialize/setup
	renderer.Create();

#if 1 // simple quad
	// 2    3
	// +----+
	// |\   |
	// | \  |
	// |  \ |
	// |   \|
	// +----+
	// 0    1

	Mesh* m = renderer.CreateMesh();

	// Load texture for triangles
	m->LoadTextureFile("/app0/Media/spritesheet1.png"); // simple 9x9 explosion animation

	float sx = 1.0f / 9.0f; // 9x9 sprite sheet
	float sy = 1.0f / 9.0f;
	//                      POSITION                COLOR                 UV
	m->AddVertex(Vertex(-0.5f, -0.5f, 0.0f,    1.0f, 1.0f, 1.0f,        sx, sy));
	m->AddVertex(Vertex( 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,       0.0f, sy));
	m->AddVertex(Vertex(-0.5f, 0.5f, 0.0f,     1.0f, 1.0f, 1.0f,        sx, 0.0f));
	m->AddVertex(Vertex( 0.5f, 0.5f, 0.0f,      1.0f, 1.0f, 1.0f,       0.0f, 0.0f));

	m->AddIndex(0, 1, 2); // Triangle 1
	m->AddIndex(1, 3, 2); // Triangle 2

	m->BuildTriangleBuffer();
#endif



	// Start drawing the triangles for 1000 frames then exit
	for (uint32_t frameIndex = 0; frameIndex < 10000; ++frameIndex)
	{
		static float uoff = 0;
		static float voff = 0;


		uoff += sx;
		if (uoff >= 1.0f)
		{
			uoff = 0;
			voff += sy;
		}// End if
		m->SetTextureCoordinateOffset(uoff, voff);

		renderer.RenderLoop();
	}// End for(..)


	// Tidy up before exiting
	renderer.Release();

}// End main(..)