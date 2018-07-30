#pragma once
/////////////////////////////////////////////////////////////////////
// DbCore.h - Implements NoSql database                   //
// ver 1.0                                                         //
// Chetanraj Kadam, CSE687 - Object Oriented Design, Spring 2018   //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides two classes:
* - DbCore implements core NoSql database operations and
*   provide editing, querying, or persisting.  
* - DbElement provides the value part of our key-value database.
*   It contains fields for name, description, date, child collection
*   and a payload field of the template type. 
* The package also provides functions for displaying:
* - set of all database keys
* - database elements
* - all records in the database

* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
* Utilities.h, Utilities.cpp
*
* Maintenance History:
* --------------------
* ver 1.0 : 05 Feb 2018
* - first release
*/

#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "../DateTime/DateTime.h"

namespace NoSqlDb
{
  /////////////////////////////////////////////////////////////////////
  // DbElement class
  // - provides the value part of a NoSql key-value database

  template<typename T>
  class DbElement
  {
  public:
    using Key = std::string;
    using Children = std::vector<Key>;

    // methods to get and set DbElement fields

    std::string& name() { return name_; }
    std::string name() const { return name_; }
    void name(const std::string& name) { name_ = name; }

    std::string& descrip() { return descrip_; }
    std::string descrip() const { return descrip_; }
    void descrip(const std::string& name) { descrip_ = name; }
    
    DateTime& dateTime() { return dateTime_; }
    DateTime dateTime() const { return dateTime_; }
    void dateTime(const DateTime& dateTime) { dateTime_ = dateTime; }

    Children& children() { return children_; }
    Children children() const { return children_; }
    void children(const Children& children) { children_ = children; }

    T& payLoad() { return payLoad_; }
    T payLoad() const { return payLoad_; }
    void payLoad(const T& payLoad) { payLoad_ = payLoad; }

  private:
    std::string name_;
    std::string descrip_;
    DateTime dateTime_;
    Children children_;
    T payLoad_;
  };

  /////////////////////////////////////////////////////////////////////
  // Payload class
  // - provides structure for storing path and categories in payload

  template <typename T>
  class Payload	
  {
  public:
	  std::string path() const { return path_; }
	  void path(const std::string& path) { path_ = path; }

	  std::vector<std::string> categories() const { return categories_; }
	  void categories(const std::vector<std::string> categories) { categories_ = categories; }

  private:
	  std::string path_;
	  std::vector<std::string> categories_;
  };

  /////////////////////////////////////////////////////////////////////
  // DbCore class
  // - provides core NoSql db operations
  // - does not provide editing, querying, or persistance operations

  template <typename T>
  class DbCore
  {
  public:
    using Key = std::string;
    using Keys = std::vector<Key>;
    using Children = Keys;
    using DbStore = std::unordered_map<Key,DbElement<T>>;
    using iterator = typename DbStore::iterator;

    // methods to access database elements

    Keys keys();
    bool contains(const Key& key);
    size_t size();
    void throwOnIndexNotFound(bool doThrow) { doThrow_ = doThrow; }
    DbElement<T>& operator[](const Key& key);
    DbElement<T> operator[](const Key& key) const;
    typename iterator begin() { return dbStore_.begin(); }
    typename iterator end() { return dbStore_.end(); }

    // methods to get and set the private database hash-map storage

    DbStore& dbStore() { return dbStore_; }
    DbStore dbStore() const { return dbStore_; }
    void dbStore(const DbStore& dbStore) { dbStore_ = dbStore; }

	// methods to perform required operations

	bool deleteKey(Key key);
	void editElement(Key key, std::string name, std::string desc, Key nChildren);
  
  private:
    DbStore dbStore_;
    bool doThrow_ = false;
  };

  /////////////////////////////////////////////////////////////////////
  // DbCore<T> methods

  //----< does db contain this key? >----------------------------------

  template<typename T>
  bool DbCore<T>::contains(const Key& key)
  {
    iterator iter = dbStore_.find(key);
    return iter != dbStore_.end();
  }
  //----< returns current key set for db >-----------------------------

  template<typename T>
  typename DbCore<T>::Keys DbCore<T>::keys()
  {
    DbCore<T>::Keys dbKeys;
    DbStore& dbs = dbStore();
    size_t size = dbs.size();
    dbKeys.reserve(size);
    for (auto item : dbs)
    {
      dbKeys.push_back(item.first);
    }
    return dbKeys;
  }
  //----< return number of db elements >-------------------------------

  template<typename T>
  size_t DbCore<T>::size()
  {
    return dbStore_.size();
  }
  //----< extracts value from db with key >----------------------------
  /*
  *  - indexes non-const db objects
  *  - In order to create a key-value pair in the database like this:
  *      db[newKey] = newDbElement
  *    we need to index with the new key and assign a default element. 
  *    That will be replaced by newDbElement when we return from operator[]
  *  - However, if we want to index without creating new elements, we need
  *    to throw an exception if the key does not exist in the database.
  *  - The behavior we get is determined by doThrow_.  If false we create
  *    a new element, if true, we throw. Creating new elements is the default
  *    behavior.
  */
  template<typename T>
  DbElement<T>& DbCore<T>::operator[](const Key& key)
  {
    if (!contains(key))
    {
      if (doThrow_)
        throw(std::exception("key does not exist in db"));
      else
        return (dbStore_[key] = DbElement<T>());
    }
    return dbStore_[key];
  }
  //----< extracts value from db with key >----------------------------
  /*
  *  - indexes const db objects
  */
  template<typename T>
  DbElement<T> DbCore<T>::operator[](const Key& key) const
  {
    if (!contains(key))
    {
      throw(std::exception("key does not exist in db"));
    }
    return dbStore_[key];
  }
  
  /////////////////////////////////////////////////////////////////////
  // display functions

  //----< display database key set >-----------------------------------

  template<typename T>
  void showKeys(DbCore<T>& db, std::ostream& out = std::cout)
  {
    out << "\n  ";
    for (auto key : db.keys())
    {
      out << key << " ";
    }
  }
  //----< show record header items >-----------------------------------

  inline void showHeader(std::ostream& out = std::cout)
  {
    out << "\n\n  ";
    out << std::setw(26) << std::left << "DateTime";
    out << std::setw(20) << std::left << "Name";
    out << std::setw(25) << std::left << "Description";
    out << std::setw(25) << std::left << "Payload";
    out << "\n  ";
    out << std::setw(26) << std::left << "------------------------";
    out << std::setw(20) << std::left << "--------";
    out << std::setw(25) << std::left << "-----------------------";
    out << std::setw(25) << std::left << "----------------------------------------------------------";
  }
  //----< display DbElements >-----------------------------------------

  template<typename T>
  void showElem(const DbElement<T>& el, std::ostream& out = std::cout)
  {
    out << "\n  ";
    out << std::setw(26) << std::left << std::string(el.dateTime());
    out << std::setw(20) << std::left << el.name();
    out << std::setw(25) << std::left << el.descrip();
    out << std::setw(25) << std::left << el.payLoad();
	

    typename DbElement<T>::Children children = el.children();
    if (children.size() > 0)
    {
      out << "\n    child keys: ";
      for (auto key : children)
      {
        out << " " << key;
      }
    }
  }

  std::ostream& operator<<(std::ostream& out, const Payload<std::string>& payload) {
	  out << payload.path();
	  out << "  \t";
	  for (std::string str : payload.categories())
		  out << str <<",";
	  return out;
  }
  //----< display all records in database >----------------------------

  template<typename T>
  void showDb(const DbCore<T>& db, std::ostream& out = std::cout)
  {
    showHeader(out);
    typename DbCore<T>::DbStore dbs = db.dbStore();
    for (auto item : dbs)
    {
      showElem(item.second, out);
    }
  }

  //----< delete particular key from database >----------------------------

  template<typename T>
  bool DbCore<T>::deleteKey(Key key)
  {
	  if (!contains(key))
	  {
		  std::cout << "Failed to find the key " << std::endl;
		  return false;
	  }

	  dbStore_.erase(key);
	  Keys nKeys = keys();

	  for (Key nKey : nKeys)
	  {
		  DbElement<T> nElement = dbStore_[nKey];
		  for (std::vector<std::string>::iterator it = nElement.children().begin(); it != nElement.children().end(); ++it)
		  {
			  if (*it == key)
				  it = nElement.children().erase(it);
		  }
		  dbStore_[nKey] = nElement;
	  }
	  return true;
  }

  //----< edit particular DataElement in database >----------------------------

  template<typename T>
  void DbCore<T>::editElement(Key key, std::string name, std::string desc, Key nChildren)
  {
	  if (!contains(key))
	  {
		  std::cout << "Failed to find the key " << std::endl;
	  }
	  else
	  {
		  DbElement<T> demoElem = dbStore_[key];
		  demoElem.name(name);
		  demoElem.descrip(desc);
		  demoElem.children().push_back(nChildren);
		  dbStore_[key] = demoElem;
	  }
  }
}