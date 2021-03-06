#include "framework.h"

#include <sys/resource.h>

/*os_thread::os_thread(DWORD (WINAPI * pfn)(LPVOID), LPVOID pv)
{
   m_pfn = pfn;
   m_pv = pv;
}




HTHREAD start_thread(DWORD (WINAPI * pfn)(LPVOID), LPVOID pv, int iPriority)
{

   UNREFERENCED_PARAMETER(iPriority);

   return create_thread(NULL, 0, pfn, pv, 0, NULL);

}

HTHREAD create_thread(LPSECURITY_ATTRIBUTES lpsa, DWORD cbStack, DWORD (WINAPI * pfn)(LPVOID), LPVOID pv, DWORD f, LPDWORD lpdwId)
{

   os_thread * pthread = new os_thread(pfn, pv);

   pthread_t thread;

   pthread_attr_t threadAttr;

//   struct sched_param param;  // scheduling priority

   // initialize the thread attribute
   pthread_attr_init(&threadAttr);

   if(cbStack > 0)
   {
      // Set the stack size of the thread
      pthread_attr_setstacksize(&threadAttr, 120*1024);
   }

   // Set thread to detached state. No need for pthread_join
   pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);

   // Create the thread
   pthread_create(&thread, &threadAttr, &thread_proc_create_thread, (LPVOID) pthread);


   return (HANDLE) pthread;

}



thread_layer::~thread_layer()
{


}




void thread_layer::wait_thread(DWORD dwMillis)
{
   
   _throw(simple_exception(get_app(), "not implemented"));

   
}




DWORD WINAPI thread_layer::proc(LPVOID lp)
{
   
   thread_layer * player   = (thread_layer *) lp;
   
 //  player->m_hthread       = ::GetCurrentThread();
   
   //player->m_nId           = ::GetCurrentThreadId();
   
   return player->run();
   
}*/

/*

namespace ca2
{

///  \brief		global function to set thread priority for current thread
	///  \param		new priority
	CLASS_DECL_AXIS bool set_thread_priority(int32_t priority)
	{

      return SetThreadPrioriy(GetCurrentThread(), priority) != FALSE;
//		return setpriority(PRIO_PROCESS, getpid(), priority) == 0;

	}


	///  \brief		global function to get thread priority for current thread
	///  \return	priority of current thread

	CLASS_DECL_AXIS int32_t thread_priority()
	{

      return GetThreadPrioriy(GetCurrentThread());
	//   return getpriority(PRIO_PROCESS, getpid());

   }
   
   
} // namespace ca2

*/


void thread_get_os_priority(int32_t * piPolicy, sched_param * pparam, int32_t nCa2Priority)
{
   
   int iOsPolicy;
   
   int iCa2Min;
   
   int iCa2Max;
   
   int iOsMax = sched_get_priority_max(SCHED_RR);
   
   int iOsMin = sched_get_priority_min(SCHED_RR);
   
   if(nCa2Priority == ::core::scheduling_priority_normal)
   {
      
      iOsPolicy = SCHED_RR; // iOsPolicy = SCHED_OTHER;
      
      iCa2Min = (int) ::core::scheduling_priority_normal;
      
      iCa2Max = (int) ::core::scheduling_priority_normal;
      
      iOsMin = (iOsMin + iOsMax) / 2;
      
      iOsMax = iOsMin;
      
   }
   else if(nCa2Priority > ::core::scheduling_priority_normal)
   {
      
      iOsPolicy = SCHED_RR;
      
      iCa2Min = (int) ::core::scheduling_priority_normal;
      
      iCa2Max = 99;
      
      iOsMin = (iOsMin + iOsMax) / 2;
      
   }
   else
   {
      
      iOsPolicy = SCHED_RR; // iOsPolicy = SCHED_IDLE;
      
      iCa2Min = 0;
      
      iCa2Max = (int) ::core::scheduling_priority_normal;
      
      iOsMax = (iOsMin + iOsMax) / 2;
      
   }
   
   
   int iOsPriority;
   
   if(iCa2Min == iCa2Max)
   {
      iOsPriority = 0;
   }
   else
   {
      iOsPriority = (((nCa2Priority - iCa2Min)  * (iOsMax - iOsMin)) / (iCa2Max - iCa2Min)) + iOsMin;
   }
   
   iOsPriority = MAX(iOsMin, MIN(iOsMax, iOsPriority));
   
   *piPolicy = iOsPolicy;
   
   pparam->sched_priority = iOsPriority;
   
}

int32_t thread_get_scheduling_priority(int32_t iOsPolicy, const sched_param * pparam)
{
   
   int iCa2Min;
   
   int iCa2Max;
   
   
//   if(iOsPolicy == SCHED_RR)
   {
      
      iCa2Min = ::core::scheduling_priority_idle;
      
      iCa2Max = 99;
      
   }
/*
   else if(iOsPolicy == SCHED_IDLE)
   {
      
      iCa2Min = 0;
      
      iCa2Max = (int) ::core::scheduling_priority_normal;
      
   }
   else
   {
      
      iCa2Min = (int) ::core::scheduling_priority_normal;
      
      iCa2Max = (int) ::core::scheduling_priority_normal;
      
   }
*/
   
   int iOsMax = sched_get_priority_max(iOsPolicy);
   
   int iOsMin = sched_get_priority_min(iOsPolicy);
   
   int iCa2Priority;
   
   if(iOsMax == iOsMin)
   {
      iCa2Priority = (int32_t) ::core::scheduling_priority_normal;
   }
   else
   {
      iCa2Priority = (((pparam->sched_priority - iOsMin)  * (iCa2Max - iCa2Min)) / (iOsMax - iOsMin)) + iCa2Min;
   }
   
   iCa2Priority = MAX(iCa2Min, MIN(iCa2Max, iCa2Priority));
   
   return iCa2Priority;
   
}


void process_get_os_priority(int32_t * piPolicy, sched_param * pparam, int32_t nCa2Priority)
{
   
   _throw(simple_exception(get_app(), "not applicable in Mac OS"));
   
}

int32_t process_get_scheduling_priority(int32_t iOsPolicy, const sched_param * pparam)
{
   
   _throw(simple_exception(get_app(), "not applicable in Mac OS"));
   
}


int32_t process_get_os_priority(int32_t nCa2Priority)
{
   
   if(nCa2Priority <= (int) ::core::scheduling_priority_none)
      return 0;
   
   if(nCa2Priority <= (int) ::core::scheduling_priority_normal)
      return MAX(-20, MIN(0, -20 * ((int) ::core::scheduling_priority_normal - nCa2Priority) / ((int) ::core::scheduling_priority_normal - (int) ::core::scheduling_priority_idle)));
   
   return MAX(0, MIN(20, 20 * (nCa2Priority - (int) ::core::scheduling_priority_normal) / ((int) ::core::scheduling_priority_time_critical - (int) ::core::scheduling_priority_normal)));
   
}


int32_t process_get_scheduling_priority(int32_t iOsPriority)
{
   
   if(iOsPriority < -15)
      return ::core::scheduling_priority_idle;
   
   if(iOsPriority < -10)
      return ::core::scheduling_priority_lowest;

   if(iOsPriority < 0)
      return ::core::scheduling_priority_below_normal;
   
   if(iOsPriority == 0)
      return ::core::scheduling_priority_normal;
   
   if(iOsPriority < 10)
      return ::core::scheduling_priority_above_normal;
   
   if(iOsPriority < 15)
      return ::core::scheduling_priority_highest;
   
   return ::core::scheduling_priority_time_critical;

   
}


namespace ca2
{



   bool set_priority_class(int32_t iCa2Priority)
   {
   
      int iOsPriority = process_get_os_priority(iCa2Priority);
   
      setpriority(PRIO_PROCESS, 0, iOsPriority);
   
      return true;
   
   }



} // namespace ca2






void * new_ns_pool();
void release_pool(void * pool);

thread_pointer < void > g_ns_pool;


void on_start_thread()
{
   
   g_ns_pool = new_ns_pool();
   
}


void on_end_thread()
{
   
   release_pool(g_ns_pool);
   
   g_ns_pool = NULL;
   
}