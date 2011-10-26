#include "StdAfx.h"
#include <memory.h>





//extern CXfplayerApp theApp;
const char Star350File::m_lpchHeader[] =
    {'S',
     'T',
     'A',
     'R',
     ' ',
     'D',
     'A',
     'T',
     'A',
     ' ',
     'V',
     '3',
     '.',
     '5',
     '0',
     0x1A,
     0x11,
     0x01,
     0x00,
     0x00};

void Star350File::delete_contents()
{
//   m_pPendingLyricEventV1 = NULL;
//    m_meLyricPending
//   m_pendingLyricEventArray.remove_all();
   m_bPendingLyric = false;
   m_strLyrics.Empty();
   try
   {
//      if(m_ptracks->get_size())
//      {
//         for(int i = m_ptracks->get_size(); i--;)
//         {
//            delete m_tracks[i];
//            m_ptracks->remove_at(i);
//         }
//      }
//      if(m_tempoMaps.get_size())
//      {
//         for(int i = m_tempoMaps.get_size(); i--;)
//         {
//            m_tempoMaps.remove_at(i);
//         }
//      }
   }
   catch(...)
   {
        return;
   }
//    m_meaOnQuarterNote.remove_all();

    while(m_ptracks->get_size() > 0)
    {
        Star350TrackBase * pTrack = m_ptracks->element_at(0);
        if(pTrack != NULL)
        {
            if(base < Star350LyricTrack >::bases(pTrack))
                delete (Star350LyricTrack *) pTrack;
            else if(base < Star350EventTrack >::bases(pTrack))
                delete (Star350EventTrack *) pTrack;
        }
        m_ptracks->remove_at(0);
    }
//   for(int i = 0; i < m_eventsTracksForScoring.get_size(); i++)
//   {
//      delete m_eventsTracksForScoring[i];
//   }
//   m_eventsTracksForScoring.remove_all();
//   for(i = 0; i < m_eventsTracksForPositionCB.get_size(); i++)
//   {
//      delete m_eventsTracksForPositionCB[i];
//   }
//   m_eventsTracksForPositionCB.remove_all();

}

SMFRESULT Star350File::OpenFile(
    Star350File &file,
   int openMode)
{
    SMFRESULT               smfrc = Success;

    if(IsOpened())
    {
        CloseFile();
    }

   m_ptracks->m_iMelodyTrackTipA = -1;

   m_iOpenMode = openMode;

    m_fdwSMF = 0;

/*    m_cbImage = spfile->m_cbImage;
   if(!allocate(m_cbImage))
   {
      TRACE( "smfOpenFile: No primitive::memory for image! [%08lX]", m_cbImage);
        smfrc = NoMemory;
        goto smf_Open_File_Cleanup;
   }

    memcpy(GetImage(), spfile->GetImage(), m_cbImage); */

    DWORD cbImage = file.GetImageSize();
   try
   {
      allocate(cbImage);
   }
   catch(memory_exception * pe)
   {
      TRACE( "smfOpenFile: No primitive::memory for image! [%08lX]", cbImage);
        smfrc = NoMemory;
      pe->Delete();
        goto smf_Open_File_Cleanup;
   }

    LPBYTE lpbImage;

    lpbImage = GetImage();

    memcpy(lpbImage, file.GetImage(), cbImage);

    /* If the file exists, parse it just enough to pull out the header and
    ** build a track index.
    */
    smfrc = BuildIndex();
   SetOpened();
//    if (MMSYSERR_NOERROR != smfrc)
  //  {
    //    TRACE( "smfOpenFile: smfBuildFileIndex failed! [%lu]", (DWORD)smfrc);
//    }

smf_Open_File_Cleanup:
//   mmfile.close();
  //  mmioClose(hmmio, 0);

    if (Success != smfrc)
    {
        //if (NULL != GetImage())
        //{
         //HeapFree(GetProcessHeap(), 0, GetImage());
      //   GetImage() = NULL;
        //}
    }

    return smfrc;
}

SMFRESULT Star350File::OpenFile(
   primitive::memory * pmemorystorage,
   int openMode,
   mus::EStorage estorage)
{
   ASSERT(pmemorystorage != NULL);

   if(IsOpened())
   {
      CloseFile();
   }

   SMFRESULT               smfrc = Success;

   m_ptracks->m_iMelodyTrackTipA = -1;

   m_iOpenMode = openMode;

   m_fdwSMF = 0;


   if(estorage == mus::StorageCopy)
   {
      FullLoad(pmemorystorage);
   }
   else if(estorage == mus::StorageAttach)
   {
      if(m_estorage == mus::StorageAttach)
      {
         detach();
      }
      else if(m_estorage == mus::StorageOwn)
      {
         delete detach();
      }
      Attach(pmemorystorage);
   }
   else if(estorage == mus::StorageOwn)
   {
      if(m_estorage == mus::StorageAttach)
      {
         detach();
      }
      FullLoad(pmemorystorage);
   }


   m_estorage = estorage;

    /* If the file exists, parse it just enough to pull out the header and
    ** build a track index.
    */
    smfrc = BuildIndex();

    if (Success != smfrc)
    {
        //if (NULL != m_hpbImage)
        //{
         //HeapFree(GetProcessHeap(), 0, m_hpbImage);
      //   m_hpbImage = NULL;
        //}
        CloseFile();
    }
    else
    {
          SetOpened();
    }

    return smfrc;
}

/*****************************************************************************
*
* smfOpenFile
*
* This function opens a MIDI file for access.
*
* psofs                     - Specifies the file to open and associated
*                             parameters. Contains a valid HSMF handle
*                             on success.
*
* Returns
*   Success The specified file was opened.
*
*   SMF_OPEN_FAILED The specified file could not be opened because it
*     did not exist or could not be created on the disk.
*
*   InvalidFile The specified file was corrupt or not a MIDI file.
*
*   NoMemory There was insufficient primitive::memory to open the file.
*
*   SMF_INVALID_PARM The given flags or time division in the
*     SMFOPENFILESTRUCT were invalid.
*
*****************************************************************************/
SMFRESULT Star350File::OpenFile(
   //const char * lpcFilePath,
    ex1::filesp &ar,
   int openMode)
{
//    HMMIO                   hmmio = (HMMIO)NULL;
//   multimedia_file         mmfile;
    SMFRESULT               smfrc = Success;
//    MMIOINFO                mmioinfo;
//    MMCKINFO                ckRIFF;
    //MMCKINFO                ckDATA;
//   DWORD               dwTemp;

    if(IsOpened())
    {
        CloseFile();
    }

   m_ptracks->m_iMelodyTrackTipA = -1;

   m_iOpenMode = openMode;

//   ASSERT(lpcFilePath != NULL);

//   LPTSTR lpFilePath = _tcsdup(lpcFilePath);

    /* Verify that the file can be opened or created
    */
//    memset(&mmioinfo, 0, sizeof(mmioinfo));

//   hmmio = mmioOpen(lpFilePath, &mmioinfo, MMIO_READ|MMIO_ALLOCBUF);
//   m_strName = lpcFilePath;
//   try
//   {
//      ex1::file_exception_sp * pfe = new ex1::file_exception_sp();
//      if(!mmfile.open(m_strName, ::ex1::file::mode_read | ::ex1::file::type_binary, pfe))
//         throw pfe;
//      else
//         pfe->Delete();
//   }
//   catch(ex1::file_exception_sp *pe)
//   {
//      pe->Delete();
//      return OpenFailed;
//   }
//    if ((HMMIO)NULL == hmmio)
  //  {
    //    TRACE(_T( "smfOpenFile: mmioOpen failed!"));
//      return OpenFailed;
//    }

    /* Now see if we can create the handle structure
    */

//   lstrcpy(m_szName, lpFilePath);

    m_fdwSMF = 0;

    /* Pull the entire file into a block of primitive::memory.
    */
//    memset(&ckRIFF, 0, sizeof(ckRIFF));

 /*   if (0 == mmioDescend(hmmio, &ckRIFF, NULL, MMIO_FINDRIFF) &&
        ckRIFF.fccType == FOURCC_RMID)
    {
        ckDATA.ckid = FOURCC_data;

        if (0 == mmioDescend(hmmio, &ckDATA, &ckRIFF, MMIO_FINDCHUNK))
        {
            m_cbImage   = ckDATA.cksize;
           if (0 == mmioDescend(hmmio, &ckDATA, &ckRIFF, MMIO_FINDCHUNK))
         {
            m_cbImage   = ckDATA.cksize;
         }
         if (0 == mmioDescend(hmmio, &ckDATA, &ckRIFF, MMIO_FINDCHUNK))
         {
            m_cbImage   = ckDATA.cksize;
         }
        }
        else
        {
            TRACE( "smfOpenFile: Could not descend into RIFF DATA chunk!");
            smfrc = InvalidFile;
            goto smf_Open_File_Cleanup;
        }
    }
    else*/
    //{
        //mmioSeek(hmmio, 0L, SEEK_SET);



        //m_cbImage = mmioSeek(hmmio, 0L, SEEK_END);
        //mmioSeek(hmmio, 0L, SEEK_SET);
    //}

//   m_cbImage = mmfile.get_length();
//    ex1::filesp * pFile = ar.GetFile();
  //  m_cbImage = pFile->get_length();
   ///mmfile.seek_to_begin();
//    pFile->seek_to_begin();
//   if(!allocate(m_cbImage))
//   {
//      TRACE( "smfOpenFile: No primitive::memory for image! [%08lX]", m_cbImage);
  //      smfrc = NoMemory;
    //    goto smf_Open_File_Cleanup;
   //}

   //if (NULL == (GetImage() = (unsigned char *) HeapAlloc(GetProcessHeap(), 0, (unsigned long) m_cbImage)))
    //{
        //TRACE( "smfOpenFile: No primitive::memory for image! [%08lX]", m_cbImage);
        //smfrc = NoMemory;
        //goto smf_Open_File_Cleanup;
    //}

   //dwTemp = (DWORD)mmioRead(hmmio, (char *) GetImage(), m_cbImage);
//   try
//   {
//      dwTemp = pFile->read(GetImage(), m_cbImage);
//   }
//   catch(ex1::file_exception_sp *pe)
//   {
  //      TRACE0("smfOpenFile: read error on image!");
    //    smfrc = InvalidFile;
//#ifdef _DEBUG
  //      pe->dump(afxdump);
//#endif
//      pe->Delete();
  //      goto smf_Open_File_Cleanup;
   //}

    FullLoad(ar);

//    if (m_cbImage != dwTemp)
  //  {
    //    TRACE0("smfOpenFile: read error on image!");
      //  smfrc = InvalidFile;
//        goto smf_Open_File_Cleanup;
  //  }

    /* If the file exists, parse it just enough to pull out the header and
    ** build a track index.
    */
    smfrc = BuildIndex();
   SetOpened();
//    if (MMSYSERR_NOERROR != smfrc)
  //  {
    //    TRACE( "smfOpenFile: smfBuildFileIndex failed! [%lu]", (DWORD)smfrc);
//    }

//smf_Open_File_Cleanup:
//   mmfile.close();
  //  mmioClose(hmmio, 0);

    if (Success != smfrc)
    {
        //if (NULL != GetImage())
        //{
         //HeapFree(GetProcessHeap(), 0, GetImage());
      //   GetImage() = NULL;
        //}
    }

    return smfrc;
}

/*****************************************************************************
*
* smfCloseFile
*
* This function closes an open MIDI file.
*
* hSmf                      - The handle of the open file to close.
*
* Returns
*   Success The specified file was closed.
*   SMF_INVALID_PARM The given handle was not valid.
*
* Any track handles opened from this file handle are invalid after this
* call.
*
*****************************************************************************/
SMFRESULT Star350File::CloseFile()
{

    single_lock(&m_cs, TRUE);
    delete_contents();
    SetOpened(false);

    return Success;
}

/******************************************************************************
*
* smfGetFileInfo This function gets information about the MIDI file.
*
* hSmf                      - Specifies the open MIDI file to inquire about.
*
* psfi                      - A structure which will be filled in with
*                             information about the file.
*
* Returns
*   Success Information was gotten about the file.
*   SMF_INVALID_PARM The given handle was invalid.
*
*****************************************************************************/
SMFRESULT Star350File::GetFileInfo(
    PSMFFILEINFO            psfi)
{
//    PSMF                    pSmf = (PSMF)hSmf;
//    assert(pSmf != NULL);
    ASSERT(psfi != NULL);

    /*
    ** Just fill in the structure with useful information.
    */
    psfi->dwTracks      = m_dwTracks;
   //psfi->dwTracks      = m_ptracks->get_size();
    psfi->dwFormat      = m_dwFormat;
    psfi->dwTimeDivision= m_dwTimeDivision;
    psfi->tkLength      = m_tkLength;

    return Success;
}

/******************************************************************************
*
* smfTicksToMillisecs
*
* This function returns the millisecond offset into the file given the
* tick offset.
*
* hSmf                      - Specifies the open MIDI file to perform
*                             the conversion on.
*
* tkOffset                  - Specifies the tick offset into the stream
*                             to convert.
*
* Returns the number of milliseconds from the start of the stream.
*
* The conversion is performed taking into account the file's time division and
* tempo ::collection::map from the first track. Note that the same millisecond value
* might not be valid at a later time if the tempo track is rewritten.
*
*****************************************************************************/
/*DWORD Star350File::TicksToMillisecs(
    imedia::position                   tkOffset)
{
   single_lock(&m_cs, true);
   if(!IsOpened())
      return 0x7fffffff;

//    PSMF                    pSmf            = (PSMF)hSmf;
    CMidiTempoMapEntry *         pTempo;
    int                    idx;
    UINT                    uSMPTE;
    DWORD                   dwTicksPerSec;

//    assert(pSmf != NULL);

    if (tkOffset > m_tkLength)
    {
        TRACE( "sTTM: Clipping ticks to file length!");
        tkOffset = m_tkLength;
    }

    /* SMPTE time is easy -- no tempo ::collection::map, just linear conversion
    ** Note that 30-Drop means nothing to us here since we're not
    ** converting to a colonized format, which is where dropping
    ** happens.
    */
  /*  if (m_dwTimeDivision & 0x8000)
    {
        uSMPTE = -(int)(char)((m_dwTimeDivision >> 8)&0xFF);
        if (29 == uSMPTE)
            uSMPTE = 30;

        dwTicksPerSec = (DWORD)uSMPTE *
                        (DWORD)(BYTE)(m_dwTimeDivision & 0xFF);

        return (DWORD)muldiv32(tkOffset, 1000L, dwTicksPerSec);
    }

    /* Walk the tempo ::collection::map and find the nearest tick position. Linearly
    ** calculate the rest (using MATH.ASM)
    */

    //pTempo = m_pTempoMap;
   //pTempo = m_tempoMaps.get_at(0);

    //assert(pTempo != NULL);

    //for (idx = 0; idx < m_cTempoMap; idx++, pTempo++)
//   for (idx = 0; idx < m_tempoMaps.get_size(); idx++)
//   {
//      pTempo = &m_tempoMaps.element_at(idx);
  //      if (tkOffset < pTempo->tkTempo)
    //        break;
   //}
   /*if(idx <= 0)
      pTempo = &m_tempoMaps.element_at(0);
   else
      pTempo = &m_tempoMaps.element_at(idx - 1);

    /* pTempo is the tempo ::collection::map entry preceding the requested tick offset.
    */

    /*return pTempo->msBase + muldiv32(tkOffset-pTempo->tkTempo,
                                     pTempo->dwTempo,
                                     1000L*m_dwTimeDivision);

}*/


/******************************************************************************
*
* smfMillisecsToTicks
*
* This function returns the nearest tick offset into the file given the
* millisecond offset.
*
* hSmf                      - Specifies the open MIDI file to perform the
*                             conversion on.
*
* msOffset                  - Specifies the millisecond offset into the stream
*                             to convert.
*
* Returns the number of ticks from the start of the stream.
*
* The conversion is performed taking into account the file's time division and
* tempo ::collection::map from the first track. Note that the same tick value
* might not be valid at a later time if the tempo track is rewritten.
* If the millisecond value does not exactly ::collection::map to a tick value, then
* the tick value will be rounded down.
*
*****************************************************************************/
/*imedia::position Star350File::MillisecsToTicks(
    DWORD                   msOffset)
{
   single_lock(&m_cs, true);
   if(!IsOpened())
      return 0x7fffffff;

//    PSMF                    pSmf            = (PSMF)hSmf;
    CMidiTempoMapEntry *    pTempo;
    int                      idx;
    UINT                    uSMPTE;
    DWORD                   dwTicksPerSec;
    imedia::position                   tkOffset;

//    assert(pSmf != NULL);

    /* SMPTE time is easy -- no tempo ::collection::map, just linear conversion
    ** Note that 30-Drop means nothing to us here since we're not
    ** converting to a colonized format, which is where dropping
    ** happens.
    */
  /*  if (m_dwTimeDivision & 0x8000)
    {
        uSMPTE = -(int)(char)((m_dwTimeDivision >> 8)&0xFF);
        if (29 == uSMPTE)
            uSMPTE = 30;

        dwTicksPerSec = (DWORD)uSMPTE *
                        (DWORD)(BYTE)(m_dwTimeDivision & 0xFF);

        return (DWORD)muldiv32(msOffset, dwTicksPerSec, 1000L);
    }

    /* Walk the tempo ::collection::map and find the nearest millisecond position. Linearly
    ** calculate the rest (using MATH.ASM)
    */
    //pTempo = m_pTempoMap;
   /*pTempo = &m_tempoMaps.element_at(0);
    ASSERT(pTempo != NULL);

    //for (idx = 0; idx < m_cTempoMap; idx++, pTempo++)
   for (idx = 0; idx < m_tempoMaps.get_size(); idx++)
   {
      pTempo = &m_tempoMaps.element_at(idx);
        if (msOffset < pTempo->msBase)
            break;
   }
    pTempo = &m_tempoMaps.element_at(--idx);

    /* pTempo is the tempo ::collection::map entry preceding the requested tick offset.
    */

/*    tkOffset = pTempo->tkTempo + muldiv32(msOffset-pTempo->msBase,
                                     1000L*m_dwTimeDivision,
                                     pTempo->dwTempo);

    if (tkOffset > m_tkLength)
    {
        TRACE( "sMTT: Clipping ticks to file length!");
        tkOffset = m_tkLength;
    }

    return tkOffset;
}*/


/*void  Star350File::TicksToMillisecs(
   C1DDWordArray *pMillisArray,
   CTickArray *pTickArray,
   int tkOffset)
{
   if(tkOffset < 0)
   {
      for(int i = 0; i < pTickArray->get_size(); i++)
      {
         imedia::position tk = pTickArray->operator [](i);
         imedia::position tkResult =  tk + tkOffset;
         if(tkResult > tk)
            tkResult = 0;

         pMillisArray->add(
            TicksToMillisecs(
            tkResult));
      }
   }
   else
   {
      for(int i = 0; i < pTickArray->get_size(); i++)
      {
         imedia::position tk = pTickArray->operator [](i);
         imedia::position tkResult =  tk + tkOffset;
         if(tkResult < tk)
            tkResult = 0xFFFFFFFF;
         pMillisArray->add(
            TicksToMillisecs(
            tkResult));
      }
   }
}*/


/*void  Star350File::MillisecsToTicks(
   CTickArray *pTickArray,
   C1DDWordArray *pMillisArray,
   int msOffset)
{
   if(msOffset < 0)
   {
      for(int i = 0; i < pMillisArray->get_size(); i++)
      {
         DWORD ms = pMillisArray->operator [](i);
         DWORD msResult = ms + msOffset;
         if(msResult > ms)
            msResult = 0;
         pTickArray->add(
            MillisecsToTicks(
             msResult));
      }
   }
   else
   {
      for(int i = 0; i < pMillisArray->get_size(); i++)
      {
         DWORD ms = pMillisArray->operator [](i);
         DWORD msResult = ms + msOffset;
         if(msResult < ms)
            msResult = 0x7fffffff;
         pTickArray->add(
            MillisecsToTicks(
             msResult));
      }
   }

}*/
VMSRESULT Star350File::ReadEvent(
    midi_event_base * pEvent,
    LPMIDIHDR               lpmh,
    imedia::position                   tkMax,
    DWORD                   cbPrerollNominalMax)
{
   UNREFERENCED_PARAMETER(tkMax);
   UNREFERENCED_PARAMETER(cbPrerollNominalMax);
    DWORD                   dwTempo;
    LPDWORD                 lpdw;
//    VMSRESULT               vmsr;
    SMFRESULT               smfrc;
    lpdw = (LPDWORD)(lpmh->lpData + lpmh->dwBytesRecorded);
    if(pEvent->GetFlags() & 1)
    {

        m_cbPendingUserEvent = pEvent->GetParamSize();
        m_hpbPendingUserEvent = pEvent->GetParam();
        ASSERT(m_hpbPendingUserEvent);
        m_fdwSMF &= ~FlagInsertSysEx;
      m_dwPendingUserEvent = ((MEVT_F_CALLBACK | MEVT_F_LONG |(((DWORD)MEVT_COMMENT)<<24)) & 0xFF000000L);

        smfrc = InsertParmData(pEvent->GetDelta(), lpmh);
        if (Success != smfrc)
        {
            TRACE( "smfInsertParmData[2] %u", (UINT)smfrc);
            return smfrc;
        }

    }
    else if (pEvent->GetFullType() < ::mus::midi::SysEx)
        {
            if(pEvent->get_type() == ::mus::midi::ProgramChange)
            {
//                m_keyframe.rbProgram[pEvent->GetTrack()] = pEvent->GetChB1();
            }
            if(pEvent->GetTrack() == 9 ||
                pEvent->GetTrack() == 15)
            {
//                TRACE("Star350File::ReadEvents Track %d Program %d", pEvent->GetTrack(), m_keyframe.rbProgram[pEvent->GetTrack()]);
            }
//            if((pEvent->GetType_() == ::mus::midi::NoteOn ||
  //              pEvent->GetType_() == ::mus::midi::NoteOff)
    //            && !((m_keyframe.rbProgram[pEvent->GetTrack()] == 0)
      //          && (pEvent->GetTrack() == 9 ||
        //        pEvent->GetTrack() == 15)))
            {

                int iNotePitch = pEvent->GetNotePitch();
                iNotePitch += m_iKeyShift;
                while(iNotePitch >= 0x80)
                {
                    iNotePitch -= 12;
                }
                while(iNotePitch < 0)
                {
                    iNotePitch += 12;
                }
                pEvent->SetNotePitch((byte) iNotePitch);
            }

            *lpdw++ = (DWORD)pEvent->GetDelta();
            *lpdw++ = 0;
            *lpdw++ = (((DWORD)MEVT_SHORTMSG)<<24) |
                      ((DWORD)pEvent->GetFullType()) |
                      (((DWORD)pEvent->GetChB1()) << 8) |
                      (((DWORD)pEvent->GetChB2()) << 16);

            lpmh->dwBytesRecorded += 3*sizeof(DWORD);
        }
        else if (pEvent->GetFullType() == ::mus::midi::Meta &&
         pEvent->GetMetaType() ==  ::mus::midi::MetaEOT)
        {
            /* These are ignoreable since smfReadNextEvent()
            ** takes care of track merging
            */
        }
        else if (::mus::midi::Meta == pEvent->GetFullType() &&
         ::mus::midi::MetaTempo == pEvent->GetMetaType())
        {
            if (pEvent->GetParamSize() != 3)
            {
                TRACE( "smfReadEvents: Corrupt tempo event");
                return InvalidFile;
            }
            dwTempo = (((DWORD)pEvent->GetParam()[0])<<16)|
                      (((DWORD)pEvent->GetParam()[1])<<8)|
                      ((DWORD)pEvent->GetParam()[2]);
//            dwTempo = (DWORD) ((double) dwTempo / GetTempoShiftRate());
            DWORD dw = (((DWORD)MEVT_TEMPO)<<24)| dwTempo;


            *lpdw++ = (DWORD)pEvent->GetDelta();
            *lpdw++ = 0;
            *lpdw++ = dw;

            lpmh->dwBytesRecorded += 3*sizeof(DWORD);
        }
//       else if ((::mus::midi::Meta == pEvent->get_type()) &&
//          (05 == pEvent->get_type()))
  ///      {
            /**lpdw++ = (DWORD)pEvent->tkDelta;
            *lpdw++ = 0;
            *lpdw++ = MEVT_F_SHORT | MEVT_F_CALLBACK |
                 (((DWORD)MEVT_SHORTMSG )<<24) |
                      ((DWORD)7) |
                      (((DWORD)13) << 8) |
                      (((DWORD)31) << 16);
            lpmh->dwBytesRecorded += 3*sizeof(DWORD);*/
            //m_cbPendingUserEvent = ((DWORD)MEVT_SHORTMSG )<<24;
//         str = (char *) malloc(pEvent->cbParm + 1);
//         strncpy(str, (const char *) pEvent->hpbParm,  pEvent->cbParm);
//         str[pEvent->cbParm] = 0;
//         printf("%s", str);

//         m_cbPendingUserEvent = pEvent->cbParm;
//            m_hpbPendingUserEvent = pEvent->hpbParm;
//            m_fdwSMF &= ~FlagInsertSysEx;
//         m_dwPendingUserEvent = MEVT_F_CALLBACK |(((DWORD)MEVT_LONGMSG )<<24);
            //m_dwPendingUserEvent = (((DWORD)MEVT_LONGMSG )<<24);
//         smfrc = smfInsertParmData(pSmf, pEvent->tkDelta, lpmh);
//            if (Success != smfrc)
//            {
//                TRACE( "smfInsertParmData[2] %u", (UINT)smfrc);
//                return smfrc;
//            }

//            lpdw = (LPDWORD)(lpmh->lpData + lpmh->dwBytesRecorded);


            //smfrc = smfInsertParmData(pSmf, pEvent->tkDelta, lpmh);
            //if (Success != smfrc)
            //{
            //    TRACE( "smfInsertParmData[2] %u", (UINT)smfrc);
            //    return smfrc;
            //}
//         _CrtSetReportFile(_CRT_WARN, debugFile);
//         _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);

         //_RPT0(_CRT_WARN, "Lyric:");
         //_RPT1(_CRT_WARN, "%s", lpmh->lpData);
         //_RPT0(_CRT_WARN, "\n");

    //    }
        else if (pEvent->GetFullType() != ::mus::midi::Meta)
   //   else
        {
//            if(pEvent->GetFullType() == ::mus::midi::SysEx)
//         {
//                break;
//            }
            /* Must be F0 or F7 system exclusive or FF meta
            ** that we didn't recognize
            */
            m_cbPendingUserEvent = pEvent->GetParamSize();
            m_hpbPendingUserEvent = pEvent->GetParam();
            m_fdwSMF &= ~FlagInsertSysEx;

         if(pEvent->GetFullType() == ::mus::midi::SysExEnd)
         {
            m_dwPendingUserEvent = ((DWORD)MEVT_LONGMSG) << 24;
         }
         else if(pEvent->GetFullType() == ::mus::midi::SysEx)
         {
            m_fdwSMF |= FlagInsertSysEx;
            ++m_cbPendingUserEvent;

            /* Falling through...
            */

            m_dwPendingUserEvent = ((DWORD)MEVT_LONGMSG) << 24;
            }

            smfrc = InsertParmData(pEvent->GetDelta(), lpmh);
            if (Success != smfrc)
            {
                TRACE( "smfInsertParmData[2] %u", (UINT)smfrc);
                return smfrc;
            }

            lpdw = (LPDWORD)(lpmh->lpData + lpmh->dwBytesRecorded);
        }
      else
      // este else trata da possiblidade de haverem meta events
      // no arquivo e eles serem contabilizados no encadeamento dos
      // eventos.
      {
         InsertPadEvent(pEvent->GetDelta(), lpmh);
         lpdw = (LPDWORD)(lpmh->lpData + lpmh->dwBytesRecorded);
      }
        return VMSR_SUCCESS;
}

/******************************************************************************
*
* smfReadEvents
*
* This function reads events from a track.
*
* hSmf                      - Specifies the file to read data from.
*
* lpmh                      - Contains information about the buffer to fill.
*
* tkMax                     - Specifies a cutoff point in the stream
*                             beyond which events will not be read.
*
* Return@rdes
*   Success The events were successfully read.
*   SMF_END_OF_TRACK There are no more events to read in this track.
*   InvalidFile A disk error occured on the file.
*
* @xref <f smfWriteEvents>
*****************************************************************************/
SMFRESULT Star350File::ReadEvents(
    LPMIDIHDR               lpmh,
    imedia::position                   tkMax,
    DWORD                   cbPrerollNominalMax)
{
//    PSMF                    pSmf = (PSMF)hSmf;
   SMFRESULT               smfrc;
    midi_event_v008          event;
    LPDWORD                 lpdw;
//    DWORD                   dwTempo;
   imedia::position               tkDelta;
//   imedia::position               tkPosition;
//   imedia::position               tkMin;
//   imedia::position               tk;
   int                  iTrackFound;
//   int                  i;
   imedia::position               tkLastDelta = 0 ;
    imedia::position                   tkOnQuarterNoteDelta = 0;
//   imedia::position               tkLastPositionCB = 0x7fffffff;
   //CXfplayerApp *pApp = (CXfplayerApp *) &System;
   //ASSERT(pApp != NULL);
   //CXfplayerDoc *pDoc = (CXfplayerDoc *) pApp->GetPlayingDocument();
   //ASSERT(pDoc != NULL);
//   char * str;
    //assert(pSmf != NULL);
    ASSERT(lpmh != NULL);

    /*
    ** read events from the track and pack them into the buffer in polymsg
    ** format.
    **
    ** If a SysEx or meta would go over a buffer boundry, split it.
    */
    lpmh->dwBytesRecorded = 0;
    if (m_dwPendingUserEvent)
    {
        smfrc = InsertParmData((imedia::position)0, lpmh);
        if (Success != smfrc)
        {
            TRACE( "smfInsertParmData() -> %u", (UINT)smfrc);
            return smfrc;
        }
    }
//   if(m_pendingLyricEventArray.get_size() > 0)
//   {
  //      smfrc = InsertLyricEvents(NULL, lpmh);
//        if (Success != smfrc)
  //      {
    //        TRACE( "smfInsertParmData() -> %u", (UINT)smfrc);
      //      return smfrc;
        //}
   //}

    lpdw = (LPDWORD)(lpmh->lpData + lpmh->dwBytesRecorded);

    if (m_fdwSMF & FlagEof)
    {
        return EndOfFile;
    }
   /*if(pDoc->m_iBufferingOffsetTokenIndex >= pDoc->m_TokensOffsetTicks.get_size())
   {
      tkNextOffsetLyricEvent = -1;
   }
   else
   {
      tkNextOffsetLyricEvent = pDoc->m_TokensOffsetTicks[pDoc->m_iBufferingOffsetTokenIndex ];
   }
   if(pDoc->m_iBufferingTokenIndex >= pDoc->m_TokensTicks.get_size())
   {
      tkNextLyricEvent = -1;
   }
   else
   {
      tkNextLyricEvent = pDoc->m_TokensTicks[pDoc->m_iBufferingTokenIndex ];
   }*/
//   CLyricEventsV1 *pEventsV1;
    while(TRUE)
    {
        ASSERT(lpmh->dwBytesRecorded <= lpmh->dwBufferLength);
        if(lpmh->dwBytesRecorded > cbPrerollNominalMax)
        {
            break;
        }

        /* If we know ahead of time we won't have room for the
        ** event, just break out now. We need 2 DWORD's for the
        ** terminator event and at least 2 DWORD's for any
        ** event we might store - this will allow us a full
        ** short event or the delta time and stub for a long
        ** event to be split.
        */
      /*
      now 16 DWORDS
      */
        if (lpmh->dwBufferLength - lpmh->dwBytesRecorded < 4*sizeof(DWORD))
        {
            break;
        }




      //smfrc = GetNextEventTkPosition(&tkPosition, tkMax);


/*        if (Success != smfrc)
        {
         smfrc = GetNextEvent((midi_event_v001 *)&event, tkMax, TRUE);
         ASSERT(Success != smfrc); */

            /* smfGetNextEvent doesn't set this because smfSeek uses it
            ** as well and needs to distinguish between reaching the
            ** seek point and reaching end-of-file.
            **
            ** To the ::fontopus::user, however, we present the selection between
            ** their given tkBase and tkEnd as the entire file, therefore
            ** we want to translate this into EOF.
            */
            /*if (ReachedTkMax == smfrc)
            {
                m_fdwSMF |= FlagEof;
            }

            TRACE( "smfReadEvents: ReadEvents() -> %u", (UINT)smfrc);
            break;
        }*/
      iTrackFound = -1;
      tkLastDelta = 0;
/*      while(TRUE)
      {
         for(i = 0; i < m_eventsTracksForPositionCB.get_size(); i++)
         {
            pEventsV1 = m_eventsTracksForPositionCB.get_at(i);
            if(pEventsV1->GetClassOrder() == 1)
                {
                    while(m_ptracks->m_tkPosition >
                        (tk = pEventsV1->GetCurrentTkTokenPosition()))
                    {
                        pEventsV1->m_iCurrentToken++;
                    }
                }
            else
                {
                    while(m_ptracks->m_tkPosition >
                        (tk = ((CLyricEventsV2 *)pEventsV1)->GetCurrentTkNotePosition()))
                    {
                        pEventsV1->m_iCurrentNote++;
                    }
                }
            if(iTrackFound < 0)
            {
                 if(tk >= 0)
                    if(tkPosition >= tk)
                    {
                       tkMin = tk;
                       iTrackFound = i;
                    }
                }
                else
                {
               if(tk >= 0)
                  if(tkPosition >= tk)
                  {
                     if(tk < tkMin)
                     {
                        tkMin = tk;
                        iTrackFound = i;
                     }
                        }

            }
         }
         if(iTrackFound < 0)
            break;
         else
         {
            pEventsV1 = m_eventsTracksForPositionCB.get_at(iTrackFound);
#ifdef _DEBUG
//         if(pEventsV1->m_iType == 1)
//         {
//            TRACE("Star350File::ReadEvents pEventsV1->m_iCurrentToken = %d\n", pEventsV1->m_iCurrentToken);
//            TRACE("Star350File::ReadEvents pEventsV1->m_iType = %d\n", pEventsV1->m_iType);
//            TRACE("Star350File::ReadEvents m_ptracks->m_tkPosition = %d\n", m_ptracks->m_tkPosition);
//         }
#endif
            tkDelta = tkMin - m_ptracks->m_tkPosition;
            if((tkDelta - tkLastDelta) == 0)
            {
               pEventsV1->m_tkDelta = 0;
               InsertLyricEvents(pEventsV1, lpmh);
            }
            else
            {
               if(m_pendingLyricEventArray.get_size() > 0)
               {
                  smfrc = InsertLyricEvents(NULL, lpmh);
                  if(MetaPending == smfrc)
                  {
                     break;
                  }
                  else if (Success != smfrc)
                  {

                     TRACE( "smfInsertLyricEvent %u", (UINT)smfrc);
                     return smfrc;
                  }
                  lpdw = (LPDWORD)(lpmh->lpData + lpmh->dwBytesRecorded);
               }
               pEventsV1->m_tkDelta = tkDelta - tkLastDelta;
               InsertLyricEvents(pEventsV1, lpmh);
            }

            iTrackFound = -1;
            tkLastDelta = tkDelta;
         }
         if (lpmh->dwBufferLength - lpmh->dwBytesRecorded < 4*sizeof(DWORD))
         {
            break;
         }
      }*/
      /*if(m_pendingLyricEventArray.get_size() > 0)
      {
         if (lpmh->dwBufferLength - lpmh->dwBytesRecorded < 4*sizeof(DWORD))
         {
            break;
         }
         smfrc = InsertLyricEvents(NULL, lpmh);
         if(MetaPending == smfrc)
         {
            break;
         }
         else if (Success != smfrc)
         {
            TRACE( "smfInsertLyricEvent %u", (UINT)smfrc);
            return smfrc;
         }
         lpdw = (LPDWORD)(lpmh->lpData + lpmh->dwBytesRecorded);
      }*/
      if (lpmh->dwBufferLength - lpmh->dwBytesRecorded < 4*sizeof(DWORD))
      {
         m_ptracks->m_tkPosition += tkDelta;
         break;
      }

        smfrc = GetNextEvent((Star350EventV001 *)&event, tkMax, TRUE);
        if (Success != smfrc)
        {
            /* smfGetNextEvent doesn't set this because smfSeek uses it
            ** as well and needs to distinguish between reaching the
            ** seek point and reaching end-of-file.
            **
            ** To the ::fontopus::user, however, we present the selection between
            ** their given tkBase and tkEnd as the entire file, therefore
            ** we want to translate this into EOF.
            */
            if (ReachedTkMax == smfrc)
            {
                m_fdwSMF |= FlagEof;
            }

            TRACE( "smfReadEvents: ReadEvents() -> %u", (UINT)smfrc);
            break;
        }
        VMSRESULT vmsr;
      event.SetDelta(event.GetDelta() - tkLastDelta);
        vmsr = ReadEvent(&event, lpmh, tkMax, cbPrerollNominalMax);

    }

    return (m_fdwSMF & FlagEof) ? EndOfFile : Success;
}

/******************************************************************************
*
* smfReadEvents
*
* This function reads events from a track.
*
* hSmf                      - Specifies the file to read data from.
*
* lpmh                      - Contains information about the buffer to fill.
*
* tkMax                     - Specifies a cutoff point in the stream
*                             beyond which events will not be read.
*
* Return@rdes
*   Success The events were successfully read.
*   SMF_END_OF_TRACK There are no more events to read in this track.
*   InvalidFile A disk error occured on the file.
*
* @xref <f smfWriteEvents>
*****************************************************************************/

/*VMSRESULT Star350File::InsertLyricEvents(
   CLyricEventV1 *      pLyricEvent,
    LPMIDIHDR         lpmh)

{*/
/*    LPDWORD                 lpdw;
   DWORD                   dwRounded;
   DWORD               dwLength;
   DWORD               dwSize;
   imedia::position               tkDelta;

   if(pLyricEvent != NULL)
   {
      m_pendingLyricEventArray.add(*pLyricEvent);
      if(pLyricEvent->GetClassOrder() == 1)
         pLyricEvent->m_iCurrentToken++;
      else
      {
         CLyricEventsV2 * pPendingLyricEventV2 =
            (CLyricEventsV2 *) pLyricEvent;
         pPendingLyricEventV2->m_iCurrentNote++;
         if(pPendingLyricEventV2->m_iCurrentNote <
            pPendingLyricEventV2->m_msaNotesPosition.get_size())
         {
            dword_array dwArray;
   //            VERIFY(1 == pPendingLyricEventV2->GetNoteTokens(m_pPendingLyricEventV1->m_iCurrentNote, dwArray));
            pPendingLyricEventV2->GetNoteTokens(pPendingLyricEventV2->m_iCurrentNote, dwArray);
            if(dwArray.get_size() > 0)
               pPendingLyricEventV2->m_iCurrentToken = dwArray.get_at(0);
            else
               pPendingLyricEventV2->m_iCurrentToken = -1;
         }
      }
      return Success;
   }

    ASSERT(lpmh != NULL);

   if(m_pendingLyricEventArray.get_size() <= 0)
   {
      return Success;
   }

   tkDelta = m_pendingLyricEventArray[0].m_tkDelta;

   ASSERT(tkDelta >= 0);

    /* Can't fit 4 DWORD's? (tkDelta + stream-id + event + some data)
    ** Can't do anything.
    */
    /*ASSERT(lpmh->dwBufferLength >= lpmh->dwBytesRecorded);
//   m_dwPendingUserEvent = MEVT_F_CALLBACK |
//             (((DWORD)MEVT_LONGMSG)<<24);
//   m_cbPendingUserEvent = 2 * sizeof(DWORD);

    if (lpmh->dwBufferLength - lpmh->dwBytesRecorded < 4 * sizeof(DWORD))
    {
        if (0 == tkDelta)
            return Success;

        /* If we got here with a real delta, that means smfReadEvents screwed
        ** up calculating left space and we should flag it somehow.
        */
/*        TRACE( "Can't fit initial piece of SysEx into buffer!");
        return InvalidFile;
    }

   CMemFile memFile;
   CArchive ar(&memFile, CArchive::store);
   m_pendingLyricEventArray.Serialize(ar);
   ar.Flush();
   dwSize = (memFile.get_length() + sizeof(DWORD) + 3) & (~3L);


    lpdw = (LPDWORD)(lpmh->lpData + lpmh->dwBytesRecorded);
   dwLength = lpmh->dwBufferLength - lpmh->dwBytesRecorded - 3*sizeof(DWORD);
   if(dwSize > dwLength)
      return MetaPending;
    dwLength = dwSize;

//   DWORD dwa = MEVT_F_CALLBACK;
//   DWORD dwb = MEVT_LONGMSG;
//   DWORD dwc = MEVT_F_LONG;
//   DWORD dwd = MEVT_COMMENT;

    ASSERT((dwSize & 3L) == 0);
    dwRounded = dwSize;
    //dwRounded = (dwLength + 3) & (~3L);

   //tkOffset = tkDelta;
   *lpdw++ = (DWORD) tkDelta;
   *lpdw++ = 0;
   //*lpdw++ = ((MEVT_F_CALLBACK | MEVT_F_LONG |(((DWORD)MEVT_LONGMSG)<<24)) & 0xFF000000L )   | (dwLength & 0x00FFFFFFL);;
   *lpdw++ = ((MEVT_F_CALLBACK | MEVT_F_LONG |(((DWORD)MEVT_COMMENT)<<24)) & 0xFF000000L )   | (dwLength & 0x00FFFFFFL);;
   *lpdw++ = EVENT_ID_LYRIC_V1;

//   if(m_cbPendingLyricEventV1 >= (3 * sizeof(DWORD)))
//   {
//      // Offset Lyric CallBack Event Code
//      *lpdw++ = m_pPendingLyricEventV1->m_Union.m_Event.m_nType;
//   }
//   if(m_cbPendingLyricEventV1 >= (2 * sizeof(DWORD)))
//   {
//      // Offset Lyric CallBack Event Code
//      *lpdw++ = m_pPendingLyricEventV1->m_Union.m_Event.m_nTrack;
//   }
//   if(m_cbPendingLyricEventV1 >= (1 * sizeof(DWORD)))
//   {
//      // Offset Lyric CallBack Event Code
//      *lpdw++ = m_pPendingLyricEventV1->m_Union.m_Event.m_nCurrentIndex;
//      m_pPendingLyricEventV1->m_Union.m_Event.m_nCurrentIndex++;
//   }
   memFile.seek_to_begin();
   memFile.read(lpdw, dwLength);
   while(0 < m_pendingLyricEventArray.get_size())
   {
      m_pendingLyricEventArray.remove_at(
         m_pendingLyricEventArray.get_size() - 1);
   }
   lpmh->dwBytesRecorded += 3*sizeof(DWORD) + dwRounded;
    */
/*   return Success;
}*/

SMFRESULT Star350File::InsertPadEvent(
   imedia::position            tkDelta,
    LPMIDIHDR         lpmh)

{
    LPDWORD                 lpdw;
   DWORD                   dwRounded;
   DWORD               dwLength;


   ASSERT(tkDelta >= 0);
//    assert(pSmf != NULL);
    ASSERT(lpmh != NULL);

    /* Can't fit 4 DWORD's? (tkDelta + stream-id + event + some data)
    ** Can't do anything.
    */
    ASSERT(lpmh->dwBufferLength >= lpmh->dwBytesRecorded);
//   m_dwPendingUserEvent = MEVT_F_CALLBACK |
//             (((DWORD)MEVT_LONGMSG)<<24);
//   m_cbPendingUserEvent = 2 * sizeof(DWORD);
//   if(!m_cbPendingLyricEventV1)
   if(!1)
   {
      return Success;
   }

    if (lpmh->dwBufferLength - lpmh->dwBytesRecorded < 4 * sizeof(DWORD))
    {
        if (0 == tkDelta)
            return Success;

        /* If we got here with a real delta, that means smfReadEvents screwed
        ** up calculating left space and we should flag it somehow.
        */
        TRACE( "Can't fit initial piece of SysEx into buffer!");
        return InvalidFile;
    }

    lpdw = (LPDWORD)(lpmh->lpData + lpmh->dwBytesRecorded);
   dwLength = lpmh->dwBufferLength - lpmh->dwBytesRecorded - 3*sizeof(DWORD);
    dwLength = min(dwLength, 1);

   //DWORD dwa = MEVT_F_CALLBACK;
//   DWORD dwb = MEVT_LONGMSG;
//   DWORD dwc = MEVT_F_LONG;
//   DWORD dwd = MEVT_COMMENT;

   dwRounded = (dwLength + 3) & (~3L);

   //tkOffset = tkDelta;
   *lpdw++ = (DWORD) tkDelta;
   *lpdw++ = 0;
   //*lpdw++ = ((MEVT_F_CALLBACK | MEVT_F_LONG |(((DWORD)MEVT_LONGMSG)<<24)) & 0xFF000000L )   | (dwLength & 0x00FFFFFFL);;
   *lpdw++ = ((MEVT_F_CALLBACK | MEVT_F_LONG |(((DWORD)MEVT_COMMENT)<<24)) & 0xFF000000L )   | (dwLength & 0x00FFFFFFL);;
   *lpdw++ = EVENT_ID_PAD;

//   if(m_cbPendingLyricEventV1 >= (3 * sizeof(DWORD)))
//   {
//      // Offset Lyric CallBack Event Code
//      *lpdw++ = m_pPendingLyricEventV1->m_Union.m_Event.m_nType;
//   }
//   if(m_cbPendingLyricEventV1 >= (2 * sizeof(DWORD)))
//   {
//      // Offset Lyric CallBack Event Code
//      *lpdw++ = m_pPendingLyricEventV1->m_Union.m_Event.m_nTrack;
//   }
//   if(m_cbPendingLyricEventV1 >= (1 * sizeof(DWORD)))
//   {
//      // Offset Lyric CallBack Event Code
//      *lpdw++ = m_pPendingLyricEventV1->m_Union.m_Event.m_nCurrentIndex;
//      m_pPendingLyricEventV1->m_Union.m_Event.m_nCurrentIndex++;
//   }
//   m_pPendingLyricEventV1->ToData(lpdw, m_cbPendingLyricEventV1);
//   if (0 == (m_cbPendingLyricEventV1 -= dwLength))
//   {
//   }
//   else
//   {
//      int i = 5;
//   }

   lpmh->dwBytesRecorded += 3*sizeof(DWORD) + dwRounded;
   return Success;
}
/******************************************************************************
*
* smfInsertParmData
*
* Inserts pending long data from a track into the given buffer.
*
* pSmf                      - Specifies the file to read data from.
*
* tkDelta                   - Specfices the tick delta for the data.
*
* lpmh                      - Contains information about the buffer to fill.
*
* Returns
*   Success The events were successfully read.
*   InvalidFile A disk error occured on the file.
*
* Fills as much data as will fit while leaving room for the buffer
* terminator.
*
* If the long data is depleted, resets m_dwPendingUserEvent so
* that the next event may be read.
*
*****************************************************************************/
 SMFRESULT Star350File::InsertParmData(
    imedia::position                   tkDelta,
    LPMIDIHDR               lpmh)
{
    DWORD                   dwLength;
    DWORD                   dwRounded;
    LPDWORD                 lpdw;

//    assert(pSmf != NULL);
    ASSERT(lpmh != NULL);

    /* Can't fit 4 DWORD's? (tkDelta + stream-id + event + some data)
    ** Can't do anything.
    */
    ASSERT(lpmh->dwBufferLength >= lpmh->dwBytesRecorded);

    if (lpmh->dwBufferLength - lpmh->dwBytesRecorded < 4*sizeof(DWORD))
    {
        if (0 == tkDelta)
            return Success;

        /* If we got here with a real delta, that means smfReadEvents screwed
        ** up calculating left space and we should flag it somehow.
        */
        TRACE( "Can't fit initial piece of SysEx into buffer!");
        return InvalidFile;
    }

    lpdw = (LPDWORD)(lpmh->lpData + lpmh->dwBytesRecorded);

    dwLength = lpmh->dwBufferLength - lpmh->dwBytesRecorded - 3*sizeof(DWORD);
    dwLength = min(dwLength, m_cbPendingUserEvent);

    *lpdw++ = (DWORD)tkDelta;
    *lpdw++ = 0L;
    *lpdw++ = (m_dwPendingUserEvent & 0xFF000000L) | (dwLength & 0x00FFFFFFL);

    dwRounded = (dwLength + 3) & (~3L);

    if (m_fdwSMF & FlagInsertSysEx)
    {
      LPBYTE lpb = (LPBYTE) lpdw;
      *lpb++ = ::mus::midi::SysEx;
        m_fdwSMF &= ~FlagInsertSysEx;
        --dwLength;
        --m_cbPendingUserEvent;
        lpdw = (LPDWORD) lpb;
    }

    if (dwLength & 0x80000000L)
    {
        TRACE( "dwLength %08lX  dwBytesRecorded %08lX  dwBufferLength %08lX", dwLength, lpmh->dwBytesRecorded, lpmh->dwBufferLength);
        TRACE( "cbPendingUserEvent %08lX  dwPendingUserEvent %08lX dwRounded %08lX", m_cbPendingUserEvent, m_dwPendingUserEvent, dwRounded);
        TRACE( "Offset into MIDI image %08lX", (DWORD)(m_hpbPendingUserEvent - GetImage()));
        TRACE( "!hmemcpy is about to fault");
    }

    memcpy(lpdw, m_hpbPendingUserEvent, dwLength);
    if (0 == (m_cbPendingUserEvent -= dwLength))
        m_dwPendingUserEvent = 0;

    lpmh->dwBytesRecorded += 3*sizeof(DWORD) + dwRounded;

    return Success;
}

/******************************************************************************
*
* smfSeek
*
* This function moves the file pointer within a track
* and gets the state of the track at the new position. It returns a buffer of
* state information which can be used to set up to play from the new position.
*
* hSmf                      - Handle of file to seek within
*
* tkPosition                - The position to seek to in the track.
*
* lpmh                      - A buffer to contain the state information.
*
* Returns
*   Success | The state was successfully read.
*   SMF_END_OF_TRACK | The pointer was moved to end of track and no state
*     information was returned.
*   SMF_INVALID_PARM | The given handle or buffer was invalid.
*   NoMemory | There was insufficient primitive::memory in the given buffer to
*     contain all of the state data.
*
* The state information in the buffer includes patch changes, tempo changes,
* time signature, key signature,
* and controller information. Only the most recent of these paramters before
* the current position will be stored. The state buffer will be returned
* in polymsg format so that it may be directly transmitted over the MIDI
* bus to bring the state up to date.
*
* The buffer is mean to be sent as a streaming buffer; i.e. immediately
* followed by the first data buffer. If the requested tick position
* does not exist in the file, the last event in the buffer
* will be a MEVT_NOP with a delta time calculated to make sure that
* the next stream event plays at the proper time.
*
* The meta events (tempo, time signature, key signature) will be the
* first events in the buffer if they exist.
*
* Use smfGetStateMaxSize to determine the maximum size of the state
* information buffer. State information that will not fit into the given
* buffer will be lost.
*
* On return, the dwBytesRecorded field of lpmh will contain the
* actual number of bytes stored in the buffer.
*
*****************************************************************************/


SMFRESULT Star350File::seek(
    imedia::position                   tkPosition,
    LPMIDIHDR               lpmh)
{
//    PSMF                    pSmf    = (PSMF)hSmf;
//    CMidiTrack *            ptrk;
//    DWORD                   idxTrack;
    SMFRESULT               smfrc;
    Star350EventV001          event;
    LPDWORD                 lpdw;
//    BYTE                    bEvent;
//    UINT                    idx;
//    UINT                    idxChannel;
//    UINT                    idxController;

//    static KEYFRAME         kf;

/*    EventsTracksV1 * pEventsArray = &m_eventsTracksForPositionCB;
   ASSERT(pEventsArray != NULL);
//   CLyricEventsV2 * pEvents;
//   if(pDoc->m_p2DTokens == NULL)
//      pEvents = NULL;
//   else if(pDoc->m_p2DTokens->get_size() <= 0)
//      pEvents = NULL;
//   else if(pEventsArray->get_size() < pDoc->m_p2DTokens->get_size())
//      pEvents = NULL;
//   else
//   {
//      pEvents = (CLyricEventsV2 *) pEventsArray->operator [](pDoc->m_p2DTokens->get_size());
//      ASSERT(pEvents != NULL);
//      ASSERT(pEvents->GetClassOrder() == 2);
//   }
//    imedia::position tkPosition = preroll.tkBase;
    CLyricEventsV1 * pEventsV1;
    CLyricEventsV2 * pEventsV2;
    int iTokenIndex = -1;
    int iNoteIndex = -1;

    memset(&m_keyframe, 0, sizeof(m_keyframe));

    for(int i = 0; i < pEventsArray->get_size(); i++)
    {
       pEventsV1 =   pEventsArray->get_at(i);
       if(pEventsV1->GetClassOrder() == 2)
        {
            pEventsV2 = (CLyricEventsV2 *) pEventsV1;
            iNoteIndex = pEventsV2->GetPositionNote(tkPosition);
            if(iNoteIndex >= 0)
            {
                break;
            }
            iTokenIndex = pEventsV2->GetPositionToken(tkPosition);
            if(iTokenIndex >= 0)
            {
                break;
            }
        }
    }
    if(iTokenIndex < 0 && iNoteIndex < 0)
    {
        iTokenIndex = 0;
        iNoteIndex = 0;
    }
    else if(iNoteIndex < 0)
    {
        iNoteIndex = pEventsV2->GetTokenFirstNote(iTokenIndex);
        if(iNoteIndex < 0)
            iNoteIndex = 0;
    }
    else if(iTokenIndex < 0)
    {
        iTokenIndex = pEventsV2->GetNoteFirstToken(iNoteIndex);
        if(iTokenIndex < 0)
            iTokenIndex = 0;
    }
   for(i = 0; i < pEventsArray->get_size(); i++)
   {
      pEventsArray->get_at(i)->m_iCurrentToken = iTokenIndex;
      pEventsArray->get_at(i)->m_iCurrentNote = iNoteIndex;
   }*/

//    memset(&m_keyframe, 0xFF, sizeof(m_keyframe));
  //  memset(&m_keyframe.rbProgram, 0x00, sizeof(m_keyframe.rbProgram));

    m_ptracks->m_tkPosition = 0;
    m_fdwSMF &= ~FlagEof;
   m_ptracks->m_fdwSMF &= ~FlagEof;

   m_ptracks->seek_begin();
    //for (ptrk = m_rTracks, idxTrack = m_dwTracks; idxTrack--; ptrk++)

    while (Success == (smfrc = GetNextEvent(&event, tkPosition, FALSE)))
    {

/*        if (::mus::midi::Meta == (bEvent = event.GetFullType()))
        {
            if (event.GetMetaType() == ::mus::midi::MetaTempo)
            {
                if (event.m_cbParm != sizeof(m_keyframe.rbTempo))
                    return InvalidFile;

                memcpy((byte *)m_keyframe.rbTempo, event.m_hpbParm, event.m_cbParm);
            }
        }
      if((bEvent & 0xF0) == ::mus::midi::ProgramChange)
      {
         m_keyframe.rbProgram[bEvent & 0x0F] = event.GetChB1();
      }
        else if((bEvent & 0xF0) == ::mus::midi::ControlChange)
      {
         m_keyframe.rbControl[(((WORD)bEvent & 0x0F)*120) + event.GetChB1()] =
         event.GetChB2();
        }*/
    }

    if (ReachedTkMax != smfrc)
    {
        return smfrc;
    }

    /* Build lpmh from keyframe
    */
    lpmh->dwBytesRecorded = 0;
    lpdw = (LPDWORD)lpmh->lpData;

    /* Tempo change event?
    */
/*    if (KF_EMPTY != m_keyframe.rbTempo[0] ||
        KF_EMPTY != m_keyframe.rbTempo[1] ||
        KF_EMPTY != m_keyframe.rbTempo[2])
    {
        if (lpmh->dwBufferLength - lpmh->dwBytesRecorded < 3*sizeof(DWORD))
            return NoMemory;

        *lpdw++ = 0;
        *lpdw++ = 0;
//      *lpdw++ = (((DWORD)m_keyframe.rbTempo[2])<<16)|
  //                (((DWORD)m_keyframe.rbTempo[1])<<8)|
    //            ((DWORD)m_keyframe.rbTempo[0])|
      //            (((DWORD)MEVT_SHORTMSG) << 24);
//
//      *lpdw++ = (((DWORD)m_keyframe.rbTempo[0])<<16)|
  //                (((DWORD)m_keyframe.rbTempo[1])<<8)|
    //             ((DWORD)m_keyframe.rbTempo[2])|
      //            (((DWORD)MEVT_SHORTMSG) << 24);

        *lpdw++ = (((DWORD)m_keyframe.rbTempo[0])<<16)|
                  (((DWORD)m_keyframe.rbTempo[1])<<8)|
                  ((DWORD)m_keyframe.rbTempo[2])|
                  (((DWORD)MEVT_TEMPO) << 24);
//        *lpdw++ = (((DWORD)m_keyframe.rbTempo[2])<<16)|
  //                (((DWORD)m_keyframe.rbTempo[1])<<8)|
    //              ((DWORD)m_keyframe.rbTempo[0])|
      ///            (((DWORD)MEVT_TEMPO) << 24);

        lpmh->dwBytesRecorded += 3*sizeof(DWORD);
    }*/

    /* Program change events?
    */
/*    for (idx = 0; idx < 16; idx++)
    {
        if (KF_EMPTY != m_keyframe.rbProgram[idx])
        {
            if (lpmh->dwBufferLength - lpmh->dwBytesRecorded < 3*sizeof(DWORD))
                return NoMemory;

            *lpdw++ = 0;
            *lpdw++ = 0;
            *lpdw++ = (((DWORD)MEVT_SHORTMSG) << 24)      |
                      ((DWORD)::mus::midi::ProgramChange)         |
                      ((DWORD)idx)                        |
                      (((DWORD)m_keyframe.rbProgram[idx]) << 8);

            lpmh->dwBytesRecorded += 3*sizeof(DWORD);
        }
    }

    /* Controller events?
    */
  /*  idx = 0;
    for (idxChannel = 0; idxChannel < 16; idxChannel++)
    {
        for (idxController = 0; idxController < 120; idxController++)
        {
            if (KF_EMPTY != m_keyframe.rbControl[idx])
            {
                if (lpmh->dwBufferLength - lpmh->dwBytesRecorded < 3*sizeof(DWORD))
                    return NoMemory;

                *lpdw++ = 0;
                *lpdw++ = 0;
                *lpdw++ = (((DWORD)MEVT_SHORTMSG << 24)     |
                          ((DWORD)::mus::midi::ControlChange)       |
                          ((DWORD)idxChannel)               |
                          (((DWORD)idxController) << 8)     |
                          (((DWORD)m_keyframe.rbControl[idx]) << 16));


                lpmh->dwBytesRecorded += 3*sizeof(DWORD);
            }

            idx++;
        }
    }

    /* Force all tracks to be at tkPosition. We are guaranteed that
    ** all tracks will be past the event immediately preceding tkPosition;
    ** this will force correct delta-ticks to be generated so that events
    ** on all tracks will line up properly on a seek into the middle of the
    ** file.
    */
   // ^^^^ This cause problems... tracks are already positioned...
   // changing track position make all the song envents be shifted
   // irregularly...
//    for (ptrk = m_rTracks, idxTrack = m_dwTracks; idxTrack--; ptrk++)
//   for (idxTrack = m_dwTracks; idxTrack--;)
  //  {
   //   ptrk = m_tracks[idxTrack];
      //  ptrk->tkPosition        = tkPosition;
    //}

    return Success;
}

SMFRESULT Star350File::seek(
    imedia::position                   tkPosition)
{
    SMFRESULT               smfrc;
    Star350EventV001              event;


    m_ptracks->m_tkPosition = 0;
    m_fdwSMF &= ~FlagEof;
   m_ptracks->m_fdwSMF &= ~FlagEof;

   m_ptracks->seek_begin();
    //for (ptrk = m_rTracks, idxTrack = m_dwTracks; idxTrack--; ptrk++)

    while(Success == (smfrc = GetNextEvent(&event, tkPosition, FALSE)));

    if (ReachedTkMax != smfrc)
    {
        return smfrc;
    }

    return Success;
}

/******************************************************************************
*
* smfGetStateMaxSize
*
* This function returns the maximum sizeof buffer that is needed to
* hold the state information returned by f smfSeek.
*
* pdwSize                   - Gets the size in bytes that should be allocated
*                             for the state buffer.
*
* Returns the state size in bytes.
*
*****************************************************************************/
DWORD Star350File::GetStateMaxSize(
    VOID)
{
    return  3*sizeof(DWORD) +           /* Tempo                */
            3*16*sizeof(DWORD) +        /* Patch changes        */
            3*16*120*sizeof(DWORD) +    /* Controller changes   */
            3*sizeof(DWORD);            /* time alignment NOP   */
}







/*****************************************************************************
*
*  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
*  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
*  A PARTICULAR PURPOSE.
*
*  Copyright (C) 1993 - 1997 ca2.
*
******************************************************************************
*
* SMFRead.C
*
* MIDI File access routines.
*
*****************************************************************************/
#include <memory.h>
#include "midiplyr.h"


Star350File::Star350File(::ca::application * papp) :
   primitive::memory_container(papp)
{
   m_estorage = mus::StorageNone;

   m_ptracks = new Star350Tracks(this);
   //CMidiTracks::CMidiTracks();
//   memset(m_szName, 0, sizeof(m_szName));
//   GetImage() = 0;
//   m_cbImage = 0;

   m_tkLength = 0;
   m_dwFormat = 0;
   m_dwTracks = 0;
   m_dwTimeDivision = 0;
   m_fdwSMF = 0;

//   m_dwAllocation = 0;
//   m_dwAllocationAddUp = 0;


//      m_cTempoMap = 0;
//      m_cTempoMapAlloc = 0;
//      m_hTempoMap = 0;
//      m_pTempoMap = 0;
//      m_tempoMaps;

   m_dwPendingUserEvent = 0;
   m_cbPendingUserEvent = 0;
   m_hpbPendingUserEvent = 0;
   //rTracks = NULL;
//   m_pPendingLyricEventV1 = NULL;
//   m_cbPendingLyricEventV1 = 0;

//   m_tempoMaps.set_size(0, C_TEMPO_MAP_CHK);

   m_ptracks->m_pfile = this;

    m_iKeyShift = 0;
    m_iTempoShift = 0;

    m_pinfoheader = new Star350InfoHeader();

    keep_pointer((void **) &m_pchHeader);
    keep_pointer((void **) &m_pevheader);


}

Star350File::~Star350File()
{

   CloseFile();

   if(m_pinfoheader != NULL)
   {
      delete m_pinfoheader;
      m_pinfoheader = NULL;
   }

   if(m_estorage == mus::StorageAttach)
   {
      detach();
   }

}

/******************************************************************************
*
* smfBuildFileIndex
*
* Preliminary parsing of a MIDI file.
*
* ppSmf                     - Pointer to a returned SMF structure if the
*                             file is successfully parsed.
*
* Returns
*   Success The events were successfully read.
*   NoMemory Out of primitive::memory to build key frames.
*   InvalidFile A disk or parse error occured on the file.
*
* This function validates the format of and existing MIDI or RMI file
* and builds the handle structure which will refer to it for the
* lifetime of the instance.
*
* The file header information will be read and verified, and
* smfBuildTrackIndices will be called on every existing track
* to build keyframes and validate the track format.
*
*****************************************************************************/
SMFRESULT Star350File::BuildIndex()

{
   SMFRESULT               smfrc = Success;
   LPSTAR350CHUNKHDR       pch;
   DWORD                   idx;
   DWORD                   dwLeft;
   byte *                  hpbImage;
//   BOOL                    fFirst = TRUE;
//   Star350TrackBase *     ptrk = NULL;
   Star350EventTrack *    ptrkevent;
   midi_event_v001          event;
   byte *                  hpbEvents;

   dwLeft   = GetImageSize();
   hpbImage = GetImage();

   if (dwLeft < sizeof(m_lpchHeader))
      return InvalidFile;

   m_pchHeader = (LPBYTE) hpbImage;

   dwLeft   -= sizeof(m_lpchHeader);
   hpbImage += sizeof(m_lpchHeader);

   if(memcmp(
        m_lpchHeader,
        m_pchHeader,
        sizeof(m_lpchHeader))
            != 0)
        return InvalidFile;

    byte * hpb;
    if(VMS_FAILED(m_pinfoheader->FromStorage(hpbImage, &hpb)))
        return InvalidFile;

    dwLeft -= hpb - hpbImage;
    hpbImage = hpb;

    /*
    ** We've successfully parsed the header. Now try to build the track
    ** index.
    **
    ** We only check out the track header chunk here; the track will be
    ** preparsed after we do a quick integretiy check.
    */

    //wMemory = sizeof(SMF) + (WORD)(m_dwTracks*sizeof(TRACK));

    //pSmfTemp = (PSMF)LocalReAlloc((HLOCAL)pSmf, wMemory, LMEM_MOVEABLE|LMEM_ZEROINIT);

    //if (NULL == pSmfTemp)
    //{
      //  TRACE( "No primitive::memory for extended pSmf");
        //return NoMemory;
    //}

   //   m_rTracks = new TRACK[m_dwTracks];
//   for(DWORD dw = 0; dw < m_dwTracks; dw++)
//   {
//      m_ptracks->add(new CMidiTrack(&m_tracks));
//   }

    //melody_track = hpbImage[20];

    hpbEvents = hpbImage;
    hpbImage += 4;

    m_pevheader = (LPSTAR350FILEHDR) hpbImage;

    m_dwTracks = m_pevheader->wTracks;
    m_dwTimeDivision = m_pevheader->wDivision;
    m_dwMelodyTrack = m_pevheader->wMelodyTrack;
    m_dwLyricTrack = m_pevheader->wLyricTrack;

    hpbImage += 34 + 6 * (m_dwTracks - 1);

//    m_ptracks->set_size(m_dwTracks);


    //pSmf = *ppSmf = pSmfTemp;
//    pTrk = m_rTracks;

    for (idx=0; idx < m_dwTracks; idx++)
    {
      ptrkevent = new Star350EventTrack();
        ptrkevent->SetLyricsTrack(m_dwLyricTrack == idx);
        m_ptracks->add(ptrkevent);
        if (dwLeft < sizeof(STAR350CHUNKHDR))
            return InvalidFile;

        pch = (LPSTAR350CHUNKHDR) hpbImage;

        dwLeft   -= sizeof(STAR350CHUNKHDR);
        hpbImage += sizeof(STAR350CHUNKHDR);

//        if (pCh->fourccType != FOURCC_MTrk)
//         return InvalidFile;


        ptrkevent->m_idxTrack      = (DWORD)(hpbImage - GetImage());
        //ptrkevent->m_smti.m_cbLength = DWORDSWAP(pCh->dwLength);
        ptrkevent->m_smti.m_cbLength = pch->dwLength;

        if (ptrkevent->m_smti.m_cbLength > dwLeft)
        {
            TRACE( "Track longer than file!");
            return InvalidFile;
        }

        dwLeft   -= ptrkevent->m_smti.m_cbLength;
        hpbImage += ptrkevent->m_smti.m_cbLength;

        ptrkevent->Initialize(m_ptracks);
        //pTrk++;
    }

   // Search for additional XF tracks...

    /*for (idx=m_dwTracks; idx< m_dwTracks + 2 ; idx++)
    {
        if (dwLeft < sizeof(CHUNKHDR))
            break;

        pCh = (CHUNKHDR FAR *)hpbImage;

        dwLeft   -= sizeof(CHUNKHDR);
        hpbImage += sizeof(CHUNKHDR);

      if(pCh->fourccType == FOURCC_XFIH ||
         pCh->fourccType == FOURCC_XFKM)
      {
         ptrkevent = new Star350EventTrack;
         m_ptracks->add(ptrkevent);
         ptrkevent->m_idxTrack      = (DWORD)(hpbImage - GetImage());
         ptrkevent->m_smti.m_cbLength = DWORDSWAP(pCh->dwLength);

         if (ptrkevent->m_smti.m_cbLength > dwLeft)
            {
               TRACE("Track longer than file!");
               return InvalidFile;
            }

            dwLeft   -= ptrkevent->m_smti.m_cbLength;
         hpbImage += ptrkevent->m_smti.m_cbLength;

            ptrkevent->Initialize(m_ptracks);
      }
      else
      {
            break;
            ASSERT(0);
         idxTrack = (DWORD)(hpbImage - GetImage());
         cbLength = DWORDSWAP(pCh->dwLength);

         if (cbLength > dwLeft)
            {
               TRACE( "Track longer than file!");
               return InvalidFile;
            }
         dwLeft   -= cbLength;
         hpbImage += cbLength;
      }

        //pTrk++;
    }*/

   //m_dwTracks = idx;
   //m_dwTracks = m_ptracks->get_size();


    /* File looks OK. Now preparse, doing the following:
    ** (1) Build tempo ::collection::map so we can convert to/from ticks quickly
    ** (2) Determine actual tick length of file
    ** (3) Validate all events in all tracks
    */
    m_ptracks->m_tkPosition = 0;
    m_fdwSMF &= ~FlagEof;


    //for (pTrk = m_rTracks, idxTrack = m_dwTracks; idxTrack--; pTrk++)
//   for (idxTrack = m_dwTracks; idxTrack--;)
  //  {
   //   pTrk = m_tracks[idxTrack];
      //  pTrk->pSmf              = this;
//        pTrk->tkPosition        = 0;
//        pTrk->cbLeft            = pTrk->smti.cbLength;
//        pTrk->hpbImage          = GetImage() + pTrk->idxTrack;
//        pTrk->bRunningStatus    = 0;
//        pTrk->fdwTrack          = 0;
//    }

   m_ptracks->seek_begin();

/*   if(m_iOpenMode == OpenForPlaying)
   {
        while (Success == (smfrc = GetNextEvent((Star350EventV001 *)&event, MAX_TICKS, TRUE)))
        {
        if (::mus::midi::Meta == event.m_abEvent[0] &&
            ::mus::midi::MetaTempo == event.m_abEvent[1])
        {
            if (3 != event.m_cbParm)
            {
                return InvalidFile;
            }

            if (fFirst && m_ptracks->m_tkPosition != 0)
            {
                /* Inserting first event and the absolute time is zero.
                ** Use defaults of 500,000 uSec/qn from MIDI spec
                */

//            pTempo = new CMidiTempoMapEntry();
/*                CMidiTempoMapEntry tempo;
                tempo.tkTempo = 0;
                tempo.msBase  = 0;
                tempo.dwTempo = ::mus::midi::DefaultTempo;
//            m_tempoMaps.add(tempo);



                fFirst = FALSE;
            }

//            pTempo = &m_pTempoMap[m_cTempoMap++];
         m_tempoMaps.IncrementSize(1);
            pTempo = &m_tempoMaps.last_element();
            pTempo->tkTempo = m_ptracks->m_tkPosition;
            if (fFirst)
                pTempo->msBase = 0;
            else
            {
            CMidiTempoMapEntry * pLastTempo = &m_tempoMaps.element_at(m_tempoMaps.get_size() - 2) ;
                /* NOTE: Better not be here unless we're q/n format!
                */
//                pTempo->msBase = (pTempo-1)->msBase +
  //                               muldiv32(pTempo->tkTempo-((pTempo-1)->tkTempo),
    //                                      (pTempo-1)->dwTempo,
      //                                    1000L*m_dwTimeDivision);
/*                      pTempo->msBase = (pLastTempo)->msBase +
                                 muldiv32(pTempo->tkTempo-((pLastTempo)->tkTempo),
                                        (pLastTempo)->dwTempo,
                                      1000L*m_dwTimeDivision);
            }
            pTempo->dwTempo = (((DWORD)event.m_hpbParm[0])<<16)|
                              (((DWORD)event.m_hpbParm[1])<<8)|
                              ((DWORD)event.m_hpbParm[2]);
        }
        }
    }
    else*/ if(m_iOpenMode == OpenForParsingLevel2)
   {
        while (Success == (smfrc = GetNextEvent((Star350EventV001 *)&event, MAX_TICKS, TRUE)))
        {
        }
   }
/*    if(m_iOpenMode == OpenForPlaying)
    {
          //if (0 == m_cTempoMap)
       if (0 == m_tempoMaps.get_size())
       {
          //TRACE("File contains no tempo ::collection::map! Insert default tempo.");

          //hLocal = LocalAlloc(LHND, sizeof(TEMPOMAPENTRY));
          //hLocal = new TEMPOMAPENTRY();
          //pTempo = new CMidiTempoMapEntry();
          //if (!hLocal)
    //      if(!pTempo)
    //         return NoMemory;


          //m_pTempoMap = (PTEMPOMAPENTRY)LocalLock(m_hTempoMap = hLocal);
       //   m_pTempoMap = (PTEMPOMAPENTRY) (m_hTempoMap = hLocal);
         //  m_cTempoMap = 1;
          //m_cTempoMapAlloc = 1;
          m_tempoMaps.IncrementSize(1);

            pTempo = &m_tempoMaps.last_element();
          //m_pTempoMap->tkTempo = 0;
          //m_pTempoMap->msBase  = 0;
          //m_pTempoMap->dwTempo = MIDI_DEFAULT_TEMPO;
          pTempo->tkTempo = 0;
          pTempo->msBase  = 0;
          pTempo->dwTempo = ::mus::midi::DefaultTempo;
       }
    }*/

    hpbImage = hpbEvents + 4 + *(DWORD*)hpbEvents;
    hpbImage += 18;
    for(DWORD dw = 0; dw < m_dwLyricTrack; dw++)
    {
        hpbImage += 4 + *(DWORD*)hpbImage;
    }
    string str((CHAR *) (hpbImage + sizeof(DWORD)), *(DWORD*)hpbImage);
    m_strLyrics = str;



    if (EndOfFile == smfrc || Success == smfrc)
    {
        m_tkLength = m_ptracks->m_tkPosition;
        smfrc = Success;
   }
//   else
//   {
//         smfrc = Success;
//   }

    return smfrc;
}


SMFRESULT Star350File::GetNextEvent(
    midi_event_base *       pEvent,
    imedia::position                   tkMax,
   BOOL               bTkMaxInclusive)
{
   if (m_fdwSMF & FlagEof)
    {
        return EndOfFile;
    }
/*    if(m_meaImmediate.get_size() > 0)
    {
        CMidiEventV016 & event = m_meaImmediate.element_at(0);
        *pEvent = event;
        m_meaImmediate.remove_at(0);
        return Star350File::Success;
    }*/
    /*if(m_meaOnQuarterNote.get_size() > 0)
    {
        imedia::position tkMod = m_ptracks->m_tkPosition  % WORDSWAP(m_pevheader->wDivision);
        imedia::position tkQuarterNote;
        if(tkMod == 0)
        {
            tkQuarterNote = m_ptracks->m_tkPosition;
        }
        else
        {
            tkQuarterNote = m_ptracks->m_tkPosition + WORDSWAP(m_pevheader->wDivision) - tkMod;
        }
        imedia::position tkPosition;
        SMFRESULT smfrc = GetNextEventTkPosition(&tkPosition, tkMax);
       if(smfrc == EndOfFile)
          m_fdwSMF |= FlagEof;
        if(tkPosition > tkQuarterNote)
        {
            m_ptracks->m_tkPosition  = tkQuarterNote;
            *pEvent = m_meaOnQuarterNote.element_at(0);
            m_meaOnQuarterNote.remove_at(0);
            return VMSR_SUCCESS;
        }
    }*/
    SMFRESULT smfrc   = m_ptracks->GetNextEvent(
       pEvent,
      tkMax,
      bTkMaxInclusive,
        false,
        false);
   if(smfrc == EndOfFile)
      m_fdwSMF |= FlagEof;
    return smfrc;



}

SMFRESULT Star350File::GetNextEventTkPosition(
      imedia::position * pTkPosition,
      imedia::position tkMax)
{

   if (m_fdwSMF & FlagEof)
    {
        return EndOfFile;
    }

   return m_ptracks->GetNextEventTkPosition(
      pTkPosition,
      tkMax);
}




/*
*   mysmfGetTrackEventData
*
*
*
*
****/
DWORD mysmfGetTrackEventData(
    Star350File * pSmf,
   midi_event_v001 *      pEvent,
    BYTE *            pData[])
{
   DWORD               len;
   byte *                  hpbImage;
   DWORD index;

    ASSERT(pSmf != NULL);
    ASSERT(pEvent != NULL);

   len = pEvent->m_cbParm;

   *pData = (unsigned char *) LocalAlloc(LPTR,len);

   hpbImage = pEvent->m_hpbParm;

   for(index = 0; len; index++)
   {
      *pData[index] = *hpbImage++;
   }

    return len;
}


//SMFRESULT Star350File::MergeTracks(
//   CHUNKHDR *   pCh,
//   MIDIFILEHDR *   pFh)
/*SMFRESULT Star350File::MergeTracks()
{
   int i;
   DWORD dwAllocation = 0;
    CMidiTrackBase * pTrk;
    CMidiTrack * ptrkevent;
   for(i = 0; i < m_ptracks->get_size(); i++)
   {
        pTrk = m_ptracks->element_at(i);
        if(base < CMidiTrack >::bases(pTrk))
        {
            ptrkevent = (CMidiTrack *) pTrk;
          dwAllocation += ptrkevent->GetImageLength();
            ptrkevent->SetAutoAllocation();
        }
   }
   dwAllocation += sizeof(CHUNKHDR) + sizeof(MIDIFILEHDR);
//   ASSERT(GetImage() == NULL);
   if(!allocate(dwAllocation))
   {
      return NoMemory;
   }

//   if((GetImage() = (byte *) HeapAlloc(GetProcessHeap(), 0, dwAllocation)) == NULL)
//   {
//      return NoMemory;
//   }
   byte * hpbDest, hpbSrc;
   hpbDest = GetImage();

    CHUNKHDR * lpch = (CHUNKHDR *) hpbDest;
    lpch->fourccType = FOURCC_MThd;
    lpch->dwLength = DWORDSWAP(dwAllocation);

//   memcpy(hpbDest, pCh, sizeof(CHUNKHDR));
   hpbDest += sizeof(CHUNKHDR);
//   memcpy(hpbDest, pFh, sizeof(MIDIFILEHDR));
   hpbDest += sizeof(MIDIFILEHDR);

   for(i = 0; i < m_ptracks->get_size(); i++)
   {
        pTrk = m_ptracks->element_at(i);
        if(base < CMidiTrack >::bases(pTrk))
        {
            ptrkevent = (CMidiTrack *) pTrk;
          hpbSrc = ptrkevent->GetAllocationImage();
          memcpy(hpbDest, hpbSrc, ptrkevent->GetImageLength());
            ptrkevent->m_idxTrack = hpbDest - GetImage() + sizeof(CHUNKHDR);
            ptrkevent->Initialize(&m_tracks);
          ptrkevent->SetAutoAllocation(FALSE);
            ptrkevent->WriteHeaderLength();
          hpbDest += ptrkevent->GetImageLength();
        }
   }
//   for(i = 0; i < m_ptracks->get_size(); i++)
//   {
//      pTrk = &m_ptracks->element_at(i);
//      delete pTrk;
//   }
   m_ptracks->remove_all();
//   CHUNKHDR *pCh;
   m_cbImage = dwAllocation;
   SetOpened();
   return Success;

}*/

//SMFRESULT Star350File::MergeWorkTracks(
//   CHUNKHDR *   pCh,
//   MIDIFILEHDR *   pFh)
/*SMFRESULT Star350File::WorkMergeTracks()
{
   CMidiTrackBase * pTrk;
    CMidiTrack * ptrkevent;
    for(int i = 0; i < m_ptracks->get_size(); i++)
   {
        pTrk = m_ptracks->element_at(i);
        if(base < CMidiTrack >::bases(pTrk))
        {
            ptrkevent = (CMidiTrack *) pTrk;
          ptrkevent->FromWorkStorage();
        }
   }
    //return MergeTracks(pCh, pFh);
    return MergeTracks();
}*/


//int Star350File::GetFormat()
//{
  //  ASSERT(FALSE);
////   return WORDSWAP(m_pevheader->wFormat);
//}

/*int Star350File::CalcMelodyTrack(CMidiEventsV1 **ppEvents, CTickArray *pTicks)
{
   return m_ptracks->CalcMelodyTrack(ppEvents, pTicks, m_pevheader->wFormat);
}*/

/*void Star350File::TicksToMillisecs(C2DDWordArray *p2DMillis, C2DTickArray *p2DTicks, int tkOffset)
{
   ASSERT(p2DMillis->get_size() == 0);
   p2DMillis->set_size(p2DTicks->get_size());
   int i;
   for(i = 0; i < p2DTicks->get_size(); i++)
   {
      TicksToMillisecs(
         p2DMillis->operator [](i),
         p2DTicks->operator [](i),
         tkOffset);
   }

}*/

/*SMFRESULT Star350File::SaveFile(const char * lpcFilePath)
{
   HMMIO                   hmmio = (HMMIO)NULL;
    SMFRESULT               smfrc = Success;
    MMIOINFO                mmioinfo;

   ASSERT(lpcFilePath != NULL);

   LPTSTR lpFilePath = _tcsdup(lpcFilePath);

    /* Verify that the file can be opened or created
    */
    /*memset(&mmioinfo, 0, sizeof(mmioinfo));

   hmmio = mmioOpen(lpFilePath, &mmioinfo, MMIO_WRITE |MMIO_ALLOCBUF);
    if ((HMMIO)NULL == hmmio)
    {
        TRACE(_T( "smfOpenFile: mmioOpen failed!"));
        return SaveFailed;
    }

   if(mmioWrite(hmmio, (char _huge* ) GetImage(), m_cbImage) != (int) m_cbImage)
   {
        TRACE(_T( "smfOpenFile: mmioOpen failed!"));
        return SaveFailed;
   }

   return Success;
}*/

SMFRESULT Star350File::SaveFile(ex1::file &ar)
{
   ar.write(GetImage(), GetImageSize());
   return Success;
}

//bool Star350File::IsSoftKaraokeFile(CStdioFile * lpFile)
//{
//   return m_ptracks->IsSoftKaraokeFile(lpFile);
//}

/*SMFRESULT Star350File::DeleteEvent(midi_event_base *pEvent)
{
   SMFRESULT   smfrc;
   if(pEvent->GetImage() > 0)
   {
      if(pEvent->GetDelta() != 0)
      {
         // if this event being deleted has non-zero delta,
         // the previous event must have it delta corrected;
         midi_event_v001   previousEvent;
         if(Success != (smfrc = GetPreviousEvent(
            &previousEvent,
            pEvent)))
         {
            return smfrc;
         }
         if(Success != (smfrc = ChangeEventDelta(
            &previousEvent,
            previousEvent.m_tkDelta + pEvent->GetDelta())))
         {
            return smfrc;
         }
      }
      CMidiTrack * pTrack = m_ptracks->GetEventTrack(pEvent);
      if(pTrack == NULL)
         return TrackNotFound;
      pTrack->m_smti.m_cbLength -= pEvent->GetImageSize();
      pTrack->WriteHeaderLength();
      m_cbImage -= pEvent->GetImageSize();
      memmove(
         pEvent->GetImage(),
         pEvent->GetImage() + pEvent->GetImageSize(),
         m_cbImage - (pEvent->GetImage() - GetImage()) - pEvent->GetImageSize());
      if(Success != (smfrc = BuildIndex()))
      {
         return smfrc;
      }

      return Success;
   }
   else
   {
      return Success;
   }

} */

VMSRESULT Star350File::GetPreviousEvent(
   midi_event_base  * pPreviousEvent,
   midi_event_base  * pEvent)
{
   UNREFERENCED_PARAMETER(pPreviousEvent);
    SMFRESULT               smfrc;
    Star350EventV001          event;
   Star350EventV001          previousEvent;

    m_ptracks->m_tkPosition = 0;
    m_fdwSMF &= ~FlagEof;
   m_ptracks->m_fdwSMF &= ~FlagEof;

   m_ptracks->seek_begin();
    //for (ptrk = m_rTracks, idxTrack = m_dwTracks; idxTrack--; ptrk++)

    while(Success == (smfrc = GetNextEvent(&event, pEvent->get_position(), TRUE)))
   {
      if(event.GetImage() == pEvent->GetImage())
      {
         pEvent->operator =(previousEvent);
         return Success;
      }
      previousEvent = event;
   }

    if (ReachedTkMax != smfrc)
    {
        return smfrc;
    }

    return Success;

}
/*BOOL Star350File::allocate_add_up(DWORD dwAddUp)
{
   return allocate(m_cbImage + dwAddUp);
}

BOOL Star350File::allocate(DWORD dwNewLength)
{
   if(GetImage() == NULL)
   {
      GetImage() = (byte *) malloc(dwNewLength);
      if(GetImage() != NULL)
      {
         m_dwAllocation = dwNewLength;
         return TRUE;
      }
      else
      {
         return FALSE;
      }
   }
   else
   {
      if(dwNewLength > m_dwAllocation)
      {
         DWORD dwAllocation = dwNewLength + m_dwAllocationAddUp;
         LPVOID lpVoid = realloc(GetImage(), dwAllocation);
         if(lpVoid == NULL)
         {
            DWORD dw = GetLastError();
            TRACELASTERROR();
            return FALSE;
         }
         else
         {
                int iOffset = (LPBYTE) lpVoid - GetImage();
                m_pchHeader += iOffset;
                m_pevheader += iOffset;
            m_dwAllocation = dwAllocation;
            GetImage() = (byte *) lpVoid;
            return TRUE;
         }
      }
      else
      {
         return TRUE;
      }

   }
}*/

/*SMFRESULT Star350File::ReplaceSameDeltaEvent(midi_event_base &eventNew)
{
    int iCurrentTrack = m_ptracks->m_iCurrentTrack;
    if(iCurrentTrack < 0)
        return VMSR_E_FAIL;
    CMidiTrack * ptrkevent = (CMidiTrack *) m_ptracks->element_at(iCurrentTrack);
    ASSERT(base < CMidiTrack >::bases(ptrkevent));
    int iCurrentEvent = ptrkevent->GetCurrentEvent();
    if(iCurrentEvent < 0)
        return VMSR_E_FAIL;

    CMidiTrackV008 & trackWork = ptrkevent->GetWorkTrack();

    midi_event_v008 & event = trackWork.m_events.element_at(iCurrentEvent);

    event = eventNew;
    return Star350File::Success;
}*/
/*SMFRESULT Star350File::ReplaceSameDeltaEvent(midi_event_base *pEventOld, CMidiEventV007 *pEventNew)
{
   ASSERT(pEventOld->GetDeltaTicks() == pEventNew->GetDeltaTicks());
   ASSERT(pEventNew->IsAutoAllocated());
   CMidiTrack * pTrack = m_ptracks->GetEventTrack(pEventOld);
   if(pTrack == NULL)
      return TrackNotFound;
   byte * hpbImage = pEventOld->GetImage();
   byte * hpbFileImage = GetImage();
   if(pEventOld->GetImageSize() != pEventNew->m_cbImage)
   {
      int iAddUp = pEventNew->m_cbImage - pEventOld->GetImageSize();
      pEventNew->m_pTrack = pTrack;
      allocate_add_up(iAddUp);
      hpbImage += hpbFileImage - GetImage();
      pTrack->m_smti.m_cbLength += iAddUp;
      pTrack->WriteHeaderLength();
      memmove(
         hpbImage + iAddUp,
         hpbImage,
         m_cbImage - (hpbImage - GetImage()));
      m_cbImage += iAddUp;
   }
   memcpy(hpbImage, pEventNew->GetImage(), pEventNew->m_cbImage);

   WriteHeader();

   return Success;*/

/*   ASSERT(pEventOld->GetDeltaTicks() == pEventNew->GetDeltaTicks());
   ASSERT(pEventNew->IsAutoAllocated());
   CMidiTrack * pTrack = m_ptracks->GetEventTrack(pEventOld);
   if(pTrack == NULL)
      return TrackNotFound;
   byte * hpbImage = pEventOld->GetImage();
   byte * hpbFileImage = GetImage();
   if(pEventOld->GetImageSize() != pEventNew->m_cbImage)
   {
      int iAddUp = pEventNew->m_cbImage - pEventOld->GetImageSize();
      pEventNew->m_pTrack = pTrack;
      allocate_add_up(iAddUp);
      hpbImage += hpbFileImage - GetImage();
      pTrack->m_smti.m_cbLength += iAddUp;
      pTrack->WriteHeaderLength();
      memmove(
         hpbImage + iAddUp,
         hpbImage,
         m_cbImage - (hpbImage - GetImage()));
      m_cbImage += iAddUp;
   }
   memcpy(hpbImage, pEventNew->GetImage(), pEventNew->m_cbImage);

   WriteHeader();

   return Success;

}*/

/*SMFRESULT Star350File::ChangeEventDelta(midi_event_base *pEvent, imedia::position tkNewDelta)
{
   SMFRESULT smfrc;
   if(pEvent->GetDelta() != tkNewDelta)
   {
      CMidiTrack * pTrack = m_ptracks->GetEventTrack(pEvent);
      if(pTrack == NULL)
         return TrackNotFound;
      BYTE buffer[256];
      DWORD dwSetUsed = midi_event_base::SetVDWord(buffer, 256, tkNewDelta);
      imedia::position tkOldDelta;
      DWORD dwGetUsed = midi_event_base::GetVDWord(pEvent->GetImage(), 256, &tkOldDelta);
      ASSERT(tkOldDelta == pEvent->GetDelta());
      byte * hpbImage = pEvent->GetImage();
      if(dwSetUsed != dwGetUsed)
      {
         int iAddUp = dwSetUsed - dwGetUsed;
         if(iAddUp > 0)
         {
            if(!allocate(m_cbImage + iAddUp))
            {
               TRACE("Star350File::DeleteEvent No Memory for deleting\n");
               return NoMemory;
            }
         }
         pTrack->m_smti.m_cbLength += iAddUp;
         pTrack->WriteHeaderLength();
         memmove(
            hpbImage + iAddUp,
            hpbImage,
            m_cbImage - (hpbImage - GetImage()));
         m_cbImage += iAddUp;
         if(Success != (smfrc = BuildIndex()))
         {
            return smfrc;
         }
      }
      midi_event_base::SetVDWord(hpbImage, dwSetUsed, tkNewDelta);
      return Success;
   }
   else
      return Star350File::Success;

}*/

/*SMFRESULT Star350File::WriteHeader(MIDIFILEHDR * lpmfh)
{
   m_pMThd = (CHUNKHDR *) GetImage();
//   DWORD dwLength = m_cbImage - sizeof(CHUNKHDR);
  //  m_pMThd->dwLength = DWORDSWAP(dwLength);
    m_pMThd->fourccType = FOURCC_MThd;
    m_pMThd->dwLength = DWORDSWAP(sizeof(MIDIFILEHDR));
    m_pevheader = (LPMIDIFILEHDR) (GetImage() + sizeof(CHUNKHDR));
    memcpy(m_pevheader, lpmfh, sizeof(MIDIFILEHDR));
   return Success;
}*/



/*bool Star350File::IsXFFile()
{
    m_ptracks->seek_begin();
    return m_ptracks->IsXFFile();
}*/

/*int Star350File::SetKeyShift(int iKeyShift)
{
    m_iKeyShift = iKeyShift;
    return true;
}*/

/*int Star350File::GetKeyShift()
{
    return m_iKeyShift;
}*/


/*void Star350File::FromWorkStorage()
{
    m_ptracks->FromWorkStorage();

}*/
void Star350File::ToWorkStorage()
{
    m_ptracks->ToWorkStorage();
}

/*VMSRESULT Star350File::SetTempoShift(int iTempoShift)
{
    m_iTempoShift = iTempoShift;
    return VMSR_SUCCESS;
}*/

/*int Star350File::GetTempoShift()
{
    return m_iTempoShift;
}*/

/*double Star350File::GetTempoShiftRate()
{
    return exp((double) m_iTempoShift * log(3.0) / 10.0);
}*/


/*CMidiTempoMapEntry * Star350File::GetTempoMapEntry(imedia::position tk)
{

    for (int idx = 0; idx < m_tempoMaps.get_size(); idx++)
   {
      CMidiTempoMapEntry & tempo = m_tempoMaps.element_at(idx);
        if (tk < tempo.tkTempo)
            break;
   }
   if(idx <= 0)
      return NULL;
   else
      return &m_tempoMaps.element_at(idx - 1);
}*/

/*VMSRESULT Star350File::InsertTempoEvent()
{
    CMidiTempoMapEntry * pmtme = GetTempoMapEntry(m_ptracks->m_tkPosition);
    if(pmtme != NULL)
    {
        CMidiEventV016 event;
        double dTempo;
        dTempo = pmtme->dwTempo;
    //    dTempo = dTempo / m_dTempoShift;
        union
        {
            DWORD   dw;
            BYTE    ba[4];
        } uA, uB;
        uA.dw = (DWORD) dTempo;
        uB.ba[0] = uA.ba[2];
        uB.ba[1] = uA.ba[1];
        uB.ba[2] = uA.ba[0];

        event.SetFullType(::mus::midi::Meta);
        event.SetMetaType(::mus::midi::MetaTempo);
        event.SetParam(&uB.dw, 3);
        m_meaOnQuarterNote.add(event);

    }
    return VMSR_SUCCESS;

}*/

/*VMSRESULT Star350File::Format0()
{
    MIDIFILEHDR fileHdr;

    fileHdr.wFormat = WORDSWAP(0);
   fileHdr.wDivision = m_pevheader->wDivision ;
    fileHdr.wTracks = WORDSWAP(1) ;

      CMidiTrack * pTrackUnion = new CMidiTrack();
    pTrackUnion->Initialize(&m_tracks);
    m_ptracks->CompactTracks(*pTrackUnion, true, true);
    CMidiTrackBase * pTrk;
    for(int i = 0; i < m_ptracks->get_size();)
    {
        pTrk = m_ptracks->operator [](i);
        if(base < CMidiTrack >::bases(pTrk))
        {
            CMidiTrack * pTrack = (CMidiTrack *) pTrk;
            CHUNKHDR FAR * pCh = (CHUNKHDR FAR *)pTrack->GetImage();
            if (pCh->fourccType == FOURCC_MTrk)
            {
                m_ptracks->remove_at(i);
              continue;
            }
        }
        i++;
    }
    m_ptracks->insert_at(0, pTrackUnion);

    SMFRESULT smfrc;
    if(MMSYSERR_NOERROR != (smfrc = MergeTracks()))
        return smfrc;

    WriteHeader(&fileHdr);

    if(MMSYSERR_NOERROR != (smfrc = BuildIndex()))
        return smfrc;

    return VMSR_SUCCESS;
}*/

bool Star350File::IsStar350(const char * lpcszPath)
{
   ex1::filesp spfile(get_app());

   spfile->open(lpcszPath,
        ::ex1::file::mode_read |
        ::ex1::file::shareDenyWrite |
        ::ex1::file::type_binary);

   bool bResult = OpenFile(spfile, OpenForIdentifying) !=
               InvalidFile;

   CloseFile();

   return bResult;
}

bool Star350File::IsStar350(primitive::memory & storage)
{
   bool bResult = OpenFile(
                     &storage,
                     OpenForIdentifying,
                     mus::StorageAttach) !=
            InvalidFile;

   CloseFile();

   return bResult;

}

/*bool Star350File::IsStar350()
{
   SMFRESULT               smfrc = Success;
    UNALIGNED CHUNKHDR *    pCh;
//    FILEHDR FAR *           pFh;
    DWORD                   idx;
    DWORD                   dwLeft;
    byte *                  hpbImage;
//    DWORD                   idxTrack;
    BOOL                    fFirst = TRUE;
    DWORD                   dwLength;
//    CMidiTrack *            pTrk = NULL;
    CMidiTrackBase *        pTrk = NULL;
    CMidiTrack *            ptrkevent;
   midi_event_v001          event;
   CMidiTempoMapEntry *    pTempo;
   DWORD               idxTrack;
   DWORD               cbLength;



//    assert(ppSmf != NULL);

//    pSmf = *ppSmf;

//    assert(pSmf != NULL);

    /* MIDI data image is already in hpbImage (already extracted from
    ** RIFF header if necessary).
    */

    /* Validate MIDI header
    */
/*    dwLeft   = m_cbImage;
    hpbImage = GetImage();

    if (dwLeft < sizeof(m_lpchHeader))
        return fa;

    m_pHeader = (char *) hpbImage;

    dwLeft   -= sizeof(m_lpchHeader);
    hpbImage += sizeof(m_lpchHeader);

    if(memcmp(
        m_lpchHeader,
        m_pHeader,
        sizeof(m_lpchHeader))
            != 0)
        return InvalidFile;
}*/

void Star350File::GetLyricsText(string &str)
{
    str = m_strLyrics;
}

Star350InfoHeader * Star350File::GetInfoHeader()
{
    return m_pinfoheader;
}

int Star350File::GetLyricTrack()
{
    return m_dwLyricTrack;
}

LPBYTE Star350File::GetImage()
{
    return get_data();
}

DWORD Star350File::GetImageSize()
{
    return this->get_size();
}


bool Star350File::IsOpened()
{
   return (m_fdwSMF & FlagOpened) > 0;
}

void Star350File::SetOpened(bool bOpened)
{
   if(bOpened)
   {
      m_fdwSMF |= FlagOpened;
   }
   else
   {
      m_fdwSMF &= ~FlagOpened;
   }
}

Star350Tracks & Star350File::GetTracks()
{
   return * m_ptracks;
}

