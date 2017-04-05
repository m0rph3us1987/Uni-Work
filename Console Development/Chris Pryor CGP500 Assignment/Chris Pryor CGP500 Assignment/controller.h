#pragma once


#include <pad.h>		  // scePadOpen(..)


namespace Solent
{

	class Controller
	{
	public:
		// Setup and initialize the sce/ps4 gamepad
		// ** 1 **
		void Create();

		// Tidy up and release any resources
		// ** 2 **
		void Release();

		// Read controllers and store any values
		// ** 3 **
		void Update();

		// Called multiple times to provide the data read from when
		// we did an update(..) call and got the information from
		// what buttons/analog input was active at that moment
		// ** 4 **
		Vector3 GetAnalogInput();

		// ** 5 **
		bool GetDigitalInput(ScePadButtonDataOffset buttonID);

	};// End class Controller



}// End namespace Solent