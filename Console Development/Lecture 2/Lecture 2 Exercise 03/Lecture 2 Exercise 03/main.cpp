#include <libsysmodule.h> // sceSysmoduleLoadModule(..)
#include <message_dialog.h> // sceMsgDialogInitialize(..)

#include <libdbg.h> // SCE_BREAK(..)

// Custom assert - if the boolean test fails the
// code will trigger the debugger to halt
#define DBG_ASSERT(f) { if (!(f)) { SCE_BREAK(); } }

int main()
{
	// startup initialise ps4 modules
	DBG_ASSERT(sceSysmoduleLoadModule(SCE_SYSMODULE_MESSAGE_DIALOG) == SCE_OK);

	// start common dialog
	DBG_ASSERT(sceCommonDialogInitialize() == SCE_OK);

	// start msg dialog
	DBG_ASSERT(sceMsgDialogInitialize() == SCE_OK);

	// show dialog
	SceMsgDialogParam param;
	sceMsgDialogParamInitialize( &param );
	param.mode = SCE_MSG_DIALOG_MODE_USER_MSG;

	SceMsgDialogUserMessageParam userMsgParam;
	memset(&userMsgParam, 0, sizeof(userMsgParam));
	static const char msg_hello[] = "Hello world!";
	userMsgParam.msg = msg_hello;
	userMsgParam.buttonType = SCE_MSG_DIALOG_BUTTON_TYPE_OK;
	param.userMsgParam = &userMsgParam;

	// Display
	DBG_ASSERT(sceMsgDialogOpen(&param) == SCE_OK);

	// Wait for the dialog to complete
	while (1)
	{
		int stat = sceMsgDialogUpdateStatus();
		if (stat == SCE_COMMON_DIALOG_STATUS_FINISHED)
		{
			// Exit while (1) loop
			break;
		}// End if state

		if (stat != SCE_COMMON_DIALOG_STATUS_RUNNING)
		{
			// Exit while (1) loop
			break;
		}// End if state

		for (SceUInt32 rate = 0; rate <= 100; rate += 10)
		{
			// Set the progress rate of the progress bar in the message dialog immediately
			sceMsgDialogProgressBarSetValue(SCE_MSG_DIALOG_PROGRESSBAR_TARGET_BAR_DEFAULT, rate);
			// Wait 1 seconf
			::sceKernelSleep(1);
		}// End for rate


		// Call sceMsgDialogClose() to force exit the dialog without waiting
		// for the user to select ok/cancel
		// DBG_ASSERT( sceMsgDialogClose() == SCE_OK);

	}// End while(1)


	// Get the result from msg dialog (y/n) if we want to use it
	SceMsgDialogResult result;
	memset(&result, 0, sizeof(result));
	DBG_ASSERT(sceMsgDialogGetResult(&result) == SCE_OK);

	// close
	DBG_ASSERT(sceMsgDialogTerminate() == SCE_OK);
	DBG_ASSERT(sceSysmoduleUnloadModule(SCE_SYSMODULE_MESSAGE_DIALOG));

	return 0;
}// End main(..)