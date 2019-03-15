#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "SDL/include/SDL_rect.h"

struct SDL_Texture;

class BaseAttribute {
private:
	int base_value;
	float base_multiplier;
	
public:
	BaseAttribute(int value, float multiplier) :
		base_value(value), base_multiplier(multiplier) {

	}

	int GetBaseValue() {
		return base_value;
	}

	float GetMultiplier() {
		return base_multiplier;
	}
};

class RawBonus : public BaseAttribute {
public:
	RawBonus(int value = 0, float multiplier = 0.f) : BaseAttribute(value, multiplier) {

	}
};

class FinalBonus : public BaseAttribute {
public:
	FinalBonus(int value = 0, float multiplier = 0.f) : BaseAttribute(value, multiplier) {

	}
};

//class Attribute : public BaseAttribute {
//private:
//	RawBonus * raw_bonuses;
//	FinalBonus * final_bonuses;
//	int final_value;
//
//public:
//	Attribute(int starting_value) : BaseAttribute(starting_value) {
//
//	}
//};

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