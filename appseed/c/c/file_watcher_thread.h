#pragma once


namespace file_watcher
{


   class CLASS_DECL_c thread :
      public file_watcher,
      public thread_layer
   {
   public:

         
      virtual int run();

         
   };


} // namespace file_watcher



