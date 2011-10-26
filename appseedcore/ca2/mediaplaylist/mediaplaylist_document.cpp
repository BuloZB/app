#include "StdAfx.h"

namespace mediaplaylist
{

   const LPARAM document::LHINT_FRAMEDOWN    = 1001;
   const LPARAM document::LHINT_FRAMEUP      = 1002;

   info::info()
   {
      m_pdocument       = NULL;
      m_bUser           = false;
      m_iCurrentSong    = -1;
      m_ehint           = hint_play_if_not_playing;
      m_bMakeVisible    = true;
   }

   info::~info()
   {
   }


   document::document(::ca::application * papp) :
      ca(papp),
      data_container(papp),
      ::document(papp),
      ::userbase::document(papp),
      m_xmlnode(papp)
   {
      m_xmlnode.m_strName = "playlist";

      m_pnodePlaylist   = NULL;
      m_pnodeCurrent    = NULL;

      ::mediaplaylist::callback_container_interface * pplaylistcallbackcontainerinterface = dynamic_cast < ::mediaplaylist::callback_container_interface * > (&System);
      if(pplaylistcallbackcontainerinterface != NULL)
      {
         m_xpcallback = pplaylistcallbackcontainerinterface->GetPlaylistCallback();
         if(m_xpcallback != NULL)
         {
            m_xpcallback->m_pplaylistdoc = this;
         }
      }
      else
      {
         m_xpcallback = NULL;
      }
   }

   BOOL document::on_new_document()
   {
      if (!::userbase::document::on_new_document())
         return FALSE;
      return TRUE;
   }

   document::~document()
   {
   }

   void document::on_final_release()
   {
      // When the last reference for an automation object is released
      // on_final_release is called.  The base class will automatically
      // deletes the object.  add additional cleanup required for your
      // object before calling the base class.

      ::userbase::document::on_final_release();
   }


   #ifdef _DEBUG
   void document::assert_valid() const
   {
      ::userbase::document::assert_valid();
   }

   void document::dump(dump_context & dumpcontext) const
   {
      ::userbase::document::dump(dumpcontext);
   }
   #endif //_DEBUG

   /////////////////////////////////////////////////////////////////////////////
   // document serialization

   void document::write(ex1::byte_output_stream & outputstream)
   {
      outputstream << m_xmlnode;
   }

   void document::read(ex1::byte_input_stream & inputstream)
   {
      inputstream >> m_xmlnode;
   }

   bool document::has_next_song()
   {
      if(get_current_playlist_song_count() <= 0)
         return false;
      if(is_loop())
         return true;
      if(get_current_queue_song_count() <= 0)
         return false;
      return true;
   }

   bool document::get_songs(stringa & stra)
   {

      if(m_pnodePlaylist == NULL)
         return false;

      m_pnodePlaylist->get_child_attr_value(stra, "song", "path", is_recursive() ? -1 : 1);

      return true;
   }

   bool document::get_song_current_queue(stringa & stra)
   {
      stringa straTotal;
      get_songs(straTotal);
      for(int i = 0; i < m_iaQueue.get_count(); i++)
      {
         stra.add(straTotal[m_iaQueue[i]]);
      }
      return true;
   }

   int document::get_current_song()
   {
      return m_iCurrentSong;
   }

   bool document::play(info * pinfo)
   {
      string str;


      if(m_xpcallback == NULL)
      {
         m_xpcallback = mediaplaylist::central_container::AppGetPlaylistAppInterface(get_app()).GetNewPlaylistCallback();
      }

      if(pinfo->m_bUser
      || pinfo->m_ehint == mediaplaylist::hint_do_play
      || (pinfo->m_ehint == mediaplaylist::hint_play_if_not_playing && (m_xpcallback == NULL || !m_xpcallback->is_playing())))
      {
         populate_queue(pinfo->m_iCurrentSong);
      }

      int iSong = -1;

      if(m_xpcallback != NULL)
      {
         e_play eplay;
         while(true)
         {
            if(!pick_a_song(iSong, str))
               return false;
            if(m_setFile[str].is_empty())
               m_setFile[str] = str;
            pinfo->m_strFilePath = m_setFile[str];
            pinfo->m_pdocument = this;
            eplay = m_xpcallback->PlaylistPlay(pinfo);
            if(eplay == play_success)
            {
               set_string("current_playlist", m_xmlnode.get_child_simple_attr_path(m_pnodePlaylist, "name"));
               set_string("current_node", m_pnodePlaylist->get_child_simple_attr_path(m_pnodeCurrent, "name"));
               set_int("current_song", iSong);
               set_int("current_song_position", 0);
               if(IsDefaultPlaylist())
               {
                  DoDefaultPlaylistSave();
               }
               return true;
            }
            else if(eplay != play_error_continue)
            {
               return false;
            }
         }
      }
      return false;
   }

   void document::restart(bool bUser)
   {
      info info;

      string strCurrentPlaylist = get_string("current_playlist", "default");

      if(strCurrentPlaylist.is_empty())
         return;

      m_pnodePlaylist = m_xmlnode.get_node_from_attr_path(strCurrentPlaylist, "playlist", "name");

      if(m_pnodePlaylist == NULL)
         return;

      string strCurrentNode = get_string("current_node", "");

      m_pnodeCurrent = m_pnodePlaylist->get_node_from_attr_path(strCurrentNode, "playlist", "name");

      if(m_pnodeCurrent == NULL)
         return;

      info.m_iCurrentSong        = get_int("current_song", 0);
      info.m_position.m_number   = get_int("current_song_position", 0);

      if(info.m_iCurrentSong < 0)
      {
         info.m_iCurrentSong     = 0;
         info.m_position         = 0;
      }

      if(info.m_iCurrentSong >= get_song_count())
      {
         info.m_iCurrentSong     = 0;
         info.m_position         = 0;
      }

      if(info.m_iCurrentSong >= get_song_count())
         return;

      info.m_ehint               = hint_play_if_not_playing;
      info.m_bMakeVisible        = true;
      info.m_bUser               = bUser;

      play(&info);

      update_all_views(NULL, 1);

   }

   void document::OnPlay()
   {
      info info;
      info.m_iCurrentSong     = get_current_song();
      info.m_ehint            = hint_do_play;
      info.m_bMakeVisible     = true;
      play(&info);
   }

   VMSRESULT document::AddSong(LPINT lpiaSongs, int iSongCount, bool bDoPlay, bool bMakeVisible)
   {

      bool bFirst = bDoPlay;

      for(int i = 0; i < iSongCount; i++)
      {
         AddSong(lpiaSongs[i], bFirst, bMakeVisible, false);
         //        if(pSongsSet->FindCode(lpiaSongs[i]))
         //      {
         //        pSongsSet->GetData(3);
         //      string str(pSongsSet->m_bstrPath);
         //    AddSong(str, bFirst, bMakeVisible, false);
         //  bFirst = false;
         //}
      }
      update_all_views(NULL);

      OnAddSong();

      return VMSR_SUCCESS;
   }

   bool document::AddSong(var varFile, EAddSong eaddsong, bool bMakeVisible, bool bUpdateAllViews)
   {
      UNREFERENCED_PARAMETER(bUpdateAllViews);
      string strPathName;
      if(varFile.get_type() == var::type_propset && varFile.propset()["url"].get_string().has_char())
      {
         strPathName = varFile.propset()["url"];
      }
      else if(varFile.ca2 < ::ex1::file > () != NULL)
      {
         strPathName = System.datetime().international().get_gmt_date_time() + "." + get_document_template()->m_set["default_extension"];
      }
      else
      {
         strPathName = varFile;
      }
      m_setFile[strPathName] = varFile;

      ensure_playlist();

      xml::node * pnodeSong = new xml::node(get_app());
      pnodeSong->m_strName = "song";
      pnodeSong->add_attr("path", strPathName);
      m_pnodeCurrent->add_child(pnodeSong);
      switch(eaddsong)
      {
      case AddSongOnly:
         break;
      case AddSongAndPlay:
      case AddSongAndPlayIfNotPlaying:
         {
            info info;
            info.m_iCurrentSong  = m_pnodeCurrent->get_children_count("song") - 1;
            info.m_pdocument     = this;
            info.m_bMakeVisible  = bMakeVisible;
            switch(eaddsong)
            {
            case AddSongAndPlay:
               info.m_ehint      = mediaplaylist::hint_do_play;
               break;
            case AddSongAndPlayIfNotPlaying:
               info.m_ehint      = mediaplaylist::hint_play_if_not_playing;
               break;
            }

            play(&info);

         }
         break;
      default:
         ASSERT(FALSE);
         return false;
      }
      update_all_views(NULL, 1);
      OnAddSong();
      return VMSR_SUCCESS;
   }

   VMSRESULT document::AddSong(int nSong, bool bDoPlay, bool bMakeVisible, bool bUpdateAllViews)
   {
      UNREFERENCED_PARAMETER(nSong);
      UNREFERENCED_PARAMETER(bDoPlay);
      UNREFERENCED_PARAMETER(bMakeVisible);
      UNREFERENCED_PARAMETER(bUpdateAllViews);
      return VMSR_E_FAIL;
      //   CVMSApp * pApp = (CVMSApp *) &System;
      //   ASSERT(pApp != NULL);
      //  db_server* pDataCentral = db();
      //ASSERT(pDataCentral != NULL);
      //   CDBSongSet * pSongsSet = pDataCentral->GetSongsSet();
      //ASSERT(pSongsSet != NULL);

      //    if(!pSongsSet->FindCode(nSong))
      //      return VMSR_E_FAIL;

      //pSongsSet->GetData(3);
      //   string str(pSongsSet->m_bstrPath);
      //   return AddSong(str, bDoPlay, bMakeVisible, bUpdateAllViews);


   }

   VMSRESULT document::AddSong(stringa  & stra, bool bDoPlay, bool bMakeVisible)
   {
      //DBCentralInterface * pDataCentral = db();
      //ASSERT(pDataCentral != NULL);

      bool bFirst = bDoPlay;

      for(int i = 0; i < stra.get_size(); i++)
      {
         AddSong(stra.element_at(i), bFirst ? (bDoPlay ? AddSongAndPlay : AddSongOnly) : AddSongOnly, bMakeVisible, false);
         //        if(pSongsSet->FindCode(lpiaSongs[i]))
         //      {
         //        pSongsSet->GetData(3);
         //      string str(pSongsSet->m_bstrPath);
         //    AddSong(str, bFirst, bMakeVisible, false);
         //  bFirst = false;
         //}
      }
      update_all_views(NULL);

      OnAddSong();

      return VMSR_SUCCESS;

   }

   bool document::on_open_document(var varFile)
   {
      if (!::userbase::document::on_open_document(varFile))
         return FALSE;
      ensure_playlist();
      update_all_views(NULL,  0, NULL);
      return TRUE;
   }

   void document::on_close_document()
   {
      if(IsDefaultPath())
      {
         mediaplaylist::central::AppGetPlaylistCentral(get_app()).OnCloseDefaultPlaylist();
      }
      ::userbase::document::on_close_document();
   }

   BOOL document::save_modified()
   {
      mediaplaylist::central & playlistcentral = mediaplaylist::central::AppGetPlaylistCentral(get_app());
      string wstrDefault;
      playlistcentral.GetDefaultPath(wstrDefault);
      if(IsDefaultPath())
         return do_save(wstrDefault, TRUE);
      return ::userbase::document::save_modified();
   }

   void document::update_title()
   {
      mediaplaylist::central & playlistcentral = mediaplaylist::central::AppGetPlaylistCentral(get_app());
      string wstrDefault;
      playlistcentral.GetDefaultPath(wstrDefault);
      if(IsDefaultPath())
      {
         string str;
         //      str.load_string(IDS_DEFAULT_PLAYLIST_TITLE);
         set_title(str);
      }
   }

   /*bool document::Play(bool bDoPlay, int iOffset, bool bLoop)
   {

      string wstrPath;
      //   bool bOpen = false;
      int iSong = m_iCurrentSong;
      int iNextSong;
      while(PeekSongPath(wstrPath, iOffset, bLoop))
      {
         iNextSong = m_iCurrentSong;
         if(iNextSong == iSong)
            break;
         try
         {
            info info;

            info.m_iCurrentSong  = iNextSong;
            info.m_ehint         = bDoPlay ? mediaplaylist::SetCurrentSongHintDoPlay : mediaplaylist::SetCurrentSongHintPlayIfNotPlaying;
            info.m_bMakeVisible  = true;
            info.m_position      = 0;

            SetCurrentSong(&info);

         }
         catch(...)
         {
            continue;
         }
         break;
      }

      return false;


   }*/

   int document::get_song_count()
   {
      stringa stra;
      get_songs(stra);
      return stra.get_size();
   }

   bool document::get_song_at(string & str, int iSong)
   {

      stringa stra;

      get_songs(stra);

      if(stra.get_size() <= 0)
         return false;

      if(iSong < 0)
         return false;

      if(iSong >= stra.get_size())
         return false;

      str = stra[iSong];

      return true;
   }

   void document::populate_queue(int iSong)
   {
      stringa stra;

      get_songs(stra);

      if(iSong < 0)
           iSong = 0;
      if(iSong >= stra.get_size())
         iSong = 0;
      if(iSong >= stra.get_size())
         return;
      m_iaQueue.remove_all();
      gen::lemon::array a(get_app());
      bool bShuffle = false;
      if(m_xmlnode.get_attr("shuffle", bShuffle) && bShuffle)
         iSong = 0;
      a.populate(m_iaQueue, iSong, (int) stra.get_upper_bound(), 1, true, true, true);
      if(bShuffle)
      {
         if(m_iaQueue.contains(iSong))
         {
            // maintains iSong the first song in queue;
            m_iaQueue.remove(iSong);
            a.randomize(m_iaQueue);
            m_iaQueue.insert_at(0, iSong);
         }
         else
         {
            a.randomize(m_iaQueue);
         }
      }
   }

   bool document::pick_a_song(int & iSong, string & str)
   {

      stringa stra;

      get_songs(stra);

      if(stra.get_count() <= 0)
         return false;

      if(m_iaQueue.get_count() <= 0)
      {
         bool bLoop = true;
         if(m_xmlnode.get_attr("loop", bLoop) && bLoop)
            return false;
         populate_queue(0);
      }

      if(m_iaQueue.get_size() <= 0)
         return false;

      iSong = m_iaQueue[0];

      m_iCurrentSong = iSong;

      m_iaQueue.remove_at(0);

      str = stra[iSong];

      return true;

   }

   void document::set_flag(const char * psz, bool bFlag)
   {
      m_xmlnode.set_attr(psz, bFlag);
   }

   bool document::get_flag(const char * psz, bool bDefault)
   {
      bool bFlag;
      if(!m_xmlnode.get_attr(psz, bFlag))
         return bDefault;
      return bFlag;
   }

   void document::set_int(const char * psz, int iValue)
   {
      m_xmlnode.set_attr(psz, iValue);
   }

   int document::get_int(const char * psz, int iDefault)
   {
      bool iValue;
      if(!m_xmlnode.get_attr(psz, iValue))
         return iDefault;
      return iValue;
   }

   void document::set_string(const char * psz, const char * pszValue)
   {
      m_xmlnode.set_attr(psz, pszValue);
   }

   string document::get_string(const char * psz, const char * pszDefault)
   {
      string strValue;
      if(!m_xmlnode.get_attr(psz, strValue))
         return pszDefault;
      return strValue;
   }

   bool document::RemoveSong(int iSong, ::userbase::view * pview)
   {

      if(iSong < 0)
         return false;

      if(iSong >= get_song_count())
         return false;


      xml::node * pnode = m_pnodeCurrent->get_child_at("song", iSong, 1);

      m_pnodeCurrent->remove_child(pnode);

      OnRemoveSong(pview);


      return true;
   }


   void document::OnAddSong()
   {

      OnChangeSongs();

   }

   void document::OnChangeSongs(::userbase::view * pview)
   {
      UNREFERENCED_PARAMETER(pview);
      if(IsDefaultPlaylist())
      {
         DoDefaultPlaylistSave();
      }

      update_all_views(NULL);

   }

   void document::DoDefaultPlaylistSave()
   {
      if(IsDefaultPlaylist())
      {
         do_save(get_path_name(), TRUE);
      }
   }

   bool document::IsDefaultPlaylist()
   {
      return IsDefaultPath();
   }

   void document::OnRemoveSong(::userbase::view * pview)
   {
      OnChangeSongs(pview);
   }


   bool document::IsDefaultPath(const char * lpcsz)
   {
      mediaplaylist::central & playlistcentral = mediaplaylist::central::AppGetPlaylistCentral(get_app());
      string strDefault;
      playlistcentral.GetDefaultPath(strDefault);
      return System.file().path().is_equal(strDefault, lpcsz);
   }

   bool document::IsDefaultPath()
   {
      return IsDefaultPath(get_path_name());
   }

   ::userbase::view * document::GetFirstAttachableView()
   {
      PlaylistViewUpdateHint uh;
      uh.SetHint(PlaylistViewUpdateHint::HintGetAttachableView);
      update_all_views(NULL, 0, &uh);
      base_array < ::userbase::view *, ::userbase::view * > viewpa;
      uh.GetAttachableViews(viewpa);
      if(viewpa.get_size() > 0)
         return viewpa[0];
      else
         return NULL;

   }

   void document::PopViews()
   {
      PlaylistViewUpdateHint uh;
      uh.SetHint(PlaylistViewUpdateHint::HintPop);
      update_all_views(NULL, 0, &uh);

   }

   void document::SetCallback(mediaplaylist::callback_interface *pcallback)
   {
      m_xpcallback = pcallback;
   }


   void document::set_current_playlist(const char * pszPath)
   {
      m_xmlnode.m_strName = "playlist";
      m_xmlnode.set_attr("current_playlist", pszPath);
   }

   string document::get_current_playlist_path()
   {
      string str;
      m_xmlnode.get_attr("current_playlist", str);
      return str;
   }

   string document::get_default_playlist_path()
   {
      return "playlist/default";
   }


   int document::get_current_playlist_song_count()
   {
      return m_iCurrentPlaylistSongCount;
   }

   int document::get_current_queue_song_count()
   {
      return m_straQueue.get_count();
   }

   bool document::is_recursive()
   {
      return get_flag("recursive", true);
   }

   bool document::is_shuffle()
   {
      return get_flag("shuffle", false);
   }

   bool document::is_loop()
   {
      return get_flag("loop", false);
   }

   void document::ensure_playlist()
   {
      if(m_pnodeCurrent == NULL || m_pnodePlaylist == NULL)
      {
         if(m_pnodePlaylist == NULL)
         {
            m_pnodePlaylist = m_xmlnode.GetChildByAttr("playlist", "name", "default");
            if(m_pnodePlaylist == NULL)
            {
               m_pnodePlaylist = new xml::node(get_app());
               m_pnodePlaylist->m_strName = "playlist";
               m_pnodePlaylist->add_attr("name", "default");
               m_xmlnode.add_child(m_pnodePlaylist);
            }
         }
         m_pnodeCurrent = m_pnodePlaylist;
      }
   }


} // namespace mediaplaylist


