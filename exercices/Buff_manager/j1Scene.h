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
};

//A class used in any stat which can be boosted by modifiers (equipment, skills, talent trees, enviroment, synergies, etc.)
class Stat { 
private:
	int base;// The value of the stat without adding any buff
	int finalValue;
	std::vector<Buff> additive_buffs;
	std::vector<Buff> multiplicative_buffs;

public:
	int GetValue() {
		return finalValue;
	}

	void CalculateStat() {
		//1. Apply addtive buffs
		//2. Add multiplicative buffs and calculate the percentage
	}
};

class Character {
private:
	int curr_health;
	Stat max_health;
	
	int curr_mana;
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
			if (reciever.curr_health <= 0) {
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