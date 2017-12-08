#include <SDL.h>
#include "gf2d_vector.h"
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "buildings.h"
#include "user_interface.h"
#include "def.h"

extern ui_element* uiList[MAX_UI_ELEMENTS];
extern int influence;
extern int happiness_avg;

int menuOpen = 1;

//ui element
ui_element *influence_ui;
ui_element *happiness_ui;
ui_element *menu_button;
ui_element *fire_button;

ui_element* new_ui_element(int type)
{
	ui_element *ui = NULL;
	ui = (ui_element *)malloc(sizeof(ui_element));
	if (type == 0)
	{
		ui->ui_type = type;
		ui->isClickable = 0;
		ui->icon = gf2d_sprite_load_all("images/happiness.png", 512, 512, 1);
		ui->fd_sprite = gf2d_sprite_load_all("images/digits.png", 32, 64, 10);
		ui->sd_sprite = gf2d_sprite_load_all("images/digits.png", 32, 64, 10);
		ui->td_sprite = gf2d_sprite_load_all("images/digits.png", 32, 64, 10);

		ui->icon_pos.x = (WINDOW_WIDTH - 128 - 50);
		ui->icon_pos.y = 10;
		ui->fd_pos.x = (WINDOW_WIDTH - 64 - 50);
		ui->fd_pos.y = 10;
		ui->sd_pos.x = (WINDOW_WIDTH - 32 - 50);
		ui->sd_pos.y = 10;
		ui->td_pos.x = (WINDOW_WIDTH - 50);
		ui->td_pos.y = 10;

		ui->icon_scale.x = 0.125;
		ui->icon_scale.y = 0.125;
		ui->digit_scale.x = 1;
		ui->digit_scale.y = 1;
		ui->update = update_ui_element;
	}
	if (type == 1)
	{
		ui->ui_type = type;
		ui->isClickable = 0;
		ui->icon = gf2d_sprite_load_all("images/dollar.png", 512, 512, 1);
		ui->fd_sprite = gf2d_sprite_load_all("images/digits.png", 32, 64, 10);
		ui->sd_sprite = gf2d_sprite_load_all("images/digits.png", 32, 64, 10);
		ui->td_sprite = gf2d_sprite_load_all("images/digits.png", 32, 64, 10);

		ui->icon_pos.x = (WINDOW_WIDTH - 128 - 50);
		ui->icon_pos.y = 80;
		ui->fd_pos.x = (WINDOW_WIDTH - 64 - 50);
		ui->fd_pos.y = 80;
		ui->sd_pos.x = (WINDOW_WIDTH - 32 - 50);
		ui->sd_pos.y = 80;
		ui->td_pos.x = (WINDOW_WIDTH - 50);
		ui->td_pos.y = 80;

		ui->icon_scale.x = 0.125;
		ui->icon_scale.y = 0.125;
		ui->digit_scale.x = 1;
		ui->digit_scale.y = 1;
		ui->update = update_ui_element;
	}
	if (type == 2)
	{
		ui->ui_type = type;
		ui->isClickable = 1;
		ui->icon = gf2d_sprite_load_all("images/menu.png", 512, 512, 1);
		ui->icon_pos.x = 20; 
		ui->icon_pos.y = 20;
		ui->icon_scale.x = 0.125;
		ui->icon_scale.y = 0.125;
		ui->bounding_box.h = 512 * ui->icon_scale.x;
		ui->bounding_box.w = 512 * ui->icon_scale.y;
		ui->bounding_box.x = ui->icon_pos.x;
		ui->bounding_box.y = ui->icon_pos.y;
		ui->update = update_clickable_ui;
		ui->click = clicked_on_menu;
	}
	if (type == 3)
	{
		ui->ui_type = type;
		ui->isClickable = 1;
		ui->icon = gf2d_sprite_load_all("images/fire.png", 512, 512, 1);
		ui->icon_pos.x = 99; //menu 20 + 64 width + 15 gap
		ui->icon_pos.y = 20;
		ui->icon_scale.x = 0.125;
		ui->icon_scale.y = 0.125;
		ui->bounding_box.h = 512 * ui->icon_scale.x;
		ui->bounding_box.w = 512 * ui->icon_scale.y;
		ui->bounding_box.x = ui->icon_pos.x;
		ui->bounding_box.y = ui->icon_pos.y;
		ui->update = update_clickable_ui;
		ui->click = resolve_ui;
	}
	ui->free = clear_ui_element;
	ui->isActive = 1;
	return ui;
}

void clear_ui_element(ui_element* ui)
{
	memset(&ui, 0, sizeof(ui_element));
}

void update_ui_element(ui_element *ui, int num)
{
	ui->td_value = num % 10;
	ui->fd_value = num / 100;
	ui->sd_value = (num - ui->td_value - (ui->fd_value * 100)) / 10;

	gf2d_sprite_draw(ui->icon, ui->icon_pos, &ui->icon_scale, NULL, NULL, NULL, NULL, 0);
	gf2d_sprite_draw(ui->fd_sprite, ui->fd_pos, &ui->digit_scale, NULL, NULL, NULL, NULL, ui->fd_value);
	gf2d_sprite_draw(ui->sd_sprite, ui->sd_pos, &ui->digit_scale, NULL, NULL, NULL, NULL, ui->sd_value);
	gf2d_sprite_draw(ui->td_sprite, ui->td_pos, &ui->digit_scale, NULL, NULL, NULL, NULL, ui->td_value);
}

void clicked_on_menu()
{
	menuOpen = !menuOpen;
	if (menuOpen == 0)
	{
		uiList[3]->isActive = 0;
	}
	else
	{
		uiList[3]->isActive = 1;
	}
}

void resolve_ui(ui_element* ui, Building* target)
{
	slog("resolve1");
	if(target != NULL)
	{
		slog("resolve2 %i", target->buildingType);
		if (target->buildingType == EMERGENCY)
		{
			slog("resolve3");
			resolve_emergency(target);
			target == NULL;
		}
	}
}

void update_clickable_ui(ui_element *ui)
{
	if (ui->isActive == 0)
	{
		return;
	}

	gf2d_sprite_draw(ui->icon, ui->icon_pos, &ui->icon_scale, NULL, NULL, NULL, NULL, 0);
	Vector4D col = { 255, 0, 255, 255 };
	//gf2d_draw_rect(ui->bounding_box, col);
}

void init_ui()
{
	happiness_ui = new_ui_element(0);
	influence_ui = new_ui_element(1);
	menu_button = new_ui_element(2);
	fire_button = new_ui_element(3);

	uiList[0] = happiness_ui;
	uiList[1] = influence_ui;
	uiList[2] = menu_button;
	uiList[3] = fire_button;
}

void update_ui()
{
	(uiList[0])->update(happiness_ui, happiness_avg);
	(uiList[1])->update(influence_ui, influence);
	(uiList[2])->update(menu_button);
	(uiList[3])->update(fire_button);
}
