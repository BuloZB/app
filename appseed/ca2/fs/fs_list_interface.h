#pragma once


namespace fs
{


   class CLASS_DECL_ca2 list_interface :
      virtual public ::userbase::form_list
   {
   public:


      stringa      m_straFileSize;
      stringa      m_straStrictOrder;



      list_interface(sp(::ca::application) appp);
      virtual ~list_interface();

      virtual void _001InsertColumns();

      virtual bool _001OnUpdateItemCount(uint32_t dwFlags = 0);

      sp(document) get_document();
      virtual list_data * get_fs_list_data();

      virtual COLORREF get_background_color();
      void add_item(const char * pszPath, const char * pszTitle);
      //virtual void schedule_file_size(const char * psz) = 0;
      virtual void _017Synchronize();
      virtual void install_message_handling(::ca::message::dispatch * pinterface);

      sp(image_list) GetActionButtonImageList(index i);



      void GetSelected(item_array & itema);

      void _001OnInitializeForm(sp(::user::control) pcontrol);
      void _001OnButtonAction(sp(::user::control) pcontrol);

      virtual void _017OpenFile(const item_array & itema);
      virtual void _017OpenFolder(sp(::fs::item)  item);
      virtual void _017OpenFolder(const ::fs::list_item & item);
      virtual void _017OpenSelected(bool bOpenFile);
      virtual void _017OpenContextMenuSelected();
      virtual void _017OpenContextMenuFolder(sp(::fs::item)  item);
      virtual void _017OpenContextMenuFile(const item_array &itema);
      virtual void _017OpenContextMenu();
      void _017PreSynchronize();
      void TakeAnimationSnapshot();
      virtual void StartAnimation();
      virtual void _001OnDraw(::ca::graphics * pdc);
      void _017UpdateList();
      void GetSelectedFilePath(stringa & array);
      virtual void _001CreateImageList();
      bool _001CreateImageListStep();
      virtual void _001GetItemImage(::user::list_item * pitem);
      virtual void _001GetItemText(::user::list_item * pitem);
      virtual count _001GetItemCount();
      void _017UpdateList(const char * lpcsz);
      void _017UpdateZipList(const char * lpcsz);

      DECL_GEN_SIGNAL(_001OnHScroll)
      DECL_GEN_SIGNAL(_001OnVScroll)
      DECL_GEN_SIGNAL(_001OnFileRename)
      DECL_GEN_SIGNAL(_001OnUpdateFileRename)
      DECL_GEN_SIGNAL(_001OnShowWindow)


      virtual void _001InitializeFormPreData();



      virtual bool query_drop(index iDisplayDrop, index iDisplayDrag);
      virtual bool do_drop(index iDisplayDrop, index iDisplayDrag);


   };


} // namespace fs

