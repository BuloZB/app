#include "StdAfx.h"

#ifndef _X86_
#include <math.h>
#endif

static int lastrand;

extern "C" int _fltused = 0;


CLASS_DECL_____ int abs_dup(int i)
{
   if(i < 0)
      return -i;
   else 
      return i;
}

#ifdef SUPORTA_MATEMATICA_AVANCADA
CLASS_DECL_____ double atan_dup(double d)
{
#ifdef _X86_
    _asm
    {
        fld DWORD PTR [d]
        fatan
        fstp DWORD ptr[d]
    }
    return d;
#else
    return atan(d);
#endif
}

CLASS_DECL_____ double asin_dup(double d)
{
   return 2.0 * atan_dup(d / (1.0 + sqrt_dup(1.0 - d * d)));
}

CLASS_DECL_____ double acos_dup(double d)
{
   return 2.0 * atan_dup(sqrt_dup(1.0 - d * d) / (1.0 + d));
}

CLASS_DECL_____ double sqrt_dup(double d)
{
#ifdef _X86_
    _asm
    {
        fld DWORD PTR [d]
        fsqrt
        fstp DWORD ptr[d]
    }
    return d;
#else
    return sqrt(d);
#endif
}

CLASS_DECL_____ double pow_dup(double base, double e)
{
   return exp_dup(e * log_dup(base));
}


CLASS_DECL_____ double fmod_dup(double x, double y)
{
   return fmod(x, y);
}

CLASS_DECL_____ double exp_dup(double d)
{
#ifdef _X86_
    _asm
    {
        fld DWORD PTR [d]
        fexp
        fstp DWORD ptr[d]
    }
    return d;
#else
    return exp(d);
#endif
}

CLASS_DECL_____ double log_dup(double d)
{
#ifdef _X86_
    _asm
    {
        fld DWORD PTR [d]
        flog
        fstp DWORD ptr[d]
    }
    return d;
#else
    return log(d);
#endif
}

CLASS_DECL_____ double tan_dup(double d)
{
#ifdef _X86_
    _asm
    {
        fld DWORD PTR [d]
        ftan
        fstp DWORD ptr[d]
    }
    return d;
#else
    return tan(d);
#endif
}

CLASS_DECL_____ double sin_dup(double d)
{
#ifdef _X86_
    _asm
    {
        fld DWORD PTR [d]
        fsin
        fstp DWORD ptr[d]
    }
    return d;
#else
    return sin(d);
#endif
}

CLASS_DECL_____ double cos_dup(double d)
{
#ifdef _X86_
    _asm
    {
        fld DWORD PTR [d]
        fcos
        fstp DWORD ptr[d]
    }
    return d;
#else
    return cos(d);
#endif
}

#endif

CLASS_DECL_____ void srand_dup(unsigned int seed)
{
	lastrand = seed;
}

CLASS_DECL_____  int rand_dup()
{
	return (((lastrand = lastrand * 214013L + 2531011L) >> 16) & 0x7FFF);
}


/*__int64 ftoi64(double d)
{
   __int64 u = 0x7fffffffffffffff;
   __int64 l = 0x8000000000000000;
   double m;
   while(true)
   {
      if(u <= l)
         return l;
      m = ((double) u + (double) l) / 2.0;
      if(d >= m)
      {
         l = u / 2 + l / 2;
      }
      else
      {
         u = u / 2 + l / 2;
      }
   }
}*/

int ftol(double d)
{
   int u = 0x7fffffff;
   int l = 0x80000000;
   double m;
   double delta;
   int i = 0;
   while(i < 128)
   {
      if(u <= l)
         return l;
      m = ((double) u + (double) l) / 2.0;
      delta = d - m;
      if(delta >= 0.0)
      {
         if(delta <= 0.5)
            return l;
         l = u / 2 + l / 2;
      }
      else
      {
         if(delta >= -0.5)
            return l;
         u = u / 2 + l / 2;
      }
      i++;
   }
   return l;
}