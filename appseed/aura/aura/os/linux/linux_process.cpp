//#include "framework.h"

//#include <signal.h>
//#include <unistd.h>


CLASS_DECL_AURA void dll_processes(uint_array & dwa, stringa & straProcesses, const char * pszDll)
{

   UNREFERENCED_PARAMETER(dwa);
   UNREFERENCED_PARAMETER(straProcesses);
   UNREFERENCED_PARAMETER(pszDll);

}






//extern thread_pointer < hthread > t_hthread;


int32_t create_process(const char * _cmd_line, int32_t * pprocessId)
{
   char *   exec_path_name;
   char *	cmd_line;

   cmd_line = (char *) memory_alloc(strlen(_cmd_line ) + 1 );

   if(cmd_line == NULL)
            return 0;

   strcpy_dup(cmd_line, _cmd_line);

   if((*pprocessId = fork()) == 0)
   {
      // child
      char		*pArg, *pPtr;
      char		*argv[1024 + 1];
      int32_t		 argc;
      if( ( pArg = strrchr_dup( exec_path_name, '/' ) ) != NULL )
         pArg++;
      else
         pArg = exec_path_name;
      argv[0] = pArg;
      argc = 1;

      if( cmd_line != NULL && *cmd_line != '\0' )
      {
         pArg = strtok_r_dup(cmd_line, " ", &pPtr);
         while( pArg != NULL )
         {
            argv[argc] = pArg;
            argc++;
            if( argc >= 1024 )
               break;
            pArg = strtok_r_dup(NULL, " ", &pPtr);
         }
      }
      argv[argc] = NULL;

      execv(exec_path_name, argv);
      free(cmd_line);
      exit( -1 );
   }
   else if(*pprocessId == -1)
   {
      // in parent, but error
      *pprocessId = 0;
      free(cmd_line);
      return 0;
   }
   // in parent, success
   return 1;
}

CLASS_DECL_AURA int32_t call_async(
                            const char * pszPath,
                            const char * pszParam,
                            const char * pszDir,
                            int32_t iShow,
		            bool bPrivileged)
{
    string strCmdLine;

    strCmdLine = pszPath;
    if(strlen_dup(pszParam) > 0)
    {
        strCmdLine +=  " ";
        strCmdLine += pszParam;
    }

    int32_t processId;

    if(!create_process(strCmdLine, &processId))
        return -1;

    return 0;

}

CLASS_DECL_AURA DWORD call_sync(const char * pszPath, const char * pszParam, const char * pszDir, int32_t iShow, int32_t iRetry, int32_t iSleep, PFNCALLSYNCONRETRY pfnOnRetry, uint_ptr dwParam)
{

    string strCmdLine;

    strCmdLine = pszPath;
    if(strlen_dup(pszParam) > 0)
    {
        strCmdLine +=  " ";
        strCmdLine += pszParam;
    }

    int32_t processId;

    if(!create_process(strCmdLine, &processId))
        return -1;


    while(true)
    {

        if(kill(processId, 0) == -1 && errno == ESRCH) // No process can be found corresponding to processId
            break;
        sleep(1);
    }

    return 0;
}








string module_path_by_pid(int iPid)
{

	int iSize = 1024 * 8;

    hstring path(iSize);
    char * systemPath = NULL;
    char * candidateDir = NULL;

string str;
str = "/proc/" + ::str::from(iPid) + "/exe";

    /* the easiest case: we are in linux */
    ssize_t s = readlink (str, path, iSize);

    if(s == -1)
    {
        return "";
    }

    path[s] = '\0';

return path;

}



int_array module_path_get_pid(const char * pszPath)
{int_array ia;

::file::patha stra;
	::dir::ls_dir(stra, "/proc/");


	for(auto & strPid : stra)
{
	int iPid = atoi(strPid.title());

if(iPid > 0)
{
string strPath =module_path_by_pid(iPid);
if(strPath	 == pszPath)
{
ia.add(iPid);;
}
}
}

return ia;

}
