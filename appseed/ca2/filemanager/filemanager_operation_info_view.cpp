#include "framework.h"


void DoBar(::ca::graphics * pdc, int ileft, int iTop, int cx, int cy, double dAnime)
{
   int iDeltaDark = 23;
      int iDeltaVermelho = 77;
      int iDeltaAzul = 84;
      int iDeltaV1 = 23;
      int iDeltaV2 = 23;
      int iW = 49;
      int x = ileft;
      double dSoft = 184.6;
      int iRight = ileft + cx;
      int iMaxW = iRight - iW;
      COLORREF cr;
      for(x = ileft; x < iMaxW; x+=iW)
      {
       cr = RGB(
         255 - iDeltaVermelho - iDeltaDark,
         ( 255 - (iDeltaV2 / 2.0) +(int) (sin((double)x / dSoft + dAnime)  *( iDeltaV2 / 2.0))) - iDeltaV1 - iDeltaDark,
         255 - iDeltaAzul - 23 - iDeltaDark);
         pdc->FillSolidRect(x, iTop, iW, cy, cr);
      }
      if(x < iRight)
      {
       cr = RGB(
         255 - iDeltaVermelho - iDeltaDark,
         ( 255 - (iDeltaV2 / 2.0) +(int) (sin((double)x / dSoft + dAnime)  *( iDeltaV2 / 2.0))) - iDeltaV1 - iDeltaDark,
         255 - iDeltaAzul - 23 - iDeltaDark);
         pdc->FillSolidRect(x, iTop, iRight - x, cy, cr);
      }
}


file_manager_operation_info_view::file_manager_operation_info_view(::ca::application * papp) :
   ca(papp),
   ::userbase::view(papp)
{
   m_dAnime = 0.0;
}


void file_manager_operation_info_view::OnDraw(::ca::graphics * pdc)
{
   UNREFERENCED_PARAMETER(pdc);
}

void file_manager_operation_info_view::_001OnDraw(::ca::graphics * pdc)
{
      /*::FillRect(hdc, &rectProgress, g_hbrushProgress3);
      rectProgress.left++;
      rectProgress.right--;
      rectProgress.top++;
      rectProgress.bottom--;*/
   rect rectClient;
   GetClientRect(rectClient);
      int iLineCount = 23;
      double dBarHeight = (double) rectClient.height() / (double) iLineCount;
      double dTop = 0.0;
      RECT rectProgress;
      rectProgress = rectClient;
      //rectProgress.left += 23;
      //rectProgress.right -= 23;
      rectProgress.top += (LONG) dTop;
      rectProgress.bottom = (LONG) (dTop + dBarHeight);
      double dProgressL = 0.0;
      double dProgressU;
      double dProgressD = 1.0 / (double) iLineCount;
      RECT rectBar;
      double dProgress;
      dProgress = get_document()->m_thread.get_progress_rate();
      for(int iLine = 0; iLine < iLineCount; iLine++)
      {

         rectBar = rectProgress;
         dProgressU = dProgressL + dProgressD;
         if(dProgress < dProgressU)
         {
            pdc->FillSolidRect(&rectProgress, RGB(255, 240, 200));
         }
         if(dProgress > dProgressL)
         {
            if(dProgress < dProgressU)
            {
               rectBar.right = ((int) ((rectProgress.right - rectProgress.left) * (dProgress - dProgressL) * ((double) iLineCount) )) + rectProgress.left;
            }
            DoBar(pdc, rectBar.left, rectBar.top,
               rectBar.right - rectBar.left, rectBar.bottom - rectBar.top, m_dAnime);
         }
         dTop += dBarHeight;
         rectProgress.top = (LONG) dTop;
         rectProgress.bottom = (LONG) (dTop + dBarHeight);
         dProgressL = dProgressU;
      }
      //::FillRect(hdc, &rectProgress, g_hbrushProgress1);

}

void file_manager_operation_info_view::OnFileOperationStep(int iOperation, bool bFinal)
{
   UNREFERENCED_PARAMETER(iOperation);
   UNREFERENCED_PARAMETER(bFinal);
}

file_manager_operation_document * file_manager_operation_info_view::get_document()
{
   return dynamic_cast < file_manager_operation_document * > (::userbase::view::get_document());
}

void file_manager_operation_info_view::_001OnTimer(gen::signal_object * pobj)
{
   SCAST_PTR(::gen::message::timer, ptimer, pobj);
   if(ptimer->m_nIDEvent == 123)
   {
   }
}

void file_manager_operation_info_view::_001OnCreate(gen::signal_object * pobj)
{
   UNREFERENCED_PARAMETER(pobj);
   //SetTimer(123, 184, NULL);
}

void file_manager_operation_info_view::install_message_handling(::gen::message::dispatch * pinterface)
{
   ::userbase::view::install_message_handling(pinterface);
   IGUI_WIN_MSG_LINK(WM_CREATE, pinterface, this, &file_manager_operation_info_view::_001OnCreate);
   IGUI_WIN_MSG_LINK(WM_TIMER, pinterface, this, &file_manager_operation_info_view::_001OnTimer);
}

void file_manager_operation_info_view::assert_valid() const
{
   ::userbase::view::assert_valid();
}
void file_manager_operation_info_view::dump(dump_context & context) const
{
   ::userbase::view::dump(context);
}

