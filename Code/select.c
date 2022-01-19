// Copyright 2021 - 2022: Limberea Gabriela 312CA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include "select.h"

int chk_select(char **params, image *img)
{
	if (atoi(params[0]) > img->width || atoi(params[0]) < 0)
		return ERROR;

	if (atoi(params[1]) > img->height || atoi(params[1]) < 0)
		return ERROR;

	if (atoi(params[2]) > img->width || atoi(params[0]) < 0)
		return ERROR;

	if (atoi(params[3]) > img->height || atoi(params[0]) < 0)
		return ERROR;

	if (atoi(params[0]) == atoi(params[2]) ||
		atoi(params[1]) == atoi(params[3]))
		return ERROR;

	return SUCCESS;
}

int chk_digits(char **params)
{
	for (int i = 0; i < PARAMETERS_MAX; i++) {
		if (params[i][0] == '-') {
			int j = 1;
			while (params[i][j]) {
				if (params[i][j] < '0' || params[i][j] > '9')
					return ERROR;
				j++;
			}
		} else {
			int j = 0;
			while (params[i][j]) {
				if (params[i][j] < '0' || params[i][j] > '9')
					return ERROR;
				j++;
			}
		}
	}

	return SUCCESS;
}

void select_area(status *img_status, char **params, int count,
				 image *img, selected_area *selected)
{
	if (img_status->load == NOT_LOADED) {
		printf(NO_IMG);
		return;
	}

	if (count < PARAMETERS_MAX) {
		printf(INVALID);
		return;
	}

	if (chk_digits(params) == ERROR) {
		printf(INVALID);
		return;
	}

	if (chk_select(params, img) == ERROR) {
		printf(SELECT_ZONE_FAIL);
		return;
	}

	selected->x1 = atoi(params[0]);
	selected->y1 = atoi(params[1]);
	selected->x2 = atoi(params[2]);
	selected->y2 = atoi(params[3]);

	if (selected->x1 > selected->x2)
		swap_int(&selected->x1, &selected->x2);

	if (selected->y1 > selected->y2)
		swap_int(&selected->y1, &selected->y2);

	if (selected->x1 == 0 && selected->x2 == img->width &&
		selected->y1 == 0 && selected->y2 == img->height)
		img_status->selection = SELECTED_ALL;
	else
		img_status->selection = SELECTED;

	printf(SELECT_ZONE_OK, selected->x1, selected->y1,
		   selected->x2, selected->y2);
}

void select_all(status *img_status, image *img, selected_area *selected)
{
	if (img_status->load == NOT_LOADED) {
		printf(NO_IMG);
		return;
	}

	selected->x1 = 0;
	selected->y1 = 0;
	selected->x2 = img->width;
	selected->y2 = img->height;
	img_status->selection = SELECTED_ALL;
	printf(SELECT_ALL_OK);
}
