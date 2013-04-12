#include "framework.h"


namespace fontopus
{


   fontopus::fontopus()
   {
      m_puser                    = ::null();
      m_pthreadCreatingUser      = ::null();
   }  


   fontopus::~fontopus()
   {
   }


   bool fontopus::initialize_instance()
   {

      return true;

   }


   int32_t fontopus::exit_instance()
   {

      int32_t iExitCode = 0;

      return iExitCode;

   }


   user * fontopus::create_current_user()
   {
      ::ca::property_set set(get_app());
      if(m_puser == ::null())
      {
         int32_t iRetry = 3;
         ::fontopus::user * puser = ::null();
         while(iRetry > 0)
         {
            puser = login(set);
            if(puser != ::null())
               break;
            //bool bOk = false;
            //string strSection;
            //strSection.Format("license_auth");
            //string strDir;
            //strDir = Application.dir().usersystemappdata(puser->m_strPathPrefix, strSection);
            //::DeleteFile(System.dir().path(strDir, "00001"));
            //::DeleteFile(System.dir().path(strDir, "00002"));
            iRetry--;
         }
         if(puser == ::null())
         {
            //System.simple_message_box(::null(), "<h1>You have not logged in!</h1><h2>Exiting</h2>");
            TRACE("<error>You have not logged in! Exiting!</error>");
            throw string("You have not logged in! Exiting!");
            return ::null();
         }
         m_puser = create_user(puser);
         System.userset().add(m_puser);


         if(m_puser != ::null() 
            && !::ca::str::begins(m_puser->m_strLogin, "system")
            && m_papp->m_pappThis->m_strAppId != "app-core/deepfish"
            && !::ca::str::begins(m_papp->m_pappThis->m_strAppName, "app-core/deepfish_")
            && !m_papp->m_pappThis->is_serviceable())
         {

            on_user_login(m_puser);

         }



      }
      return m_puser;
   }

   user * fontopus::login(::ca::property_set & set)
   {
      /*::ca::fontopus * papp;
      if(m_puiInitialPlaceHolderContainer != ::null())
      {
         papp = m_puiInitialPlaceHolderContainer->m_papp;
      }
      else if(System.m_puiInitialPlaceHolderContainer != ::null())
      {
         papp = System.m_puiInitialPlaceHolderContainer->m_papp;
      }
      else
      {
         papp = this;
      }*/
      //class validate authuser(papp, "system\\user\\authenticate.xhtml", true);

      user * puser = ::null();

      if(m_papp->m_pappThis->m_psession != ::null() && m_papp->m_pappThis->m_psession->m_pbergedge != ::null())
      {
       
         puser = m_papp->m_pappThis->m_psession->m_pbergedgeInterface->m_pfontopus->login(set);

      }
      else
      {

         class validate authuser(m_papp->m_pappThis, "system\\user\\authenticate.xhtml", true);
         authuser.m_bDeferRegistration = true;
         authuser.propset().merge(set);
         puser = authuser.get_user();

      }

      return puser;

   }

   bool fontopus::get_auth(const char * psz, string & strUsername, string & strPassword)
   {
      /*::ca::fontopus * papp;
      if(m_puiInitialPlaceHolderContainer != ::null())
      {
         papp = m_puiInitialPlaceHolderContainer->m_papp;
      }
      else if(System.m_puiInitialPlaceHolderContainer != ::null())
      {
         papp = System.m_puiInitialPlaceHolderContainer->m_papp;
      }
      else
      {
         papp = this;
      }*/
      //class validate authuser(papp, psz);
      class validate authuser(m_papp->m_pappThis, psz);
      validate::auth * pauth = authuser.get_auth();
      if(pauth == ::null())
         return false;
      else
      {
         strUsername = pauth->m_strUsername;
         strPassword = pauth->m_strPassword;
         delete pauth;
         return true;
      }
   }

   void fontopus::logout()
   {
      if(m_puser != ::null())
      {
         try
         {
            System.file().del(::dir::userappdata("license_auth/00001.data"));
         }
         catch(...)
         {
         }
         try
         {
            System.file().del(::dir::userappdata("license_auth/00002.data"));
         }
         catch(...)
         {
         }
         delete m_puser;
      }
      m_puser = ::null();
   }



   user * fontopus::allocate_user()
   {
      return new class user(m_papp->m_pappThis);
   }

   user * fontopus::create_user(::fontopus::user * puser)
   {
      if(puser->m_strPathPrefix.is_empty())
      {
         puser->m_strPathPrefix = Application.dir().default_os_user_path_prefix();
      }
      puser->m_strPath = Application.dir().default_userfolder(puser->m_strPathPrefix, puser->m_strLogin);
      App(m_papp->m_pappThis).dir().mk(puser->m_strPath);
      puser->m_strDataPath = Application.dir().default_userdata(puser->m_strPathPrefix, puser->m_strLogin);
      App(m_papp->m_pappThis).dir().mk(puser->m_strDataPath);
      puser->m_strAppDataPath = Application.dir().default_userappdata(puser->m_strPathPrefix, puser->m_strLogin);
      App(m_papp->m_pappThis).dir().mk(puser->m_strAppDataPath);
      puser->create_ifs();
      return puser;
   }


   user * fontopus::create_system_user(const string & strSystemUserName)
   {
      
      m_puser                 = new ::fontopus::user(get_app());
      
      m_puser->m_strLogin     = strSystemUserName;
      
      user * puserNew = create_user(m_puser);

      if(puserNew == ::null())
      {
         
         delete m_puser;

         m_puser = ::null();

      }

      return m_puser;

   }



   user * fontopus::get_user()
   {
      if(m_papp->m_pappThis->is_session())
      {
         if(m_puser == ::null())
         {
            if(m_pthreadCreatingUser == ::ca::get_thread())
               return ::null();
            if(m_pthreadCreatingUser != ::null())
            {
               while(m_pthreadCreatingUser != ::null())
               {
                  Sleep(84);
               }
               if(m_puser != ::null())
                  return m_puser;
               return ::null();
            }
            keeper < ::ca::thread * > keepCreatingUser(&m_pthreadCreatingUser, ::ca::get_thread(), ::null(), true);
            user * puser = create_current_user();
            if(!puser->initialize())
            {
               delete puser;
               m_puser = ::null();
               return ::null();
            }
            m_puser = puser;
         }
         if(m_puser != ::null())
         {
            if(m_puser->m_pifs == ::null())
            {
               m_puser->create_ifs();
            }
         }
         return m_puser;
      }
      else if(m_papp == ::null() || m_papp->m_psession == ::null())
      {
         return ::null();
      }
      else
      {
         return Sess(m_papp->m_pappThis).get_user();
      }
   }

   void fontopus::set_user(const char * psz)
   {
      UNREFERENCED_PARAMETER(psz);
   }

   void fontopus::set_user(user * puser)
   {
      m_puser = puser;
   }

   bool fontopus::check_license(const char * pszId, bool bInteractive)
   {

      class validate authuser(m_papp->m_pappThis, "err\\user\\authentication\\not_licensed.xhtml", true, bInteractive);
      return authuser.get_license(pszId);

   }

   void fontopus::on_user_login(user * puser)
   {


#ifndef METROWIN

      if(!System.directrix()->m_varTopicQuery.has_property("install")
      && !System.directrix()->m_varTopicQuery.has_property("uninstall"))
      {

         sp(::ca::create_context) spcreatecontext(allocer());

         sp(::ca::application) papp = Session.start_application("application", "app-core/deepfish", spcreatecontext);

         if(papp == ::null())
         {
            Application.simple_message_box(::null(), "deepfish subsystem - responsible for running background applications - could not be started");
         }

      }

#endif

   }


} // namespace fontopus






#include "framework.h"


namespace ca
{


   namespace fontopus
   {


      fontopus::fontopus()
      {

         m_puser                    = ::null();
         m_pthreadCreatingUser      = ::null();

      }


      void fontopus::construct(sp(::ca::application) papp)
      {

         ::fontopus::fontopus::construct(papp);

      }


      fontopus::~fontopus()
      {

      }


      bool fontopus::initialize_instance()
      {

         if(!::fontopus::fontopus::initialize_instance())
            return false;

         return TRUE;

      }


      int32_t fontopus::exit_instance()
      {

         try
         {

            ::fontopus::fontopus::exit_instance();

         }
         catch(...)
         {

         }
         return 0;
      }


      ::fontopus::user * fontopus::login(::ca::property_set & set)
      {
         /*::ca::fontopus * papp;
         if(m_puiInitialPlaceHolderContainer != ::null())
         {
            papp = m_puiInitialPlaceHolderContainer->m_papp;
         }
         else if(System.m_puiInitialPlaceHolderContainer != ::null())
         {
            papp = System.m_puiInitialPlaceHolderContainer->m_papp;
         }
         else
         {
            papp = this;
         }*/
         //class validate authuser(papp, "system\\user\\authenticate.xhtml", true);
         class validate authuser(get_app(), "system\\user\\authenticate.xhtml", true);
         authuser.m_bDeferRegistration = true;
         authuser.propset().merge(set);
         if(set.has_property("ruri"))
            return authuser.get_user(set["ruri"]);
         else
            return authuser.get_user();
      }


      bool fontopus::get_auth(const char * psz, string & strUsername, string & strPassword)
      {
         /*::ca::fontopus * papp;
         if(m_puiInitialPlaceHolderContainer != ::null())
         {
            papp = m_puiInitialPlaceHolderContainer->m_papp;
         }
         else if(System.m_puiInitialPlaceHolderContainer != ::null())
         {
            papp = System.m_puiInitialPlaceHolderContainer->m_papp;
         }
         else
         {
            papp = this;
         }*/
         //class validate authuser(papp, psz);
         class validate authuser(get_app(), psz);
         validate::auth * pauth = authuser.get_auth();
         if(pauth == ::null())
            return false;
         else
         {
            strUsername = pauth->m_strUsername;
            strPassword = pauth->m_strPassword;
            delete pauth;
            return true;
         }
      }

      bool fontopus::check_license(const char * pszId, bool bInteractive)
      {

         class validate authuser(get_app(), "err\\user\\authentication\\not_licensed.xhtml", true, bInteractive);
         return authuser.get_license(pszId);

      }


      void fontopus::on_request(sp(::ca::create_context) pcreatecontext)
      {

         if(pcreatecontext->m_spCommandLine.is_set()
          && pcreatecontext->m_spCommandLine->m_varFile == "ca2login")
         {
            //Sleep(15 * 1000);
            ::ca::property_set setLogin(get_app());
            if(pcreatecontext->m_spCommandLine->m_varQuery["ruri"].is_set())
            {
               setLogin["ruri"] = pcreatecontext->m_spCommandLine->m_varQuery["ruri"];
            }
            ::fontopus::user * puser = ::null();
            while(true)
            {
               if((puser = login(setLogin)) != ::null())
                  break;
            }
            if(pcreatecontext->m_spCommandLine->m_varQuery["ruri"].is_set())
            {
               string strUri = pcreatecontext->m_spCommandLine->m_varQuery["ruri"];
               System.url().string_set(strUri, "sessid", puser->m_strFontopusServerSessId);
               System.open_link(strUri);
            }
         }

      }



   } // namespace fontopus


} // namespace ca


