#pragma once


namespace winactionarea
{


   class view;


   class CLASS_DECL_CA2_WINACTIONAREA pane_view : 
      public ::userex::pane_tab_view,
      public FileManagerCallbackInterface,
      public winactionarea::form_callback
   {
   public:


      winactionarea::view *                     m_pwinactionareaview;


	   pane_view(::ca::application * papp);


      void rotate();
      
      void on_create_view(::user::view_creator_data * pcreatordata);
      void on_show_view();

      virtual void on_before_dock();
      virtual void on_after_dock();

      virtual void install_message_handling(::user::win::message::dispatch * pinterface);

      //void check_menu_dir(const char * psz);
      void check_3click_dir(const char * psz);

      virtual void on_update(::view * pSender, LPARAM lHint, ::radix::object* pHint);
	   virtual BOOL PreCreateWindow(CREATESTRUCT& cs);


      void request(gen::command_line * pline);

      DECL_GEN_SIGNAL(_001OnMenuMessage)
	   virtual ~pane_view();
   #ifdef _DEBUG
	   virtual void assert_valid() const;
	   virtual void dump(dump_context & dumpcontext) const;
   #endif

	   DECL_GEN_SIGNAL(_001OnCreate)
      DECL_GEN_SIGNAL(_001OnTimer)
      DECL_GEN_SIGNAL(_001OnShowWindow)

   };


} // namespace winactionarea


