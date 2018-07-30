///////////////////////////////////////////////////////////////////////
// Executive.cpp - Executes all the operation on Database and        //
//                 demonstartes all the requirements                 //
// ver 1.0                                                           //
// Chetanraj Kadam, CSE687 - Object Oriented Design, Spring 2018     //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package 
* -
* Required Files:
* ---------------
* DbCore.h
* Query.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 05 Feb 2018
* - first release
*/

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <functional>
#include "../DbCore/DbCore.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include "../DateTime/DateTime.h"
#include "../Query/Query.h"
#include "../PersistDb/PersistDb.h"
#include "..\XmlDocument\XmlElement\XmlElement.h"
#include "..\XmlDocument\XmlDocument\XmlDocument.h"
#include "..\XmlDocument\XmlParser\XmlParser.h"

using namespace NoSqlDb;

//----< reduce the number of characters to type >----------------------

auto putLine = [](size_t n = 1, std::ostream& out = std::cout)
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
	DbCore<NoSqlDb::Payload<std::string>>& db() { return db_; }
private:
	static DbCore<NoSqlDb::Payload<std::string>> db_;
};

DbCore<NoSqlDb::Payload<std::string>> DbProvider::db_;

///////////////////////////////////////////////////////////////////////
// test functions

//----< demo requirement #1 >------------------------------------------

bool testR1()
{
	Utilities::Title("Demonstrating Requirement #1");
	std::cout << "\n  " << typeid(std::function<bool()>).name()
		<< ", declared in this function, "
		<< "\n  is only valid for C++11 and later versions.";
	putLine();
	return true; // would not compile unless C++11
}

//----< demo requirement #2 >------------------------------------------

bool testR2()
{
	Utilities::Title("Demonstrating Requirement #2");
	std::cout << "\n  A visual examination of all the submitted code "
		<< "will show only\n  use of streams and operators new and delete.";
	putLine();
	return true;
}

//----< demo first part of requirement #3 >----------------------------

bool testR3a()
{
	Utilities::Title("Demonstrating Requirement #3a - Creating DbElement");
	std::cout << "\n  Creating a db element with key \"Taylor\":";

	// create database to hold std::string payload

	DbCore<NoSqlDb::Payload<std::string>> db;
	DbProvider dbp;
	dbp.db() = db;

	// create some demo elements and insert into db

	Payload<std::string> demoPayload;
	DbElement<NoSqlDb::Payload<std::string>> demoElem;

	demoElem.name("Jacob");
	demoElem.descrip("Parent of family");
	demoElem.dateTime(DateTime().now());
	demoPayload.path("../dummy/Taylor/path");
	demoPayload.categories({ "Professor", "Graduate", "Department" });
	demoElem.payLoad(demoPayload);
	db["Taylor"] = demoElem;

	showHeader();
	showElem(demoElem);

	dbp.db() = db;
	putLine();
	return true;
}
//----< demo second part of requirement #3 >---------------------------

bool testR3b()
{
	Utilities::Title("Demonstrating Requirement #3b - Creating DbCore and Requirement #9 - Implementing a Payload type");

	DbProvider dbp;
	DbCore<NoSqlDb::Payload<std::string>> db = dbp.db();

	Payload<std::string> demoPayload;
	DbElement<NoSqlDb::Payload<std::string>> demoElem;

	demoElem.name("Abcd");
	demoElem.descrip("Description about");
	demoElem.dateTime(DateTime().now());
	demoPayload.path("../dummy/Abc/path");
	demoPayload.categories({ "General", "Employee","Department"});
	demoElem.payLoad(demoPayload);
	db["Abc"] = demoElem;

	demoElem.name("Chetanraj");
	demoElem.descrip("Student Description");
	demoElem.dateTime(DateTime().now());
	demoPayload.path("../dummy/chetan/path");
	demoPayload.categories({ "Student", "Graduate", "Computer Science" });
	demoElem.payLoad(demoPayload);
	db["Chetan"] = demoElem;

	demoElem.name("William");
	demoElem.descrip("Child of Jacob");
	demoElem.dateTime(DateTime().now());
	demoPayload.path("../dummy/Lee/path");
	demoPayload.categories({ "Teaching Assistant", "Graduate", "Department" });
	demoElem.payLoad(demoPayload);
	db["Lee"] = demoElem;

	demoElem.name("Noah");
	demoElem.descrip("Child of Jacob");
	demoPayload.path("../dummy/Newton/path");
	demoPayload.categories({"Graduate", "Department", "Student" });
	demoElem.dateTime(DateTime().now());
	demoElem.payLoad(demoPayload);
	db["Newton"] = demoElem;

	demoElem.name("Ethan");
	demoElem.descrip("Child of Jacob");
	demoElem.dateTime(DateTime().now());
	demoPayload.path("../dummy/Hunt/path");
	demoPayload.categories({ "Department", "Graduate", "Teaching Assistant"});
	demoElem.payLoad(demoPayload);
	db["Hunt"] = demoElem;

	std::cout << "\n  After adding elements with keys: \n";
	DbCore<std::string>::Keys keys = db.keys();
	showKeys(db);
	putLine();

	std::cout << "\n  Showing all the database elements:";
	showDb(db);
	putLine();

	std::cout << "\n  Make some new elements children of element with key \"Taylor\"";
	db["Taylor"].children().push_back("Lee");
	db["Taylor"].children().push_back("Newton");
	db["Taylor"].children().push_back("Hunt");

	showHeader();
	showElem(db["Taylor"]);

	// display the results
	putLine();
	std::cout << "\n  Database keys are: \n";
	showKeys(db);

	dbp.db() = db;
	putLine();
	return true;
}

//----< demo requirement #4 >------------------------------------------

bool testR4()
{
	Utilities::Title("Demonstrating Requirement 4 - Addition and Deletion of key/value pair ");
	std::cout << "\n  Deleting key Abc from db... \n ";

	Payload<std::string> demoPayload;
	DbElement<NoSqlDb::Payload<std::string>> demoElem;

	DbProvider dbp;
	DbCore<NoSqlDb::Payload<std::string>> db = dbp.db();
	db.deleteKey("Abc");

	std::cout << "\n  DB after deleting";
	showDb(db);
	std::cout << "\n\n  Database keys are: ";
	showKeys(db);

	std::cout << "\n\n  Adding new key Newkey in db... " << "\n";

	demoElem.name("New Key");
	demoElem.descrip("Description about");
	demoElem.dateTime(DateTime().now());
	demoPayload.path("../dummy/Newkey/path");
	demoPayload.categories({ "Department", "Undergraduate", "Student" });
	demoElem.payLoad(demoPayload);
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
	Utilities::Title("Demonstrating Requirement 5 - Editing metadata, relationships and replacing instance ");
	DbProvider dbp;
	DbCore<NoSqlDb::Payload<std::string>> db = dbp.db();

	std::cout << "\n Changing name from  'Chetanraj' to 'Kadam'";
	std::cout << "\n Changing description from  'Student' to 'Graduate Student'";
	std::cout << "\n Making key 'Newkey' as its children";
	std::cout << "\n Replacing existing value's instance with new instance";

	db.editElement("Chetan", "Kadam", "Graduate Student", "Newkey");

	std::cout << "\n\n DB after editing";
	showDb(db);

	dbp.db() = db;
	putLine();
	return true;
}

//----< demo requirement #6 >------------------------------------------

bool testR6()
{
	Utilities::Title("Demonstrating Requirement 6 - Support different queries");
	DbProvider dbp;
	DbCore<NoSqlDb::Payload<std::string>> db = dbp.db();
	Query<NoSqlDb::Payload<std::string>> query;

	std::cout << "\n\n====> Query1- Return the value of a specified key:  Chetan";
	DbElement<NoSqlDb::Payload<std::string>> demoElem;
	demoElem = query.Query1(db,"Chetan");
	
	putLine();
	std::cout << "\n\n====> Query2- Return children of a specified key:  Taylor \n";
	Keys result2 = query.Query2(db, "Taylor");

	std::cout << "\n\n====> Query3- Return set of all keys matching pattern: New\n";
	Keys result3 = query.Query3(db, "New");

	std::cout << "\n\n====> Query4- Return set of all keys contain a \"am\" string in their Name section\n";
	Keys result4 = query.Query4(db, "am");

	std::cout << "\n\n====> Query5- Return set of all keys contain a \"Child\" string in their Description section\n";
	Keys result5 = query.Query5(db, "Child");

	std::cout << "\n\n====> Query6- Return set of all keys contain a \"Student\" category in their Payload section\n";
	Keys result6 = query.Query6(db, "Student");

	std::cout << "\n\n====> Query7a- Return set of all keys from  \"Wed Jan 10 15:30:00 2018\" to \"Mon Feb 5 16:30:00 2018\"\n";
	Keys result7a = query.Query7(db, "Wed Jan 10 15:30:00 2018", "Mon Feb 5 16:30:00 2018");

	std::cout << "\n\n====> Query7b- Return set of all keys from  \"Wed Jan 10 15:30:00 2018\" to \"Current time\"\n";
	Keys result7b = query.Query7(db, "Wed Jan 10 15:30:00 2018", "");

	putLine();
	return true;
}

//----< demo requirement #7 >------------------------------------------

bool testR7()
{
	putLine();
	Utilities::Title("Demonstrating Requirement 7 - Support queries on queries and Union of queries");
	DbProvider dbp;
	DbCore<NoSqlDb::Payload<std::string>> db = dbp.db();
	Query<NoSqlDb::Payload<std::string>> query;

	std::cout << "\n\n====> Query8- Return keys who are child of \"Taylor\" and contain \"am\" in their Name section";
	std::cout << "\n\n     (Running Query4 on set of keys returned by Query2)";
	Keys result8 = query.Query8(db,"Taylor", "am");

	std::cout << "\n\n====> Query9- Return keys with matching pattern \"New\" or containing \"Child\" string in their Description section\n";
	std::cout << "\n     (Union of Query3 and Query5)";
	Keys result9 = query.Query9(db, "New", "Child");

	putLine();
	return true;
}

bool testR8()
{
	putLine();
	Utilities::Title("Demonstrating Requirement 8 - Persisting a DB contents");
	DbProvider dbp;
	DbCore<NoSqlDb::Payload<std::string>> db = dbp.db();
	DbCore<NoSqlDb::Payload<std::string>> newdb;
	PersistDb < NoSqlDb::Payload<std::string>> persDb;

	persDb.persistDb(db);
	std::cout << "\n\n====> Restoring the database from new XML file:\n";
	db = persDb.constructDb("../newSqlDb.xml");
	std::cout << "\n   Contents of the database after restoring from XML : \n";
	showDb(db);

	std::cout << "\n\n\n====> Augmenting the database from new XML : \n";
	newdb = persDb.constructDb("../newSqlDb2.xml");
	persDb.augmentDb(db,newdb);
	std::cout << "\n   Contents of the database after augmenting: \n";
	showDb(db);
	dbp.db() = db;

	putLine();
	return true;
}

int main()
{
	Utilities::Title("<------------ TestExecutive ---------->");
	putLine();

	TestExecutive ex;

	// define test structures with test function and message

	TestExecutive::TestStr ts1{ testR1, "Use C++11" };
	TestExecutive::TestStr ts2{ testR2, "Use streams and new and delete" };
	TestExecutive::TestStr ts3a{ testR3a, "Create DbElement<NoSqlDb::Payload<std::string>>" };
	TestExecutive::TestStr ts3b{ testR3b, "Create DbCore<NoSqlDb::Payload<std::string>>" };

	TestExecutive::TestStr ts4{ testR4, "Addition and Deletion of key/value pair" };
	TestExecutive::TestStr ts5{ testR5, "Editing metadata, relationships and replacing instance" };
	TestExecutive::TestStr ts6{ testR6, "Support different queries" };
	TestExecutive::TestStr ts7{ testR7, "Support queries on queries and Union of queries" };
	TestExecutive::TestStr ts8{ testR8, "Persisting a DB contents" };
	// register test structures with TestExecutive instance, ex

	ex.registerTest(ts1);
	ex.registerTest(ts2);
	ex.registerTest(ts3a);
	ex.registerTest(ts3b);
	ex.registerTest(ts4);
	ex.registerTest(ts5);
	ex.registerTest(ts6);
	ex.registerTest(ts7);
	ex.registerTest(ts8);
	// run tests

	bool result = ex.doTests();
	if (result == true)
	std::cout << "\n  all tests passed";
	else
	std::cout << "\n  at least one test failed";

	putLine(2);
	return 0;
}