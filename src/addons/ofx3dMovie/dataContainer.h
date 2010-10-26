/*
 *  dataContainer.h
 *  mesa_editor
 *
 *  Created by 2bit on 10/04/04.
 *  Copyright 2010 4nchor5 la6. All rights reserved.
 *
 */

#pragma once
#include <time.h>
#define POINTS_LENGTH (176 * 144)
#define containerSize sizeof(dataContainer)

class dataContainer {
public:
	double stamp;
	float depth[POINTS_LENGTH];
	unsigned char brightness[POINTS_LENGTH];
};