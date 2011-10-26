#pragma once

namespace simpledb
{

   class table;
   class set;

   class CLASS_DECL_ca base : 
      public ::database::base
   {
   public:

      ::ex1::filesp                 m_spfileMeta;
      ::xml::node                   m_nodeMeta;
      ::collection::string_map < table *, table * > m_mapTable;
      int                           m_iErrorCode;


      base(::ca::application * papp);


      virtual int connect();


      virtual class ::simpledb::set * create_dataset() const;

      table * get_table(const char * pszName);
      table * create_table(const char * pszName);


      // class ::database::base
      virtual class ::database::set * CreateDataset() const;
      virtual int setErr(int err_code);
      virtual long nextid(const char* sname);



   };


} // namespace simpledb
