#pragma once


namespace str
{


   namespace international
   {


//      void CLASS_DECL_AXIS unicode_to_utf8(string_array & stra, string_array & stra);
  //    void CLASS_DECL_AXIS utf8_to_unicode(string_array & stra, string_array & stra);


      bool CLASS_DECL_AXIS UnicodeToMultiByte(UINT uiCodePage, primitive::memory & str, const wchar_t * lpcsz);

      bool CLASS_DECL_AXIS MultiByteToMultiByte(UINT uiCodePageDest, primitive::memory & str, UINT uiCodePageSrc, const char * lpcsz);
      bool CLASS_DECL_AXIS MultiByteToMultiByte(UINT uiCodePageDest, primitive::memory & str, UINT uiCodePageSrc, const char * lpcsz, strsize nCount);

      bool CLASS_DECL_AXIS multibyte_to_utf8(UINT uiCodePageSrc, primitive::memory & str, const char * lpcsz);
      bool CLASS_DECL_AXIS multibyte_to_utf8(UINT uiCodePageSrc, primitive::memory & str, const char * lpcsz, strsize nCount);

//      bool CLASS_DECL_AXIS OEMToMultiByte(UINT uiCodePage, primitive::memory & str, const char * lpcsz);
//      bool CLASS_DECL_AXIS OEMToMultiByte(UINT uiCodePage, primitive::memory & str, const char * lpcsz, strsize nCount);

      bool CLASS_DECL_AXIS utf8_to_multibyte(UINT uiCodePage, primitive::memory & str, const char * lpcsz);
      bool CLASS_DECL_AXIS utf8_to_multibyte(UINT uiCodePage, primitive::memory & str, const char * lpcsz, strsize nCount);

//      bool CLASS_DECL_AXIS OemToUtf8(primitive::memory & str, const char * lpcsz);
//      bool CLASS_DECL_AXIS OemToUtf8(primitive::memory & str, const char * lpcsz, strsize nCount);

/*      inline void unicode_to_utf8(string_array & stra, string_array & stra)
      {
         string str;
         for(strsize i = 0; i < stra.get_size(); i++)
         {
            unicode_to_utf8(str, stra[i]);
            stra.add(str);
         }
      }

      inline void utf8_to_unicode(string_array & stra, string_array & stra)
      {
         string str;
         for(strsize i = 0; i < stra.get_size(); i++)
         {
            utf8_to_unicode(str, stra[i]);
            stra.add(str);
         }
      }*/

      inline bool multibyte_to_utf8(UINT uiCodePageSrc, primitive::memory & str, const char * lpcsz)
      {
         return MultiByteToMultiByte(CP_UTF8, str, uiCodePageSrc, lpcsz);
      }

      inline bool multibyte_to_utf8(UINT uiCodePageSrc, primitive::memory & str, const char * lpcsz, strsize nCount)
      {
         return MultiByteToMultiByte(CP_UTF8, str, uiCodePageSrc, lpcsz, nCount);
      }

      //inline bool OemToUtf8(primitive::memory & str, const char * lpcsz)
      //{
      //   return MultiByteToMultiByte(CP_UTF8, str, CP_OEMCP, lpcsz);
      //}

      //inline bool OemToUtf8(primitive::memory & str, const char * lpcsz, strsize nCount)
      //{
      //   return MultiByteToMultiByte(CP_UTF8, str, CP_OEMCP, lpcsz, nCount);
      //}

      inline bool utf8_to_multibyte(UINT uiCodePage, primitive::memory & str, const char * lpcsz)
      {
         return MultiByteToMultiByte(CodePageUtf8, str, uiCodePage, lpcsz);
      }

      inline bool utf8_to_multibyte(UINT uiCodePage, primitive::memory & str, const char * lpcsz, strsize nCount)
      {
         return MultiByteToMultiByte(CodePageUtf8, str, uiCodePage, lpcsz, nCount);
      }


   } // namespace international


} // namespace str



