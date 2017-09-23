#pragma once


namespace user
{


   enum e_appearance
   {

      appearance_none,
      appearance_current,
      appearance_iconic,
      appearance_normal,
      appearance_left,
      appearance_top,
      appearance_right,
      appearance_bottom,
      appearance_top_left,
      appearance_top_right,
      appearance_bottom_left,
      appearance_bottom_right,
      appearance_zoomed,
      appearance_full_screen,
      appearance_minimal,
      appearance_notify_icon,
      appearance_up,
      appearance_down,
      appearance_fixed_size,


   };


   enum e_translucency
   {

      translucency_undefined,
      translucency_none,
      translucency_present,
      translucency_total

   };

   enum e_control_type
   {
      control_type_none,
      control_type_static,
      control_type_check_box,
      control_type_edit,
      control_type_simple_list,
      control_type_button,
      control_type_combo_box,
      control_type_edit_plain_text,
      control_type_list,
      control_type_mesh,
      control_type_tree,
      control_type_split,
      control_type_tab,
      control_type_toolbar,
      control_type_statusbar,
      control_type_menu
   };


} // namespace user










