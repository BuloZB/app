#include "StdAfx.h"


namespace backup
{

   pane_view::pane_view(::ca::application * papp) :
      ca(papp),
      ::user::tab(papp),
      ::userbase::view(papp),
      ::userex::pane_tab_view(papp),
      ::userbase::tab_view(papp),
      place_holder_container(papp)
   {

      m_etranslucency      = TranslucencyPresent;

   }

   pane_view::~pane_view()
   {
   }


   #ifdef _DEBUG
   void pane_view::assert_valid() const
   {
	   ::userbase::view::assert_valid();
   }

   void pane_view::dump(dump_context & dumpcontext) const
   {
	   ::userbase::view::dump(dumpcontext);
   }
   #endif //_DEBUG

   void pane_view::_001OnCreate(gen::signal_object * pobj) 
   {
//      SCAST_PTR(::user::win::message::create, pcreate, pobj)

      if(pobj->previous())
         return;

      add_tab("backup", backup::pane_view_backup);
      add_tab("Options", backup::pane_view_options);
      
      set_cur_tab_by_id(backup::pane_view_backup);
      
   }

   void pane_view::on_update(::view* pSender, LPARAM lHint, ::radix::object* pHint) 
   {
      ::userbase::tab_view::on_update(pSender, lHint, pHint);
      if(lHint == 543218)
      {
         set_cur_tab_by_id(backup::pane_view_backup);
      }
      if(pHint != NULL)
      {
         if(base < pane_view_update_hint >::bases(pHint))
         {
            pane_view_update_hint * puh = (pane_view_update_hint *) pHint;
            if(puh->is_type_of(pane_view_update_hint::TypeGetView))
            {
            }
            else if(puh->is_type_of(pane_view_update_hint::TypeSetView))
            {
               set_cur_tab_by_id(puh->m_eview);
            }
         }
      }
	      if(pHint != NULL)
      {
   	   if(base < pane_view_update_hint >::bases(pHint))
         {
            pane_view_update_hint * puh = (pane_view_update_hint * ) pHint;
            if(puh->is_type_of(pane_view_update_hint::TypeOnShowView))
            {
               
//               int iTab;
   //            if(puh->m_eview == PaneViewContextMenu)
     //          {
       //           m_tab._001AddSel(0);
         //      }
   //            else
               {
               }
               
            }
         }
      }


   }




   void pane_view::on_create_view(::user::view_creator_data * pcreatordata)
   {
//      application * papp = dynamic_cast < application * > (get_app());
      switch(pcreatordata->m_id)
      {
      case pane_view_backup:
         {
            pcreatordata->m_pwnd = create_view < backup::view > ();
            if(pcreatordata->m_pwnd != NULL)
            {
               pcreatordata->m_pdoc = get_document();
            }
         }
         break;
      case pane_view_options:
      {
         form_document * pdoc = App(get_app()).create_form(this, this);
         if(pdoc == NULL)
            return;
         ::view * pview = pdoc->get_view();
         form_update_hint uh;
         uh.m_etype = form_update_hint::type_browse;
         uh.m_strForm = "filemanager\\replace_name_in_file_system.xhtml";
         pdoc->update_all_views(NULL, 0, &uh);
         
         uh.m_etype = form_update_hint::type_get_form_view;
         pdoc->update_all_views(NULL, 0, &uh);

         uh.m_etype = form_update_hint::type_after_browse;
         pdoc->update_all_views(NULL, 0, &uh);


         pcreatordata->m_pwnd = dynamic_cast < ::ca::window * >(pview->GetParentFrame());
//         form_child_frame * pframe = dynamic_cast < form_child_frame * >(pcreatordata->m_pwnd);
         pcreatordata->m_pdoc = pdoc;

      }
      break;
      default:
         ASSERT(FALSE);
         break;
      }

   }


   void pane_view::install_message_handling(::user::win::message::dispatch * pinterface)
   {
      ::userex::pane_tab_view::install_message_handling(pinterface);
	   IGUI_WIN_MSG_LINK(WM_CREATE       , pinterface, this, &pane_view::_001OnCreate);
   }


} // namespace backup