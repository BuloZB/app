#pragma once


namespace simple_ui
{


   class CLASS_DECL_BASE edit_box :
      virtual public interaction
   {
   public:


      int_ptr         m_iPos;


      edit_box(sp(::base::application) papp);
      virtual ~edit_box();


      virtual void install_message_handling(::message::dispatch * pdispatch);

      virtual void _001OnDraw(::draw2d::graphics * pgraphics);


      DECL_GEN_SIGNAL(_001OnChar);
      DECL_GEN_SIGNAL(_001OnLButtonDown);
      DECL_GEN_SIGNAL(_001OnLButtonUp);

      virtual bool keyboard_focus_is_focusable();


   };



} // namespace simple_ui



