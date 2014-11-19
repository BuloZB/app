#include "framework.h" // from "axis/user/user.h"


namespace user
{


   menu_base_item::menu_base_item(sp(::aura::application) papp) :
      element(papp)
   {

   }


   menu_base_item::~menu_base_item()
   {

   }


   bool menu_base_item::create_menu(const stringa & straCommand, const stringa & straCommandTitle)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }

   bool menu_base_item::load_menu(sp(::xml::node) lpnode)
   {

      ::exception::throw_interface_only(get_app());

      return false;

   }


} // namespace user




