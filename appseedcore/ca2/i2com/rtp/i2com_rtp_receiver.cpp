#include "stdafx.h"



namespace i2com
{


	namespace rtp
	{


		receiver::receiver(::ca::application * papp) :
			ca(papp),
         m_memfileRawPcm(papp),
			m_transferfile(papp, &m_memfileRawPcm, &m_mutexRaw)
		{
         m_transferfile.m_ptimeoutfile->m_dwTimeOut = 0;
         m_uiReadPosition = 0;
         m_uiRawPosition = 0;
		}

	   void receiver::start_receiving(room * proom)
	   {
		   m_proom  = proom;
         m_iPort  = proom->m_pserver->get_free_rtp_port();
         if(m_iPort < 0)
            return;
         proom->m_pserver->m_mapRtpPort[m_iPort] = proom;
		   AfxBeginThread(get_app(), &thread_proc_receive, (LPVOID) this);
	   }

	   UINT receiver::thread_proc_receive(LPVOID lpparam)
		{
			receiver * preceiver = (receiver *) lpparam;
			preceiver->receive();
			return 0;
		}

	   void receiver::receive()
		{
         m_prtpfile = new ::rtp::file(get_app());
         if(m_prtpfile->rx_open("0.0.0.0", m_iPort))
            return;
			primitive::memory memMp3;
			memMp3.allocate(1024);
			primitive::memory memRaw;
			memRaw.allocate(1024);
         gen::memory_file fileBuf(get_app());
			::primitive::memory_size uiRead;
//			file_position uiPosition;

         audio_decode::decoder_plugin_set decoderset(get_app());

         audio_decode::decoder_plugin * pdecoderplugin = decoderset.LoadPlugin("audio_decode_libmpg123.dll");

         audio_decode::decoder * pdecoder = pdecoderplugin->NewDecoder();

         pdecoder->DecoderInitialize(m_prtpfile);

         if(pdecoder->DecoderSetSeekable(false))
         {
            delete pdecoder;
            return;
         }
         if(pdecoder->DecoderSetReadBlockSize(1024) != 1024)
         {
            delete pdecoder;
            return;
         }


         int iSize = 1024 * 2 * 2 * 4;

         char * bufferWav = (char *) malloc(iSize);
//         char * bufferMp3 = (char *) malloc(iSize * 128);

			while(true)
			{
            uiRead = pdecoder->DecoderFillBuffer(bufferWav, iSize);
            m_transferfile.m_pmemoryfileIn->write(bufferWav, uiRead);
			}
		}

      UINT receiver::read(void * p, count c)
      {
         single_lock sl(&m_mutexRaw, TRUE);

         ::ex1::file * pfile = m_transferfile.m_pmemoryfileOut;

         primitive::memory mem;
         mem.allocate(1024);
         UINT uiRead;
         while(m_uiReadPosition > m_uiRawPosition &&
            (uiRead = pfile->read(mem, min(mem.get_size(), m_uiReadPosition - m_uiRawPosition))) > 0)
         {
            m_uiReadPosition += uiRead;
         }
         m_uiReadPosition += c;
         return m_memfileRawPcm.read(p, c);


      }


	} // 	namespace rtp


} // namespace i2com

