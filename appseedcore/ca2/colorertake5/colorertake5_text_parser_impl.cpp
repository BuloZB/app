#include "StdAfx.h"

namespace colorertake5
{

   text_parser_impl::text_parser_impl()
   {
      CLR_TRACE("text_parser_impl", "constructor");
      cache = new parse_cache();
      clearCache();
      lineSource = NULL;
      regionHandler = NULL;
      picked = NULL;
      baseScheme = NULL;
   }
   text_parser_impl::~text_parser_impl()
   {
      clearCache();
      delete cache;
   }

   void text_parser_impl::setFileType(file_type *type){
      baseScheme = NULL;
      if (type != NULL){
         baseScheme = (scheme_impl*)(type->getBaseScheme());
      };
      clearCache();
   };

   void text_parser_impl::setLineSource(line_source *lh){
      lineSource = lh;
   };

   void text_parser_impl::setRegionHandler(RegionHandler *rh){
      regionHandler = rh;
   };

   int text_parser_impl::parse(int from, int num, TextParseMode mode)
   {
      gx = 0;
      gy = from;
      gy2 = from+num;
      clearLine = -1;

      invisibleSchemesFilled = false;
      //  schemeStart = -1;
      breakParsing = false;
      updateCache = (mode == TPM_CACHE_UPDATE);

      CLR_TRACE("text_parser_impl", "parse from=%d, num=%d", from, num);
      /* Check for initial bad conditions */
      if (regionHandler == NULL) return from;
      if (lineSource == NULL) return from;
      if (baseScheme == NULL) return from;

      vtlist = new VTList();

      lineSource->startJob(from);
      regionHandler->startParsing(from);

      /* Init cache */
      parent = cache;
      forward = NULL;
      cache->scheme = baseScheme;

      if (mode == TPM_CACHE_READ || mode == TPM_CACHE_UPDATE) {
         parent = cache->searchLine(from, &forward);
         if (parent != NULL){
            CLR_TRACE("TPCache", "searchLine() parent:%s,%d-%d", parent->scheme->getName(), parent->sline, parent->eline);
         }
      }
      cachedLineNo = from;
      cachedParent = parent;
      cachedForward = forward;
      CLR_TRACE("text_parser_impl", "parse: cache filled");


      do{
         if (!forward) {
            if (!parent) {
               return from;
            }
            if (updateCache){
               delete parent->children;
               parent->children = NULL;
            }
         }else{
            if (updateCache){
               delete forward->next;
               forward->next = NULL;
            }
         };
         baseScheme = parent->scheme;

         stackLevel = 0;
         CLR_TRACE("text_parser_impl", "parse: goes into colorize()");
         if (parent != cache) {
            vtlist->restore(parent->vcache);
            parent->clender->end->setBackTrace(*parent->backLine, &parent->matchstart);
            colorize(parent->clender->end, parent->clender->lowContentPriority);
            vtlist->clear();
         }else{
            colorize(NULL, false);
         };

         if (updateCache){
            if (parent != cache){
               parent->eline = gy;
            };
         }
         if (parent != cache && gy < gy2){
            leaveScheme(gy, &matchend, parent->clender);
         };
         gx = matchend.e[0];

         forward = parent;
         parent = parent->parent;
      }while(parent);
      regionHandler->endParsing(endLine);
      lineSource->endJob(endLine);
      delete vtlist;
      return endLine;
   };

   void text_parser_impl::clearCache()
   {
      delete cache->children;
      cache->sline = 0;
      cache->eline = 0x7FFFFFF;
      cache->children = cache->parent = cache->next = NULL;
   };

   void text_parser_impl::breakParse()
   {
      breakParsing = true;
   };

   void text_parser_impl::addRegion(int lno, int sx, int ex, class region* region){
      if (sx == -1 || region == NULL) return;
      regionHandler->addRegion(lno, str, sx, ex, region);
   };
   void text_parser_impl::enterScheme(int lno, int sx, int ex, class region* region){
      regionHandler->enterScheme(lno, str, sx, ex, region, baseScheme);
   };
   void text_parser_impl::leaveScheme(int lno, int sx, int ex, class region* region){
      regionHandler->leaveScheme(lno, str, sx, ex, region, baseScheme);
      if (region != NULL) picked = region;
   };


   void text_parser_impl::enterScheme(int lno, SMatches *match, SchemeNode *schemeNode)
   {
      int i;

      if (schemeNode->innerRegion == false)
      {
         enterScheme(lno, match->s[0], match->e[0], schemeNode->region);
      }

      for (i = 0; i < match->cMatch; i++)
         addRegion(lno, match->s[i], match->e[i], schemeNode->regions[i]);
      //  for (i = 0; i < match->cnMatch; i++)
      //  addRegion(lno, match->ns[i], match->ne[i], schemeNode->regionsn[i]);

      if (schemeNode->innerRegion == true){
         enterScheme(lno, match->e[0], match->e[0], schemeNode->region);
      }
   }

   void text_parser_impl::leaveScheme(int lno, SMatches *match, SchemeNode *schemeNode)
   {
      UNREFERENCED_PARAMETER(lno);
      int i;

      if (schemeNode->innerRegion == true){
         leaveScheme(gy, match->s[0], match->s[0], schemeNode->region);
      }

      for (i = 0; i < match->cMatch; i++)
         addRegion(gy, match->s[i], match->e[i], schemeNode->regione[i]);
      //  for (i = 0; i < match->cnMatch; i++)
      //  addRegion(gy, match->ns[i], match->ne[i], schemeNode->regionen[i]);

      if (schemeNode->innerRegion == false){
         leaveScheme(gy, match->s[0], match->e[0], schemeNode->region);
      }
   }

   void text_parser_impl::fillInvisibleSchemes(parse_cache *ch)
   {
      if (!ch->parent || ch == cache){
         return;
      }
      /* Fills output stream with valid "pseudo" enterScheme */
      fillInvisibleSchemes(ch->parent);
      enterScheme(gy, 0, 0, ch->clender->region);
      return;
   };

   int text_parser_impl::searchKW(const SchemeNode *node, int no, int lowlen, int hilen)
   {
      UNREFERENCED_PARAMETER(no);
      UNREFERENCED_PARAMETER(hilen);
      if (!node->kwList->num) return MATCH_NOTHING;

      if (node->kwList->minKeywordLength+gx > lowlen) return MATCH_NOTHING;
      if (gx < lowlen && !node->kwList->firstChar->in_class(&((const char *)str)[gx])) return MATCH_NOTHING;

      int left = 0;
      int right = node->kwList->num;
      while(true){
         int pos = left + (right-left)/2;
         int kwlen = node->kwList->kwList[pos].keyword.get_length();
         if (lowlen < gx+kwlen) kwlen = lowlen-gx;

         int cr;
         if (node->kwList->matchCase)
            cr = node->kwList->kwList[pos].keyword.Compare(string(&((const char*)str)[gx], kwlen));
         else
            cr = node->kwList->kwList[pos].keyword.CompareNoCase(string(&((const char*)str)[gx], kwlen));

         if (cr == 0 && right-left == 1){
            bool badbound = false;
            if (!node->kwList->kwList[pos].isSymbol){
               if (!node->worddiv){
                  if (gx && (gen::ch::is_letter_or_digit(&((const char *)str)[gx-1]) || ((const char *)str)[gx-1] == '_')) badbound = true;
                  if (gx + kwlen < lowlen &&
                     (gen::ch::is_letter_or_digit(&((const char *)str)[gx + kwlen]) || ((const char *)str)[gx + kwlen] == '_')) badbound = true;
               }else{
                  // custom check for word bound
                  if (gx && !node->worddiv->in_class(&((const char *)str)[gx-1])) badbound = true;
                  if (gx + kwlen < lowlen &&
                     !node->worddiv->in_class(&((const char *)str)[gx + kwlen])) badbound = true;
               };
            };
            if (!badbound){
               addRegion(gy, gx, gx + kwlen, node->kwList->kwList[pos].region);
               gx += kwlen;
               return MATCH_RE;
            };
         };
         if (right-left == 1){
            left = node->kwList->kwList[pos].ssShorter;
            if (left != -1){
               right = left+1;
               continue;
            };
            break;
         };
         if (cr == 1) right = pos;
         if (cr == 0 || cr == -1) left = pos;
      };
      return MATCH_NOTHING;
   };

   int text_parser_impl::searchRE(scheme_impl *cscheme, int no, int lowLen, int hiLen)
   {
      int i, re_result;
      scheme_impl *ssubst = NULL;
      SMatches match;
      parse_cache *OldCacheF = NULL;
      parse_cache *OldCacheP = NULL;
      parse_cache *ResF = NULL;
      parse_cache *ResP = NULL;

      CLR_TRACE("text_parser_impl", "searchRE: entered scheme \"%s\"", cscheme->getName());

      if (!cscheme){
         return MATCH_NOTHING;
      }
      for(int idx = 0; idx < cscheme->nodes.get_size(); idx++){
         SchemeNode *schemeNode = cscheme->nodes.element_at(idx);
         CLR_TRACE("text_parser_impl", "searchRE: processing node:%d/%d, type:%s", idx+1, cscheme->nodes.get_size(), schemeNodeTypeNames[schemeNode->type]);
         switch(schemeNode->type){
         case SNT_INHERIT:
            if (!schemeNode->scheme) break;
            ssubst = vtlist->pushvirt(schemeNode->scheme);
            if (!ssubst){
               bool b = vtlist->push(schemeNode);
               re_result = searchRE(schemeNode->scheme, no, lowLen, hiLen);
               if (b) vtlist->pop();
            }else{
               re_result = searchRE(ssubst, no, lowLen, hiLen);
               vtlist->popvirt();
            };
            if (re_result != MATCH_NOTHING) return re_result;
            break;

         case SNT_KEYWORDS:
            if (searchKW(schemeNode, no, lowLen, hiLen) == MATCH_RE) return MATCH_RE;
            break;

         case SNT_RE:
            if (!schemeNode->start->parse(str, gx, schemeNode->lowPriority?lowLen:hiLen, &match, &schemeStart))
               break;
            for (i = 0; i < match.cMatch; i++)
               addRegion(gy, match.s[i], match.e[i], schemeNode->regions[i]);
            /*        for (i = 0; i < match.cnMatch; i++)
            addRegion(gy, match.ns[i], match.ne[i], schemeNode->regionsn[i]);*/

            /* skips regexp if it has zero get_length */
            if (match.e[0] == match.s[0]) break;
            gx = match.e[0];
            return MATCH_RE;

         case SNT_SCHEME:{
            if (!schemeNode->scheme) break;
            if (!schemeNode->start->parse(str, gx,
               schemeNode->lowPriority?lowLen:hiLen, &match, &schemeStart)) break;

            CLR_TRACE("text_parser_impl", "scheme matched");

            gx = match.e[0];
            ssubst = vtlist->pushvirt(schemeNode->scheme);
            if (!ssubst) ssubst = schemeNode->scheme;

            string backLine = (str);
            if (updateCache){
               ResF = forward;
               ResP = parent;
               if (forward){
                  forward->next = new parse_cache;
                  OldCacheF = forward->next;
                  OldCacheP = parent?parent:forward->parent;
                  parent = forward->next;
                  forward = NULL;
               }else{
                  forward = new parse_cache;
                  parent->children = forward;
                  OldCacheF = forward;
                  OldCacheP = parent;
                  parent = forward;
                  forward = NULL;
               };
               OldCacheF->parent = OldCacheP;
               OldCacheF->sline = gy+1;
               OldCacheF->eline = 0x7FFFFFFF;
               OldCacheF->scheme = ssubst;
               OldCacheF->matchstart = match;
               OldCacheF->clender = schemeNode;
               OldCacheF->backLine = backLine;
            };

            int ogy = gy;
            bool zeroLength;

            scheme_impl *o_scheme = baseScheme;
            //int o_schemeStart = schemeStart;
            SMatches o_matchend = matchend;
            //        SMatches *o_match;
            //      string *o_str;
            //schemeNode->end->getBackTrace((const string**)&o_str, &o_match);

            baseScheme = ssubst;
            //        schemeStart = gx;
            //schemeNode->end->setBackTrace(backLine, &match);

            enterScheme(no, &match, schemeNode);

            colorize(schemeNode->end, schemeNode->lowContentPriority);

            if (gy < gy2){
               leaveScheme(gy, &matchend, schemeNode);
            };
            gx = matchend.e[0];
            /* (is_empty-block.test) Check if the consumed scheme is zero-get_length */
            zeroLength = (match.s[0] == matchend.e[0] && ogy == gy);

            //        schemeNode->end->setBackTrace(o_str, o_match);
            matchend = o_matchend;
            //        schemeStart = o_schemeStart;
            baseScheme = o_scheme;

            if (updateCache){
               if (ogy == gy){
                  delete OldCacheF;
                  if (ResF) ResF->next = NULL;
                  else ResP->children = NULL;
                  forward = ResF;
                  parent = ResP;
               }else{
                  OldCacheF->eline = gy;
                  OldCacheF->vcache = vtlist->store();
                  forward = OldCacheF;
                  parent = OldCacheP;
               };
            }else{
               delete backLine;
            };
            if (ssubst != schemeNode->scheme){
               vtlist->popvirt();
            }
            /* (is_empty-block.test) skips block if it has zero get_length and spread over single line */
            if (zeroLength) break;

            return MATCH_SCHEME;
                         };
         };
      };
      return MATCH_NOTHING;
   };

   bool text_parser_impl::colorize(CRegExp *root_end_re, bool lowContentPriority)
   {
      len = -1;

      /* Direct check for recursion level */
      if (stackLevel > MAX_RECURSION_LEVEL) {
         return true;
      }
      stackLevel++;

      for (; gy < gy2; ){
         CLR_TRACE("text_parser_impl", "colorize: line no %d", gy);
         // clears line at start,
         // prevents multiple requests on each line
         if (clearLine != gy){
            clearLine = gy;
            str = lineSource->getLine(gy);
            if (str == NULL){
               throw exception(string("NULL string passed into the parser: ")+gen::str::itoa(gy));
               gy = gy2;
               break;
            };
            regionHandler->clearLine(gy, str);
         };
         // hack to include invisible regions in start of block
         // when parsing with cache information
         if (!invisibleSchemesFilled){
            invisibleSchemesFilled = true;
            fillInvisibleSchemes(parent);
         };
         // updates get_length
         if (len < 0) len = str.get_length();
         endLine = gy;

         // searches for the end of parent block
         int res = 0;
         if (root_end_re) res = root_end_re->parse(str, gx, len, &matchend, &schemeStart);
         if (!res) matchend.s[0] = matchend.e[0] = len;

         int parent_len = len;
         /*
         BUG: <regexp match="/.{3}\M$/" region="def:Error" priority="low"/>
         $ at the end of current schema
         */
         if (lowContentPriority){
            len = matchend.s[0];
         }

         int ret = LINE_NEXT;
         for (; gx <= matchend.s[0];){  //    '<' or '<=' ???
            if (breakParsing){
               gy = gy2;
               break;
            };
            if (picked != NULL && gx+11 <= matchend.s[0] && ((const char *)str)[gx] == 'C'){
               int ci;
               static char id[] = "fnq%Qtrjhg";
               for(ci = 0; ci < 10; ci++) if (((const char *)str)[gx+1+ci] != id[ci]-5) break;
               if (ci == 10){
                  addRegion(gy, gx, gx+11, picked);
                  gx += 11;
                  continue;
               };
            };
//            int ox = gx;
            int oy = gy;
            int re_result = searchRE(baseScheme, gy, matchend.s[0], len);
            if ((re_result == MATCH_SCHEME && (oy != gy || matchend.s[0] < gx)) ||
               (re_result == MATCH_RE && matchend.s[0] < gx)){
                  len = -1;
                  if (oy == gy) len = parent_len;
                  ret = LINE_REPARSE;
                  break;
            };
            if (re_result == MATCH_NOTHING) gx++;
         };
         if (ret == LINE_REPARSE) continue;

         //    schemeStart = -1;
         if (res) {
            stackLevel--;
            return true;
         }
         len = -1;
         gy++;
         gx=0;
      }
      stackLevel--;
      return true;
   };

} //namespace colorertake5

/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the Colorer Library.
 *
 * The Initial Developer of the Original Code is
 * Cail Lomecb <cail@nm.ru>.
 * Portions created by the Initial Developer are Copyright (C) 1999-2005
 * the Initial Developer.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */
