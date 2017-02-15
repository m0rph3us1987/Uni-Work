#include <stdio.h>
#include <web_browser_dialog.h>
#include <libsysmodule.h>
#include <user_service.h>

#include <libdbg.h> // SCE_BREAK(..)

// Custom assert - if the boolean test fails the
// code will trigger the debugger to halt
#define DBG_ASSERT(f) { if (!(f)) { SCE_BREAK(); } }

#define DBG_ASSERT_MSG(f,s,m) { if (!(f)) { printf((s),(m)); SCE_BREAK(); } }

#define DBG_ASSERT_SCE_OK(f,m) { int reta = f; if ((reta)!=SCE_OK) { printf((m),(reta)); SCE_BREAK(); } }

// Program Entry Point
int main()
{
	DBG_ASSERT(sceSysmoduleLoadModule(SCE_SYSMODULE_WEB_BROWSER_DIALOG) == SCE_OK);

	//start common dialog
	DBG_ASSERT(sceCommonDialogInitialize() == SCE_OK);

	int32_t ret = sceWebBrowserDialogInitialize();
	DBG_ASSERT(ret == SCE_OK);

	ret = sceUserServiceInitialize(NULL);
	DBG_ASSERT(ret == SCE_OK);

	char url[SCE_WEB_BROWSER_DIALOG_URL_SIZE];
	memset(url, 0x0, sizeof(url));
	strncpy(url, "http://www.scei.co.jp", SCE_WEB_BROWSER_DIALOG_URL_SIZE);

	SceWebBrowserDialogParam param;
	sceWebBrowserDialogParamInitialize(&param);
	param.url = url;
	ret = sceUserServiceGetInitialUser(&param.userId);
	param.mode = SCE_WEB_BROWSER_DIALOG_MODE_DEFAULT;
	DBG_ASSERT(ret == SCE_OK);

	DBG_ASSERT(sceWebBrowserDialogOpen(&param) >= 0);

	bool need_close = false;
	while (1)
	{
		int stat = sceWebBrowserDialogUpdateStatus();
		if (stat == SCE_COMMON_DIALOG_STATUS_FINISHED)
		{
			break;
		}
		else
		if (stat == SCE_COMMON_DIALOG_STATUS_RUNNING)
		{
			// Add code to close dialog given some boolean test
			if (need_close)
			{
				sceWebBrowserDialogClose();
				break;
			}

		}

	}// End While (..)

	SceWebBrowserDialogResult result;
	memset(&result, 0, sizeof(result));
	sceWebBrowserDialogGetResult(&result);

	// Tidy up - terminate browser
	sceWebBrowserDialogTerminate();

	return 0;
}// End main (..)