/////////////////////////////////////////////////////////////////////
// DbCore.cpp - Implements NoSql database                          //
// ver 1.0                                                         //
// Chetanraj Kadam, CSE687 - Object Oriented Design, Spring 2018   //
/////////////////////////////////////////////////////////////////////

#include "DbCore.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include <iostream>
#include <iomanip>
#include <functional>

using namespace NoSqlDb;

//----< reduce the number of characters to type >----------------------

auto putLine = [](size_t n=1, std::ostream& out = std::cout)
{
  Utilities::putline(n, out);
};

///////////////////////////////////////////////////////////////////////
// DbProvider class
// - provides mechanism to share a test database between test functions
//   without explicitly passing as a function argument.

class DbProvider
{
public:
  DbCore<std::string>& db() { return db_; }
private:
  static DbCore<std::string> db_;
};

DbCore<std::string> DbProvider::db_;

///////////////////////////////////////////////////////////////////////
// test functions

//----< demo requirement #1 >------------------------------------------

bool testR1()
{
  Utilities::title("Demonstrating Requirement #1");
  std::cout << "\n  " << typeid(std::function<bool()>).name()
    << ", declared in this function, "
    << "\n  is only valid for C++11 and later versions.";
  putLine();
  return true; // would not compile unless C++11
}

//----< demo requirement #2 >------------------------------------------

bool testR2()
{
  Utilities::title("Demonstrating Requirement #2");
  std::cout << "\n  A visual examination of all the submitted code "
    << "will show only\n  use of streams and operators new and delete.";
  putLine();
  return true;
}

//----< demo first part of requirement #3 >----------------------------

bool testR3a()
{
  Utilities::title("Demonstrating Requirement #3a - creating DbElement");
  std::cout << "\n  Creating a db element with key \"Taylor\":";

  // create database to hold std::string payload

  DbCore<std::string> db;
  DbProvider dbp;
  dbp.db() = db;

  // create some demo elements and insert into db

  DbElement<std::string> demoElem;

  demoElem.name("Jacob");
  demoElem.descrip("Parent of family");
  demoElem.dateTime(DateTime().now());
  demoElem.payLoad("Data about this key");

  if (demoElem.name() != "Jacob")
    return false;
  if (demoElem.descrip() != "Parent of family")
    return false;
  if (demoElem.dateTime().now() != DateTime().now())
    return false;
  if (demoElem.payLoad() != "Data about this key")
    return false;

  showHeader();
  showElem(demoElem);

  db["Taylor"] = demoElem;

  demoElem.name("Abcd");
  demoElem.descrip("Description about");
  demoElem.dateTime(DateTime().now());
  demoElem.payLoad("Added key no.2");
  db["Abc"] = demoElem;

  demoElem.name("Chetanraj");
  demoElem.descrip("Student");
  demoElem.dateTime(DateTime().now());
  demoElem.payLoad("Testing DB elements");
  db["Chetan"] = demoElem;

  dbp.db() = db;
  putLine();
  return true;
}
//----< demo second part of requirement #3 >---------------------------

bool testR3b()
{
  Utilities::title("Demonstrating Requirement #3b - creating DbCore");

  DbProvider dbp;
  DbCore<std::string> db = dbp.db();

  DbElement<std::string> demoElem = db["Taylor"];

  demoElem.name("William");
  demoElem.descrip("Child of Jacob");
  demoElem.payLoad("Some data for description");
  db["Lee"] = demoElem;

  demoElem.payLoad("He has some data in storage..");
  demoElem.name("Noah");
  db["White"] = demoElem;

  demoElem.payLoad("Sharing some important data..");
  demoElem.name("Ethan");
  db["Hunt"] = demoElem;

  std::cout << "\n  after adding elements with keys: ";
  DbCore<std::string>::Keys keys = db.keys();
  showKeys(db);
  putLine();

  std::cout << "\n  Make some new elements children of element with key \"Taylor\"";
  db["Taylor"].children().push_back("Lee");
  db["Taylor"].children().push_back("White");
  db["Taylor"].children().push_back("Hunt");
 

  showHeader();
  showElem(db["Taylor"]);


  // display the results

  putLine();
  std::cout << "\n  showing all the database elements:";
  showDb(db);
  putLine();

  std::cout << "\n  database keys are: ";
  showKeys(db);

  dbp.db() = db;
  putLine();
  return true;
}

//----< demo requirement #4 >------------------------------------------

bool testR4()
{
	Utilities::title("Demonstrating Requirement 4");
	std::cout << "\n  Deleting key Abc from db... \n ";
	
	DbProvider dbp;
	DbCore<std::string> db = dbp.db();
	db.deleteKey("Abc");

	std::cout << "\n  DB after deleting";
	showDb(db);
	std::cout << "\n\n  Database keys are: ";
	showKeys(db);

	std::cout << "\n\n  Adding new key Newkey in db... " << "\n";

	DbElement<std::string> demoElem;
	demoElem.name("New Key");
	demoElem.descrip("Description about");
	demoElem.dateTime(DateTime().now());
	demoElem.payLoad("Added new key to DB");
	db["Newkey"] = demoElem;

	std::cout << "\n  DB after adding";
	showDb(db);
	std::cout << "\n\n  Database keys are: ";
	showKeys(db);

	dbp.db() = db;
	putLine();
	return true;
}

//----< demo requirement #5 >------------------------------------------

bool testR5()
{
	Utilities::title("Demonstrating Requirement 5");
	DbProvider dbp;
	DbCore<std::string> db = dbp.db();

	std::cout << "\n Changing name from  'Chetanraj' to 'Kadam'";
	std::cout << "\n Changing description from  'Student' to 'Graduate Student'";
	std::cout << "\n Making key 'Newkey' as its children";
	std::cout << "\n Replacing existing value's instance with new instance";

	db.editElement("Chetan", "Kadam", "Graduate Student", "Newkey");

	std::cout << "\n\n DB after editing";
	showDb(db);

	putLine();
	return true;
}


//----< test stub >----------------------------------------------------

#ifdef TEST_DBCORE

using namespace Utilities;

int main()
{
  Utilities::Title("Testing DbCore - He said, she said database");
  putLine();


  TestExecutive ex;

  // define test structures with test function and message

  TestExecutive::TestStr ts1{ testR1, "Use C++11" };
  TestExecutive::TestStr ts2{ testR2, "Use streams and new and delete" };
  TestExecutive::TestStr ts3a{ testR3a, "Create DbElement<std::string>" };
  TestExecutive::TestStr ts3b{ testR3b, "Create DbCore<std::string>" };

  TestExecutive::TestStr ts4{ testR4, "Addition and Deletion of key/value pair" };
  TestExecutive::TestStr ts5{ testR5, "Editing metadata, relationships and replacing instance" };


  // register test structures with TestExecutive instance, ex

  ex.registerTest(ts1);
  ex.registerTest(ts2);
  ex.registerTest(ts3a);
  ex.registerTest(ts3b);

  ex.registerTest(ts4);
  ex.registerTest(ts5);
 
  // run tests

  bool result = ex.doTests();
  if (result == true)
    std::cout << "\n  all tests passed";
  else
    std::cout << "\n  at least one test failed";

  putLine(2);
  return 0;
}
#endif
