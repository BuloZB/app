#include "framework.h"

namespace user
{

   keyboard_focus::~keyboard_focus()
   {
      if(m_papp != ::null() && &System != ::null() && System.user().is_set())
      {
         if(System.user()->m_pkeyboardfocus == this)
         {
            Application.user()->set_keyboard_focus(::null());
         }
      }
   }

   void keyboard_focus::keyboard_focus_OnTimer(int32_t iTimer)
   {
      UNREFERENCED_PARAMETER(iTimer);
   }

   void keyboard_focus::keyboard_focus_OnChar(::ca::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void keyboard_focus::keyboard_focus_OnSysChar(::ca::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   void keyboard_focus::keyboard_focus_OnKeyDown(::ca::signal_object * pobj)
   {

      SCAST_PTR(::ca::message::key, pkey, pobj)

      if(pkey->m_ekey == ::user::key_tab)
      {
         control_event ev;
         ev.m_puie         = dynamic_cast < interaction * > (this);
         ev.m_eevent       = ::user::event_tab_key;
         ev.m_bUser        = true;
         get_parent()->BaseOnControlEvent(&ev);
         BaseOnControlEvent(&ev);
      }

   }

   void keyboard_focus::keyboard_focus_OnKeyUp(::ca::signal_object * pobj)
   {
      UNREFERENCED_PARAMETER(pobj);
   }

   bool keyboard_focus::keyboard_focus_is_focusable()
   {
      return false;
   }

   bool keyboard_focus::keyboard_focus_OnSetFocus()
   {
      // get_keyboard_focus will return the control with focus

      // return true to set focus to this control
      return true;
   }

   keyboard_focus * keyboard_focus::keyboard_get_next_focusable()
   {
      interaction * puiThis = dynamic_cast < interaction * > (this);
      if(puiThis == ::null())
         return ::null();
      single_lock (&puiThis->m_pthread->m_pthread->m_mutex, TRUE);
      interaction * pui = puiThis->above_sibling();
      while(pui != ::null())
      {
         if(pui->keyboard_focus_is_focusable())
            return pui;
         pui = pui->above_sibling();
      }
      pui = puiThis->get_parent()->get_bottom_child();
      while(pui != ::null() && pui != puiThis)
      {
         if(pui->keyboard_focus_is_focusable())
            return pui;
         pui = pui->above_sibling();
      }
      return ::null();
   }

   bool keyboard_focus::keyboard_set_focus()
   {
      Application.user()->set_keyboard_focus(dynamic_cast < ::user::interaction * > (this));
      return true;
   }

} // namespace user
