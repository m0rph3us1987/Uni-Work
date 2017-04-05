#include "renderer.h"
#include "controller.h"
#include "font.h"
using namespace Solent;

#include <math.h>

#include "game.h"

#define ENABLE_FONTS

Renderer* g_renderer = NULL;
Mesh* g_mesh = NULL;
// Mesh object used by the `font' class
//#ifdef ENABLE_FONTS
Mesh*		g_fontMesh = NULL;
//#endif

Controller* g_controller = NULL;

#ifdef ENABLE_FONTS
Font*		g_font = NULL;
#endif

// Program Entry Point
int main()
{

	// Create game components
	g_controller = new Controller();
	g_renderer = new Renderer();
#ifdef ENABLE_FONTS
	g_font = new Font();
#endif

	// Initialize/setup
	g_renderer->Create();

	// Create controller config
	g_controller->Create();

	// Create a simple quad mesh - test our drawing code -
	// flushed out when the game is init/run - buffer data is replaced

	// Simple quad
	// 2    3
	// +----+
	// |\   |
	// | \  |
	// |  \ |
	// |   \|
	// +----+
	// 0    1

#if 1
	g_mesh = g_renderer->CreateMesh();
	g_mesh->enabled = true;
	g_mesh->primitiveType = Gnm::kPrimitiveTypeLineList;
	g_mesh->scale = Vector3(640, 480, 1);
	g_mesh->translation = Vector3(0, 0, 0);

	// Load texture for triangles
	g_mesh->CreateBlankTexture();

	//	                     POSITION                COLOR               UV
	g_mesh->AddVertex(Vertex(-0.5f, -0.5f, 0.0f, 0.7f, 0.7f, 1.0f, 0.0f, 0.0f));
	g_mesh->AddVertex(Vertex(0.5f, -0.5f, 0.0f, 0.7f, 0.7f, 1.0f, 1.0f, 0.0f));

	g_mesh->AddVertex(Vertex(-0.5f, 0.5f, 0.0f, 0.7f, 1.0f, 1.0f, 0.0f, 1.0f));
	g_mesh->AddVertex(Vertex(0.5f, 0.5f, 0.0f, 1.0f, 0.7f, 1.0f, 1.0f, 1.0f));

	// Line 1
	g_mesh->AddIndex(0);
	g_mesh->AddIndex(1);
	// Line 2
	g_mesh->AddIndex(1);
	g_mesh->AddIndex(2);

	// Build our `base' mesh holder for drawing all the lines/objects for the game
	g_mesh->BuildDrawBuffer();
#endif


	// Create a font manager class - draw to a texture/mesh provided
	// by the renderer class
#ifdef ENABLE_FONTS
	g_fontMesh = g_renderer->CreateMesh();
	g_font->Create();
#endif

	// Create our game
#if 1
	//Create();
#endif

	// MAIN RENDER/UPDATE LOOP
	while (true)
	{
		// Ask controller if any buttons/analog input and store data
		g_controller->Update();

		// Game logic/mechanics
#if 1
		Update();

		Render();
#endif


		// Update the vertex data/indices from the game (i.e., line
		// positions, number of lines, )
#if 1 // debug lines
		g_mesh->RebuildDrawBuffer();
#endif


		// draw the font/text to an offscreen texture buffer
#ifdef ENABLE_FONTS
		g_font->Render();
#endif

		// render everything to the screen
		g_renderer->RenderLoop();

	}// End while(..)

	Release();

	// Tidy up before exiting
	g_renderer->Release();
	g_controller->Release();
#ifdef ENABLE_FONTS
	g_font->Release();
#endif


	delete g_renderer;
	g_renderer = NULL;

	delete g_controller;
	g_controller = NULL;

#ifdef ENABLE_FONTS
	delete g_font;
	g_font = NULL;
#endif

}