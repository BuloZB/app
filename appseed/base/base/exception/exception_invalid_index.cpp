#include "framework.h"


invalid_index_exception::invalid_index_exception(sp(::base::application) papp) :
   element(papp),
   ::call_stack(papp),
   ::exception::base(papp),
   ::simple_exception(papp),
   invalid_argument_exception(papp)
{

}


invalid_index_exception::invalid_index_exception(sp(::base::application) papp, const char * pszMessage) :
   element(papp),
   ::call_stack(papp),
   ::exception::base(papp),
   ::simple_exception(papp, pszMessage),
   invalid_argument_exception(papp, pszMessage)
{

}


invalid_index_exception::~invalid_index_exception()
{

}
