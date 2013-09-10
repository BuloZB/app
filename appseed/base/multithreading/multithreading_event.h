#pragma once

#ifdef LINUX
#include <sys/types.h>
#include <sys/sem.h>
union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                (Linux-specific) */
};
#endif

class CLASS_DECL_c event :
   virtual public event_base
{
public:

#if defined(LINUX) || defined(MACOS) || defined(ANDROID)
   bool              m_bManualEvent;
   bool              m_bSignaled;  // meaningful only when m_bManualEvent
   int32_t           m_iSignalId;  // meaningful only when m_bManualEvent
   pthread_cond_t    m_cond;
   pthread_mutex_t   m_mutex;
#endif


   event(sp(base_application) papp, bool bInitiallyOwn = FALSE, bool bManualReset = FALSE, const char * lpszNAme = NULL, LPSECURITY_ATTRIBUTES lpsaAttribute = NULL);
   virtual ~event()
   {

   #if defined(LINUX)

      semun ignored_argument;

      semctl(m_object, 0, IPC_RMID, ignored_argument);

   #endif

   }


   virtual bool lock(const duration & durationTimeout = duration::infinite());

   using waitable::unlock;
   virtual bool unlock();

   virtual void * get_os_data() const;

   bool SetEvent();
   bool set_event() {return SetEvent(); }
   //bool PulseEvent();
   bool ResetEvent();

/**
* \brief	Platform independent waitables (windows version)
*/

	///  \brief		waits for an event forever
	virtual void wait ();

	///  \brief		waits for an event for a specified time
	///  \param		duration time period to wait for an event
	///  \return	waiting action result as WaitResult
   virtual wait_result wait (const duration & duration);

   bool is_signaled() const;

};

