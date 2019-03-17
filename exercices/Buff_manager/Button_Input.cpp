#include "Button_Input.h"
#include "Module_UI.h"
#include "j1Render.h"
#include  "p2Log.h"
#include "j1Input.h"
#include "Label.h"
#include "j1Audio.h"
#include "j1App.h"
#include "j1Window.h"


Button::Button(const iPoint position, const SDL_Rect rect, SDL_Texture * texture, Gui_Listener* listener) : UI_Object(position, listener)
{
	this->rect = rect;
	this->texture = texture;
	int scale = (int)App->win->GetScale();
	section.w = rect.w /** scale*/;
	section.h = rect.h /** scale*/;
}

Button::~Button()
{
	if (label != nullptr)
	{
		App->ui->DeleteObject(label);
		label = nullptr;
	}
}

bool Button::Draw()
{
	App->render->BlitUI(texture, position.x, position.y, 2, &rect, false, 0.0f);
	return true;
}

bool Button::SetLabel(const iPoint position, const String text, _TTF_Font* font, const SDL_Color color)
{
	if (label != nullptr)
	{
		App->ui->DeleteObject(label);
		label = nullptr;
	}

	label = App->ui->CreateLabel(position, text, font, this, color);
	label->SetAnchor(this);

	return true;
}


