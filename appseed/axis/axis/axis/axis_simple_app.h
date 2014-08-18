#pragma once


namespace base
{

   class CLASS_DECL_AXIS simple_app:
      virtual public ::base::system
   {
   public:


      //HINSTANCE                   m_hinstance;
      int32_t                       __argc;
      TCHAR **                      __targv;

      MESSAGE                       m_msg;


      simple_app();
      virtual ~simple_app();

      virtual int32_t main();

      virtual void body();

      virtual int32_t simple_app_pre_run();

      virtual bool intro();
      virtual int32_t refrain();
      virtual bool end();


   };


} // namespace base




