#include "StdAfx.h"

#include "mixeruserbase_document.h"
#include "mixeruserbase_control_view.h"
#include "mixeruserbase_control_data.h"

#define IDC_MIXER_SLIDER 1000
//#define IDC_LIST 1001
#define IDC_BUTTON_RESERVE 1002
#define ID_VIEW_UPDATE 1003
#define IDI_ALBUM 1004
#define IDS_MIXER_DIALOG_TITLEFORMAT 1006


namespace mixeruserbase
{


   const int control_view::cnListCount = 9;

   control_view::control_view(::ca::application * papp) :
      ca(papp),
      ::userbase::view(papp),
      m_brushBackground(papp)
   {

      m_pdata = new control_data(this);


      m_etranslucency = TranslucencyPresent;

      m_bInitialized = false;
   //   m_iCacheNextIndex = 0;
      m_uiNextSliderID = IDC_MIXER_SLIDER;

      //m_crForeground = GetSysColor(COLOR_BTNTEXT);
      //m_crForeground = RGB(255, 255, 255);
      //m_crForeground = RGB(255, 255, 255)
      //m_crBackground = RGB(58,80,110);
      //m_crBackground = GetSysColor(COLOR_3DFACE);
      m_crBackground = RGB(74, 70, 64);

      //m_brushBackground.CreateSolidBrush(m_crBackground);
      //m_crForeground = RGB(255, 255, 255);

      m_brushBackground->CreateStockObject(NULL_BRUSH);
      m_crForeground = RGB(0, 0, 0);


      m_dwUpdateTime = 0;
      TRACE("AlbumFormView this = %x \n", this);
   }

   control_view::~control_view()
   {
      if(m_pdata != NULL)
      {
         delete m_pdata;
         m_pdata = NULL;
      }
   }

   void control_view::install_message_handling(::user::win::message::dispatch * pinterface)
   {
      ::userbase::view::install_message_handling(pinterface);
      IGUI_WIN_MSG_LINK(WM_SIZE, pinterface, this, &control_view::_001OnSize);
      IGUI_WIN_MSG_LINK(WM_DESTROY, pinterface, this, &control_view::_001OnDestroy);
      IGUI_WIN_MSG_LINK(WM_CREATE, pinterface, this, &control_view::_001OnCreate);
   //   IGUI_WIN_MSG_LINK(WM_TIMER, pinterface, this, &control_view::_001OnTimer);
   //   IGUI_WIN_MSG_LINK(WM_CONTEXTMENU, pinterface, this, &control_view::_001OnContextMenu);
      IGUI_WIN_MSG_LINK(WM_VSCROLL, pinterface, this, &control_view::_001OnVScroll);
      IGUI_WIN_MSG_LINK(WM_CTLCOLOR, pinterface, this, &control_view::_001OnCtlColor);
      IGUI_WIN_MSG_LINK(WM_HSCROLL, pinterface, this, &control_view::_001OnHScroll);

      connect_command(IDC_BUTTON_RESERVE, &control_view::_001OnButtonReserve);
      connect_update_cmd_ui(ID_VIEW_UPDATE, &control_view::_001OnUpdateViewUpdate);

   //   IGUI_WIN_ON_NOTIFY(LVN_COLUMNCLICK  , IDC_LIST  , this, this, &control_view::_001OnColumnclickList);
      //IGUI_WIN_ON_NOTIFY(LVN_GETDISPINFO  , IDC_LIST  , this, this, &control_view::_001OnGetdispinfoList);
      //IGUI_WIN_ON_NOTIFY(NM_DBLCLK        , IDC_LIST  , this, this, &control_view::_001OnDblclkList);
   }


   #ifdef _DEBUG
   void control_view::assert_valid() const
   {
      ::userbase::view::assert_valid();
   }

   void control_view::dump(dump_context & dumpcontext) const
   {
      ::userbase::view::dump(dumpcontext);
   }

   document * control_view::get_document() // non-debug version is inline
   {
      ASSERT(base < document >::bases(::view::get_document()));
      return dynamic_cast < document * > (::view::get_document());
   }
   #endif //_DEBUG


   void control_view::_001OnSize(gen::signal_object * pobj)
   {
      if(!m_bInitialized)
         return;
      pobj->previous();

   }

   void control_view::OnInitialUpdate()
   {
       wait_cursor cwc(get_app());

   }

   void control_view::_001OnGetdispinfoList(gen::signal_object * pobj)
   {
      SCAST_PTR(::user::win::message::notify, pnotify, pobj)

      pnotify->set_lresult(0);
   }

   void control_view::_001OnDestroy(gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void control_view::_001OnColumnclickList(gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }



   void control_view::_001OnDblclkList(gen::signal_object * pobj)
   {
      SCAST_PTR(::user::win::message::notify, pnotify, pobj)
      // TODO: add your control notification handler code here
   //   LPNMLISTVIEW lpnmlv = (LPNMLISTVIEW) pnotify->get_lpnmhdr();
   //   CListCtrl * pList = (CListCtrl *) GetDlgItem(lpnmlv->hdr.idFrom);
      pnotify->set_lresult(0);
   }

   void control_view::_001OnButtonReserve(gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      // TODO: add your control notification handler code here
   }






   void control_view::pre_translate_message(gen::signal_object * pobj)
   {
      SCAST_PTR(::user::win::message::base, pbase, pobj);
      if(pbase->m_uiMessage == MM_MIXM_CONTROL_CHANGE)
       {
         ASSERT(FALSE);
       }
       else if(pbase->m_uiMessage == MM_MIXM_LINE_CHANGE)
       {
         ASSERT(FALSE);
       }
      if(pbase->m_uiMessage == WM_DISPLAYCHANGE)
      {
   //      SetImageLists();
      }

      return ::userbase::view::pre_translate_message(pobj);
   }

   void control_view::_001OnCreate(gen::signal_object * pobj)
   {
      wait_cursor cwc(get_app());
      if(pobj->previous())
         return;

      ::userbase::view::_001OnInitialUpdate(NULL);

      m_bInitialized = true;

   }


   BOOL control_view::PreCreateWindow(CREATESTRUCT& cs)
   {
      cs.style = cs.style;
   //   int i = WS_POPUP;
      cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
      cs.style |= WS_CLIPCHILDREN;
      return ::userbase::view::PreCreateWindow(cs);
   }


   /*void control_view::SetImageLists()
   {
      ::ca::graphics_sp spgraphics(get_app());
      spgraphics->CreateDC(
         "DISPLAY",
         NULL,
         NULL,
         NULL);
       //CTabCtrl * pTab = (CTabCtrl *) GetDlgItem(IDC_TAB);
      if(m_SmallImageList.GetSafeHandle() != NULL)
      {
         //  pTab->SetImageList(NULL);
      }
      int iBitCount = spgraphics->GetDeviceCaps(BITSPIXEL);
      if(iBitCount > 8)
      {
         m_SmallImageListBitmap.delete_object();
         m_SmallImageListBitmap.LoadBitmap(IDB_ALBUM_SMALL_256);
         m_SmallImageListBitmapV001.delete_object();
         m_SmallImageListBitmapV001.LoadBitmap(IDB_ALBUM_SMALL_256);
         m_SmallImageList.create(
            16, 16,
            ILC_COLOR32 |
            ILC_MASK,
            2,
            1);
         m_SmallImageListV001.create(
            16, 16,
            ILC_COLOR32 |
            ILC_MASK,
            2,
            1);
           //m_SmallImageList.SetBkColor(CLR_NONE);
           m_SmallImageList.SetBkColor(GetSysColor(COLOR_WINDOW));
         m_SmallImageList.add(&m_SmallImageListBitmap, RGB(255, 0, 255));
           m_SmallImageListV001.SetBkColor(GetSysColor(COLOR_BTNFACE));
         m_SmallImageListV001.add(&m_SmallImageListBitmapV001, RGB(255, 0, 255));

      }
      else
      {
         m_SmallImageListBitmap.delete_object();
         m_SmallImageListBitmap.LoadBitmap(IDB_ALBUM_SMALL_16);
         m_SmallImageListBitmapV001.delete_object();
         m_SmallImageListBitmapV001.LoadBitmap(IDB_ALBUM_SMALL_16);
         m_SmallImageList.create(
            16, 16,
            ILC_COLOR32 |
            ILC_MASK,
            2,
            1);
         m_SmallImageListV001.create(
            16, 16,
            ILC_COLOR32 |
            ILC_MASK,
            2,
            1);
           m_SmallImageList.SetBkColor(CLR_NONE);
         m_SmallImageList.add(&m_SmallImageListBitmap, RGB(255, 0, 255));
           m_SmallImageListV001.SetBkColor(CLR_NONE);
         m_SmallImageListV001.add(&m_SmallImageListBitmapV001, RGB(255, 0, 255));

      }
      if(m_SmallImageList.GetSafeHandle() != NULL)
      {
      }
      if(m_SmallImageListV001.GetSafeHandle() != NULL)
      {
   //        pTab->SetImageList(&m_SmallImageListV001);

      }


      spgraphics->DeleteDC();
   }
   */

   void control_view::_001OnUpdateViewUpdate(gen::signal_object * pobj)
   {
      SCAST_PTR(::user::win::message::update_cmd_ui, pupdatecmdui, pobj)
      pupdatecmdui->m_pcmdui->Enable();
      pupdatecmdui->m_bRet = true;
   }







   void control_view::on_update(::view * pSender, LPARAM lHint, ::radix::object* pHint)
   {
      UNREFERENCED_PARAMETER(pSender);
      UNREFERENCED_PARAMETER(pHint);
      switch(lHint)
      {
      case 13:
         break;
      case 4:
         break;
      case 14:
         break;
      case 17:
         //New destination set
         {
             set_title();

            MixerDestination * pdestination = GetDestination();
            pdestination->GetAllControls();

            Mixer * pmixer = GetMixer();

            pmixer->m_pdevice->MapLines();
            pmixer->m_pdevice->MapControls();

            CreateControls();
            pmixer->m_pdevice->MapDlgCtrlIDToControls();
            pdestination->SendOnChangeAllControls();

            layout();

         }
         break;
       }

   }

   void control_view::OnDraw(::ca::graphics * pgraphics)
   {
      UNREFERENCED_PARAMETER(pgraphics);
   }

   bool control_view::CreateControls()
   {
      delete_contents();

      CreateControls(GetDestination());

      MixerSourceArray & sourcea = GetDestination()->GetSources();

      for(int i = 0; i < sourcea.get_size(); i++)
      {
         CreateControls(sourcea.ptr_at(i));
      }

      return true;
   }

   bool control_view::CreateControls(MixerSource *pSource)
   {
      bool bHasControls = false;

      MixerControl * pcontrol;
      MixerControlArray & controla = pSource->m_mixercontrola;

      for(int iControl = 0; iControl < controla.get_size(); iControl++)
      {
         pcontrol = &controla[iControl];
         if(pcontrol->m_mixercontrol.dwControlType == MIXERCONTROL_CONTROLTYPE_VOLUME)
         {
            if(CreateVolumeControl(pcontrol))
               bHasControls = true;
         }
         else if((pcontrol->m_mixercontrol.dwControlType & MIXERCONTROL_CT_UNITS_MASK) == MIXERCONTROL_CT_UNITS_BOOLEAN)
         {
            if(CreateMuteControl(pcontrol))
               bHasControls = true;
         }
      }
      if(bHasControls)
      {
         rect rect(0, 0, 0, 0);
         simple_static * pst = new simple_static(get_app());
         pst->create(this, pSource->GetSZName());
         pst->SetWindowTextA(pSource->GetSZName());
         pst->ModifyStyleEx(0, WS_EX_TRANSPARENT, SWP_SHOWWINDOW);
         pst->SetFont(GetFont());
         m_staticsLines.set_at(pSource->GetLineID(), pst);
      }
      return true;
   }

   void control_view::delete_contents()
   {
       DWORD dwKey;
       simple_static * pst;
       POSITION pos = m_staticsLines.get_start_position();
       while(pos != NULL)
       {
           m_staticsLines.get_next_assoc(pos, dwKey, pst);
           delete pst;
       }
       m_staticsLines.remove_all();

   }

   bool control_view::CreateVolumeControl(MixerControl *pControl)
   {
       bool ok = pControl->CreateWindowsVolumeV001(this, m_uiNextSliderID, &m_uiNextSliderID);
       return ok;
   }

   bool control_view::CreateMuteControl(MixerControl *pControl)
   {
       bool ok = pControl->_001CreateMuteControl(
          this,
          m_uiNextSliderID,
          &m_uiNextSliderID);
       return ok;

   }



   void control_view::layout()
   {
       int iStartX = 0;
       int iEndX = 0;
       int iStartY = 0;
       int iEndY = 0;

       if(GetDestination() == NULL)
          return;

       LayoutLine(-1, GetDestination(), iStartX, iStartY, &iEndX, &iEndY);
       iStartX = iEndX;

       MixerSourceArray & sourcea = GetDestination()->GetSources();

       for(int i = 0; i < sourcea.get_size(); i++)
       {
           LayoutLine(i, sourcea.ptr_at(i), iStartX, iStartY, &iEndX, &iEndY);
           iStartX = iEndX;
       }
       //rect rect(0, 0, iEndX, iEndY);
       //::AdjustWindowRectEx(&rect, GetStyle(), FALSE,
      //      GetExStyle());
       //SetWindowPos(NULL, 0, 0, rect.width(), rect.height(), SWP_NOMOVE | SWP_NOACTIVATE);
       //rect rectClient;
       //GetClientRect(rectClient);

      _001RedrawWindow();

   }

   void control_view::set_title()
   {
       string strFormat;
   //    strFormat.load_string(IDS_MIXER_DIALOG_TITLEFORMAT);

       string strAppTitle;
       //strAppTitle.load_string(AFX_IDS_APP_TITLE);

       string strTitle;
       ASSERT(GetDestination() != NULL);
       strTitle.Format(
           strFormat,
           strAppTitle,
           GetMixer()->m_pdevice->GetSZPName(),
           GetDestination()->GetSZName());

       SetWindowText(strTitle);

   }

   void control_view::LayoutLine(int iSource, MixerSource *pSource, int iStartX, int iStartY, int *piEndX, int *piEndY)
   {
      MixerControlArray & controla = pSource->m_mixercontrola;
      MixerControl * pcontrol;
      mixer::control * pWnd;
      bool bHasAtLeastOneControl = false;
      int y = iStartY;
      int y2;
      y += 2;
      simple_static * pst;
      if(m_staticsLines.Lookup(pSource->GetLineID(), pst))
      {
         pst->SetWindowPos(
            NULL,
            iStartX, y,
            70, 30,
            SWP_SHOWWINDOW);
      }
      y += 30;
      for(int iControl = 0; iControl < controla.get_size(); iControl++)
      {
         pcontrol = &controla[iControl];
         if(pcontrol->m_mixercontrol.dwControlType == MIXERCONTROL_CONTROLTYPE_VOLUME)
         {
            if(pcontrol->get_size() >= 2)
            {
               pWnd = pcontrol->GetControl(MixerCtrlData::TypeStereoBalanceLabel);
               y2 = y;
               if(pWnd != NULL)
               {
                  bHasAtLeastOneControl = true;
                  pWnd->GetWnd()->SetWindowPos(
                     NULL,
                     iStartX, y2,
                     80, 15,
                     SWP_SHOWWINDOW);
                  y2 += 15;
                  pWnd = pcontrol->GetControl(MixerCtrlData::TypeStereoBalance);
                  if(pWnd != NULL)
                  {
                     pWnd->GetWnd()->SetWindowPos(
                        NULL,
                        iStartX, y,
                        70, 30,
                        SWP_SHOWWINDOW);
                  }
               }
               y += 45;
               y2 = y;
               pWnd = pcontrol->GetControl(MixerCtrlData::TypeStereoVolumeLabel);
               if(pWnd != NULL)
               {
                  bHasAtLeastOneControl = true;
                  pWnd->GetWnd()->SetWindowPos(
                     NULL,
                     iStartX, y2,
                     80, 15,
                     SWP_SHOWWINDOW);
                  y2 += 15;
                  pWnd = pcontrol->GetControl(MixerCtrlData::TypeStereoVolume);
                  if(pWnd != NULL)
                  {
                     bHasAtLeastOneControl = true;
                     pWnd->GetWnd()->SetWindowPos(
                        NULL,
                        iStartX, y,
                        50, 120,
                        SWP_SHOWWINDOW);
                  }
               }
               y += 135;
            }

         }
      }
      for(int iControl = 0; iControl < controla.get_size(); iControl++)
      {
         pcontrol = &controla[iControl];
         if(pcontrol->m_mixercontrol.dwControlType == MIXERCONTROL_CONTROLTYPE_MUX)
         {
            m_controlptraMux.add_unique(pcontrol);
         }
         if((pcontrol->m_mixercontrol.dwControlType & MIXERCONTROL_CT_UNITS_MASK) == MIXERCONTROL_CT_UNITS_BOOLEAN)
         {
            int iSize = pcontrol->get_size();
            for(int i = 0; i < iSize; i++)
            {
               pWnd = pcontrol->GetControlByIndex(i);
               bHasAtLeastOneControl = true;
               pWnd->GetWnd()->SetWindowPos(
                  NULL,
                  iStartX, y,
                  80, 30,
                  SWP_SHOWWINDOW);
               y += 30;
            }
         }
      }
      if(iSource >= 0)
      {
         for(int iControl = 0; iControl < m_controlptraMux.get_size(); iControl++)
         {
            pcontrol = m_controlptraMux[iControl];
            if((pcontrol->m_mixercontrol.dwControlType & MIXERCONTROL_CT_UNITS_MASK) == MIXERCONTROL_CT_UNITS_BOOLEAN)
            {
               pWnd = pcontrol->GetControlByIndex(iSource);
               bHasAtLeastOneControl = true;
               pWnd->GetWnd()->SetWindowPos(
                  NULL,
                  iStartX, y,
                  80, 30,
                  SWP_SHOWWINDOW);
               y += 30;
            }
         }
      }
      if( bHasAtLeastOneControl)
      {
         *piEndX = iStartX + 80;
         *piEndY = 300;
      }
   }


   void control_view::_001OnVScroll(gen::signal_object * pobj)
   {
      IGUI_WIN_CST(scroll);
      ::user::interaction* pWnd = pscroll->m_pScrollBar;
      MixerControl * pControl;
      if(GetMixer()->m_pdevice->m_mapDlgItemIDToControl.Lookup(pWnd->GetDlgCtrlId(), pControl))
      {
         pControl->OnVHScroll(pscroll->m_nSBCode, (uint) pscroll->m_nPos, pscroll->m_pScrollBar);
      }
   }

   void control_view::_001OnCtlColor(gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   //   SCAST_PTR(::user::win::message::ctl_color, pctlcolor, pobj)
      throw not_implemented_exception();
   //   HBRUSH hbr = ::userbase::view::OnCtlColor(pgraphics, pWnd, nCtlColor);
      /*HBRUSH hbr;
      if(pctlcolor->m_nCtlType == CTLCOLOR_EDIT)
      {
         pctlcolor->m_pdc->SetTextColor(m_crForeground);
         hbr = m_brushBackground;
         pctlcolor->m_pdc->SetBkColor(m_crBackground);
      }
      else if(pctlcolor->m_nCtlType == CTLCOLOR_MSGBOX)
      {
         hbr = m_brushBackground;
      }
      else if(pctlcolor->m_nCtlType == CTLCOLOR_STATIC)
      {
         pctlcolor->m_pdc->SetTextColor(m_crForeground);
         hbr = m_brushBackground;
         pctlcolor->m_pdc->SetBkColor(m_crBackground);
         pctlcolor->m_pdc->SetBkMode(TRANSPARENT);
      }
      else if(pctlcolor->m_nCtlType == CTLCOLOR_DLG)
      {
   //      pgraphics->SetTextColor(RGB(255, 255, 255));
   //      hbr = m_brushBackground;
   //      pgraphics->SetBkColor(RGB(0, 0, 0));
      }
      else
      {
   // trans      pctlcolor->m_hbrush = ::userbase::view::OnCtlColor(pctlcolor->m_pdc, pctlcolor->m_pwnd, pctlcolor->m_nCtlType);
      }*/
   }

   void control_view::_001OnHScroll(gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
      //SCAST_PTR(::user::win::message::scroll, pscroll, pobj)
   }


   Mixer * control_view::GetMixer()
   {
      return get_data()->GetMixer();
   }

   MixerDestination * control_view::GetDestination()
   {
      return get_data()->GetDestination();
   }

   BOOL control_view::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
   {
      UNREFERENCED_PARAMETER(wParam);
      UNREFERENCED_PARAMETER(lParam);
      UNREFERENCED_PARAMETER(pResult);
      return FALSE;
   }

   BOOL control_view::OnCommand(WPARAM wParam, LPARAM lParam)
   {
      if(GetMixer()->OnCommand(wParam, lParam))
         return TRUE;

      return FALSE;
   }

   control_data * control_view::get_data()
   {
      return m_pdata;
   }

   void control_view::_OnUpdate(::userbase::view * pview, LPARAM lhint, ::radix::object * phint)
   {
      on_update(pview, lhint, phint);
   }


} // namespace mixeruserbase


