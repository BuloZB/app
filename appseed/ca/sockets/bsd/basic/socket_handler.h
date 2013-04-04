/** \file socket_handler.h
**   \date  2004-02-13
**   \author grymse@alhem.net
**/
/*
Copyright (C) 2004-2007  Anders Hedstrom

This library is made available under the terms of the GNU GPL.

If you would like to use this library in a closed-source application,
a separate license agreement is available. For information about
the closed-source license agreement for the C++ sockets library,
please visit http://www.alhem.net/Sockets/license.html and/or
email license@alhem.net.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#pragma once


   namespace sockets
   {

      class socket;
      class resolv_server;

      /** socket container class, event generator.
      \ingroup basic */
      class CLASS_DECL_ca socket_handler : public socket_handler_base
      {
      protected:
         /** Map type for holding file descriptors/socket object pointers. */


      public:
         /** socket_handler constructor.
         \param log Optional log class pointer */
         socket_handler(::ca::application * papp, StdLog *log = ::null());

         /** socket_handler threadsafe constructor.
         \param mutex Externally declared mutex var
         \param log Optional log class pointer */
         socket_handler(::ca::application * papp, mutex & mutex,StdLog *log = ::null());

         ~socket_handler();

         /** get mutex reference for threadsafe operations. */
         mutex & GetMutex() const;

         /** Register StdLog object for error callback.
         \param log Pointer to log class */
         void RegStdLog(StdLog *log);

         /** Log error to log class for print out / storage. */
         void LogError(socket *p,const string & user_text,int32_t err,const string & sys_err, ::ca::log::e_level elevel = ::ca_get_level_warning());

         /** add socket instance to socket map. Removal is always automatic. */
         void add(socket *);

         /** get status of read/write/exception file descriptor set for a socket. */
         void get(SOCKET s,bool& r,bool& w,bool& e);

         /** set read/write/exception file descriptor sets (fd_set). */
         void set(SOCKET s,bool bRead,bool bWrite,bool bException = true);

         /** Wait for events, generate callbacks. */
         int32_t Select(int32_t sec, int32_t usec);

         /** This method will not return until an event has been detected. */
         int32_t Select();

         /** Wait for events, generate callbacks. */
         int32_t Select(struct timeval *tsel);

         /** Check that a socket really is handled by this socket handler. */
         bool Valid(socket *);

         /** Return number of sockets handled by this handler.  */
         size_t get_count();

         /** Override and return false to deny all incoming connections.
         \param p listen_socket class pointer (use GetPort to identify which one) */
         bool OkToAccept(socket *p);

         /** Called by socket when a socket changes state. */
         void AddList(SOCKET s,list_t which_one,bool add);

         // Connection pool
         /** find available open connection (used by connection pool). */
         socket_handler_base::PoolSocket *FindConnection(int32_t type,const string & protocol,::sockets::address &);
         /** Enable connection pool (by default disabled). */
         void EnablePool(bool x = true);
         /** Check pool status.
         \return true if connection pool is enabled */
         bool PoolEnabled();

         // Socks4
         /** set socks4 server ip that all new tcp sockets should use. */
         void SetSocks4Host(in_addr addr);
         /** set socks4 server hostname that all new tcp sockets should use. */
         void SetSocks4Host(const string & );
         /** set socks4 server port number that all new tcp sockets should use. */
         void SetSocks4Port(port_t);
         /** set optional socks4 userid. */
         void SetSocks4Userid(const string & );
         /** If connection to socks4 server fails, immediately try direct connection to final host. */
         void SetSocks4TryDirect(bool x = true);
         /** get socks4 server ip.
         \return socks4 server ip */
         in_addr GetSocks4Host();
         /** get socks4 port number.
         \return socks4 port number */
         port_t GetSocks4Port();
         /** get socks4 userid (optional).
         \return socks4 userid */
         const string & GetSocks4Userid();
         /** Check status of socks4 try direct flag.
         \return true if direct connection should be tried if connection to socks4 server fails */
         bool Socks4TryDirect();

         // DNS resolve server
         /** Enable asynchronous DNS.
         \param port Listen port of asynchronous dns server */
         void EnableResolver(port_t port = 16667);
         /** Check resolver status.
         \return true if resolver is enabled */
         bool ResolverEnabled();
         /** Queue a dns request.
         \param host Hostname to be resolved
         \param port Port number will be echoed in socket::OnResolved callback */
         int32_t Resolve(socket *,const string & host,port_t port);
         int32_t Resolve6(socket *,const string & host,port_t port);
         /** Do a reverse dns lookup. */
         int32_t Resolve(socket *,in_addr a);
         int32_t Resolve(socket *,in6_addr& a);
         /** get listen port of asynchronous dns server. */
         port_t GetResolverPort();
         /** Resolver thread ready for queries. */
         bool ResolverReady();
         /** Returns true if the socket is waiting for a resolve event. */
         bool Resolving(socket *);

         /** Fetch unique trigger id. */
         int32_t TriggerID(socket *src);
         /** Subscribe socket to trigger id. */
         bool Subscribe(int32_t id, socket *dst);
         /** Unsubscribe socket from trigger id. */
         bool Unsubscribe(int32_t id, socket *dst);
         /** Execute OnTrigger for subscribed sockets.
         \param id Trigger ID
         \param data Data passed from source to destination
         \param erase Empty trigger id source and destination maps if 'true',
         Leave them in place if 'false' - if a trigger should be called many times */
         void Trigger(int32_t id, socket::TriggerData& data, bool erase = true);

         /** Indicates that the handler runs under socket_thread. */
         void SetSlave(bool x = true);
         /** Indicates that the handler runs under socket_thread. */
         bool IsSlave();

         /** Sanity check of those accursed lists. */
         void CheckSanity();

      public:
         socket_map     m_sockets; ///< Active sockets map
         socket_map     m_add; ///< Sockets to be added to sockets map
         socket_list    m_delete; ///< Sockets to be deleted (failed when add)
      protected:
         StdLog *m_stdlog; ///< Registered log class, or ::null()
         mutex * m_pmutex; ///< Thread safety mutex
         bool m_b_use_mutex; ///< mutex correctly initialized

      private:
         void CheckList(socket_id_list&,const string &); ///< Used by CheckSanity
         /** remove socket from socket map, used by socket class. */
         void remove(socket *);
         SOCKET m_maxsock; ///< Highest file descriptor + 1 in active sockets list
         fd_set m_rfds; ///< file descriptor set monitored for read events
         fd_set m_wfds; ///< file descriptor set monitored for write events
         fd_set m_efds; ///< file descriptor set monitored for exceptions
         int32_t m_preverror; ///< debug select() error
         int32_t m_errcnt; ///< debug select() error
         time_t m_tlast; ///< timeout control

         // state lists
         socket_id_list m_fds; ///< Active file descriptor list
         socket_id_list m_fds_erase; ///< File descriptors that are to be erased from m_sockets
         socket_id_list m_fds_callonconnect; ///< checklist CallOnConnect
         socket_id_list m_fds_detach; ///< checklist detach
         socket_id_list m_fds_timeout; ///< checklist timeout
         socket_id_list m_fds_retry; ///< checklist retry client connect
         socket_id_list m_fds_close; ///< checklist close and delete
         in_addr m_socks4_host; ///< Socks4 server host ip
         port_t m_socks4_port; ///< Socks4 server port number
         string m_socks4_userid; ///< Socks4 userid
         bool m_bTryDirect; ///< Try direct connection if socks4 server fails
         int32_t m_resolv_id; ///< Resolver id counter
         resolv_server *m_resolver; ///< Resolver thread pointer
         port_t m_resolver_port; ///< Resolver listen port
         map < socket *, socket *, bool, bool> m_resolve_q; ///< resolve queue
         bool m_b_enable_pool; ///< Connection pool enabled if true
         int32_t m_next_trigger_id; ///< Unique trigger id counter
         map<int32_t, int32_t, socket *, socket *> m_trigger_src; ///< mapping trigger id to source socket
         map<int32_t, int32_t, map<socket *, socket *, bool, bool>, map<socket *, socket *, bool, bool> & > m_trigger_dst; ///< mapping trigger id to destination sockets
         bool m_slave; ///< Indicates that this is a socket_handler_base run in socket_thread
      };


   } // namespace sockets


