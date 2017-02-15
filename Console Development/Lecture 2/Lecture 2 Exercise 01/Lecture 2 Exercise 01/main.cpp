#include <libdbg.h> // SCE_BREAK(..)

// Custom assert - if the boolean test fails the
// code will trigger the debugger to halt
#define DBG_ASSERT(f) { if (!(f)) {SCE_BREAK(); } }

// Program Entry Point
int main()
{
	int a = 4;
	DBG_ASSERT(a == 4);

	// Test breakpoint, press f5 to continue
	DBG_ASSERT(a == 5); // Halt in Visual Studio on this line

	return 0;
}// End main(..)