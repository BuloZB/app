#pragma once


template < typename T > class numeric_array;
typedef CLASS_DECL_AURA numeric_array < index > index_array;


namespace constructor
{


   template < class TYPE >
   class def
   {
   public:

      inline static void construct(void * pvoid)
      {
         ::new(pvoid) TYPE;
      }
      inline static void construct(void * pvoid, ::count c)
      {
         TYPE * p = (TYPE *) pvoid;
         while(c > 0)
         {
            ::new((void *) p) TYPE;
            p++;
            c--;
         }
      }



   };


   template < class TYPE >
   class zero
   {
   public:

      inline static void construct(void * pvoid)
      {

         ::zero(pvoid, sizeof(TYPE));

      }

      inline static void construct(void * pvoid, ::count c)
      {

         TYPE * p = (TYPE *)pvoid;

         while (c > 0)
         {

            ::zero(p, sizeof(TYPE));

            p++;

            c--;

         }

      }



   };

   class nodef
   {
   public:

      inline static void construct(void * p)
      {
         UNREFERENCED_PARAMETER(p);
      }
      inline static void construct(void * p, :: count c)
      {
         UNREFERENCED_PARAMETER(p);
         UNREFERENCED_PARAMETER(c);
      }

   };


} // namespace constructor


// raw_array is an array that does not call constructors or destructor in elements
// array is an array that call only copy constructor and destructor in elements
// array is an array that call default constructors, copy constructs and destructors in elements
class CLASS_DECL_AURA array_base:
   virtual public ::object
{
public:

   class iterator_base
   {
   public:

      index            m_i;
      array_base *     m_parray;

      iterator_base()
      {
         m_i = 0;
         m_parray = NULL;
      }

      iterator_base(index i,array_base * parray)
      {
         m_i = i;
         m_parray = parray;
      }

      iterator_base(const iterator_base & it)
      {
         operator = (it);
      }


      iterator_base & operator = (const iterator_base & it)
      {
         if(this != &it)
         {
            m_i         = it.m_i;
            m_parray    = it.m_parray;
         }
         return *this;
      }

      bool operator == (const iterator_base & it)
      {
         if(this == &it)
            return true;
         if(m_parray != it.m_parray)
            return false;
         if(m_i >= m_parray->get_size() && it.m_i >= m_parray->get_size())
            return true;
         if(m_i <= 0 && it.m_i <= 0)
            return true;
         return m_i == it.m_i;
      }

      bool operator != (const iterator_base & it)
      {
         return !operator==(it);
      }

      iterator_base & operator ++()
      {
         m_i++;
         if(m_i >= m_parray->get_size())
            m_i = m_parray->get_size();
         return *this;
      }

      iterator_base & operator +(index i)
      {
         m_i += i;
         if(m_i >= m_parray->get_size())
            m_i = m_parray->get_size();
         return *this;
      }

      iterator_base & operator --()
      {
         m_i--;
         if(m_i < 0)
            m_i = 0;
         return *this;
      }

      iterator_base mid(const iterator_base & i) const
      {
         return iterator_base((m_i + i.m_i + 1) / 2,m_parray);
      }

      iterator_base & operator -(::count c)
      {
         m_i-=c;
         if(m_i < 0)
            m_i = 0;
         return *this;
      }

      bool operator < (const iterator_base & i) const
      {

         return m_i < i.m_i;

      }

      ::count get_count() const
      {
         return m_parray->get_count();
      }

   };


   class const_iterator_base
   {
   public:

      index            m_i;
      const array_base *     m_parray;

      const_iterator_base()
      {
         m_i = 0;
         m_parray = NULL;
      }

      const_iterator_base(index i,const array_base * parray)
      {
         m_i = i;
         m_parray = parray;
      }

      const_iterator_base(const iterator_base & it)
      {
         operator = (it);
      }

      const_iterator_base(const const_iterator_base & it)
      {
         operator = (it);
      }

      const_iterator_base & operator = (const iterator_base & it)
      {
         m_i         = it.m_i;
         m_parray    = it.m_parray;
         return *this;
      }

      const_iterator_base & operator = (const const_iterator_base & it)
      {
         if(this != &it)
         {
            m_i         = it.m_i;
            m_parray    = it.m_parray;
         }
         return *this;
      }

      bool operator == (const const_iterator_base & it)
      {
         if(this == &it)
            return true;
         if(m_parray != it.m_parray)
            return false;
         if(m_i >= m_parray->get_size() && it.m_i >= m_parray->get_size())
            return true;
         if(m_i <= 0 && it.m_i <= 0)
            return true;
         return m_i == it.m_i;
      }

      bool operator != (const const_iterator_base & it)
      {
         return !operator==(it);
      }

      const_iterator_base & operator ++()
      {
         m_i++;
         if(m_i >= m_parray->get_size())
            m_i = m_parray->get_size();
         return *this;
      }

      const_iterator_base & operator +(index i)
      {
         m_i += i;
         if(m_i >= m_parray->get_size())
            m_i = m_parray->get_size();
         return *this;
      }

      const_iterator_base & operator --()
      {
         m_i--;
         if(m_i < 0)
            m_i = 0;
         return *this;
      }

      const_iterator_base mid(const const_iterator_base & i) const
      {
         return const_iterator_base((m_i + i.m_i + 1) / 2,m_parray);
      }

      const_iterator_base & operator -(::count c)
      {
         m_i-=c;
         if(m_i < 0)
            m_i = 0;
         return *this;
      }

      bool operator < (const const_iterator_base & i) const
      {

         return m_i < i.m_i;

      }

      ::count get_count() const
      {
         return m_parray->get_count();
      }

   };

   bool           m_bRaw; // if raw, does not call destructors or constructors
   int            m_iTypeSize;
   void *         m_pData;    // the actual array of data
   ::count        m_nSize;    // # of elements (upperBound - 1)
   ::count        m_nMaxSize; // MAX allocated
   ::count        m_nGrowBy;  // grow amount

   array_base(int iTypeSize,bool bRaw);
   array_base(::aura::application * papp, int iTypeSize, bool bRaw);
   virtual ~array_base();

   inline ::count get_size() const;
   inline ::count get_size_in_bytes() const;
   inline ::count get_count() const;
   inline ::count get_byte_count() const;
   inline ::count size() const;
   inline ::count count() const;

   inline bool is_empty(::count countMinimum = 1) const;
   inline bool empty(::count countMinimum = 1) const;
   inline bool has_elements(::count countMinimum = 1) const;
   inline index get_upper_bound(index i = -1) const;


   void * element_at(index i) const { return (byte *) m_pData + i * m_iTypeSize; }

   ::count set_size(index nNewSize,::count nGrowBy = -1); // does not call default constructors on new items/elements
   ::count allocate(index nNewSize,::count nGrowBy = -1); // does not call default constructors on new items/elements
   ::count allocate_in_bytes(index nNewSize,::count nGrowBy = -1); // does not call default constructors on new items/elements
   ::count set_raw_size(index nNewSize,::count nGrowBy = -1); // does not call constructors and destructors on items/elements
   ::count resize(index nNewSize,::count nGrowBy = -1); // does not call default constructors on new items/elements
   void free_extra();
   virtual void destroy();

   void _001RemoveIndexes(index_array & ia);
   void remove_indexes(const index_array & ia);
   void remove_descending_indexes(const index_array & ia);

   inline void remove_last();
   inline ::count remove_all();
   inline void clear();
  
   virtual void on_construct_element(void *);
   virtual void on_construct_element(void *,::count);
   virtual void on_destruct_element(void *);
   virtual void on_copy_element(index i, const void *);

   index insert_at(index nIndex,const void * newElement,::count nCount = 1);
   index remove_at(index nIndex,::count nCount = 1);
   index insert_at(index nStartIndex,array_base * pNewArray);


   virtual ::count append(const array_base & src); // return old size
   virtual void copy(const array_base & src);
 
   virtual void on_after_read();

};


template < class TYPE, class ARG_TYPE = const TYPE &, class DEFCONSTRUCTOR = ::constructor::def < TYPE > >
class array :
   public ::array_base
{
public:

   typedef TYPE BASE_TYPE;
   typedef ARG_TYPE BASE_ARG_TYPE;
   typedef array < TYPE, ARG_TYPE > BASE_ARRAY;


   class iterator :
      public iterator_base
   {
   public:

      typedef TYPE BASE_TYPE;
      typedef ARG_TYPE BASE_ARG_TYPE;
      typedef array < TYPE,ARG_TYPE > BASE_ARRAY;


      iterator() {}
      iterator(index i,array_base * parray): iterator_base(i,parray) {}
      iterator(const iterator_base & it): iterator_base(it){}


      TYPE & operator * ()
      {
         return ((TYPE*) m_parray->m_pData)[m_i];
      }

      const TYPE & operator * () const
      {
         return ((const TYPE*)m_parray->m_pData)[m_i];
      }


   };


   class const_iterator :
      public const_iterator_base
   {
   public:


      typedef TYPE BASE_TYPE;
      typedef ARG_TYPE BASE_ARG_TYPE;
      typedef array < TYPE,ARG_TYPE > BASE_ARRAY;

      const_iterator() {}
      const_iterator(index i,const array_base * parray): const_iterator_base(i,parray) {}
      const_iterator(const iterator & it): const_iterator_base(it){}
      const_iterator(const const_iterator & it): const_iterator_base(it){}

      const TYPE & operator * ()
      {
         return ((const TYPE*)m_parray->m_pData)[m_i];
      }

   };




   array(::aura::application * papp = NULL, ::count nGrowBy = 32);
   array(const array <TYPE, ARG_TYPE> & a);
   array(::count n);
   virtual ~array();

   virtual void on_construct_element(void * p) { DEFCONSTRUCTOR::construct(p); }
   virtual void on_construct_element(void * p, ::count c) { DEFCONSTRUCTOR::construct(p, c); }
   virtual void on_destruct_element(void * p) { ((TYPE*)p)->~TYPE(); }
   virtual void on_copy_element(index i,const void * p) { *((TYPE*)((byte *) m_pData + m_iTypeSize * i)) = *((const TYPE*)p); }

   inline const TYPE& get_at(index nIndex) const;
   inline TYPE& get_at(index nIndex);
   inline void set_at(index nIndex, ARG_TYPE newElement);

   inline const TYPE & element_at(index nIndex) const;
   inline TYPE & element_at(index nIndex);

   inline TYPE & first(index n = 0);
   inline const TYPE & first(index n = 0) const;

   inline TYPE & last(index n = -1);
   inline const TYPE & last(index n = -1) const;

   inline const TYPE* get_data() const;
   inline TYPE* get_data();

   
   inline index add(ARG_TYPE newElement);
   inline index add(const array& src);
   inline index append(const array& src);
   inline void copy(const array& src);

   inline TYPE & add_new();
   inline index add_new( ::count count);


   inline TYPE pop(index index = -1);
   inline index push(ARG_TYPE newElement,index i = 0);
   inline void pop_back(index index = -1);
   inline void push_back(ARG_TYPE newElement,index = 0);



   inline iterator erase(iterator pos);
   inline iterator erase(iterator first,iterator last);
   inline iterator begin()
   {
      return iterator(0, this);
   }

   inline iterator end()
   {
      return iterator(this->get_size(), this);
   }

   inline const_iterator begin() const
   {
      return const_iterator(0,this);
   }

   inline const_iterator end() const
   {
      return const_iterator(this->get_size(),this);
   }

   // overloaded operator helpers
   inline const TYPE& operator[](index nIndex) const;
   inline TYPE& operator[](index nIndex);

   // Operations that move elements around
   inline index insert_at(index nIndex, ARG_TYPE newElement, ::count nCount = 1);
   //void _001RemoveIndexes(index_array & ia);
   //void remove_indexes(const index_array & ia); // remove indexes from index array upper bound to index array lower bound
   //void remove_descending_indexes(const index_array & ia); // remove indexes from index array lower bound to index array upper bound
   //index insert_at(index nStartIndex, array* pNewArray);
   inline void swap(index index1, index index2);

   inline array & operator = (const array & src);

   //inline index find_first(ARG_TYPE t, index (* lpfnCompare)(ARG_TYPE, ARG_TYPE), index start = 0, index last = -1) const;
   //index raw_find_first(TYPE * pt, index first = 0, index last = -1) const;

   //template < class DERIVED >
   //inline index find_first(DERIVED * pt, index first = 0, index last = -1)
   //{
   //   return raw_find_first(dynamic_cast < TYPE * > (pt), first, last);
   //}

   template < class ARRAY >
   inline ::count slice(ARRAY & a, index iStart = 0, ::count nCount = -1)
   {

      ::count ca = 0;

      index iEnd;

      if(nCount < 0)
         iEnd = get_upper_bound(nCount);
      else
         iEnd = iStart + nCount - 1;

      for(index i = iStart; i <= iEnd; i++)
      {

         a.add(element_at(i));
         ca++;

      }

      return ca;

   }

   //void quick_sort(index (* fCompare)(TYPE *, TYPE *));
   //void quick_sort(index (* fCompare)(TYPE *, TYPE *), void (* fSwap)(TYPE *, TYPE *));
   //void quick_sort(index (* fCompare)(TYPE *, TYPE *), index_array & ia);
   //bool binary_search(ARG_TYPE t, index & iIndex, index ( * fCompare ) (TYPE *, TYPE *)) const;

   //bool binary_search(ARG_TYPE t, index & iIndex, index ( * fCompare ) (TYPE *, TYPE *), index_array & ia) const;
   //index sort_add(ARG_TYPE t, index ( * fCompare ) (TYPE *, TYPE *), index_array & ia);
   //::count sort_add(const array < TYPE, ARG_TYPE > & a, index ( * fCompare ) (TYPE *, TYPE *), index_array & ia);
   //index sort_remove(ARG_TYPE t, index ( * fCompare ) (TYPE *, TYPE *), index_array & ia);



   inline array & operator += (const array & a);
   inline array operator + (const array & a) const;

   void dump(dump_context &) const;
   void assert_valid() const;

#ifdef MOVE_SEMANTICS
   array(array && a) :
   element(a.get_app()),
   array_base(a.get_app(),sizeof(TYPE),false)
   {

      m_nGrowBy      = a.m_nGrowBy;
      m_pData        = a.m_pData;
      m_nSize        = a.m_nSize;
      m_nMaxSize     = a.m_nMaxSize;

      a.m_pData      = NULL;
      a.m_nSize      = 0;
      a.m_nMaxSize   = 0;

   }


   inline array & operator = (array && a)
   {

      if(&a != this)
      {
         destroy();

         m_iTypeSize    = a.m_iTypeSize;
         m_nGrowBy      = a.m_nGrowBy;
         m_pData        = a.m_pData;
         m_nSize        = a.m_nSize;
         m_nMaxSize     = a.m_nMaxSize;

         a.m_pData      = NULL;
         a.m_nSize      = 0;
         a.m_nMaxSize   = 0;

      }

      return *this;
   }

#endif

   inline void set_at_grow(index nIndex, ARG_TYPE newElement);

   inline TYPE & element_at_grow(index nIndex);

};




template < class TYPE, class ARG_TYPE = const TYPE & >
class nodefctr_array :
   public array < TYPE, ARG_TYPE, ::constructor::nodef >
{
public:


   nodefctr_array(::aura::application * papp = NULL, ::count nGrowBy = 32) : array < TYPE, ARG_TYPE, ::constructor::nodef > (papp, nGrowBy) {}
   nodefctr_array(const array <TYPE, ARG_TYPE> & a) : array < TYPE, ARG_TYPE, ::constructor::nodef > (a) {}
   nodefctr_array(::count n) : array < TYPE, ARG_TYPE, ::constructor::nodef > (n){}
   virtual ~nodefctr_array() {}

};


template < class TYPE,class ARG_TYPE = const TYPE &,class DEFCONSTRUCTOR = ::constructor::def < TYPE > >
::file::output_stream & operator << (::file::output_stream & os,const array < TYPE,ARG_TYPE,DEFCONSTRUCTOR > & a)
{
   ::file::array::write(os,a);
   return os;
}

template < class TYPE,class ARG_TYPE = const TYPE &,class DEFCONSTRUCTOR = ::constructor::def < TYPE > >
::file::input_stream & operator >> (::file::input_stream & is,array < TYPE,ARG_TYPE,DEFCONSTRUCTOR > & a)
{
   ::file::array::read(is,a);
   return is;
}




