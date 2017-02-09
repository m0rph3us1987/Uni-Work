// Include header files for our PS4 renderer class
#include "renderer.h"
using namespace Solent;

// Program Entry Point
int main()
{
	// Renderer Class
	Renderer renderer;

	// Initialize/setup
	renderer.Create();

	// Simple quad
	// 2    3
	// +----+
	// |\   |
	// | \  |
	// |  \ |
	// |   \|
	// +----+
	// 0    1

	Mesh* m = renderer.CreateMesh();

	// Load for triangles
	m->LoadTextureFile("snowman.bmp"); // try different test images, font.bmp, test.bmp

	//						POSITION             COLOR            UV
	m->AddVertex(Vertex(-0.5f, -0.5f, 0.0f, 0.7f, 0.7f, 1.0f, 0.0f, 0.0f));
	m->AddVertex(Vertex(0.5f, -0.5f, 0.0f, 0.7f, 0.7f, 1.0f, 1.0f, 0.0f));
	m->AddVertex(Vertex(-0.5f, 0.5f, 0.0f, 0.7f, 1.0f, 1.0f, 0.0f, 1.0f));
	m->AddVertex(Vertex(0.5f, 0.5f, 0.0f, 1.0f, 0.7f, 1.0f, 1.0f, 1.0f));

	// Triangle 1
	m->AddIndex(0, 1, 2);

	// Triangle 2
	m->AddIndex(1, 3, 2);

	// Create Buffers for renderer
	m->BuildTriangleBuffer();

	// Start drawing the triangles for 1000 frames then exit
	for (uint32_t frameIndex = 0; frameIndex < 1000; ++frameIndex)
	{
		m->rotation.setZ(m->rotation.getZ() + 0.005f);

		renderer.RenderLoop();

	}// End for (..)


	// Tidy up before exiting
	renderer.Release();

}// End main (..)
