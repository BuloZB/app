#pragma once


namespace winactionarea
{


   class CLASS_DECL_CA2_WINACTIONAREA application :
      public ::cube::application
   {
   public:
      
      
      class helper_thread :
         virtual public thread
      {
      public:
         helper_thread(::ca::application * papp);
         int run();
      };


      ::userbase::single_document_template *  m_ptemplate_html;
      comparable_array < HWND >     m_hwndaHidden;


      application(void);
      virtual ~application(void);


      virtual bool initialize_instance();
      virtual int  exit_instance();

      virtual void on_request(::ca::create_context * pcreatecontext);

      virtual void _001OnFileNew();

	   virtual bool _001OnCmdMsg(BaseCmdMsg * pcmdmsg);
   		
      virtual bool on_assert_failed_line(const char * lpszFileName, int iLine);

      virtual bool final_handle_exception(::ca::exception & e);

      void menu_view_check_menu_dir(const char * psz);


   };


   inline CLASS_DECL_CA2_WINACTIONAREA application & app_cast(::ca::application * papp)
   {
      return *dynamic_cast < application * > (papp);
   }


} // namespace winactionarea

