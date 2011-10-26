#include "StdAfx.h"
#include "kar/KaraokeBouncingBall.h"

namespace mediaplay
{

   view::view(::ca::application * papp) :
      ca(papp),
      gcom::backview::user::interaction(papp),
      gcom::backview::Interface(papp),
      ::userbase::view(papp),
      KaraokeView(papp),
      view_interface(papp),
      midi_player_interface(papp),
      audio::WavePlayerInterface(papp),
      audio::WaveRecorderInterface(papp),
      midi_player_callback(papp)
   {
      m_dataid = "mediaplay::view";
      m_viewlineaStatus.set_size(1);
      m_lyrictemplatelines.set_app(papp);
   }

   view::~view()
   {
   }

   void view::install_message_handling(::user::win::message::dispatch * pinterface)
   {
      ::userbase::view::install_message_handling(pinterface);
      ::mediaplay::view_interface::install_message_handling(pinterface);
      m_plinesTitle->InstallMessageHandling(pinterface);

      IGUI_WIN_MSG_LINK(WM_DESTROY     , pinterface, this, &view::_001OnDestroy);
      IGUI_WIN_MSG_LINK(WM_SIZE        , pinterface, this, &view::_001OnSize);
      IGUI_WIN_MSG_LINK(WM_CREATE      , pinterface, this, &view::_001OnCreate);
      IGUI_WIN_MSG_LINK(WM_CONTEXTMENU , pinterface, this, &view::_001OnContextMenu);
      IGUI_WIN_MSG_LINK(WM_SETCURSOR   , pinterface, this, &view::_001OnSetCursor);

   //   IGUI_WIN_MSG_LINK(WM_USER + 177     , this, this, &view::_001OnTabClick);
      IGUI_WIN_MSG_LINK(WM_APP + 119      , pinterface, this, &view::_001OnWavePlayerEvent);
      //connect_command(ID_FILE_PRINT, ::userbase::view::OnFilePrint)
      //connect_command(ID_FILE_PRINT_DIRECT, ::userbase::view::OnFilePrint)
      //connect_command(ID_FILE_PRINT_PREVIEW, ::userbase::view::OnFilePrintPreview)

      IGUI_WIN_MSG_LINK(
         WM_APP + 111,
         pinterface,
         this,
         &::mediaplay::view_interface::_001OnMessageX);


      IGUI_WIN_MSG_LINK(
         WM_SIZE,
         pinterface,
         this,
         &::mediaplay::view_interface::_001OnSize);

      IGUI_WIN_MSG_LINK(
         WM_TIMER,
         pinterface,
         this,
         &::mediaplay::view_interface::_001OnTimer);

   }

   #ifdef _DEBUG
   void view::assert_valid() const
   {
      ::userbase::view::assert_valid();
   }

   void view::dump(dump_context & dumpcontext) const
   {
      ::userbase::view::dump(dumpcontext);
   }
   #endif //_DEBUG

   BOOL view::PreCreateWindow(CREATESTRUCT& cs)
   {
      cs.lpszClass = System.RegisterWndClass(
         CS_DBLCLKS |
         CS_OWNDC,
         0, 0, 0);
      cs.style &= ~WS_EX_CLIENTEDGE;
      return ::userbase::view::PreCreateWindow(cs);
   }
   void view::_001OnInitialUpdate(gen::signal_object * pobj)
   {
      ::userbase::view::_001OnInitialUpdate(pobj);



   }

   ::user::interaction* view::KaraokeGetWnd()
   {
      return this;
   }


   void view::on_update(::view * pSender, LPARAM lHint, ::radix::object* phint)
   {
      UNREFERENCED_PARAMETER(lHint);
      UNREFERENCED_PARAMETER(pSender);
      if(phint != NULL)
      {
         if(base < ::mediaplay::view_update_hint > :: bases(phint))
         {
            ::mediaplay::view_update_hint * puh = (::mediaplay::view_update_hint * ) phint;
            if(puh->is_type_of(::mediaplay::view_update_hint::TypeDeleteContents))
            {
               on_delete_contents();
            }
            else if(puh->is_type_of(::mediaplay::view_update_hint::TypeExecuteStop))
            {
               _ExecuteStop();
            }
            else if(puh->is_type_of(::mediaplay::view_update_hint::TypeCloseDevice))
            {
               CloseDevice();
            }
            else if(puh->is_type_of(::mediaplay::view_update_hint::TypeDoPlay))
            {
               _ExecutePlay(true, puh->m_position);
            }
            else if(puh->is_type_of(::mediaplay::view_update_hint::TypePlayIfNotPlaying))
            {
               if(!_ExecuteIsPlaying())
               {
                  _ExecutePlay(true, puh->m_position);
               }
            }
            else if(puh->is_type_of(::mediaplay::view_update_hint::TypeGetMidiPlayerInterface)
               && puh->m_pmidiplayer == NULL)
            {
               puh->m_pmidiplayer = this;
            }
            else if(puh->is_type_of(::mediaplay::view_update_hint::TypeGetPlaylistCallback))
            {
               puh->m_pplaylistcallback = GetPlaylistCallback();
            }
            else if(puh->is_type_of(::mediaplay::view_update_hint::TypeGetWavePlayer)
               && puh->m_pmidiplayer == NULL)
            {
               puh->m_pwaveplayer = GetWavePlayer();
            }
            else if(puh->is_type_of(::mediaplay::view_update_hint::TypeAttachPlaylist)
               && puh->m_pplaylistdoc != NULL)
            {
   // xxx            puh->m_pplaylistdoc->SetCallback(m_pplaylistcallback);
   // xxx            m_pplaylistcallback->m_pplaylistdoc = puh->m_pplaylistdoc;
            }
            else if(puh->is_type_of(::mediaplay::view_update_hint::TypeFullScreen))
            {
               if(puh->m_bFullScreen)
               {
                  ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
               }
               else
               {
                  ModifyStyleEx(0, WS_EX_CLIENTEDGE, SWP_FRAMECHANGED);
               }
            }
            else if(puh->is_type_of(::mediaplay::view_update_hint::TypeBeforeOpenDocument))
            {
               data * pdata = get_data();
               SetKaraokeData(&pdata->GetKaraokeData());
               m_lpbouncingball->SetKaraokeData(&pdata->GetKaraokeData());
            }
            else if(puh->is_type_of(::mediaplay::view_update_hint::TypeAfterOpenDocument))
            {
               data * pdata = get_data();
               if(get_data()->GetMode() == data::ModeMidi)
               {
                  if(GetMidiPlayer() != NULL)
                  {
                     GetMidiPlayer()->SetInterface(this);
                  }
                  else
                  {
                     if(!OpenMidiPlayer())
                     {
                        ASSERT(FALSE);
                        throw 0;
                     }
                  }
                  m_lpbouncingball->SetQuarterNoteDuration(GetMidiSequence().GetQuarterNote());
               }

               bool bListenMinusOne;
               string str;
               str = pdata->get_path_name();
               if(DBListenMinusOne(false, pdata->get_path_name(), bListenMinusOne))
               {
                  _ExecuteListenMinusOne(bListenMinusOne);
               }

               keeper < bool > keepHardWriting(&m_bHardWriting, true, false, true);
               System.wait_twf();
               PrepareLyricLines();
               PrepareLyricTemplateLines();
               keepHardWriting.KeepAway();

               if(!data_get("GlobalLyricsDelay", ::radix::system::idEmpty, m_iDelay))
                  m_iDelay = 0;
            }
         }
      }
   }

   void view::_001OnDestroy(gen::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);

      gcom::backview::Main & main = gcom::backview::Interface::GetMain();
      gcom::backview::TransitionEffect & effect = main.GetTransitionEffect();

      effect.m_bActive = false;
      effect.m_bDestroy = true;

      Finalize();


      if(m_pwaverecorder->IsRecording())
      {
         _StopRecording();
         m_pwaverecorder->m_eventStopped.wait(millis((1984 + 197) * 20));
      }

      if(_ExecuteIsPlaying())
      {
         _ExecuteStopAndQuit();
      }
      else
      {
         if(GetWavePlayer() != NULL)
         {
            GetWavePlayer()->PostThreadMessage(WM_QUIT, 0, 0);
         }
      }

      m_bDestroy = true;
   }



   void view::_001OnSize(gen::signal_object * pobj)
   {
      SCAST_PTR(::user::win::message::size, psize, pobj)

      pobj->previous();

      double d = psize->m_size.cy;
      d /= GetSystemMetrics(SM_CYSCREEN);
      SetScale(d);

      GetClientRect(m_rectKaraokeView);


      rect rectClient;
      GetClientRect(rectClient);
      rect rect = rectClient;

      rect.top = rect.bottom - 24;
      m_viewlineaStatus.m_pinteraction = this;
      m_viewlineaStatus.Prepare();
      m_viewlineaStatus[0].SetPlacement(rect);


      m_rectKaraokeView.bottom -= 16;

      LayoutTitleLines();
      LayoutLyricTemplateLines();
      LayoutLyricLines();
      LayoutKaraokeBouncingBall();

   }

   /*void view::_001OnPaint(gen::signal_object * pobj)
   {
      CPaintDC spgraphics(this); // device context for painting
      _001OnDraw(&spgraphics);
   }
   */


   void view::_001OnCreate(gen::signal_object * pobj)
   {
      if(pobj->previous())
         return;

      GetWavePlayer()->SetCallback(this);


      if(m_bEnableShowGcomBackground)
      {
         
         bool bShowGcomBackground = true;
         
         if(!data_get("ShowGcomBackground", ::radix::system::idEmpty, bShowGcomBackground))
            bShowGcomBackground = true;

         show_gcom_background(bShowGcomBackground);
         
      }


   /*xxx   m_pplaylistcallback = new CVmpLightPlaylistCallback(
         &musctrl::CMusicalPlayerCentral::AppGetMusicalPlayerCentral(),
         get_data());*/


     // m_gdi.m_spgraphics->Attach(GetDC()->get_os_data());
   /*   GLuint PixelFormat;         // Holds The Results After Searching For A Match

      ::ca::graphics * pdc = GetDC();
      HDC hdc = (HDC)pdc->get_os_data();

      if (!(PixelFormat=ChoosePixelFormat(hdc, &m_pixelformatdescriptor)))   // Did Windows find A Matching Pixel Format?
      {
   //      MessageBox(NULL,"Can't find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
         return -1;                        // Return FALSE
      }

      if(!SetPixelFormat(hdc, PixelFormat,&m_pixelformatdescriptor))      // Are We Able To Set The Pixel Format?
      {
   //      MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
         return -1;                        // Return FALSE
      }

      if (!(m_hglrc = wglCreateContext(hdc)))            // Are We Able To get A Rendering Context?
      {
   //      MessageBox(NULL,"Can't create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
         return -1;                        // Return FALSE
      }

      if(!wglMakeCurrent(hdc, m_hglrc))               // Try To Activate The Rendering Context
      {
   //      MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
         return -1;                        // Return FALSE
      }*/
      ::mediaplay::view_interface::m_pdocument = dynamic_cast < document * > (get_document());
      ::mediaplay::view_interface::m_pdocument->m_pview = this;


      int i;
      if(data_get("GradualFilling", ::radix::system::idEmpty, i))
      {
         m_bGradualFilling = i != 0;
      }
      if(data_get("BouncingBall", ::radix::system::idEmpty, i))
      {
         m_bBouncingBall = i != 0;
      }

      SetTimer(TimerLyric, 40, NULL); // max. 25 fps
      SetTimer(10, 784, NULL);
      SetTimer(gen::Timer::ID_HOVER, 100, NULL);


      m_lyrictemplatelines.remove_all();
      m_lyrictemplatelines.set_size(4);

      PrepareLyricTemplateLines();

      //AfxBeginThread(
        // ThreadProcLyricEvent,
         //(LPVOID) this,
         //THREAD_PRIORITY_NORMAL,
         //0,
         //0,
         //NULL);


   }
   void view::_001OnContextMenu(gen::signal_object * pobj)
   {
      SCAST_PTR(::user::win::message::context_menu, pcontextmenu, pobj)
      point point = pcontextmenu->GetPoint();
//      LyricViewLines & lyriclines = GetLyricLines();
      class point ptCursor;
      ptCursor = point;
      ScreenToClient(&ptCursor);
   /* linux   if(m_lyrictemplatelines.hit_test(ptCursor, iLine, iToken, iChar)
         == ::user::line_hit_link)
      {
         string str;
         lyriclines.GetLink(str, iLine, iToken, iChar);
         m_wstrCurrentLink = str;
         SimpleMenu menu(BaseMenuCentral::GetMenuCentral(get_app()));
         if (menu.LoadMenu(m_uiPopupLink))
         {
            SimpleMenu* pPopup = (SimpleMenu *) menu.GetSubMenu(0);
            ASSERT(pPopup != NULL);
            frame_window * pframe = GetParentFrame();
            pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
               point.x, point.y,
               (::ca::window *) pframe);
         }
      }
      else
      {
         ::userbase::menu menu;
         if (menu.LoadMenu(m_uiPopup))
         {
            ::userbase::menu* pPopup = menu.GetSubMenu(0);
            ASSERT(pPopup != NULL);
            frame_window * pframe = GetParentFrame();
            pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
               point.x, point.y,
               (::ca::window *) pframe);
         }
      }*/

   }


   void view::_001OnTabClick(int iTab)
   {
      if(iTab == 1)
      {
         System.simple_message_box(NULL, "Playlist");
      }
   }

   void view::_001OnWavePlayerEvent(gen::signal_object * pobj)
   {

      SCAST_PTR(::user::win::message::base, pbase, pobj)
      audWavePlayer::e_event eevent = (audWavePlayer::e_event) pbase->m_wparam;
      switch(eevent)
      {
      case audWavePlayer::EventOpenDecoder:
         {

            data * pcreatordata = get_data();
            if(pcreatordata->GetMode() == data::ModeWave)
            {
               stringa wstraFormat;
               string2a wstr2aTitle;
               GetWavePlayer()->FillTitleInfo(wstraFormat, wstr2aTitle);
               if(wstraFormat.get_size() <= 0)
               {
                  wstraFormat.add(L"%0");
                  wstr2aTitle.add_new();
                  wstr2aTitle[wstr2aTitle.get_upper_bound()].add(get_document()->get_title());
               }
               pcreatordata->GetKaraokeData().GetStaticData().m_straTitleFormat = wstraFormat;
               pcreatordata->GetKaraokeData().GetStaticData().m_str2aTitle = wstr2aTitle;
               GetKaraokeData().GetStaticData().m_dwDefaultCodePage = CP_UTF8;
            }
            keeper < bool > keepHardWriting(&m_bHardWriting, true, false, true);
            System.wait_twf();
            PrepareLyricTemplateLines();
            PrepareLyricLines();
            SetTitleVisible(true);
         }
         break;
      case audWavePlayer::EventPlay:
         OnMediaPlaybackStart();
         break;
      case audWavePlayer::EventPlaybackEnd:
         OnMediaPlaybackEnd();
         break;
      }
   }

   void view::_001OnUpdateViewEncoding(gen::signal_object * pobj)
   {
      SCAST_PTR(::user::win::message::update_cmd_ui, pupdatecmdui, pobj)
      DWORD dwCodePage = IdToCodePage(pupdatecmdui->m_pcmdui->m_id);
      if(GetKaraokeCodePage(NULL) == dwCodePage)
      {
         pupdatecmdui->m_pcmdui->_001SetCheck(check::checked);
         pupdatecmdui->m_pcmdui->Enable(0);
      }
      else
      {
         pupdatecmdui->m_pcmdui->_001SetCheck(check::unchecked);
         pupdatecmdui->m_pcmdui->Enable(1);
      }
      pupdatecmdui->m_bRet = true;
   }
   void view::_001OnViewEncoding(gen::signal_object * pobj)
   {
      SCAST_PTR(BaseCommand, pcommand, pobj)
      DWORD dwCodePage = IdToCodePage(pcommand->m_id);
      SetKaraokeCodePage(dwCodePage);
   }


   /*xxxvoid view::data_on_after_change(int iConfigurationId, int iLine, int iColumn, CVmsDataUpdateHint * puh)
   {
      if(iConfigurationId == _vmsp::CConfiguration::CfgKaraokeEncoding)
      {
         PrepareLyricLines();
         RedrawWindow();
      }
   }*/


   void view::_001OnSetCursor(gen::signal_object * pobj)
   {
      ::SetCursor(::LoadCursor(NULL, IDC_ARROW));

      pobj->previous();
   }


   data * view::get_data()
   {
      return ::mediaplay::view_interface::get_data();
   }

   


} // namespace mediaplay
