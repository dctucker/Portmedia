/*
 *  chord.h
 *  wxMedia
 *
 *  Created by D. Casey Tucker on 2010-07-08.
 *  Copyright 2010. All rights reserved.
 *
 */
#pragma once

typedef struct {
	int type;
	int root;
	int bass;
	int add1;
	int add2;
	int weight;
} chord;


/*
const char *note_name[12] = {
	"C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B"
};
const char *type_name[4] = {
	"",
	"m",
	"dim",
	"aug"
};
*/
