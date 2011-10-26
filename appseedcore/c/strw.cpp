#include "StdAfx.h"
#include "x/x_defines.h"
#include "x/x_tables.h"
#include "x/x_charcategory_names.h"
#include "x/x_charcategory.h"




int wcslen_dup(const wchar_t * str)
{
   const wchar_t * pszEnd = str;
   while(*pszEnd != 0)
      pszEnd++;
   return pszEnd - str;
}

CLASS_DECL_____ int iswalpha_dup(int ch)
{
   return (ch >= L'A' && ch <= L'Z') || (ch >= L'a' && ch <= L'z');
}


CLASS_DECL_____ int iswdigit_dup(int ch)
{
   return ch >= L'0' && ch <= L'9';
}


CLASS_DECL_____ int iswspace_dup(int ch)
{
   return ch == L' ' || ch == L'\t' || ch == L'\r' || ch == L'\n' || ch == L'\t';
}


int wcsicmp_dup(const wchar_t * sz1, const wchar_t * sz2)
{
   if(sz1 == NULL)
   {
      if(sz2 == NULL)
         return 0;
      else
         return -1;
   }
   else if(sz2 == NULL)
   {
      return 1;
   }
   int iCmp = 0;
   while(*sz1 != L'\0' && *sz2 != L'\0')
   {
      iCmp = w_to_lower(*sz1) - w_to_lower(*sz2);
      if(iCmp != 0)
         return iCmp;
      sz1++;
      sz2++;
   }
   return iCmp;
}


CLASS_DECL_____ wchar_t w_to_lower(int c)
{
   unsigned long c1 = CHAR_PROP(c);
   if (CHAR_CATEGORY(c1) == CHAR_CATEGORY_Ll) return wchar_t(c);
   if (CHAR_CATEGORY(c1) == CHAR_CATEGORY_Lt) return wchar_t(c+1);
   return wchar_t(c - (c1>>16));
}


CLASS_DECL_____ int iswalnum_dup(int wch)
{
   if(wch >= 256 || wch <= -128)
      return 0;
   return isdigit_dup(wch) || isalpha_dup(wch);
}


CLASS_DECL_____ wchar_t * wcschr_dup(const wchar_t * sz, wchar_t ch)
{
   if(sz == NULL)
      return NULL;
   while(true)
   {
      if(*sz == ch)
         return (wchar_t *) sz;
      if(*sz == L'\0')
         return NULL;
      sz++;
   }
}


#define NULWCHAR     L'\0'
#define TABWCHAR     L'\t'
#define SLASHWCHAR   L'\\'
#define SPACEWCHAR   L' '
#define DQUOTEWCHAR  L'\"'





void __cdecl wparse_cmdline(WCHAR *cmdstart, WCHAR **argv, WCHAR *args, int * numargs, int * numchars)
{
        WCHAR *p;
        WCHAR c;
        int inquote;                    /* 1 = inside quotes */
        int copychar;                   /* 1 = copy char to *args */
        unsigned numslash;              /* num of backslashes seen */

        *numchars = 0;
        *numargs = 1;                   /* the program name at least */

        /* first scan the program name, copy it, and count the bytes */
        p = cmdstart;
        if (argv)
            *argv++ = args;

#ifdef WILDCARD
        /* To handle later wild card expansion, we prefix each entry by
        it's first character before quote handling.  This is done
        so _[w]cwild() knows whether to expand an entry or not. */
        if (args)
            *args++ = *p;
        ++*numchars;

#endif  /* WILDCARD */

        /* A quoted program name is handled here. The handling is much
           simpler than for other arguments. Basically, whatever lies
           between the leading double-quote and next one, or a terminal null
           character is simply accepted. Fancier handling is not required
           because the program name must be a legal NTFS/HPFS file name.
           Note that the double-quote characters are not copied, nor do they
           contribute to numchars. */
        if ( *p == DQUOTEWCHAR ) {
            /* scan from just past the first double-quote through the next
               double-quote, or up to a null, whichever comes first */
            while ( (*(++p) != DQUOTEWCHAR) && (*p != NULWCHAR) ) {

/*#ifdef _MBCS
                if (_ismbblead(*p)) {
                    ++*numchars;
                    if ( args )
                        *args++ = *p++;
                }
#endif  /* _MBCS */
                ++*numchars;
                if ( args )
                    *args++ = *p;
            }
            /* append the terminating null */
            ++*numchars;
            if ( args )
                *args++ = NULWCHAR;

            /* if we stopped on a double-quote (usual case), skip over it */
            if ( *p == DQUOTEWCHAR )
                p++;
        }
        else {
            /* Not a quoted program name */
            do {
                ++*numchars;
                if (args)
                    *args++ = *p;

                c = (WCHAR) *p++;
/*#ifdef _MBCS
                if (_ismbblead(c)) {
                    ++*numchars;
                    if (args)
                        *args++ = *p;   /* copy 2nd byte too */
    //                p++;  /* skip over trail byte */
  //              }
//#endif  /* _MBCS */

            } while ( c != SPACEWCHAR && c != NULWCHAR && c != TABWCHAR );

            if ( c == NULWCHAR ) {
                p--;
            } else {
                if (args)
                    *(args-1) = NULWCHAR;
            }
        }

        inquote = 0;

        /* loop on each argument */
        for(;;) {

            if ( *p ) {
                while (*p == SPACEWCHAR || *p == TABWCHAR)
                    ++p;
            }

            if (*p == NULWCHAR)
                break;              /* end of args */

            /* scan an argument */
            if (argv)
                *argv++ = args;     /* store ptr to arg */
            ++*numargs;

#ifdef WILDCARD
        /* To handle later wild card expansion, we prefix each entry by
        it's first character before quote handling.  This is done
        so _[w]cwild() knows whether to expand an entry or not. */
        if (args)
            *args++ = *p;
        ++*numchars;

#endif  /* WILDCARD */

        /* loop through scanning one argument */
        for (;;) {
            copychar = 1;
            /* Rules: 2N backslashes + " ==> N backslashes and begin/end quote
               2N+1 backslashes + " ==> N backslashes + literal "
               N backslashes ==> N backslashes */
            numslash = 0;
            while (*p == SLASHWCHAR) {
                /* count number of backslashes for use below */
                ++p;
                ++numslash;
            }
            if (*p == DQUOTEWCHAR) {
                /* if 2N backslashes before, start/end quote, otherwise
                    copy literally */
                if (numslash % 2 == 0) {
                    if (inquote) {
                        if (p[1] == DQUOTEWCHAR)
                            p++;    /* Double quote inside quoted string */
                        else        /* skip first quote char and copy second */
                            copychar = 0;
                    } else
                        copychar = 0;       /* don't copy quote */

                    inquote = !inquote;
                }
                numslash /= 2;          /* divide numslash by two */
            }

            /* copy slashes */
            while (numslash--) {
                if (args)
                    *args++ = SLASHWCHAR;
                ++*numchars;
            }

            /* if at end of arg, break loop */
            if (*p == NULWCHAR || (!inquote && (*p == SPACEWCHAR || *p == TABWCHAR)))
                break;

            /* copy character into argument */
/*#ifdef _MBCS
            if (copychar) {
                if (args) {
                    if (_ismbblead(*p)) {
                        *args++ = *p++;
                        ++*numchars;
                    }
                    *args++ = *p;
                } else {
                    if (_ismbblead(*p)) {
                        ++p;
                        ++*numchars;
                    }
                }
                ++*numchars;
            }
            ++p;
#else  /* _MBCS */
            if (copychar) {
                if (args)
                    *args++ = *p;
                ++*numchars;
            }
            ++p;
//#endif  /* _MBCS */
            }

            /* null-terminate the argument */

            if (args)
                *args++ = NULWCHAR;          /* terminate string */
            ++*numchars;
        }

        /* We put one last argument in -- a null ptr */
        if (argv)
            *argv++ = NULL;
        ++*numargs;
}








void wcscat_dup(wchar_t * dest, const wchar_t * cat)
{
   if(dest == NULL)
      return;
   if(cat == NULL)
      return;
   while(*dest != 0)
      dest++;
   while(*cat != 0)
      *dest++ = *cat++;
   *dest = L'\0';
}
