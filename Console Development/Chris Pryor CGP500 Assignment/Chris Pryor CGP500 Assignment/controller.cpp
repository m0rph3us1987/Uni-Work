#include <user_service.h> // sceUserServiceGetInitialUser(..)
#include <pad.h>		  // scePadOpen(..)


#include "renderer.h"	  // DBG_ASSERT(..)

#include "controller.h"

namespace Solent
{

	int32_t					handle;
	SceUserServiceUserId	userId;

	// Get the data for an individual controller
	ScePadData				padData;

	void Controller::Create()
	{
		DBG_ASSERT(sceUserServiceInitialize(NULL) == SCE_OK);

		// uses library: libSceUserService_stub_weak.a
		DBG_ASSERT(sceUserServiceGetInitialUser(&userId) == SCE_OK);

		DBG_ASSERT(scePadInit() == SCE_OK);


		// Example that specifies SCE_PAD_PORT_TYPE_STANDARD to type
		// Specify 0 for index
		// pParam is a reserved area (specify NULL)
		// uses library: libScePad_stub_weak.a
		handle = scePadOpen(userId, SCE_PAD_PORT_TYPE_STANDARD, 0, NULL);
		DBG_ASSERT(handle >= 0);

	}// End Create(..)


	void Controller::Release()
	{
		scePadClose(handle);

	}// End Release(..)


	void Controller::Update()
	{
		// Obtain state of controller data
		memset(&padData, 0, sizeof(padData));
		DBG_ASSERT(scePadReadState(handle, &padData) == SCE_OK);

	} // End Update(..)


	Vector3 Controller::GetAnalogInput()
	{
		ScePadAnalogStick& padStick = padData.rightStick;

		float xf = padStick.x / 255.0f;
		float yf = padStick.y / 255.0f;

		return Vector3(xf, yf, 0);
	}// End GetAnalogInput(..)



	bool Controller::GetDigitalInput(ScePadButtonDataOffset buttonID)
	{
		// If the user presses the circle button exit
		//if ( padData.buttons & SCE_PAD_BUTTON_CIRCLE )
		//{
		//	return true;
		//}

		// If the user presses the circle button exit
		if (padData.buttons & buttonID)
		{
			return true;
		}

		return false;
	}// End GetDigitalInput(..)


}// End namespace Solent