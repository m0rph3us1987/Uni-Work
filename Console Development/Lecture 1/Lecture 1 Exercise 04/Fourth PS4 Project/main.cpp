#include <user_service.h> // sceUserServiceGetInitialiser(..)
#include <pad.h> // scePadOpen(..)
//
// Program Entry Point
//
int main()
{
	int32_t ret = sceUserServiceInitialize(NULL);
	if (ret != 0) printf("sceUserServiceInitialize failed\n");

	// uses library: libSceUserService_stub_weak.a
	SceUserServiceUserId userId;
	ret = sceUserServiceGetInitialUser(&userId);
	if (ret != 0) printf("sceUserServiceInitialUser failed\n");

	ret = scePadInit();
	if (ret != 0) printf("scePadInit failed\n");

	// Example that specifies SCE_PAD_PORT_TYPE_STANDARD controller type
	int32_t handle = scePadOpen(userId, SCE_PAD_PORT_TYPE_STANDARD, 0, NULL);
	if (handle < 0) printf("scePadOpen failed\n");

	while (true)
	{
		// Get the data for an individual controller
		ScePadData data;

		// Obtain state of controller data
		ret = scePadReadState(handle, &data);

		// If data is obtained from the controller
		if (ret == 0)
		{
			// Use the input from the controller to change
			// the speed of the vibrate

			ScePadVibrationParam vibrateParam;
			vibrateParam.largeMotor = data.leftStick.x;
			vibrateParam.smallMotor = data.rightStick.y;
			ret = scePadSetVibration(handle, &vibrateParam);

			// If the user presses the circle button exit
			if (data.buttons & SCE_PAD_BUTTON_CIRCLE)
			{
				break;
			}
		}
		// small delay
		sceKernelUsleep(100); // wait here for 100 us
	} // End main(..)

	//Tidy up
	scePadClose(handle);

	return 0;
}// End main(..)