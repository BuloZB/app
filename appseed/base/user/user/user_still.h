#pragma once


namespace user
{


   class CLASS_DECL_BASE still :
      virtual public control
   {
   public:

      enum e_style
      {
         style_none,
         style_text,
         style_image
      };

      ::visual::dib_sp                    m_dib;          // not pressed default bitmap
      e_rect                              m_erectMargin;
      e_rect                              m_erectBorder;
      e_rect                              m_erectPadding;
      e_int                               m_eintTextAlign;
      e_style                             m_estyle;
      ::check::e_check                    m_echeck;
      rect                                m_rectText;
      index                               m_iClick;
      //      ::user::front_end_schema::button *  m_pschema;
      //      ::user::front_end_schema::button *  m_pschemaDraw;
      e_stock_icon                        m_estockicon;

      rect                    m_rectCheckBox;
      string m_strLink;

      still();
      still(::aura::application * papp);
      virtual ~still();


      virtual bool should_hover();

      virtual bool has_link();

      virtual bool create_control(class control_descriptor * pdescriptor) override;

      virtual void resize_to_fit() override;

      virtual void install_message_routing(::message::sender * pinterface) override;

      virtual void _001OnDrawImage(::draw2d::graphics * pgraphics);

      virtual void _001OnDraw(::draw2d::graphics * pgraphics) override;
      virtual void _002OnDraw(::draw2d::graphics * pgraphics);

      virtual void _001SetCheck(::check::e_check check, ::action::context actioncontext) override;
      virtual ::check::e_check _001GetCheck() override;

      virtual bool is_pressed();


      virtual index hit_test(point pt, e_element & eelement) override;

      DECL_GEN_SIGNAL(_001OnKeyDown);
      DECL_GEN_SIGNAL(_001OnLButtonDown);
      DECL_GEN_SIGNAL(_001OnLButtonUp);
      DECL_GEN_SIGNAL(_001OnMButtonDown);
      DECL_GEN_SIGNAL(_001OnMButtonUp);
      DECL_GEN_SIGNAL(_001OnMouseMove);
      DECL_GEN_SIGNAL(_001OnMouseLeave);
      DECL_GEN_SIGNAL(_001OnCreate);


      ::size calc_text_size();

      virtual bool keyboard_focus_is_focusable() override;

      virtual bool has_action_hover() override;

      bool LoadBitmaps(::var var, ::var varSel = ::var::type_null, ::var varFocus = ::var::type_null, ::var varDisabled = ::var::type_null, ::var varHover = ::var::type_null);


      virtual void set_button_style(e_style estyle);
      virtual void on_enter_button_style(e_style estyle);
      virtual void on_exit_button_style(e_style estyle);

      virtual void BaseToolTipRelayEvent(::message::message * pobj);
      virtual void BaseToolTipGetRect(LPRECT lprect);
      virtual int32_t BaseToolTipGetIndex();

      virtual void pre_translate_message(::message::message * pobj) override;


      virtual void on_layout() override;


      virtual void set_stock_icon(e_stock_icon eicon) override;
      virtual e_stock_icon get_stock_icon() override;

   };


} // namespace user



