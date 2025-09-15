#ifndef GLOBAL_DEF_H_
#define CONFIG_H_
#include "main.h"

#ifndef PI
#define PI 3.14159265358979
#endif
#define deg2rad(a) (PI * (a) / 180)
#define rad2deg(a) (180 * (a) / PI)
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

#define rad60 deg2rad(60)
#define SQRT3 1.73205080756887729353
#define deg2rad(a) (PI * (a) / 180)
#define rad2deg(a) (180 * (a) / PI)

#endif

