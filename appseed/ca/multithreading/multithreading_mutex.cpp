#include "framework.h"


#if defined(LINUX)
#include <sys/ipc.h>
#include <sys/sem.h>
#endif


mutex::mutex(::ca::application * papp, bool bInitiallyOwn, const char * pstrName, LPSECURITY_ATTRIBUTES lpsaAttribute /* = ::null() */) :
   sync_object(pstrName)
{

#ifdef _WIN32

   m_object = ::CreateMutexExW(lpsaAttribute, pstrName == ::null() ? ::null() : (const wchar_t *) ::ca::international::utf8_to_unicode(pstrName), bInitiallyOwn ?  CREATE_MUTEX_INITIAL_OWNER : 0, DELETE | SYNCHRONIZE);

   if(m_object == ::null())
   {

      if(pstrName == ::null())
      {

         throw resource_exception(papp);

      }
      else
      {

         m_object = ::OpenMutexW(SYNCHRONIZE, TRUE, ::ca::international::utf8_to_unicode(pstrName));

         if(m_object == ::null())
         {

            throw resource_exception(papp);

         }

      }

   }

#else

   if(pstrName != ::null() && *pstrName != '\0')
   {

      m_strName = pstrName;

      m_key = ftok(m_strName, 0); //Generate a unique key or supply a value

      m_semid = semget(
                  m_key, // a unique identifier to identify semaphore set
                  1,  // number of semaphore in the semaphore set
                  0666 | IPC_CREAT // permissions (rwxrwxrwx) on the new
                       //semaphore set and creation flag
                  );

      if(m_semid < 0)
         throw resource_exception(get_app());

      semun semctl_arg;

      //set Initial value for the resource
      semctl_arg.val = 1; //Setting semval to 1

      semctl(m_semid, 0, SETVAL, semctl_arg);

   }
   else
   {


      m_key = 0;

      m_semid = -1;

      pthread_mutexattr_t attr;

      pthread_mutexattr_init(&attr);

      pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

      pthread_mutex_init(&m_mutex, &attr);

   }


#endif

}


#ifdef WINDOWS

mutex::mutex(::ca::application * papp, const char * pstrName, HANDLE h) :
   ca(papp),
   sync_object(pstrName)
{

   m_object = h;

}

#else

mutex::mutex(const char * pstrName, key_t key, int32_t semid) :
   sync_object(pstrName)
{

   m_strName      = pstrName;
   m_key          = key;
   m_semid        = semid;

}

#endif




mutex::~mutex()
{

#ifndef _WIN32

   if(m_semid >= 0)
   {

      semun ignored_argument;

      semctl(m_semid, 1, IPC_RMID , ignored_argument);

   }
   else
   {

      pthread_mutex_destroy(&m_mutex);

   }

#endif

}

#ifndef WINDOWS

wait_result mutex::wait(const duration & duration)
{

   uint32_t dwTimeout = duration.os_lock_duration();

   timespec delay;

   delay.tv_sec = 0;
   delay.tv_nsec = 1000000;  // 1 milli sec delay

   int32_t irc;

   bool bFirst = true;

   uint32_t start = ::get_tick_count();

   if(m_semid >= 0)
   {
      //Wait for Zero

      struct sembuf operation[1] ;


      while((dwTimeout == (uint32_t) INFINITE && ::get_tick_count() - start < dwTimeout) || bFirst)
      {

         bFirst = false;

         //Call Wait for Zero with IPC_NOWAIT option,so it will be
         // non blocking

         operation[0].sem_op = -1; // Wait
         operation[0].sem_num = 0;
         operation[0].sem_flg = IPC_NOWAIT;

         int32_t ret = semop(m_semid, operation,1);

         if(ret < 0)
         {
            /* check whether somebody else has the mutex */
            if (ret == EPERM )
            {
               /* sleep for delay time */
               nanosleep(&delay, ::null());
            }
            else
            {
               return wait_result(wait_result::Failure);
            }
         }
         else
         {
            return wait_result(wait_result::Event0);
         }
      }

   }
   else
   {

      while((dwTimeout == (uint32_t) INFINITE && ::get_tick_count() - start < dwTimeout) || bFirst)
      {

         bFirst = false;

         // Tries to acquire the mutex and access the shared resource,
         // if success, access the shared resource,
         // if the shared reosurce already in use, it tries every 1 milli sec
         // to acquire the resource
         // if it does not acquire the mutex within 2 secs delay,
         // then it is considered to be failed

          irc = pthread_mutex_trylock(&m_mutex);
          if (!irc)
          {
               return wait_result(wait_result::Event0);
          }
          else
          {
            // check whether somebody else has the mutex
            if (irc == EPERM )
            {
               // Yes, Resource already in use so sleep
               nanosleep(&delay, ::null());
            }
            else
            {
               return wait_result(wait_result::Failure);
            }
          }
       }
   }

   return wait_result(wait_result::Timeout);

}


bool mutex::lock(const duration & duration)
{

    wait_result result = wait(duration);

    if(!result.signaled())
        return false;

    return true;

}

#endif











bool mutex::unlock()
{

#ifdef _WIN32

   return ::ReleaseMutex(m_object) != FALSE;

#else

   if(m_semid >= 0)
   {

      struct sembuf operation[1] ;

      operation[0].sem_op = 1; //Release the resource so Wait in process
                               // 1 will be triggered
      operation[0].sem_num = 0;
      operation[0].sem_flg = SEM_UNDO;

      //Release semaphore
      semop(m_semid, operation, 1);

      return !is_locked();

   }
   else
   {

      return pthread_mutex_unlock(&m_mutex) != 0;

   }

#endif

}



mutex * mutex::open_mutex(::ca::application * papp,  const char * pstrName)
{

#ifdef WINDOWS

   HANDLE h = ::OpenMutexW(SYNCHRONIZE, FALSE, ::ca::international::utf8_to_unicode(pstrName));

   if(h == ::null() || h == INVALID_HANDLE_VALUE)
      return ::null();

   mutex * pmutex = new mutex(papp, pstrName, h);

   return pmutex;

#else

   if(pstrName == ::null() || *pstrName == '\0')
      return ::null();

   string strName = pstrName;

   key_t key = ftok(strName, 0); //Generate a unique key or supply a value

   int32_t semid = semget(
               key, // a unique identifier to identify semaphore set
               1,  // number of semaphore in the semaphore set
               0666 // permissions (rwxrwxrwx) on the new
                    //semaphore set and creation flag
                  );
   if(semid < 0)
   {
      return ::null();
   }

   mutex * pmutex = new mutex(strName, key, semid);

   return pmutex;

#endif

}
