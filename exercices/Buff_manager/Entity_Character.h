#ifndef __ENTITY_CHARACTER_H__
#define __ENTITY_CHARACTER_H__

#include <vector>
#include <algorithm>
#include "p2Defs.h"
#include "p2Log.h"
#include "PugiXml/src/pugiconfig.hpp"
#include "PugiXml/src/pugixml.hpp"
#include "SDL/include/SDL_rect.h"
#include "j1Entity.h"
#include <string.h>
#include <map>

struct SDL_Texture;
class Label;

enum class BUFF_TYPE {
	ADDITIVE, //AKA: Flat, raw
	MULTIPLICATIVE, // AKA: Percent
	MAX
};

enum class STAT_TYPE {
	ATTACK,
	DEFENSE,
	MAX
};

class Buff {
private:
	BUFF_TYPE type = BUFF_TYPE::MAX;
	std::string stat = "\0";
	float value = 0.0f;
	uint source_id = 0;//ID from which modifier (object, spell, etc) the buff came from

public:
	Buff(BUFF_TYPE type, std::string stat, float value, uint source_id);
	BUFF_TYPE GetType();
	std::string GetStat();
	float GetValue();
	uint GetSource();
	bool IsCausedBySource(uint source_id);
};

class BuffSource {
public:
	BuffSource(pugi::xml_node buff_source_node);

public:
	uint source_id;
	std::list<Buff*> buffs;
};

class Spell : public BuffSource {
public:
	Spell(pugi::xml_node spell_node);

	//INFO: Function pointer
	//INFO: Points to the function that's going to be called when the spell is executed
	void(*function_ptr)(Spell *) = nullptr;

public:
	bool is_active = false;//INFO: Used in spells which can be toggled
	//Possible stats
	//- Mana cost
	//- Duration
};

//class Equipment : public BuffSource {
//	Possible stats
//	- Durability
//	- Cost
//};

//A class used in any stat which can be boosted by modifiers (equipment, skills, talent trees, enviroment, synergies, etc.)
class Stat {
private:
	float final_value;
	std::vector<Buff*> additive_buffs;
	std::vector<Buff*> multiplicative_buffs;

public:
	float base_value;// The value of the stat without adding any buff
	//TO IMPROVE: Make this field private
	Stat(float base);

	void AddBuff(Buff & buff);
	void RemoveBuff(uint source_id);
	void CalculateStat();
	float GetValue();
};

class Character : public Entity
{
public:
	int curr_health;
	bool alive = true;
	int max_health;
	std::string tex_path = "\0";
	SDL_Texture * tex = nullptr;
	SDL_Rect frame = { 0, 0, 0, 0 };
	std::string character_name = "\0";
	std::map<std::string, Stat*> stats;

private:
	std::map<std::string, Label*> stat_labels;

public:
	Character(pugi::xml_node character_node);
	bool Start() override;
	bool Update(float dt) override;

	void AddBuff(BuffSource * buff_source);
	void RemoveBuff(BuffSource * buff_source);
	void UpdateStatLabels();

private:
	int GetStatBaseValue(STAT_TYPE stat, pugi::xml_node stats_node);
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

#endif // !__ENTITY_CHARACTER_H__

