#include "framework.h" // from "axis/user/user.h"


namespace user
{


   window_draw::window_draw(sp(::aura::application) papp) :
      element(papp),
      thread(papp),
      m_eventFree(papp, TRUE, TRUE)
   {
      m_bProDevianMode                    = true;
      m_iFramesPerSecond                  = 24.0;
      m_bRunning                          = false;
      m_bRun                              = true;
   }


   bool window_draw::twf_start()
   {

      int iReturnCode;

      if(begin_synch(&iReturnCode))
         return false;

      return true;

   }


   bool window_draw::UpdateBuffer()
   {
   
      if(m_bRender)
         return false;
      
      keep<bool> keepRender(&m_bRender, true, false, true);
      
      ::user::interaction_ptr_array wndpa(get_app());
      
      wndpa = get_wnda();
      
      rect rectWindow;
      
      rect rect9;
      
      ::user::oswindow_array wndaApp;
      
      for(int32_t l = 0; l < wndpa.get_count();)
      {
      
         try
         {
            
            if(wndpa[l].oprop("session").is_new())
            {
            
               if(wndpa[l].m_bMayProDevian)
               {
               
                  wndpa[l]._001UpdateScreen();
                  
               }
               
            }
            
            l++;
            
         }
         catch(simple_exception & se)
         {
            if(se.m_strMessage == "no more a window")
            {
               System.frames().remove(&wndpa[l]);
               wndpa.remove_at(l);
               
            }
         }
         catch(...)
         {
            System.frames().remove(&wndpa[l]);
            wndpa.remove_at(l);
         }
      }
      
      return true;
      
   }
   

   bool window_draw::ScreenOutput()
   {
      throw interface_only_exception(get_app());
   }

   bool window_draw::twf_stop()
   {

      m_bRun = false;

      return true;

   }

   void window_draw::asynch_redraw()
   {
      throw interface_only_exception(get_app());
   }

   void window_draw::synch_redraw()
   {
      throw interface_only_exception(get_app());
   }


   ptr_array < ::user::interaction > window_draw::get_wnda()
   {

      return System.frames();

   }


} // namespace user



