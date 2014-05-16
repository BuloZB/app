#include "framework.h"
#include <malloc.h>


// interesting function
/*bool CLASS_DECL_BASE __custom_log_font(UINT nIDS, LOGFONTW* pLogFont)
{
   ENSURE_ARG(pLogFont != NULL);
   ASSERT(nIDS != 0);

   char szFontInfo[256];
   if (!::core::LoadString(nIDS, szFontInfo,_countof(szFontInfo)))
      return FALSE;

   LPTSTR lpszSize = _tcschr(szFontInfo, '\n');
   if (lpszSize != NULL)
   {
      // get point size and convert to pixels
      pLogFont->lfHeight = _ttoi(lpszSize+1);
      pLogFont->lfHeight =
         MulDiv(pLogFont->lfHeight, afxData.cyPixelsPerInch, 72);
      *lpszSize = '\0';
   }
   ::core::tcsncpy_s(pLogFont->lfFaceName, _countof(pLogFont->lfFaceName), szFontInfo, _TRUNCATE);
   return TRUE;
}*/

bool CLASS_DECL_BASE __is_combo_box_control(oswindow oswindow, UINT nStyle)
{
   if (oswindow == NULL)
      return FALSE;
   // do cheap style compare first
   if ((UINT)(::GetWindowLong(oswindow, GWL_STYLE) & 0x0F) != nStyle)
      return FALSE;

   // do expensive classname compare next
   char szCompare[_countof("combobox")+1];
   ::GetClassName(oswindow, szCompare, _countof(szCompare));
   return ::__invariant_stricmp(szCompare, "combobox") == 0;
}

bool CLASS_DECL_BASE __compare_class_name(oswindow oswindow, const char * lpszClassName)
{
   ASSERT(::IsWindow(oswindow));
   char szTemp[32];
   ::GetClassName(oswindow, szTemp, _countof(szTemp));
   return ::__invariant_stricmp(szTemp, lpszClassName) == 0;
}

oswindow CLASS_DECL_BASE __child_window_from_point(oswindow oswindow, POINT pt)
{
   ASSERT(oswindow != NULL);

   // check child windows
   ::ClientToScreen(oswindow, &pt);
   ::oswindow oswindow_Child = ::GetWindow(oswindow, GW_CHILD);
   for (; oswindow_Child != NULL; oswindow_Child = ::GetWindow(oswindow_Child, GW_HWNDNEXT))
   {
      if (__get_dialog_control_id(oswindow_Child) != (WORD)0 &&
         (::GetWindowLong(oswindow_Child, GWL_STYLE) & WS_VISIBLE))
      {
         // see if point hits the child window
         rect rect;
         ::GetWindowRect(oswindow_Child, rect);
         if (rect.contains(pt))
            return oswindow_Child;
      }
   }

   return NULL;    // not found
}

void CLASS_DECL_BASE __set_window_text(sp(::user::interaction) oswindow_Ctrl, const char * lpszNew)
{
   oswindow_Ctrl->SetWindowText(lpszNew);
   /*ENSURE(oswindow_Ctrl);
   ENSURE(lpszNew);

   int32_t nNewLen = lstrlen(lpszNew);
   char szOld[256]="";
   // fast check to see if text really changes (reduces flash in controls)
   if (nNewLen > _countof(szOld) ||
      ::GetWindowText(oswindow_Ctrl, szOld, _countof(szOld)) != nNewLen ||
      lstrcmp(szOld, lpszNew) != 0)
   {
      // change it
      ::SetWindowText(oswindow_Ctrl, lpszNew);
   }*/
}

void CLASS_DECL_BASE __delete_object(HGDIOBJ* pObject)
{
   ENSURE_ARG(pObject != NULL);   
   if (*pObject != NULL)
   {
      ::DeleteObject(*pObject);
      *pObject = NULL;
   }
}
/*
void CLASS_DECL_BASE __cancel_modes(oswindow oswindow_Rcvr)
{
   // if we receive a message destined for a window, cancel any combobox
   //  popups that could be in toolbars or dialog bars
   oswindow oswindow_Cancel = ::GetFocus();
   if (oswindow_Cancel == NULL)
      return;     // nothing to cancel

   if (oswindow_Cancel == oswindow_Rcvr)
      return;     // let input go to window with focus

   // focus is in part of a combo-box
   if (!__is_combo_box_control(oswindow_Cancel, (UINT)CBS_DROPDOWNLIST))
   {
      // check as a dropdown
      oswindow_Cancel = ::get_parent(oswindow_Cancel);   // parent of edit is combo
      if (oswindow_Cancel == oswindow_Rcvr)
         return;     // let input go to part of combo

      if (!__is_combo_box_control(oswindow_Cancel, (UINT)CBS_DROPDOWN))
         return;     // not a combo-box that is active
   }

   // combo-box is active, but if receiver is a popup, do nothing
   if (oswindow_Rcvr != NULL &&
     (::GetWindowLong(oswindow_Rcvr, GWL_STYLE) & WS_CHILD) != 0 &&
     ::get_parent(oswindow_Rcvr) == ::GetDesktopWindow())
      return;
  
   // finally, we should cancel the mode!
   ::SendMessage(oswindow_Cancel, CB_SHOWDROPDOWN, FALSE, 0L);
}*/

void CLASS_DECL_BASE __global_free(HGLOBAL hGlobal)
{
   if (hGlobal == NULL)
      return;

   // avoid bogus warning error messages from various debugging tools
   ASSERT(GlobalFlags(hGlobal) != GMEM_INVALID_HANDLE);
   UINT nCount = GlobalFlags(hGlobal) & GMEM_LOCKCOUNT;
   while (nCount--)
      GlobalUnlock(hGlobal);

   // finally, really free the handle
   GlobalFree(hGlobal);
}




