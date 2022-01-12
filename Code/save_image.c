// Copyright 2021 - 2022: Limberea Gabriela 312CA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "save_image.h"

int save_ascii(char *filename, image img)
{
	FILE *file = fopen(filename, "w");

	if (!file)
		return ERROR;

	magic tmp_type = img.type;
	if (tmp_type > P3)
		tmp_type -= 3;

	fprintf(file, "P%d\n", tmp_type + 1);
	fprintf(file, "%d %d\n", img.width, img.height);
	if (tmp_type > P1)
		fprintf(file, "%d\n", img.max_value);

	for (int i = 0; i < img.height; i++) {
		for (int j = 0; j < img.width; j++) {
			switch (tmp_type) {
			case P1:
				if (is_bit_set(img.pixels.bw, i, j))
					fprintf(file, "%u ", 1);
				else
					fprintf(file, "%u ", 0);
				break;
			case P2:
				fprintf(file, "%u ", img.pixels.gray[i][j]);
				break;
			case P3:
				fprintf(file, "%u ", img.pixels.red[i][j]);
				fprintf(file, "%u ", img.pixels.green[i][j]);
				fprintf(file, "%u ", img.pixels.blue[i][j]);
				break;
			default:
				return ERROR;
				break;
			}
		}
		fprintf(file, "\n");
	}

	fclose(file);
	return SAVE;
}

int save_binary(char *filename, image img)
{
	FILE *file = fopen(filename, "wb");

	if (!file)
		return ERROR;

	magic tmp_type = img.type;
	if (tmp_type <= P3)
		tmp_type += 3;

	fprintf(file, "P%d\n", tmp_type + 1);
	fprintf(file, "%d %d\n", img.width, img.height);
	if (tmp_type > P4)
		fprintf(file, "%d\n", img.max_value);

	for (int i = 0; i < img.height; i++) {
		for (int j = 0; j < img.width; j++) {
		switch (tmp_type) {
		case P4:
			if (j % 8 == 0) {
				uint_fast8_t tmp;
				set_bit_reversed(img.pixels.bw[i][j / 8], &tmp);
				fwrite(&tmp, SIZE, 1, file);
			}
			break;
		case P5:
			fwrite(&img.pixels.gray[i][j], SIZE, 1, file);
			break;
		case P6:
			fwrite(&img.pixels.red[i][j], SIZE, 1, file);
			fwrite(&img.pixels.green[i][j], SIZE, 1, file);
			fwrite(&img.pixels.blue[i][j], SIZE, 1, file);
			break;
		default:
			return ERROR;
			break;
			}
		}
	}

	fclose(file);
	return SAVE;
}

void save_img(status img_status, char **params, image img, int count)
{
	if (img_status.load == NOT_LOADED) {
		printf(NO_IMG);
		return;
	}

	int saved = 0;

	if (count == 2) {
		if (!strcmp(params[1], "ascii"))
			saved = save_ascii(params[0], img);
	} else {
		saved = save_binary(params[0], img);
	}

	if (saved == SAVE)
		printf(SAVE_OK, params[0]);
}
