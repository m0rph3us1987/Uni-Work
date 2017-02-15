#include <kernel.h> //sceKernelSetGPO(..)
#include <stdio.h> // printf(..)
//
// Program Entry Point
//
int main()
{
	printf("\n\n *** Hello World *** \n\n");

	// Bit pattern to set each LED on/off (only the lower 8 bits are valid)
	uint32_t uiBits = 0xA7D8CB; // 11000011
	sceKernelSetGPO(uiBits);

	// Waits for 10 seconds
	for (int i = 0; i < 10; i++)
	{

		// Sleep in microseconds
		sceKernelSleep(1);

		// Scroll the LED lights
		uiBits = uiBits << 1;
		sceKernelSetGPO(uiBits);

		printf("Tick..\n");
	} // End for i

	return 0;
} // End main(..)