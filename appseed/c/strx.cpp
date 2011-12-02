#include "StdAfx.h"
#include "ca/x/x_defines.h"
#include "ca/x/x_tables.h"
#include "ca/x/x_charcategory_names.h"
#include "ca/x/x_charcategory.h"

      /* --------------------------------------------------------------------- */

      /*
       * Index into the table below with the first byte of a UTF-8 sequence to
       * get the number of trailing bytes that are supposed to follow it.
       * Note that *legal* UTF-8 values can't have 4 or 5-bytes. The table is
       * left as-is for anyone who may want to do such conversion, which was
       * allowed in earlier algorithms.
       */
      static const char trailingBytesForUTF8[256] = {
          -1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
          1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
          2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
      };

      /*
       * Magic values subtracted from a buffer value during UTF8 conversion.
       * This table contains as many values as there might be trailing bytes
       * in a UTF-8 sequence.
       */
      static const unsigned int offsetsFromUTF8[6] = { 0x00000000UL, 0x00003080UL, 0x000E2080UL,
                 0x03C82080UL, 0xFA082080UL, 0x82082080UL };


   const char * utf8_inc(const char * psz)
   {
      char len =  1 + trailingBytesForUTF8[(unsigned char) *psz];
      if(len == 0)      return NULL;
      if(*psz++ == 0)   return NULL;
      if(len == 1)      return psz;
      if(*psz++ == 0)   return NULL;
      if(len == 2)      return psz;
      if(*psz++ == 0)   return NULL;
      if(len == 3)      return psz;
      if(*psz++ == 0)   return NULL;
      if(len == 4)      return psz;
      if(*psz++ == 0)   return NULL;
      if(len == 5)      return psz;
      if(*psz++ == 0)   return NULL;
      if(len == 6)      return psz;
      return NULL;
   }


int uni_index(const char * pszUtf8)
{
unsigned char * source = (unsigned char *) pszUtf8;
int ch = 0;
int extraBytesToRead = trailingBytesForUTF8[*source];
/*         if(natural(extraBytesToRead) >= strlen(pszUtf8))
}*/
//         if(natural(extraBytesToRead) >= strlen(pszUtf8))
//       {
//        return -1; // source exhausted
//   }
if(*source == '\0') return -1;
switch (extraBytesToRead)
{
   case 5: ch += *source++; ch <<= 6;
       if(*source == '\0') return -1;
   case 4: ch += *source++; ch <<= 6;
       if(*source == '\0') return -1;
   case 3: ch += *source++; ch <<= 6;
       if(*source == '\0') return -1;
   case 2: ch += *source++; ch <<= 6;
       if(*source == '\0') return -1;
   case 1: ch += *source++; ch <<= 6;
       if(*source == '\0') return -1;
   case 0: ch += *source++;
}
ch -= offsetsFromUTF8[extraBytesToRead];
return ch;
}

int uni_to_utf8(char * psz, int w)
{
   if(w < 0x0080 )
   {
      psz[0] = (char) w;
      return 1;
   }
   else if(w < 0x0800)
   {
      psz[0] = (char)(0xc0 | ((w) >> 6));
      psz[1] = (char)(0x80 | ((w) & 0x3f));
      return 2;
   }
   else
   {
      psz[0] = (char)(0xe0 | ((w) >> 12));
      psz[1] = (char)(0x80 | (((w) >> 6) & 0x3f));
      psz[2] = (char)(0x80 | ((w) & 0x3f));
      return 3;
   }
}


int utf16_len(const char * psz)
{
   if(psz == NULL)
      return -1;
   int count = 0;
   while((psz = utf8_inc(psz)) != NULL)
   {
      count++;
   }
   return count;
}

void utf8_to_16(wchar_t * pwsz, const char * psz)
{
   while(psz != NULL && *psz != '\0')
   {
      *pwsz++ = uni_index(psz);
      psz = utf8_inc(psz);
   }
   *pwsz = L'\0';
}


WCHAR * utf8_to_16(const char * psz)
{
   int iCount = utf16_len(psz);
   if(iCount < 0)
      return NULL;
   WCHAR * pwz = new WCHAR[iCount + 1];
   utf8_to_16(pwz, psz);
   return pwz;
}

int utf8_len(const wchar_t * pwsz)
{
   if(pwsz == NULL)
      return -1;
   int count = 0;
   int n;
   char sz[16];
   while(*pwsz != L'\0')
   {
      n = uni_to_utf8(sz, *pwsz);
      if(n <= 0)
         break;
      count += n;
      pwsz++;
   }
   return count;
}

void utf16_to_8(char * psz, const wchar_t * pwsz)
{
   int n;
   while(*pwsz != L'\0')
   {
      n = uni_to_utf8(psz, *pwsz);
      if(n <= 0)
         break;
      pwsz++;
      psz += n;
   }
   *psz = L'\0';
}


char * utf16_to_8(const wchar_t * pwsz)
{
   int iCount = utf8_len(pwsz);
   if(iCount < 0)
      return NULL;
   char * psz = new char[iCount + 1];
   utf16_to_8(psz, pwsz);
   return psz;
}






















