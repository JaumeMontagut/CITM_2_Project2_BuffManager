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

bool Module_Buff::OnClick(UI_Object * object)
{
	if (object == attack_button) {
		App->scene->caster->DealDamage(App->scene->target);
	}

	return true;
}

void Module_Buff::AddOutPutText(std::string text)
{
	for (std::list<Label*>::reverse_iterator iter = output_Texts.rbegin(); iter != output_Texts.rend(); ++iter)
	{
		iPoint newPos = { (*iter)->position.x,(*iter)->position.y + 30 };
		(*iter)->SetPosition(newPos);
	}

	output_Texts.push_back(App->ui->CreateLabel({ 700,0 }, text, App->ui->pixel_font_small, nullptr, { 0,0,0,255 }));

	if (output_Texts.size() > 30)
	{
		App->ui->DeleteObject((UI_Object*)*output_Texts.begin());
		output_Texts.pop_front();
	}
}