#include "renderer.h"				// ps4 renderer
#include "controller.h"				// ps4 controller input
#include "font.h"					// ps4 font class
using namespace Solent;

#define ENABLE_FONTS				// enable/disable fonts

// We need this for input output using fopen and fread.
#include <stdio.h>


// Declared in main.cpp - and is used to access our renderere surface 
// or and other system api's that need the ps4
extern Renderer*	g_renderer;//= NULL; main.cpp
extern Mesh*		g_mesh;//= NULL; main.cpp
// Mesh object used by the `font' class
#ifdef ENABLE_FONTS
extern Mesh*		g_fontMesh;//= NULL; main.cpp
#endif
// Input class
extern Controller*	g_controller;//= NULL; main.cpp
// Font class
#ifdef ENABLE_FONTS
extern Font*		g_font;//= NULL; main.cpp
#endif

////////////////////////////////////////////////////////////////////////////////////
//                                                                                //
//  These four functions will be called externally from main, and update our      //
//  game, initilise it.... render it...etc.                                       //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

bool Create();                            // Called once at start of program loading
void Release();                           // Called when program ends
void Update();                            // Just before Render is called
void Render();                            // Called in main render loop.

float g_scrollOffset = 0;

struct Rectangle
{
public:
	float X, Y;
	float Width, Height;

	Rectangle(const Rectangle& r)
	{
		this->X = r.X;
		this->Y = r.Y;
		this->Width = r.Width;
		this->Height = r.Height;
	}// End copy constrcutor

	Rectangle() { }

	Rectangle(float x, float y, float w, float h)
	{
		this->X = x;
		this->Y = y;
		this->Height = h;
		this->Width = w;
	}// End Rectangle(..)

	float Left() const  { return X; }
	float Right() const  { return X + Width; }
	float Top() const  { return Y; }
	float Bottom() const  { return Y + Height; }

	bool IntersectsWith(const Rectangle& r) const
	{
#if 1
		float dx = X + Width*0.5f - (r.X + r.Width*0.5f);
		float dy = Y + Height*0.5f - (r.Y + r.Height*0.5f);
		float tw = (Width + r.Width)*0.5f;
		float th = (Height + r.Height)*0.5f;

		if (abs(dx) <= abs(tw) &&
			abs(dy) <= abs(th))
		{
			return true;
		}// End if
		return false;
#else
		return !((Left() >= r.Right()) || (Right() <= r.Left()) ||
			(Top() >= r.Bottom()) || (Bottom() <= r.Top()));
#endif
	}// End IntersectsWith(..)

	//void Draw()
	//{
	//	// Show contact information for the ground - turned
	//	// off in debug
	//	DrawWireRectangle( Vector3(0,0,0), X, Y, Width, Height );
	//}// End Draw(..)
};// End struct Rectangle

enum InputState
{
	VK_UNKNOWN = -1,
	VK_LEFT = 0,
	VK_RIGHT,
	VK_UP,
	VK_SHIFT,
	VK_CONTROL,
	VK_RETURN,
};


// Controller input 
bool GetInput(InputState key)
{
	if (g_controller->GetDigitalInput(SCE_PAD_BUTTON_CROSS))
	{
		if (key == VK_SHIFT || key == VK_CONTROL) return true;
	}

	if (g_controller->GetDigitalInput(SCE_PAD_BUTTON_UP))
	{
		if (key == VK_UP) return true;
	}

	if (g_controller->GetDigitalInput(SCE_PAD_BUTTON_RIGHT))
	{
		if (key == VK_RIGHT) return true;
	}

	if (g_controller->GetDigitalInput(SCE_PAD_BUTTON_LEFT))
	{
		if (key == VK_LEFT) return true;
	}

	if (g_controller->GetDigitalInput(SCE_PAD_BUTTON_TRIANGLE))
	{
		if (key == VK_RETURN) return true;
	}

	return false;
}// End GetKeyDown(..)

enum Status
{
	DEAD,
	ALIVE
};

struct Point
{
	float x;
	float y;
};// End struct Point


struct stPlayer
{
	vector<Mesh*> meshes;
	Rectangle rect = { 0, 0, 0, 0 };
	int				direction = 0;
	Point			velocity = { 0, 0 };
	Point			force = { 0, 0 };
	float			frame = 0;
	Status			status = ALIVE;
	int				score = 0;

	void Render()
	{
		DBG_ASSERT(meshes.size() > 0); // couldn't find mesh

		// Don't draw
		for (int i = 0; i<meshes.size(); ++i)
		{
			meshes[i]->enabled = false;
		}// End for i

		if (abs(velocity.x) > 0.1f)
		{
			frame += 0.1f;
		}// End if direction

		if (frame >= 1.0f) frame = 0.0f;

		int indx = (int)(direction * 2 + frame + 0.5f);
		DBG_ASSERT(indx >= 0 && indx< meshes.size())

			Mesh* mesh = meshes[indx];
		mesh->translation = Vector3(rect.X + g_scrollOffset,
			rect.Y,
			0);
		mesh->scale = Vector3(rect.Width, rect.Height, 1);
		mesh->enabled = true;
	}
};

struct stLevel
{
	vector<stPlayer>		players; // future support for multiplayer
	//vector<stTurtle>		turtles;
	//vector<stBricks>		bricks;
	//vector<stRiver>			rivers;
	//vector<stBackground>	backgrounds;
};


////////////////////////////////////////////////////////////////////////////////////

// Our entire game/level is essentially stored in `g_level'
stLevel			g_level;
// Keep track of all our meshes - so we can decide what needs drawing/not drawing
// for instance if we want to turn off all drawing except for a warning messages
vector<Mesh*>	g_meshes;

////////////////////////////////////////////////////////////////////////////////////

Mesh* LoadAsset(const char* szFileName,
	const Rectangle& rect,
	float zval = 0.0f)
{
	/*
	WorldViewTransform set in the ps4 renderer uses the `scale' and `translate'
	transforms on the mesh to generate the matrix and display quads scaled
	to the set screen size - for this demo - 640x480
	*/
	Mesh* mesh = g_renderer->CreateMesh();

	// Load texture for triangles
	mesh->LoadTextureFile(szFileName);

	const float			ww = 1.0f;
	const float			hh = 1.0f;

	//	                     POSITION                COLOR               UV
	mesh->AddVertex(Vertex(-ww, -hh, zval, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f));
	mesh->AddVertex(Vertex(ww, -hh, zval, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f));
	mesh->AddVertex(Vertex(-ww, hh, zval, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f));
	mesh->AddVertex(Vertex(ww, hh, zval, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f));

	mesh->AddIndex(0);
	mesh->AddIndex(1);
	mesh->AddIndex(2); // Triangle 1

	mesh->AddIndex(1);
	mesh->AddIndex(3);
	mesh->AddIndex(2); // Triangle 2


	// Build our `base' mesh holder for drawing all the lines/objects for the game
	mesh->BuildDrawBuffer();

	mesh->scale = Vector3(rect.X, rect.Y, 1.0f);
	mesh->translation = Vector3(0, 0, 0);
	g_meshes.push_back(mesh);

	return mesh;
} // End of LoadAsset(..)

void ReadLevelData()
{

	const int level1 = 1;

	char name[256];

	sprintf(name, "/app0/Media/data/level%d.txt", level1);
	FILE* fp = fopen(name, "r");
	DBG_ASSERT(fp);

	while (true)
	{
		char type[256];
		int val = fscanf(fp, "%s", type);
		if (val <= 0)
		{
			break;
		} // End if val

		if (strstr("player", type) != 0)
		{
			int xpos = 0;
			int ypos = 0;
			int width = 0;
			int height = 0;

			val = fscanf(fp, "%d %d %d %d",
				&xpos, &ypos, &width, &height);
			DBG_ASSERT(val == 4);
			Rectangle rect(xpos, ypos, width, height);

			stPlayer player;
			player.rect = rect;
			player.velocity = { 0, 0 };
			player.status = ALIVE;
			player.direction = 0;
			player.meshes.push_back(LoadAsset(".\\images\\playerright1.bmp", rect, -0.5));
			//player.meshes.push_back(LoadAsset(".\\images\\right1.bmp", rect, -0.5));
			//player.meshes.push_back(LoadAsset(".\\images\\right2.bmp", rect, -0.5));
			//player.meshes.push_back(LoadAsset(".\\images\\left1.bmp", rect, -0.5));
			//player.meshes.push_back(LoadAsset(".\\images\\left2.bmp", rect, -0.5));
			g_level.players.push_back(player);
		}
	}
}




bool Create()
{
	ReadLevelData();

	return true;

} // End of Create()



void Release()
{
	// todo

} // End of Release()


float Clamp(float in, float lo, float hi)
{
	if (in > hi) in = hi;
	if (in < lo) in = lo;
	return in;
}// End Clamp(..)



void Update()
{
	DBG_ASSERT(g_level.players.size() == 1);
	stPlayer* player = &g_level.players[0];
	if (player->status == DEAD)
	{
		if (GetInput(VK_RETURN)) // Start new game if your dead :)
		{
			player->status = ALIVE;
			player->rect.X = 10;
			player->rect.Y = 500;
			g_scrollOffset = 0;

			// ReadLevelData();
			//InitGame();
		}
		return;
	}// End if player-status


	// User input
	const float moveSpeed = 305.0f;

	// 0 - forward
	// 1 - backward
	player->direction = 0;

	if (GetInput(VK_RIGHT))
	{
		player->force.x += moveSpeed;
		player->direction = 0;
	}
	else
	if (GetInput(VK_LEFT))
	{
		player->force.x -= moveSpeed;
		player->direction = 1;
	}// End if GetInput(..)

	// Multiple players
	for (int i = 0; i<g_level.players.size(); ++i)
	{
		stPlayer* players = &g_level.players[i];

		// Constant downward gravity
		players->force.y -= 50.0f;

		// f = ma ( mass==1 )
		players->velocity.x += player->force.x * 0.1f;
		players->velocity.y += player->force.y * 100.1f;


		players->rect.X += player->velocity.x * 0.1f;
		// Clamp is to fix tunneling
		players->rect.Y += Clamp(player->velocity.y, -50, 50) * 0.1f;

		players->velocity.x *= 0.1f;
		players->velocity.y *= 0.9f;


		// Update player movement
		player->force.x = 0;
		player->force.y = 0;
	}// End for i

}




inline
void RenderGame()
{

	for (int i = 0; i<g_level.players.size(); ++i)
	{
		stPlayer* player = &g_level.players[i];
		player->Render();
	}// End for i
}

void Render()
{
	RenderGame();

	//DisplayScore();

	// **************************Debug Information*************************//
#if 0
	{
		static int fps = 0;
		// Todo - add timing information from the console

		DisplayText("Console Programming", 50, 15, Vector3(1, 1, 1));
		DisplayText("DemoLevel - MiniGame", 50, 33, Vector3(1, 1, 1));

		char buf[200];
		sprintf(buf, "Debug: Frame Per Sec: %d", fps);
		DisplayText(buf, 50, 50, Vector3(1, 1, 1));
	}
#endif
} // End of Render(..)
