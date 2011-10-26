#include "StdAfx.h"


namespace netshareservercfg
{


   configuration::configuration(::ca::application * papp) :
      ca(papp),
      ip_enum(papp)
   {
      m_ptemplatePane = new ::userbase::single_document_template(
         papp,
         "system/form",
         ::ca::get_type_info < form_document > (),
         ::ca::get_type_info < simple_frame_window > (),
         ::ca::get_type_info < userex::pane_tab_view > ());
   }

   configuration::~configuration()
   {
   }

   void configuration::set_configuration()
   {
      ip_enum::enumerate();
      form_document * pdoc = dynamic_cast < form_document * > (m_ptemplatePane->open_document_file());
      userex::pane_tab_view * pview = dynamic_cast < userex::pane_tab_view * > (pdoc->get_view());
      pview->set_view_creator(this);
      rect rectOpen;
      Application.get_screen_rect(rectOpen);
      int iWidth = rectOpen.width();
      int iHeight = rectOpen.width();
      rectOpen.deflate(iWidth / 3, iHeight / 3);
      pview->GetParentFrame()->SetWindowPos(ZORDER_TOP, rectOpen.left,
         rectOpen.top,
         rectOpen.width(), rectOpen.height(), SWP_SHOWWINDOW);
      m_ptabview = pview;
      pview->add_tab("netshare server setup", 1);
      pview->add_tab("network", 2);
      pview->set_cur_tab_by_id(1);
   }

   void configuration::on_create_view(::user::view_creator_data * pcreatordata)
   {
      switch(pcreatordata->m_id)
      {
      case 1:
         {
            m_pdocServer = Application.create_form(this, m_ptabview);
            if(m_pdocServer != NULL)
            {
               m_pviewServer = dynamic_cast < form_view * > (m_pdocServer->get_view());
               m_pviewServer->m_pcallback = this;

               string strIpEnum = System.file().time_square();

               string str;
               string strHtml;
               strHtml = "<html><head></head><body>";
               strHtml += "<h1>net share server cfg</h1>";
               stringa stra;
               string strCheck;
               stra = Application.db().data_load("netshareserver", ::radix::system::idEmpty, ::radix::system::idEmpty);
               for(int i = 0; i < m_straIp.get_size(); i++)
               {
                  if(stra.find_first(m_straIp[i]) >= 0)
                     strCheck = "checked";
                  else
                     strCheck = "unchecked";

                  str.Format("<input type=\"checkbox\" id=\"check%d\" value=\"%s\"/>%s<br /><br />", i, strCheck, m_straIp[i]);
                  strHtml += str;
               }
               strHtml += "<input type=\"button\" id=\"submit\" value=\"Salvar e Fechar\"/>";
               strHtml += "</body></html>";

               Application.file().put_contents(strIpEnum, strHtml);

               
               /*string strUsername;
               string strPassword;
               primitive::memory strUsernameEncrypt;
               primitive::memory strPasswordEncrypt;
               string strSection;
               strSection.Format("license_auth");
               string strDir;
               strDir = System.dir().usersystemappdata(strSection);
               bool bOk = false;
               System.dir().mk(strDir);
               ex1::filesp spfile(get_app());
               spfile->open(System.dir().path(strDir, "00001"), ::ex1::file::type_binary | ::ex1::file::mode_read_write);
               if(spfile->IsOpened())
               {
                  strUsernameEncrypt.FullLoad(spfile);
                  spfile->close();
               }
               spfile->open(System.dir().path(strDir, "00002"), ::ex1::file::type_binary | ::ex1::file::mode_read_write);
               if(spfile->IsOpened())
               {
                  strPasswordEncrypt.FullLoad(spfile);
                  spfile->close();
               }
               if(strUsernameEncrypt.get_size() > 0)
               {
                  primitive::memory strUsernameDecrypt;
                  primitive::memory strPasswordDecrypt;
                  Application.crypt().decrypt(strUsernameDecrypt, strUsernameEncrypt);
                  Application.crypt().decrypt(strPasswordDecrypt, strPasswordEncrypt);
                  strUsernameDecrypt.ToAsc(strUsername);
                  strPasswordDecrypt.ToAsc(strPassword);
                  m_pviewServer->m_pcontainer->m_puser = User(System.user().get_user());
                  m_pviewServer->m_pcontainer->m_strUser = strUsername;
                  m_pviewServer->m_pcontainer->m_strPassword = strPassword;*/
                  m_pviewServer->get_html_data()->open_document(strIpEnum);
                  pcreatordata->m_pdoc = m_pdocServer;
                  pcreatordata->m_pwnd = m_pviewServer->GetParentFrame();
/*
               }*/
            }
         }
         break;
      case 2:
         {
         }
         break;
      }
   }



   bool configuration::BaseOnControlEvent(::user::form * pview, ::user::control_event * pevent)
  {
     UNREFERENCED_PARAMETER(pview);
   if(pevent->m_eevent == ::user::event_button_clicked)
   {
      if(pevent->m_puie->m_id == "submit")
      {
         string str;
         stringa straSubmit;
         for(int i = 0; i < m_straIp.get_size(); i++)
         {
            str.Format("check%d", i);
            ::user::interaction * pguie = m_pviewServer->GetChildById(str);
            ::user::check_interface * pcheck = dynamic_cast < ::user::check_interface * > (pguie);
            if(pcheck->_001GetCheck() == check::checked)
            {
               straSubmit.add(m_straIp[i]);
            }
         }
         
         Application.db().data_save("netshareserver", ::radix::system::idEmpty, ::radix::system::idEmpty, straSubmit);
         //m_pviewServer->GetTopLevelParent()->EndModalLoop(IDOK);
         m_pviewServer->GetTopLevelParent()->PostMessageA(WM_CLOSE);
      }
   }
   return false;
}


   void configuration::OnBeforeNavigate2(html::data * pdata, var & varUrl, DWORD nFlags, const char * lpszTargetFrameName, byte_array& baPostedData, const char * lpszHeaders, BOOL* pbCancel)
   {
      UNREFERENCED_PARAMETER(pdata);
      UNREFERENCED_PARAMETER(nFlags);
      UNREFERENCED_PARAMETER(lpszTargetFrameName);
      UNREFERENCED_PARAMETER(baPostedData);
      UNREFERENCED_PARAMETER(lpszHeaders);
      string str(varUrl);
      if(gen::str::begins_eat(str, "netshareserversfg://"))
      {
         m_pviewServer->GetTopLevelParent()->EndModalLoop(IDOK);
         m_pviewServer->GetTopLevelParent()->ShowWindow(SW_HIDE);
         *pbCancel = TRUE;
      }
   }

   BOOL configuration::EnumCallbackFunction(int nAdapter, const in_addr& address)
   {
      UNREFERENCED_PARAMETER(nAdapter);
      string strIpAddress = inet_ntoa(address);
      m_straIp.add(strIpAddress);
      return TRUE;
   }


} // namespace netshareservercfg


