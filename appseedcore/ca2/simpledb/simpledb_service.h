#pragma once


#ifdef WINDOWS


namespace simpledb
{

   class socket_thread;

   class CLASS_DECL_ca service :
      public plain_service
   {
   public:

      manager *            m_pmanager;
      base_array < socket_thread *, socket_thread * > m_threadptra;


      service(::ca::application * papp);
      virtual ~service();

      virtual void ServiceThread();


   };



} // namespace simpledb


#endif
