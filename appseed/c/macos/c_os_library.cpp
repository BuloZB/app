#include "framework.h"
#include <dlfcn.h>


namespace c
{


   library::library()
   {
      
      m_plibrary = NULL;
      
   }

   
   library::library(const char * pszOpen)
   {
      
      m_plibrary = NULL;
      
      open(pszOpen);
      
   }
   

   library::~library()
   {
      
      close();
      
   }
   

   bool library::open(const char * pszPath)
   {
      
      vsstring strPath(pszPath);
      
      if(strPath == "os")
      {
         strPath = "ca2os";
      }
      else if(strPath == "os2")
      {
         strPath = "ca2os2";
      }
      
      if(!str_ends_dup(strPath, ".dylib"))
         strPath += ".dylib";

      if(!str_begins_dup(strPath, "lib"))
         strPath = "lib" + strPath;
      
      m_plibrary = dlopen(strPath, RTLD_LOCAL | RTLD_LAZY);
      
      return true;
      
   }
   

   bool library::close()
   {
      
      if(m_plibrary != NULL)
      {
         
         dlclose(m_plibrary);
         
      }
      
      return true;
      
   }


   void * library::raw_get(const char * pszElement)
   {
      
      return dlsym(m_plibrary, pszElement);
      
   }
   
   
   bool library::is_opened()
   {
      
      return m_plibrary != NULL;
      
   }
   
   
   bool library::is_closed()
   {
      
      return m_plibrary == NULL;
      
   }

   
   ca2_library::ca2_library()
   {
      
   }

   
   ca2_library::ca2_library(const char * pszOpen) :
      library(pszOpen)
   {

   }

   
   ca2_library::~ca2_library()
   {

   }

   
   bool ca2_library::open(const char * pszPath)
   {
      return library::open(pszPath);
   }
   
   
} // namespace ca



