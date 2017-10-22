#ifndef __USER_INTERFACE_H__
#define __USER_INTERFACE_H__

typedef struct ui_element_S
{
	enum{influence_type, happiness_type} ui_type;
	int value;
	int fd_value;
	int sd_value;
	int td_value;

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

	void(*update)(struct ui_element_S *self);
	void(*free)(struct ui_element_S *self);

}ui_element;

ui_element* new_ui_element(int type);

void clear_ui_element(ui_element*);

void update_ui_element(ui_element *ui, int num);

#endif

