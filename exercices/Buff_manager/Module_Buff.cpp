#include <vector>
#include <algorithm>
#include "p2Defs.h"
#include "p2Log.h"
#include "Module_Buff.h"
#include <iostream> //To be able to use "unique_ptr"
#include "j1Render.h"
#include "j1App.h"
#include "PugiXml/src/pugiconfig.hpp"
#include "PugiXml/src/pugixml.hpp"
#include "j1Textures.h"
#include "SDL/include/SDL_rect.h"
#include "j1Window.h"
#include "j1Entity.h"
#include "j1Module.h"
#include "UI_Object.h"
#include "Module_UI.h"
#include "Button_Input.h"
#include "Label.h"
#include "j1Input.h"
#include "j1Scene.h"

Module_Buff::Module_Buff() : j1Module()
{
	name.assign("buff");
}

bool Module_Buff::Awake(pugi::xml_node & buff_node)
{
	this->buff_node = buff_node;
	return true;
}

bool Module_Buff::Start() {
	for (pugi::xml_node spell_node = buff_node.child("spell"); spell_node; spell_node = spell_node.next_sibling("spell"))
	{
		spells.push_back(new Spell(spell_node));
	}

	//App->ui->CreateLabel({ 50, 50 }, "this is a label", pixel_font, this);
	//App->ui->CreateImage({ 50, 50 }, {0,0,16,16}, this);
	attack_button = App->ui->CreateButton({ 60, 605 }, { 0,0,16,16 }, App->buff);
	attack_button->SetLabel(
		{ attack_button->position.x, attack_button->position.y + attack_button->section.h * attack_button->scale_factor },
		"sword",
		App->ui->pixel_font_small);
	//Center label (button center - label half width)
	attack_button->label->position.x = (attack_button->position.x + attack_button->section.w * attack_button->scale_factor * 0.5f) - (attack_button->label->section.w * 0.5f);

	return true;
}

uint Module_Buff::GetNewSourceID()
{
	return last_source_id++;
}

BUFF_TYPE Module_Buff::GetBuffType(std::string buff_type)
{
	if (std::strcmp(buff_type.c_str(), "additive") == 0) {
		return BUFF_TYPE::ADDITIVE;
	}
	else if (std::strcmp(buff_type.c_str(), "multiplicative") == 0) {
		return BUFF_TYPE::MULTIPLICATIVE;
	}
	else {
		LOG("Buff type not found.");
	}
}

STAT_TYPE Module_Buff::GetStatType(std::string stat_type)
{
	if (std::strcmp(stat_type.c_str(), "attack") == 0) {
		return STAT_TYPE::ATTACK;
	}
	else if (std::strcmp(stat_type.c_str(), "defense") == 0) {
		return STAT_TYPE::DEFENSE;
	}
	else {
		LOG("Stat type not found.");
	}
}

bool Module_Buff::OnClick(UI_Object * object)
{
	if (object == attack_button) {
		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
			App->scene->caster->DealDamage(App->scene->target);
		}
		LOG("attack button clicked");
	}

	return true;
}