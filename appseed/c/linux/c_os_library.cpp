#include "framework.h"
#include <dlfcn.h>

namespace ca
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
      if(strstr_dup(strPath, ".") == NULL)
         strPath += ".so";
      m_plibrary = dlopen(strPath, RTLD_GLOBAL | RTLD_LAZY);
      return m_plibrary != NULL;
   }

   bool library::close()
   {
      if(m_plibrary != NULL)
      {
         dlclose(m_plibrary);
      }
   }


   void * library::raw_get(const char * pszElement)
   {
      return dlsym(m_plibrary, pszElement);
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

   bool library::is_opened()
   {

      return m_plibrary != NULL;

   }

   bool library::is_closed()
   {

      return !is_opened();

   }

} // namespace ca



