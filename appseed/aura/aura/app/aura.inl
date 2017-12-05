﻿#include "aura/aura/aura/aura.h"


#ifdef METROWIN

//#include "core/core/core.h"
//
//#undef CoreApplication
//

#undef new

[MTAThread]
int main(Array<String^>^)
{

   ap(aura_main_data) pmaindata = new aura_main_data(MAIN_STRING);

   return (int)aura_aura(pmaindata);

}

#include "include/applink.cpp"

#else


#undef new

int main(int argc, char * argv[])
{

   ap(aura_main_data) pmaindata = new aura_main_data(argc, argv);

   return (int)aura_aura(pmaindata);

}


#ifdef WINDOWS

BEGIN_EXTERN_C

int32_t WINAPI _tWinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int32_t nCmdShow)
{

   ap(aura_main_data) pmaindata = new aura_main_data(hinstance, hPrevInstance, lpCmdLine, nCmdShow);

   return aura_aura(pmaindata);

}

END_EXTERN_C

#endif


#endif

