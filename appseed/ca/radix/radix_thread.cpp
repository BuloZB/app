#include "framework.h"

namespace radix
{

   bool thread::s_bAllocReady = false;

   CLASS_DECL_ca void thread_alloc_ready(bool bReady)
   {
      ::radix::thread::s_bAllocReady = bReady;
   }

   thread::thread() :
      m_mutex(NULL)
   {
      m_pthread = this;
   }

   thread::thread(::ca::application * papp) :
      ca(papp),
      m_mutex(papp)
   {
      m_pthread = this;
      if(papp == NULL)
         return;
      set_app(papp);
      if(!s_bAllocReady)
         return;
      ::ca::thread_sp::create(papp);
      m_p->set_p(this);
      m_p->construct();
   }

   thread::thread(::ca::application * papp, __THREADPROC pfnThreadProc, LPVOID pParam) :
      ca(papp),
      m_mutex(papp)
   {
      m_pthread = this;
      ::ca::thread_sp::create(papp);
      m_p->set_p(this);
      m_p->construct(pfnThreadProc, pParam);
   }

   thread::~thread()
   {
   }

   void * thread::get_os_data() const
   {
      return m_p->get_os_data();
   }

   int_ptr thread::get_os_int() const
   {
      return m_p->get_os_int();
   }

   bool thread::Begin(::ca::e_thread_priority epriority, UINT nStackSize, uint32_t dwCreateFlags, LPSECURITY_ATTRIBUTES lpSecurityAttrs)
   {
      return m_p->Begin(epriority, nStackSize, dwCreateFlags, lpSecurityAttrs);
   }

   bool thread::create_thread(::ca::e_thread_priority epriority, uint32_t dwCreateFlags, UINT nStackSize, LPSECURITY_ATTRIBUTES lpSecurityAttrs)
   {
      return m_p->create_thread(epriority, dwCreateFlags, nStackSize, lpSecurityAttrs);
   }

   ::ca::e_thread_priority thread::get_thread_priority()
   {
      return m_p->get_thread_priority();
   }

   bool thread::set_thread_priority(::ca::e_thread_priority epriority)
   {
      return m_p->set_thread_priority(epriority);
   }

/*   uint32_t thread::SuspendThread()
   {
      return m_p->SuspendThread();
   }
   */

   uint32_t thread::ResumeThread()
   {
      return m_p->ResumeThread();
   }
   

   bool thread::post_thread_message(UINT message, WPARAM wParam, LPARAM lParam)
   {
      return m_p->post_thread_message(message, wParam, lParam);
   }

   void thread::ProcessMessageFilter(int32_t code, gen::signal_object * pobj)
   {
      return  m_p->ProcessMessageFilter(code, pobj);
   }

   bool thread::post_message(::user::interaction * pguie, UINT message, WPARAM wParam, LPARAM lParam)
   {
      if(m_p != NULL)
      {
         return m_p->post_message(pguie, message, wParam, lParam);
      }
      else
      {
         return false;
      }
   }

   bool thread::PreInitInstance()
   {
      return m_p->PreInitInstance();
   }

   // called when occurs an standard_exception exception in run
   // return true to call run again
   bool thread::on_run_exception(::ca::exception & e)
   {
      return m_p->on_run_exception(e);
   }

   // Overridables
   // thread initialization
   bool thread::initialize_instance()
   {
      return m_p->initialize_instance();
   }

   ::gen::message::e_prototype thread::GetMessagePrototype(UINT uiMessage, UINT uiCode)
   {
      return m_p->GetMessagePrototype(uiMessage, uiCode);
   }

   // running and idle processing
   int32_t thread::run()
   {
      return m_p->run();
   }

   void thread::pre_translate_message(gen::signal_object * pobj)
   {
      if(m_p == NULL)
         return;
      return m_p->pre_translate_message(pobj);
   }

   bool thread::pump_message()     // low level message pump
   {
      return m_p->pump_message();
   }

   bool thread::on_idle(LONG lCount) // return TRUE if more idle processing
   {
      return m_p->on_idle(lCount);
   }

   bool thread::is_idle_message(gen::signal_object * pobj)  // checks for special messages
   {
      return m_p->is_idle_message(pobj);
   }

   // thread termination
   int32_t thread::exit_instance() // default will 'delete this'
   {


      return m_p->exit_instance();
   }

   // Advanced: exception handling
   void thread::ProcessWndProcException(base_exception* e, gen::signal_object * pobj)
   {
      return m_p->ProcessWndProcException(e, pobj);
   }


   // Advanced: access to GetMainWnd()
   ::user::interaction* thread::GetMainWnd()
   {
      return m_p->GetMainWnd();
   }

   ::user::interaction * thread::SetMainWnd(::user::interaction * pui)
   {
      return m_p->SetMainWnd(pui);
   }

   void thread::add(::user::interaction * pui)
   {
      m_p->add(pui);
   }

   void thread::remove(::user::interaction * pui)
   {

      try
      {

         if(m_p->m_ptimera != NULL)
         {

            m_p->m_ptimera->on_delete(pui);

         }

      }
      catch(...)
      {
      }
      try
      {
         if(m_p != NULL)
         {
            m_p->remove(pui, false);
         }
      }
      catch(...)
      {
      }
      if(pui == GetMainWnd()
      || pui->m_pguie == GetMainWnd()
      || pui->m_pimpl == GetMainWnd())
      {
         SetMainWnd(NULL);
      }
/*      try
      {
         if(pui->m_pthread == this)
         {
            pui->m_pthread = NULL;
         }
      }
      catch(...)
      {
      }
      try
      {
         if(pui->m_pimpl != NULL && pui->m_pimpl != pui)
         {
            if(pui->m_pimpl->m_pthread == this)
            {
               pui->m_pimpl->m_pthread = NULL;
            }
         }
      }
      catch(...)
      {
      }
      try
      {
         if(pui->m_pguie != NULL && pui->m_pguie != pui)
         {
            if(pui->m_pguie->m_pthread == this)
            {
               pui->m_pguie->m_pthread = NULL;
            }
         }
      }
      catch(...)
      {
      }*/
   }

   ::count thread::get_ui_count()
   {
      return m_p->get_ui_count();
   }

   ::user::interaction * thread::get_ui(index iIndex)
   {
      return m_p->get_ui(iIndex);
   }


   void thread::set_timer(::user::interaction * pui, uint_ptr nIDEvent, UINT nEllapse)
   {
      m_p->set_timer(pui, nIDEvent, nEllapse);
   }

   void thread::unset_timer(::user::interaction * pui, uint_ptr nIDEvent)
   {
      m_p->unset_timer(pui, nIDEvent);
   }

   void thread::set_auto_delete(bool bAutoDelete)
   {
      m_p->set_auto_delete(bAutoDelete);
   }

   void thread::set_run(bool bRun)
   {
      m_p->set_run(bRun);
   }

   event & thread::get_finish_event()
   {
      return m_p->get_finish_event();
   }

   bool thread::get_run()
   {
      return m_p->get_run();
   }

   ::ca::thread * thread::get_app_thread()
   {
      return m_p->get_app_thread();
   }

   ::user::interaction * thread::get_active_ui()
   {
      return m_p->get_active_ui();
   }

   ::user::interaction * thread::set_active_ui(::user::interaction * pui)
   {
      return m_p->set_active_ui(pui);
   }

   void thread::step_timer()
   {
/*      try
      {
         ::radix::thread * pthreadApp = dynamic_cast < ::radix::thread * > (System.GetThread());
         if(pthreadApp != NULL && m_p != pthreadApp)
         {
            pthreadApp->step_timer();
         }
      }
      catch(...)
      {
      }*/
      return m_p->step_timer();
   }

   int32_t thread::main()
   {
      return m_p->main();
   }



   void thread::assert_valid() const
   {
      m_p->assert_valid();
   }
   void thread::dump(dump_context & dumpcontext) const
   {
      m_p->dump(dumpcontext);
   }


   // 'delete this' only if m_bAutoDelete == TRUE
   void thread::Delete()
   {
      m_p->Delete();
   }

   void thread::DispatchThreadMessageEx(gen::signal_object * pobj)  // helper
   {
      return m_p->DispatchThreadMessageEx(pobj);
   }

	void thread::wait()
   {

      return m_p->wait();
      // on Windows ==>       ::WaitForSingleObject(m_loginthread.get_os_data(), INFINITE);

   }

#ifdef WINDOWS
	HANDLE thread::item() const
   {

      return m_p->item();
   }
#else
	int_ptr thread::item() const
   {
      return m_p->item();

   }
#endif



} // namespace radix




::radix::thread* __begin_thread(::ca::application * papp, __THREADPROC pfnThreadProc, LPVOID pParam, ::ca::e_thread_priority epriority, UINT nStackSize, uint32_t dwCreateFlags, LPSECURITY_ATTRIBUTES lpSecurityAttrs)
{

   ASSERT(pfnThreadProc != NULL);

   ::radix::thread* pThread = new ::radix::thread(papp, pfnThreadProc, pParam);
   ASSERT_VALID(pThread);

   if (!pThread->create_thread(epriority, dwCreateFlags, nStackSize, lpSecurityAttrs))
   {
      pThread->Delete();
      return NULL;
   }

   return pThread;

}



