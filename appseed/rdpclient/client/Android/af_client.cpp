/**
* FreeRDP: A Remote Desktop Protocol Implementation
* Windows Client
*
* Copyright 2009-2011 Jay Sorg
* Copyright 2010-2011 Vic Lee
* Copyright 2010-2011 Marc-Andre Moreau <marcandre.moreau@gmail.com>
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*	 http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <winpr/windows.h>

#include <winpr/crt.h>
#include <winpr/credui.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <tchar.h>
#include <assert.h>
#include <sys/types.h>

#include <freerdp/log.h>
#include <freerdp/event.h>
#include <freerdp/freerdp.h>
#include <freerdp/constants.h>

#include <freerdp/codec/region.h>
#include <freerdp/client/cmdline.h>
#include <freerdp/client/channels.h>
#include <freerdp/channels/channels.h>

//#include "af_gdi.h"
//#include "af_rail.h"
#include "af_channels.h"
//#include "af_graphics.h"
//#include "af_cliprdr.h"

#include "af_client.h"

//#include "resource.h"

#define TAG CLIENT_TAG("windows")

//int af_create_console(void)
//{
//   if(!AllocConsole())
//      return 1;
//
//   freopen("CONOUT$","w",stdout);
//   freopen("CONOUT$","w",stderr);
//   WLog_INFO(TAG,"Debug console created.");
//   return 0;
//}

//BOOL af_sw_begin_paint(wfContext* wfc)
//{
//   rdpGdi* gdi = ((rdpContext*)wfc)->gdi;
//   gdi->primary->hdc->hwnd->invalid->null = 1;
//   gdi->primary->hdc->hwnd->ninvalid = 0;
//   return TRUE;
//}
//
//BOOL af_sw_end_paint(wfContext* wfc)
//{
//   int i;
//   rdpGdi* gdi;
//   int ninvalid;
//   RECT updateRect;
//   HGDI_RGN cinvalid;
//   REGION16 invalidRegion;
//   RECTANGLE_16 invalidRect;
//   const RECTANGLE_16* extents;
//   rdpContext* context = (rdpContext*)wfc;
//
//   gdi = context->gdi;
//
//   ninvalid = gdi->primary->hdc->hwnd->ninvalid;
//   cinvalid = gdi->primary->hdc->hwnd->cinvalid;
//
//   if(ninvalid < 1)
//      return TRUE;
//
//   region16_init(&invalidRegion);
//
//   for(i = 0; i < ninvalid; i++)
//   {
//      invalidRect.left = cinvalid[i].x;
//      invalidRect.top = cinvalid[i].y;
//      invalidRect.right = cinvalid[i].x + cinvalid[i].w;
//      invalidRect.bottom = cinvalid[i].y + cinvalid[i].h;
//
//      region16_union_rect(&invalidRegion,&invalidRegion,&invalidRect);
//   }
//
//   if(!region16_is_empty(&invalidRegion))
//   {
//      extents = region16_extents(&invalidRegion);
//
//      updateRect.left = extents->left;
//      updateRect.top = extents->top;
//      updateRect.right = extents->right;
//      updateRect.bottom = extents->bottom;
//
//      InvalidateRect(wfc->hwnd,&updateRect,FALSE);
//
//      if(wfc->rail)
//         af_rail_invalidate_region(wfc,&invalidRegion);
//   }
//
//   region16_uninit(&invalidRegion);
//   return TRUE;
//}

//BOOL af_sw_desktop_resize(wfContext* wfc)
//{
//	rdpGdi* gdi;
//	rdpContext* context;
//	rdpSettings* settings;
//	freerdp* instance = wfc->instance;
//
//	context = (rdpContext*) wfc;
//	settings = wfc->instance->settings;
//	gdi = context->gdi;
//
//	wfc->width = settings->DesktopWidth;
//	wfc->height = settings->DesktopHeight;
//
//	gdi->primary->bitmap->data = NULL;
//	gdi_free(instance);
//
//	if (wfc->primary)
//	{
//		af_image_free(wfc->primary);
//		wfc->primary = af_image_new(wfc, wfc->width, wfc->height, wfc->dstBpp, NULL);
//	}
//
//	if (!gdi_init(instance, CLRCONV_ALPHA | CLRBUF_32BPP, wfc->primary->pdata))
//		return FALSE;
//
//	gdi = instance->context->gdi;
//	wfc->hdc = gdi->primary->hdc;
//
//	return TRUE;
//}
//
//BOOL af_hw_begin_paint(wfContext* wfc)
//{
//	wfc->hdc->hwnd->invalid->null = 1;
//	wfc->hdc->hwnd->ninvalid = 0;
//	return TRUE;
//}
//
//BOOL af_hw_end_paint(wfContext* wfc)
//{
//	return TRUE;
//}
//
//BOOL af_hw_desktop_resize(wfContext* wfc)
//{
//	BOOL same;
//	RECT rect;
//	rdpSettings* settings;
//
//	settings = wfc->instance->settings;
//
//	wfc->width = settings->DesktopWidth;
//	wfc->height = settings->DesktopHeight;
//
//	if (wfc->primary)
//	{
//		same = (wfc->primary == wfc->drawing) ? TRUE : FALSE;
//
//		af_image_free(wfc->primary);
//
//		wfc->primary = af_image_new(wfc, wfc->width, wfc->height, wfc->dstBpp, NULL);
//
//		if (same)
//			wfc->drawing = wfc->primary;
//	}
//
//	if (wfc->fullscreen != TRUE)
//	{
//		if (wfc->hwnd)
//			SetWindowPos(wfc->hwnd, HWND_TOP, -1, -1, wfc->width + wfc->diff.x, wfc->height + wfc->diff.y, SWP_NOMOVE);
//	}
//	else
//	{
//		af_update_offset(wfc);
//		GetWindowRect(wfc->hwnd, &rect);
//		InvalidateRect(wfc->hwnd, &rect, TRUE);
//	}
//	return TRUE;
//}


//void af_add_system_menu(wfContext* wfc)
//{
//	HMENU hMenu = GetSystemMenu(wfc->hwnd, FALSE);
//
//	MENUITEMINFO item_info;
//	ZeroMemory(&item_info, sizeof(MENUITEMINFO));
//
//	item_info.fMask = MIIM_CHECKMARKS | MIIM_FTYPE | MIIM_ID | MIIM_STRING | MIIM_DATA;
//	item_info.cbSize = sizeof(MENUITEMINFO);
//	item_info.wID = SYSCOMMAND_ID_SMARTSIZING;
//	item_info.fType = MFT_STRING;
//	item_info.dwTypeData = _wcsdup(_T("Smart sizing"));
//	item_info.cch = (UINT) _wcslen(_T("Smart sizing"));
//	item_info.dwItemData = (ULONG_PTR) wfc;
//
//	InsertMenuItem(hMenu, 6, TRUE, &item_info);
//
//	if (wfc->instance->settings->SmartSizing)
//	{
//		CheckMenuItem(hMenu, SYSCOMMAND_ID_SMARTSIZING, MF_CHECKED);
//	}
//}

//BOOL af_post_connect(freerdp* instance)
//{
//	rdpGdi* gdi;
//	DWORD dwStyle;
//	rdpCache* cache;
//	wfContext* wfc;
//	rdpContext* context;
//	WCHAR lpWindowName[64];
//	rdpSettings* settings;
//	EmbedWindowEventArgs e;
//
//	settings = instance->settings;
//	context = instance->context;
//	wfc = (wfContext*) instance->context;
//	cache = instance->context->cache;
//
//	wfc->dstBpp = 32;
//	wfc->width = settings->DesktopWidth;
//	wfc->height = settings->DesktopHeight;
//
//	if (settings->SoftwareGdi)
//	{
//		wfc->primary = af_image_new(wfc, wfc->width, wfc->height, wfc->dstBpp, NULL);
//
//		if (!gdi_init(instance, CLRCONV_ALPHA | CLRBUF_32BPP, wfc->primary->pdata))
//			return FALSE;
//
//		gdi = instance->context->gdi;
//		wfc->hdc = gdi->primary->hdc;
//	}
//	else
//	{
//		af_gdi_register_update_callbacks(instance->update);
//		wfc->srcBpp = instance->settings->ColorDepth;
//		wfc->primary = af_image_new(wfc, wfc->width, wfc->height, wfc->dstBpp, NULL);
//
//		if (!(wfc->hdc = gdi_GetDC()))
//			return FALSE;
//
//		wfc->hdc->bitsPerPixel = wfc->dstBpp;
//		wfc->hdc->bytesPerPixel = wfc->dstBpp / 8;
//
//		wfc->hdc->alpha = wfc->clrconv->alpha;
//		wfc->hdc->invert = wfc->clrconv->invert;
//
//		wfc->hdc->hwnd = (HGDI_WND) malloc(sizeof(GDI_WND));
//		wfc->hdc->hwnd->invalid = gdi_CreateRectRgn(0, 0, 0, 0);
//		wfc->hdc->hwnd->invalid->null = 1;
//
//		wfc->hdc->hwnd->count = 32;
//		wfc->hdc->hwnd->cinvalid = (HGDI_RGN) malloc(sizeof(GDI_RGN) * wfc->hdc->hwnd->count);
//		wfc->hdc->hwnd->ninvalid = 0;
//
//		if (settings->RemoteFxCodec)
//		{
//			wfc->tile = af_image_new(wfc, 64, 64, 32, NULL);
//		}
//	}
//
//	if (settings->WindowTitle != NULL)
//		_snwprintf(lpWindowName, ARRAYSIZE(lpWindowName), L"%S", settings->WindowTitle);
//	else if (settings->ServerPort == 3389)
//		_snwprintf(lpWindowName, ARRAYSIZE(lpWindowName), L"FreeRDP: %S", settings->ServerHostname);
//	else
//		_snwprintf(lpWindowName, ARRAYSIZE(lpWindowName), L"FreeRDP: %S:%d", settings->ServerHostname, settings->ServerPort);
//
//	if (settings->EmbeddedWindow)
//		settings->Decorations = FALSE;
//
//	if (wfc->fullscreen)
//		dwStyle = WS_POPUP;
//	else if (!settings->Decorations)
//		dwStyle = WS_CHILD | WS_BORDER;
//	else
//		dwStyle = WS_CAPTION | WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX | WS_SIZEBOX | WS_MAXIMIZEBOX;
//
//	if (!wfc->hwnd)
//	{
//		wfc->hwnd = CreateWindowEx((DWORD) NULL, wfc->wndClassName, lpWindowName, dwStyle,
//			0, 0, 0, 0, wfc->hWndParent, NULL, wfc->hInstance, NULL);
//
//		SetWindowLongPtr(wfc->hwnd, GWLP_USERDATA, (LONG_PTR) wfc);
//	}
//
//	af_resize_window(wfc);
//
//	af_add_system_menu(wfc);
//
//	BitBlt(wfc->primary->hdc, 0, 0, wfc->width, wfc->height, NULL, 0, 0, BLACKNESS);
//	wfc->drawing = wfc->primary;
//
//	EventArgsInit(&e, "wfreerdp");
//	e.embed = FALSE;
//	e.handle = (void*) wfc->hwnd;
//	PubSub_OnEmbedWindow(context->pubSub, context, &e);
//
//	ShowWindow(wfc->hwnd, SW_SHOWNORMAL);
//	UpdateWindow(wfc->hwnd);
//
//	if (settings->SoftwareGdi)
//	{
//		instance->update->BeginPaint = (pBeginPaint) af_sw_begin_paint;
//		instance->update->EndPaint = (pEndPaint) af_sw_end_paint;
//		instance->update->DesktopResize = (pDesktopResize) af_sw_desktop_resize;
//	}
//	else
//	{
//		instance->update->BeginPaint = (pBeginPaint) af_hw_begin_paint;
//		instance->update->EndPaint = (pEndPaint) af_hw_end_paint;
//		instance->update->DesktopResize = (pDesktopResize) af_hw_desktop_resize;
//	}
//
//	pointer_cache_register_callbacks(instance->update);
//	af_register_pointer(context->graphics);
//
//	if (!settings->SoftwareGdi)
//	{
//		brush_cache_register_callbacks(instance->update);
//		bitmap_cache_register_callbacks(instance->update);
//		offscreen_cache_register_callbacks(instance->update);
//		af_register_graphics(context->graphics);
//		instance->update->BitmapUpdate = af_gdi_bitmap_update;
//	}
//
//	if (freerdp_channels_post_connect(context->channels, instance) < 0)
//		return FALSE;
//
//	if (wfc->fullscreen)
//		floatbar_window_create(wfc);
//
//	return TRUE;
//}
//
//static CREDUI_INFOA wfUiInfo =
//{
//	sizeof(CREDUI_INFOA),
//	NULL,
//	"Enter your credentials",
//	"Remote Desktop Security",
//	NULL
//};

//static BOOL af_authenticate_raw(freerdp* instance, const char* title,
//		char** username, char** password, char** domain)
//{
//   //PubSub_Subscribe(instance->context->pubSub,"ConnectionResult",(pEventHandler)rdpclient_connection_result);
//   return freerdp_authenticate(instance,username,password,domain,instance->context->settings->ServerHostname,0);
//
//
//	//BOOL fSave;
//	//DWORD status;
//	//DWORD dwFlags;
//	//char UserName[CREDUI_MAX_USERNAME_LENGTH + 1];
//	//char Password[CREDUI_MAX_PASSWORD_LENGTH + 1];
//	//char User[CREDUI_MAX_USERNAME_LENGTH + 1];
//	//char Domain[CREDUI_MAX_DOMAIN_TARGET_LENGTH + 1];
//
//	//fSave = FALSE;
//	//ZeroMemory(UserName, sizeof(UserName));
//	//ZeroMemory(Password, sizeof(Password));
//	//dwFlags = CREDUI_FLAGS_DO_NOT_PERSIST | CREDUI_FLAGS_EXCLUDE_CERTIFICATES;
//
//	//status = CredUIPromptForCredentialsA(&wfUiInfo, title, NULL, 0,
//	//	UserName, CREDUI_MAX_USERNAME_LENGTH + 1,
//	//	Password, CREDUI_MAX_PASSWORD_LENGTH + 1, &fSave, dwFlags);
//
//	//if (status != NO_ERROR)
//	//{
//	//	WLog_ERR(TAG, "CredUIPromptForCredentials unexpected status: 0x%08X", status);
//	//	return FALSE;
//	//}
//
//	//ZeroMemory(User, sizeof(User));
//	//ZeroMemory(Domain, sizeof(Domain));
//
//	//status = CredUIParseUserNameA(UserName, User, sizeof(User), Domain, sizeof(Domain));
//	////WLog_ERR(TAG, "User: %s Domain: %s Password: %s", User, Domain, Password);
//	//*username = _strdup(User);
//	//if (!(*username))
//	//{
//	//	WLog_ERR(TAG, "strdup failed", status);
//	//	return FALSE;
//	//}
//
//	//if (strlen(Domain) > 0)
//	//	*domain = _strdup(Domain);
//	//else
//	//	*domain = _strdup("\0");
//
//	//if (!(*domain))
//	//{
//	//	free(*username);
//	//	WLog_ERR(TAG, "strdup failed", status);
//	//	return FALSE;
//	//}
//
//	//*password = _strdup(Password);
//	//if (!(*password))
//	//{
//	//	free(*username);
//	//	free(*domain);
//	//	return FALSE;
//	//}
//
//	//return TRUE;
//}
//
//static BOOL af_authenticate(freerdp* instance,
//		char** username, char** password, char** domain)
//{
//	return af_authenticate_raw(instance, instance->settings->ServerHostname,
//			username, password, domain);
//}
//
//static BOOL af_gw_authenticate(freerdp* instance,
//		char** username, char** password, char** domain)
//{
//	char tmp[MAX_PATH];
//
//	sprintf(tmp, sizeof(tmp), "Gateway %s", instance->settings->GatewayHostname);
//	return af_authenticate_raw(instance, tmp, username, password, domain);
//}
//
//BOOL af_verify_certificate(freerdp* instance, char* subject, char* issuer, char* fingerprint)
//{
//#if 0
//	DWORD mode;
//	int read_size;
//	DWORD read_count;
//	TCHAR answer[2];
//	TCHAR* read_buffer;
//	HANDLE input_handle;
//#endif
//	WLog_INFO(TAG, "Certificate details:");
//	WLog_INFO(TAG, "\tSubject: %s", subject);
//	WLog_INFO(TAG, "\tIssuer: %s", issuer);
//	WLog_INFO(TAG, "\tThumbprint: %s", fingerprint);
//	WLog_INFO(TAG, "The above X.509 certificate could not be verified, possibly because you do not have "
//			  "the CA certificate in your certificate store, or the certificate has expired. "
//			  "Please look at the documentation on how to create local certificate store for a private CA.");
//	/* TODO: ask for user validation */
//#if 0
//	input_handle = GetStdHandle(STD_INPUT_HANDLE);
//
//	GetConsoleMode(input_handle, &mode);
//	mode |= ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT;
//	SetConsoleMode(input_handle, mode);
//#endif
//
//	return TRUE;
//}
//
//static BOOL af_auto_reconnect(freerdp* instance)
//{
//	wfContext* wfc = (wfContext *)instance->context;
//
//	UINT32 num_retries = 0;
//	UINT32 max_retries = instance->settings->AutoReconnectMaxRetries;
//
//	/* Only auto reconnect on network disconnects. */
//	if (freerdp_error_info(instance) != 0)
//		return FALSE;
//
//	/* A network disconnect was detected */
//	WLog_ERR(TAG, "Network disconnect!");
//
//	if (!instance->settings->AutoReconnectionEnabled)
//	{
//		/* No auto-reconnect - just quit */
//		return FALSE;
//	}
//
//	/* Perform an auto-reconnect. */
//	for (;;)
//	{
//		/* Quit retrying if max retries has been exceeded */
//		if (num_retries++ >= max_retries)
//			return FALSE;
//
//		/* Attempt the next reconnect */
//		WLog_INFO(TAG,  "Attempting reconnect (%u of %u)", num_retries, max_retries);
//
//		if (freerdp_reconnect(instance))
//		{
//			return TRUE;
//		}
//
//		Sleep(5000);
//	}
//
//	WLog_ERR(TAG, "Maximum reconnect retries exceeded");
//	return FALSE;
//}
//
//void* af_input_thread(void* arg)
//{
//	int status;
//	wMessage message;
//	wMessageQueue* queue;
//	freerdp* instance = (freerdp*) arg;
//
//	assert( NULL != instance);
//
//	status = 1;
//	queue = freerdp_get_message_queue(instance, FREERDP_INPUT_MESSAGE_QUEUE);
//
//	while (MessageQueue_Wait(queue))
//	{
//		while (MessageQueue_Peek(queue, &message, TRUE))
//		{
//			status = freerdp_message_queue_process_message(instance,
//					FREERDP_INPUT_MESSAGE_QUEUE, &message);
//
//			if (!status)
//				break;
//		}
//
//		if (!status)
//			break;
//	}
//
//	ExitThread(0);
//
//	return NULL;
//}

//DWORD WINAPI af_client_thread(LPVOID lpParam)
//{
//	MSG msg;
//	int width;
//	int height;
//	BOOL msg_ret;
//	int quit_msg;
//	DWORD nCount;
//	HANDLE handles[64];
//	wfContext* wfc;
//	freerdp* instance;
//	rdpContext* context;
//	rdpChannels* channels;
//	rdpSettings* settings;
//	BOOL async_input;
//	BOOL async_transport;
//	HANDLE input_thread;
//
//	instance = (freerdp*) lpParam;
//	context = instance->context;
//	wfc = (wfContext*) instance->context;
//
//	if (!freerdp_connect(instance))
//		return 0;
//
//	channels = instance->context->channels;
//	settings = instance->context->settings;
//
//	async_input = settings->AsyncInput;
//	async_transport = settings->AsyncTransport;
//
//	if (async_input)
//	{
//		if (!(input_thread = CreateThread(NULL, 0,
//				(LPTHREAD_START_ROUTINE) af_input_thread,
//				instance, 0, NULL)))
//		{
//			WLog_ERR(TAG, "Failed to create async input thread.");
//			goto disconnect;
//		}
//	}
//
//	while (1)
//	{
//		nCount = 0;
//
//		if (freerdp_focus_required(instance))
//		{
//			af_event_focus_in(wfc);
//			af_event_focus_in(wfc);
//		}
//
//		if (!async_transport)
//		{
//			DWORD tmp = freerdp_get_event_handles(context, &handles[nCount], 64 - nCount);
//
//			if (tmp == 0)
//			{
//				WLog_ERR(TAG, "freerdp_get_event_handles failed");
//				break;
//			}
//
//			nCount += tmp;
//		}
//
//		if (MsgWaitForMultipleObjects(nCount, handles, FALSE, 1000, QS_ALLINPUT) == WAIT_FAILED)
//		{
//			WLog_ERR(TAG, "wfreerdp_run: WaitForMultipleObjects failed: 0x%04X", GetLastError());
//			break;
//		}
//
//		if (!async_transport)
//		{
//			if (!freerdp_check_event_handles(context))
//			{
//				if (af_auto_reconnect(instance))
//					continue;
//
//				WLog_ERR(TAG, "Failed to check FreeRDP file descriptor");
//				break;
//			}
//		}
//
//		if (freerdp_shall_disconnect(instance))
//			break;
//
//		quit_msg = FALSE;
//
//		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
//		{
//			msg_ret = GetMessage(&msg, NULL, 0, 0);
//
//			if (instance->settings->EmbeddedWindow)
//			{
//				if ((msg.message == WM_SETFOCUS) && (msg.lParam == 1))
//				{
//					PostMessage(wfc->hwnd, WM_SETFOCUS, 0, 0);
//				}
//				else if ((msg.message == WM_KILLFOCUS) && (msg.lParam == 1))
//				{
//					PostMessage(wfc->hwnd, WM_KILLFOCUS, 0, 0);
//				}
//			}
//
//			if (msg.message == WM_SIZE)
//			{
//				width = LOWORD(msg.lParam);
//				height = HIWORD(msg.lParam);
//
//				SetWindowPos(wfc->hwnd, HWND_TOP, 0, 0, width, height, SWP_FRAMECHANGED);
//			}
//
//			if ((msg_ret == 0) || (msg_ret == -1))
//			{
//				quit_msg = TRUE;
//				break;
//			}
//
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		}
//
//		if (quit_msg)
//			break;
//	}
//
//	/* cleanup */
//	freerdp_channels_disconnect(channels, instance);
//
//	if (async_input)
//	{
//		wMessageQueue* input_queue;
//		input_queue = freerdp_get_message_queue(instance, FREERDP_INPUT_MESSAGE_QUEUE);
//		if (MessageQueue_PostQuit(input_queue, 0))
//			WaitForSingleObject(input_thread, INFINITE);
//		CloseHandle(input_thread);
//	}
//
//disconnect:
//	freerdp_disconnect(instance);
//	WLog_DBG(TAG, "Main thread exited.");
//
//	ExitThread(0);
//	return 0;
//}
//
//DWORD WINAPI af_keyboard_thread(LPVOID lpParam)
//{
//   MSG msg;
//   BOOL status;
//   wfContext* wfc;
//   HHOOK hook_handle;
//
//   wfc = (wfContext*)lpParam;
//   assert(NULL != wfc);
//
//   hook_handle = SetWindowsHookEx(WH_KEYBOARD_LL,af_ll_kbd_proc,wfc->hInstance,0);
//
//   if(hook_handle)
//   {
//      while((status = GetMessage(&msg,NULL,0,0)) != 0)
//      {
//         if(status == -1)
//         {
//            WLog_ERR(TAG,"keyboard thread error getting message");
//            break;
//         }
//         else
//         {
//            TranslateMessage(&msg);
//            DispatchMessage(&msg);
//         }
//      }
//
//      UnhookWindowsHookEx(hook_handle);
//   }
//   else
//   {
//      WLog_ERR(TAG,"failed to install keyboard hook");
//   }
//
//   WLog_DBG(TAG,"Keyboard thread exited.");
//   ExitThread(0);
//   return (DWORD)NULL;
//}

rdpSettings* freerdp_client_get_settings(rdp_context* wfc)
{
   //return wfc->instance->settings;
   return wfc->settings;
}

//int freerdp_client_focus_in(wfContext* wfc)
//{
//   PostThreadMessage(wfc->mainThreadId,WM_SETFOCUS,0,1);
//   return 0;
//}
//
//int freerdp_client_focus_out(wfContext* wfc)
//{
//   PostThreadMessage(wfc->mainThreadId,WM_KILLFOCUS,0,1);
//   return 0;
//}

//int freerdp_client_set_window_size(wfContext* wfc,int width,int height)
//{
//   WLog_DBG(TAG,"freerdp_client_set_window_size %d, %d",width,height);
//
//   if((width != wfc->client_width) || (height != wfc->client_height))
//   {
//      PostThreadMessage(wfc->mainThreadId,WM_SIZE,SIZE_RESTORED,((UINT)height << 16) | (UINT)width);
//   }
//
//   return 0;
//}
//
//// TODO: Some of that code is a duplicate of af_pre_connect. Refactor?
//int freerdp_client_load_settings_from_rdp_file(wfContext* wfc,char* filename)
//{
//   rdpSettings* settings;
//
//   settings = wfc->instance->settings;
//
//   if(filename)
//   {
//      settings->ConnectionFile = _strdup(filename);
//      if(!settings->ConnectionFile)
//      {
//         return 3;
//      }
//
//      // free old settings file
//      freerdp_client_rdp_file_free(wfc->connectionRdpFile);
//      wfc->connectionRdpFile = freerdp_client_rdp_file_new();
//      WLog_INFO(TAG,"Using connection file: %s",settings->ConnectionFile);
//
//      if(!freerdp_client_parse_rdp_file(wfc->connectionRdpFile,settings->ConnectionFile))
//      {
//         return 1;
//      }
//
//      if(!freerdp_client_populate_settings_from_rdp_file(wfc->connectionRdpFile,settings))
//      {
//         return 2;
//      }
//   }
//
//   return 0;
//}
//
//void af_size_scrollbars(wfContext* wfc,UINT32 client_width,UINT32 client_height)
//{
//   if(wfc->disablewindowtracking)
//      return;
//
//   // prevent infinite message loop
//   wfc->disablewindowtracking = TRUE;
//
//   if(wfc->instance->settings->SmartSizing)
//   {
//      wfc->xCurrentScroll = 0;
//      wfc->yCurrentScroll = 0;
//
//      if(wfc->xScrollVisible || wfc->yScrollVisible)
//      {
//         if(ShowScrollBar(wfc->hwnd,SB_BOTH,FALSE))
//         {
//            wfc->xScrollVisible = FALSE;
//            wfc->yScrollVisible = FALSE;
//         }
//      }
//   }
//   else
//   {
//      SCROLLINFO si;
//      BOOL horiz = wfc->xScrollVisible;
//      BOOL vert = wfc->yScrollVisible;;
//
//      if(!horiz && client_width < wfc->instance->settings->DesktopWidth)
//      {
//         horiz = TRUE;
//      }
//      else if(horiz && client_width >= wfc->instance->settings->DesktopWidth/* - GetSystemMetrics(SM_CXVSCROLL)*/)
//      {
//         horiz = FALSE;
//      }
//
//      if(!vert && client_height < wfc->instance->settings->DesktopHeight)
//      {
//         vert = TRUE;
//      }
//      else if(vert && client_height >= wfc->instance->settings->DesktopHeight/* - GetSystemMetrics(SM_CYHSCROLL)*/)
//      {
//         vert = FALSE;
//      }
//
//      if(horiz == vert && (horiz != wfc->xScrollVisible && vert != wfc->yScrollVisible))
//      {
//         if(ShowScrollBar(wfc->hwnd,SB_BOTH,horiz))
//         {
//            wfc->xScrollVisible = horiz;
//            wfc->yScrollVisible = vert;
//         }
//      }
//
//      if(horiz != wfc->xScrollVisible)
//      {
//         if(ShowScrollBar(wfc->hwnd,SB_HORZ,horiz))
//         {
//            wfc->xScrollVisible = horiz;
//         }
//      }
//
//      if(vert != wfc->yScrollVisible)
//      {
//         if(ShowScrollBar(wfc->hwnd,SB_VERT,vert))
//         {
//            wfc->yScrollVisible = vert;
//         }
//      }
//
//      if(horiz)
//      {
//         // The horizontal scrolling range is defined by
//         // (bitmap_width) - (client_width). The current horizontal
//         // scroll value remains within the horizontal scrolling range.
//         wfc->xMaxScroll = MAX(wfc->instance->settings->DesktopWidth - client_width,0);
//         wfc->xCurrentScroll = MIN(wfc->xCurrentScroll,wfc->xMaxScroll);
//         si.cbSize = sizeof(si);
//         si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
//         si.nMin   = wfc->xMinScroll;
//         si.nMax   = wfc->instance->settings->DesktopWidth;
//         si.nPage  = client_width;
//         si.nPos   = wfc->xCurrentScroll;
//         SetScrollInfo(wfc->hwnd,SB_HORZ,&si,TRUE);
//      }
//
//      if(vert)
//      {
//         // The vertical scrolling range is defined by
//         // (bitmap_height) - (client_height). The current vertical
//         // scroll value remains within the vertical scrolling range.
//         wfc->yMaxScroll = MAX(wfc->instance->settings->DesktopHeight - client_height,0);
//         wfc->yCurrentScroll = MIN(wfc->yCurrentScroll,wfc->yMaxScroll);
//         si.cbSize = sizeof(si);
//         si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
//         si.nMin   = wfc->yMinScroll;
//         si.nMax   = wfc->instance->settings->DesktopHeight;
//         si.nPage  = client_height;
//         si.nPos   = wfc->yCurrentScroll;
//         SetScrollInfo(wfc->hwnd,SB_VERT,&si,TRUE);
//      }
//   }
//
//   wfc->disablewindowtracking = FALSE;
//   //	af_update_canvas_diff(wfc);
//}
//
//BOOL wfreerdp_client_global_init(void)
//{
//   WSADATA wsaData;
//
//   if(!getenv("HOME"))
//   {
//      char home[MAX_PATH * 2] = "HOME=";
//      strcat(home,getenv("HOMEDRIVE"));
//      strcat(home,getenv("HOMEPATH"));
//      _putenv(home);
//   }
//
//   WSAStartup(0x101,&wsaData);
//
//#if defined(WITH_DEBUG) || defined(_DEBUG)
//   af_create_console();
//#endif
//
//   freerdp_register_addin_provider(freerdp_channels_load_static_addin_entry,0);
//   return TRUE;
//}
//
//void wfreerdp_client_global_uninit(void)
//{
//   WSACleanup();
//}
//
////BOOL wfreerdp_client_new(freerdp* instance, rdpContext* context)
////{
////	wfContext* wfc = (wfContext*) context;
////
////	if (!(wfreerdp_client_global_init()))
////		return FALSE;
////
////	if (!(context->channels = freerdp_channels_new()))
////		return FALSE;
////
////	instance->PreConnect = af_pre_connect;
////	instance->PostConnect = af_post_connect;
////	instance->Authenticate = af_authenticate;
////	instance->GatewayAuthenticate = af_gw_authenticate;
////	instance->VerifyCertificate = af_verify_certificate;
////
////	wfc->instance = instance;
////	wfc->settings = instance->settings;
////
////	return TRUE;
////}
//
//void wfreerdp_client_free(freerdp* instance,rdpContext* context)
//{
//   if(!context)
//      return;
//
//   if(context->channels)
//   {
//      freerdp_channels_close(context->channels,instance);
//      freerdp_channels_free(context->channels);
//      context->channels = NULL;
//   }
//
//   if(context->cache)
//   {
//      cache_free(context->cache);
//      context->cache = NULL;
//   }
//}
////
////int wfreerdp_client_start(rdpContext* context)
////{
////	HWND hWndParent;
////	HINSTANCE hInstance;
////	wfContext* wfc = (wfContext*) context;
////	freerdp* instance = context->instance;
////
////	hInstance = GetModuleHandle(NULL);
////	hWndParent = (HWND) instance->settings->ParentWindowId;
////	instance->settings->EmbeddedWindow = (hWndParent) ? TRUE : FALSE;
////
////	wfc->hWndParent = hWndParent;
////	wfc->hInstance = hInstance;
////	wfc->cursor = LoadCursor(NULL, IDC_ARROW);
////	wfc->icon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
////	wfc->wndClassName = _tcsdup(_T("FreeRDP"));
////
////	wfc->wndClass.cbSize = sizeof(WNDCLASSEX);
////	wfc->wndClass.style = CS_HREDRAW | CS_VREDRAW;
////	wfc->wndClass.lpfnWndProc = af_event_proc;
////	wfc->wndClass.cbClsExtra = 0;
////	wfc->wndClass.cbWndExtra = 0;
////	wfc->wndClass.hCursor = wfc->cursor;
////	wfc->wndClass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
////	wfc->wndClass.lpszMenuName = NULL;
////	wfc->wndClass.lpszClassName = wfc->wndClassName;
////	wfc->wndClass.hInstance = hInstance;
////	wfc->wndClass.hIcon = wfc->icon;
////	wfc->wndClass.hIconSm = wfc->icon;
////	RegisterClassEx(&(wfc->wndClass));
////
////	wfc->keyboardThread = CreateThread(NULL, 0, af_keyboard_thread, (void*) wfc, 0, &wfc->keyboardThreadId);
////
////	if (!wfc->keyboardThread)
////		return -1;
////
////	freerdp_client_load_addins(context->channels, instance->settings);
////
////	wfc->thread = CreateThread(NULL, 0, af_client_thread, (void*) instance, 0, &wfc->mainThreadId);
////
////	if (!wfc->thread)
////		return -1;
////
////	return 0;
////}
//
//int wfreerdp_client_stop(rdpContext* context)
//{
//   wfContext* wfc = (wfContext*)context;
//
//   if(wfc->thread)
//   {
//      PostThreadMessage(wfc->mainThreadId,WM_QUIT,0,0);
//
//      WaitForSingleObject(wfc->thread,INFINITE);
//      CloseHandle(wfc->thread);
//      wfc->thread = NULL;
//      wfc->mainThreadId = 0;
//   }
//
//   if(wfc->keyboardThread)
//   {
//      PostThreadMessage(wfc->keyboardThreadId,WM_QUIT,0,0);
//
//      WaitForSingleObject(wfc->keyboardThread,INFINITE);
//      CloseHandle(wfc->keyboardThread);
//
//      wfc->keyboardThread = NULL;
//      wfc->keyboardThreadId = 0;
//   }
//
//   return 0;
//}
//
////int RdpClientEntry(RDP_CLIENT_ENTRY_POINTS* pEntryPoints)
////{
////	pEntryPoints->Version = 1;
////	pEntryPoints->Size = sizeof(RDP_CLIENT_ENTRY_POINTS_V1);
////
////	pEntryPoints->GlobalInit = wfreerdp_client_global_init;
////	pEntryPoints->GlobalUninit = wfreerdp_client_global_uninit;
////
////	pEntryPoints->ContextSize = sizeof(wfContext);
////	pEntryPoints->ClientNew = wfreerdp_client_new;
////	pEntryPoints->ClientFree = wfreerdp_client_free;
////
////	pEntryPoints->ClientStart = wfreerdp_client_start;
////	pEntryPoints->ClientStop = wfreerdp_client_stop;
////
////	return 0;
////}