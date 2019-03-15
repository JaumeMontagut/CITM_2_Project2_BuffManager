#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "SDL/include/SDL_rect.h"
#include <vector>

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
	Buff(BUFF_TYPE type, float value, uint source_id) :
		type(type),
		value(value),
		source_id(source_id) {
	}

	BUFF_TYPE GetType() {
		return type;
	}

	int GetValue() {
		return value;
	}

	uint GetSource() {
		return source_id;
	}
};

//A class used in any stat which can be boosted by modifiers (equipment, skills, talent trees, enviroment, synergies, etc.)
class Stat { 
private:
	float base;// The value of the stat without adding any buff
	float finalValue;
	std::vector<Buff> additive_buffs;
	std::vector<Buff> multiplicative_buffs;

public:
	Stat() {
		finalValue = base;
	}

	void AddBuff(Buff buff) {
		switch (buff.GetType())
		{
		case BUFF_TYPE::ADDITIVE:
			additive_buffs.push_back(buff);
			break;
		case BUFF_TYPE::MULTIPLICATIVE:
			multiplicative_buffs.push_back(buff);
			break;
		default:
			LOG("Buff type not detected.");
			break;
		}
	}

	void CalculateStat() {
		//1. Apply addtive buffs
		for (std::vector<Buff>::iterator *iter = additive_buffs.begin; iter != additive_buffs.end; *iter++) {
			(*iter)->GetValue();
		}
		//2. Add multiplicative buffs and calculate the percentage
	}

	int GetValue() {
		return finalValue;
	}
};

class Character {
private:
	float curr_health;
	Stat max_health;
	
	float curr_mana;
	Stat max_mana;

	Stat attack;

	Stat defense;

	Stat xp_gain;

	Stat gold_gain;

public:
	void DealDamage(Character reciever) {
		int damage = attack.GetValue() - reciever.defense.GetValue();
		if (damage > 0) {
			reciever.curr_health -= damage;
			if (reciever.curr_health <= 0.f) {
				//Die
			}
		}
	}

	//void AddBuff() {
	//}
	
	//void RemoveBuff() {
	//}
};
//Other possible stats:
//- Evasion
//- Critical chance
//- Movement points
//- Attack speed
//- Cooldown
//- Range
//- Cost (to deploy a unit)

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();
};

#endif // __j1SCENE_H__