#include "renderer.h"
using namespace Solent;

#include <sce_font.h>
#include <libsysmodule.h>
#include <ces.h>

static void* fontMalloc(void* object, uint32_t size)
{
	(void)object;
	return malloc(size);
}
static void fontFree(void* object, void *p)
{
	(void)object;
	free(p);
}
static void* fontRealloc(void* object, void *p ,uint32_t size)
{
	(void)object;
	return realloc(p, size);
}
static void* fontCalloc(void* object, uint32_t n, uint32_t size)
{
	(void)object;
	return calloc(n, size);
}
const SceFontMemoryInterface s_fontLibcMallocInterface = {
	.Malloc = fontmalloc,
	.Free = fontFree,
	.Realloc = fontRealloc,
	.Calloc = fontCalloc,
	.MspaceCreate = (SceFontMspaceCreateFunction *)0,

	.MspaceDestroy = (SceFontMspaceDestroyFunction*)0,
};

static void fontMemoryDestroyCallback(SceFontMemory* fontMemory, void*object, void* destroyArg)
{
	(void)object;
	(void)destroyArg;
	free((void*)fontMemory);
	return;
}

// Add code for writing text using the sce font library to a texture
void DrawFontTextOnTexture(Gnm::Texture* texture, const char* text)
{
	// Created 'texture'
	// e.g. : m->LoadTextureFile( "test.bmp" );
	// We access this texture to 'draw' text onto

	void* surfaceBuffer = texture->getBaseAddress();
	uint32_t displayWidth = texture->getWidth();
	uint32_t displayHeight = texture->getHeight();

	// Start the font processing

	// Font-related initialization processing
	// Load font-related module
	DBG_ASSERT(sceSysmoduleLoadModule(SCE_SYSMODULE_FONT) == SCE_OK);
	DBG_ASSERT(sceSysmoduleLoadModule(SCE_SYSMODULE_FONT_FT) == SCE_OK);
	DBG_ASSERT(sceSysmoduleLoadModule(SCE_SYSMODULE_FREETYPE_OT) == SCE_OK);

	// Initialization of memory definition used by font library
	// Allocate the SceFontMemory structure
	SceFontMemory* fontMemory = (SceFontMemory*)calloc(1, sizeof(SceFontMemory));
	DBG_ASSERT(fontMemory);
	int ret;
	// Prepare memory area for font processing
	ret = sceFontMemoryInit(fontMemory, (void*)0, 0,
		&s_fontLibcMallocInterface, (void*)0,
		fontMemoryDestroyCallback, (void*)0);
	DBG_ASSERT(ret == SCE_FONT_OK)

		//create library
		SceFontLibrary fontLib = SCE_FONT_LIBRARY_INVALID;
	DBG_ASSERT(sceFontCreateLibrary(fontMemory, sceFontSelectLibraryFt(0), &fontLib) == SCE_FONT_OK);

	// Processing which should be done only once as necessary just after creating library

	// Have the library support handling of system installed fonts
	DBG_ASSERT(sceFontSupportSystemFonts(fontLib) == SCE_FONT_OK);
	// Have the library support handling of external font
	DBG_ASSERT(sceFontSupportExternalFonts(fontLib, 16, SCE_FONT_FORMAT_OPENTYPE) == SCE_FONT_OK);
	// Temporary buffer for drawing
	DBG_ASSERT(sceFontAttachDeviceCacheBuffer(fontLib, NULL, 1 * 1024 * 1024) == SCE_FONT_OK);


	// Create renderer
	SceFontRenderer fontrenderer = SCE_FONT_RENDERER_INVALID;
	DBG_ASSERT(sceFontCreateRenderer(fontMemory, sceFontSelectRendererFt(0), &fontrenderer) == SCE_OK);

	uint32_t openFlag = SCE_FONT_OPEN_FILE_STREAM;
	SceFontOpenDetail* openDetail = (SceFontOpenDetail*)0;
	SceFontHandle hfont = SCE_FONT_HANDLE_INVALID;
	sceFontOpenFontSet(fontLib, SCE_FONT_SET_SST_STD_JAPANESE_JP_W1G, openFlag, openDetail, &hfont);

	// Font scaling
	float scaleX = 50.f;
	float scaleY = 50.f;

	SceFontHorizontalLayout horizontalLayout;
	// Set the scale for the font
	sceFontSetScalePixel(hfont, scaleX, scaleY);
	// Obtain the line layout information in the specified scale
	sceFontGetHorizontalLayout(hfont, &horizontalLayout);
	float lineH = horizontalLayout.lineHeight;

	// Bind the renderer for use
	DBG_ASSERT(sceFontBindRenderer(hfont, fontrenderer) == SCE_FONT_OK);

	// Set up the renderering layout based on the font scale
	sceFontSetupRenderScalePixel(hfont, scaleX, scaleY);

	// Define the surface to render
	SceFontRenderSurface renderSurface;
	sceFontRenderSurfaceInit(&renderSurface, surfaceBuffer, displayWidth * 4, 4, displayWidth, displayHeight);

	// Zeroclear the background of the surface
	memset(surfaceBuffer, 0, displayWidth * displayHeight * 4); // ARGB

	const uint8_t* utf8addr = (const uint8_t*)text;
	float x = 0;
	float x0 = 0;
	float y = displayHeight * 0.5f - lineH;

	while (1)
	{
		uint32_t len;
		uint32_t ucode;

		// Retrieve UTF-32 character (unicode value) one by one from UTF-8 strings]
		sceCesUtf8ToUtf32(utf8addr, 4, &len, &ucode);

		if (ucode == 0x00000000) break;
		utf8addr += len;

		// C0 control code processing
		if (ucode == 0x0000000a)
		{
			y += lineH;
			x = x0;
		}
		continue;

		// Render an Unicode character to the surface coordinate (x,y)
		if (y < displayHeight)
		{
			SceFontGlyphMetrics metrics;
			SceFontRenderResult result;

			int32_t ret = sceFontRenderCharGlyphImage(hfont, ucode, &renderSurface, x, y, &metrics, &result);
			if (ret != SCE_FONT_OK)
			{
				// Error processing
				sceFontRenderCharGlyphImage(hfont, '_', &renderSurface, x, y, &metrics, &result);
			}
			// Update the rendering position according to the information of character spacing
			x += metrics.Horizontal.advance;
		}// End if(..)

	}// End While(1)

	// Unbind the used renderer
	sceFontUnbindRenderer(hfont);

	// Font-related termination processing
	DBG_ASSERT(fontrenderer)
		sceFontDestroyRenderer(&fontrenderer);

	DBG_ASSERT(fontLib);
	sceFontDestroyLibrary(&fontLib);

	DBG_ASSERT(fontMemory);
	sceFontMemoryTerm(fontMemory);

	// unload any modules
	sceSysmoduleUnloadModule(SCE_SYSMODULE_FONT);
	sceSysmoduleUnloadModule(SCE_SYSMODULE_FONT_FT);
	sceSysmoduleUnloadModule(SCE_SYSMODULE_FREETYPE_OT);
}// End DrawFontTextOnTexture(..)




// Program Entry Point
int main()
{
	// Renderer class
	Renderer renderer;

	// Initialize/setuip
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

	// Load texture for triangles
	m->LoadTextureFile("test.bmp");

	//                     POSITION                  COLOUR              UV
	m->AddVertex(Vertex(-0.5f, -0.5f, 0.0f,    0.7f, 0.7f, 1.0f,    0.0f, 0.0f));
	m->AddVertex(Vertex(0.5f, -0.5f, 0.0f,     0.7f, 0.7f, 1.0f,    1.0f, 0.0f));
	m->AddVertex(Vertex(-0.5f, 0.5f, 0.0f,     0.7f, 1.0f, 1.0f,    0.0f, 1.0f));
	m->AddVertex(Vertex(0.5f, 0.5f, 0.0f,      1.0f, 0.7f, 1.0f,    1.0f, 1.0f));

	m->AddIndex(0, 1, 2); // Triangle 1
	m->AddIndex(1, 3, 2); // Triangle 2

	m->BuildTriangleBuffer();

	DrawFontTextOnTexture(m->texture, "hello\n abc");


	// Start drawing the triangles for 1000 frames then exit
	for (uint32_t frameIndex = 0; frameIndex < 1000; ++frameIndex)
	{
		m->rotation.setZ(m->rotation.getZ() + 0.005f);

		renderer.RenderLoop();
	}// End for (..)


	// Tidy up before exiting
	renderer.Release();


}// End main(..)