// Include header files for our PS4 renderer class
#include "renderer.h"
using namespace Solent;

// Program Entry Point
int main()
{
	// Renderer class
	Renderer renderer;

	// Initialize/setup
	renderer.Create();

	Mesh* m = renderer.CreateMesh();

	// Load texture for triangles
	m->LoadTextureFile("test.bmp");

	// Textured circle
	m->AddVertex(Vertex(0, 0, 0, 1, 1, 1, 0.5f, 0.5f));
	const int numTriangles = 10;
	for (int i = 0; i < numTriangles; i++)
	{
		// 0.0f to 1.0f
		float i0 = ((i + 0) % numTriangles) / (float)(numTriangles - 1);
		float i1 = ((i + 1) % numTriangles) / (float)(numTriangles - 1);

		// 0 to 2PI
		float e0 = (2.0f*3.14f) * i0;
		float e1 = (2.0f*3.14f) * i1;

		// sin/cos -0.5 to 0.5
		float x0 = cosf(e0) * 0.5f;
		float y0 = sinf(e0) * 0.5f;

		float x1 = cosf(e1) * 0.5f;
		float y1 = sinf(e1) * 0.5f;

		// tex coords 0 to 1.0
		float u0 = x0 + 0.5f;
		float v0 = y0 + 0.5f;

		float u1 = x1 + 0.5f;
		float v1 = y1 + 0.5f;

		// Use 1,1,1 colour (white)
		m->AddVertex(Vertex(x0, y0, 0, 1, 1, 1, u0, v0));
		m->AddVertex(Vertex(x1, y1, 0, 1, 1, 1, u1, v1));

		int numVerts = m->GetVertexCount();
		// Create triangle indices
		m->AddIndex(0, numVerts - 2, numVerts - 1);
	}// End for i

	// Create render buffer
	m->BuildTriangleBuffer();

	// Start drawing the triangles for 1000 frames then exit
	for (uint32_t frameIndex = 0; frameIndex < 1000; ++frameIndex)
	{
		// Update transforms
		m->rotation.setZ(m->rotation.getZ() + 0.005f);

		// Perform draw
		renderer.RenderLoop();
	}// End for(..)


	// Tidy up before exiting
	renderer.Release();

}// End main(..)