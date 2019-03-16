#include "j1Entity.h"
#include <vector>
#include "PugiXml/src/pugiconfig.hpp"
#include "PugiXml/src/pugixml.hpp"
#include "p2Log.h"
#include "BuffManager.h"

Entity::Entity(int x, int y) : x(x), y(y) {
}

bool Entity::Start()
{
	return true;
}

bool Entity::Update(float dt) {
	return true;
}

j1Entity::j1Entity() {
	name.assign("entity");
}

bool j1Entity::Awake(pugi::xml_node & node) {
	for (pugi::xml_node entity_node = node.child("entity"); entity_node != nullptr; entity_node = entity_node.next_sibling("entity")) {
		if (std::strcmp(entity_node.attribute("type").as_string(),"character") == 0) {
			entities.push_back(new Character(entity_node));
		}
		else {
			LOG("Entity type not found.");
		}
	}

	return true;
}

bool j1Entity::Start() {
	for (std::vector<Entity*>::iterator iter = entities.begin(); iter != entities.end(); ++iter)
	{
		(*iter)->Start();
	}
	return true;
}

bool j1Entity::Update(float dt) {
	for (std::vector<Entity*>::iterator iter = entities.begin(); iter != entities.end(); ++iter)
	{
		(*iter)->Update(dt);
	}

	return true;
}
