
namespace path
{


   ::file::path app()
   {

      return ::dir::stage() / "app.exe";

   }
   
   ::file::path app_install(string strPlatform)
   {

      return ::dir::app_install(strPlatform) / "app.install.exe";

   }


   ::file::path app_plugin_container()
   {

      return ::dir::stage() / "app.plugin.container.exe";

   }


   ::file::path a_spa()
   {

      return ::dir::a_spa() / "a_spa.exe";

   }


   ::file::path a_spaadmin()
   {

      return ::dir::a_spa() / "a_spaadmin.exe";

   }

   ::file::path module(HMODULE hmodule)
   {
#ifdef WINDOWSEX

      hwstring wstr(MAX_PATH * 8);
      
      GetModuleFileNameW(hmodule,wstr,wstr.count());

      return defer_solve_relative_compresions(string(wstr));


#elif defined(METROWIN)

      return "m_app.exe";

#else

      char * lpszModuleFilePath = br_find_exe_dir("app");

      if(lpszModuleFilePath == NULL)
         return "";

      string strModuleFileName(lpszModuleFilePath);

      free(lpszModuleFilePath);

      return strModuleFileName;

#endif

   }

   ::file::path install_log()
   {

      return dir::element() / ("install-" + process_platform_dir_name() + ".log");

   }


} // namespace path