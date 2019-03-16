#ifndef __BUFF_MANAGER_H__
#define __BUFF_MANAGER_H__

#include <vector>
#include <algorithm>
#include "p2Defs.h"
#include "p2Log.h"
#include "PugiXml/src/pugiconfig.hpp"
#include "PugiXml/src/pugixml.hpp"
#include "SDL/include/SDL_rect.h"
#include "j1Entity.h"
#include <string.h>

struct SDL_Texture;

enum class BUFF_TYPE {
	ADDITIVE, //AKA: Flat, raw
	MULTIPLICATIVE, // AKA: Percent
};

class Buff {
private:
	BUFF_TYPE type;
	float value;
	uint source_id;//ID from which modifier (object, etc) the buff came from

public:
	Buff(BUFF_TYPE type, float value, uint source_id);
	BUFF_TYPE GetType();
	int GetValue();
	uint GetSource();
	bool IsCausedBySource(uint source_id);
};

//A class used in any stat which can be boosted by modifiers (equipment, skills, talent trees, enviroment, synergies, etc.)
class Stat { 
private:
	float final_value;
	std::vector<Buff*> additive_buffs;
	std::vector<Buff*> multiplicative_buffs;

public:
	float base;// The value of the stat without adding any buff
	//TO IMPROVE: Make this field private
	Stat(int base);

	void AddBuff(Buff buff);
	void RemoveBuff(uint source_id);
	void CalculateStat();
	int GetValue();
};

class Character : public Entity
{
private:
	int curr_health;
	int max_health;

	Stat attack;
	Stat defense;

	std::string tex_path = "\0";
	SDL_Texture * tex = nullptr;
	SDL_Rect frame = { 0, 0, 0, 0 };

public:
	Character(pugi::xml_node character_node);
	bool Start() override;
	bool Update(float dt) override;

	void DealDamage(Character * reciever);
	void AddBuff();
	void RemoveItem(int source_id);
};
//Other possible stats:
//- Stat xp_gain;
//- Stat gold_gain;
//- float curr_mana;
//- Stat max_mana;
//- Stat max_health;
//- Evasion
//- Critical chance
//- Movement points
//- Attack speed
//- Cooldown
//- Range
//- Cost (to deploy a unit)
//- Duration
#endif
