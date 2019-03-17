#include "Image.h"
#include "j1Render.h"
#include "Module_UI.h"
#include "j1App.h"

Image::Image(iPoint position, SDL_Rect draw_rect, SDL_Texture * texture, Gui_Listener* listener) : UI_Object(position, listener), rect(draw_rect)
{
	this->texture = texture;
}

bool Image::Draw()
{
	App->render->BlitUI( texture,  position.x, position.y, 3.0f, &rect, false, 0.0f);
	return true;
}

