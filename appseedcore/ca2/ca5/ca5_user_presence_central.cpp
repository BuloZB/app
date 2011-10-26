#include "StdAfx.h"

namespace ca5
{

   namespace user
   {

      presence_central::presence_central(::ca::application * papp) :
         ca(papp),
         message_window_simple_callback(papp)
      {
      }
      
      presence_central::~presence_central()
      {
      }

      bool presence_central::initialize()
      {
         
         if(!initialize_message_window("ca5::user::presence_central::message_window"))
            return false;

         m_spwindowMessage->SetTimer(1984, 1000 * 23, NULL);

         if(ApplicationUser.m_ppresence == NULL)
         {
            presence * ppresence = new presence(get_app());
            ppresence->report_activity();
            ppresence->pulse_user_presence();
            ApplicationUser.m_ppresence = ppresence;
         }

         return true;

      }

      bool presence_central::finalize()
      {

         if(m_spwindowMessage.is_null())
            return true;

         m_spwindowMessage->KillTimer(1984);
         
         finalize_message_window();

         if(ApplicationUser.m_ppresence != NULL)
         {
            delete ApplicationUser.m_ppresence;
            ApplicationUser.m_ppresence = NULL;
         }

         return true;

      }

      bool presence_central::is_initialized()
      {
         
         if(!m_spwindowMessage->IsWindow())
            return false;

         return true;

      }

      void presence_central::message_window_message_handler(gen::signal_object * pobj)
      {
         SCAST_PTR(::user::win::message::base, pbase, pobj);
         if(pbase->m_uiMessage == WM_TIMER)
         {
            SCAST_PTR(::user::win::message::timer, ptimer, pobj);
            if(&ApplicationUser != NULL)
            {
               presence * ppresence = ApplicationUser.m_ppresence;
               if(ptimer->m_nIDEvent == 1984 && ppresence != NULL)
               {
                  ppresence->defer_pulse_user_presence();
               }
            }
         }
      }

   } // namespace user

} // namespace ca5