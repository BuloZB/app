#include "StdAfx.h"

namespace netshareserver
{

   application::application(void)
   {
      m_strAppName                  = "netshareserver";
      m_strBaseSupportId            = "netshareserver";
      m_strLicense                  = "";
   }

   application::~application(void)
   {
   }


   int application::CreateService()
   {
	   SC_HANDLE hdlSCM = OpenSCManager(0, 0, SC_MANAGER_CREATE_SERVICE);

      string strCalling = m_strModulePath + " : service usehostlogin";

	   if (hdlSCM == 0) return ::GetLastError();
    
      SC_HANDLE hdlServ = ::CreateService(
		   hdlSCM,                    // SCManager database 
		   "CGCLCSTvotagusCa2FontopusMainNetShare",               // name of service 
		   "ccvotagus ca2 fontopus netshare",        // service name to display 
		   STANDARD_RIGHTS_REQUIRED,  // desired access 
		   SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, // service type 
		   SERVICE_AUTO_START,      // start type 
		   SERVICE_ERROR_NORMAL,      // error control type 
         strCalling,                   // service's binary Path name
		   0,                      // no load ordering group 
		   0,                      // no tag identifier 
		   0,                      // no dependencies 
		   0,                      // LocalSystem account 
		   0);                     // no password 
    
      DWORD Ret = 0;
      if (!hdlServ) Ret = ::GetLastError();
         CloseServiceHandle(hdlServ);
      return Ret;
   }

   int application::RemoveService()
   {
	   SC_HANDLE hdlSCM = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS);

	   string strCalling = m_strModulePath + " service";

	   if (hdlSCM == 0) return ::GetLastError();
    
      SC_HANDLE hdlServ = ::OpenService(
		   hdlSCM,                    // SCManager database 
		   "CGCLCSTvotagusCa2FontopusMainNetShare",               // name of service 
		   SC_MANAGER_ALL_ACCESS);                     // no password 
    
      DWORD Ret = 0;
      if (!hdlServ) Ret = ::GetLastError();
     ::DeleteService(hdlServ);
      CloseServiceHandle(hdlServ);
      return Ret;
   }




   bool application::initialize_instance()
   {
      if(!cube2::application::initialize_instance())
         return false;

      savings().save(gen::resource_display_bandwidth);

      m_strAppName                     = "netshareserver";
      //System.get_twf()->m_bProDevianMode      = false;
      m_bSessionSynchronizedCursor  = false;
      m_bSessionSynchronizedScreen  = false;

      return true;

   }

   void application::netnode_run()
   {
      m_pservice = new netshareserver::service(this);
      m_pservice->Start(0);
   }

   bool application::on_install()
   {
      CreateService();
      return true;
   }


   void application::on_request(::ca::create_context * pcreatecontext)
   {
      if (pcreatecontext->m_spCommandLine->m_varQuery.has_property("debugbreak"))
      {
         ::DebugBreak();
      }
      if (pcreatecontext->m_spCommandLine->m_varQuery.has_property("run"))
      {
         netnode_run();
      }
      else if(pcreatecontext->m_spCommandLine->m_varQuery.has_property("service"))
      {
         //return true;
      }
      else if (pcreatecontext->m_spCommandLine->m_varQuery.has_property("install"))
      {
         //return false;
      }
      else if (pcreatecontext->m_spCommandLine->m_varQuery.has_property("remove"))
      {
      }
      else if(pcreatecontext->m_spCommandLine->m_varQuery.has_property("create_service"))
      {
         CreateService();
         //return false;
      }
      else if(pcreatecontext->m_spCommandLine->m_varQuery.has_property("remove"))
      {
         RemoveService();
         //return false;
      }
      else
      {
         netnode_run();
      }
      //return true;
   }

   int application::run()
   {
      if (command().m_varTopicQuery.has_property("run"))
      {
         return cube2::application::run();
      }
      else if (command().m_varTopicQuery.has_property("service"))
      {
         class service service(this);
         service_base::run(service);
         return cube2::application::run();
      }
      else
      {
         return cube2::application::run();
      }
      return TRUE;
   }


} // namespace netshareserver


ca2::library * get_new_library()
{
   return new ::ca2::single_application_library < netshareserver::application > ();
}

