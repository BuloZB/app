#include "framework.h"



extern thread_pointer < memdleak_block > t_plastblock;

namespace base
{


   //UINT application::APPM_LANGUAGE = WM_APP + 117;
   //WPARAM application::WPARAM_LANGUAGE_UPDATE = 1;


   application::application()
   {

      m_pevAppBeg = NULL;


#ifdef WINDOWS

      m_hinstance                = NULL;

#endif

      if(m_pauraapp != NULL)
      {

         m_paxisapp              = m_pauraapp->m_paxisapp;

      }

      m_pbaseapp                 = this;

      if(m_pauraapp != NULL)
      {

         m_paxissystem           = m_pauraapp->m_paxissystem;

         if(m_pauraapp->m_pbasesession == NULL && m_paxissystem != NULL)
         {

            m_pbasesession       = m_paxissystem->m_pbasesession;

         }
         else
         {

            m_pbasesession       = m_pauraapp->m_pbasesession;

         }

#ifdef WINDOWS

         m_hinstance             = m_pauraapp->m_hinstance;

#endif

      }
      else
      {

         m_paxissystem           = NULL;

      }

      m_bBaseProcessInitialize         = false;
      m_bBaseProcessInitializeResult   = false;

      m_bBaseInitialize1               = false;
      m_bBaseInitialize1Result         = false;

      m_bBaseInitialize                = false;
      m_bBaseInitializeResult          = false;

      m_bBaseInitializeInstance        = false;
      m_bBaseInitializeInstanceResult  = false;




   }

   application::~application()
   {

   }


   bool application::process_init()
   {

      if(::axis::application::process_init())
      {

         return true;

      }

      return true;

   }


   void application::process_message_filter(int32_t code,::message::message * pobj)
   {

      if(pobj == NULL)
         return;   // not handled

      SCAST_PTR(::message::base,pbase,pobj);

      sp(::user::frame_window) pTopFrameWnd;
      //::user::interaction * pMainWnd;
      ::user::interaction * pMsgWnd;
      switch(code)
      {
      //case MSGF_DDEMGR:
      // Unlike other WH_MSGFILTER codes, MSGF_DDEMGR should
      //  never call the next hook.
      // By returning FALSE, the message will be dispatched
      //  instead (the default behavior).
      //return;

      case MSGF_MENU:

         pMsgWnd = dynamic_cast < ::user::interaction * > (pbase->m_pwnd);

         if(pMsgWnd != NULL)
         {
            pTopFrameWnd = pMsgWnd->GetTopLevelFrame();
            if(pTopFrameWnd != NULL && pTopFrameWnd->IsTracking() &&
                  pTopFrameWnd->m_bHelpMode)
            {
               //pMainWnd = __get_main_window();
               //if((m_puiMain != NULL) && (IsEnterKey(pbase) || IsButtonUp(pbase)))
               //{
               //   //                  pMainWnd->SendMessage(WM_COMMAND, ID_HELP);
               //   pbase->m_bRet = true;
               //   return;
               //}
            }
         }
      // fall through...

      case MSGF_DIALOGBOX:    // handles message boxes as well.
         //pMainWnd = __get_main_window();
         if(code == MSGF_DIALOGBOX && m_puiActive != NULL &&
               pbase->m_id >= WM_KEYFIRST && pbase->m_id <= WM_KEYLAST)
         {
            //// need to translate messages for the in-place container
            //___THREAD_STATE* pThreadState = __get_thread_state();
            //ENSURE(pThreadState);

            //if (pThreadState->m_bInMsgFilter)
            //   return;
            //pThreadState->m_bInMsgFilter = TRUE;    // avoid reentering this code
            //if (m_puiActive->is_window_enabled())
            //{
            //   pre_translate_message(pobj);
            //   if(pobj->m_bRet)
            //   {
            //      pThreadState->m_bInMsgFilter = FALSE;
            //      return;
            //   }
            //}
            //pThreadState->m_bInMsgFilter = FALSE;    // ok again
         }
         break;
      }
      // default to not handled

   }





















   /*string application::get_cred(const string & strRequestUrlParam,const RECT & rect,string & strUsername,string & strPassword,string strToken,string strTitle,bool bInteractive)
   {

      string str = ::fontopus::get_cred(this,strUsername,strPassword,strToken);

      if(str == "ok" && strUsername.has_char() && strPassword.has_char())
         return "ok";

      if(!bInteractive)
         return "failed";

      sp(::fontopus::simple_ui) pui;

      string strRequestUrl(strRequestUrlParam);

      if(strRequestUrl.is_empty())
      {

         string strIgnitionServer = file_as_string_dup(::dir::system() / "config\\system\\ignition_server.txt");

         if(::str::ends_ci(strIgnitionServer,".ca2.cc"))
         {

            strRequestUrl = "https://" + strIgnitionServer + "/";

         }
         else
         {

            strRequestUrl = "https://account.ca2.cc/";

         }

         strRequestUrl = "https://account.ca2.cc/";

      }

      pui = canew(::fontopus::simple_ui(this,strRequestUrl));

      pui->m_login.m_peditUser->set_window_text(strUsername);

      pui->m_login.m_ppassword->set_window_text("");

      string strResult = pui->get_cred(rect,strUsername,strPassword,strToken,strTitle);

      pui->DestroyWindow();

      ::fontopus::set_cred(this,strToken,strUsername,strPassword);

      return strResult;

   }*/


   ::user::user * application::create_user()
   {

      return canew(::user::user(this));

   }




   bool application::on_thread_on_idle(::thread_impl * pimpl,LONG lCount)
   {

      //      ASSERT_VALID(this);
      //
      //#if defined(WINDOWS) && defined(DEBUG) && !defined(___NO_DEBUG_CRT)
      //      // check core API's allocator (before idle)
      //      if(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) & _CRTDBG_CHECK_ALWAYS_DF)
      //         ASSERT(__check_memory());
      //#endif

      //single_lock sl(&pimpl->m_mutexUiPtra,TRUE);

      if(lCount <= 0)
      {

         sp(::user::interaction) pui;

         while(get_frame(pui))
         {
            //::user::interaction * pui = (::user::interaction *) pimpl->m_spuiptra->element_at(i)->m_pvoidUserInteraction;
            bool bOk = false;
            try
            {

               bOk = pui != NULL && pui->IsWindowVisible();
            }
            catch(...)
            {
            }
            if(!bOk)
            {
               //   try
               //   {
               //      Application.remove_frame(pui);
               //   }
               //   catch(...)
               //   {
               //   }
               //   try
               //   {
               //      Session.remove_frame(pui);
               //   }
               //   catch(...)
               //   {
               //   }
               //   try
               //   {
               //      System.remove_frame(pui);
               //   }
               //   catch(...)
               //   {
               //   }
            }
            else
            {
//               sl.unlock();
               try
               {
                  pui->send_message(WM_IDLEUPDATECMDUI,(WPARAM)TRUE);
               }
               catch(...)
               {

               }
               //             sl.lock();
            }
         }


      }
      else if(lCount >= 0)
      {
      }

      //#if defined(WINDOWS) && defined(DEBUG) && !defined(___NO_DEBUG_CRT)
      //      // check core API's allocator (after idle)
      //      if(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) & _CRTDBG_CHECK_ALWAYS_DF)
      //         ASSERT(__check_memory());
      //#endif
      //


      return lCount < 0;  // nothing more to do if lCount >= 0

   }


   bool application::post_user_message(::thread * pthread,::user::primitive * pui,UINT message,WPARAM wparam,lparam lparam)
   {

      if(pthread->m_hthread == NULL)
         return false;

      sp(::user::message) pmessage    = canew(::user::message);

#if MEMDLEAK

      ::aura::application * papp = this;

      if(pthread == NULL)
      {

         set_last_block_file_name(string(typeid(*papp).name()));

      }
      else
      {

         set_last_block_file_name(string(typeid(*papp).name()) +"&nbsp;-&nbsp;" +string(typeid(*pthread).name()));

      }

#endif

      pmessage->m_pui               = pui->m_puiThis;
      pmessage->m_uiMessage         = message;
      pmessage->m_wparam            = wparam;
      pmessage->m_lparam            = lparam;

      return pthread->post_object(message_system,system_message_user,pmessage) != FALSE;

   }



   bool application::is_window(::user::primitive * pui)
   {

      return pui->m_puiThis->IsWindow();

   }

   LRESULT application::send_message(::user::primitive * pui,UINT message,WPARAM wparam,lparam lparam)
   {

      return pui->m_puiThis->send_message(message,wparam,lparam);

   }


   oswindow application::get_safe_handle(::user::primitive * pui)
   {

      return pui->m_puiThis->get_safe_handle();

   }


   void application::dispatch_user_message_object(::object * pobject)
   {

      dispatch_user_message(dynamic_cast < ::user::message * > (pobject));

   }


   void application::dispatch_user_message(::user::message * pmessage)
   {

      pmessage->send();

   }


   ::user::interaction * application::get_parent(::user::interaction * pui)
   {

      return pui->GetParent();

   }


   bool application::enable_window(::user::primitive * pui,bool bEnable)
   {

      // if control has the focus, move the focus before disabling
      if(!bEnable)
      {

         if(Session.get_focus_ui() == pui->m_puiThis)
         {

            Application.send_message(pui->m_puiThis->GetParent(), WM_NEXTDLGCTL, 0, (LPARAM)FALSE);

         }

      }

      return pui->m_puiThis->enable_window(bEnable);

   }


   bool application::set_window_text(::user::interaction * pui,const string & strText)
   {

      pui->set_window_text(strText);

      return true;

   }



   sp(type) application::user_default_controltype_to_typeinfo(::user::e_control_type e_type)
   {

      return sp(type)();

   }


   int32_t application::hotplugin_host_host_starter_start_sync(const char * pszCommandLine,::aura::application * papp,::hotplugin::host * phost,::hotplugin::plugin * pplugin)
   {

      return hotplugin::host::host_starter_start_sync(pszCommandLine,get_app(),NULL);

   }


   ::user::interaction * application::FindWindow(const char * lpszClassName,const char * lpszWindowName)
   {

      ::exception::throw_interface_only(this);

      return NULL;

   }


   ::user::interaction * application::FindWindowEx(oswindow oswindowParent,oswindow oswindowChildAfter,const char * lpszClass,const char * lpszWindow)
   {

      ::exception::throw_interface_only(this);

      return NULL;

   }

   void application::remove_document_template(::user::impact_system * pimpactsystem)
   {

   }




//   ::user::style_base * application::userstyle()
//   {
//
//      if (m_pbasesession->m_puserstyle == NULL)
//      {
//
//         m_pbasesession->defer_create_user_style(preferred_userschema());
//
//      }
//
//      return m_pbasesession->m_puserstyle;
//
//   }
//
//
//   ::user::style_base * application::userstyle(::user::e_schema estyle)
//   {
//
//      ::user::style * puserstyle = userstyle();
//
//      if(puserstyle == NULL)
//      {
//
//         return NULL;
//
//      }
//
//      return puserstyle->operator[](estyle);
//
//   }


   void application::on_create_view(::user::view_creator_data * pcreatordata)
   {


   }


   bool application::process_message(LPMESSAGE lpmessage)
   {


      return ::thread::process_message(lpmessage);

   }


   void application::SetCurrentHandles()
   {

      ::axis::application::SetCurrentHandles();

   }


   void application::term_application()
   {

      try
      {

         for(auto & pair : System.m_appmap)
         {

            try
            {

               if(pair.m_element2->m_pbaseapp == this)
               {

                  pair.m_element2->m_pbaseapp = NULL;

               }

            }
            catch(...)
            {

            }

         }

      }
      catch(...)
      {

      }

      try
      {

         axis::application::term_application();

      }
      catch(...)
      {

      }

   }


   ::visual::icon * application::set_icon(object * pobject, ::visual::icon * picon, bool bBigIcon)
   {

      ::visual::icon * piconOld = get_icon(pobject, bBigIcon);

      if (bBigIcon)
      {

         pobject->oprop("big_icon").operator =((sp(object)) picon);

      }
      else
      {

         pobject->oprop("small_icon").operator =((sp(object)) picon);

      }

      return piconOld;

   }


   ::visual::icon * application::get_icon(object * pobject, bool bBigIcon) const
   {

      if (bBigIcon)
      {

         return const_cast <object *> (pobject)->oprop("big_icon").cast < ::visual::icon >();

      }
      else
      {

         return const_cast <object *> (pobject)->oprop("small_icon").cast < ::visual::icon >();

      }

   }


   void application::on_control_event(::user::control_event * pevent)
   {

      if (pevent->m_eevent == ::user::event_initialize_control)
      {

         if (pevent->m_puie->m_id == __id(system_startup_checkbox))
         {

            try
            {
               sp(::user::check) pcheck = pevent->m_puie;

               if(pcheck.is_set())
               {

                  pcheck->_001SetCheck(
                  System.os().is_user_auto_start(get_executable_appid()),
                  ::action::source_initialize);

               }
            }
            catch (...)
            {

            }

         }

      }
      else if (pevent->m_eevent == ::user::event_set_check)
      {

         if (pevent->m_puie->m_id == __id(system_startup_checkbox)
               && pevent->m_actioncontext.is_user_source())
         {

            try
            {

               sp(::user::check) pcheck = pevent->m_puie;

               if(pcheck.is_set())
               {

                  System.os().register_user_auto_start(
                  get_executable_appid(),
                  get_executable_path(),
                  pcheck->_001GetCheck() == ::check::checked);

               }

               pevent->m_bRet = true;

               return;

            }
            catch (...)
            {

            }

         }

      }

   }


   ::user::interaction * application::create_menu_interaction()
   {

      return canew(::user::button);

   }


} // namespace base







