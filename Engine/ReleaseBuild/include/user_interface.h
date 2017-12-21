#ifndef __USER_INTERFACE_H__
#define __USER_INTERFACE_H__

#define MAX_UI_ELEMENTS 5

typedef struct ui_element_S
{
	enum{start, influence_type, happiness_type, menu, fire} ui_type;
	int isClickable;
	int isActive;
	int value;
	int fd_value;
	int sd_value;
	int td_value;
	SDL_Rect bounding_box;

	Sprite* icon;
	Sprite* fd_sprite;
	Sprite* sd_sprite;
	Sprite* td_sprite;

	Vector2D icon_pos;
	Vector2D fd_pos;
	Vector2D sd_pos;
	Vector2D td_pos;
	Vector2D icon_scale;
	Vector2D digit_scale;

	void(*click)(struct ui_element_s *self);
	void(*update)(struct ui_element_S *self);
	void(*free)(struct ui_element_S *self);

}ui_element;

ui_element* new_ui_element(int type);

void clear_ui_element(ui_element* ui);

void update_ui_element(ui_element *ui, int num);

void update_clickable_ui(ui_element *ui);

void clicked_on_menu(ui_element* ui);

void clicked_on_start(ui_element* ui, int* var);

void update_ui();

void init_ui();

void resolve_ui(ui_element* ui, Building* target);

#endif

