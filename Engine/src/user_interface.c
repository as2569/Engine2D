#include <SDL.h>
#include "gf2d_vector.h"
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "user_interface.h"
#include "def.h"

ui_element* happiness_ui;
ui_element* influence_ui;

ui_element* new_ui_element(int type)
{
	//Vector2D temp_vec = { 1, 1 };
	ui_element *ui = NULL;
	ui = (ui_element *)malloc(sizeof(ui_element));
	if (type == 0)
	{
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
	}
	if (type == 1)
	{
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
	}
	ui->update = update_ui_element;
	ui->free = clear_ui_element;
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
