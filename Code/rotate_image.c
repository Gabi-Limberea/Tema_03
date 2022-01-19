// Copyright 2021 - 2022: Limberea Gabriela 312CA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "rotate_image.h"

int exe_rotation_bw(uint_fast8_t ***matrix, int angle_val,
					selected_area selected)
{
	int side = selected.x2 - selected.x1;
	int rotations = angle_val / DEFAULT_ANGLE;

	if (angle_val >= 0) {
		while (rotations) {
			uint_fast8_t **copy = alloc_bw(side, side);

			if (!copy) {
				free_channel_bw(copy, side);
				return ERROR;
			}

			for (int i = 0; i < side; i++)
				for (int j = 0; j < side; j++)
					if (is_bit_set(*matrix, i + selected.y1, j + selected.x1))
						set_bit(1, copy, i, j);

			for (int i = 0; i < side; i++)
				for (int j = 0; j < side; j++) {
					reset_bit(*matrix, i + selected.y1, j + selected.x1);
					if (is_bit_set(copy, side - j - 1, i))
						set_bit(1, *matrix, i + selected.y1, j + selected.x1);
				}

			rotations--;
			free_channel_bw(copy, side);
		}
	} else {
		while (rotations) {
			uint_fast8_t **copy = alloc_bw(side, side);

			if (!copy) {
				free_channel_bw(copy, side);
				return ERROR;
			}

			for (int i = 0; i < side; i++)
				for (int j = 0; j < side; j++)
					if (is_bit_set(*matrix, i + selected.y1, j + selected.x1))
						set_bit(1, copy, i, j);

			for (int i = 0; i < side; i++)
				for (int j = 0; j < side; j++) {
					reset_bit(*matrix, i + selected.y1, selected.x1);
					if (is_bit_set(copy, j, side - i - 1))
						set_bit(1, *matrix, i + selected.y1, j + selected.x1);
				}

			rotations++;
			free_channel_bw(copy, side);
		}
	}

	return SUCCESS;
}

int exe_rotation(double ***matrix, int angle_val, selected_area selected)
{
	int side = selected.x2 - selected.x1;
	int rotations = angle_val / DEFAULT_ANGLE;

	if (angle_val >= 0) {
		while (rotations) {
			double **copy = alloc(side, side);

			if (!copy) {
				free_channel(copy, side);
				return ERROR;
			}

			for (int i = 0; i < side; i++)
				for (int j = 0; j < side; j++)
					copy[i][j] = (*matrix)[i + selected.y1][j + selected.x1];

			for (int i = 0; i < side; i++)
				for (int j = 0; j < side; j++)
					swap(&(*matrix)[i + selected.y1][j + selected.x1],
						 &copy[side - j - 1][i]);

			rotations--;
			free_channel(copy, side);
		}
	} else {
		while (rotations) {
			double **copy = alloc(side, side);

			if (!copy) {
				free_channel(copy, side);
				return ERROR;
			}

			for (int i = 0; i < side; i++)
				for (int j = 0; j < side; j++)
					copy[i][j] = (*matrix)[i + selected.y1][j + selected.x1];

			for (int i = 0; i < side; i++)
				for (int j = 0; j < side; j++)
					swap(&(*matrix)[i + selected.y1][j + selected.x1],
						 &copy[j][side - i - 1]);

			rotations++;
			free_channel(copy, side);
		}
	}

	return SUCCESS;
}

int rotate_select(image *img, int angle_val, selected_area selected)
{
	if (selected.x2 - selected.x1 != selected.y2 - selected.y1)
		return ERROR;

	switch (img->type) {
	case P1:
	case P4:
		if (exe_rotation_bw(&img->pixels.bw, angle_val, selected) == ERROR)
			return ERROR;
		break;
	case P2:
	case P5:
		if (exe_rotation(&img->pixels.gray, angle_val, selected) == ERROR)
			return ERROR;
		break;
	case P3:
	case P6:
		if (exe_rotation(&img->pixels.red, angle_val, selected) == ERROR ||
			exe_rotation(&img->pixels.green, angle_val, selected) == ERROR ||
			exe_rotation(&img->pixels.blue, angle_val, selected) == ERROR)
			return ERROR;
		break;
	default:
		return ERROR;
	}

	return SUCCESS;
}

int rotate_all(image *img, int angle_val)
{
	int old_width = img->width, old_height = img->height;
	switch (img->type) {
	case P1:
	case P4:
		if (exe_rotation_all_bw(&img->pixels.bw, angle_val,
								&img->width, &img->height) == ERROR)
			return ERROR;
		break;
	case P2:
	case P5:
		if (exe_rotation_all(&img->pixels.gray, angle_val,
							 &img->width, &img->height) == ERROR)
			return ERROR;
		break;
	case P3:
	case P6:
		if (exe_rotation_all(&img->pixels.red, angle_val,
							 &img->width, &img->height) == ERROR)
			return ERROR;

		img->height = old_height;
		img->width = old_width;

		if (exe_rotation_all(&img->pixels.green, angle_val,
							 &img->width, &img->height) == ERROR)
			return ERROR;

		img->height = old_height;
		img->width = old_width;

		if (exe_rotation_all(&img->pixels.blue, angle_val,
							 &img->width, &img->height) == ERROR)
			return ERROR;
		break;
	default:
		return ERROR;
	}

	return SUCCESS;
}

int exe_rotation_all(double ***matrix, int angle_val,
					 int *width, int *height)
{
	int rotations = angle_val / DEFAULT_ANGLE;
	if (rotations >= 0) {
		while (rotations) {
			double **copy = *matrix;
			int width_new = *height, height_new = *width;

			*matrix = alloc(width_new, height_new);
			if (!(*matrix))
				return ERROR;

			for (int i = 0; i < height_new; i++)
				for (int j = 0; j < width_new; j++)
					(*matrix)[i][j] = copy[*height - j - 1][i];

			free_channel(copy, *height);
			rotations--;
			*width = width_new;
			*height = height_new;
		}
	} else {
		while (rotations) {
			double **copy = *matrix;
			int width_new = *height, height_new = *width;

			*matrix = alloc(width_new, height_new);
			if (!(*matrix))
				return ERROR;

			for (int i = 0; i < height_new; i++)
				for (int j = 0; j < width_new; j++)
					(*matrix)[i][j] = copy[j][*width - i - 1];

			free_channel(copy, *height);
			rotations++;
			*width = width_new;
			*height = height_new;
		}
	}
	return SUCCESS;
}

int exe_rotation_all_bw(uint_fast8_t ***matrix, int angle_val,
						int *width, int *height)
{
	int rotations = angle_val / DEFAULT_ANGLE;

	if (rotations >= 0) {
		while (rotations) {
			uint_fast8_t **copy = *matrix;
			int width_new = *height, height_new = *width;

			*matrix = alloc_bw(width_new, height_new);
			if (!(*matrix))
				return ERROR;

			for (int i = 0; i < height_new; i++) {
				for (int j = 0; j < width_new; j++) {
					if (is_bit_set(copy, *height - j - 1, i))
						set_bit(1, *matrix, i, j);
				}
			}

			free_channel_bw(copy, *height);
			rotations--;
			*width = width_new;
			*height = height_new;
		}
	} else {
		while (rotations) {
			uint_fast8_t **copy = *matrix;
			int width_new = *height, height_new = *width;

			*matrix = alloc_bw(width_new, height_new);
			if (!(*matrix))
				return ERROR;

			for (int i = 0; i < height_new; i++) {
				for (int j = 0; j < width_new; j++) {
					if (is_bit_set(copy, j, *width - i - 1))
						set_bit(1, *matrix, i, j);
				}
			}

			free_channel_bw(copy, *height);
			rotations++;
			*width = width_new;
			*height = height_new;
		}
	}
	return SUCCESS;
}

void rotate(status *img_status, char *angle, image *img,
			selected_area *selected, int count)
{
	if (img_status->load == NOT_LOADED) {
		printf(NO_IMG);
		return;
	}

	if (count == 0) {
		printf(INVALID);
		return;
	}

	int angle_val = atoi(angle);
	if (angle_val % DEFAULT_ANGLE != 0 || angle_val > DEFAULT_ANGLE * 4 ||
		angle_val < DEFAULT_ANGLE * (-4)) {
		printf(ROTATE_FAIL_ANGlE);
		return;
	}

	if (img_status->selection == SELECTED_ALL) {
		if (rotate_all(img, angle_val) == SUCCESS) {
			printf(ROTATE_OK, angle);
			selected->x2 = img->width;
			selected->y2 = img->height;
		} else {
			fprintf(stderr, ERROR_MESSAGE);
		}
		return;
	}

	if (selected->x2 - selected->x1 == selected->y2 - selected->y1) {
		if (rotate_select(img, angle_val, *selected) == SUCCESS)
			printf(ROTATE_OK, angle);
		return;
	}
	printf(ROTATE_FAIL_SQUARE);
}
