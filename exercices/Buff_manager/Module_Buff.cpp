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
#include "Entity_Character.h"

Module_Buff::Module_Buff() : j1Module()
{
	name.assign("buff");
}

bool Module_Buff::Awake(pugi::xml_node & buff_node)
{
	this->buff_node = buff_node;
	FillFunctionsMap();
	return true;
}

bool Module_Buff::Start() {
	Button * new_button = nullptr;
	for (pugi::xml_node spell_node = buff_node.child("spell"); spell_node; spell_node = spell_node.next_sibling("spell"))
	{
		//Create the spell
		spells.push_back(new Spell(spell_node));

		//TO IMPROVE: Set position dynamically
		//Create a button for the spell
		new_button = App->ui->CreateButton(
			{ 60, 605 },
			{ 16 * spell_node.child("atlas_icon").attribute("column").as_int(), 16 * spell_node.child("atlas_icon").attribute("row").as_int(), 16, 16 },
			App->buff);
		new_button->SetLabel(
			{ new_button->position.x, new_button->position.y + new_button->section.h * new_button->scale_factor },
			spell_node.attribute("name").as_string(),
			App->ui->pixel_font_small);
		//TO IMPROVE: Center label (button center - label half width)
		//new_button->label->position.x = (attack_button->position.x + attack_button->section.w * attack_button->scale_factor * 0.5f) - (attack_button->label->section.w * 0.5f);
		spell_buttons.push_back(new_button);
	}
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

void Module_Buff::FillFunctionsMap()
{
	spell_functions["cut"] = &Cut;
}

void(*Module_Buff::GetFunctionPointer(std::string functionName))(Spell *) {
	return spell_functions[functionName];
}

bool Module_Buff::OnClick(UI_Object * object)
{
	for (int i = 0; i < spell_buttons.size(); ++i)
	{
		if (spell_buttons[i] == object)
		{
			spells[i]->function_ptr(spells[i]);
		}
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

void Cut(Spell * spell)
{
	if (App->scene->goblin->alive) {
		int damage = App->scene->dwarf->stats["attack"]->GetValue() - App->scene->goblin->stats["defense"]->GetValue();
		if (damage > 0)
		{
			App->scene->goblin->curr_health -= damage;
			App->buff->AddOutPutText(App->scene->dwarf->character_name + " dealt " + std::to_string(damage) + " damage to " + App->scene->goblin->character_name + ".");
			if (App->scene->goblin->curr_health <= 0 && App->scene->goblin->alive)
			{
				App->scene->goblin->alive = false;
				App->scene->goblin->curr_health = 0;
				App->buff->AddOutPutText(App->scene->goblin->character_name + " died.");//TO IMPROVE: Add character names
			}
		}
	}
	else {
		App->buff->AddOutPutText("it's already dead...");
	}
}

void AddSpellBuff(Spell * spell) {
	for (std::list<Buff*>::iterator buff = spell->buffs.begin(); buff != spell->buffs.end(); ++buff) {
		App->scene->dwarf->stats[(*buff)->GetStat()]->AddBuff(*(*buff));
	}
}

void RemoveSpellBuff(Spell * spell) {
	for (std::list<Buff*>::iterator buff = spell->buffs.begin(); buff != spell->buffs.end(); ++buff) {
		App->scene->dwarf->stats[(*buff)->GetStat()]->RemoveBuff((*buff)->GetSource());
	}
	//Go through each state and call Remove_buff
}
