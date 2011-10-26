#include "StdAfx.h"

void * DECL_C _malloc_dbg(size_t _Size, int _BlockType, const char * _Filename, int _LineNumber)
{
   return malloc(_Size);
}

void * DECL_C _calloc_dbg(size_t _Count, size_t _Size, int _BlockType, const char * _Filename, int _LineNumber)
{
   return malloc(_Count * _Size);
}

void * DECL_C _realloc_dbg(void * _Memory, size_t _NewSize, int _BlockType, const char * _Filename, int _LineNumber)
{
   return realloc(_Memory, _NewSize);
}

void * DECL_C _recalloc_dbg(void * _Memory, size_t _NumOfElements, size_t _SizeOfElements, int _BlockType, const char * _Filename, int _LineNumber)
{
   return realloc(_Memory, _NumOfElements * _SizeOfElements);
}

void * DECL_C _expand_dbg(void * _Memory, size_t _NewSize, int _BlockType, const char * _Filename, int _LineNumber)
{
   return realloc(_Memory, _NewSize);
}

void DECL_C _free_dbg(void * _Memory, int _BlockType)
{
   free(_Memory);
}



CLASS_DECL__ int DECL_C _CrtDbgReportW(
        _In_ int _ReportType,
        _In_opt_z_ const wchar_t * _Filename,
        _In_ int _LineNumber,
        _In_opt_z_ const wchar_t * _ModuleName,
        _In_opt_z_ const wchar_t * _Format,
        ...)
{
   return 0;
}


CLASS_DECL__ int __cdecl _CrtReportBlockType(const void * p)
{
   return _AFX_CLIENT_BLOCK;
}

CLASS_DECL__ int __cdecl _CrtSetReportMode(int,int)
{
   return TRUE;
}

CLASS_DECL__ int __cdecl _CrtSetReportHook2(int,int (__cdecl*)(int,char *,int *))
{
   return TRUE;
}

CLASS_DECL__ _CRT_DUMP_CLIENT DECL_C _CrtSetDumpClient(_CRT_DUMP_CLIENT _PFnNewDump)
{
   return NULL;
}

CLASS_DECL__ int __cdecl _CrtSetDbgFlag(int)
{
   return TRUE;
}

CLASS_DECL__ int __cdecl _CrtDumpMemoryLeaks(void)
{
   return TRUE;
}

CLASS_DECL__ int DECL_C _CrtCheckMemory(void)
{
   return 1;
}