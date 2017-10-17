#include "framework.h"


namespace user
{
   //key::key() :
   //   ::message::base(NULL)
   //{


   //}


   key::key(::aura::application * papp) :
      ::message::base(papp)
   {



   }


   key::key(const ::user::key & key) :
      ::message::base(key)
   {

      copy_this(key);

   }


   void key::copy_this(const key & key)
   {

      m_iCode = key.m_iCode;
      m_nChar = key.m_nChar;
      m_nScanCode = key.m_nScanCode;
      m_nRepCnt = key.m_nRepCnt;
      m_nFlags = key.m_nFlags;
      m_bExt = key.m_bExt;
      m_ekey = key.m_ekey;
      m_strText = key.m_strText;

   }



} // namespace user