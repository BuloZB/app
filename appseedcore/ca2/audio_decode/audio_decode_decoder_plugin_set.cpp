#include "StdAfx.h"

namespace audio_decode
{

   decoder_plugin_set::decoder_plugin_set(::ca::application * papp)
      : ca(papp)
   {
   }

   decoder_plugin_set::~decoder_plugin_set(void)
   {
   }

   decoder_plugin * decoder_plugin_set::FindPlugin(const char * pszTitle)
   {
      for(int i = 0; i < this->get_size(); i++)
      {
         if(this->element_at(i).GetPluginTitle().CompareNoCase(pszTitle) == 0)
            return &this->element_at(i);
      }
      return NULL;
   }

   decoder_plugin * decoder_plugin_set::LoadPlugin(const char * lpcszDll)
   {
      decoder_plugin * pplugin = FindPlugin(lpcszDll);
      if(pplugin != NULL)
         return pplugin;
      pplugin = new decoder_plugin(get_app());
      if(!pplugin->Load(lpcszDll))
      {
         delete pplugin;
         return NULL;
      }
      add(pplugin);
      return pplugin;
   }

} // namespace audio_decode
