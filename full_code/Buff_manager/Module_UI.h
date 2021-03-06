#ifndef __MODULE_UI_H__
#define __MODULE_UI_H__

#include "j1Module.h"
#include "SDL_ttf/include/SDL_ttf.h"
#include "Animation.h"
#include "p2Point.h"
#include <string>
#include <list>

#define CURSOR_WIDTH 2

using namespace std;
typedef string String;

class UI_Object;
enum class ObjectState;

class Gui_Listener;
class Image;
class Button;
class Slider;
class Label;
class Checkbox;
class TextPanel;

struct Button_Definition;
struct Slider_Definition;
struct Checkbox_Definition;
struct TextPanel_Definition;

enum class ClickState
{
	On,
	Out,
	Repeat,
	None
};

class Module_UI : public j1Module
{
public:

	Module_UI();

	virtual ~Module_UI();

	bool Awake(pugi::xml_node& node);

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	SDL_Texture* GetAtlas() const;

	ClickState GetClickState() const;

	// Creation functions ---------------------------------------------------------
	UI_Object* CreateObject(iPoint position, Gui_Listener* listener = nullptr);

	Label* CreateLabel(iPoint position, String text, _TTF_Font* font, Gui_Listener* listener = nullptr ,SDL_Color color = {255,255,255,255});

	Image* CreateImage(iPoint position, SDL_Rect rect, Gui_Listener* listener = nullptr);

	Button* CreateButton(iPoint position, SDL_Rect rect, Gui_Listener* listener = nullptr);

	Slider* CreateSlider(iPoint position, Slider_Definition definition, Gui_Listener* listener = nullptr);

	TextPanel * CreateTextPanel(const iPoint position, TextPanel_Definition definition, Gui_Listener* listener);

	// Object functions ----------------------------------------------------------
	UI_Object*  GetClickedObject();

	UI_Object* GetScreen();

	bool DeleteObject(UI_Object* object);

	void SetStateToBranch(const ObjectState state, UI_Object* branch_root);

private:

	bool SelectClickedObject();

	void DrawUI(UI_Object* object); // Recursive function to draw gui as a tree

	void UpdateGuiPositions(UI_Object* object, iPoint cumulated_position);

public:
	// Atlas Texture ---------------------------------------
	SDL_Texture*		atlas = nullptr;

	TTF_Font *			pixel_font = nullptr;
	TTF_Font *			pixel_font_small = nullptr;

	iPoint				cursor_position;

private:

	bool debug = false;

	// Objects ---------------------------------------------
	list<UI_Object*> objects_list;

	UI_Object* screen = nullptr;

	UI_Object* clicked_object = nullptr;

	ClickState click_state = ClickState::None;
};


class Gui_Listener
{
public:
	Gui_Listener()
	{}

	virtual bool OnHover(UI_Object* object) { return true; }
	virtual bool RepeatHover(UI_Object* object) { return true; }
	virtual bool OutHover(UI_Object* object) { return true; }

	virtual bool OnClick(UI_Object* object) { return true; }
	virtual bool RepeatClick(UI_Object* object) { return true; }
	virtual bool OutClick(UI_Object* object) { return true; }
};



#endif // __MODULE_UI_H__