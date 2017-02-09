#include <libsysmodule.h> // sceSysmoduleLoadModule (..)
#include <ime_dialog.h> // sceImeDialogParamInit(..)
#include <user_service.h> // sceUserServiceGetInitialiser(..)

#include <libdbg.h> // SCE_BREAK(..)

// Custom assert - if the boolean test fails the
// code will trigger the debugger to halt
#define DBG_ASSERT(f) { if (!(f)) { SCE_BREAK(); } }

#define DIALOG_SIZE_WIDTH 960
#define DIALOG_SIZE_HEIGHT 592
#define TEXT_MAX_LENGTH 128

static wchar_t s_resultTextBuf[TEXT_MAX_LENGTH + 1];

// Program Entry Point
int main()
{
	int32_t ret = sceUserServiceInitialize(NULL);
	DBG_ASSERT(ret == SCE_OK );

	// uses library: libSceUserService_stub_weak.a
	SceUserServiceUserId userId;
	ret = sceUserServiceGetInitialUser(&userId);
	DBG_ASSERT(ret == SCE_OK);

	DBG_ASSERT(sceSysmoduleLoadModule(SCE_SYSMODULE_IME_DIALOG) == SCE_OK);

	memset(s_resultTextBuf, 0, sizeof(s_resultTextBuf));

	SceImeDialogParam dialogParam;
	sceImeDialogParamInit(&dialogParam);
	dialogParam.userId = userId;
	dialogParam.option = 0;
	dialogParam.supportedLanguages = 0;
	dialogParam.type = SCE_IME_TYPE_DEFAULT;
	dialogParam.inputTextBuffer = s_resultTextBuf;
	dialogParam.title = L"Test";
	dialogParam.maxTextLength = TEXT_MAX_LENGTH;
	dialogParam.posx = 100;
	dialogParam.posy = 100;

	ret = sceImeDialogInit(&dialogParam, NULL);
	DBG_ASSERT(ret == SCE_OK);

	while (true)
	{
		SceImeDialogStatus status = sceImeDialogGetStatus();

		// If the dialog has exited or finished then exit
		if (status != SCE_IME_DIALOG_STATUS_RUNNING)
		{
			break;
		}

		if (status == SCE_IME_DIALOG_STATUS_FINISHED)
		{
			// Get the result from the keyboard
			SceImeDialogResult dialogResult;

			// get ime dialog result
			memset(&dialogResult, 0, sizeof(SceImeDialogResult));
			DBG_ASSERT(sceImeDialogGetResult(&dialogResult) == SCE_OK);
			break;
		}
	}// End while(..)

	DBG_ASSERT(sceImeDialogTerm() == SCE_OK);

	return 0;
}// End main(..)