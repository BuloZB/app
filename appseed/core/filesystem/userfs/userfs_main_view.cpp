#include "framework.h"


namespace userfs
{


   main_view::main_view(::aura::application * papp) :
      object(papp),
      ::user::split_layout(papp),
      
      ::user::split_view(papp),
      place_holder_container(papp)
   {
   }

   void main_view::install_message_routing(::message::sender * pinterface)
   {
      ::user::split_view::install_message_routing(pinterface);
      IGUI_MSG_LINK(WM_CREATE, pinterface, this, &main_view::_001OnCreate);
   }

   void main_view::_001OnCreate(::message::message * pobj)
   {
      pobj->previous();
      if(pobj->m_bRet)
         return;

//      sp(application) papp =  (get_app());
      //papp->m_pcoreapp->m_pmainview = this;
      
      SetPaneCount(2);
      
      SetSplitOrientation(orientation_vertical);
      
      set_position_rate(0, 0.2);
      
      initialize_split_layout();
      
      sp(::create) cc(allocer());

//      cc->m_usercreatecontext.m_pCurrentDoc = get_document();
  //    cc->m_usercreatecontext.m_typeinfoNewView = System.type_info < tree > ();
      sp(::user::interaction) pwnd = create_view(System.type_info < tree >(),get_document(),null_rect(),get_pane_holder(0),100);
      SetPane(0, pwnd, false);
//      sp(tree) ptree =  (pwnd);


//      cc->m_usercreatecontext.m_pCurrentDoc = get_document();
  //    cc->m_usercreatecontext.m_typeinfoNewView = System.type_info < list > ();
      pwnd = create_view(System.type_info < list >(),get_document(),null_rect(),get_pane_holder(1),101);
//      list * plist = dynamic_cast < list * > (pwnd);
      SetPane(1, pwnd, false);
      on_layout();


   }

   
   void main_view::_001OnTimer(::timer * ptimer)
   {

      ::user::split_view::_001OnTimer(ptimer);

   }


   ::user::document * main_view::get_document()
   {
      return  ::user::split_view::get_document();
   }


} // namespace userfs






