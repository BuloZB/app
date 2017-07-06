//#include "framework.h" // from "base/user/user.h"
//#include "base/user/user.h"


namespace user
{


   ::user::document * server::open_new_document()
   {

      return open_document_file(sp(::create)());

   }


   void server::on_request(::create * pcreate)
   {

      sp(::create) cc(canew(::create(Application.creation(), var(var::type_empty), true, NULL)));

      if (pcreate == NULL)
      {
         pcreate = cc;
      }

      request_create(pcreate);

//      return pcreate->m_spCommandLine->m_varQuery["document"].cast < ::user::document >();

   }


   ::user::document * server::open_document_file(var varFile, bool bMakeVisible, ::user::interaction * puiParent)
   {

      sp(::create) cc(canew(::create(Application.creation(), varFile, bMakeVisible, puiParent)));


      return open_document_file(cc);

   }


} // namespace user



