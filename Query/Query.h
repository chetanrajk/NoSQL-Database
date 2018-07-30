#pragma once
/////////////////////////////////////////////////////////////////////
// Query.h - Implements Queries on Database                        //
// ver 1.0                                                         //
// Chetanraj Kadam, CSE687 - Object Oriented Design, Spring 2018   //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides Query class
* -
* Required Files:
* ---------------
* DbCore.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 05 Feb 2018
* - first release
*/

#include "../DbCore/DbCore.h"
#include <regex>
#include <set>

using namespace NoSqlDb;

using Key = std::string;
using Children = std::vector<Key>;
using Keys = std::vector<Key>;


template <typename T>
class Query {
public:
	DbElement<T> Query1(DbCore<T>& db, Key querykey);
	Children Query2(DbCore<T>& db, Key querykey);
	Keys Query3(DbCore<T>& db, std::string str);
	Keys Query4(DbCore<T>& db, std::string str);
	Keys Query5(DbCore<T>& db, std::string str);
	Keys Query6(DbCore<T>& db, std::string str);
	Keys Query7(DbCore<T>& db, std::string startTime, std::string endTime);
	Keys Query8(DbCore<T>& db, std::string parent, std::string pattern);
	Keys Query9(DbCore<T>& db, std::string pattern, std::string desc);
};

template <typename T>
DbElement<T> Query<T>::Query1(DbCore<T>& db, Key querykey)
{
	DbElement<T> dbElem;
	Keys keySet = db.keys();

	for (Key key : keySet)
	{
		if (key == querykey)
		{
			dbElem = db[key];
			showHeader();
			showElem(dbElem);
			return dbElem;
		}
	}
	std::cout << "\n\n  Given Key not present in Database ";
	return dbElem;
}

template <typename T>
Children Query<T>::Query2(DbCore<T>& db, Key querykey)
{
	if (!db.contains(querykey))
	{
		std::cout << "\n\n Given Key not present in Database";
		return Children();
	}
	putLine();
	DbElement<T> dbElem = db[querykey];
	std::cout << "  Child Keys:";
	for(Key key : dbElem.children())
	{
		std::cout << "\t" << key;
	}
	putLine();
	return dbElem.children();
}

template <typename T>
Keys Query<T>::Query3(DbCore<T>& db, std::string str)
{
	Keys result;
	Keys keySet = db.keys();
	std::regex e("(.*)" + str + "(.*)");

	putLine();
	std::cout << "  Matched Keys:";
	for (Key key : keySet)
	{
		if (regex_match(key, e))
		{
			std::cout << "\t " << key;
			result.push_back(key);
		}
	}
	if (result.empty())
	{
		std::cout << "\n\n  Given pattern does not match with any key in Database";
		return result;
	}
	return result;
}

template <typename T>
Keys Query<T>::Query4(DbCore<T>& db, std::string str)
{
	Keys result;
	Keys keySet = db.keys();
	DbElement<T> dbElem;
	std::string name;
	std::regex e("(.*)" + str + "(.*)");

	putLine();
	std::cout << "  Matched Keys:";
	for (Key key : keySet)
	{
		dbElem = db[key];
		name = dbElem.name();
		if (regex_match(name, e))
		{
			std::cout << "\t " << key;
			result.push_back(key);
		}
	}
	if (result.empty())
	{
		std::cout << "\n\n  No keys in Database contains provided string in their Name section";
		return result;
	}
	return result;
}

template <typename T>
Keys Query<T>::Query5(DbCore<T>& db, std::string str)
{
	Keys result;
	Keys keySet = db.keys();
	DbElement<T> dbElem;
	std::string description;
	std::regex e("(.*)" + str + "(.*)");

	putLine();
	std::cout << "  Matched Keys:";
	for (Key key : keySet)
	{
		dbElem = db[key];
		description = dbElem.descrip();
		if (regex_match(description, e))
		{
			std::cout << "\t " << key;
			result.push_back(key);
		}
	}
	if (result.empty())
	{
		std::cout << "\n\n  No keys in Database contains provided string in their Description section";
		return result;
	}

	return result;
}

template <typename T>
Keys Query<T>::Query6(DbCore<T>& db, std::string str)
{
	Keys result;
	Keys keySet = db.keys();
	DbElement<T> dbElem;
	std::vector<std::string> catSet;
	std::regex e("(.*)" + str + "(.*)");

	putLine();
	std::cout << "  Matched Keys:";
	for (Key key : keySet)
	{
		dbElem = db[key];
		catSet = dbElem.payLoad().categories();

		for (Key cat : catSet)
		{
			if (regex_match(cat, e))
			{
				std::cout << "\t " << key;
				result.push_back(key);
			}
		}
	}
	if (result.empty())
	{
		std::cout << "\n\n  No keys in Database contains provided category in their Payload section";
		return result;
	}

	return result;
}


template <typename T>
Keys Query<T>::Query7(DbCore<T>& db, std::string startTime, std::string endTime)
{
	Keys result;
	Keys keySet = db.keys();
	DbElement<T> dbElem;
	DateTime start = DateTime(startTime);
	DateTime end;
	DateTime elemTime;

	if (!endTime.empty())
		 end = DateTime(endTime);
	else
		 end = DateTime();
	
	std::cout << "\n  Matched Keys:";
	for (Key key : keySet)
	{
		dbElem = db[key];
		elemTime = dbElem.dateTime();

		if (start < elemTime && elemTime < end)
		{
			std::cout << "\t " << key;
			result.push_back(key);
		}
	}
	if (result.empty())
	{
		std::cout << "\n\n  There are no keys present within given time-date interval";
		return result;
	}
	return result;
}

template <typename T>
Keys Query<T>::Query8(DbCore<T>& db, std::string parent, std::string pattern)
{
	Keys result;
	
	DbElement<T> dbElem;
	std::string name;
	std::regex e("(.*)" + pattern + "(.*)");

	Keys keySet = Query2(db, parent);
	putLine();
	std::cout << "  Matched Keys:";
	for (Key key : keySet)
	{
		dbElem = db[key];
		name = dbElem.name();
		if (regex_match(name, e))
		{
			std::cout << "\t " << key;
			result.push_back(key);
		}
	}
	if (result.empty())
	{
		std::cout << "\n\n No keys in the given key set contains provided string in their Name section";
		return result;
	}
	return result;
}

template <typename T>
Keys Query<T>::Query9(DbCore<T>& db, std::string pattern, std::string desc)
{
	Keys resultunion;
	Keys result3 = Query3(db, pattern);
	Keys result5 = Query5(db, desc);

	std::set<std::string> res3(result3.begin(), result3.end());
	std::set<std::string> res5(result5.begin(), result5.end());

	std::set_union(res3.begin(), res3.end(), res5.begin(), res5.end(), std::back_inserter(resultunion));
	putLine();
	std::cout << "\n  Union of Matched Keys:";
	for (Key key : resultunion)
	{
		std::cout << " "<<key;
	}
	return resultunion;
}
