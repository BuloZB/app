#pragma once


template < class VALUE, class ARG_VALUE = const VALUE &, class HASH = ::ca::hash < int32_t > , class EQUALS = ::ca::equals_type_arg_type < int32_t, int32_t > >
class int_map :
   virtual public attrib_map < map < int32_t, int32_t, VALUE, ARG_VALUE, HASH, EQUALS > >
{
public:

   int_map(::count nBlockSize = 10);
   int_map(const int_map & map);


   int_map & operator = (const int_map & map);

};

template < class VALUE, class ARG_VALUE, class HASH, class EQUALS >
int_map < VALUE, ARG_VALUE, HASH, EQUALS >::int_map(::count nBlockSize) :
   map < int32_t, int32_t, VALUE, ARG_VALUE, HASH, EQUALS > (nBlockSize)
{
}

template < class VALUE, class ARG_VALUE, class HASH, class EQUALS >
int_map < VALUE, ARG_VALUE, HASH, EQUALS >::int_map(const int_map & map) :
   attrib_map <:: map < int32_t, int32_t, VALUE, ARG_VALUE, HASH, EQUALS > > (map)
{
}

template < class VALUE, class ARG_VALUE, class HASH, class EQUALS >
int_map < VALUE, ARG_VALUE, HASH, EQUALS > & int_map < VALUE, ARG_VALUE, HASH, EQUALS >::operator = (const int_map & map)
{

   if(this != &map)
   {
      attrib_map < ::map < int32_t, int32_t, VALUE, ARG_VALUE, HASH, EQUALS > >::operator = (map);
   }

   return *this;

}


typedef CLASS_DECL_ca int_map < int, int > int_to_int;

typedef CLASS_DECL_ca int_map < string, const string & > int_to_string;


