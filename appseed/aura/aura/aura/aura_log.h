#pragma once


namespace aura
{


   class CLASS_DECL_AURA log :
      virtual public element
   {
   public:

      enum e_level
      {

         level_warning = 0,
         level_error,
         level_fatal,
         level_info

      };

      bool              m_bLog;
      bool              m_bExtendedLog;



      bool                    m_bTrace;
      ::aura::trace::trace *     m_ptrace;
      mutex *      m_pcsTrace;
      stringa *               m_pstraSeparator;
      FILE *                  m_pfile;
      mutex *                 m_pmutex;
      bool                    m_bInitialized;
      string *                m_pstrLogPath;
      id *                    m_pid;
      int32_t                     m_iYear;
      int32_t                     m_iMonth;
      int32_t                     m_iDay;


      log(::aura::application * papp);
      virtual ~log();

      virtual void print(const char * psz, ...);

      virtual bool initialize(const char * pszId);
      virtual bool initialize(id id);
      virtual bool finalize();

      void trace_str(const char *pszFileName, int32_t nLine, uint32_t dwCategory, uint32_t nLevel, const char * psz) const;
      void trace_v(const char *pszFileName, int32_t nLine, uint32_t dwCategory, uint32_t nLevel, const char * pszFmt, va_list args) const;

      void set_trace_category(uint32_t dwCategory, const char * pszName, uint32_t uiLevel);


      /** error level enum. */
      //      log();
      //    virtual ~log();

      virtual void success(const char * psz);

      //      virtual void print(const char * psz, ...);

      //      virtual bool initialize(const char * pszId);
      //      virtual bool finalize();

      virtual void set_extended_log(bool bSet = true);
      virtual bool get_extended_log();


      //virtual void trace_v(const char *pszFileName, int32_t nLine, uint32_t dwCategory, uint32_t nLevel, const char * pszFmt, va_list args) const;
      //      virtual void trace_v(const char *pszFileName, int32_t nLine, uint32_t dwCategory, uint32_t nLevel, const wchar_t * pszFmt, va_list args) const;

      void __cdecl trace(const char * pszFormat, ...);
      void __cdecl trace2(uint32_t dwCategory, UINT nLevel, const char * pszFormat, ...);


      //virtual void set_trace_category(uint32_t dwCategory, const char * pszName, uint32_t uiLevel);



   };


   CLASS_DECL_AURA int32_t SimpleDebugReport(int32_t,const char *,int32_t,const char *,const char * pszFormat,va_list list);


} // namespace aura


inline ::aura::log::e_level ca_get_level_warning() { return ::aura::log::level_warning; }



