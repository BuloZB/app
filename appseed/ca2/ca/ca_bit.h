#pragma once


namespace ca
{
   namespace bit
   {
      CLASS_DECL_ca2 void int_aligned_copy(int32_t * pDest, int32_t iDest, int32_t * pSrc, int32_t iSrc, int32_t iCount);
      CLASS_DECL_ca2 void int_aligned_copy(int32_t * pDest, int32_t * pSrc, int32_t start, int32_t end);
      CLASS_DECL_ca2 void set(void * pDest, bool b, int32_t start, int32_t end);
      CLASS_DECL_ca2 void byte_set(uchar * pDest, bool b, int32_t start, int32_t end);
   }
}
