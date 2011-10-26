#include "StdAfx.h"


// IMPLEMENT_DYNCREATE(file_manager_operation_list_view, ::userbase::view)

file_manager_operation_list_view::file_manager_operation_list_view(::ca::application * papp) :
   ca(papp),
   m_headerctrl(papp),
   ::userbase::view(papp),
   ::user::scroll_view(papp),
   ::user::list(papp)
{
   m_dwLast123Update = ::GetTickCount();
   m_pcache = &m_listcache;
   m_pheaderctrl     = &m_headerctrl;
   m_pheaderctrl->SetBaseListCtrlInterface(this);
}

void file_manager_operation_list_view::install_message_handling(::user::win::message::dispatch * pinterface)
{
   ::userbase::view::install_message_handling(pinterface);
   ::user::list::install_message_handling(pinterface);
   IGUI_WIN_MSG_LINK(WM_TIMER, pinterface, this, &file_manager_operation_list_view::_001OnTimer);
   IGUI_WIN_MSG_LINK(WM_CREATE, pinterface, this, &file_manager_operation_list_view::_001OnCreate);
}

void file_manager_operation_list_view::OnDraw(::ca::graphics * pdc)
{
   UNREFERENCED_PARAMETER(pdc);
}

count file_manager_operation_list_view::_001GetItemCount()
{
   if(get_document() == NULL)
      return 0;
   return get_document()->m_thread.get_item_count();
}

file_manager_operation_document * file_manager_operation_list_view::get_document()
{
   return dynamic_cast < file_manager_operation_document * > (::userbase::view::get_document());
}

void file_manager_operation_list_view::_001GetItemText(::user::list_item * pitem)
{
   if(pitem->m_iSubItem == 0)
   {
      pitem->m_strText = get_document()->m_thread.get_item_message(pitem->m_iItem);
      pitem->m_bOk = true;
   }
   else
   {
      double d;
      d = get_document()->m_thread.get_item_progress(pitem->m_iItem);
      pitem->m_strText.Format("%0.1f%%", d * 100.0);
      pitem->m_bOk = true;
   }
}

void file_manager_operation_list_view::_001InsertColumns()
{
   ::user::list_column column;
   column.m_iWidth               = 500;
   column.m_iSubItem             = 0;
   column.m_sizeIcon.cx          = 16;
   column.m_sizeIcon.cy          = 16;
   _001AddColumn(column);
   column.m_iWidth               = 500;
   column.m_iSubItem             = 1;
   column.m_sizeIcon.cx          = 16;
   column.m_sizeIcon.cy          = 16;
   _001AddColumn(column);

}
void file_manager_operation_list_view::_001OnInitialUpdate(gen::signal_object * pobj)
{
   UNREFERENCED_PARAMETER(pobj);
   m_dataid = "file_manager_operation_list_view";
   _001UpdateColumns();
}


void file_manager_operation_list_view::on_update(::view * pSender, LPARAM lHint, ::radix::object * pHint)
{
   UNREFERENCED_PARAMETER(pSender);
   UNREFERENCED_PARAMETER(pHint);
   if(lHint == 123)
   {
      /*if(::GetTickCount() - m_dwLast123Update > 500)
      {
         m_dwLast123Update = ::GetTickCount();
         _001OnUpdateItemCount();
         m_cache._001Invalidate();
      }*/

   }
}

void file_manager_operation_list_view::_001OnCreate(gen::signal_object * pobj)
{
   pobj->previous();
   SetTimer(123, 500, NULL);
}
void file_manager_operation_list_view::_001OnTimer(gen::signal_object * pobj)
{
   SCAST_PTR(::user::win::message::timer, ptimer, pobj);
   if(ptimer->m_nIDEvent == 123)
   {
       /*if(::GetTickCount() - m_dwLast123Update > 500)
      {
         m_dwLast123Update = ::GetTickCount();
         _001OnUpdateItemCount();
         m_cache._001Invalidate();
      }*/
  }
}

void file_manager_operation_list_view::OnFileOperationStep(int iOperation, bool bFinal)
{
   UNREFERENCED_PARAMETER(iOperation);
   if(bFinal || ::GetTickCount() - m_dwLast123Update > 184)
   {
      m_dwLast123Update = ::GetTickCount();
      _001OnUpdateItemCount();
      m_listcache._001Invalidate();
      int iItem = 0;
      for(int i = 0; i < get_document()->m_thread.m_iOperation; i++)
      {
         iItem += get_document()->m_thread.m_fileoperationa.get_size();
      }
      if(get_document()->m_thread.m_iOperation < get_document()->m_thread.m_fileoperationa.get_size())
      {
         iItem += get_document()->m_thread.m_fileoperationa[get_document()->m_thread.m_iOperation].m_iFile;
      }
      _001ItemScroll(iItem);
   }
}
