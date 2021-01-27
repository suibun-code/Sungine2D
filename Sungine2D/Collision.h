#pragma once

#include <string>

class Collision
{
private:
	std::string name;
	std::string tag;

public:
	void SetName(std::string name) { this->name = name; }
	void SetTag(std::string tag) { this->tag = tag; }

	std::string GetName() { return name; }
	std::string GetTag() { return tag; }
	
};
