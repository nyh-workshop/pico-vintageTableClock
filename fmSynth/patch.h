#ifndef _PATCH_H
#define _PATCH_H

#include "fmSynth.h"

// OpSettings' variables appear in float data type to facilitate reading!
// These are converted into their respective fixed points data type in the Envelope later! 
struct opSettings {
	// Levels:
	float L0;
	float L1;
	float L3;
	// Rates:
	float R0;
	float R1;
	float R3;
	// Sustain time:
	float sustainInSecs;
	// Ratio:
	float ratio;
};

struct fmPatch {
	//std::string name;
	const char* name;
	uint8_t algorithm;
	uint8_t feedback;
	opSettings ops[4];
};

// p00 is test patch, use it for only testing! :D
extern const struct fmPatch p00;
extern const struct fmPatch p02;
extern const struct fmPatch p04;
extern const struct fmPatch p05;
extern const struct fmPatch p07;
extern const struct fmPatch p07_1;
extern const struct fmPatch p09;
extern const struct fmPatch p13;
extern const struct fmPatch p16;
extern const struct fmPatch p19;
extern const struct fmPatch p23;
extern const struct fmPatch p27;

extern const struct fmPatch fmPatchList[32];

#endif
