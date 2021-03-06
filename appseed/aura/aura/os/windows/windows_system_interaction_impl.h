#pragma once


#ifdef WINDOWSEX


namespace user
{

   // top level window for receiving system messages broadcasted to top level windows in Windows

   class CLASS_DECL_AURA system_interaction_impl :
      virtual public ::user::interaction
   {
      public:

         system_interaction_impl(::aura::application * papp);
         virtual ~system_interaction_impl();

         void install_message_routing(::message::sender * pdispath);

         DECL_GEN_SIGNAL(_001MessageHub);


   };


} // namespace base


#endif


