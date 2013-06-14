#pragma once


class CLASS_DECL_ca2 hresult_exception : 
   virtual public base_exception
{
public:
   
   
   HRESULT     m_hresult;


   hresult_exception(sp(::ca2::application) papp, HRESULT hr);
   virtual ~hresult_exception();


   operator HRESULT () const
   {
      return m_hresult;
   }

};
