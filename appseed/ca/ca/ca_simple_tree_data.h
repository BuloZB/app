#pragma once


namespace ca
{


   class CLASS_DECL_ca simple_tree_data :
      virtual public tree_data
   {
   public:


      simple_tree_data(sp(::ca::application) papp);


      virtual sp(tree_item_data) on_allocate_item();
      virtual void on_delete_item(sp(tree_item_data) pitem);

   };



} // namespace ca



