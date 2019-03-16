#include <vector>
#include <algorithm>
#include "p2Defs.h"
#include "p2Log.h"
#include "BuffManager.h"
#include <iostream> //To be able to use "unique_ptr"
#include "j1Render.h"
#include "j1App.h"
#include "PugiXml/src/pugiconfig.hpp"
#include "PugiXml/src/pugixml.hpp"
#include "j1Textures.h"
#include "SDL/include/SDL_rect.h"
#include "j1Window.h"
#include "j1Entity.h"

#pragma Buff region

Buff::Buff(BUFF_TYPE type, float value, uint source_id) :
	type(type),
	value(value),
	source_id(source_id)
{
}

BUFF_TYPE Buff::GetType()
{
	return type;
}

int Buff::GetValue()
{
	return value;
}

uint Buff::GetSource()
{
	return source_id;
}

bool Buff::IsCausedBySource(uint source_id) {
	return this->source_id == source_id;
}

#pragma endregion Buff



# pragma region Stat

Stat::Stat()
{
	finalValue = base;
}

void Stat::AddBuff(Buff buff)
{
	switch (buff.GetType())
	{
	case BUFF_TYPE::ADDITIVE:
		additive_buffs.push_back(&buff);
		break;
	case BUFF_TYPE::MULTIPLICATIVE:
		multiplicative_buffs.push_back(&buff);
		break;
	default:
		LOG("Buff type not detected.");
		break;
	}
}

//Searches through all the buffs and removes the ones caused by the source
void Stat::RemoveBuff(uint source_id)
{
	//additive_buffs.erase(std::remove_if(
	//	additive_buffs.begin(),
	//	additive_buffs.end(),
	//	[source_id](std::unique_ptr<Buff*>& e)
	//		{return (*e)->IsCausedBySource(source_id); }),
	//	additive_buffs.end());

	//multiplicative_buffs.erase(std::remove_if(
	//	multiplicative_buffs.begin(),
	//	multiplicative_buffs.end(),
	//	[source_id](std::unique_ptr<Buff*>& e)
	//		{return (*e)->IsCausedBySource(source_id); }),
	//	multiplicative_buffs.end());
	//TO IMPROVE: We could have an array of vectors and iterate through those.
	//In that way each time we add a new BUFF_TYPE we wouldn't have to create a new "remove_if" like we've done here
}

void Stat::CalculateStat()
{
	//1. Apply addtive buffs
	for (std::vector<Buff*>::iterator iter = additive_buffs.begin(); iter != additive_buffs.end(); ++iter)
	{
		finalValue += (*iter)->GetValue();
	}
	//2. Add multiplicative buffs and calculate the percentage
	float totalMult;
	for (std::vector<Buff*>::iterator iter = additive_buffs.begin(); iter != additive_buffs.end(); ++iter)
	{
		totalMult = (*iter)->GetValue();
	}
	finalValue += totalMult * finalValue;
}

int Stat::GetValue()
{
	return finalValue;
}

#pragma endregion Stat



#pragma region Character
Character::Character(pugi::xml_node character_node) : Entity(character_node.attribute("x").as_int(), character_node.attribute("y").as_int())
{
	max_health = curr_health = character_node.child("stats").child("health").attribute("value").as_int();
	attack.base = character_node.child("stats").child("attack").attribute("value").as_int();
	defense.base = character_node.child("stats").child("defense").attribute("value").as_int();
	tex_path = character_node.child("spritesheet").attribute("path").as_string();
	frame = {	character_node.child("animation").child("frame").attribute("x").as_int(),
				character_node.child("animation").child("frame").attribute("y").as_int(),
				character_node.child("animation").child("frame").attribute("w").as_int(),
				character_node.child("animation").child("frame").attribute("h").as_int() };
}

bool Character::Start()
{
	tex = App->tex->Load(tex_path.c_str());
	return false;
}

bool Character::Update(float dt)
{
	App->render->Blit(tex, x - frame.w * 0.5f, y - frame.h, &frame);

	//Health bar background
	//App->render->DrawQuad({});
	//Health bar fill
	return true;
}


void Character::DealDamage(Character reciever)
{
	int damage = attack.GetValue() - reciever.defense.GetValue();
	if (damage > 0)
	{
		reciever.curr_health -= damage;
		if (reciever.curr_health <= 0.f)
		{
			//Die
		}
	}
}

void Character::AddBuff() {
}

void Character::RemoveItem(int source_id) {
	//Go through each state and call Remove_buff
}
#pragma endregion Character