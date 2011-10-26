#pragma once


#include "ex1_reader.h"
#include "ex1_writer.h"
#include "ex1_stream.h"



namespace ex1
{


   class byte_output_stream;
   class byte_input_stream;
   class byte_serializable;


   class CLASS_DECL_ca byte_input_stream  :
      virtual public reader
   {
   public:


      byte_input_stream();
      byte_input_stream(reader * preader);
      byte_input_stream(const byte_input_stream & istream);
      virtual ~byte_input_stream();


      byte_input_stream & operator >> (char & ch);
      byte_input_stream & operator >> (unsigned char & uch);
      byte_input_stream & operator >> (short & sh);
      byte_input_stream & operator >> (unsigned short & ush);
      byte_input_stream & operator >> (wchar_t & wch);
      byte_input_stream & operator >> (bool & b);
      byte_input_stream & operator >> (int & i);
      byte_input_stream & operator >> (unsigned int & ui);
#ifdef _WINDOWS
      byte_input_stream & operator >> (long & l);
      byte_input_stream & operator >> (unsigned long & ul);
#endif
      byte_input_stream & operator >> (__int64 & i);
      byte_input_stream & operator >> (uint64_t & ui);
      byte_input_stream & operator >> (float & f);
      byte_input_stream & operator >> (double & d);
      byte_input_stream & operator >> (LPRECT lprect);
      byte_input_stream & operator >> (SIZE & size);
      byte_input_stream & operator >> (::ca::type_info & info);
      byte_input_stream & operator >> (byte_serializable & serializable);

      virtual string get_location() const;

      byte_input_stream & operator = (const byte_input_stream & istream);



   };





   class CLASS_DECL_ca byte_output_stream :
      virtual public writer,
      virtual public out_stream_flush
   {
   public:


      byte_output_stream();
      byte_output_stream(writer * pwriter);
      byte_output_stream(const byte_output_stream & ostream);
      virtual ~byte_output_stream();


      byte_output_stream & operator << (char ch);
      byte_output_stream & operator << (unsigned char uch);
      byte_output_stream & operator << (short sh);
      byte_output_stream & operator << (unsigned short ush);
      byte_output_stream & operator << (wchar_t wch);
      byte_output_stream & operator << (bool b);
      byte_output_stream & operator << (int i);
      byte_output_stream & operator << (unsigned int ui);
#ifdef _WINDOWS
      byte_output_stream & operator << (long i);
      byte_output_stream & operator << (unsigned long i);
#endif
      byte_output_stream & operator << (__int64 i);
      byte_output_stream & operator << (uint64_t ui);
      byte_output_stream & operator << (float f);
      byte_output_stream & operator << (double d);
      byte_output_stream & operator << (LPCRECT lpcrect);
      byte_output_stream & operator << (SIZE & size);
      byte_output_stream & operator << (::ca::type_info & info);
      byte_output_stream & operator << (byte_serializable & serializable);
      byte_output_stream & operator << (const char * psz);

      virtual string get_location() const;

      byte_output_stream & operator = (const byte_output_stream & ostream);

   };

   class CLASS_DECL_ca byte_stream :
      virtual public stream,
      virtual public byte_input_stream,
      virtual public byte_output_stream
   {
   public:


      byte_stream();
      byte_stream(const filesp & filesp);
      byte_stream(reader * preader, writer * pwriter);
      byte_stream(const byte_stream & stream);
      virtual ~byte_stream();


      virtual string get_location() const;

      byte_stream & operator = (const byte_stream & stream);


   };


   CLASS_DECL_ca HRESULT ReadStream(reader * stream, void * data, ::primitive::memory_size * size);
   CLASS_DECL_ca HRESULT ReadStream_FALSE(reader * stream, void * data, ::primitive::memory_size size);
   CLASS_DECL_ca HRESULT ReadStream_FAIL(reader * stream, void * data, ::primitive::memory_size size);
   CLASS_DECL_ca HRESULT WriteStream(writer * stream, const void * data, ::primitive::memory_size size);

} // namespace ex1




