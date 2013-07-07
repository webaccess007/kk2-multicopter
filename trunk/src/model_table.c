/*
 * model_table.c
 *
 * Created: 30.07.2012 12:39:29
 *  Author: OliverS
 *
 * $Id$
 *
 * --> this file is automatically generated. Do not modify.
 */

// number of models:   22
// number of channels: 123

#include "global.h"
#include "mixer.h"
#include <avr/pgmspace.h>

#ifndef P_STR
#define P_STR static const char PROGMEM
#endif

// ------------ model names
P_STR strModel01[] = "SingleCopter 2M 2S";
P_STR strModel02[] = "SingleCopter 1M 4S";
P_STR strModel03[] = "DualCopter";
P_STR strModel04[] = "TriCopter";
P_STR strModel05[] = "Y6";
P_STR strModel06[] = "QuadroCopter +";
P_STR strModel07[] = "QuadroCopter x";
P_STR strModel08[] = "X8 +";
P_STR strModel09[] = "X8 x";
P_STR strModel10[] = "HexaCopter +";
P_STR strModel11[] = "HexaCopter x";
P_STR strModel12[] = "OctoCopter +";
P_STR strModel13[] = "OctoCopter x";
P_STR strModel14[] = "H6";
P_STR strModel15[] = "H8";
P_STR strModel16[] = "V6";
P_STR strModel17[] = "V8";
P_STR strModel18[] = "Airplane 1S Ail";
P_STR strModel19[] = "Airplane 2S Ail";
P_STR strModel20[] = "Flying Wing";
P_STR strModel21[] = "Y4";
P_STR strModel22[] = "V-Tail";


// ------------ model definitions
// SingleCopter 2M 2S
static const model_t model01 PROGMEM =
{
    strModel01, 4,
    {
        //  thr   ail   ele   rud  offs  flags
        {{  100,    0,    0,  100,    0, FLAG_ESC }},
        {{  100,    0,    0, -100,    0, FLAG_ESC }},
        {{    0,  100,    0,    0,    0, FLAG_SERVO }},
        {{    0,    0,  100,    0,    0, FLAG_SERVO }},
    }
};

// SingleCopter 1M 4S
static const model_t model02 PROGMEM =
{
    strModel02, 5,
    {
        //  thr   ail   ele   rud  offs  flags
        {{  100,    0,    0,    0,    0, FLAG_ESC }},
        {{    0,  100,    0,  100,    0, FLAG_SERVO }},
        {{    0,    0,  100,  100,    0, FLAG_SERVO }},
        {{    0, -100,    0,  100,    0, FLAG_SERVO }},
        {{    0,    0, -100,  100,    0, FLAG_SERVO }},
    }
};

// DualCopter
static const model_t model03 PROGMEM =
{
    strModel03, 4,
    {
        //  thr   ail   ele   rud  offs  flags
        {{  100,  100,    0,    0,    0, FLAG_ESC }},
        {{  100, -100,    0,    0,    0, FLAG_ESC }},
        {{    0,    0,  100,  100,    0, FLAG_SERVO }},
        {{    0,    0, -100,  100,    0, FLAG_SERVO }},
    }
};

// TriCopter
static const model_t model04 PROGMEM =
{
    strModel04, 4,
    {
        //  thr   ail   ele   rud  offs  flags
        {{  100,  -87,   50,    0,    0, FLAG_ESC }},
        {{  100,   87,   50,    0,    0, FLAG_ESC }},
        {{  100,    0, -100,    0,    0, FLAG_ESC }},
        {{    0,    0,    0,  100,    0, FLAG_SERVO }},
    }
};

// Y6
static const model_t model05 PROGMEM =
{
    strModel05, 6,
    {
        //  thr   ail   ele   rud  offs  flags
        {{  100,  -87,   50,  100,    0, FLAG_ESC }},
        {{  100,  -87,   50, -100,    0, FLAG_ESC }},
        {{  100,   87,   50,  100,    0, FLAG_ESC }},
        {{  100,   87,   50, -100,    0, FLAG_ESC }},
        {{  100,    0, -100,  100,    0, FLAG_ESC }},
        {{  100,    0, -100, -100,    0, FLAG_ESC }},
    }
};

// QuadroCopter +
static const model_t model06 PROGMEM =
{
    strModel06, 4,
    {
        //  thr   ail   ele   rud  offs  flags
        {{  100,    0,  100,  100,    0, FLAG_ESC }},
        {{  100,  100,    0, -100,    0, FLAG_ESC }},
        {{  100,    0, -100,  100,    0, FLAG_ESC }},
        {{  100, -100,    0, -100,    0, FLAG_ESC }},
    }
};

// QuadroCopter x
static const model_t model07 PROGMEM =
{
    strModel07, 4,
    {
        //  thr   ail   ele   rud  offs  flags
        {{  100,  -71,   71,  100,    0, FLAG_ESC }},
        {{  100,   71,   71, -100,    0, FLAG_ESC }},
        {{  100,   71,  -71,  100,    0, FLAG_ESC }},
        {{  100,  -71,  -71, -100,    0, FLAG_ESC }},
    }
};

// X8 +
static const model_t model08 PROGMEM =
{
    strModel08, 8,
    {
        //  thr   ail   ele   rud  offs  flags
        {{  100,    0,  100,  100,    0, FLAG_ESC }},
        {{  100,    0,  100, -100,    0, FLAG_ESC }},
        {{  100,  100,    0,  100,    0, FLAG_ESC }},
        {{  100,  100,    0, -100,    0, FLAG_ESC }},
        {{  100,    0, -100,  100,    0, FLAG_ESC }},
        {{  100,    0, -100, -100,    0, FLAG_ESC }},
        {{  100, -100,    0,  100,    0, FLAG_ESC }},
        {{  100, -100,    0, -100,    0, FLAG_ESC }},
    }
};

// X8 x
static const model_t model09 PROGMEM =
{
    strModel09, 8,
    {
        //  thr   ail   ele   rud  offs  flags
        {{  100,  -71,   71,  100,    0, FLAG_ESC }},
        {{  100,  -71,   71, -100,    0, FLAG_ESC }},
        {{  100,   71,   71,  100,    0, FLAG_ESC }},
        {{  100,   71,   71, -100,    0, FLAG_ESC }},
        {{  100,   71,  -71,  100,    0, FLAG_ESC }},
        {{  100,   71,  -71, -100,    0, FLAG_ESC }},
        {{  100,  -71,  -71,  100,    0, FLAG_ESC }},
        {{  100,  -71,  -71, -100,    0, FLAG_ESC }},
    }
};

// HexaCopter +
static const model_t model10 PROGMEM =
{
    strModel10, 6,
    {
        //  thr   ail   ele   rud  offs  flags
        {{  100,    0,  100,  100,    0, FLAG_ESC }},
        {{  100,   87,   50, -100,    0, FLAG_ESC }},
        {{  100,   87,  -50,  100,    0, FLAG_ESC }},
        {{  100,    0, -100, -100,    0, FLAG_ESC }},
        {{  100,  -87,  -50,  100,    0, FLAG_ESC }},
        {{  100,  -87,   50, -100,    0, FLAG_ESC }},
    }
};

// HexaCopter x
static const model_t model11 PROGMEM =
{
    strModel11, 6,
    {
        //  thr   ail   ele   rud  offs  flags
        {{  100,   50,   87,  100,    0, FLAG_ESC }},
        {{  100,  100,    0, -100,    0, FLAG_ESC }},
        {{  100,   50,  -87,  100,    0, FLAG_ESC }},
        {{  100,  -50,  -87, -100,    0, FLAG_ESC }},
        {{  100, -100,    0,  100,    0, FLAG_ESC }},
        {{  100,  -50,   87, -100,    0, FLAG_ESC }},
    }
};

// OctoCopter +
static const model_t model12 PROGMEM =
{
    strModel12, 8,
    {
        //  thr   ail   ele   rud  offs  flags
        {{  100,    0,  100,  100,    0, FLAG_ESC }},
        {{  100,   71,   71, -100,    0, FLAG_ESC }},
        {{  100,  100,    0,  100,    0, FLAG_ESC }},
        {{  100,   71,  -71, -100,    0, FLAG_ESC }},
        {{  100,    0, -100,  100,    0, FLAG_ESC }},
        {{  100,  -71,  -71, -100,    0, FLAG_ESC }},
        {{  100, -100,    0,  100,    0, FLAG_ESC }},
        {{  100,  -71,   71, -100,    0, FLAG_ESC }},
    }
};

// OctoCopter x
static const model_t model13 PROGMEM =
{
    strModel13, 8,
    {
        //  thr   ail   ele   rud  offs  flags
        {{  100,   38,   92,  100,    0, FLAG_ESC }},
        {{  100,   92,   38, -100,    0, FLAG_ESC }},
        {{  100,   92,  -38,  100,    0, FLAG_ESC }},
        {{  100,   38,  -92, -100,    0, FLAG_ESC }},
        {{  100,  -38,  -92,  100,    0, FLAG_ESC }},
        {{  100,  -92,  -38, -100,    0, FLAG_ESC }},
        {{  100,  -92,   38,  100,    0, FLAG_ESC }},
        {{  100,  -38,   92, -100,    0, FLAG_ESC }},
    }
};

// H6
static const model_t model14 PROGMEM =
{
    strModel14, 6,
    {
        //  thr   ail   ele   rud  offs  flags
        {{  100,   71,   71,  100,    0, FLAG_ESC }},
        {{  100,   71,    0, -100,    0, FLAG_ESC }},
        {{  100,   71,  -71,  100,    0, FLAG_ESC }},
        {{  100,  -71,  -71, -100,    0, FLAG_ESC }},
        {{  100,  -71,    0,  100,    0, FLAG_ESC }},
        {{  100,  -71,   71, -100,    0, FLAG_ESC }},
    }
};

// H8
static const model_t model15 PROGMEM =
{
    strModel15, 8,
    {
        //  thr   ail   ele   rud  offs  flags
        {{  100,   71,   71,  100,    0, FLAG_ESC }},
        {{  100,   71,   24, -100,    0, FLAG_ESC }},
        {{  100,   71,  -24,  100,    0, FLAG_ESC }},
        {{  100,   71,  -71, -100,    0, FLAG_ESC }},
        {{  100,  -71,  -71,  100,    0, FLAG_ESC }},
        {{  100,  -71,  -24, -100,    0, FLAG_ESC }},
        {{  100,  -71,   24,  100,    0, FLAG_ESC }},
        {{  100,  -71,   71, -100,    0, FLAG_ESC }},
    }
};

// V6
static const model_t model16 PROGMEM =
{
    strModel16, 6,
    {
        //  thr   ail   ele   rud  offs  flags
        {{  100,  100,   71,  100,    0, FLAG_ESC }},
        {{  100,   71,    0, -100,    0, FLAG_ESC }},
        {{  100,   42,  -71,  100,    0, FLAG_ESC }},
        {{  100,  -42,  -71, -100,    0, FLAG_ESC }},
        {{  100,  -71,    0,  100,    0, FLAG_ESC }},
        {{  100, -100,   71, -100,    0, FLAG_ESC }},
    }
};

// V8
static const model_t model17 PROGMEM =
{
    strModel17, 8,
    {
        //  thr   ail   ele   rud  offs  flags
        {{  100,  100,   71,  100,    0, FLAG_ESC }},
        {{  100,   81,   24, -100,    0, FLAG_ESC }},
        {{  100,   61,  -24,  100,    0, FLAG_ESC }},
        {{  100,   42,  -71, -100,    0, FLAG_ESC }},
        {{  100,  -42,  -71,  100,    0, FLAG_ESC }},
        {{  100,  -61,  -24, -100,    0, FLAG_ESC }},
        {{  100,  -81,   24,  100,    0, FLAG_ESC }},
        {{  100, -100,   71, -100,    0, FLAG_ESC }},
    }
};

// Airplane 1S Ail
static const model_t model18 PROGMEM =
{
    strModel18, 4,
    {
        //  thr   ail   ele   rud  offs  flags
        {{  100,    0,    0,    0,    0, FLAG_ESC }},
        {{    0,  100,    0,    0,    0, FLAG_SERVO }},
        {{    0,    0,  100,    0,    0, FLAG_SERVO }},
        {{    0,    0,    0,  100,    0, FLAG_SERVO }},
    }
};

// Airplane 2S Ail
static const model_t model19 PROGMEM =
{
    strModel19, 5,
    {
        //  thr   ail   ele   rud  offs  flags
        {{  100,    0,    0,    0,    0, FLAG_ESC }},
        {{    0,  100,    0,    0,    0, FLAG_SERVO }},
        {{    0,  100,    0,    0,    0, FLAG_SERVO }},
        {{    0,    0,  100,    0,    0, FLAG_SERVO }},
        {{    0,    0,    0,  100,    0, FLAG_SERVO }},
    }
};

// Flying Wing
static const model_t model20 PROGMEM =
{
    strModel20, 3,
    {
        //  thr   ail   ele   rud  offs  flags
        {{  100,    0,    0,    0,    0, FLAG_ESC }},
        {{    0,   50,   50,    0,    0, FLAG_SERVO }},
        {{    0,   50,  -50,    0,    0, FLAG_SERVO }},
    }
};

// Y4
static const model_t model21 PROGMEM =
{
    strModel21, 4,
    {
        //  thr   ail   ele   rud  offs  flags
        {{  100,  -71,   71,  100,    0, FLAG_ESC }},
        {{  100,   71,   71, -100,    0, FLAG_ESC }},
        {{  100,    0, -100,  100,    0, FLAG_ESC }},
        {{  100,    0, -100, -100,    0, FLAG_ESC }},
    }
};

// V-Tail
static const model_t model22 PROGMEM =
{
    strModel22, 4,
    {
        //  thr   ail   ele   rud  offs  flags
        {{  100,  -71,   71,    0,    0, FLAG_ESC }},
        {{  100,   71,   71,    0,    0, FLAG_ESC }},
        {{  120,    0,  -90,  100,    0, FLAG_ESC }},
        {{  120,    0,  -90, -100,    0, FLAG_ESC }},
    }
};


// ------------ model table
const model_t* const modelTable[] PROGMEM = {
    &model01,
    &model02,
    &model03,
    &model04,
    &model05,
    &model06,
    &model07,
    &model08,
    &model09,
    &model10,
    &model11,
    &model12,
    &model13,
    &model14,
    &model15,
    &model16,
    &model17,
    &model18,
    &model19,
    &model20,
    &model21,
    &model22,
};
