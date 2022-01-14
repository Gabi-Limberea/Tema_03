// Copyright 2021 - 2022: Limberea Gabriela 312CA
#pragma once

#include "macros.h"
#include "outputs.h"

void free_channel(uint_fast8_t **matrix, int height);
uint_fast8_t **alloc_channel(int width, int height);
void reset(image *img, status *img_status);
void swap_values(int *a, int *b);