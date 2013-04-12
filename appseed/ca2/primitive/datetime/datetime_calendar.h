#pragma once


namespace html
{


   class file;


} // namespace html


class CLASS_DECL_ca2 calendar :
   public calendar_interface
{
protected:


   point    m_point;
   size     m_size;
   int32_t      m_iColWidth;
   int32_t      m_iLineHeight;


public:

   
   ::ca::font_sp    m_font;
   ::ca::font_sp    m_fontYear;
   ::ca::font_sp    m_fontMonth;
   ::ca::font_sp    m_fontSpin;


   calendar(sp(::ca::application) papp);


   virtual void GetRectDay(::datetime::time & time, LPRECT lprect);
   virtual void GetRectDay(int32_t iWeekDay, int32_t iLine, LPRECT lprect);
   virtual void GetRect(LPRECT lprect, enum EElement eelement);

   void _001OnDraw(::ca::graphics * pdc);
   void _001GetHtml(sp(::html::file) pfile);

   void set_pos(point pt, size sz);

   virtual EElement hit_test(point pt);
   virtual bool hit_test(EElement eelement, point pt);
   virtual bool time_hit_test(::datetime::time & time, point pt);





};



