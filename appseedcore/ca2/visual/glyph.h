#pragma once

namespace visual
{

   class CLASS_DECL_ca glyph
   {
   public:
      glyph();
      virtual ~glyph();

   public:
      UINT   m_ui;
      base_array <point_array, point_array &> m_pointsets;
#ifdef WINDOWS
      GLYPHMETRICS   m_gm;
#endif

   public:
      void DrawGlyph(
         ::ca::graphics *      pdc,
         bool      bFill,
         double      dRateX,
         LPPOINT      lppointoffset);

#ifdef WINDOWS
      void Initialize(
         LPTTPOLYGONHEADER lpph,
         DWORD cbSize,
         int iFontHiHeight);
#endif

      void GetGlyphRect(
         int x,
         int y,
         LPRECT lpRect);

      void clear();

      glyph & operator = (const glyph & glyph);
      bool operator <= (const glyph & glyph) const;


   };

} // namespace visual
