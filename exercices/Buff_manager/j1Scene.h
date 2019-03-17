#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "SDL/include/SDL_rect.h"
#include <vector>
#include <algorithm>
#include "BuffManager.h"
#include "Module_UI.h"

class Button;
struct SDL_Texture;

class j1Scene : public j1Module, public Gui_Listener
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

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

	//virtual bool OnHover(UI_Object* object) { return true; }
	//virtual bool RepeatHover(UI_Object* object) { return true; }
	//virtual bool OutHover(UI_Object* object) { return true; }

	virtual bool OnClick(UI_Object* object);
	//virtual bool RepeatClick(UI_Object* object) { return true; }
	//virtual bool OutClick(UI_Object* object) { return true; }

public:
	Character* caster = nullptr;
	Character* target = nullptr;

	TTF_Font * pixel_font = nullptr;

	Button * attack_button = nullptr;
};

#endif // __j1SCENE_H__