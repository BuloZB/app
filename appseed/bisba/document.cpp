#include "StdAfx.h"

namespace bisba
{

   document::document(::ca::application * papp) :
      ca(papp),
      ::ca::data_container(papp),
      ::document(papp),
      ::userbase::document(papp),
      m_spdib(papp)
   {
   }

   document::~document()
   {
   }


#ifdef _DEBUG
   void document::assert_valid() const
   {
	   ::userbase::document::assert_valid();
   }

   void document::dump(dump_context & dumpcontext) const
   {
	   ::userbase::document::dump(dumpcontext);
   }
#endif //_DEBUG


   bool document::on_open_document(var varFile)
   {

      if(!m_spdib.load_from_file(varFile))
         return false;

      return true;

   }

} // namespace bisba