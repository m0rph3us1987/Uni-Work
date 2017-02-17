#include "renderer.h"
using namespace Solent;

// Program Entry Point
int main()
{
	// Renderer class
	Renderer renderer;

	// Initialize/setup
	renderer.Create();

	// Create 1st Mesh Object
	Mesh* m0 = renderer.CreateMesh();

	// Load texture for triangles
	m0->LoadTextureFile("/app0/Media/spaghetti.jpg");

	//          *
	//        *   *
	//      *       *
	//    *  *  *  *  *
	//                      POSITION                COLOR                 UV
	m0->AddVertex(Vertex(-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f));
	m0->AddVertex(Vertex(0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f));
	m0->AddVertex(Vertex(0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f));

	// Triangle 1
	m0->AddIndex(0, 1, 2);
	// Build our render buffer array
	m0->BuildTriangleBuffer();

	// Create 2nd Mesh Object
	Mesh* m1 = renderer.CreateMesh();

	// Load texture for triangles
	m1->LoadTextureFile("/app0/Media/spaghetti.jpg");

	//    *  *  *  *  *
	//      *       *
	//        *   *
	//          *
	//                      POSITION                COLOR                 UV
	// Counter-clockwise
	m1->AddVertex(Vertex(-0.5f, 0.5f, 0.2f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f));
	m1->AddVertex(Vertex(0.5f, -0.5f, 0.2f, 1.0f, 1.0f, 1.0f, 0.5f, 0.0f));
	m1->AddVertex(Vertex(0.5f, 0.5f, 0.2f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f));

	// Triangle 2
	m1->AddIndex(0, 1, 2);
	// Build our render buffer array
	m1->BuildTriangleBuffer();



	// Start drawing the triangles for 1000 frames then exit
	for (uint32_t frameIndex = 0; frameIndex < 1000; ++frameIndex)
	{
		m0->rotation.setZ(m0->rotation.getZ() + 0.005f);

		renderer.RenderLoop();
	}// End for(..)


	// Tidy up before exiting
	renderer.Release();

}//End main(..)