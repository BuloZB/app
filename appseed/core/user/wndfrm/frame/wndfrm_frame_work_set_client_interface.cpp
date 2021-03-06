#include "framework.h"


namespace user
{


   namespace wndfrm
   {


      namespace frame
      {


         WorkSetClientInterface::WorkSetClientInterface()
         {

            m_eupdown = updown_normal_frame;

            m_workset.set_app(m_pauraapp);

            m_pupdowntarget = NULL;

            m_bFullScreenOnZoom = false;

            m_bInitialFramePosition = true;

         }

         WorkSetClientInterface::~WorkSetClientInterface()
         {

         }

         sp(::user::interaction) WorkSetClientInterface::WfiGetWindow()
         {
            return  (this);
         }

         void WorkSetClientInterface::WfiOnClose()
         {

         }


         void WorkSetClientInterface::WfiOnMaximize()
         {


         }


         void WorkSetClientInterface::WfiOnFullScreen()
         {

         }


         void WorkSetClientInterface::WfiOnNotifyIcon()
         {

            ShowWindow(SW_HIDE);

         }

         void WorkSetClientInterface::WfiOnMinimize(bool bNoActivate)
         {

         }

         void WorkSetClientInterface::WfiOnRestore()
         {

         }

         bool WorkSetClientInterface::WfiOnStartDock()
         {

            return true;

         }

         void WorkSetClientInterface::WfiOnDock(e_appearance eapperance)
         {

         }

         void WorkSetClientInterface::WfiOnUp()
         {

         }

         void WorkSetClientInterface::WfiOnDown()
         {

         }

         bool WorkSetClientInterface::Wfi(e_appearance eappearance)
         {

            if (!IsWindowVisible())
            {

               return true;

            }

            if(eappearance == appearance_current)
            {

               eappearance = m_eappearance;

            }

            if(eappearance == appearance_none)
            {

               return true;

            }

            if(::user::is_docking_appearance(eappearance))
            {
               WfiDock(eappearance);
            }
            else
            {
               switch(eappearance)
               {
               case appearance_zoomed:
                  WfiMaximize();
                  break;
               case appearance_iconic:
                  WfiMinimize();
                  break;
               case appearance_normal:
                  WfiRestore(true);
                  break;
               case appearance_notify_icon:
                  WfiNotifyIcon();
                  break;
               case appearance_full_screen:
                  WfiFullScreen();
                  break;
               case appearance_up:
                  WfiUp();
                  break;
               case appearance_down:
                  WfiDown();
                  break;
               default:
                  WfiRestore(true);
                  break;
               }
            }

            m_workset.update_control_box();

            return false;

         }


         bool WorkSetClientInterface::WfiDock(e_appearance eappearance)
         {

            if(!::user::is_docking_appearance(eappearance))
               return false;

            if(!WfiOnBeforeDock(eappearance))
               return false;

            m_workset.SetAppearance(eappearance);

            WfiOnDock(eappearance);

            WfiOnAfterDock(eappearance);

            m_workset.update_control_box();

            return false;

         }


         bool WorkSetClientInterface::WfiClose()
         {

            if(!WfiOnBeforeClose())
               return false;

            WfiOnClose();

            WfiOnAfterClose();

            return true;

         }


         bool WorkSetClientInterface::WfiRestore(bool bForceNormal)
         {

            ::user::e_appearance eappearanceRestore;

            if(bForceNormal)
            {

               eappearanceRestore = appearance_normal;

            }
            else if(m_workset.GetAppearance() == appearance_iconic
               || (m_workset.GetAppearance() == appearance_full_screen
               && m_eappearanceBefore != appearance_iconic))
            {

               eappearanceRestore = m_eappearanceBefore;

               if(m_eappearanceBefore == m_eappearance)
               {

                  eappearanceRestore = appearance_normal;

               }

            }
            else
            {

               eappearanceRestore = appearance_normal;

            }

            switch(eappearanceRestore)
            {
            case appearance_zoomed:
               return WfiMaximize();
            case appearance_iconic:
               return WfiMinimize();
            case appearance_full_screen:
               return WfiFullScreen();
            case appearance_notify_icon:
               return WfiNotifyIcon();
            case appearance_up:
               return WfiUp();
            case appearance_down:
               return WfiDown();
            default:
               break;
            }

            if(!WfiOnBeforeRestore())
               return false;

            m_workset.SetAppearance(appearance_normal);

            WfiOnRestore();

            WfiOnAfterRestore();

            m_workset.update_control_box();

            return true;

         }


         bool WorkSetClientInterface::WfiMaximize()
         {

            if (m_bFullScreenOnZoom)
            {

               return WfiFullScreen();

            }

            if(!WfiOnBeforeMaximize())
               return false;

            m_workset.SetAppearance(appearance_zoomed);

            WfiOnMaximize();

            WfiOnAfterMaximize();

            m_workset.update_control_box();

            return true;

         }


         bool WorkSetClientInterface::WfiFullScreen()
         {

            if(!m_workset.IsAppearanceEnabled())
            {

               index iMatchingMonitor = best_monitor(NULL,null_rect(),true);

               return iMatchingMonitor >= 0;

            }

            if(!m_workset.IsFullScreenEnabled())
               return false;

            if(!WfiOnBeforeFullScreen())
               return false;

            m_eappearanceBefore = m_workset.GetAppearance();

            m_workset.SetAppearance(appearance_full_screen);

            WfiOnFullScreen();

            WfiOnAfterFullScreen();

            m_workset.update_control_box();

            return true;

         }

         bool WorkSetClientInterface::WfiToggleTransparentFrame()
         {

            return true;

         }


         bool WorkSetClientInterface::WfiNotifyIcon()
         {

            if(!m_workset.IsNotifyIconEnabled())
               return false;

            if(!WfiOnBeforeNotifyIcon())
               return false;

            m_workset.SetAppearance(appearance_notify_icon);

            WfiOnNotifyIcon();

            WfiOnAfterNotifyIcon();

            m_workset.update_control_box();

            return true;

         }


         bool WorkSetClientInterface::WfiMinimize(bool bNoActivate)
         {

#ifndef LINUX

            if (m_workset.m_bMinimizeToTray)
            {

               return WfiNotifyIcon();

            }

#endif

            if(!WfiOnBeforeMinimize())
               return false;

            m_eappearanceBefore = m_workset.GetAppearance();

            m_workset.SetAppearance(appearance_iconic);

            WfiOnMinimize(bNoActivate);

            WfiOnAfterMinimize();

            m_workset.update_control_box();

            return true;

         }


         bool WorkSetClientInterface::WfiDown()
         {

            if(!WfiOnBeforeDown())
               return false;

            m_workset.SetAppearance(appearance_down);

            WfiOnDown();

            WfiOnAfterDown();

            m_workset.update_control_box();

            return true;

         }


         bool WorkSetClientInterface::WfiUp()
         {

            if(!WfiOnBeforeUp())
               return false;

            m_workset.SetAppearance(appearance_up);

            WfiOnUp();

            WfiOnAfterUp();

            m_workset.update_control_box();

            return true;

         }


         bool WorkSetClientInterface::WfiOnBeforeClose()
         {

            return true;

         }


         bool WorkSetClientInterface::WfiOnBeforeFullScreen()
         {

            if(m_eappearanceRequest == ::user::appearance_full_screen)
            {

               return false;

            }

            return true;

         }


         bool WorkSetClientInterface::WfiOnBeforeMaximize()
         {

            if(WfiIsFullScreen())
            {

               WfiOnExitFullScreen();

            }

            return true;

         }


         bool WorkSetClientInterface::WfiOnBeforeMinimize()
         {

            if(WfiIsFullScreen())
            {

               WfiOnExitFullScreen();

            }

            return true;

         }


         bool WorkSetClientInterface::WfiOnBeforeRestore()
         {

            if(WfiIsFullScreen())
            {

               WfiOnExitFullScreen();

            }

            return true;

         }


         bool WorkSetClientInterface::WfiOnBeforeNotifyIcon()
         {

            if(WfiIsFullScreen())
            {

               WfiOnExitFullScreen();

            }

            return true;

         }


         bool WorkSetClientInterface::WfiOnBeforeUp()
         {

            if(WfiIsFullScreen())
            {

               WfiOnExitFullScreen();

            }

            return true;

         }

         bool WorkSetClientInterface::WfiOnBeforeDock(e_appearance eappearance)
         {

            if(WfiIsFullScreen())
            {

               WfiOnExitFullScreen();

            }

            return true;

         }

         bool WorkSetClientInterface::WfiOnBeforeDown()
         {

            if(WfiIsFullScreen())
            {

               WfiOnExitFullScreen();

            }

            return true;

         }


         void WorkSetClientInterface::WfiOnAfterClose()
         {

         }

         void WorkSetClientInterface::WfiOnExitFullScreen()
         {

            m_workset.m_pframeschema->get_control_box()->KillTimer(123);

            m_workset.m_pframeschema->get_control_box()->ShowWindow(SW_SHOW);

         }


         void WorkSetClientInterface::WfiOnAfterFullScreen()
         {

            m_workset.m_pframeschema->get_control_box()->ShowWindow(SW_HIDE);

            m_workset.m_pframeschema->get_control_box()->SetTimer(123,184, NULL);

            WindowDataSaveWindowRect();

         }


         void WorkSetClientInterface::WfiOnAfterNotifyIcon()
         {
            WindowDataSaveWindowRect();
         }

         void WorkSetClientInterface::WfiOnAfterMaximize()
         {

            WindowDataSaveWindowRect();

         }

         void WorkSetClientInterface::WfiOnAfterMinimize()
         {
            WindowDataSaveWindowRect();
         }

         void WorkSetClientInterface::WfiOnAfterRestore()
         {
            WindowDataSaveWindowRect();
         }

         void WorkSetClientInterface::WfiOnAfterUp()
         {
            WindowDataSaveWindowRect();
         }

         void WorkSetClientInterface::WfiOnAfterDown()
         {
            WindowDataSaveWindowRect();
         }

         void WorkSetClientInterface::WfiOnAfterDock(e_appearance eappearance)
         {
            WindowDataSaveWindowRect();
         }

         bool WorkSetClientInterface::WfiOnBeginMoving(point pt)
         {
            UNREFERENCED_PARAMETER(pt);
            return false;
         }

         bool WorkSetClientInterface::WfiOnBeginSizing(UINT nType, point pt)
         {
            UNREFERENCED_PARAMETER(nType);
            UNREFERENCED_PARAMETER(pt);
            return false;
         }

         bool WorkSetClientInterface::WfiOnMove(bool bTracking)
         {
            UNREFERENCED_PARAMETER(bTracking);
            return false;
         }

         bool WorkSetClientInterface::WfiOnSize(bool bTracking)
         {
            UNREFERENCED_PARAMETER(bTracking);
            return false;
         }

         bool WorkSetClientInterface::WfiIsFullScreen()
         {
            return m_workset.IsFullScreen();
         }

         bool WorkSetClientInterface::WfiIsFullScreenEnabled()
         {
            return m_workset.IsFullScreenEnabled();
         }


         void WorkSetClientInterface::WfiEnableFullScreen(bool bEnable)
         {
            m_workset.EnableFullScreen(bEnable);
         }


         void WorkSetClientInterface::_001OnDraw(::draw2d::graphics * pgraphics)
         {

            m_workset._001OnDraw(pgraphics);

         }


         //WorkSetClientInterface::CWorkSetDownUpInterface()
         //{
         //
         //}
         void WorkSetClientInterface::frame_Attach()
         {
            if(m_pupdowntarget != NULL)
            {
               m_pupdowntarget->UpDownTargetAttach(this);
            }
         }
         void WorkSetClientInterface::frame_Detach()
         {
            if(m_pupdowntarget != NULL)
            {
               m_pupdowntarget->UpDownTargetDetach(this);
            }
         }



         void WorkSetClientInterface::_001OnTimer(::timer * ptimer)
         {

            m_workset._001OnTimer(ptimer);

         }


      } // namespace frame


   } // namespace wndfrm


} // namespace user




