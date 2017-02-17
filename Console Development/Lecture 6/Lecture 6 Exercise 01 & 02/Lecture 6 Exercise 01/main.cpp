#include "renderer.h"
using namespace Solent;

// Program Entry Point
int main()
{
	// Renderer class
	Renderer renderer;

	// Initialize/setup
	renderer.Create();

#if 1

	Mesh* m = renderer.CreateMesh();

	// Load texture for triangles
	m->LoadTextureFile("/app0/Media/spaghetti.jpg");

	//          *
	//        *   *
	//      *       *
	//    *  *  *  *  *
	//                      POSITION                COLOR                 UV
	/* Counter-clockwise
	m->AddVertex(Vertex(-0.5f, -0.5f, 0.0f,    0.7f, 0.7f, 1.0f,     0.0f, 0.0f));
	m->AddVertex(Vertex(0.5f, -0.5f, 0.0f,     0.7f, 0.7f, 1.0f,     1.0f, 0.0f));
	m->AddVertex(Vertex(0.0f, 0.5f, 0.0f,      0.7f, 1.0f, 1.0f,     0.5f, 1.0f));
	*/
	// Clockwise
	m->AddVertex(Vertex(-0.5f, -0.5f, 0.0f, 0.7f, 0.7f, 1.0f, 0.0f, 0.0f));
	m->AddVertex(Vertex(0.0f, 0.5f, 0.0f, 0.7f, 1.0f, 1.0f, 0.5f, 1.0f));
	m->AddVertex(Vertex(0.5f, -0.5f, 0.0f, 0.7f, 0.7f, 1.0f, 1.0f, 0.0f));

	// Triangle 1
	m->AddIndex(0, 1, 2);

	m->BuildTriangleBuffer();
#endif

	// Start drawing the triangles for 1000 frames then exit
	for (uint32_t frameIndex = 0; frameIndex < 1000; ++frameIndex)
	{
		m->rotation.setZ(m->rotation.getZ() + 0.005f);

		renderer.RenderLoop();
	}// End for(..)


	// Tidy up before exiting
	renderer.Release();

}//End main(..)
