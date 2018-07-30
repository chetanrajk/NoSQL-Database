#pragma once
/////////////////////////////////////////////////////////////////////
// PersistDb.h - Implements persistence on Database                    //
// ver 1.0                                                         //
// Chetanraj Kadam, CSE687 - Object Oriented Design, Spring 2018   //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides mechanism for persisting db to xml 
* and retrieving db from xml
* 
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
#include <iomanip>
#include <unordered_map>
#include <string>
#include "..\XmlDocument\XmlElement\XmlElement.h"
#include "..\XmlDocument\XmlDocument\XmlDocument.h"
#include "..\XmlDocument\XmlParser\XmlParser.h"

using namespace XmlProcessing;

using Key = std::string;
using Children = std::vector<Key>;
using Keys = std::vector<Key>;

template <typename T>
class PersistDb
{
public:
	void persistDb(DbCore<T>& db);
	std::string dbtoXmlString(DbCore<T>& db);
	void stringtoXml(std::string xml);
	DbCore<T> constructDb(std::string fileName);
	void augmentDb(DbCore<T>& db, DbCore<T>& newdb);
};

template <typename T>
void PersistDb<T> :: persistDb(DbCore<T>& db)
{
	std::string Xml;
	Xml = dbtoXmlString(db);
	std::cout << "\n\n====> Displaying XML representation of current database:\n\n";
	std::cout << Xml << std::endl;
	std::cout << "\n\n====> Persisting a collection of database contents to XML:\n";
	std::cout << "\n\n  XML file is generated in project folder...\n  File name : ../noSqlDbContent.xml\n";
	stringtoXml(Xml);
}

template <typename T>
std::string PersistDb<T> :: dbtoXmlString(DbCore<T>& db)
{
	using Sptr = std::shared_ptr<AbstractXmlElement>;
	Sptr pDb = makeTaggedElement("db");
	Sptr pDocElem = makeDocElement(pDb);
	XmlDocument xDoc(pDocElem);
	
	for (auto item : db)
	{
		Sptr pRecord = makeTaggedElement("dbRecord");
		pDb->addChild(pRecord);
		Sptr pKey = makeTaggedElement("key", item.first);
		pRecord->addChild(pKey);

		Sptr pValue = makeTaggedElement("value");
		pRecord->addChild(pValue);

		Sptr pName = makeTaggedElement("name", item.second.name());
		pValue->addChild(pName);
		Sptr pDescrip = makeTaggedElement("description", item.second.descrip());
		pValue->addChild(pDescrip);
		Sptr pDateTime = makeTaggedElement("dateTime", item.second.dateTime());
		pValue->addChild(pDateTime);
		
		Sptr pPayload = makeTaggedElement("payload");
		pValue->addChild(pPayload);
		Sptr pPath = makeTaggedElement("path", item.second.payLoad().path());
		pPayload->addChild(pPath);

		Sptr pCategories = makeTaggedElement("categories");
		pPayload->addChild(pCategories);
		std::vector<std::string> catSet = item.second.payLoad().categories();
		for (Key cat : catSet)
		{
			Sptr pCategory = makeTaggedElement("category",cat);
			pCategories->addChild(pCategory);
		}

		Sptr pChildren = makeTaggedElement("children");
		pValue->addChild(pChildren);
		std::vector<std::string> childSet = item.second.children();
		for (Key key : childSet)
		{
			Sptr pChild = makeTaggedElement("child", key);
			pChildren->addChild(pChild);
		}
	}
	std::string Xml = xDoc.toString();
	return Xml;
}

template <typename T>
void PersistDb<T> ::stringtoXml(std::string xml)
{
	std::ofstream out("../noSqlDbContent.xml");
	out << xml;
	out.close();
}

template <typename T>
DbCore<T> PersistDb<T> ::constructDb(std::string fileName)
{
	DbCore<T> newDb;
	XmlParser parser(fileName);
	XmlDocument* pDoc = parser.buildDocument();
	const std::string& xml = pDoc->toString();
	std::cout << "\n\n   File name:   "<< fileName;
	//std::cout << "\n   Contents of New XML File:\n" << xml;
	using Sptr = std::shared_ptr<AbstractXmlElement>;
    XmlDocument newXDoc(pDoc->toString(), XmlDocument::str);
	using Key = std::string;
	std::vector<Sptr> records = newXDoc.descendents("dbRecord").select();
	for (auto pRecord : records)
	{
		Key key;
		std::vector<Key> childset;
		std::vector<Key> catset;
		DbElement<T> elem;
		std::vector<Sptr> pChildren = pRecord->children();
		for (auto pChild : pChildren)
		{
			if (pChild->tag() == "key")
			{
				key = pChild->children()[0]->value();
			}
			else
			{
				std::vector<Sptr> pValueChildren = pChild->children();
				for (auto pValueChild : pValueChildren)
				{
					if (pValueChild->tag() == "name")
					{
						elem.name(pValueChild->children()[0]->value());
					}
					if (pValueChild->tag() == "description")
					{
						elem.descrip(pValueChild->children()[0]->value());
					}
					if (pValueChild->tag() == "dateTime")
					{
						elem.dateTime(pValueChild->children()[0]->value());
					}
					if (pValueChild->tag() == "children")
					{
						std::vector<Sptr> pTemp = pValueChild->children();	
						for (auto pTempChild : pTemp)
						{
							childset.push_back(pTempChild->children()[0]->value());
						}
						elem.children(childset);
					}
					if (pValueChild->tag() == "payload")
					{
						std::vector<Sptr> pTempPayChild = pValueChild->children();				
						for (auto pTempPayload : pTempPayChild)
						{
							if (pTempPayload->tag() == "path")
							{
								elem.payLoad().path(pTempPayload->children()[0]->value());
							}
							else
							{
								std::vector<Sptr> pTemp2 = pTempPayload->children();
								for (auto pTempChild2 : pTemp2)
								{
									catset.push_back(pTempChild2->children()[0]->value());
								}
								elem.payLoad().categories(catset);
							}
						}
					}
				}
			}
		}
		newDb[key] = elem;
	}
	std::cout << "\n\n";
	return newDb;
}

template <typename T>
void PersistDb<T> :: augmentDb(DbCore<T>& db, DbCore<T>& newdb)
{
	Keys newkeys = newdb.keys();

	for (Key key : newkeys)
	{
		DbElement<T> demoelem = newdb[key];
		db[key] = demoelem;
	}

}
