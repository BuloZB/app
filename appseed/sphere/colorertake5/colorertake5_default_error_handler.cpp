#include "framework.h" // previously aura/user/user.h
#include "aura/user/colorertake5/colorertake5.h"

namespace colorertake5
{

   DefaultErrorHandler::DefaultErrorHandler()
   {
   }

   DefaultErrorHandler::~DefaultErrorHandler()
   {
   }

   void DefaultErrorHandler::fatalError(const string &msg)
   {
      UNREFERENCED_PARAMETER(msg);
   }

   void DefaultErrorHandler::error(const string &msg)
   {
      UNREFERENCED_PARAMETER(msg);
   }

   void DefaultErrorHandler::warning(const string &msg)
   {
      UNREFERENCED_PARAMETER(msg);
   }


} // namespace colorertake5