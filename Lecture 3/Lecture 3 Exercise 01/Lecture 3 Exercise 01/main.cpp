#include <video_out.h> // SceVideoOutBufferAttribute(..)
#include <gnmx.h> // SceVideoOutSubmitFlip(..)
using namespace sce;

#include <libdbg.h> // SCE_BREAK(..)

// Custom assert - if the boolean test fails the
// code will trigger the debugger to halt
#define DBG_ASSERT(f) { if (!(f)) { SCE_BREAK(); } }

// Program Entry Point
int main()
{
	//
	// Define window size
	//
	const uint32_t targetWidth = 1920;
	const uint32_t targetHeight = 1080;

	// Specify the size and alignment for buffer/screen memory
	const int graMemSize = 40 * 1024 * 1024;
	const int graMemAlign = 2 * 1024 * 1024;

	// Allocate render buffer video memory (Garlic)
	void * fbBaseAddr = 0;
	off_t offset = 0;

	int retSys = sceKernelAllocateDirectMemory(0,
		SCE_KERNEL_MAIN_DMEM_SIZE,
		graMemSize,
		graMemAlign, // alignment
		SCE_KERNEL_WC_GARLIC,
		&offset);
	DBG_ASSERT(retSys == SCE_OK);

	retSys = sceKernelMapDirectMemory(&reinterpret_cast<void*&>(fbBaseAddr),
		graMemSize,
		SCE_KERNEL_PROT_CPU_READ | SCE_KERNEL_PROT_CPU_WRITE | SCE_KERNEL_PROT_GPU_ALL,
		0, //flags
		offset,
		graMemAlign);
	DBG_ASSERT(retSys == SCE_OK);

	//
	// Setup the render target
	//

	// Creating render target descriptor and initializing it.
	Gnm::RenderTarget fbTarget;

	Gnm::SizeAlign fbSize =
		fbTarget.init(targetWidth, targetHeight,
		1,
		sce::Gnm::kDataFormatB8G8R8A8Unorm,
		sce::Gnm::kTileModeDisplay_LinearAligned,
		sce::Gnm::kNumSamples1,
		sce::Gnm::kNumFragments1,
		NULL,
		NULL);

	SceVideoOutBufferAttribute videoOutBufferAttribute;
	sceVideoOutSetBufferAttribute( &videoOutBufferAttribute,
		SCE_VIDEO_OUT_PIXEL_FORMAT_A8R8G8B8_SRGB,
		SCE_VIDEO_OUT_TILING_MODE_LINEAR,
		SCE_VIDEO_OUT_ASPECT_RATIO_16_9,
		targetWidth,
		targetHeight,
		targetWidth);

	int videoOut = sceVideoOutOpen(SCE_USER_SERVICE_USER_ID_SYSTEM, SCE_VIDEO_OUT_BUS_TYPE_MAIN, 0, NULL);
	DBG_ASSERT(videoOut >= 0);

	int regId = sceVideoOutRegisterBuffers(videoOut,
		0,
		reinterpret_cast<void**>(&fbBaseAddr),
		1,
		&videoOutBufferAttribute );
	DBG_ASSERT(regId >= 0);

	//Flip/display the frame buffer
	while (true)
	{
		//Gradually change from white to black
		static int shade = 0;
		if (shade >= 0xff) shade = 0;
		shade += 1;

		// In order to simplify the code, we use memset to clear the render target buffer.
		// This shows the frame buffer memory on the screen changing to different shades.
		// In practise you'll NOT directly access/set the GPU memory - as you'll learn, you'll
		// use shaders/graphics API as you learn more about the PS4 abilities
		memset(fbBaseAddr, shade, fbSize.m_size);

		// Set render target memory base address (gpu adress)
		fbTarget.setAddresses(fbBaseAddr, 0, 0);

		sce::Gnm::submitDone();

		sceVideoOutSubmitFlip(videoOut,
			0,
			SCE_VIDEO_OUT_FLIP_MODE_VSYNC,
			0);
	}// End while (true)

	return 0;
}// End main (..)
