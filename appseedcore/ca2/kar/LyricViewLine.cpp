﻿#include "StdAfx.h"
#include <math.h>

#define _DELETE(ptr) if(ptr != NULL) { delete ptr; ptr = NULL; }


#define ROUND(x,y) (((x)+(y-1))&~(y-1))
#define ROUND4(x) ROUND(x, 4)

AFX_STATIC fixed_alloc _vmsLyricViewLineAlloc(ROUND4(sizeof(LyricViewLine)), ROUND4(1024));

const int LyricViewLine::AlignLeft = 1;
const int LyricViewLine::AlignRight = 2;

LyricViewLine::OnVisible::OnVisible(::ca::application * papp) :
   m_font(papp),
   m_fontInt(papp),
   m_fontExt(papp),
   m_fontLink(papp),
   m_fontJp1(papp)
{

}

LyricViewLine::LyricViewLine(::ca::application * papp) :
   ca(papp),
   m_dibMain(papp),
   m_dibWork(papp),
   m_dibWork2(papp),
   m_dibWork3(papp),
   m_dibWorkB(papp),
   m_dibWorkB2(papp),
   m_dibWorkB3(papp)
{
   CommonConstruct();
}

LyricViewLine::LyricViewLine(const LyricViewLine & line) :
   ::ca::ca(line.get_app()),
   m_dibMain(line.get_app())
{
   CommonConstruct();
   operator = (line);
}

LyricViewLine::~LyricViewLine()
{
   if(m_ponvisible != NULL)
   {
      delete m_ponvisible;
   }
   if(m_ptemplate != NULL)
   {
      if(m_ptemplate->GetLine() == this)
      {
         m_ptemplate->SetLine(NULL);
      }
   }
}

LyricViewLine::CToken::CToken()
{
   clear();
}

LyricViewLine::CToken::CToken(const CToken & token)
{
   operator =(token);
}
LyricViewLine::CToken::~CToken()
{
}

LyricViewLine::CTokenJp1::CTokenJp1()
{
}
LyricViewLine::CTokenJp1::CTokenJp1(const CTokenJp1 & token)
{
   operator =(token);
}
LyricViewLine::CTokenJp1::~CTokenJp1()
{
}

LyricViewLine::CTokenSet::CTokenSet()
{
   m_pline        = NULL;
}

LyricViewLine::CTokenSet::~CTokenSet()
{
}


LyricViewLine::CTokenJp1Set::CTokenJp1Set()
{
}
LyricViewLine::CTokenJp1Set::~CTokenJp1Set()
{
}

LyricViewLine::ERenderResult LyricViewLine::Prepare(
   DWORD                dwCP,
   ::ca::graphics *     pdcForeground,
   stringa *          ptokena,
   int                  tokenIndex,
   int *                nextTokenIndex,
   int                  flags,
   LPCRECT              lpcrect,
   LyricEventsV2 *      pEvents)
{

   UNREFERENCED_PARAMETER(lpcrect);
   UNREFERENCED_PARAMETER(dwCP);

   m_bCacheEmboss = false;
   kar::KaraokeView * pview = GetKaraokeView();
   visual::font * pfont = pview->GetLyricFont();
   int               iTokens;
   int               iChars;
   int               iCharsFirst;
   m_iIndent;
   //   char            ch;
   string            strChar;
   string            strNextChar;
   string            str;
   int               iStr;
   int               iStrLen;
   int               xPosition;
   BOOL             forceInsertion = FALSE;
   ERenderResult            eresult;
   CToken            token;
   //::ca::graphics * pdcForeground = GetWndRender()->TwiGetDC();

   ASSERT(tokenIndex >= 0);
   ASSERT(ptokena != NULL);
   ASSERT(pdcForeground != NULL);
   //   ASSERT(m_iStatus != XFPLYR_LINE_STATUS_NEW);
   //   ASSERT(m_iStatus != XFPLYR_LINE_STATUS_PLAYING);
//   ASSERT(ptwi != NULL);
//   m_ptwi = ptwi;
   if(tokenIndex >= ptokena->get_size())
   {
      //m_iStatus = XFPLYR_LINE_STATUS_NEW;
      return XFPLYR_LINE_SUCCESS_NO_LINE;
   }

   m_bVisible = false;
   iChars = -1;
   iCharsFirst = 0;
   iStr = 0;
   iTokens = -1;
   xPosition = 0;
   m_iFirstToken = tokenIndex;
   m_iIndent = 0;
   m_iBBCount = 0;
   m_tokenaRaw.remove_all();
   m_tokenaMain.remove_all();
   m_tokenaJp1.remove_all();



   //m_strRaw.Empty();
   //m_iStatus = XFPLYR_LINE_STATUS_NEW; // default
   //rectPlacement.left = 0;

   eresult = XFPLYR_LINE_NULL;
   for(; tokenIndex < ptokena->get_size(); tokenIndex++)
   {
      string strSrc = ptokena->element_at(tokenIndex);
      //gen::international::multibyte_to_utf8(dwCP, str,  strSrc);
      str = strSrc;
      iStrLen = str.get_length();
      const char * pszAlloc = _strdup(str);
      const char * psz = pszAlloc;
      for(;; psz = gen::str::utf8_inc(psz))
      {
         strChar = gen::str::utf8_char(psz);
         strNextChar = gen::str::utf8_next_char(str, psz);
         if(iTokens == -1)
         {
            if(strChar == "\\")
            {
               forceInsertion = TRUE;
            }
            else if(strChar == "^")
            {
               token.AddChar(" ");
            }
            else if(strChar == "/")
            {
               // This is the first token of the
               // line (iTokens == -1).
               // If the first token has other
               // characters (iStrLen != 1), then
               // the line must be wiped.
               // Otherwise, this token indicates that
               // it is the first token of this new line.
               if(MaybeURL(str))
               {
                  token.AddChar(strChar);
               }
               else if(iStrLen != 1)
               {
                  m_tokenaRaw.add(token);
                  token.clear();
                  eresult = XFPLYR_LINE_SUCCESS;
                  break;
               }
            }
            else if(strChar == "%")
            {
               if(flags & XFPLYR_BREAK_LINE_FEED)
               {
                  m_tokenaRaw.add(token);
                  token.clear();
                  eresult = XFPLYR_LINE_SUCCESS;
                  break;
               }
            }
            else if(strChar == "<")
            {
               if(!IsFirstLine() && (flags & LyricViewLine::XFPLYR_LINE_PAGE_BREAK))
               {
                  eresult = XFPLYR_LINE_PAGE_BREAK;
                  break;
               }
            }
            else if(strChar == ">")
            {
               if(iStr == 0)
                  m_iIndent = pdcForeground->GetDeviceCaps(LOGPIXELSX) / 2;
            }
            else if(strChar.is_empty())
            {
               break;
            }
            else
            {
               token.AddChar(strChar);
            }
         }
         else
         {
            if(forceInsertion)
            {
               forceInsertion = FALSE;
               token.AddChar(strChar);
            }
            else if(strChar == "\\")
            {
               forceInsertion = TRUE;
            }
            else if(strChar == "^")
            {
               token.AddChar(" ");
            }
            else if(strChar == "/")
            {
               if(MaybeURL(str))
               {
                  token.AddChar(strChar);
               }
               else
               {
                  //            if(iStrLen != 1)
                  //            {
                  m_tokenaRaw.add(token);
                  token.clear();
                  //            }
                  eresult = XFPLYR_LINE_SUCCESS;
                  break;
               }
            }
            else if(strChar == "%")
            {
               if(flags & XFPLYR_BREAK_LINE_FEED)
               {
                  m_tokenaRaw.add(token);
                  token.clear();
                  eresult = XFPLYR_LINE_SUCCESS;
                  break;
               }
            }
            else if(strChar == "<")
            {
               if(iStr == 0)
               {
                  eresult = XFPLYR_LINE_SUCCESS;
                  break;
               }
            }
            else if(strChar == ">")
            {
               //m_xPos = GetDeviceCaps(hDC, LOGPIXELSX) / 2;
            }
            else if(strChar == "\n")
            {
               m_tokenaRaw.add(token);
               token.clear();
               eresult = XFPLYR_LINE_SUCCESS;
               break;
            }
            else if(strChar == "\r")
            {
               m_tokenaRaw.add(token);
               token.clear();
               eresult = XFPLYR_LINE_SUCCESS;
               break;
            }
            else
            {
               token.AddChar(strChar);
            }
         }
         if(*psz == '\0')
            break;
      }
      free((void *) pszAlloc);
      if(eresult != XFPLYR_LINE_NULL)
         break;
      iTokens++;
      m_tokenaRaw.add(token);
      token.clear();
      if(iChars >= iCharsFirst)
         iCharsFirst = iChars + 1;
      iStr = 0;
      continue;
   }

   if(tokenIndex >= ptokena->get_size())
   {
      if(m_tokenaRaw.get_size() > 0)
      {
         eresult = XFPLYR_LINE_SUCCESS_LAST_LINE;
      }
      else
      {
         eresult = XFPLYR_LINE_SUCCESS_NO_LINE;
      }
   }


   SetTokenCount(m_tokenaRaw.get_size());
   //m_charsLength = iChars + 1;
   *nextTokenIndex = GetLastToken() + 1;
   m_tokenaRaw.Prepare();
   CalcMainTokenSet(dwCP);
   CalcMainLayout(pdcForeground, pfont);
   CalcNotesPositions(pEvents);

//   string strLanguage;
//   strLanguage = m_ptemplate->GetTemplates()->m_wstrLanguage;
   if(dwCP == 932)
   {
      m_tokenaJp1.copy(m_tokenaRaw);
      for(int i = 0; i < m_tokenaJp1.get_size(); i++)
      {
         m_tokenaJp1[i].RawToJp1();
      }
      m_tokenaJp1.Prepare();
      CalcJp1Layout(pdcForeground, pfont);
   }

   ASSERT(*nextTokenIndex >= 0);
   m_iMiddle = -1;
   m_iMiddle = 0;
   m_iLastMiddle = 0;

   if(!m_bVisible)
   {
      if(m_ponvisible != NULL)
      {
         delete m_ponvisible;
         m_ponvisible = NULL;
      }
   }

   return eresult;

}

LyricViewLine::ERenderResult LyricViewLine::to(
   ::ca::graphics *            pdc,
   ikar::data &        karaokedata,
   bool                       bUpdate)
{
   UNREFERENCED_PARAMETER(bUpdate);
   //TRACE("CLrcViewLine::to ");
   kar::KaraokeView *       pview = GetKaraokeView();
   int iEmbossWidth = (int) pview->m_dLyricFontEmbossWidth;
   visual::font *         pfont = pview->GetLyricFont();
   rect rectTextOut;
   rect rectText;
   rect rectSel;
   rect rectChar;
   rect rect;
   int iLToken;
   int iUToken;
   int iMToken;
   int iToken;
   int iLeft, iTop, iRight, iBottom;
   string strFinal;
   string strMid;
   int iMidStart;
   int iMidLen;
   rect_array rectaModified;

//   int iPlayingTokenIndex = m_iFillRateToken;
//   int iPlayingNoteIndex = m_iFillRateNote;

   LyricEventsV2 *      pevent = karaokedata.GetStaticData().GetEvents();


   // get placement rectangle
   class rect rectPlacement;
   GetPlacement(rectPlacement);

   ikar::dynamic_data & dynamicdata = karaokedata.GetDynamicData();

   int iPlayingTokenIndex = dynamicdata.m_iPlayingTokenIndex;

   bool bRenderEnable = m_pContainer->IsRenderEnabled();

   size sizeMargin;
   m_ptemplate->GetMargin(sizeMargin);

   bool bSSEmp = m_pContainer->m_pkaraokeview->m_bSuperScriptEmphasis;

   sizeMargin.cx = (long) (10.0 * m_floatRateX);
   sizeMargin.cy = (long) (10.0 * m_floatRateX);

   ::ca::pen * ppenLeft, * ppenRight;
   ::ca::pen * ppenLeftSmall, * ppenRightSmall;
   ::ca::pen * ppenLinkLeft, * ppenLinkRight;
   ::ca::pen * ppenLinkHoverLeft, * ppenLinkHoverRight;
   m_ptemplate->GetPens(&ppenLeft, &ppenRight, &ppenLeftSmall, &ppenRightSmall);
   m_ptemplate->GetLinkPens(ppenLinkLeft, ppenLinkRight, ppenLinkHoverLeft, ppenLinkHoverRight);

   COLORREF cr, crOutline;
   COLORREF crLeft, crRight;
   COLORREF crLeftOutline, crRightOutline;
   COLORREF crLinkLeft, crLinkRight;
   COLORREF crLinkLeftOutline, crLinkRightOutline;
   COLORREF crLinkHoverLeft, crLinkHoverRight;
   COLORREF crLinkHoverLeftOutline, crLinkHoverRightOutline;
   m_ptemplate->GetColors(crLeft, crRight, crLeftOutline, crRightOutline);
   m_ptemplate->GetLinkColors(
      crLinkLeft, crLinkRight,
      crLinkLeftOutline, crLinkRightOutline,
      crLinkHoverLeft, crLinkHoverRight,
      crLinkHoverLeftOutline, crLinkHoverRightOutline);

   if(pdc != NULL)
   {
      pdc->SetBkMode(TRANSPARENT);
   }

//   TEXTMETRIC & textMetric = pfont->m_tm;
   if(!IsVisible())
   {
      rect = rectPlacement;

      if(bRenderEnable)
      {
         Validate();
      }

      return XFPLYR_LINE_SUCCESS;
   }

   iLToken = 0;
   iUToken = GetTokenCount() - 1;

   if(m_tokenaRaw.get_size() <= 0)
      return XFPLYR_LINE_ERROR_OUT_OF_BOUND;

   iTop      = rectPlacement.top;
   iBottom   = rectPlacement.bottom;
   if(pdc != NULL)
   {
      pdc->SetTextAlign(TA_LEFT | TA_TOP);
   }

   if(m_bPendingLayoutUpdate || m_ponvisible == NULL)
   {
      CalcMainLayout(
         pdc,
         pfont);
      CalcJp1Layout(
         pdc,
         pfont);
      m_bPendingLayoutUpdate = false;
   }

   if(bRenderEnable)
   {
      string wstrMain;
      GetMainString(wstrMain);
      CacheEmboss(pdc, wstrMain, wstrMain.get_length());
   }
   int iT = iTop + m_ptTextOffset.y + m_ptemplate->m_iJp1Provision;
   int iRadius = max(1, (int) (m_floatRateX * 5.0));
   ::ca::rgn_sp rgnClip(get_app());
   {
      class rect rectClip;
      //rectClip = rectPlacement;
      m_pContainer->m_pkaraokeview->GetClientRect(rectClip);
      //if(!IsFirstLine())
      {
        // rectClip.offset(m_ptTextOffset);
      }
      //rectClip.offset(pdc->GetViewportOrg());
      rgnClip->CreateRectRgnIndirect(rectClip);
      //pdc->SelectClipRgn(rgnClip);
   }
   keeper < ::ca::dib * > keepBBAlphaBlend(&pdc->m_pdibAlphaBlend, (System.savings().should_save(gen::resource_processing)
      || (m_bEnhancedEmboss && System.savings().should_save(gen::resource_blurred_text_embossing)))
       ? NULL :  GetTemplate()->get_bb_alpha_blend(), NULL, FALSE);
   if(IsFirstLine() && GetContainer()->m_pkaraokeview->IsKaraokeBouncingBall())
   {
      keepBBAlphaBlend.Keep();
      pdc->m_ptAlphaBlend = rectPlacement.top_left();
      pdc->m_ptAlphaBlend.y -= rectPlacement.height();
   }
   double dStandUp;
   int iStandUpHeight =  rectPlacement.height() / 3;
   if(m_dwNoteMsLength < 284) // || (m_dwNoteMsLength > 484 && iPlayingTokenIndex == (m_iFirstToken + m_iTokenCount - 1)))
   {
      if(m_dNoteFillRate > 0)
      {
         dStandUp = 1.0;
      }
      else
      {
         dStandUp = 1.0;
      }
      //dStandUp -= min(1.0, max(0.0, abs(((((double)m_ptTextOffset.y)) * ((double)m_ptTextOffset.y)) / ((double)rectPlacement.height()))));
   }
   else if(m_dwNoteMsLength > 484)
   {
      double dExpand = 0.40 * min(1.0, (double) (m_dwNoteMsLength - 484.0) / 884.0);
      if(m_dNoteFillRate <= (0.50 - dExpand))
      {
         dStandUp = sin((1.0 + m_dNoteFillRate * 5.0 / (0.50 - dExpand))  * 3.1415 / 12.0);
      }
      else if(m_dNoteFillRate >= (0.50 + dExpand))
      {
         dStandUp = sin((6.0 + 1.0 + (m_dNoteFillRate - (1.0 - (0.50 - dExpand))) * 5.0 / (0.50 - dExpand))  * 3.1415 / 12.0);
      }
      else
      {
         dStandUp = 1.0;
      }
   }
   else
   {
      dStandUp = sin((1.0 + m_dNoteFillRate * 5.0)  * 3.1415 / 6.0);
   }

   if(bSSEmp && (iPlayingTokenIndex >= m_iFirstToken) && (iPlayingTokenIndex < (m_iFirstToken + m_iTokenCount)))
   {
      dynamicdata.feed_standup(dStandUp);
   }
   if(m_ptemplate->IsEmbossed()
      && !(System.savings().should_save(gen::resource_processing)
      || (m_bEnhancedEmboss && System.savings().should_save(gen::resource_blurred_text_embossing))))
   {

      for(int iToken = 0; iToken < GetTokenCount(); iToken++)
      {
         CToken & token = m_tokenaMain[iToken];

         if(token.m_dibEmboss.is_null())
            continue;

         class rect rect = rectPlacement;
         rect.left   = - iRadius * 2;
         rect.top    = iT - iRadius * 2;
         rect.bottom = iT + rectPlacement.height() + iRadius * 2;
         rect.left   += token.get_position(0);
         rect.right  = rect.left + token.m_dibEmboss->width();

         if(bSSEmp && ((m_iFirstToken + iToken) < dynamicdata.m_daStandUp.get_size()))
         {
            rect.top       -= (LONG) (dynamicdata.m_daStandUp[m_iFirstToken + iToken] * iStandUpHeight);
            rect.bottom    -= (LONG) (dynamicdata.m_daStandUp[m_iFirstToken + iToken] * iStandUpHeight);
            System.imaging().true_blend(pdc, rect, token.m_dibEmboss->get_graphics(), null_point(), m_dibWork, m_dibWork2, m_dibWork3);
         }
         else
         {
            System.imaging().true_blend(pdc, rect, token.m_dibEmboss->get_graphics(), null_point(), m_dibWorkB, m_dibWorkB2, m_dibWorkB3);
         }
      }
   }
   //return XFPLYR_LINE_SUCCESS;
   int iOldMapMode = MM_TEXT;
   if(pdc != NULL)
   {
//      int iMapMode = pdc->GetMapMode();
   }

   if(m_floatRateX == 0.0)
      m_floatRateX = 1.0;

   ::ca::font * pfontOld = NULL;
//   VERIFY(hfontOld = (HFONT) ::SelectObject(pdc->m_hDC, m_hfont));
   if(pdc != NULL && m_ponvisible != NULL)
   {
      pfontOld = pdc->SelectObject(m_ponvisible->m_font);
   }

   int iFirstChar;
   int charsLength;
   string str;
   int iTrimLength;

   int iLastMiddle = m_iLastMiddle;
   int iMiddle = m_iMiddle;

   iMiddle = GetMiddle(pevent);
   if(bRenderEnable)
   {
      m_iLastMiddle = iLastMiddle;
      //ASSERT(iPlayingNoteIndex <= 18 || iMiddle >= m_iMiddle);
      m_iMiddle = iMiddle;
   }

   int iCharIndex;
   int iLength;

   CTokenJp1 * ptokenjp1 = NULL;

   GetSelRect(rectSel);

   int iMaxChar;
   int iChar;
   enum ETool
   {
      ToolNone,
      ToolHoverLeft,
      ToolLinkLeft,
      ToolLeft,
      ToolHoverRight,
      ToolLinkRight,
      ToolRight,
   };

   ETool etool = ToolNone;
   ETool etoolSelect = ToolNone;


   iMToken = iPlayingTokenIndex - GetFirstToken();
   iMToken = min(iMToken, iUToken);
   int iStandUpOffset;
   for(iToken = iLToken; iToken <= iMToken; iToken++)
   {

      if(bSSEmp && ((m_iFirstToken + iToken) < dynamicdata.m_daStandUp.get_size()))
      {
         iStandUpOffset = (int) (dynamicdata.m_daStandUp[m_iFirstToken + iToken] * iStandUpHeight);
      }
      else
      {
         iStandUpOffset = 0;
      }

      {
         pdc->SelectClipRgn(NULL);
         rgnClip->delete_object();
         class rect rectClip;
         rectClip = rectPlacement;
         //if(!IsFirstLine())
         {
            rectClip.offset(m_ptTextOffset);
         }
         rectClip.offset(0, -iStandUpOffset);
         rectClip.right = iMiddle + 2;
         rectClip.offset(pdc->GetViewportOrg());
         rgnClip->CreateRectRgnIndirect(rectClip);
         pdc->SelectClipRgn(rgnClip);
      }

      CToken & token = m_tokenaMain[iToken];
      token.GetText(str);
      iFirstChar = 0;
      charsLength = str.get_length();

      str.trim_right();
      iTrimLength = str.get_length();
      if(iTrimLength == 0)
         continue;

      iCharIndex   = iFirstChar;
      iLength      = iTrimLength;
      iMaxChar    = iFirstChar + iTrimLength - 1;

      iChar       = iMaxChar;
      while(iChar > 0 && token.get_position(iChar - 1) == token.get_position(iChar))
      {
         iChar--;
      }
      string strChar;
      strChar = gen::str::utf8_char(&((const char *) str)[iChar]);
      iChar = min(iChar + strChar.get_length(), iMaxChar);

      if(iChar < iCharIndex)
         continue;

      iLength  = iChar - iCharIndex + 1;
      iLeft    = token.get_position(iCharIndex);
      iRight   = token.get_position(iCharIndex + iLength);

      strFinal = str.Mid(iCharIndex, iLength);

      int iLeftOut         = iLeft;
      int iMiddleOut       = iMiddle;
//      int iLastMiddleOut   = iLastMiddle;
//      int iRightOut        = iRight;

      if(iToken + m_iFirstToken == iPlayingTokenIndex)
      {
         if(iMiddleOut < iLeftOut)
            continue;
      }


      ptokenjp1 = NULL;
      if(m_tokenaJp1.get_size() > 0)
      {
         ptokenjp1 = &m_tokenaJp1[iToken];
      }

      rectText = rectPlacement;

      rectText.offset(m_ptTextOffset);
      if(bRenderEnable)
      {

         int iL = iLeftOut + m_ptTextOffset.x ;
//         int iR = iRight + m_ptTextOffset.x ;

         rectTextOut.left     = iL;
         rectTextOut.top      = iT;
         rectTextOut.right    = iL + rectText.width();
         rectTextOut.bottom   = iT + rectText.height();

         if(m_ponvisible != NULL)
         {
            pdc->SelectObject(m_ponvisible->m_font);
         }

         iMidStart = 0;
         for(int iCh = 0; iCh < iLength; iCh++)
         {
            if(HasLink(GetFirstToken() + iToken, iCh + iCharIndex))
            {
               if(GetFirstToken() + iToken == m_iLinkHoverToken
                  && GetLinkIndex(GetFirstToken() + iToken, iCh + iCharIndex) == m_iLinkHoverIndex)
               {
                  etool = ToolHoverLeft;
               }
               else
               {
                  etool = ToolLinkLeft;
               }
            }
            else
            {
               etool = ToolLeft;
            }
            if(etool != etoolSelect && iCh > 0)
            {
               iCh--;
               goto printLeft;
            }
            etoolSelect = etool;
            if(iCh < iLength - 1)
            {
               continue;
            }
printLeft:
            rectChar = rectTextOut;
            rectChar.left += token.get_position(iMidStart + iCharIndex) - token.get_position(iCharIndex);
            iMidLen = iCh - iMidStart + 1;
            if(iCh < 0)
            {
               if(iLength == 1)
               {
                  etoolSelect = etool;
               }
               else
                  continue;
            }
            switch(etoolSelect)
            {
            case ToolHoverLeft:
               pdc->SelectObject(ppenLinkHoverLeft);
               pdc->SelectObject(m_ponvisible->m_fontLink);
               pdc->SetTextColor(crLinkHoverLeft);
               cr = crLinkHoverLeft;
               crOutline = crLinkHoverLeftOutline;
               break;
            case ToolLinkLeft:
               pdc->SelectObject(ppenLinkLeft);
               pdc->SelectObject(m_ponvisible->m_fontLink);
               pdc->SetTextColor(crLinkLeft);
               cr = crLinkLeft;
               crOutline = crLinkLeftOutline;
               break;
            case ToolLeft:
               pdc->SelectObject(ppenLeft);
               pdc->SelectObject(m_ponvisible->m_font);
               pdc->SetTextColor(crLeft);
               cr = crLeft;
               crOutline = crLeftOutline;
               break;
            }
            etoolSelect = etool;
            if(m_ptemplate->IsEmbossed())
            {
               strMid = str.Mid(iMidStart + iCharIndex, iMidLen);
               EmbossedTextOut(
                  pdc,
                  NULL,
                  strMid,
                  rectChar.left,
                  rectChar.top - iStandUpOffset,
                  iEmbossWidth,
                  cr,
                  crOutline,
                  iMidLen,
                  iToken == iLToken,
                  iToken == iUToken);
            }
            else
            {
               pfont->SimpleTextOut(
                  pdc,
                  rectChar.left, iT,
                  strMid,
                  NULL,
                  iCharIndex);
            }
            iMidStart = iCh + 1;
         }
         if(ptokenjp1 != NULL)
         {
            string str;
            ptokenjp1->GetText(str);
            pdc->SelectObject(ppenLeftSmall);
            pdc->SetTextColor(crLeft);

            if(!str.is_empty())
            {
               VERIFY(pdc->SelectObject(m_ponvisible->m_fontJp1));
               class rect rectJp1;
               class rect rectBlend;
               rectJp1 = rectTextOut;
               rectJp1.top -= m_ptemplate->m_iJp1Provision + iStandUpOffset;
               rectJp1.bottom = rectJp1.top + m_ptemplate->m_iJp1Provision;
               rectBlend = rectJp1;
               size size = pdc->GetTextExtent(str);
               int iJpR = iL + size.cx;
               rectBlend.right = min(iJpR, iMiddle + 2);
               rectBlend.left = iL;

               System.imaging().color_blend(pdc, rectBlend, RGB(120, 180, 100), 96);
               visual::api::SimpleTextOut(
                  pdc,
                  rectJp1,
                  ptokenjp1->m_floatRateX,
                  rectTextOut.height(),
                  str,
                  NULL,
                  NULL,
                  0);

            }
         }
      }
   }


   iMToken = iPlayingTokenIndex - GetFirstToken();
   iMToken = max(0, iMToken);
   for(iToken = iMToken; iToken <= iUToken; iToken++)
   {

      if(bSSEmp && ((m_iFirstToken + iToken) < dynamicdata.m_daStandUp.get_size()))
      {
         iStandUpOffset = (int) (dynamicdata.m_daStandUp[m_iFirstToken + iToken] * iStandUpHeight);
      }
      else
      {
         iStandUpOffset = 0;
      }

      {
         pdc->SelectClipRgn(NULL);
         rgnClip->delete_object();
         class rect rectClip;
         rectClip = rectPlacement;
         //if(!IsFirstLine())
         {
            rectClip.offset(m_ptTextOffset);
         }
         rectClip.offset(0, -iStandUpOffset);
         rectClip.left = iMiddle + 2;
         rectClip.offset(pdc->GetViewportOrg());
         rgnClip->CreateRectRgnIndirect(rectClip);
         pdc->SelectClipRgn(rgnClip);
      }

      CToken & token = m_tokenaMain[iToken];
      token.GetText(str);
      iFirstChar = 0;
      charsLength = str.get_length();

      str.trim_right();
      iTrimLength = str.get_length();
      if(iTrimLength == 0)
         continue;

      iCharIndex   = iFirstChar;
      iLength      = iTrimLength;
      iMaxChar    = iFirstChar + iTrimLength - 1;

      iChar       = iMaxChar;
      while(iChar > 0 && token.get_position(iChar - 1) == token.get_position(iChar))
      {
         iChar--;
      }
      string strChar;
      strChar = gen::str::utf8_char(&((const char *) str)[iChar]);
      iChar = min(iChar + strChar.get_length(), iMaxChar);

      if(iChar < iCharIndex)
         continue;

      iLength = iChar - iCharIndex + 1;
      iLeft = token.get_position(iCharIndex);
      iRight = token.get_position(iCharIndex + iLength);

      string strFinal;
      strFinal = str.Mid(iCharIndex, iLength);

      int iLeftOut         = iLeft;
//      int iMiddleOut       = iMiddle;
//      int iLastMiddleOut   = iLastMiddle;
//      int iRightOut        = iRight;

       ptokenjp1 = NULL;
      if(m_tokenaJp1.get_size() > 0)
      {
         ptokenjp1 = &m_tokenaJp1[iToken];
      }
      //About to play

      rectText = rectPlacement;
      rectText.offset(m_ptTextOffset);

      if(bRenderEnable)
      {
          pdc->SelectObject(m_ponvisible->m_font);


         int iL = iLeftOut + m_ptTextOffset.x ;
//         int iR = iRight + m_ptTextOffset.x ;


         int iT = iTop + m_ptTextOffset.y+ m_ptemplate->m_iJp1Provision;

         rectTextOut.left = iL;
         rectTextOut.top = iT;
         rectTextOut.right = iL + rectText.width();
         rectTextOut.bottom = iT + rectText.height();

         iMidStart = 0;
         for(int iCh = 0; iCh < iLength; iCh++)
         {
            if(HasLink(GetFirstToken() + iToken, iCh + iCharIndex))
            {
               if(GetFirstToken() + iToken == m_iLinkHoverToken
                  && GetLinkIndex(GetFirstToken() + iToken, iCh + iCharIndex) == m_iLinkHoverIndex)
               {
                  etool = ToolHoverRight;
               }
               else
               {
                  etool = ToolLinkRight;
               }
            }
            else
            {
               etool = ToolRight;
            }
            if(etool != etoolSelect && iCh > 0)
            {
               iCh--;
               goto printRight;
            }
            etoolSelect = etool;
            if(iCh < iLength - 1)
            {
               continue;
            }
printRight:
            rectChar = rectTextOut;
            rectChar.left += token.get_position(iMidStart + iCharIndex) - token.get_position(iCharIndex);
            iMidLen = iCh - iMidStart + 1;
            if(iCh < 0)
            {
               if(iLength == 1)
               {
                  etoolSelect = etool;
               }
               else
                  continue;
            }
            switch(etoolSelect)
            {
            case ToolHoverRight:
               pdc->SelectObject(ppenLinkHoverRight);
               pdc->SelectObject(m_ponvisible->m_fontLink);
               pdc->SetTextColor(crLinkHoverRight);
               cr = crLinkHoverRight;
               crOutline = crLinkHoverRightOutline;
               break;
            case ToolLinkRight:
               pdc->SelectObject(ppenLinkRight);
               pdc->SelectObject(m_ponvisible->m_fontLink);
               pdc->SetTextColor(crLinkRight);
               cr = crLinkRight;
               crOutline = crLinkRightOutline;
               break;
            case ToolRight:
               pdc->SelectObject(ppenRight);
               pdc->SelectObject(m_ponvisible->m_font);
               pdc->SetTextColor(crRight);
               cr = crRight;
               crOutline = crRightOutline;
               break;
            }
            etoolSelect = etool;
            strMid = str.Mid(iMidStart + iCharIndex, iMidLen);
            if(m_ptemplate->IsEmbossed())
            {
               EmbossedTextOut(
                  pdc,
                  NULL,
                  strMid,
                  rectChar.left,
                  rectChar.top - iStandUpOffset,
                  iEmbossWidth,
                  cr,
                  crOutline,
                  iMidLen,
                  iToken == iLToken,
                  iToken == iUToken);
            }
            else
            {
               pfont->SimpleTextOut(
                  pdc,
                  rectChar.left, iT,
                  strMid,
                  NULL,
                  iCharIndex);
            }
            iMidStart = iCh + 1;
         }
         if(ptokenjp1 != NULL)
         {
            string str;
            ptokenjp1->GetText(str);
            if(!str.is_empty())
            {
               VERIFY(pdc->SelectObject(m_ponvisible->m_fontJp1));
               class rect rectJp1;
               class rect rectBlend;
               rectJp1 = rectTextOut;
               rectJp1.top -= m_ptemplate->m_iJp1Provision + iStandUpOffset;
               rectJp1.bottom = rectJp1.top + m_ptemplate->m_iJp1Provision;
               rectBlend = rectJp1;
               rectBlend.left = max(iL, iMiddle + 2);

               size size = pdc->GetTextExtent(str);

               rectBlend.right = iL + size.cx;


               System.imaging().color_blend(pdc, rectBlend, RGB(200, 200, 180), 96);
               visual::api::SimpleTextOut(
                  pdc,
                  rectJp1,
                  ptokenjp1->m_floatRateX,
                  rectTextOut.height(),
                  str,
                  NULL,
                  NULL,
                  0);

            }

         }

      }
   }

   {
      pdc->SelectClipRgn(NULL);
      rgnClip->delete_object();
   }

   // to Selection

   if(pdc != NULL)
   {
      pdc->SetMapMode(iOldMapMode);
      if(pfontOld != NULL)
      {
         pdc->SelectObject(pfontOld);
      }
   }

   if(bRenderEnable)
   {
      Validate();
   }

   class rect rectBox;

   rectaModified.get_box(rectBox);

   //TRACE(" End\n");
   return XFPLYR_LINE_SUCCESS;
}




void LyricViewLine::Reset()
{
   m_tokenaRaw.remove_all();
   m_tokenaMain.remove_all();
   m_tokenaJp1.remove_all();
}

void LyricViewLine::UpdateFillRate(double msElapsed, double msLength, int iOffset, int iCount)
{
   UNREFERENCED_PARAMETER(msElapsed);
   UNREFERENCED_PARAMETER(msLength);
   UNREFERENCED_PARAMETER(iOffset);
   UNREFERENCED_PARAMETER(iCount);
   // initial filling minimum time and initial filling rate
   // is applied to facilitate karaoke text reading. When
   // the filling event is slow, initial filling rate may be
   // so small as zero rate. But to give a hint, a minimum
   // fill rate is applied. When the filling event is faster,
   // the initial filling rate must be greater so the hint
   // is more obvious..

   // minimum time where maximim fill rate is applied
   // msInitialFillingMinimumTime, msInitialFillintMinimumRate
   /*const double x0 = 70.0, y1 = 0.7;

   // maximumm time where minimum fill rate is applied
   // msInitialFillingMaximumTime, msInitialFillingMaximumTime
   const double x1 = 5000.0, y0 = 0.35;

   const double zr = 0.03;
   // calcula o initial rate com base numa curva
   // ir = (y1-y0)(db^(-length+x0))+y0
   // onde a base db ・calculada para ter somente zr
   // da diferen軋 (y1 - y0) para atingir
   // o patamar y0, depois que o tempo passa x1 segundos.
   // para isso db deve ser calculado como se segue.

   double db = exp(log(zr) / (x0 - x1));
   double ir = (y1 - y0) *(pow(db, x0 - msLength)) + y0;
   double dFillRate = ir + ((1.0 - ir) * msElapsed / msLength);*/


}

void LyricViewLine::CalcMainLayout(
   ::ca::graphics *  pgdi,
   visual::font *       pfont)
{
   UNREFERENCED_PARAMETER(pgdi);
   m_bCacheEmboss = false;
   rect rectClient;
   m_pContainer->m_pkaraokeview->GetClientRect(rectClient);
   pfont = GetKaraokeView()->GetLyricFont();
   ::ca::graphics_sp spgraphics(get_app());
   spgraphics->CreateCompatibleDC(NULL);
   ::ca::graphics * pdc = spgraphics;
   //::ca::graphics * pdc = (::ca::graphics *) pgdi->GetGdi();
   //LyricViewLineTemplate * lptemplate = GetTemplate();
   ASSERT(pfont != NULL);
   rect rectPlacement;
   m_ptemplate->GetPlacement_(rectPlacement);
   if(m_tokenaRaw.get_size() <= 0)
      return;
   string strMain;
   GetMainString(strMain);
   const char * pszMain = strMain;
   int iMaxExtent, iLeft;
   size size;
   ::ca::font * pfontOld = pdc->SelectObject(pfont->GetFont());
   size = pdc->GetTextExtent(strMain);
   pdc->SelectObject(pfont->GetFont());
   if(size.cx > rectClient.width())
   {
      m_floatRateX =
         (float)
         rectClient.width()/
         (size.cx + 10);
   }
   else
   {
      m_floatRateX = 1.0;
   }
   if(m_ponvisible != NULL)
   {
      delete m_ponvisible;
      m_ponvisible = NULL;
   }
   m_ponvisible = new OnVisible(get_app());
   LOGFONT lf, lfInt, lfExt;
   pfont->GetFont()->GetLogFont(&lf);
   pdc->SelectObject(pfont->GetFont());
   TEXTMETRIC tm;
   pdc->GetTextMetrics(&tm);
   lf.lfHeight = (long) (lf.lfHeight * 0.9);
   lf.lfWidth = (long) (tm.tmAveCharWidth * m_floatRateX - 1.0);
   lfInt = lf;
   lfExt = lf;
   lfInt.lfHeight    = (long) (lfInt.lfHeight   * 0.8);
   lfInt.lfWidth     = (long) (lfInt.lfWidth    * 0.8);
   lfExt.lfHeight    = (long) (lfExt.lfHeight   * 1.2);
   lfExt.lfWidth     = (long) (lfExt.lfWidth    * 1.2);
   m_ponvisible->m_font->CreateFontIndirect(&lf);
   m_ponvisible->m_fontInt->CreateFontIndirect(&lfInt);
   m_ponvisible->m_fontExt->CreateFontIndirect(&lfExt);





   if(HasLink())
   {
      lf.lfUnderline = TRUE;
      m_ponvisible->m_fontLink->CreateFontIndirect(&lf);
   }

   pdc->SelectObject(m_ponvisible->m_font);
   if(m_iAlign == AlignLeft)
   {
      SetMainPosition(0, rectPlacement.left + m_iIndent);
      for(const char * psz = gen::str::utf8_inc(pszMain);
         ;
         psz = gen::str::utf8_inc(psz))
      {
         int i = (const char *) psz - (const char *) strMain;
         size = pdc->GetTextExtent(strMain, i);
         SetMainPosition(i, size.cx + rectPlacement.left  + m_iIndent);
         if(*psz == '\0')
            break;
      }
      if(m_bAutoSizeX)
      {
         rectPlacement.right = rectPlacement.left  + m_iIndent + size.cx;
      }
      if(m_bAutoSizeY)
      {
         rectPlacement.bottom = rectPlacement.top + size.cy;
      }
   }
   else if(m_iAlign == AlignRight)
   {
      size = pdc->GetTextExtent(strMain);
      iMaxExtent = size.cx;
      iLeft = rectPlacement.right - iMaxExtent;
      SetMainPosition(0, iLeft);
      for(const char * psz = gen::str::utf8_inc(pszMain);
         ;
         psz = gen::str::utf8_inc(psz))
      {
         int i = (const char *) psz - (const char *) pszMain;
         size = pdc->GetTextExtent(strMain, i);
         SetMainPosition(i, size.cx + iLeft);
         if(*psz == '\0')
            break;
      }
      if(m_bAutoSizeX)
      {
         rectPlacement.left = rectPlacement.right - size.cx;
      }
      if(m_bAutoSizeY)
      {
         rectPlacement.bottom = rectPlacement.top + size.cy;
      }
   }
   //CacheEmboss(pdc, strMain, strMain.get_length(), m_dibMain->get_graphics(), m_dibMain->get_bitmap());
   pdc->SelectObject(pfontOld);

   return;

}


// CalcMainLayout must be called before this function

void LyricViewLine::CalcJp1Layout(
   ::ca::graphics *  pgdi,
   visual::font *       pfont
   )
{
   UNREFERENCED_PARAMETER(pgdi);
   ::ca::graphics_sp spgraphics(get_app());
   spgraphics->CreateCompatibleDC(NULL);
   ::ca::graphics * pdc = (::ca::graphics *) spgraphics;
   size size;

   int iJp1Provision = m_ptemplate->m_iJp1Provision;
   for(int i = 0; i < m_tokenaJp1.get_size(); i++)
   {
      CTokenJp1 & token = m_tokenaJp1[i];
      string str;
      token.GetText(str);
      if(!str.is_empty())
      {
         CToken & tokenMain = m_tokenaMain[i];
         ::ca::font *pfontOld = pdc->SelectObject(pfont->GetFont());
//         pdc->SelectObject(pfont->GetFont());
//         token.m_font.delete_object();
         m_ponvisible->m_fontJp1->delete_object();
         LOGFONT lf;
         pfont->GetFont()->GetLogFont(&lf);
         TEXTMETRIC tm;
         pdc->GetTextMetrics(&tm);
         lf.lfWidth = tm.tmAveCharWidth * iJp1Provision / lf.lfHeight;
         lf.lfHeight = iJp1Provision;

         VERIFY(m_ponvisible->m_fontJp1->CreateFontIndirect(&lf));
         VERIFY(pdc->SelectObject(m_ponvisible->m_fontJp1));
         pdc->GetTextMetrics(&tm);
         size = pdc->GetTextExtent(str);
         if(size.cx > tokenMain.width())
         {
            token.m_floatRateX =
               (float)
               tokenMain.width()/
               size.cx;
         }
         else
         {
            token.m_floatRateX =  1.0;
         }
         pdc->SelectObject(pfontOld);
         lf.lfWidth = (long) (tm.tmAveCharWidth * token.m_floatRateX) - 1;
         //lf.lfHeight = lf.lfHeight * 0.5;
         m_ponvisible->m_fontJp1->delete_object();
         VERIFY(m_ponvisible->m_fontJp1->CreateFontIndirect(&lf));
         VERIFY(pdc->SelectObject(m_ponvisible->m_fontJp1));
         token.SetPosition(0, tokenMain.Left());
         const char * psz = str;
         for(const char * pszInc = gen::str::utf8_inc(psz);
            ;
            pszInc = gen::str::utf8_inc(pszInc))
         {
            int j = (const char *) pszInc - (const char *) psz;
            size = pdc->GetTextExtent(str, j);
            token.SetPosition(
               j,
               tokenMain.Left() + size.cx);
            if(*pszInc == '\0')
               break;
         }
         VERIFY(pdc->SelectObject(pfontOld));
         //CTokenJp1 * ptokenjp1 = &m_tokenaJp1[i];
      }
   }

   return;
}


void LyricViewLine::CalcNotesPositions(LyricEventsV2 * pEvents)
{
   UNREFERENCED_PARAMETER(pEvents);
   //int i, iStartNote, iNotesLength;

   //for(i = 0; i < m_iTokensLength; i++)
   //{
   //   pEvents->GetTokenNotes(
   //      &iStartNote,
   //      &iNotesLength,
   //      m_iFirstToken + i);
   //   m_tokensFirstNoteIndexes[i] = iStartNote;
   //   m_tokensNotesLengths[i] = iNotesLength;
   //}

}

//BOOL LyricViewLine::IsEmptyTimedOut()
//{
//   if(m_iStatus ==   XFPLYR_LINE_STATUS_EMPTY)
//   {
//      if(m_dwEmptyTime + 2000 < GetTickCount())
//         return TRUE;
//   }
//   return FALSE;
//
//}


bool LyricViewLine::IsNewTimedOut()
{
   //if(m_iStatus ==   XFPLYR_LINE_STATUS_NEW)
   //{
   if(m_bVisible
      && m_dwNewTime + 2000 < GetTickCount())
   {
      //      TRACE("LyricViewLine::IsNewTimedOut() = true m_iFirstToken = %d\n", m_iFirstToken);
      return true;
   }
   //}
   //    TRACE("LyricViewLine::IsNewTimedOut() = false m_iFirstToken = %d\n", m_iFirstToken);
   return false;

}

bool LyricViewLine::IsCleanTimedOut()
{
   //if(m_iStatus ==   XFPLYR_LINE_STATUS_NEW)
   //{
   if(!m_bVisible
      && m_dwCleanTime + 2000 < GetTickCount())
   {
      //          TRACE("LyricViewLine::IsCleanTimedOut() = true m_iFirstToken = %d\n", m_iFirstToken);
      return true;
   }
   //}
    //TRACE("LyricViewLine::IsCleanTimedOut() = false m_iFirstToken = %d\n", m_iFirstToken);
   return false;

}




                            void LyricViewLine::SetAutoSize(bool bAutoSize)
                            {
                               m_bAutoSizeX = bAutoSize;
                               m_bAutoSizeY = bAutoSize;
                            }

                            void LyricViewLine::SetAlign(int iAlign)
                            {
                               m_iAlign = iAlign;
                            }

                            /*void LyricViewLine::set(LPRECT lpRect)
                            {
                            rectPlacement.left     = lpRect->left;
                            rectPlacement.top      = lpRect->top;
                            rectPlacement.right    = lpRect->right;
                            rectPlacement.bottom   = lpRect->bottom;
                            }

                              void LyricViewLine::GetRect(LPRECT lpRect)
                              {
                              lpRect->left    = rectPlacement.left;
                              lpRect->top     = rectPlacement.top;
                              lpRect->right   = rectPlacement.right;
                              lpRect->bottom  = rectPlacement.bottom;
}*/

                            void LyricViewLine::SetPendingLayoutUpdate(bool bPending)
                            {
                               m_bPendingLayoutUpdate = bPending;
                            }


                            void LyricViewLine::SetNewTime()
                            {
                               m_dwNewTime = GetTickCount();
                            }

void LyricViewLine::Show(bool bVisible)
{
   //SetTemplate(m_ptemplate);
   if(bVisible && !m_bVisible)
   {
      m_iMiddle = 0;
      m_bVisible = true;
      m_bPendingLayoutUpdate = true;
      m_pContainer->OnChildSetVisible(*this);
      m_ptemplate->SetLine(this);
   }
   else if(!bVisible && m_bVisible)
   {
      //TRACE("LyricViewLine::Show(false) %d\n", GetLine());
      m_bVisible = false;
      m_dwCleanTime = GetTickCount();
      m_pContainer->OnChildSetVisible(*this);
      if(m_ponvisible != NULL)
      {
         delete m_ponvisible;
         m_ponvisible = NULL;
      }
   }
   Invalidate();
}


                            void LyricViewLine::OnTimerAnimate(::ca::rgn * pModifiedRgn)
                            {
                               UNREFERENCED_PARAMETER(pModifiedRgn);
                               ASSERT(FALSE);
                               rect rectPlacement;
                               m_ptemplate->GetPlacement_(rectPlacement);

                               switch(m_iAnimateType)
                               {
                               case AnimateNoAnimate:
                                  break;
                               case AnimateRHL:
                                  {
                                     m_dAnimateProgress+= m_dAnimateProgressIncrement;
                                     string wstrMain;
                                     GetMainString(wstrMain);
                                     if(wstrMain.get_length() > 0)
                                     {
                                        if((int)m_dAnimateProgress > max(GetMainPosition(wstrMain.get_length()) - GetMainPosition(0) + 100, rectPlacement.right - rectPlacement.left))
                                           m_dAnimateProgress = 0.0;
                                        rect rect;
                                        //                RenderLine(rectPlacement, pModifiedRgn, XFPLYR_LINE_RENDER_CLEAR_FULL, NULL, GetFonts(), false, m_crForeground, *m_lpPenEmboss);
                                     }
                                  }
                                  break;
                               default:
                                  ASSERT(FALSE);
                               }
                            }

                            void LyricViewLine::SetAnimateType(int iAnimateType)
                            {
                               m_iAnimateType = iAnimateType;
                               m_dAnimateProgress = 0.0;

                            }

                            void LyricViewLine::SetTextEffect(int iTextEffect)
                            {
                               m_iTextEffect = iTextEffect;
                            }

                            void LyricViewLine::SetEmbossPen(::ca::pen *lpPen)
                            {
                               m_lpPenEmboss = lpPen;
                            }

                            void LyricViewLine::SetForegroundColor(COLORREF cr)
                            {
                               m_crForeground = cr;
                            }

                            //primitive_array < visual::font *> * LyricViewLine::GetFonts()
                            //{
                            //  return &m_fonts;
                            //}

                            int LyricViewLine::MapToFontEffect(int iLineEffect)
                            {
                               switch(iLineEffect)
                               {
                               case EffectSimple:
                                  return visual::font::EffectSimple;
                               case EffectEmbossed:
                                  return visual::font::EffectEmbossed;
                               }
                               ASSERT(FALSE);
                               return -1;
                            }

                            void LyricViewLine::SetAnimateIncrement(double dIncrement)
                            {
                               m_dAnimateProgressIncrement = dIncrement;
                            }

                            //void LyricViewLine::SetRedrawMode(int iMode)
                            //{
                            //  m_iRedrawMode = iMode;
                            //}

                            //void LyricViewLine::SetRenderWindow(::ca::window *pWnd)
                            //{
                            //  m_pRenderWnd = pWnd;
                            //}

                            //void LyricViewLine::SetRenderCriticalSection(critical_section *pcs)
                            //{
                            //    m_pcsRender =   pcs;
                            //}


                            LyricViewLineTemplate * LyricViewLine::GetTemplate()
                            {
                               return m_ptemplate;
                            }

                            void LyricViewLine::SetTemplate(LyricViewLineTemplate *ptemplate)
                            {

                               m_ptemplate = ptemplate;
                               if(ptemplate != NULL)
                               {
                                 m_ptemplate->SetLine(this);
                               }

                               //    m_ptemplate->GetPlacement_(m_rectPlacement);

                            }

                            bool LyricViewLine::GetTokenX_(
                               int iTokenIndex,
                               double * pdLeft,
                               double * pdRight)
                            {
                               //   int iLeft;
                               //  int iRight;

                               ASSERT(iTokenIndex >= GetFirstToken());
                               ASSERT(iTokenIndex <= GetLastToken() + 1);

                               if(iTokenIndex == GetLastToken() + 1)
                               {
                                  *pdLeft   = 0.0;
                                  *pdRight = 0.0;
                                  return false;
                               }
                               else
                               {
                                  int iToken = iTokenIndex - m_iFirstToken;
                                  CToken & token = m_tokenaMain[iToken];
                                  string str;
                                  token.GetText(str);
                                  str.trim_right();
                                  *pdLeft      = token.get_position(0);
                                  *pdRight   = token.get_position(str.get_length());
                               }
                               if(*pdLeft == *pdRight)
                                  return false;
                               else
                                  return true;

                               //return ((double) iLeft + iRight) / 2.0;
                            }

bool LyricViewLine::GetTokenBBCenter(
   int iTokenIndex,
   double * pdLeft,
   double * pdRight)
{
   //   int iLeft;
   //  int iRight;

   ASSERT(iTokenIndex >= GetFirstToken());
   ASSERT(iTokenIndex <= GetLastToken() + 1);

   if(iTokenIndex == GetLastToken() + 1)
   {
      *pdLeft   = 0.0;
      *pdRight = 0.0;
   }
   else
   {
      string str;
      int iToken = iTokenIndex - m_iFirstToken;
      CToken & token = m_tokenaMain[iToken];
      token.GetText(str);
      int iFirstCharIndex = token.GetStartIndex();
      int iCharsLength = str.get_length();
      if(iCharsLength > 1)
      iCharsLength = 1;
      string wstrMain;
      GetMainString(wstrMain);
      while(iFirstCharIndex < m_iFirstToken + iCharsLength &&
      wstrMain[iFirstCharIndex] == ' ')
      iFirstCharIndex++;
      str = wstrMain.Mid(iFirstCharIndex, iCharsLength);
      //TRACE("%s****\n", str);
      str.trim_right();
      //TRACE("%s<<<<\n", str);
      int nTrimLength = str.get_length();
      *pdLeft   = GetMainPosition(iFirstCharIndex);
      *pdRight = GetMainPosition(iFirstCharIndex + nTrimLength);
   }
//return ((double) iLeft + iRight) / 2.0;
return true;
}



                            void LyricViewLine::SetPlacement(LPCRECT lpcrect,int iJp1Provision)
                            {
                               //m_rectPlacement = lprect;
                               m_ptemplate->SetPlacement_(lpcrect, iJp1Provision);
                            }


bool LyricViewLine::CalcMainTokenSet(DWORD dwCP)
{
   m_tokenaMain = m_tokenaRaw;
   for(int i = 0; i < m_tokenaMain.get_size(); i++)
   {
      m_tokenaMain[i].RawToMain(dwCP);
   }
   m_tokenaMain.Prepare();
   return true;
}

bool LyricViewLine::CToken::clear()
{
   m_str.Empty();
   m_iaPosition.set_size(1);
   m_iaPosition[0] = 0;
   return true;
}

bool LyricViewLine::CToken::AddChar(string strChar)
{
   m_str += strChar;
   for(int i = 0; i < strChar.get_length(); i++)
   {
      m_iaPosition.add(0);
   }
   return true;
}





void LyricViewLine::CToken::SetPosition(int iPosition)
{
   int i;
   for(i = 1; i < m_iaPosition.get_size(); i++)
   {
      m_iaPosition[i] -= m_iaPosition[0];
   }
   m_iaPosition[0] = iPosition;
   for(i = 1; i < m_iaPosition.get_size(); i++)
   {
      m_iaPosition[i] += iPosition;
   }
}

bool LyricViewLine::GetMainString(string & wstrMain)
{
   string str;
   wstrMain.Empty();
   for(int i = 0; i < m_tokenaMain.get_size(); i++)
   {
      m_tokenaMain[i].GetText(str);
      wstrMain += str;
   }
   return true;
}

bool LyricViewLine::SetMainPosition(int iChar, int iPosition)
{
   int iToken = GetMainTokenIndex(iChar);
   CToken & token = m_tokenaMain[iToken];
   int iTokenChar = iChar - token.GetStartIndex();
   if(iTokenChar == 0
      && iChar > 0
      && iToken > 0)
   {
      CToken & tokenPrevious = m_tokenaMain[iToken - 1];
      tokenPrevious.SetPosition(iChar - tokenPrevious.GetStartIndex(), iPosition);
   }
   token.SetPosition(iTokenChar, iPosition);
   while(true)
   {
      iToken++;
      if(iToken >= m_tokenaMain.get_size())
         break;
      CToken & tokenk = m_tokenaMain[iToken];
      if(tokenk.GetStartIndex() == token.GetStartIndex())
         tokenk.SetPosition(iTokenChar, iPosition);
      else
         break;
   }
   return true;
}

int LyricViewLine::GetMainTokenIndex(int iChar)
{
   int i;
   ASSERT(m_tokenaMain.get_size() > 0);
   for(i = 0; i < m_tokenaMain.get_size(); i++)
   {
      if(i + 1 < m_tokenaMain.get_size())
      {
         if(iChar < m_tokenaMain[i + 1].GetStartIndex())
            break;
      }
      else
         break;
   }
 int iTokenChar = iChar - m_tokenaMain[i].GetStartIndex();
 if(iTokenChar == 0)
 {
    while(i > 0)
    {
       if(m_tokenaMain[i].GetStartIndex()
          == m_tokenaMain[i - 1].GetStartIndex())
          i--;
       else
          break;
    }
 }
 return i;
}

LyricViewLine::CToken & LyricViewLine::GetCharTokenMain(int iChar)
{
   ASSERT(m_tokenaMain.get_size() > 0);
   return m_tokenaMain[GetMainTokenIndex(iChar)];
}

                            void LyricViewLine::CToken::SetStartIndex(int iIndex)
                            {
                               m_iStartIndex = iIndex;
                            }


                            int LyricViewLine::GetMainPosition(int iChar)
                            {
                               CToken & token = GetCharTokenMain(iChar);
                               return token.get_position(iChar - token.GetStartIndex());
                            }


                            bool LyricViewLine::CToken::RawToMain(DWORD dwCP)
                            {
                               if(dwCP == 932)
                               {
                                  string str;
                                  int iOpenBracket = 0;
                                  string strChar;
                                  for(const char * psz = m_str;; psz = gen::str::utf8_inc(psz))
                                  {
                                     strChar = gen::str::utf8_char(psz);
                                     if(strChar == "\x5b")
                                        iOpenBracket++;
                                     else if(strChar == "\x5")
                                        iOpenBracket--;
                                     else if(iOpenBracket <= 0)
                                        str += strChar;
                                     if(*psz == '\0')
                                        break;
                                  }
                                  m_str = str;
                                  m_iaPosition.set_size(m_str.get_length() + 1);
//                                  m_iaCharLink.set_size(m_wstr.get_length());
                               }
                               else
                               {
                               }
                               //int iIndex = 0;
//                               int iStart = 0;

                               return true;
                            }

                            bool LyricViewLine::CToken::RawToJp1()
                            {
                               string str;
                               int iOpenBracket = 0;
                               string strChar;
                               for(const char * psz = m_str; ; psz = gen::str::utf8_inc(psz))
                               {
                                  strChar = gen::str::utf8_char(psz);
                                  if(strChar == "\x5b")
                                     iOpenBracket++;
                                  else if(strChar == "\x5d")
                                     iOpenBracket--;
                                  else if(iOpenBracket > 0)
                                     str += strChar;
                                  if(*psz == '\0')
                                      break;
                               }
                               m_str = str;
                               m_iaPosition.set_size(m_str.get_length() + 1);
                               return true;
                            }


                            void LyricViewLine::CToken::SetPosition(int iChar, int iPosition)
                            {
                               if(iChar == 0)
                                  SetPosition(iPosition);
                               else
                               {
                                 ASSERT(iChar > 0);
                                 ASSERT(iChar <= m_iaPosition.get_size());
                                 string str;
                                 GetText(str);
                                 string strChar = gen::str::utf8_char(&((const char *)str)[iChar]);
                                 if(strChar.get_length() == 0)
                                 {
                                    m_iaPosition[iChar] = iPosition;
                                 }
                                 else
                                 {
                                    for(int i = 0; i < strChar.get_length(); i++)
                                    {
                                       m_iaPosition[iChar + i] = iPosition;
                                    }
                                 }
                              }
                            }




bool LyricViewLine::CTokenSet::Prepare()
{
   int i, j;
   int iLen = 0;
   string str;
   string strTemp;

   int iFirstToken = GetFirstToken();
   for(i = 0; i < this->get_size(); i++)
   {
      CToken & token = this->element_at(i);
      token.SetStartIndex(iLen);
      token.GetText(str);
      iLen += str.get_length();
      for(j = 0; j < str.get_length(); j++)
      {
         m_iaCharToken.add(iFirstToken + i);
         m_iaChar.add(j);
      }
   }



   return true;
}

void LyricViewLine::CTokenSet::GetText(string &wstrParam)
{
   string str;
   wstrParam.Empty();
   for(int i = 0; i < this->get_size(); i++)
   {
      this->element_at(i).GetText(str);
      wstrParam += str;
   }
}


bool LyricViewLine::CTokenJp1Set::Prepare()
{
   int iLen = 0;
   string str;
   for(int i = 0; i < this->get_size(); i++)
   {
      CToken & token = this->element_at(i);
      token.SetStartIndex(iLen);
      token.GetText(str);
      iLen += str.get_length();
      }
   return true;
}


             LyricViewLine::CToken & LyricViewLine::CToken::operator = (const CToken & token)
             {
                m_iaPosition.copy(token.m_iaPosition);
                m_iStartIndex = token.m_iStartIndex;
                m_str = token.m_str;

                return *this;
             }

             LyricViewLine::CTokenJp1 & LyricViewLine::CTokenJp1::operator = (const CToken & token)
             {
                CToken::operator =(token);
                return *this;
             }

             LyricViewLine::CTokenJp1 & LyricViewLine::CTokenJp1::operator = (const CTokenJp1 & token)
             {
                CToken::operator =(token);
                m_floatRateX = token.m_floatRateX;
                return *this;
             }


             string LyricViewLine::GetMainString()
             {
                string str;
                GetMainString(str);
                return str;
             }

             bool LyricViewLine::CToken::Prepare()
             {
                return true;
             }



             void LyricViewLine::CTokenJp1Set::copy(const CTokenSet & src)
             {
                remove_all();
                CTokenJp1 tokenJp1;
                for(int i = 0; i < src.get_size(); i++)
                {
                   tokenJp1 = src[i];
                   add(tokenJp1);
                }
             }

             int LyricViewLine::CToken::width()
             {
                if(m_iaPosition.get_size())
                   return m_iaPosition.last_element();
                else
                   return 0;
             }

             int LyricViewLine::CToken::Left()
             {
                return m_iaPosition[0];
             }

             int LyricViewLine::CToken::Right()
             {
                if(m_iaPosition.get_size() > 0)
                   return m_iaPosition.last_element();
                else
                   return m_iaPosition[0];

             }

             void LyricViewLine::CTokenJp1Set::copy(const CTokenJp1Set &src)
             {
                remove_all();
                for(int i = 0; i < src.get_size(); i++)
                {
                   add(src[i]);
                }
             }




bool LyricViewLine::CalcChar(point pt, int &iToken, int &iChar)
{
   if(m_tokenaMain.CalcChar(pt, iToken, iChar))
   {
      iToken += m_iFirstToken;
      return true;
   }
   return false;
}

bool LyricViewLine::CToken::CalcChar(point point, int &iChar)
{
   for(int i = 0; i <= m_str.get_length(); i++)
   {
      if(point.x < get_position(i))
      {
         if(i <= 0)
            return false;
         else
         {
            iChar = i - 1;
            return true;
         }
      }
   }
   return false;

}

bool LyricViewLine::CTokenSet::CalcChar(point point, int &iToken, int &iChar)
{
   int iPosition;
   int i;
   for(i = 0; i < this->get_size(); i++)
   {
      iPosition = this->element_at(i).get_position();
      if(point.x < iPosition)
      {
         if(i <= 0)
            return false;
         else
         {
            if(this->element_at(i - 1).CalcChar(point, iChar))
            {
               iToken = i - 1;
               return true;
            }
            else
               return false;
         }
      }
   }
   i = this->get_size() - 1;
   if(i >= 0)
   {
      if(this->element_at(i).CalcChar(point, iChar))
      {
         iToken = i;
         return true;
      }
      else
         return false;
   }
   else
      return false;

}

int LyricViewLine::CTokenSet::GetEndPosition()
{
   if(this->get_size() <= 0)
      return 0x80000000;
   else
      return this->element_at(this->get_size() - 1).GetEndPosition();
}

int LyricViewLine::CToken::GetEndPosition()
{
   return get_position(m_str.get_length());
}



void LyricViewLine::GetSelRect(LPRECT lprect)
{
   kar::LyricViewLineSelectionItem item;
   if(GetSelection().get_item(item, *this))
   {
      rect rect;
      GetPlacement(rect);
      int iTokenStart = item.GetTokenStart() - m_iFirstToken;
      int iTokenEnd = item.GetTokenEnd() - m_iFirstToken;
      int iCharStart = item.GetCharStart();
      int iCharEnd = item.GetCharEnd();
      if(iTokenStart < 0)
      {
         iTokenStart = 0;
         iCharStart = 0;
      }
      if(iTokenEnd >= m_tokenaMain.get_size())
      {
         iTokenEnd = m_tokenaMain.get_size() - 1;
         iCharEnd = m_tokenaMain.element_at(iTokenEnd).GetCharCount() - 1;
      }
      if(iCharEnd < 0)
      {
         while(iTokenEnd >= iTokenStart)
         {
            iCharEnd = m_tokenaMain.element_at(iTokenEnd).GetCharCount() - 1;
            if(iCharEnd >= 0)
               break;
            iTokenEnd--;
         }
      }
      if(iTokenEnd >= iTokenStart)
      {
         lprect->left = m_tokenaMain.element_at(iTokenStart).get_position(iCharStart);
         lprect->top = rect.top;
         lprect->right = m_tokenaMain.element_at(iTokenEnd).get_position(iCharEnd);
         lprect->bottom = rect.bottom;
      }
      else
      {
         lprect->left = 0;
         lprect->top = 0;
         lprect->right = 0;
         lprect->bottom = 0;

      }
   }
   else
   {
      lprect->left = 0;
      lprect->top = 0;
      lprect->right = 0;
      lprect->bottom = 0;
   }


}



user::e_line_hit LyricViewLine::hit_test(const POINT &ptCursorParam, int &iToken, int &iChar)
{
   bool bInside;
   point ptCursor = ptCursorParam;
   rect rectPlacement;
   GetPlacement(rectPlacement);
   bInside = rectPlacement.contains(ptCursor) != 0;
   if(!bInside)
   {
      iToken =  -1;
      iChar =  -1;
      return ::user::line_hit_none;
   }
   else
   {
      if(m_tokenaMain.get_size() <= 0)
         return ::user::line_hit_none;
      if(CalcChar(ptCursor, iToken, iChar))
      {
         if(HasLink(iToken, iChar))
         {
            return ::user::line_hit_link;
         }
         else
         {
            return ::user::line_hit_normal;
         }
      }
      else
      {
         iToken =  -1;
         iChar =  -1;
         return ::user::line_hit_none;
      }
   }

}


void LyricViewLine::UpdateHover(point &ptCursor)
{
   int iToken, iChar;
   user::e_line_hit etest = hit_test(
      ptCursor, iToken, iChar);
   if(etest == ::user::line_hit_link)
   {
      if(GetLinkIndex(iToken, iChar))
      {
         m_iLinkHoverToken = iToken;
         m_iLinkHoverIndex = GetLinkIndex(iToken, iChar);
         rect rect;
         GetPlacement(rect);
         GetKaraokeView()->_001RedrawWindow();
      }
   }
   else
   {
      if(m_iLinkHoverToken != -1
         ||
         m_iLinkHoverIndex != -1)
      {
         m_iLinkHoverToken = -1;
         m_iLinkHoverIndex = -1;
         rect rect;
         GetPlacement(rect);
         GetKaraokeView()->_001RedrawWindow();
      }
   }

}

bool LyricViewLine::IsInHover()
{
   if(m_iLinkHoverIndex >= 0
      && m_iLinkHoverToken >= 0)
      return true;
   else
      return false;
}

user::e_line_hit LyricViewLine::GetLink(string &str, const POINT & ptCursor)
{
   int iToken, iChar;
   user::e_line_hit etest = hit_test(ptCursor, iToken, iChar);
   if(etest == ::user::line_hit_link)
   {
      GetLink(str, iToken, iChar);
   }
   return etest;
}



bool LyricViewLine::GetLink(string & str, int iToken, int iChar)
{
   if(!HasLink(iToken, iChar))
      return false;
   str = m_straLink.element_at(GetLinkIndex(iToken, iChar));
   return !str.is_empty();
}


void LyricViewLine::Invalidate(LPCRECT lpcrect)
{
   rect rect;
   GetPlacement(rect);
   if(lpcrect != NULL)
   {
      rect.intersect(rect, lpcrect);
   }
   m_rectaInvalidate.add(rect);
}

void LyricViewLine::Validate()
{
   m_rectaInvalidate.remove_all();
}

void LyricViewLine::CommonConstruct()
{
   m_tokenaMain.SetLine(this);
   m_tokenaRaw.SetLine(this);

   m_ponvisible         = NULL;
   m_bEnhancedEmboss    = true;
   m_iFirstToken        = -1;
   m_iTokenCount        = 0;
   m_iLastToken         = -2;
   m_ptTextOffset.x     = 0;
   m_ptTextOffset.y     = 0;
   m_iLine              = -1;
   m_dwNewTime          = 0;
   m_lpBitmapData       = NULL;
   m_bAutoSizeX         = false;
   m_bAutoSizeY         = false;
   m_iAlign             = AlignLeft;
   m_iRelativeLineIndex = -1;
   m_bVisible           = false;
   m_pContainer         = NULL;
   m_iAnimateType       = AnimateNoAnimate;
   m_iTextEffect        = EffectSimple;
   m_dAnimateProgress   = 0;
   m_iIndent            = 0;
   m_ptemplate          = NULL;
   m_bCacheEmboss       = false;

}


int LyricViewLine::GetMiddle(LyricEventsV2 * pevent)
{
   UNREFERENCED_PARAMETER(pevent);
   kar::KaraokeView * pview = m_pContainer->GetKaraokeView();
   ikar::data & data = pview->GetKaraokeData();
   ikar::dynamic_data & dynamicdata = data.GetDynamicData();
   m_iFillRateToken = dynamicdata.m_iPlayingTokenIndex;
   m_iFillRateNote = dynamicdata.m_iPlayingNoteIndex;

   int iPlayingTokenIndex = m_iFillRateToken;
//   int iPlayingNoteIndex = m_iFillRateNote;
   int iMiddle = m_iMiddle;
   rect rectPlacement;
   GetPlacement(rectPlacement);
   if(iPlayingTokenIndex < GetFirstToken())
   {
      iMiddle = rectPlacement.left;
   }
   else if(iPlayingTokenIndex > GetLastToken())
   {
      iMiddle = rectPlacement.right;
   }
   else
   {
      int iToken = iPlayingTokenIndex - m_iFirstToken;
      string str;
      m_tokenaMain[iToken].GetText(str);

      int iFirstChar = 0;
//      int charsLength = str.get_length();
      str.trim_right();
      int iTrimLength = str.get_length();
      if(iTrimLength > 0)
      {
         while(str[iFirstChar] == L' ')
         {
            iFirstChar++;
         }
      }
      int iLeft   = m_tokenaMain[iToken].get_position(iFirstChar);
      int iRight  = m_tokenaMain[iToken].get_position(iTrimLength);
      iMiddle     = (int) ((iRight - iLeft) * m_dNoteFillRate)  + iLeft;
    }
    return iMiddle;
}

void LyricViewLine::Invalidate(
   int iPlayingTokenIndex,
   int iPlayingNoteIndex,
   LyricEventsV2 * pevent)
{
   UNREFERENCED_PARAMETER(iPlayingTokenIndex);
   UNREFERENCED_PARAMETER(iPlayingNoteIndex);
   int iMiddleStart = m_iMiddle;
   int iMiddleEnd = m_iMiddle;

   int iNewMiddle = GetMiddle(pevent);

   if(iNewMiddle == m_iMiddle)
   {
      return;
   }
   else if(iNewMiddle > m_iMiddle)
   {
      iMiddleEnd = iNewMiddle;
   }
   else
   {
      iMiddleStart = iNewMiddle;
   }

   if(iMiddleStart < iMiddleEnd)
   {
      rect rect;
      GetPlacement(rect);
      rect.left = iMiddleStart;
      rect.right = iMiddleEnd;
      Invalidate(rect);
   }



}

bool LyricViewLine::HasPlayed(int iPlayingTokenIndex)
{
   return  iPlayingTokenIndex > GetLastToken();
}

bool LyricViewLine::IsPlaying(int iPlayingTokenIndex)
{
   return ContainsToken(iPlayingTokenIndex);
}

void LyricViewLine::SetLine(int iLine)
{
   m_iLine = iLine;
}


void LyricViewLine::SetPlayStartTime(imedia::time itime)
{
   m_msPlayStart = itime;
}

imedia::time LyricViewLine::GetPlayStartTime()
{
   return m_msPlayStart;
}

void LyricViewLine::SetPlayStartTime()
{
   SetPlayStartTime(GetTickCount());
}

LyricViewLine &    LyricViewLine::operator =(const LyricViewLine & viewline)
{
   m_pContainer                  = viewline.m_pContainer;
   m_bAutoSizeX                  = viewline.m_bAutoSizeX;
   m_bAutoSizeY                  = viewline.m_bAutoSizeY;
   m_bPendingLayoutUpdate        = viewline.m_bPendingLayoutUpdate;
   m_bVisible                    = viewline.m_bVisible;
   m_crForeground                = viewline.m_crForeground;
   m_dAnimateProgress            = viewline.m_dAnimateProgress;
   m_dAnimateProgressIncrement   = viewline.m_dAnimateProgressIncrement;
   m_dNoteFillRate               = viewline.m_dNoteFillRate;
   m_iFillRateNote               = viewline.m_iFillRateNote;
   m_dwCleanTime                 = viewline.m_dwCleanTime;
   m_dwNewTime                   = viewline.m_dwNewTime;
   m_msPlayStart                 = viewline.m_msPlayStart;
   m_floatRateX                  = viewline.m_floatRateX;
   m_bAutoSizeX                  = viewline.m_bAutoSizeX;
   m_bAutoSizeY                  = viewline.m_bAutoSizeY;
   m_bVisible                    = viewline.m_bVisible;
   m_dwCleanTime                 = viewline.m_dwCleanTime;
   m_dwNewTime                   = viewline.m_dwNewTime;
   m_iAlign                      = viewline.m_iAlign;
   m_iLineIndex                  = viewline.m_iLineIndex;
   m_iMiddle                     = viewline.m_iMiddle;
   m_iNotesLength                = viewline.m_iNotesLength;
   m_iFirstToken                 = viewline.m_iFirstToken;
   m_iStatus                     = viewline.m_iStatus;
   m_iTokenCount                 = viewline.m_iTokenCount;
   m_iLastToken                  = viewline.m_iLastToken;
   m_lpBitmapData                = viewline.m_lpBitmapData;
   m_nFont                       = viewline.m_nFont;
   m_iRelativeLineIndex          = viewline.m_iRelativeLineIndex;
   m_iAnimateType                = viewline.m_iAnimateType;
   m_iTextEffect                 = viewline.m_iTextEffect ;
   m_dAnimateProgress            = viewline.m_dAnimateProgress;
   m_dAnimateProgressIncrement   = viewline.m_dAnimateProgressIncrement;
   m_tokenaRaw.copy(viewline.m_tokenaRaw);
   m_tokenaMain.copy(viewline.m_tokenaMain);
   m_tokenaJp1.copy(viewline.m_tokenaJp1);
   m_iaCharLink.copy(viewline.m_iaCharLink);
   m_iaLink.copy(viewline.m_iaLink);
   m_straLink.copy(viewline.m_straLink);
   SetTemplate(viewline.m_ptemplate);
   return *this;
}


kar::LyricViewLineSelection::e_state LyricViewLine::GetSelectionState()
{
   return GetContainer()->GetSelectionState();
}


void LyricViewLine::relay_event(gen::signal_object * pobj)
{
   GetSelection().relay_event(*this, pobj);
}

void LyricViewLine::SetTokenCount(int iCount)
{
   m_iTokenCount = iCount;
   if(m_iFirstToken < 0)
      m_iLastToken = m_iFirstToken - 1;
   else
      m_iLastToken = m_iFirstToken + iCount -1;
}




kar::KaraokeView * LyricViewLine::GetKaraokeView()
{
   return m_pContainer->GetKaraokeView();
}

bool LyricViewLine::OnMouseMove(UINT user, point pt)
{
   UNREFERENCED_PARAMETER(user);

   kar::LyricViewLineSelection & selection = GetSelection();

   if(selection.GetState() == kar::LyricViewLineSelection::StateTracking)
   {

      bool bInside;
      int iToken;
      int iChar;
      rect rectPlacement;

      GetPlacement(rectPlacement);
      bInside = rectPlacement.contains(pt) != 0;

      if(!bInside)
      {
         if(pt.y < rectPlacement.top
            || (pt.y <= rectPlacement.bottom &&
            pt.x < rectPlacement.left))
         {
            selection.SetSelBefore(*this);
            GetKaraokeView()->_001RedrawWindow();
         }
         else
         {
            selection.SetSelAfter(*this);
            GetKaraokeView()->_001RedrawWindow();
         }
         return false;
      }
      else // bInside == true
      {
         //DWORD fwKeys = user; // key flags
         if(m_tokenaMain.get_size() <= 0)
            return false;
         if(CalcChar(pt, iToken, iChar))
         {
            selection.m_item.SetTokenEnd(iToken);
            selection.m_item.SetCharEnd(iChar);
            GetKaraokeView()->_001RedrawWindow();
         }
         else
         {
            if(pt.x < m_tokenaMain.element_at(0).get_position())
            {
               selection.SetSelBefore(*this);
            }
            else
            {
               selection.SetSelAfter(*this);
            }
            GetKaraokeView()->_001RedrawWindow();
         }
         return true;
      }
   }
   else
   {
      if(IsInHover())
      {
         UpdateHover(pt);
      }
      else
      {
         bool bInside;
         rect rectPlacement;

         GetPlacement(rectPlacement);
         bInside = rectPlacement.contains(pt) != 0;
         if(bInside)
         {
            UpdateHover(pt);
         }
      }

   }
   return false;
}

BOOL LyricViewLine::OnSetCursor(HWND hwnd, UINT uiHitTest, UINT uiMessage)
{
   UNREFERENCED_PARAMETER(hwnd);
   UNREFERENCED_PARAMETER(uiHitTest);
   UNREFERENCED_PARAMETER(uiMessage);
   if(IsInHover())
   {
      ::SetCursor(GetKaraokeView()->KaraokeGetHandCursor());
      return TRUE;
   }

   return FALSE;
}

bool LyricViewLine::OnLButtonDown(UINT user, point pt)
{
   if(GetSelection().OnLButtonDown(*this, user, pt))
      return true;
   return false;
}

bool LyricViewLine::OnLButtonUp(UINT user, point pt)
{
   if(GetSelection().OnLButtonUp(*this, user, pt))
      return true;
   return false;
}

bool LyricViewLine::OnTimer(UINT user)
{
   if(GetSelection().OnTimer(*this, user))
      return true;
   return false;
}

int LyricViewLine::CToken::GetCharCount()
{
   return m_str.get_length();
}

int LyricViewLine::GetLinkIndex(int iToken, int iChar)
{
   if(!HasLink())
      return -1;
   iToken -= GetFirstToken();
   iChar += m_tokenaMain.element_at(iToken).GetStartIndex();
   return m_iaCharLink.element_at(iChar);
}



bool LyricViewLine::HasLink()
{
   return m_iaLink.get_size() > 0;
}


void LyricViewLine::PrepareURLLinks()
{
   int j;
   string str;
   string strTemp;
   m_tokenaMain.GetText(str);
   int iLink = 0;
   int iStart = 0;
   int iIndex;
   int iOldIndex = 0;
   enum
   {
      typeMail,
      typeURL,
      typeMail2,
   };
   int iType;
   int iCharCount;
   int iChar;
   m_iaLink.remove_all();
   m_iaCharLink.remove_all();
   m_straLink.remove_all();
   while(true)
   {
      iIndex = str.find("http:/", iOldIndex);
      if(iIndex < 0)
      {
         iIndex = str.find("www.", iOldIndex);
      }
      iType = typeURL;
      if(iIndex < 0)
      {
         iType = typeMail;
         iIndex = str.find("mailto:", iOldIndex);
         if(iIndex < 0)
         {
            iType = typeMail2;
            iIndex = str.find("@", iOldIndex);
            if(iIndex > 0)
            {
               int iDot = str.find(".", iIndex);
               if(iDot > 0)
               {
                  int iEnd = str.find(" ", iIndex);
                  if(iEnd >= 0)
                  {
                     if(iEnd < iDot)
                     {
                        iIndex = -1;
                     }
                  }
                  int iArrobaIndex = iIndex;
                  if(iIndex > 0)
                  {
                     iIndex--;
                     while(iIndex > 0)
                     {
                        if(isalpha(str[iIndex])
                        || isdigit(str[iIndex])
                        || str[iIndex] == '.'
                        || str[iIndex] == '-'
                        || str[iIndex] == '_')
                        {
                           iIndex--;
                        }
                        else
                        {
                           if(iIndex < (iArrobaIndex - 1))
                           {
                              if(iIndex > 0 &&
                                 (isspace(str[iIndex])
                                 || str[iIndex] == '\\'
                                 || str[iIndex] == '/'
                                 || str[iIndex] == ':'
                                 || str[iIndex] == ';'
                                 || str[iIndex] == ','))
                              {
                                 iIndex++;
                              }
                           }
                           break;
                        }

                     }
                  }
               }
               else
               {
                  iIndex = -1;
               }
            }
            if(iIndex < 0)
               break;
         }
      }


      for(j = m_iaCharLink.get_size(); j < iIndex; j++)
      {
         m_iaCharLink.add(-1);
      }
      iStart = iIndex;
      iCharCount = 0;
      iChar = iIndex;
      if(iType == typeURL)
      {
         int iColon = 0;
         while(iChar < str.get_length())
         {
            WCHAR wchar = str[iChar];
            if(iswalpha(wchar)
            || wchar == L'.'
            || wchar == L'/'
            || wchar == L'?'
            || wchar == L'&'
            || (wchar == L':' && iColon++ == 0))
            {
               m_iaCharLink.add(iLink);
               iCharCount++;
               iChar++;
            }
            else
            {
               break;
            }
         }
      }
      else
      {
         int iArrobaCount = 0;
         while(iChar < str.get_length()
            && (iswalpha(str[iChar])
            || iswdigit(str[iChar])
            || (iCharCount > 0 && iArrobaCount == 0 && str[iChar] == '@')
            || str[iChar] == '.'
            || str[iChar] == '-'
            || str[iChar] == '_'))
         {
            if(str[iChar] == L'@')
            {
               iArrobaCount++;
            }
            m_iaCharLink.add(iLink);
            iCharCount++;
            iChar++;
         }
      }
      if(iCharCount > 0)
      {
         m_iaLink.add(iLink);
         if(iType == typeMail2)
         {
            strTemp = "mailto:";
            strTemp += str.Mid(iStart, iCharCount);
            m_straLink.add(strTemp);
         }
         else
         {
            m_straLink.add(str.Mid(iStart, iCharCount));
         }
         iLink++;
      }
      iStart = iIndex + iCharCount;
      if(iCharCount <= 0)
      {
         if(iIndex <= iOldIndex)
         {
            iOldIndex++;
         }
         else
         {
            iOldIndex = iIndex + 1;
         }
      }
      else
      {
         iOldIndex = iIndex + iCharCount;
      }
   }
   if(m_iaLink.get_size() > 0)
   {
      iIndex = str.get_length();
      for(j = iStart; j < iIndex; j++)
      {
         m_iaCharLink.add(-1);
      }
   }

}

/*
void LyricViewLine::EmbossedTextOut(
   ::ca::graphics *          pdc,
   const char *   psz,
   int            iLeft,
   int            iTop,
   int            iWidth,
   COLORREF       cr,
   COLORREF       crOutline,
   int            iLen)
{
   ASSERT(FALSE);
   return;
   rect rect;
   size size;
   size sizeInt;
   size sizeExt;
   pdc->SelectObject(m_font);
   int cx = 0;
   int width;
   int widthInt;
   int widthExt;
   int cxInt;
   int cxExt;
   int cyInt;
   int cyExt;
   ::GetTextExtentPoint32W(
      (HDC)pdc->get_os_data(),
      lpcsz,
      iLen,
      &size);
   pdc->SelectObject(m_fontInt);
   ::GetTextExtentPoint32W(
      (HDC)pdc->get_os_data(),
      lpcsz,
      iLen,
      &sizeInt);
   //cyInt = (size.cy - sizeInt.cy) / 2;
   cyInt = (size.cy * 1000 / 1200) / 2;

   pdc->SelectObject(m_fontExt);
   ::GetTextExtentPoint32W(
      (HDC)pdc->get_os_data(),
      lpcsz,
      iLen,
      &sizeExt);
   //cyExt = (size.cy - sizeExt.cy) / 2;
   cyExt = -(size.cy * 1200 / 1000) / 2;

   //XFORM xform;

   //pdc->GetWorldTransform(&xform);

   ::SetTextColor((HDC)pdc->get_os_data(), crOutline);
   for(int i = 0; i < iLen; i++)
   {
      WCHAR wch = lpcsz[i];


      pdc->SelectObject(m_font);
      ::GetTextExtentPoint32W(
         (HDC)pdc->get_os_data(),
         lpcsz,
         i + 1,
         &size);

      pdc->SetMapMode( MM_ANISOTROPIC);
      cxInt = (size.cx * 1000 / 1200) / 2;
      ::ScaleViewportExtEx(
         (HDC)pdc->get_os_data(),
         1000,
         1200,
         1000,
         1200,
         NULL);
      ::OffsetViewportOrgEx(
         (HDC)pdc->get_os_data(),
         iLeft + cx,
         iTop * cyInt,
         NULL);
      ::TextOutW((HDC)pdc->get_os_data(), 0, 0, &wch, 1);

      cxInt = -(size.cx * 1000 / 1200) / 2;
      ::ScaleViewportExtEx(
         (HDC)pdc->get_os_data(),
         1000,
         1200,
         1000,
         1200,
         NULL);
      ::OffsetViewportOrgEx(
         (HDC)pdc->get_os_data(),
         iLeft + cx,
         iTop  + cyExt,
         NULL);
      ::TextOutW((HDC)pdc->get_os_data(), 0, 0, &wch, 1);

      //pdc->SelectObject(m_fontInt);
      ::GetTextExtentPoint32W(
         (HDC)pdc->get_os_data(),
         &wch,
         1,
         &sizeInt);
      ::TextOutW((HDC)pdc->get_os_data(), 0, 0, &wch, 1);

      //pdc->SelectObject(m_fontExt);
      ::GetTextExtentPoint32W(
         (HDC)pdc->get_os_data(),
         &wch,
         1,
         &sizeExt);
      cxExt = (size.cx * 1200 / 1000);
      cyExt = (size.cy - sizeExt.cy) / 2;
      ::TextOutW((HDC)pdc->get_os_data(), iLeft + cx + cxExt, iTop + cyExt, &wch, 1);



      cx = size.cx;
   }
   pdc->SetMapMode(MM_TEXT);
   ::SetTextColor((HDC)pdc->get_os_data(), cr);
   pdc->SelectObject(m_font);
   ::TextOutW((HDC)pdc->get_os_data(), iLeft, iTop, lpcsz, iLen);*/


/*   ::SetTextColor((HDC)pdc->get_os_data(), crOutline);
   ::TextOutW((HDC)pdc->get_os_data(), iLeft - iWidth, iTop - iWidth, lpcsz, iLen);
   ::TextOutW((HDC)pdc->get_os_data(), iLeft - iWidth, iTop, lpcsz, iLen);
   ::TextOutW((HDC)pdc->get_os_data(), iLeft - iWidth, iTop + iWidth, lpcsz, iLen);
   ::TextOutW((HDC)pdc->get_os_data(), iLeft, iTop - iWidth, lpcsz, iLen);
   ::TextOutW((HDC)pdc->get_os_data(), iLeft + iWidth, iTop, lpcsz, iLen);
   ::TextOutW((HDC)pdc->get_os_data(), iLeft + iWidth, iTop - iWidth, lpcsz, iLen);
   ::TextOutW((HDC)pdc->get_os_data(), iLeft, iTop + iWidth, lpcsz, iLen);
   ::TextOutW((HDC)pdc->get_os_data(), iLeft + iWidth, iTop + iWidth, lpcsz, iLen);

   ::SetTextColor((HDC)pdc->get_os_data(), cr);
   ::TextOutW((HDC)pdc->get_os_data(), iLeft, iTop, lpcsz, iLen);*/

   /*string str;
   str = gen::international::utf8_to_unicode(psz);
   size size;
   ::GetTextExtentPoint32W(
      (HDC)pdc->get_os_data(),
      str,
      str.get_length(),
      &size);

   if(m_dc1.get_os_data() == NULL)
   {
      m_dc1.CreateCompatibleDC(pdc);

   }
   m_dc1.SelectObject(pdc->GetCurrentFont());

   if(m_bmp1.GetSafeHandle() == NULL)
   {
      m_bmp1.CreateCompatibleBitmap(pdc, size.cx, size.cy);
      m_dc1.SelectObject(m_bmp1);
   }
   else
   {
      BITMAP bm;
      m_bmp1.GetBitmap(&bm);
      if(bm.bmHeight < size.cx ||
         bm.bmWidth < size.cy)
      {
         m_bmp1.delete_object();
         m_bmp1.CreateCompatibleBitmap(pdc, size.cx, size.cy);
      }
      m_dc1.SelectObject(m_bmp1);
   }


   m_dc1.FillSolidRect(0, 0, size.cx,size.cy, RGB(0, 0, 0));
   m_dc1.SetTextColor(RGB(255, 255, 255));
   ::TextOutW(m_dc1.get_os_data(), 0, 0, lpcsz, iLen);

   System.imaging().channel_gray_blur(&m_dc1,0, 0, size.cx, size.cy,
      &m_dc1, 0, 0, 0, 2);

   System.imaging().clip_color_blend(pdc, iLeft, iTop, size.cx, size.cy,
      crOutline, &m_dc1, 0, 0);


/*   pdc->BeginPath();
   ::TextOutW((HDC)pdc->get_os_data(), iLeft, iTop, lpcsz, iLen);
   pdc->EndPath();

   LOGBRUSH lb;
   lb.lbStyle = BS_SOLID;
   lb.lbColor = crOutline;
   //::ca::pen_sp pen(get_app(), PS_SOLID, iWidth * 2 + 2, crOutline);
   ::ca::pen_sp pen(get_app(),
      PS_SOLID
      | PS_GEOMETRIC
      | PS_ENDCAP_ROUND
      | PS_JOIN_ROUND,
      iWidth * 2,
      &lb);

   ::ca::pen * ppenOld = pdc->SelectObject(pen);
   pdc->StrokePath();
   pdc->SelectObject(ppenOld);*/

   //::SetTextColor((HDC)pdc->get_os_data(), cr);
   ////m_dc1.SetBkMode(TRANSPARENT);
   //::TextOutW((HDC)pdc->get_os_data(), iLeft, iTop, lpcsz, iLen);

/*
}
*/

void LyricViewLine::EmbossedTextOut(
      ::ca::graphics * pdc,
      ::ca::graphics * pdcCache,
      const char * lpcsz,
      int iLeft,
      int iTop,
      int iWidth,
      COLORREF cr,
      COLORREF crOutline,
      int iLen,
      bool bBegin,
      bool bEnd)
{
   UNREFERENCED_PARAMETER(pdcCache);
   UNREFERENCED_PARAMETER(iLen);
   UNREFERENCED_PARAMETER(pdcCache);
   UNREFERENCED_PARAMETER(iWidth);
   UNREFERENCED_PARAMETER(crOutline);
   UNREFERENCED_PARAMETER(bBegin);
   UNREFERENCED_PARAMETER(bEnd);

   bool bSaveProcessing = System.savings().should_save(gen::resource_processing)
      || (m_bEnhancedEmboss && System.savings().should_save(gen::resource_blurred_text_embossing));

   if(bSaveProcessing)
   {
//      int iRadius = (int) (max(1.0f, m_floatRateX * 5.0));
      pdc->BeginPath();
      pdc->TextOut(iLeft, iTop, lpcsz);
      pdc->EndPath();

      //LOGBRUSH lb;
      //lb.lbStyle = BS_SOLID;
      //lb.lbColor = crOutline;
      //lb.lbHatch = 0;
      //::ca::pen_sp pen(get_app(), PS_SOLID | PS_GEOMETRIC | PS_ENDCAP_ROUND | PS_JOIN_ROUND, iRadius * 2, &lb);

      //::ca::pen * ppenOld = pdc->SelectObject(pen);
      pdc->StrokePath();
      //pdc->SelectObject(ppenOld);

      pdc->SetTextColor(cr);
      pdc->TextOut(iLeft, iTop, lpcsz);
   }
   else
   {
      pdc->SetTextColor(cr);
      pdc->TextOut(iLeft, iTop, lpcsz);
   }


}
void LyricViewLine::CacheEmboss(::ca::graphics * pdc, const char * lpcsz, int iLen)
{
   UNREFERENCED_PARAMETER(pdc);
   UNREFERENCED_PARAMETER(iLen);
   UNREFERENCED_PARAMETER(lpcsz);
   if(m_bCacheEmboss)
      return;

   m_bCacheEmboss = true;

   if(!m_bEnhancedEmboss)
      return;

   if(System.savings().should_save(gen::resource_processing)
      || System.savings().should_save(gen::resource_blurred_text_embossing))
      return;


   for(int iToken = 0; iToken < GetTokenCount(); iToken++)
   {
      CToken & token = m_tokenaMain[iToken];
      if(token.m_dibEmboss.is_null())
         token.m_dibEmboss.create(get_app());
      ::ca::dib & dibEmboss = token.m_dibEmboss;
      dibEmboss.create(1, 1);
      ::ca::graphics * pdcCache = dibEmboss.get_graphics();
      ::ca::bitmap * pbmpCache = dibEmboss.get_bitmap();
      pdcCache = dibEmboss.get_graphics();
      pbmpCache = dibEmboss.get_bitmap();

      int iRadius = max(1, (int) (m_floatRateX * 5.0));


      if(m_ponvisible == NULL)
         continue;

      pdcCache->SelectObject(m_ponvisible->m_font);

      string str;
      token.GetText(str);
      size size = pdcCache->GetTextExtent(str, str.get_length());

      class size sizeBmp(size);

      sizeBmp.cx += iRadius * 2 * 2;
      sizeBmp.cy += iRadius * 2 * 2;


      dibEmboss.create(sizeBmp);
      pdcCache = dibEmboss.get_graphics();
      pbmpCache = dibEmboss.get_bitmap();

      pdcCache->SelectObject(m_ponvisible->m_font);
      pdcCache->FillSolidRect(0, 0, sizeBmp.cx, sizeBmp.cy, RGB(0, 0, 0));
      pdcCache->SetTextColor(RGB(255, 255, 255));
      pdcCache->TextOut(iRadius * 2, iRadius * 2, str, str.get_length());

      System.imaging().channel_spread(pdcCache, null_point(), sizeBmp, pdcCache, null_point(), 0, iRadius / 2);
      System.imaging().channel_alpha_gray_blur(pdcCache, null_point(), sizeBmp,  pdcCache, null_point(), 0, iRadius);
      //System.imaging().channel_gray_blur(pdcCache, null_point(), sizeBmp,  pdcCache, null_point(), 0, 2);
      //System.imaging().pre_color_blend(pdcCache, pdcCache, RGB(96, 96, 92));
      //System.imaging().pre_color_blend(pdcCache, pdcCache, RGB(255, 255, 255));
      System.imaging().pre_color_blend(pdcCache, pdcCache, RGB(84, 84, 77));
      dibEmboss.channel_multiply(visual::rgba::channel_alpha, 1.2);
      //m_dibEmboss->channel_invert(visual::rgba::channel_alpha);
   }

}


#ifndef _DEBUG


void * LyricViewLine::operator new(size_t st)
{
   return _vmsLyricViewLineAlloc.Alloc();
}

void LyricViewLine::operator delete (void * p)
{
   _vmsLyricViewLineAlloc.Free(p);
}

#endif _DEBUG

void LyricViewLine::SetOffsetY(int iy)
{
   if(GetLine() == 1 || GetLine() == 0)
   {
      //TRACE("LyricViewLine::SetOffsetY iY=%d iLine=%d templ=%d\n", iy, GetLine(), (long) GetTemplate());
   }
   m_ptTextOffset.y = iy;
}

int LyricViewLine::GetOffsetY()
{
   return m_ptTextOffset.y;
}

point LyricViewLine::GetOffset()
{
   return m_ptTextOffset;
}



void LyricViewLine::SetNoteFillRate(double dFillRate)
{
   kar::KaraokeView * pview = m_pContainer->GetKaraokeView();
   ikar::data & data = pview->GetKaraokeData();
   ikar::dynamic_data & dynamicdata = data.GetDynamicData();
   m_iFillRateToken = dynamicdata.m_iPlayingTokenIndex;
   m_iFillRateNote = dynamicdata.m_iPlayingNoteIndex;
   m_dNoteFillRate = dFillRate;
}

void LyricViewLine::SetNoteMsLength(imedia::time dwMsLength)
{
   m_dwNoteMsLength = dwMsLength;
}

double LyricViewLine::GetFillRate()
{
   /*kar::KaraokeView * pview = m_pContainer->GetKaraokeView();
   ikar::data & data = pview->GetKaraokeData();
   ikar::dynamic_data & dynamicdata = data.GetDynamicData();
   if(m_iFillRateNote == dynamicdata.m_iPlayingNoteIndex &&
      m_iFillRateToken == dynamicdata.m_iPlayingTokenIndex)
   {*/
      return m_dNoteFillRate;
   /*}
   else if(m_iFillRateNote < dynamicdata.m_iPlayingNoteIndex)
   {
      return 1.0;
   }
   else
   {
      return 0.0;
   }*/
}

bool LyricViewLine::MaybeURL(const char * lpcsz)
{
   string str(lpcsz);
   int iStr = str.get_length();
   if((iStr >= 5 && str.Mid(0, 5) == L"http:"))
      return true;
   if((iStr >= 4 && str.Mid(0, 4) == L"www."))
      return true;
   return false;
}


bool LyricViewLine::is_empty()
{
   if(m_iTokenCount == 0)
      return true;
   string str;
   m_tokenaRaw.GetText(str);
   str.trim();
   return str.is_empty();
}

bool LyricViewLine::IsFirstLine()
{
   return m_ptemplate->IsFirstLine();
}

int LyricViewLine::GetLine()
{
   return m_iLine;
}

int LyricViewLine::CToken::GetStartIndex()
{
   return m_iStartIndex;
}

int LyricViewLine::CToken::get_position()
{
   return m_iaPosition[0];
}

int LyricViewLine::CToken::get_position(int iChar)
{
   return m_iaPosition[iChar];
}

bool LyricViewLine::HasLink(int iToken, int iChar)
{
   return GetLinkIndex(iToken, iChar) >= 0;
}

int LyricViewLine::CTokenSet::GetFirstToken()
{
   return m_pline->GetFirstToken();
}

void LyricViewLine::CTokenSet::SetLine(LyricViewLine * pline)
{
   m_pline = pline;
}


bool LyricViewLine::IsVisible()
{
   return m_bVisible;
}


bool kar::LyricViewLineSelectionItem::Intersect(LyricViewLine &viewline)
{
   return Intersect(
      viewline.GetFirstToken(),
      viewline.GetLastToken());

}


int LyricViewLine::GetFirstToken()
{
   return m_iFirstToken;
}

int LyricViewLine::GetLastToken()
{
   return m_iLastToken;
}

bool LyricViewLine::ContainsToken(int iToken)
{
   return iToken >= m_iFirstToken
      && iToken <= m_iLastToken;
}

int LyricViewLine::GetTokenCount()
{
   return m_iTokenCount;
}

bool LyricViewLine::GetTokenText(string & str)
{
   UNREFERENCED_PARAMETER(str);
   ASSERT(FALSE);
   return false;
}

LyricViewLines * LyricViewLine::GetContainer()
{
 return m_pContainer;
}

void LyricViewLine::CToken::GetText(string & str)
{
   str = m_str;
}



kar::LyricViewLineSelection & LyricViewLines::GetSelection()
{
   return m_selection;
}

kar::LyricViewLineSelection & LyricViewLine::GetSelection()
{
   if(GetContainer() == NULL)
      return *((kar::LyricViewLineSelection *) NULL);
   else
      return GetContainer()->GetSelection();
}

