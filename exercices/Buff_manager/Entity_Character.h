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

	std::map<std::string, Stat*> stats;

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

class BuffSource {
public:
	BuffSource(pugi::xml_node buff_source_node);

private:
	uint source_id;
	std::list<Buff*> buffs;
};

class Spell : public BuffSource {
public:
	Spell(pugi::xml_node spell_node);
	//Possible stats
	//- Mana cost
	//- Duration
};

//class Equipment : public BuffSource {
//	Possible stats
//	- Durability
//	- Cost
//};

#endif // !__ENTITY_CHARACTER_H__

