#pragma once


namespace linux
{


   class CLASS_DECL_AXIS window_draw :
      virtual public ::user::window_draw,
      virtual public ::aura::message_queue_listener
   {
   public:


      enum EOptimize
      {
         OptimizeNone,
         OptimizeThis,
         OptimizeAllNext,
      };


      ::aura::message_queue_sp               m_spqueue;

      user::interaction_ptr_array            m_wndpaOut;
      mutex                                  m_mutexRendering;
      mutex                                  m_mutexRgnUpdate;
      semaphore                              m_semaphoreBuffer;
      bool                                   m_bRender;
      DWORD                                  m_dwLastRedrawRequest;
      mutex                                  m_mutexRender;
      critical_section                       m_csWndInterfaceMap;
      typed_pointer_map <
         map_ptr_to_ptr,
         void *,
         ::user::interaction_base *>
                                             m_wndinterfacemap;
      DWORD                                  m_dwLastUpdate;
      DWORD                                  m_dwLastDelay;


      window_draw(sp(::aura::application) papp);
      virtual ~window_draw();


      bool UpdateBuffer(LPCRECT lpcrect);
      //void OnPaint(void * hwnd, CPaintDC & spgraphics);

      semaphore * TwfGetBufferSemaphore();

      /*

      // Bit blitting spread functions
      virtual bool ScreenOutput(
         user::buffer *,
         ::draw2d::region & rgnUpdate);

      virtual bool ScreenOutput();

      virtual bool ScreenOutput(user::buffer *, sp(::user::interaction) pwnd);

      */

      // Transparent drawing operations
      // controllers
      virtual bool UpdateBuffer();

      virtual bool to(
         ::draw2d::graphics *       pdc,
         LPCRECT     lprectUpdate,
         user::oswindow_tree & hwndtree,
         bool        bGdiLocked,
         bool        bExcludeParamWnd);

      virtual bool to(
         ::draw2d::graphics *       pdc,
         LPCRECT     lprectUpdate,
         user::oswindow_tree::Array & hwndtreea,
         bool        bGdiLocked,
         bool        bExcludeParamWnd);

   /*     EOptimize TwfOptimizeRender2(
         user::oswindow_tree::Array & hwndtreea,
         LPCRECT lpcrect);

      EOptimize TwfOptimizeRender2(
         user::oswindow_tree & hwndtree,
         LPCRECT lpcrect);*/


      EOptimize TwfOptimizeRender(
         user::oswindow_tree::Array & hwndtreea,
         LPCRECT lpcrect);

      EOptimize TwfOptimizeRender(
         user::oswindow_tree & hwndtree,
         LPCRECT lpcrect);

      void TwfGetTopWindow(
         void * hwnd,
         user::oswindow_array & hwnda,
         array < HRGN, HRGN > & hrgna,
         user::oswindow_tree::Array & hwndtreea,
         LPCRECT lpcrect);

      bool TwfGetTopWindow(
         void * hwnd,
         user::oswindow_array & hwnda,
         array < HRGN, HRGN > & hrgna,
         user::oswindow_tree::Array & hwndtreea,
         HRGN hrgn);

      bool TwfGetTopWindow(
         void * hwnd,
         user::oswindow_array & hwnda,
         array < HRGN, HRGN > & hrgna,
         user::oswindow_tree & hwndtree,
         HRGN hrgn);

      void TwfGetTopWindowOptimizeOpaque(
         void * hwndOpaque,
         user::oswindow_array & hwnda,
         array < HRGN, HRGN > & hrgna);


      EOptimize TwfOptimizeRenderRemoveNextProper(
         user::oswindow_tree::Array & hwndtreea,
         int32_t iIndex,
         LPCRECT lpcrect);

      ::user::interaction_ptr_array get_wnda();
      //::user::oswindow_array get_hwnda();

      virtual int32_t run();

      virtual UINT RedrawProc();

      virtual void asynch_redraw();
      virtual void _asynch_redraw();
      virtual void synch_redraw();
      virtual void _synch_redraw();

      virtual void message_queue_message_handler(::signal_details * pobj);

   };

} // namespace linux
