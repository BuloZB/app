#pragma once


namespace user
{


   class CLASS_DECL_BASE form_callback:
      virtual public ::object
   {
   public:


      ::user::map_form_window *                        	m_pmapform;


      form_callback();
      virtual ~form_callback();


      virtual void on_update(::user::impact * pform,sp(::user::impact) pSender,LPARAM lHint,object* phint);
      virtual void on_control_event(::user::control_event * pevent);
      virtual void OnUser123(WPARAM wparam,LPARAM lparam);
      virtual void OnBeforeNavigate2(::html::data * pdata,var & varFile,uint32_t nFlags,const char * lpszTargetFrameName,byte_array& baPostedData,const char * lpszHeaders,bool* pbCancel);
      virtual void _001InitializeFormPreData(::user::form * pform);

      form_control * get_form_control(const string & strId);
      ::user::document * get_form_document(const string & strId);

   };


} // namespace user


