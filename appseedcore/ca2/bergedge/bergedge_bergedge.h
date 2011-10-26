#pragma once


namespace nature
{


   class database;


} // namespace nature


namespace bergedge
{


   class view;


   class CLASS_DECL_ca bergedge :
      virtual public platform::application,
      virtual public filemanager::item_action,
      virtual public ::ca::bergedge
   {
   public:


      class run_application
      {
      public:
         run_application();
         index                   m_iEdge;
         string                  m_strApp;
         string                  m_strQuery;
         ::bergedge::bergedge *  m_pbergedgeParent;
         ::ca::application *     m_papp;
         bool                    m_bMakeVisible;
         ::user::interaction *   m_puiParent;
         gen::property_set       m_setParameters;
      };


      class run_start_installer
      {
      public:
         virtual void run_start_install(const char * pszInstall) = 0;
      };

      class CLASS_DECL_ca map :
         virtual public ::collection::map < int, int, ::bergedge::bergedge *, ::bergedge::bergedge * >
      {
      };

      
      index                                                 m_iEdge;

      bool                                                  m_bShowPlatform;

      ::ca::application *                                   m_pappCurrent;
      ::collection::string_map < ::ca::application * >      m_mapApplication;


      ::userbase::single_document_template *                m_ptemplate_bergedge;
      ::userbase::single_document_template *                m_ptemplate_platform;
      ::userbase::single_document_template *                m_ptemplate_nature;
      ::userbase::single_document_template *                m_ptemplate_html;
      document *                                            m_pbergedgedocument;
      platform::document *                                  m_pplatformdocument;
      nature::document *                                    m_pnaturedocument;
      nature::database *                                    m_pdatabase;
      

      var                                                   m_varTopicFile;
      var                                                   m_varCurrentViewFile;
      
      bool                                                  m_bDrawCursor;


      bergedge();
      virtual ~bergedge();
      virtual void construct();

      bool initialize_instance();
      BOOL exit_instance();

      bool InitializeLocalDataCentral();

      virtual bool bergedge_start();

      void _001OnFileNew();

      virtual bool _001OnCmdMsg(BaseCmdMsg * pcmdmsg);

      virtual ::ca::application * get_app() const;

      void OnFileManagerOpenFile(::filemanager::data * pdata, ::fs::item_array & itema);

      void load_string_table();

      virtual bool file_manager_open_file(::filemanager::data * pdata, ::fs::item_array & itema);

      void initialize_bergedge_application_interface();

      virtual bool create_bergedge(::ca::create_context * pcreatecontext);


      // semantics defined by application
      using platform::application::request;
      virtual void request(::ca::create_context * pcreatecontext);
      // main loosely coupled semantics
      // varFile   : empty, one file path, many file paths, one file object, one or more file objects to be opened
      // varQuery  : more ellaborated requests for the application - syntax and semantic defined by requested application

      virtual ::user::interaction * get_request_parent_ui(::user::interaction * pinteraction, ::ca::create_context * pcontext);

      virtual ::user::interaction * get_request_parent_ui(::userbase::main_frame * pmainframe, ::ca::create_context * pcontext);

      virtual ::user::place_holder_ptra get_place_holder(::userbase::main_frame * pmainframe, ::ca::create_context * pcontext);

      virtual bool place(::userbase::main_frame * pmainframe, ::ca::create_context * pcontext);

      virtual void request_topic_file(var & varQuery);

      virtual void request_topic_file();

      virtual void check_topic_file_change();

      void on_exclusive_instance_conflict(::radix::EExclusiveInstance eexclusive);
      
      void launch_app(const char * psz);
      void install_app(const char * psz);

      bergedge::document *          get_document();
      bergedge::view *              get_view();
      platform::document *          get_platform();
      nature::document *            get_nature();
      
      

      void on_request(::ca::create_context * pcreatecontext);

      ::ca::application * application_get(const char * pszId, bool bCreate = true, bool bSynch = true, ::ca::application_bias * pbiasCreate = NULL);

      virtual bool open_by_file_extension(const char * pszPathName, ::ca::application_bias * pbiasCreate = NULL);

      virtual bool open_by_file_extension(::ca::create_context * pcreatecontext);
      
      virtual bool is_bergedge();

      ::ca::application * get_current_application();

      virtual void get_screen_rect(LPRECT lprect);

      virtual bool on_install();

   };


} // namespace bergedge



