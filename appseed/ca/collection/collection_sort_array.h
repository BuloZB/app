#pragma once

#undef new

template < class T >
inline index BaseSortCompare(T * p1, T * p2)
{
   if(*p1 > *p2)
      return 1;
   else if(*p1 < *p2)
      return -1;
   else
      return 0;
}

template < class T >
inline index BaseNullCompare(T * p1, T * p2)
{
   return 0;
}


template < class TYPE, class ARG_TYPE = const TYPE &, class BASE_ARRAY = array < TYPE, ARG_TYPE >, index ( * DEFAULT_COMPARE)( TYPE *, TYPE *) = &BaseNullCompare < TYPE > >
class sort_array :
   protected BASE_ARRAY
{
public:

   class sort_index :
      virtual public ::ca::object
   {
   public:


      bool           m_bUpdated;
      index_array    m_indexa;


      sort_index()
      {
         m_bUpdated = false;
      }

   };

   class sort_index_map :
      virtual public mapsp(index ( * ) (TYPE *, TYPE *), index ( * ) (TYPE *, TYPE *), sort_index)
   {
   public:

      inline void mark_dirty()
      {
         
         sort_index_map::assoc * passoc = sort_index_map::PGetFirstAssoc();

         while(passoc != NULL)
         {
            passoc->m_element2->m_bUpdated = false;
            passoc = passoc->m_pnext;
         }

      }

   };


   sort_index_map    m_indexmap;
         
   index_array & defer_update(index ( * fCompare ) (TYPE *, TYPE *) = DEFAULT_COMPARE);

   index add(ARG_TYPE t, index ( * fCompare ) (TYPE *, TYPE *) = DEFAULT_COMPARE);

   ::count add(const array <TYPE, ARG_TYPE> & a, index ( * fCompare ) (TYPE *, TYPE *) = DEFAULT_COMPARE);

   ::count remove(ARG_TYPE t, index ( * fCompare ) (TYPE *, TYPE *) = DEFAULT_COMPARE);

   bool find(ARG_TYPE t, index & iIndex, index ( * fCompare ) (TYPE *, TYPE *) = DEFAULT_COMPARE) const;

   bool contains(ARG_TYPE t, index ( * fCompare ) (TYPE *, TYPE *) = DEFAULT_COMPARE) const;

   bool add_unique(ARG_TYPE t, index ( * fCompare ) (TYPE *, TYPE *) = DEFAULT_COMPARE);

   void quick_sort(index ( * fCompare ) (TYPE *, TYPE *) = DEFAULT_COMPARE);

   void set_size(::count c);

   const TYPE & element_at (::index i, index ( * fCompare ) (TYPE *, TYPE *) = DEFAULT_COMPARE) const;

   TYPE & element_at (::index i, index ( * fCompare ) (TYPE *, TYPE *) = DEFAULT_COMPARE);

   const TYPE & operator [] (::index i) const;

   TYPE & operator [] (::index i);

   bool operator == (const sort_array & a) const;

   bool operator != (const sort_array & a) const;

   using BASE_ARRAY::remove_all;

   using BASE_ARRAY::clear;

   using BASE_ARRAY::quick_sort;

   using BASE_ARRAY::operator new;

   using BASE_ARRAY::operator delete;

   using BASE_ARRAY::set_app;

   using BASE_ARRAY::get_size;



};

template < class TYPE, class ARG_TYPE, class BASE_ARRAY, index ( * DEFAULT_COMPARE)( TYPE *, TYPE *) >
index_array & sort_array < TYPE, ARG_TYPE, BASE_ARRAY, DEFAULT_COMPARE >::
defer_update(index ( * fCompare ) (TYPE *, TYPE *))
{
   
   sp(sort_index) & sortindex = m_indexmap[fCompare];

   if(sortindex.is_null())
      sortindex = new sort_index;

   if(!sortindex->m_bUpdated)
   {
      sortindex->m_indexa.ensure_sequence(0, get_upper_bound());
      quick_sort(fCompare, sortindex->m_indexa);
      sortindex->m_bUpdated = true;
   }

   return sortindex->m_indexa;

}

template < class TYPE, class ARG_TYPE, class BASE_ARRAY, index ( * DEFAULT_COMPARE)( TYPE *, TYPE *) >
bool sort_array < TYPE, ARG_TYPE, BASE_ARRAY, DEFAULT_COMPARE >::
find(ARG_TYPE t, index & iIndex, index ( * fCompare ) (TYPE *, TYPE *)) const
{

   if(this->get_size() == 0)
   {
      return false;
   }

   return binary_search(t, iIndex, fCompare, ((sort_array *) this)->defer_update(fCompare));


}

template < class TYPE, class ARG_TYPE, class BASE_ARRAY, index ( * DEFAULT_COMPARE)( TYPE *, TYPE *) >
bool sort_array < TYPE, ARG_TYPE, BASE_ARRAY, DEFAULT_COMPARE >::
contains(ARG_TYPE t, index ( * fCompare ) (TYPE *, TYPE *)) const
{
   index iIndex = 0;
   return find(t, iIndex, fCompare);
}

template < class TYPE, class ARG_TYPE, class BASE_ARRAY, index ( * DEFAULT_COMPARE)( TYPE *, TYPE *) >
bool sort_array < TYPE, ARG_TYPE, BASE_ARRAY, DEFAULT_COMPARE >::
add_unique(ARG_TYPE t, index ( * fCompare ) (TYPE *, TYPE *))
{
   if(contains(t, fCompare))
      return false;
   add(t, fCompare);
   return true;
}


template < class TYPE, class ARG_TYPE, class BASE_ARRAY, index ( * DEFAULT_COMPARE)( TYPE *, TYPE *) >
::index sort_array < TYPE, ARG_TYPE, BASE_ARRAY, DEFAULT_COMPARE >::
add(ARG_TYPE t, index ( * fCompare ) (TYPE *, TYPE *))
{

   ::index i = sort_add(t, fCompare, defer_update(fCompare));

   m_indexmap.mark_dirty();

   return i;

}

template < class TYPE, class ARG_TYPE, class BASE_ARRAY, index ( * DEFAULT_COMPARE)( TYPE *, TYPE *) >
::count sort_array < TYPE, ARG_TYPE, BASE_ARRAY, DEFAULT_COMPARE >::
add(const array < TYPE, ARG_TYPE > & a, index ( * fCompare ) (TYPE *, TYPE *))
{

   ::count c = sort_add(a, fCompare, defer_update(fCompare));

   m_indexmap.mark_dirty();

   return c;

}

template < class TYPE, class ARG_TYPE, class BASE_ARRAY, index ( * DEFAULT_COMPARE)( TYPE *, TYPE *) >
::count sort_array < TYPE, ARG_TYPE, BASE_ARRAY, DEFAULT_COMPARE >::
remove(ARG_TYPE t, index ( * fCompare ) (TYPE *, TYPE *))
{

   ::count c = sort_remove(t, fCompare, defer_update(fCompare));

   m_indexmap.mark_dirty();

   return c;

}


template < class TYPE, class ARG_TYPE, class BASE_ARRAY, index ( * DEFAULT_COMPARE)( TYPE *, TYPE *) >
void sort_array < TYPE, ARG_TYPE, BASE_ARRAY, DEFAULT_COMPARE >::
quick_sort(index ( * fCompare ) (TYPE *, TYPE *))
{

   defer_update(fCompare);

}

template < class TYPE, class ARG_TYPE, class BASE_ARRAY, index ( * DEFAULT_COMPARE)( TYPE *, TYPE *) >
void sort_array < TYPE, ARG_TYPE, BASE_ARRAY, DEFAULT_COMPARE >::
set_size(::count n)
{

   BASE_ARRAY::set_size(n);

   m_indexmap.mark_dirty();

}

template < class TYPE, class ARG_TYPE, class BASE_ARRAY, index ( * DEFAULT_COMPARE)( TYPE *, TYPE *) >
TYPE & sort_array < TYPE, ARG_TYPE, BASE_ARRAY, DEFAULT_COMPARE >::
element_at(::index i, index ( * fCompare ) (TYPE *, TYPE *))
{

   return BASE_ARRAY::element_at(defer_update(fCompare)[i]);

}

template < class TYPE, class ARG_TYPE, class BASE_ARRAY, index ( * DEFAULT_COMPARE)( TYPE *, TYPE *) >
const TYPE & sort_array < TYPE, ARG_TYPE, BASE_ARRAY, DEFAULT_COMPARE >::
element_at(::index i, index ( * fCompare ) (TYPE *, TYPE *)) const
{

   return ((sort_array *) this)->element_at(i, fCompare);

}


template < class TYPE, class ARG_TYPE, class BASE_ARRAY, index ( * DEFAULT_COMPARE)( TYPE *, TYPE *) >
TYPE & sort_array < TYPE, ARG_TYPE, BASE_ARRAY, DEFAULT_COMPARE >::
operator [](::index i)
{

   return this->element_at(i);

}

template < class TYPE, class ARG_TYPE, class BASE_ARRAY, index ( * DEFAULT_COMPARE)( TYPE *, TYPE *) >
const TYPE & sort_array < TYPE, ARG_TYPE, BASE_ARRAY, DEFAULT_COMPARE >::
operator [](::index i) const
{

   return ((sort_array *) this)->operator [](i);

}

template < class TYPE, class ARG_TYPE, class BASE_ARRAY, index ( * DEFAULT_COMPARE)( TYPE *, TYPE *) >
bool sort_array < TYPE, ARG_TYPE, BASE_ARRAY, DEFAULT_COMPARE >::
operator == (const sort_array & a) const
{

   if(&a == this)
      return true;

   if(get_size() != a.get_size())
      return false;

   index_array & ia1 = ((sort_array *) this)->defer_update();

   index_array & ia2 = ((sort_array *) &a)->defer_update();

   for(index i = 0; i < get_size(); i++)
   {
      if(DEFAULT_COMPARE(&((sort_array *) this)->BASE_ARRAY::element_at(ia1[i]), &((sort_array *) &a)->BASE_ARRAY::element_at(ia2[i])) != 0)
         return false;
   }

   return true;

}

template < class TYPE, class ARG_TYPE, class BASE_ARRAY, index ( * DEFAULT_COMPARE)( TYPE *, TYPE *) >
bool sort_array < TYPE, ARG_TYPE, BASE_ARRAY, DEFAULT_COMPARE >::
operator != (const sort_array & a) const
{

   return !this->operator == (a);

}



#define new DEBUG_NEW


// smart_pointer_sort_array
#define spsa(T) sort_array < sp(T), const sp(T) &, spa(T) >