#include "StdAfx.h"

namespace backup
{

form_callback::form_callback()
{
}

form_callback::~form_callback()
{
}

void form_callback::on_update(::user::form * pview, ::view* pSender, LPARAM lHint, ::radix::object* phint) 
{
   UNREFERENCED_PARAMETER(pview);
   UNREFERENCED_PARAMETER(pSender);
   UNREFERENCED_PARAMETER(lHint);
   UNREFERENCED_PARAMETER(phint);
}


bool form_callback::BaseOnControlEvent(::user::form * pview, ::user::control_event * pevent)
{

   if(pevent->m_eevent == ::user::event_button_clicked)
   {
      if(pevent->m_puie->m_id == "lfs")
      {
         form_update_hint uh;
         uh.m_etype = form_update_hint::type_browse;
         uh.m_strForm = "filemanager_add_location_lfs.xhtml";
         pview->get_document()->update_all_views(NULL, 0, &uh);
         ::user::interaction * pguie = pview->GetChildByName("lfs");
         text_interface * ptext = dynamic_cast < text_interface * > (pguie);
         string strPath;
         ptext->_001SetText(strPath);
      }
      else if(pevent->m_puie->m_id == "ftp")
      {
         form_update_hint uh;
         uh.m_etype = form_update_hint::type_browse;
         uh.m_strForm = "filemanager_add_location_ftp.xhtml";
         pview->get_document()->update_all_views(NULL, 0, &uh);
      }
      else if(pevent->m_puie->m_id == "submit")
      {
/*         if(pview->m_strPathName == "filemanager_add_location_lfs.xhtml")
         {
            stringa wstra;
            //VmsDataGet(m_pmanager->get_filemanager_data()->m_ptemplate->m_datakeyStatic, 0, 0, wstra);
            Ex1VirtualGuie * pguie = pview->GetChildByName("lfs");
            text_interface * ptext = dynamic_cast < text_interface * > (pguie);
            string str;
            ptext->_001GetText(str);
            wstra.AddUnique(str);
            //VmsDataSet(m_pmanager->get_filemanager_data()->m_ptemplate->m_datakeyStatic, 0, 0, wstra);

         }
         else if(pview->m_strPathName == "filemanager_add_location_ftp.xhtml")
         {
         }
         else if(pview->m_strPathName == "filemanager\\replace_name_in_file_system.xhtml")
         {
            form_update_hint uh;
            Ex1VirtualGuieInterface * pguie = pview->GetChildByName("encontrar");
            text_interface * ptext = dynamic_cast < text_interface * > (pguie);
            //ptext->_001GetText(uh.m_strFind);
            pguie = pview->GetChildByName("substituir");
            ptext = dynamic_cast < text_interface * > (pguie);
            //ptext->_001GetText(uh.m_strReplace);
            pview->GetDocument()->update_all_views(NULL, 0, &uh);
         }*/
      }
   }
   return false;
}

} // namespace backup