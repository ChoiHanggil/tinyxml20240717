#include <iostream>
#include"monster.h"
#include"tinyxml2.h"
//XMLDocument
//XMLDeclaration
//XMLElement

using namespace tinyxml2;

bool SaveToXML(const char* filename, const std::vector<Monster>& monsters)
{
	XMLDocument doc;
	XMLDeclaration* decl = doc.NewDeclaration("xml version = \"1.0\" encoding = \"UTF-8\"");
	doc.LinkEndChild(decl);

	XMLElement* pRoot = doc.NewElement("monsters");

	for (auto monster : monsters)
	{
		auto pNode = doc.NewElement("monster");

		pNode->SetAttribute("name", monster.GetName().c_str());
		auto pStatus = doc.NewElement("status");
		Status stat = monster.GetStatus();
		pStatus->SetAttribute("level", stat.mLevel);
		pStatus->SetAttribute("hp", stat.mHp);
		pStatus->SetAttribute("mp", stat.mMp);
		pNode->LinkEndChild(pStatus);

		auto pItems = doc.NewElement("items");
		for (auto item : monster.GetDropItems())
		{
			auto pItem = doc.NewElement("item");
			pItem->SetAttribute("name", item.mName.c_str());
			pItem->SetAttribute("price", item.mPrice);
			pItems->LinkEndChild(pItem);
		}
		pNode->LinkEndChild(pItems);

		pRoot->LinkEndChild(pNode);
	}

	doc.LinkEndChild(pRoot);

	return doc.SaveFile(filename) == XML_SUCCESS;
}

bool LoadFromXML(const char* filename, std::vector<Monster>& monsters)
{
	XMLDocument doc;
	if(doc.LoadFile(filename) != XML_SUCCESS)
	{
		return false;
	}
	auto pMonstersNode = doc.FirstChildElement("monsters");
	for (auto pMonsterNode = pMonstersNode->FirstChildElement(); pMonsterNode; pMonsterNode = pMonsterNode->NextSiblingElement())
	{
		Monster m;
		m.SetName(pMonsterNode->Attribute("name"));

		auto pStatusNode = pMonsterNode->FirstChildElement("status");
		Status s;
		s.mLevel = pStatusNode->IntAttribute("level");
		s.mHp = pStatusNode->IntAttribute("hp");
		s.mHp = pStatusNode->IntAttribute("mp");
		m.SetStatus(s);

		auto pItemsNode = pMonsterNode->FirstChildElement("items");
		for (auto pItemNode = pItemsNode->FirstChildElement(); pItemNode; pItemNode = pItemNode->NextSiblingElement())
		{
			Item item;
			item.mName = pItemNode->Attribute("name");
			item.mPrice = pItemNode->IntAttribute("price");

			m.AddDropItem(item);
		}
		monsters.push_back(m);
	}

	return true;
}

#include<rapidjson/document.h>
#include<rapidjson/prettywriter.h>
#include<fstream>
#include<sstream>

using namespace rapidjson;

bool SaveToJSON(const char* filename, const std::vector<Monster>& monsters)
{
	StringBuffer sb;
	PrettyWriter<StringBuffer> writer(sb);

	writer.StartObject();
	{
		writer.Key("monsters"); writer.StartArray();

		for (auto monster : monsters)
		{
			writer.StartObject();
			{
				writer.Key("name"); writer.String(monster.GetName().c_str());

				writer.Key("status"); writer.StartObject();
				{
					Status s = monster.GetStatus();
					writer.Key("level"); writer.Int(s.mLevel);
					writer.Key("hp"); writer.Int(s.mHp);
					writer.Key("mp"); writer.Int(s.mMp);
				}
				writer.EndObject();

				writer.Key("items"); writer.StartArray();
				{
					for (auto item : monster.GetDropItems())
					{
						writer.StartObject();
						writer.Key("name"); writer.String(item.mName.c_str());
						writer.Key("price"); writer.Int(item.mPrice);
						writer.EndObject();
					}
				}
				writer.EndArray();
			}
			writer.EndObject();
		}
		writer.EndArray();
	}
	writer.EndObject();

	std::ofstream ofs(filename);
	ofs << sb.GetString();
	ofs.close();

	return true;
}

bool LoadFromJSON(const char* filename, std::vector<Monster>& monsters)
{
	std::ifstream ifs(filename);
	std::stringstream ss;
	ss << ifs.rdbuf();
	ifs.close();

	Document doc;
	doc.Parse(ss.str().c_str());

	for (auto& element : doc["monsters"].GetArray())
	{
		Monster m;
		m.SetName(element["name"].GetString());

		Status s;
		auto statObject = element["status"].GetObject();
		s.mLevel = statObject["level"].GetInt();
		s.mHp = statObject["hp"].GetInt();
		s.mMp = statObject["mp"].GetInt();
		m.SetStatus(s);

		for (auto& element2 : element["items"].GetArray())
		{
			Item item;
			item.mName = element2["name"].GetString();
			item.mPrice = element2["price"].GetInt();
			m.AddDropItem(item);
		}
		monsters.push_back(m);
	}

	return true;
}

int main()
{
	std::vector<Monster> monsters;

	Monster m;
	m.SetName("Jelly");
	m.SetStatus({ 1,1,1 });
	m.AddDropItem({ "money", 1 });
	monsters.push_back(m);

	m.GetDropItems().clear();
	m.SetName("WereWlf");
	m.SetStatus({ 5,5,5 });
	m.AddDropItem({ "claw",10 });
	m.AddDropItem({ "leather",15 });
	monsters.push_back(m);

	m.GetDropItems().clear();
	m.SetName("Demon");
	m.SetStatus({ 10,10,10 });
	m.AddDropItem({ "taill",10 });
	m.AddDropItem({ "money",100 });
	monsters.push_back(m);

	/*
	SaveToXML("Data/monsters.mxl", monsters);

	monsters.clear();
	LoadFromXML("Data/monsters.mxl", monsters);
	*/
	
	SaveToJSON("Data/monsters.json", monsters);

	monsters.clear();
	LoadFromJSON("Data/monsters.json", monsters);
	
}