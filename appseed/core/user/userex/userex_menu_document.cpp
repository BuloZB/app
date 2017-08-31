#include "framework.h"


menu_document::menu_document(::aura::application * papp) :
   object(papp),
   ::data::data_container_base(papp),
   ::user::controller(papp),
   ::user::document(papp)
{

}


void menu_document::OnBeforeNavigate2(::html::data * pdata, var & varFile, uint32_t nFlags, const char * lpszTargetFrameName, byte_array& baPostedData, const char * lpszHeaders, bool* pbCancel)
{

   ::user::document::OnBeforeNavigate2(pdata, varFile, nFlags, lpszTargetFrameName, baPostedData, lpszHeaders, pbCancel);

}


