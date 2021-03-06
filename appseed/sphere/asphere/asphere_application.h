﻿#pragma once


namespace asphere
{


   class CLASS_DECL_SPHERE application :
      virtual public ::core::application
   {
   public:


      bool                                         m_bContextTheme;
      colorertake5::ParserFactory *                m_pparserfactory;


      stringa                                      m_straTheme;
      string                                       m_strTheme;
      ::file::path                                 m_pathTheme;
      string_map < COLORREF >                      m_mapBg;
      string_map < COLORREF >                      m_mapIconGlow;
      string_map < COLORREF >                      m_mapFg;
      string_map < COLORREF >                      m_mapMg;
      string_map < COLORREF >                      m_mapBack;
      string_map < COLORREF >                      m_mapText;
      COLORREF                                     m_crBg;
      COLORREF                                     m_crFg;
      COLORREF                                     m_crMg;
      COLORREF                                     m_crBack;
      COLORREF                                     m_crText;
      COLORREF                                     m_crIconGlow;


      application();
      virtual ~application();


      virtual bool init_instance() override;
      virtual bool init1() override;
      virtual bool init() override;


      virtual void get_theme_text_color(COLORREF & crText, COLORREF & crBack);

      virtual void sync_context_theme();
      virtual string get_theme_context();
      virtual string get_context_default_theme();
      virtual string get_current_context_theme();
      virtual void set_context_theme(string strTheme);
      virtual void on_change_theme();
      virtual string get_theme();

      virtual ::file::path   get_theme_path();
      inline COLORREF get_theme_bg_color() { return m_crBg; }
      inline COLORREF get_theme_fg_color() { return m_crFg; }
      inline COLORREF get_theme_mg_color() { return m_crMg; }
      inline COLORREF get_theme_icon_glow_color() { return m_crIconGlow; }

      virtual void _001OnTheme(::message::message * pobj);

      virtual uint32_t guess_code_page(const string & str);

      ::colorertake5::ParserFactory                &  parser_factory();

   };



} // namespace sphere


