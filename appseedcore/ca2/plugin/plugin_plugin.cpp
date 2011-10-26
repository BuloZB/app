#include "StdAfx.h"


void CLASS_DECL_ca __cdecl _ca2_purecall_(void)
{
    throw simple_exception();
}


namespace plugin
{


   plugin::plugin()
   {

      m_puiHost               = NULL;
      m_iHealingSurface       = 0;
      m_iEdge                 = -1;
      m_pbergedge             = NULL;
      m_bAppStarted           = false;
      m_hEventReady           = NULL;
      m_psystem               = NULL;
      m_bMainReady            = false;
      m_bOpenUrl              = false;

   }

   bool plugin::hist(const char * pszUrl)
   {
      return open_url(pszUrl);
   }

   void plugin::run_start_install(const char * pszInstall)
   {
      UNREFERENCED_PARAMETER(pszInstall);
   }

   int plugin::start_ca2_system()
   {

      bool bNew = false;

      if(m_psystem == NULL)
      {

         _set_purecall_handler(_ca2_purecall_);

         m_bAppStarted = false;
         
         m_psystem = new ::plugin::system();

         set_app(m_psystem);

         m_psystem->m_pplugin = this;

         m_psystem->m_hInstance = ::GetModuleHandle("ca2.dll");
      
         
         if(!m_psystem->InitApplication())
            return 0;
      
         
         if(!m_psystem->process_initialize())
            return 0;


         m_psystem->set_history(new history(m_psystem, this));

         m_psystem->m_prunstartinstaller = new run_start_installer(m_psystem, this);

         m_psystem->m_bInitializeProDevianMode = false;
         
         string strId;
         strId.Format("npca2::%08x", (UINT_PTR) m_psystem);
         
         m_psystem->command().m_varTopicQuery["local_mutex_id"] = strId;


      }

      m_puiHost = create_host_interaction();
      m_puiHost->m_pplugin = this;
      m_puiHost->install_message_handling(m_puiHost->m_pimpl);

      if(m_puiHost != NULL)
      {
         m_psystem->oprop("top_parent") = m_puiHost;
/*         m_puiHost->m_bRectOk = true;
         m_puiHost->m_pimpl->m_bRectOk = true;
         m_puiHost->m_rectParentClient = m_rect;
         m_puiHost->m_rectParentClient.offset(-point64(m_puiHost->m_rectParentClient.top_left()));
         m_puiHost->m_pimpl->m_rectParentClient = m_puiHost->m_rectParentClient;
         m_puiHost->SendMessageA(WM_SIZE);
         m_puiHost->SendMessageA(WM_MOVE);*/
         class rect rect;
         get_window_rect(rect);
         set_window_rect(rect);
      }

///      m_psystem->m_puiInitialPlaceHolderContainer = m_puiHost;
      m_psystem->add_frame(m_puiHost);
      m_puiHost->layout();

      if(m_hEventReady == NULL)
         m_hEventReady = ::CreateEventA(NULL, TRUE, FALSE, NULL);

      // Create Message Queue
      MSG msg;
	   PeekMessage(&msg, NULL, 0, 0xffffffffu, FALSE);

      if(bNew || !m_bAppStarted)
      {
         m_bAppStarted = true;
         m_psystem->Begin();
      }

      m_bInitialized = true;

      return 0;
   }




   void plugin::on_paint(HDC hdcWindow, LPCRECT lprect)
   {

      if(m_puiHost == NULL)
         return;

      RECT rect;
      rect.left = m_rect.left;
      rect.top = m_rect.top;
      rect.right = m_rect.right;
      rect.bottom = m_rect.bottom;
      rect.bottom -= rect.top;
      rect.top = 0;
      rect.right -= rect.left;
      rect.left = 0;
      rect64 rectWindow64;
      m_puiHost->GetWindowRect(rectWindow64);
      class rect rectWindow;
      rectWindow = rectWindow64;
      RECT rectDesktop;
      ::GetWindowRect(::GetDesktopWindow(), &rectDesktop);
      int w = rectDesktop.right;
      int h = rectDesktop.bottom;
      HBITMAP hbmp = ::CreateCompatibleBitmap(hdcWindow, w, h);
      HDC hdc = ::CreateCompatibleDC(hdcWindow);
      HBITMAP hbmpOld =  (HBITMAP) ::SelectObject(hdc, (HGDIOBJ) hbmp);
      HFONT hfontOld = NULL;
      HFONT hfont = NULL;
      ::BitBlt(hdc, lprect->left + rectWindow.left - m_rect.left, lprect->top + rectWindow.top - m_rect.top, lprect->right - lprect->left, lprect->bottom - lprect->top, hdcWindow, lprect->left, lprect->top, SRCCOPY);
      try
      {
         ::ca::graphics * pgraphics = m_psystem->graphics_from_os_data(hdc);
         pgraphics->set_app(m_puiHost->m_papp);
         m_puiHost->_000OnDraw(pgraphics);
      }
      catch(...)
      {
      }
      POINT pointViewport;
      ::SetViewportOrgEx(hdc, 0, 0, &pointViewport);
      ::BitBlt(hdcWindow, lprect->left, lprect->top, lprect->right - lprect->left, lprect->bottom - lprect->top, hdc, lprect->left + rectWindow.left - m_rect.left, lprect->top + rectWindow.top - m_rect.top, SRCCOPY);
      ::SelectObject(hdc, (HGDIOBJ) hbmpOld);
      if(hfontOld != NULL)
      {
         ::SelectObject(hdc,(HGDIOBJ)  hfontOld);
      }
      if(hfont != NULL)
      {
         ::DeleteObject(hfont);
      }
      ::DeleteObject(hbmp);
      ::DeleteDC(hdc);

      if(m_bOpenUrl)
      {
         m_bOpenUrl = false;
         string strOpenUrl = m_strOpenUrl;
         m_strOpenUrl.Empty();
         open_url(strOpenUrl);
      }

   }

   void plugin::on_ready()
   {
      m_puiHost->PostMessageA(host_interaction::message_check, 0, 1);
   }

   void plugin::start_ca2_login()
   {
      AfxBeginThread(m_puiHost->get_app(), &plugin::thread_proc_ca2_login, (LPVOID) this); 
   }

   void plugin::start_ca2_logout()
   {
      AfxBeginThread(m_puiHost->get_app(), &plugin::thread_proc_ca2_logout, (LPVOID) this); 
   }

   UINT AFX_CDECL plugin::thread_proc_ca2_login(LPVOID pvoid)
   {
      plugin * pplugin = (plugin *) pvoid;
      pplugin->ca2_login();
      return 0;
   }

   UINT AFX_CDECL plugin::thread_proc_ca2_logout(LPVOID pvoid)
   {
      plugin * pplugin = (plugin *) pvoid;
      pplugin->ca2_logout();
      return 0;
   }

   void plugin::ca2_login()
   {
      gen::property_set set(m_psystem);
      set.parse_url_query(m_strCa2LoginRuri);
      string strLocation = set["ruri"];
      if(strLocation.is_empty())
         strLocation = m_strCa2LoginRuri;
      gen::property_set setUri(m_psystem);
      setUri.parse_url_query(strLocation);
      if(Sys(m_psystem).url().get_server(strLocation).is_empty())
      {
         strLocation = Sys(m_psystem).url().override_if_empty(strLocation, get_host_location_url(), false);
      }
      string strSessId = set["sessid"];
      
      ::fontopus::user * puser = NULL;
      while(puser == NULL)
      {
         ::fontopus::validate authuser(m_psystem, "system\\user\\authenticate.xhtml", true);
         puser = authuser.get_user(strLocation, strSessId);
      }
      
      if(strSessId == puser->m_strFontopusServerSessId ||
         puser->m_strFontopusServerSessId.get_length() < 16)
      {
         m_psystem->url().remove(strLocation, "sessid");
      }
      else
      {
         m_psystem->url().set(strLocation, "sessid", puser->m_strFontopusServerSessId);
      }
      m_psystem->url().remove(strLocation, "action");
      m_strOpenUrl = strLocation;
      m_bOpenUrl = true;
   }

   void plugin::ca2_logout()
   {
      m_psystem->logout();
      gen::property_set set(m_psystem);
      set.parse_url_query(m_strCa2LogoutRuri);
      string strLocation = set["ruri"];
      strLocation = m_psystem->url().remove(strLocation, "sessid");
      strLocation = m_psystem->url().remove(strLocation, "action");
      string strUrl;
      strUrl = "https://fontopus.com/sec?action=logout";
      m_psystem->url().set(strUrl, "ruri", strLocation);
      m_strOpenUrl = strLocation;
      m_bOpenUrl = true;
   }


   void plugin::ready_on_main_thread()
   {

      if(m_bMainReady)
         return;

      m_bMainReady = true;

      int iCount = get_memory_length();

      if(iCount > 0)
      {
         m_memory.allocate(iCount + 1);

         read_memory(m_memory, iCount);

         m_memory.get_data()[iCount] = '\0';

         open_ca2_string((const char *) m_memory.get_data());
      }
      else if(m_phost->m_strPluginUrl.get_length() > 0)
      {

         string strPluginUrl     = Sys(m_psystem).url().override_if_empty(m_phost->m_strPluginUrl, get_host_location_url());

         string strPluginData    = ms_get_dup(strPluginUrl);

         open_ca2_string(strPluginData);

      }

   }

   void plugin::open_ca2_string(const char * psz)
   {
      
      LPSTR lpszAlloc = (LPSTR) (void *) psz;
      
      int iCount = strlen(psz);

      //sleep(seconds(15));

      string strPluginUrl;

      strPluginUrl = m_phost->m_strPluginUrl;

      Sys(m_psystem).url().override_if_empty(strPluginUrl, get_host_location_url());

      string strPluginScript = m_psystem->url().get_script(m_phost->m_strPluginUrl);


      gen::property_set headers(m_psystem);

      headers.parse_http_headers(m_phost->m_strPluginHeaders);

      string strContentType = headers["Content-Type"];
   
      string str;

      // TODO |) : Should parse Content-type:
      // ALSO: this case only happens if all file has been downloaded before the plugin has initialized
      if(gen::str::ends_ci(strPluginScript, ".mp3")
      || gen::str::ends_ci(strPluginScript, ".mid")
      || gen::str::ends_ci(strPluginScript, ".kar")
      || gen::str::ends_ci(strPluginScript, ".st3"))
      {
         //m_psystem->m_puiInitialPlaceHolderContainer = m_puiHost;
         ::ca::application_bias * pbiasCreate = new ::ca::application_bias;
         pbiasCreate->m_puiParent = m_puiHost;
         pbiasCreate->m_set["NativeWindowFocus"] = false;
         m_psystem->get_bergedge(0)->open_by_file_extension("\"" + strPluginUrl + "\"", pbiasCreate);
      }
      else
      {
         LPSTR lpszStart = lpszAlloc;
         LPSTR lpszEnd = NULL;
         int i = 0;
         for(; i < iCount; i++)
         {
            lpszEnd = &lpszStart[i];
            if(*lpszEnd == '\n')
            {
               str = string(lpszStart, lpszEnd - lpszStart);
               break;
            }
         }
         str.trim();

         if(str == "ca2login")
         {
            m_strCa2LoginRuri = string(lpszEnd + 1, iCount - (lpszEnd - lpszStart) - 1);
            start_ca2_login();
         }
         else if(str == "ca2logout")
         {
            m_strCa2LogoutRuri = string(lpszEnd + 1, iCount - (lpszEnd - lpszStart) - 1);
            start_ca2_logout();
         }
         else if(str == "ca2prompt")
         {
            if(System.url().get_script(get_host_location_url()) == "/non_auth")
            {
               m_strCa2LoginRuri = System.url().set_script(get_host_location_url(), "/");
               start_ca2_login();
            }
            else
            {
               m_puiHost->layout();
               lpszStart = lpszEnd;
               for(; (lpszEnd - lpszAlloc) <= iCount; i++)
               {
                  if(*lpszEnd == '\0' || !gen::ch::is_whitespace(lpszEnd))
                     break;
                  lpszEnd = (char *) gen::str::utf8_inc(lpszEnd);
               }
               lpszStart = lpszEnd;
               for(; (lpszEnd - lpszAlloc) <= iCount; i++)
               {
                  if(*lpszEnd == '\0' || gen::ch::is_space_char(lpszEnd) || (lpszEnd - lpszAlloc) == iCount)
                  {
                     str = string(lpszStart, lpszEnd - lpszStart);
                     break;
                  }
                  lpszEnd = (char *) gen::str::utf8_inc(lpszEnd);
               }
               if(!m_bApp)
               {
                  ::WaitForSingleObject(m_hEventReady, (1984 + 1977) * 20);
                  if(!m_bApp)
                  {
                     printf("Bergedge application is not initialized. Cannot start mplite.");
                     return;
                  }
               }
               if(str.has_char())
               {
                  string strId = str;
                  int iFind = strId.find("?");
                  if(iFind >= 0)
                  {
                     strId = strId.Left(iFind);
                  }
                  if(strId.has_char() && !m_psystem->install().is(strId))
                  {
                     Sys(m_psystem).install().ca2_cube_install(strId);
                     m_puiHost->SetTimer(19841115, (1984 + 1977 )* 2, NULL);
                     m_bMainReady = false;
                  }
                  else
                  {
                     m_puiHost->KillTimer(19841115);
                     sleep(seconds(15));
//                     m_psystem->m_puiInitialPlaceHolderContainer = m_puiHost;
                     ::ca::application_bias * pbiasCreate = new ::ca::application_bias;
                     pbiasCreate->m_set["NativeWindowFocus"] = false;
                     pbiasCreate->m_puiParent = m_puiHost;
                     m_psystem->command().add_fork_uri(str, pbiasCreate);
                  }
               }
            }
         }
      }
   }



   bool plugin::finalize()
   {

      if(!m_bAppStarted)
         return true;

      try
      {
         m_puiHost->DestroyWindow();
      }
      catch(...)
      {
      }

      ::radix::thread * pthread = dynamic_cast < ::radix::thread * > (m_psystem->smart_pointer < ::ca::thread >::m_p);

      if(pthread->m_bRun)
      {
         ::ResetEvent(m_hEventReady);
         pthread->m_peventReady = m_hEventReady;
         pthread->m_bRun = false;
         ::WaitForSingleObject(m_hEventReady, (1984 + 1977) * 20);
      }
      try
      {
         ::DeleteObject(m_hEventReady);
      }
      catch(...)
      {
      }

      try
      {
         delete m_psystem;
      }
      catch(...)
      {
      }
      return true;
   }





   UINT_PTR plugin::message_handler(UINT_PTR uiMessage, WPARAM wparam, LPARAM lparam)
   {
      if(m_puiHost != NULL)
      {
         if(uiMessage >= WM_MOUSEFIRST && uiMessage <= WM_MOUSELAST)
         {
            point pt = point(lparam);
            //pt.x -= m_rect.left;
            //pt.y -= m_rect.top;
            lparam = pt._001GetLparam();
         }
         if(uiMessage == WM_MOUSEMOVE)
         {
            ::ca::window * pwindow = dynamic_cast < ::ca::window * > (m_puiHost->m_pimpl);
            pwindow->m_bMouseHover = true; // avoids tracking mouse leave;
         }
         ::ca::window * pwindow = dynamic_cast < ::ca::window * > (m_puiHost->m_pimpl);
         HWND hwnd = pwindow->_get_handle();
         BOOL bIsWindow = ::IsWindow(hwnd);
         if(bIsWindow)
         {
            LRESULT l = m_puiHost->SendMessageA(uiMessage, wparam, lparam);
            return l;
         }
         else
         {
            return 0;
         }
      }
      return 0;
   }

   bool plugin::os_native_bergedge_start()
   {

      m_bApp = true;

      set_ready();
      
      return true;

   }

   void plugin::set_window_rect(LPCRECT lpcrect)
   {
      m_rect = *lpcrect;
      if(m_puiHost != NULL)
      {
         m_puiHost->m_bRectOk = true;
         m_puiHost->m_pimpl->m_bRectOk = true;
         m_puiHost->m_rectParentClient =  m_rect;
         m_puiHost->m_rectParentClient.offset(-point64(m_puiHost->m_rectParentClient.top_left()));
         m_puiHost->m_pimpl->m_rectParentClient = m_puiHost->m_rectParentClient;
         rect64 rectWindow64;
         m_puiHost->GetWindowRect(rectWindow64);
         class rect rectWindow;
         rectWindow = rectWindow64;
         m_puiHost->SetWindowPos(ZORDER_TOP, rectWindow.left, rectWindow.top, rectWindow.width(), rectWindow.height(), 0);
         m_puiHost->SendMessageA(WM_SIZE);
         m_puiHost->SendMessageA(WM_MOVE);
      }
   }



} // namespace plugin




