#pragma once


namespace filehandler
{


   class CLASS_DECL_ca tree_data :
      virtual public ::ca::tree_data
   {
   public:


      tree_data(sp(::ca::application) papp);


      virtual sp(::ca::tree_item_data) on_allocate_item();
      virtual void on_delete_item(sp(::ca::tree_item_data) pitem);


   };


} // namespace filehandler






