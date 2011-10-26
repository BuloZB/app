#pragma once

class FileManagerCallbackInterface;
class FileManagerInterface;
class ::fs::item;
class ::fs::item_array;
class FileManagerTemplate;
class CWStrArray;
class CWStr;

namespace filemanager
{

   class CLASS_DECL_ca data :
      virtual public ::fs::tree_data,
      virtual public ::fs::list_data
   {
   public:


      ::document *                     m_pdocumentSave;
      file_manager_interface *         m_pfilemanager;
      FileManagerInterface *           m_pmanager;
      FileManagerInterface *           m_pmanagerMain;
      FileManagerCallbackInterface *   m_pcallback;
      FileManagerTemplate *            m_ptemplate;
      bool                             m_bTransparentBackground;
      string                           m_strDISection;
      ::user::place_holder *           m_pholderFileList;
      ::user::interaction *            m_ptreeFileTreeMerge;
      bool                             m_bSetBergedgeTopicFile;
   
   

   //   UINT  m_uiToolBar;
   //   UINT  m_uiMenuBar;

      int   m_iTemplate;
      int   m_iDocument;
      int   m_iIconSize;
      bool  m_bListText;
      bool  m_bListSelection;
      bool  m_bFileSize;
      bool  m_bPassBk;
      bool  m_bIconView;


      data(::ca::application * papp);
      virtual ~data();

      bool is_saving();

      void FileManagerBrowse(const char * lpcsz);
      void FileManagerBrowse(const ::fs::item & item);
      void OnFileManagerOpenContextMenuFolder(const ::fs::item & item);
      void OnFileManagerOpenContextMenuFile(const ::fs::item_array & itema);
      void OnFileManagerOpenContextMenu();
      void OnFileManagerOpenFile(const ::fs::item_array & itema);
      void OnFileManagerOpenFolder(const ::fs::item & str);

      void OnFileManagerItemUpdate(cmd_ui * pcmdui, const ::fs::item_array & itema);
      void OnFileManagerItemCommand(const char * pszId, const ::fs::item_array & itema);



   };

} // namespace filemanager