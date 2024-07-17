#pragma once

#include<string>
#include<vector>

struct Status
{
	int mLevel;
	int mHp;
	int mMp;
};

struct Item
{
std::string mName;
int mPrice;

};

class Monster
{
private:
	std::string mName;
	Status mStatys;
	std::vector<Item> mDropItems;

public:
	Monster() :mName(), mStatys(), mDropItems() {}

	std::string GetName() const { return mName; }
	void SetName(const char* name) { mName = name; }

	Status GetStatus() const { return mStatys; }
	void SetStatus(const Status& stat) { mStatys = stat; }

	std::vector<Item>& GetDropItems() { return mDropItems; }
	void AddDropItem(const Item& item) { mDropItems.push_back(item); }
};