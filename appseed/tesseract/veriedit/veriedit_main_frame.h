#pragma once

namespace veriedit
{

   class CLASS_DECL_CA2_TESSERACT main_frame : public simple_frame_window
   {
   public:
      main_frame(::ca::application * papp);
      virtual ~main_frame();

      simple_toolbar m_toolbar;

      bool LoadToolBar(const char * lpcszMatter);
   };

} // namespace syllomatter