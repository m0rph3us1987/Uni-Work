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

	// Make pad vibrate
	//typedef struct ScePadVibrationParam{
	//uint8_t largeMotor;     /*E large(left) motor(0:stop 1~255:rotate).   */
	//uint8_t smallMotor;     /*E small(right) motor(0:stop 1~255:rotate).   */
	//} ScePadVibrationParam;
	ScePadVibrationParam vibrateParam;
	vibrateParam.largeMotor = 128;
	vibrateParam.smallMotor = 55;
	ret = scePadSetVibration(handle, &vibrateParam);
	if (ret != 0) printf("scePadSetVibration failed\n");

	// Wait here for 20 seconds
	for (int i = 0; i < 20; i++)
	{
		// Small delay
		sceKernelSleep(1); // wait here for a second

		vibrateParam.largeMotor += 1;
		vibrateParam.smallMotor += 1;
		ret = scePadSetVibration(handle, &vibrateParam);
	}// End while(..)

	//Tidy up
	scePadClose(handle);

	return 0;
}// End main(..)