#pragma once


   namespace sockets
   {

      /** \defgroup http HTTP Sockets */
      /** HTTP request/response base class.
      \ingroup http */
      class CLASS_DECL_ca2 http_socket :
         virtual public tcp_socket
      {
      public:




         http::request        m_request;
         http::response       m_response;

         bool                 m_b_http_1_1;
         bool                 m_b_keepalive;

         bool                 m_bFirst;
         bool                 m_bHeader;
         string               m_strLine;
         bool                 m_bRequest;
         bool                 m_bResponse;
         size_t               m_body_size_left;
         bool                 m_b_chunked;
         size_t               m_chunk_size;
         int32_t                  m_chunk_state;
         string               m_chunk_line;

         int64_t              m_iFirstTime;
         bool                 m_bNoClose;
         bool                 m_bOnlyHeaders;


         http_socket(base_socket_handler& );
         ~http_socket();

         void OnRawData(char *buf,size_t len);
         void OnLine(const string & line);

         /** Callback executes when first line has been received.
         GetMethod, GetUrl/GetUri, and GetHttpVersion are valid when this callback is executed. */
         virtual void OnFirst() = 0;
         /** For each header line this callback is executed.
         \param key Http header name
         \param value Http header value */
         virtual void OnHeader(id idKey,const string & value);
         /** Callback fires when all http headers have been received. */
         virtual void OnHeaderComplete();
         /** Chunk of http body data recevied. */
         virtual void OnData(const char *,size_t) = 0;
         /** The full request/response body has been received. */
         virtual void OnDataComplete() {}

         bool IsRequest();

         bool IsResponse();


         virtual id key_id(string key);
         virtual string id_key(id key);

         /** Send response prepared with calls to methods SetHttpVersion, SetStatus, SetStatusText,
         and AddResponseHeader. */
         void SendResponse();
         /** Send request prepared with calls to methods SetMethod, SetUrl, SetHttpVersion,
         and AddResponseHeader. */
         void SendRequest();

         void SendResponseBody();

         /** Implement this to return your own User-agent string. */
         virtual string MyUseragent();

         /** Parse url. If protocol is https, EnableSSL() will be called. */
         void url_this(const string & url_in,string & protocol,string & host,port_t& port,string & url,string & file);

         /** Transfer coding 'chunked' */
         bool IsChunked() { return m_b_chunked; }

         property & inattr(id idName);
         property_set & inattrs();

         property & inheader(id idName);
         property_set & inheaders();

         property & outattr(id idName);
         property_set & outattrs();

         property & outheader(id idName);
         property_set & outheaders();

         http::request & request();
         http::response & response();


         

         /** Reset state of socket to sucessfully implement keep-alive. */
         virtual void Reset();

         virtual bool http_filter_response_header(id key, string & strValue);

         virtual void client_to_server(http_socket * psocket);


      };



      inline ::http::request & http_socket::request()
      {
         return m_request;
      }

      inline ::http::response & http_socket::response()
      {
         return m_response;
      }

      inline property & http_socket::inattr(id idName)
      {
         return m_request.attr(idName);
      }

      inline property_set & http_socket::inattrs()
      {
         return m_request.attrs();
      }

      inline property & http_socket::outattr(id idName)
      {
         return m_response.attr(idName);
      }

      inline property_set & http_socket::outattrs()
      {
         return m_response.attrs();
      }

      inline property & http_socket::inheader(id idName)
      {
         return m_request.header(idName);
      }

      inline property_set & http_socket::inheaders()
      {
         return m_request.headers();
      }

      inline property & http_socket::outheader(id idName)
      {
         return m_response.header(idName);
      }

      inline property_set & http_socket::outheaders()
      {
         return m_response.headers();
      }

      inline bool http_socket::IsRequest()
      {
         return m_bRequest;
      }


      inline bool http_socket::IsResponse()
      {
         return m_bResponse;
      }







   } // namespace sockets


