#ifndef __MODULE_BUFF_MANAGER_H__
#define __MODULE_BUFF_MANAGER_H__

#include "j1Module.h"
#include "PugiXml/src/pugiconfig.hpp"
#include "PugiXml/src/pugixml.hpp"
#include "Module_UI.h"
#include "Entity_Character.h"

class UI_Object;
class Button;

class Module_Buff : public j1Module, public Gui_Listener {
public:
	Module_Buff();
	bool Awake(pugi::xml_node & buf_node) override;
	bool Start() override;

	//Next turn

	uint GetNewSourceID();
	BUFF_TYPE GetBuffType(std::string);
	STAT_TYPE GetStatType(std::string);


	//Button functions
	//virtual bool OnHover(UI_Object* object) { return true; }
	//virtual bool RepeatHover(UI_Object* object) { return true; }
	//virtual bool OutHover(UI_Object* object) { return true; }
	virtual bool OnClick(UI_Object* object);
	//virtual bool RepeatClick(UI_Object* object) { return true; }
	//virtual bool OutClick(UI_Object* object) { return true; }

private:
	uint last_source_id = 0u;

	pugi::xml_node buff_node;

	std::list<Spell*> spells;

	//Spell buttons
	Button *			attack_button = nullptr;
};

#endif
