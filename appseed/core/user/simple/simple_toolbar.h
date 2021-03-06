#pragma once


#define TBSTYLE_ALIGN_LEFT     0x10000
#define TBSTYLE_ALIGN_RIGHT    0x30000
#define TBSTYLE_ALIGN_CENTER   0x20000
#define TBSTYLE_ALIGN_TOP      0x40000
#define TBSTYLE_ALIGN_BOTTOM   0x80000
#define TBSTYLE_ALIGN_VCENTER  0xC0000


class image_list;



class CLASS_DECL_CORE simple_toolbar :
   virtual public ::user::toolbar
{

public:

   int                        m_iImageSpacing;
   size                       m_sizePress;
   size                       m_sizeSpacing;
   size                       m_sizeSeparator;
   rect                       m_rectBorder;
   rect                       m_rectItemPad;
   bool                       m_bTransparentBackground;
   bool                       m_bSimpleLayout;
   int32_t                    m_iHover;
   double                     m_dFontSize;

   size                       m_sizeButton;
   size                       m_sizeImage;
#ifdef WINDOWS
    HRSRC           m_hRsrcImageWell; // handle to loaded resource for image well
#endif
   HINSTANCE        m_hInstImageWell; // instance handle to load image well from
   HBITMAP          m_hbmImageWell; // contains color mapped button images
//   sp(image_list)    m_pimagelist;
//   sp(image_list)    m_pimagelistHue;
//   sp(image_list)    m_pimagelistBlend;
//   sp(image_list)    m_pimagelistHueLight;
   //bool           m_bInternalImageList;
   bool             m_bDelayedButtonLayout;
   ::draw2d::dib_sp     m_dibDraft;

   



   simple_toolbar(::aura::application * papp);
   virtual ~simple_toolbar();

   virtual int  get_image_spacing();
   virtual size get_press_shift();
   virtual size get_item_spacing();
   virtual size get_separator_size();
   virtual rect get_bar_border();
   virtual rect get_item_pad();

   bool create(sp(::user::interaction) pParentWnd, uint32_t dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP, id nID = "__IDW_TOOLBAR"); 
   using ::user::interaction::create_window_ex;
   virtual bool create_toolbar(::user::interaction * pParentWnd, uint32_t dwCtrlStyle = TBSTYLE_FLAT, uint32_t dwStyle = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP,const RECT & rect = ::null_rect(), id nID = "__IDW_TOOLBAR") override;


   int32_t WrapToolBar(int32_t nCount, int32_t nWidth);
   void SizeToolBar( int32_t nCount, int32_t nLength, bool bVert = FALSE);

   size CalcDynamicLayout(int32_t nLength, uint32_t dwMode) override;
   size CalcLayout(uint32_t dwMode, int32_t nLength = -1) override;
   //bool CalcSize(size & size, bool bHorz);
   size CalcSize(int32_t nCount);
   virtual void OnBarStyleChange(uint32_t dwOldStyle, uint32_t dwNewStyle) override;
   virtual size CalcFixedLayout(bool bStretch, bool bHorz) override;


   void RemoveAllTools();
   virtual int32_t _001GetHoverItem() override;
   virtual void OnUpdateHover();
   void SetItemImage(int32_t iItem, int32_t iImage);
   void GetButtonText(int32_t i, string & str);
   void SetButtonStyle(int32_t nIndex, UINT nStyle);
   UINT GetButtonStyle(int32_t iButton);
   bool SetItemStyle(int32_t iItem, BYTE bStyle);
   int32_t GetItemStyle(int32_t iItem);
   void _001SetImageList(sp(image_list) pimagelist);
   void _001DiscardImageList();
   void _001OnClick(index iItem);
   int32_t _001HitTest(point pt);
   void _001Hover(point pt, bool bRedraw = true);
   void _001Hover(bool bRedraw = true);


   void on_layout() override;


   void _001DrawItem(::draw2d::graphics * pgraphics, int32_t iItem);
   void _001DrawSimpleToolbarItem(::draw2d::graphics * pgraphics, int32_t iItem);

/*
   bool LoadToolBar(const char * lpszResourceName);
   bool LoadToolBar(UINT nIDResource);
   bool LoadBitmap(const char * lpszResourceName);
   bool LoadBitmap(UINT nIDResource);
*/

//  virtual bool _001GetItemRect(int32_t iItem,LPRECT lprect)
   virtual bool _001GetElementRect(int32_t iItem,LPRECT lprect,e_element eelement) override;



//   bool SetButtons(const UINT* lpIDArray, int32_t nIDCount);
   void SetSizes(SIZE sizeButton, SIZE sizeImage);

   void on_command_probe(::user::frame_window * ptarget, bool bDisableIfNoHndler) override;


   void TransparentEraseNonClient(::draw2d::graphics * pgraphics);
   void SetTransparentBackground(bool bSet);
   virtual void _001OnDraw(::draw2d::graphics * pgraphics) override;
   size CalcSimpleLayout();

   void _001OnImageListAttrib();
   DECL_GEN_SIGNAL(_001OnCreate);
   DECL_GEN_SIGNAL(_001OnMouseMove);
   DECL_GEN_SIGNAL(_001OnLButtonDown);
   void _001OnTimer(::timer * ptimer) override;
   DECL_GEN_SIGNAL(_001OnLButtonUp);
   DECL_GEN_SIGNAL(_001OnNcCalcSize);
   DECL_GEN_SIGNAL(_001OnNcHitTest);
   DECL_GEN_SIGNAL(_001OnMouseLeave);

   virtual void install_message_routing(::message::sender * psender) override;


};
