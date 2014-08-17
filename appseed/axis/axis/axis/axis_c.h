#pragma once


#ifndef __cplusplus

//#include <assert.h>

#define ASSERT assert

#endif

#define low_byte(w)              ((BYTE)(w & 0xff))
#define argb_get_r_value(rgb)    (low_byte(rgb))
#define argb_get_g_value(rgb)    (low_byte((rgb)>>8))
#define argb_get_b_value(rgb)    (low_byte((rgb)>>16))
#define argb_get_a_value(rgb)    (low_byte((rgb)>>24))
#define RGBA(r, g, b, a)         ((COLORREF)((low_byte(r)|((WORD)(low_byte(g))<<8))|(((uint32_t)low_byte(b))<<16)|(((uint32_t)low_byte(a))<<24)))
#define ARGB(a, r, g, b)         RGBA(r, g, b, a)


#define _(c_string_to_be_translated_from_english) c_string_to_be_translated_from_english


#include "primitive/math/math_mkint_c.h"

#if defined(ANDROID)
#include "axis/axis/os/android/android.h"
#elif defined(MACOS)
#include "axis/axis/os/macos/macos.h"
#elif defined(LINUX)
#include "axis/axis/os/linux/linux.h"
#elif defined(METROWIN)
#include "axis/axis/os/metrowin/metrowin.h"
#elif defined(WINDOWS)
#include "axis/axis/os/windows/windows.h"
#elif defined(SOLARIS)
#include "axis/axis/os/solaris/solaris.h"
#elif defined(APPLE_IOS)
#include "axis/axis/os/ios/ios.h"
#else
#error Not Supported Operational System
#endif


#include "axis/axis/crypto/crypto/crypto.h"

#include "axis/net/idn/idna.h"


CLASS_DECL_BASE int get_base_init();
CLASS_DECL_BASE int_bool defer_base_init();
CLASS_DECL_BASE int_bool defer_base_term();
