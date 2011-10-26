#include "StdAfx.h"


const char * const szInvalidDateTime = "Invalid DateTime";
const char * const szInvalidDateTimeSpan = "Invalid DateTimeSpan";


date_span::date_span()
{
   m_iMonth = 0;
   m_iYear = 0;
   m_bConstraintMonth = 0;

}
/////////////////////////////////////////////////////////////////////////////
// time_span
/////////////////////////////////////////////////////////////////////////////

time_span::time_span() throw() :
   m_timeSpan(0)
{
}

time_span::time_span( __time64_t time ) throw() :
   m_timeSpan( time )
{
}

time_span::time_span(LONG lDays, int nHours, int nMins, int nSecs) throw()
{
    m_timeSpan = nSecs + 60* (nMins + 60* (nHours + __int64(24) * lDays));
}

LONGLONG time_span::GetDays() const throw()
{
   return( m_timeSpan/(24*3600) );
}

LONGLONG time_span::GetTotalHours() const throw()
{
   return( m_timeSpan/3600 );
}

LONG time_span::GetHours() const throw()
{
   return( LONG( GetTotalHours()-(GetDays()*24) ) );
}

LONGLONG time_span::GetTotalMinutes() const throw()
{
   return( m_timeSpan/60 );
}

LONG time_span::GetMinutes() const throw()
{
   return( LONG( GetTotalMinutes()-(GetTotalHours()*60) ) );
}

LONGLONG time_span::GetTotalSeconds() const throw()
{
   return( m_timeSpan );
}

LONG time_span::GetSeconds() const throw()
{
   return( LONG( GetTotalSeconds()-(GetTotalMinutes()*60) ) );
}

__time64_t time_span::GetTimeSpan() const throw()
{
   return( m_timeSpan );
}

time_span time_span::operator+( time_span span ) const throw()
{
   return( time_span( m_timeSpan+span.m_timeSpan ) );
}

time_span time_span::operator-( time_span span ) const throw()
{
   return( time_span( m_timeSpan-span.m_timeSpan ) );
}

time_span& time_span::operator+=( time_span span ) throw()
{
   m_timeSpan += span.m_timeSpan;
   return( *this );
}

time_span& time_span::operator-=( time_span span ) throw()
{
   m_timeSpan -= span.m_timeSpan;
   return( *this );
}

bool time_span::operator==( time_span span ) const throw()
{
   return( m_timeSpan == span.m_timeSpan );
}

bool time_span::operator!=( time_span span ) const throw()
{
   return( m_timeSpan != span.m_timeSpan );
}

bool time_span::operator<( time_span span ) const throw()
{
   return( m_timeSpan < span.m_timeSpan );
}

bool time_span::operator>( time_span span ) const throw()
{
   return( m_timeSpan > span.m_timeSpan );
}

bool time_span::operator<=( time_span span ) const throw()
{
   return( m_timeSpan <= span.m_timeSpan );
}

bool time_span::operator>=( time_span span ) const throw()
{
   return( m_timeSpan >= span.m_timeSpan );
}


string time_span::Format(const char * pFormat) const
// formatting timespans is a little trickier than formatting CTimes
//  * we are only interested in relative time formats, ie. it is illegal
//      to format anything dealing with absolute time (i.e. years, months,
//         day of week, day of year, timezones, ...)
//  * the only valid formats:
//      %D - # of days
//      %H - hour in 24 hour format
//      %M - minute (0-59)
//      %S - seconds (0-59)
//      %% - percent sign
//   %#<any_of_mods> - skip leading zeros
{
   ATLASSERT( pFormat != NULL );
   if( pFormat == NULL )
      AtlThrow( E_INVALIDARG );

   string strBuffer;
  // string hmsFormats [_CTIMESPANFORMATS] = {"%c","%02ld","%d"};
//   string dayFormats [_CTIMESPANFORMATS] = {"%c","%I64d","%I64d"};
   strBuffer.Preallocate(maxTimeBufferSize);
   char ch;

   while ((ch = *pFormat++) != '\0')
   {
      enum _CTIMESPANFORMATSTEP formatstep = _CTFS_NONE;
      if(ch == '%')
      {
         formatstep = _CTFS_FORMAT;
         ch = *pFormat++;
         if(ch == '#')
         {
               formatstep = _CTFS_NZ;
               ch = *pFormat++;
         }
      }
      switch (ch)
      {
         case '%':
            strBuffer += ch;
            break;
         case 'D':
//            strBuffer.AppendFormat(dayFormats[formatstep], formatstep ? GetDays()    : ch);
            break;
         case 'H':
//            strBuffer.AppendFormat(hmsFormats[formatstep], formatstep ? GetHours()   : ch);
            break;
         case 'M':
//            strBuffer.AppendFormat(hmsFormats[formatstep], formatstep ? GetMinutes() : ch);
            break;
         case 'S':
//            strBuffer.AppendFormat(hmsFormats[formatstep], formatstep ? GetSeconds() : ch);
            break;
         default:
            if(formatstep)
            {
#pragma warning (push)
#pragma warning (disable: 4127)  // conditional expression constant
               ATLENSURE(FALSE);      // probably a bad format character
#pragma warning (pop)
            }
            else
            {
               strBuffer += ch;
#ifdef _MBCS
               if (_istlead(ch))
               {
                  strBuffer += *pFormat++;
               }
#endif
            }
            break;
         }
   }

   return strBuffer;
}


/////////////////////////////////////////////////////////////////////////////
// time
/////////////////////////////////////////////////////////////////////////////
#pragma push_macro("get_current_time")
class time WINAPI time::get_current_time() throw()
{
   return( time( ::_time64( NULL ) ) );
}
#pragma pop_macro("get_current_time")

BOOL WINAPI time::is_valid_FILETIME(const FILETIME& fileTime) throw()
{
   FILETIME localTime;
   if (!FileTimeToLocalFileTime(&fileTime, &localTime))
   {
      return FALSE;
   }

   // then convert that time to system time
   SYSTEMTIME sysTime;
   if (!FileTimeToSystemTime(&localTime, &sysTime))
   {
      return FALSE;
   }

   return TRUE;
}

time::time() throw() :
   m_time(0)
{
}

time::time( __time64_t time )  throw():
   m_time( time )
{
}

time::time(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec,
   int nDST)
{
#pragma warning (push)
#pragma warning (disable: 4127)  // conditional expression constant


#pragma warning (pop)

   struct tm atm;

   atm.tm_sec = nSec;
   atm.tm_min = nMin;
   atm.tm_hour = nHour;
   atm.tm_mday = nDay;
   atm.tm_mon = nMonth - 1;        // tm_mon is 0 based
   atm.tm_year = nYear - 1900;     // tm_year is 1900 based
   atm.tm_isdst = nDST;

   m_time = _mktime64(&atm);
   /*
   Remember that:
   ATLENSURE( nYear >= 1900 );
   ATLENSURE( nMonth >= 1 && nMonth <= 12 );
   ATLENSURE( nDay >= 1 && nDay <= 31 );
   ATLENSURE( nHour >= 0 && nHour <= 23 );
   ATLENSURE( nMin >= 0 && nMin <= 59 );
   ATLENSURE( nSec >= 0 && nSec <= 59 );
   ATLASSUME(m_time != -1);   */    // indicates an illegal input time
   if(m_time == -1)
   {
      AtlThrow(E_INVALIDARG);
   }
}

time::time(WORD wDosDate, WORD wDosTime, int nDST)
{
   struct tm atm;
   atm.tm_sec = (wDosTime & ~0xFFE0) << 1;
   atm.tm_min = (wDosTime & ~0xF800) >> 5;
   atm.tm_hour = wDosTime >> 11;

   atm.tm_mday = wDosDate & ~0xFFE0;
   atm.tm_mon = ((wDosDate & ~0xFE00) >> 5) - 1;
   atm.tm_year = (wDosDate >> 9) + 80;
   atm.tm_isdst = nDST;
   m_time = _mktime64(&atm);
   ATLASSUME(m_time != -1);       // indicates an illegal input time

   if(m_time == -1)
      AtlThrow(E_INVALIDARG);

}

time::time(const SYSTEMTIME& sysTime, int nDST)
{
   if (sysTime.wYear < 1900)
   {
      __time64_t time0 = 0L;
      time timeT(time0);
      *this = timeT;
   }
   else
   {
      time timeT(
         (int)sysTime.wYear, (int)sysTime.wMonth, (int)sysTime.wDay,
         (int)sysTime.wHour, (int)sysTime.wMinute, (int)sysTime.wSecond,
         nDST);
      *this = timeT;
   }
}

time::time(const FILETIME& fileTime, int nDST)
{
   // first convert file time (UTC time) to local time
   FILETIME localTime;
   if (!FileTimeToLocalFileTime(&fileTime, &localTime))
   {
      m_time = 0;
      AtlThrow(E_INVALIDARG);
      return;
   }

   // then convert that time to system time
   SYSTEMTIME sysTime;
   if (!FileTimeToSystemTime(&localTime, &sysTime))
   {
      m_time = 0;
      AtlThrow(E_INVALIDARG);
      return;
   }

   // then convert the system time to a time_t (C-runtime local time)
   time timeT(sysTime, nDST);
   *this = timeT;
}

class time & time::operator=( __time64_t time ) throw()
{
   m_time = time;

   return( *this );
}

class time & time::operator+=( time_span span ) throw()
{
   m_time += span.GetTimeSpan();

   return( *this );
}

class time & time::operator-=( time_span span ) throw()
{
   m_time -= span.GetTimeSpan();

   return( *this );
}

class time & time::operator+=( date_span span )
{
   UNREFERENCED_PARAMETER(span);
   throw not_implemented_exception();

   return( *this );
}

class time& time::operator-=( date_span span )
{
   UNREFERENCED_PARAMETER(span);
   throw not_implemented_exception();

   return( *this );
}

class time_span time::operator-( time time ) const throw()
{
   return( time_span( m_time-time.m_time ) );
}

class time time::operator-( time_span span ) const throw()
{
   return( time( m_time-span.GetTimeSpan() ) );
}

class time time::operator+( time_span span ) const throw()
{
   return( time( m_time+span.GetTimeSpan() ) );
}

class time time::operator-( date_span span ) const
{
   UNREFERENCED_PARAMETER(span);
   throw not_implemented_exception();
}

class time time::operator+( date_span span ) const
{
   UNREFERENCED_PARAMETER(span);
   throw not_implemented_exception();
}

bool time::operator==( time time ) const throw()
{
   return( m_time == time.m_time );
}

bool time::operator!=( time time ) const throw()
{
   return( m_time != time.m_time );
}

bool time::operator<( time time ) const throw()
{
   return( m_time < time.m_time );
}

bool time::operator>( time time ) const throw()
{
   return( m_time > time.m_time );
}

bool time::operator<=( time time ) const throw()
{
   return( m_time <= time.m_time );
}

bool time::operator>=( time time ) const throw()
{
   return( m_time >= time.m_time );
}

#if !_SECURE_ATL

   _ATL_INSECURE_DEPRECATE("Pass an output time structure to time::GetGmtTm")
struct tm* time::GetGmtTm() const throw()
{
#pragma warning(push)
#pragma warning(disable : 4996)
   return _gmtime64(&m_time);
#pragma warning(pop)
}

   _ATL_INSECURE_DEPRECATE("Pass an output time structure to time::GetLocalTm")
struct tm* time::GetLocalTm() const throw()
{
#pragma warning(push)
#pragma warning(disable : 4996)
   return _localtime64(&m_time);
#pragma warning(pop)
}

#endif // !_SECURE_ATL

struct tm* time::GetGmtTm(struct tm* ptm) const
{
#if _SECURE_ATL
   // Ensure ptm is valid
   ATLENSURE( ptm != NULL );

   if (ptm != NULL)
   {
      struct tm ptmTemp;
      errno_t err = _gmtime64_s(&ptmTemp, &m_time);

      // Be sure the call succeeded
      if(err != 0) { return NULL; }

      *ptm = ptmTemp;
      return ptm;
   }

   return NULL;
#else
   if (ptm != NULL)
   {
      struct tm * ptmTemp;
      ptmTemp = _gmtime64(&m_time);

      if(ptmTemp == NULL)
         return NULL;

      *ptm = *ptmTemp;

      return ptm;
   }
   else
      return _gmtime64(&m_time);
#endif
}

struct tm* time::GetLocalTm(struct tm* ptm) const
{
#if _SECURE_ATL
   // Ensure ptm is valid
   ATLENSURE( ptm != NULL );

   if (ptm != NULL)
   {
      struct tm ptmTemp;
      errno_t err = _localtime64_s(&ptmTemp, &m_time);

      if (err != 0)
      {
         return NULL;    // indicates that m_time was not initialized!
      }

      *ptm = ptmTemp;
      return ptm;
   }

   return NULL;
#else
   if (ptm != NULL)
   {
      struct tm* ptmTemp = _localtime64(&m_time);
      if (ptmTemp == NULL)
         return NULL;    // indicates the m_time was not initialized!

      *ptm = *ptmTemp;
      return ptm;
   }
   else
      return _localtime64(&m_time);
#endif
}

bool time::get_as_system_time(SYSTEMTIME& timeDest) const throw()
{
    struct tm ttm;
   struct tm* ptm;

    ptm = GetLocalTm(&ttm);

   if(!ptm) { return false; }

   timeDest.wYear = (WORD) (1900 + ptm->tm_year);
   timeDest.wMonth = (WORD) (1 + ptm->tm_mon);
   timeDest.wDayOfWeek = (WORD) ptm->tm_wday;
   timeDest.wDay = (WORD) ptm->tm_mday;
   timeDest.wHour = (WORD) ptm->tm_hour;
   timeDest.wMinute = (WORD) ptm->tm_min;
   timeDest.wSecond = (WORD) ptm->tm_sec;
   timeDest.wMilliseconds = 0;

   return true;
}

__time64_t time::get_time() const throw()
{
   return( m_time );
}

int time::GetYear() const
{
   struct tm ttm;
   struct tm * ptm;

   ptm = GetLocalTm(&ttm);
   return ptm ? (ptm->tm_year) + 1900 : 0 ;
}

int time::GetMonth() const
{
   struct tm ttm;
   struct tm * ptm;

   ptm = GetLocalTm(&ttm);
   return ptm ? ptm->tm_mon + 1 : 0;
}

int time::GetDay() const
{
   struct tm ttm;
   struct tm * ptm;

   ptm = GetLocalTm(&ttm);
   return ptm ? ptm->tm_mday : 0 ;
}

int time::GetHour() const
{
   struct tm ttm;
   struct tm * ptm;

   ptm = GetLocalTm(&ttm);
   return ptm ? ptm->tm_hour : -1 ;
}

int time::GetMinute() const
{
   struct tm ttm;
   struct tm * ptm;

   ptm = GetLocalTm(&ttm);
   return ptm ? ptm->tm_min : -1 ;
}

int time::GetSecond() const
{
   struct tm ttm;
   struct tm * ptm;

   ptm = GetLocalTm(&ttm);
   return ptm ? ptm->tm_sec : -1 ;
}

int time::GetDayOfWeek() const
{
   struct tm ttm;
   struct tm * ptm;

   ptm = GetLocalTm(&ttm);
   return ptm ? ptm->tm_wday + 1 : 0 ;
}

int time::GetGmtYear() const
{
   struct tm ttm;
   struct tm * ptm;

   ptm = GetGmtTm(&ttm);
   return ptm ? (ptm->tm_year) + 1900 : 0 ;
}

int time::GetGmtMonth() const
{
   struct tm ttm;
   struct tm * ptm;

   ptm = GetGmtTm(&ttm);
   return ptm ? ptm->tm_mon + 1 : 0;
}

int time::GetGmtDay() const
{
   struct tm ttm;
   struct tm * ptm;

   ptm = GetGmtTm(&ttm);
   return ptm ? ptm->tm_mday : 0 ;
}

int time::GetGmtHour() const
{
   struct tm ttm;
   struct tm * ptm;

   ptm = GetGmtTm(&ttm);
   return ptm ? ptm->tm_hour : -1 ;
}

int time::GetGmtMinute() const
{
   struct tm ttm;
   struct tm * ptm;

   ptm = GetGmtTm(&ttm);
   return ptm ? ptm->tm_min : -1 ;
}

int time::GetGmtSecond() const
{
   struct tm ttm;
   struct tm * ptm;

   ptm = GetGmtTm(&ttm);
   return ptm ? ptm->tm_sec : -1 ;
}

int time::GetGmtDayOfWeek() const
{
   struct tm ttm;
   struct tm * ptm;

   ptm = GetGmtTm(&ttm);
   return ptm ? ptm->tm_wday + 1 : 0 ;
}

/////////////////////////////////////////////////////////////////////////////
// CFileTimeSpan
/////////////////////////////////////////////////////////////////////////////

CFileTimeSpan::CFileTimeSpan() throw() :
   m_nSpan( 0 )
{
}

CFileTimeSpan::CFileTimeSpan( const CFileTimeSpan& span ) throw() :
   m_nSpan( span.m_nSpan )
{
}

CFileTimeSpan::CFileTimeSpan( LONGLONG nSpan ) throw() :
   m_nSpan( nSpan )
{
}

CFileTimeSpan& CFileTimeSpan::operator=( const CFileTimeSpan& span ) throw()
{
   m_nSpan = span.m_nSpan;

   return( *this );
}

CFileTimeSpan& CFileTimeSpan::operator+=( CFileTimeSpan span ) throw()
{
   m_nSpan += span.m_nSpan;

   return( *this );
}

CFileTimeSpan& CFileTimeSpan::operator-=( CFileTimeSpan span ) throw()
{
   m_nSpan -= span.m_nSpan;

   return( *this );
}

CFileTimeSpan CFileTimeSpan::operator+( CFileTimeSpan span ) const throw()
{
   return( CFileTimeSpan( m_nSpan+span.m_nSpan ) );
}

CFileTimeSpan CFileTimeSpan::operator-( CFileTimeSpan span ) const throw()
{
   return( CFileTimeSpan( m_nSpan-span.m_nSpan ) );
}

bool CFileTimeSpan::operator==( CFileTimeSpan span ) const throw()
{
   return( m_nSpan == span.m_nSpan );
}

bool CFileTimeSpan::operator!=( CFileTimeSpan span ) const throw()
{
   return( m_nSpan != span.m_nSpan );
}

bool CFileTimeSpan::operator<( CFileTimeSpan span ) const throw()
{
   return( m_nSpan < span.m_nSpan );
}

bool CFileTimeSpan::operator>( CFileTimeSpan span ) const throw()
{
   return( m_nSpan > span.m_nSpan );
}

bool CFileTimeSpan::operator<=( CFileTimeSpan span ) const throw()
{
   return( m_nSpan <= span.m_nSpan );
}

bool CFileTimeSpan::operator>=( CFileTimeSpan span ) const throw()
{
   return( m_nSpan >= span.m_nSpan );
}

LONGLONG CFileTimeSpan::GetTimeSpan() const throw()
{
   return( m_nSpan );
}

void CFileTimeSpan::SetTimeSpan( LONGLONG nSpan ) throw()
{
   m_nSpan = nSpan;
}


/////////////////////////////////////////////////////////////////////////////
// CFileTime
/////////////////////////////////////////////////////////////////////////////

CFileTime::CFileTime() throw()
{
   dwLowDateTime = 0;
   dwHighDateTime = 0;
}

CFileTime::CFileTime( const FILETIME& ft ) throw()
{
   dwLowDateTime = ft.dwLowDateTime;
   dwHighDateTime = ft.dwHighDateTime;
}

CFileTime::CFileTime( ULONGLONG nTime ) throw()
{
   dwLowDateTime = DWORD( nTime );
   dwHighDateTime = DWORD( nTime>>32 );
}

CFileTime& CFileTime::operator=( const FILETIME& ft ) throw()
{
   dwLowDateTime = ft.dwLowDateTime;
   dwHighDateTime = ft.dwHighDateTime;

   return( *this );
}

CFileTime WINAPI CFileTime::get_current_time() throw()
{
   CFileTime ft;
   GetSystemTimeAsFileTime(&ft);
   return ft;
}

CFileTime& CFileTime::operator+=( CFileTimeSpan span ) throw()
{
   SetTime( get_time()+span.GetTimeSpan() );

   return( *this );
}

CFileTime& CFileTime::operator-=( CFileTimeSpan span ) throw()
{
   SetTime( get_time()-span.GetTimeSpan() );

   return( *this );
}

CFileTime CFileTime::operator+( CFileTimeSpan span ) const throw()
{
   return( CFileTime( get_time()+span.GetTimeSpan() ) );
}

CFileTime CFileTime::operator-( CFileTimeSpan span ) const throw()
{
   return( CFileTime( get_time()-span.GetTimeSpan() ) );
}

CFileTimeSpan CFileTime::operator-( CFileTime ft ) const throw()
{
   return( CFileTimeSpan( get_time()-ft.get_time() ) );
}

bool CFileTime::operator==( CFileTime ft ) const throw()
{
   return( get_time() == ft.get_time() );
}

bool CFileTime::operator!=( CFileTime ft ) const throw()
{
   return( get_time() != ft.get_time() );
}

bool CFileTime::operator<( CFileTime ft ) const throw()
{
   return( get_time() < ft.get_time() );
}

bool CFileTime::operator>( CFileTime ft ) const throw()
{
   return( get_time() > ft.get_time() );
}

bool CFileTime::operator<=( CFileTime ft ) const throw()
{
   return( get_time() <= ft.get_time() );
}

bool CFileTime::operator>=( CFileTime ft ) const throw()
{
   return( get_time() >= ft.get_time() );
}

ULONGLONG CFileTime::get_time() const throw()
{
   return( (ULONGLONG( dwHighDateTime )<<32)|dwLowDateTime );
}

void CFileTime::SetTime( ULONGLONG nTime ) throw()
{
   dwLowDateTime = DWORD( nTime );
   dwHighDateTime = DWORD( nTime>>32 );
}

CFileTime CFileTime::UTCToLocal() const throw()
{
   CFileTime ftLocal;

   ::FileTimeToLocalFileTime( this, &ftLocal );

   return( ftLocal );
}

CFileTime CFileTime::LocalToUTC() const throw()
{
   CFileTime ftUTC;

   ::LocalFileTimeToFileTime( this, &ftUTC );

   return( ftUTC );
}



const ULONGLONG CFileTime::Millisecond = 10000;
const ULONGLONG CFileTime::Second = Millisecond * static_cast<ULONGLONG>(1000);
const ULONGLONG CFileTime::Minute = Second * static_cast<ULONGLONG>(60);
const ULONGLONG CFileTime::Hour = Minute * static_cast<ULONGLONG>(60);
const ULONGLONG CFileTime::Day = Hour * static_cast<ULONGLONG>(24);
const ULONGLONG CFileTime::Week = Day * static_cast<ULONGLONG>(7);
