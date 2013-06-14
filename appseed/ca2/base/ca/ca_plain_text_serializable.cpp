#include "framework.h"


namespace ca2
{


   plain_text_serializable::plain_text_serializable()
   {

   }

   plain_text_serializable::~plain_text_serializable()
   {

   }

   void plain_text_serializable::write(::ca2::writer & writer)
   {
      plain_text_output_stream ostream(&writer);
      write(ostream);
   }

   void plain_text_serializable::read(::ca2::reader & reader)
   {
      plain_text_input_stream istream(&reader);
      read(istream);
   }

}


