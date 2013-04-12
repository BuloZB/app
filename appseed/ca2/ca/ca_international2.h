#pragma once


namespace ca
{


   namespace international
   {


//      void CLASS_DECL_ca2 unicode_to_utf8(stringa & stra, stringa & stra);
  //    void CLASS_DECL_ca2 utf8_to_unicode(stringa & stra, stringa & stra);


      bool CLASS_DECL_ca2 UnicodeToMultiByte(UINT uiCodePage, primitive::memory & str, const wchar_t * lpcsz);

      bool CLASS_DECL_ca2 MultiByteToMultiByte(UINT uiCodePageDest, primitive::memory & str, UINT uiCodePageSrc, const char * lpcsz);
      bool CLASS_DECL_ca2 MultiByteToMultiByte(UINT uiCodePageDest, primitive::memory & str, UINT uiCodePageSrc, const char * lpcsz, strsize nCount);

      bool CLASS_DECL_ca2 multibyte_to_utf8(UINT uiCodePageSrc, primitive::memory & str, const char * lpcsz);
      bool CLASS_DECL_ca2 multibyte_to_utf8(UINT uiCodePageSrc, primitive::memory & str, const char * lpcsz, strsize nCount);

//      bool CLASS_DECL_ca2 OEMToMultiByte(UINT uiCodePage, primitive::memory & str, const char * lpcsz);
//      bool CLASS_DECL_ca2 OEMToMultiByte(UINT uiCodePage, primitive::memory & str, const char * lpcsz, strsize nCount);

      bool CLASS_DECL_ca2 utf8_to_multibyte(UINT uiCodePage, primitive::memory & str, const char * lpcsz);
      bool CLASS_DECL_ca2 utf8_to_multibyte(UINT uiCodePage, primitive::memory & str, const char * lpcsz, strsize nCount);

//      bool CLASS_DECL_ca2 OemToUtf8(primitive::memory & str, const char * lpcsz);
//      bool CLASS_DECL_ca2 OemToUtf8(primitive::memory & str, const char * lpcsz, strsize nCount);

/*      inline void unicode_to_utf8(stringa & stra, stringa & stra)
      {
         string str;
         for(strsize i = 0; i < stra.get_size(); i++)
         {
            unicode_to_utf8(str, stra[i]);
            stra.add(str);
         }
      }

      inline void utf8_to_unicode(stringa & stra, stringa & stra)
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


} // namespace ca



