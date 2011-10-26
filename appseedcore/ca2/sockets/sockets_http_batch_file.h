#pragma once

namespace sockets
{

   namespace http
   {

      class CLASS_DECL_ca batch_file : 
         virtual public ::ex1::file
      {
      public:

         UINT                 m_nOpenFlags;
         string               m_strPath;



         batch_file(::ca::application * papp);

         virtual BOOL open(const char * lpszFileName, UINT nOpenFlags, ::ex1::file_exception_sp * pError = NULL);
         virtual void close();
         virtual void Flush();

         virtual void get_file_data() = 0;
         virtual void set_file_data() = 0;

      };

   } // namespace http

} // namespace sockets