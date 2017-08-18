#pragma once



#if defined(__cplusplus) || defined(__cplusplus_winrt)

#include "axis/user/user.h"


#define new AURA_NEW


class simple_scroll_bar;


namespace user
{

   class interaction;
   class create_struct;
   class control_event;

   class menu_item;
   class menu_item_ptra;
   class menu;

   class tree;
   class plain_text_group_command;

   class tooltip;

   class control_event;
   class create_context;
   class printer;
   class user;
   class frame_window;

   // class form_interface;
   class form_list;
   class form_callback;
   class form_view;


   class interaction;
   class interaction_impl;
   class view_creator_data;
   class tree;
   class str_context;
   class window_draw;
   class str;

   class interaction;
   class control_event;
   class create_context;
   class user;
   class frame_window;

   // class form_interface;
   class form_list;
   class form_callback;

   class interaction;
   class interaction_impl;
   class frame_window;
   class create_struct;


   class tab;

   class tab_pane;

   class toolbar;

   class scroll_bar;

   enum e_object
   {

      object_list,
      object_tree

   };

   enum e_style
   {

      style_default,
      style_menu,
      style_button,
      style_menu_button,
      style_menu_close,
      style_menu_popup,
      style_system_menu_button,
      style_system_menu_close,
      style_system_menu_popup,

   };

   using style_sp = sp(style);

} // namespace user


namespace simple_ui
{

   class label;
   class edit_box;
   class password;
   class tap;


}



typedef sp(::user::interaction_impl) window_sp;


namespace user
{

   class place_holder;

#if defined METROWIN

   class CLASS_DECL_BASE native_window_initialize
   {
   public:


      Agile<Windows::UI::Core::CoreWindow> window;
      ::axis::system_window ^ pwindow;


   };

#elif defined(APPLE_IOS) || defined(ANDROID)

   class CLASS_DECL_BASE native_window_initialize
   {
   public:

      RECT   m_rect;

   };

#else

   class native_window_initialize;

#endif


   class control_event;
   class frame_window;


   CLASS_DECL_BASE bool is_descendant(::user::interaction * puiParent, ::user::interaction * puiChild);


} // namespace user



#if defined(__cplusplus) || defined(__cplusplus_winrt)


namespace user
{

   class CLASS_DECL_BASE create_struct :
#ifdef WINDOWSEX
      public CREATESTRUCTW
#else
      public CREATESTRUCTA
#endif
   {
   public:


      create_struct & operator = (const RECT & rect)
      {

         x = rect.left;
         y = rect.top;
         cx = rect.right - rect.left;
         cy = rect.bottom - rect.top;

         return *this;

      }


      create_struct & operator = (LPCRECT lpcrect)
      {

         if (lpcrect == NULL)
         {

            x = 0;
            y = 0;
            cx = 0;
            cy = 0;

         }
         else
         {

            operator = (*lpcrect);

         }

         return *this;

      }


   };


} // namespace user


#endif







#ifndef TBSTYLE_TOOLTIPS
#define TBSTYLE_TOOLTIPS        0x0100
#define TBSTYLE_WRAPABLE        0x0200
#define TBSTYLE_ALTDRAG         0x0400
#define TBSTYLE_FLAT            0x0800
#define TBSTYLE_LIST            0x1000
#define TBSTYLE_CUSTOMERASE     0x2000
#define TBSTYLE_REGISTERDROP    0x4000
#define TBSTYLE_TRANSPARENT     0x8000
#endif


#include "user/user_mouse.h"


#include "user/user_style_base.h"
#include "user/user_style_composite.h"
#include "user/user_style.h"


#include "base/base/message/message_user.h"




#include "base/user/user/user_message_queue_listener.h"

  //#include "user/user/user_keyboard_focus.h"
  //#include "user/user/user_mouse_focus.h"
  //#include "base/user/user/user_elemental.h"
  //#include "user/user/user_check_interface.h"
  //#include "base/user/user/user_schema_simple_impl.h"
  //#include "base/user/user/user_schema_layered_frame.h"
  //#include "user/user/user_text_interface.h"
  //#include "user/user/user_draw_interface.h"
#include "base/user/user/user_window_util.h"
#include "base/user/user/user_interaction_base.h"
  //#include "user/user/user_buffer.h"
#include "base/user/user/user_interaction.h"
#include "base/user/user/user_interaction_impl_base.h"
#include "base/user/user/user_interaction_child.h"
#include "base/user/user/user_interaction_impl.h"

#endif






#pragma once


#if defined(__cplusplus) || defined(__cplusplus_winrt)

  //#include "base/user/user/user_front_end_schema.h"
  //#include "base/user/user/user_front_end.h"
#include "base/user/user/user_control_bar.h"
#include "base/user/user/user_wait_cursor.h"
#include "simple_ui/simple_ui_style.h"
#include "simple_ui/simple_ui_interaction.h"
#include "base/user/user/user_scroll.h"
#include "base/user/user/user_box.h"
#include "base/user/user/user_frame_window.h"
#include "base/user/user/user_server.h"
#include "base/user/user/user_impact_system.h"


#include "base/user/user/user_tooltip.h"


#ifdef WINDOWSEX




#endif




#include "base/user/user/user_form_callback.h"


#include "base/user/user/user_document_data_map.h"
#include "base/user/user/user_document.h"
  //#include "base/user/user/user_application_as_proxy_document.h"
#include "base/user/user/user_impact.h"





  //#if defined(METROWIN)
  //
  //#include "app/appseed/base/base/os/metrowin/metrowin_sockets.h"
  //
  //#endif



#include "base/user/user/user_window_map.h"



#endif



