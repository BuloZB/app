#include "stdafx.h"



namespace i2com
{


	namespace rtp
	{


		transmitter::transmitter(::ca::application * papp) :
			ca(papp),
         m_memfileMp3(papp),
         m_transferfile(papp, &m_memfileMp3, &m_mutexMp3)
		{
		}

	   void transmitter::start_transmitting(room * proom)
	   {
		   m_proom  = proom;
         m_iPort  = proom->m_pserver->get_free_rtp_port();
         if(m_iPort < 0)
            return;
         proom->m_pserver->m_mapRtpPort[m_iPort] = proom;
		   AfxBeginThread(get_app(), &thread_proc_transmit, (LPVOID) this);
	   }

	   UINT transmitter::thread_proc_transmit(LPVOID lpparam)
		{
			transmitter * ptransmitter = (transmitter *) lpparam;
			ptransmitter->transmit();
			return 0;
		}

	   void transmitter::transmit()
		{
			m_prtpfile = new ::rtp::file(get_app());
         if(m_prtpfile->rx_open("0.0.0.0", m_iPort))
            return;

         primitive::memory memMp3;
         memMp3.allocate(1024);
         UINT uiRead;

			while(true)
			{
            uiRead = m_transferfile.m_ptimeoutfile->read(memMp3, memMp3.get_size());
            m_prtpfile->write(memMp3, uiRead);
			}
		}

      void transmitter::write(void * p, count c)
      {
         single_lock sl(&m_mutexMp3, TRUE);

         ::ex1::file * pfile = m_transferfile.m_pmemoryfileIn;

         pfile->write(p, c);

      }


	} // 	namespace rtp


} // namespace i2com

