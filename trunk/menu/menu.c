/*
 * menu.c
 *
 * Created: 30.07.2012 18:52:51
 *  Author: OliverS
 *
 * $Id$
 */ 

#include "global.h"
#include "lcd.h"
#include "keyboard.h"
#include "sensors.h"
#include "rx.h"
#include "mixer.h"
#include "buzzer.h"
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <avr/wdt.h>
#include <string.h>

uint8_t _mykey;
#define KEY_INIT	1
#define KEYINIT		(_mykey & KEY_INIT)
#define KEY1		(_mykey & KEY_1)
#define KEY2		(_mykey & KEY_2)
#define KEY3		(_mykey & KEY_3)
#define KEY4		(_mykey & KEY_4)
#define ANYKEY		(_mykey)
#define KEYPRESS	(_mykey & (KEY_1|KEY_2|KEY_3|KEY_4))
#define NOKEYRETURN {if (!_mykey) return;}

//typedef const prog_char screen_t[7][22];
// typedef struct  
// {
// 	uint8_t posX, posY;
// 	const prog_char *text;
// } PROGMEM element_t;

typedef struct
{
	uint8_t X, Y;
	void *valuePtr;
	int16_t loLimit, hiLimit;
	uint8_t len;
} edit_element_t;

typedef void (pageHandler)(void);

typedef struct
{
	const char *softkeys;
	pageHandler *handler;
	const char *screen;
} page_t;

typedef struct
{
	uint8_t len;
	PGM_P (*textSelector)(uint8_t);
	uint8_t top;
	uint8_t marked;
} menu_t;


//////////////////////////////////////////////////////////////////////////
#define P_STR static const prog_char
#include "menu_text.h"
#include "menu_screen.h"

//////////////////////////////////////////////////////////////////////////
static void _hStart();
static void _hMenu();
static void _hReceiverTest();
static void _hSensorTest();
static void _hSensorCalibration();
static void _hESCCalibration();
static void _hStickCentering();
static void _hStickScaling();
static void _hShowModelLayout();
static void _hLoadModelLayout();
static void _hDebug();
static void _hFactoryReset();
static void _hPIEditor();
static void _hMiscSettings();
static void _hSelflevelSettings();
static void _hCPPMSettings();
static void _hModeSettings();

//////////////////////////////////////////////////////////////////////////
// softkeys
static const prog_char _skSTART[]     = "                 MENU";
static const prog_char _skMENU[]      = "BACK  UP   DOWN ENTER";
static const prog_char _skBACK[]      = "BACK";
static const prog_char _skCONTINUE[]  = "BACK         CONTINUE";
static const prog_char _skCANCELYES[] = "CANCEL            YES";
static const prog_char _skPAGE[]      = "BACK PREV NEXT CHANGE";
static const prog_char _skEDIT[]      = "CLR  DOWN   UP   DONE";
static const prog_char _skBACKNEXT[]  = "BACK  NEXT";
static const prog_char _skCANCEL[]    = "CANCEL";

//////////////////////////////////////////////////////////////////////////
static const page_t pages[] PROGMEM = {
/*  0 */	{ _skSTART, _hStart },
/*  1 */	{ _skMENU, _hMenu },
/*  2 */	{ _skPAGE, _hPIEditor, scrPIEditor},
/*  3 */	{ _skBACK, _hReceiverTest, scrReceiverTest},
/*  4 */	{ _skPAGE, _hModeSettings, scrModeSettings},
/*  5 */	{ _skPAGE, _hStickScaling, scrStickScaling},
/*  6 */	{ _skPAGE, _hMiscSettings, scrMiscSettings},
/*  7 */	{ _skPAGE, _hSelflevelSettings, scrSelflevelSettings},
/*  8 */	{ _skBACK, _hSensorTest, scrSensorTest},
/*  9 */	{ _skCONTINUE, _hSensorCalibration, scrSensorCal0},
/* 10 */	{ _skCONTINUE, _hESCCalibration, scrESCCal0},
/* 11 */	{ _skPAGE, _hCPPMSettings, scrCPPMSettings},
/* 12 */	{ _skCONTINUE, _hStickCentering},
/* 13 */	{ _skPAGE, NULL, scrMixerEditor},
/* 14 */	{ _skBACKNEXT, _hShowModelLayout},
/* 15 */	{ _skMENU, _hLoadModelLayout },
/* 16 */	{ _skBACK, _hDebug },
/* 16 */	{ _skCANCELYES, _hFactoryReset },
};

static const prog_char *lstMenu[] PROGMEM = {
	strPIEditor,
	strReceiverTest,
	strModeSettings,
	strStickScaling,
	strMiscSettings,
	strSelflevelSettings,
	strSensorTest,
	strSensorCalibration,
	strESCCalibration,
	strCPPMSettings,
	strRadioCalibration,
	strMixerEditor,
	strShowMotorLayout,
	strLoadMotorLayout,
	strDebug,
	strFactoryReset,
};

#define PAGE_START			0
#define PAGE_MENU			1
#define PAGE_SHOW_LAYOUT	14

static PGM_P tsmMain(uint8_t);
static PGM_P tsmLoadModelLayout(uint8_t);

static uint8_t page, subpage;
static uint16_t _tStart;
static page_t currentPage;
static menu_t mnuMain = {length(lstMenu), tsmMain};
static menu_t mnuMLayout = {MIXER_TABLE_LEN, tsmLoadModelLayout};
static int16_t editValue, editLoLimit, editHiLimit;
static uint8_t editMode;
static void* editValuePtr;

static void writeSpace(uint8_t len)
{
	for (uint8_t i = 0; i < len; i++)
		lcdWriteChar(32);
}

static void writePadded(char *s, uint8_t len)
{
	lcdWriteString(s);
	writeSpace(len - strlen(s));
}

static void writePadded_P(const char *s, uint8_t len)
{
	lcdWriteString_P(s);
	writeSpace(len - strlen_P(s));
}

static void writeSoftkeys(const char* sk)
{
	if (!sk)
		sk = currentPage.softkeys;
	if (sk)
	{
		lcdSetPos(7, 0);
		writePadded_P(sk, 21);
	}
}

static void writeString_P(uint8_t x, uint8_t y, PGM_P str, uint8_t len, uint8_t index)
{
	lcdReverse(index == subpage);
	lcdSetPos(x, y);
	writePadded_P(str, len);
	lcdReverse(0);
}

static void writeValue(uint8_t x, uint8_t y, int16_t value, uint8_t len, uint8_t index)
{
	char s[7];
	itoa(value, s, 10);
	lcdReverse(index == subpage);
	lcdSetPos(x, y);
	writePadded(s, len);
	lcdReverse(0);
}

static void loadPage(uint8_t pageIndex)
{
	memcpy_P(&currentPage, &pages[pageIndex], sizeof(currentPage));
	page = pageIndex;
}

static void pageKey(uint8_t num)
{
	if (KEY2)	// PREV
		subpage = subpage == 0 ? num - 1 : subpage - 1;
	else if (KEY3) // NEXT
		subpage = (subpage + 1) % num;
}

static void editModeHandler();
static void defaultHandler()
{
	if (editMode)		// edit mode?
		editModeHandler();
	else
	{
		if (KEYINIT)
		{
			lcdClear();
			if (currentPage.screen)
				lcdWriteString_P(currentPage.screen);
			writeSoftkeys(currentPage.softkeys);
			lcdSetPos(0, 0);
		}
		
		if (currentPage.handler)
			currentPage.handler();
		
	}
}

static void editModeHandler()
{
	if (KEY4)	// DONE;
	{
		editMode = 0;
		_mykey = KEY_INIT;
		*(uint8_t*)editValuePtr = editValue;
		configSave();
		lcdSelectFont(NULL);
		defaultHandler();
		return;
	}
	if (ANYKEY)
	{
		if (KEY2)	// DOWN?
		{
			if (editValue > editLoLimit)
				editValue--;
		}
		else if (KEY3)	// UP?
		{
			if (editValue < editHiLimit)
				editValue++;
		}
		else if (KEY1)	// CLR?
			editValue = 0;
			
		writeValue(2, 34, editValue, 5, -1);
	}
}

static void startEditMode(void* valuePtr, int16_t loLimit, int16_t hiLimit, uint8_t valueType)
{
	editMode = 1;
	_mykey = KEY_INIT;
	editValuePtr = valuePtr;
	editValue = *(uint8_t*)valuePtr;
	editLoLimit = loLimit;
	editHiLimit = hiLimit;
	
	lcdFillRectangle(30, 11, 98, 34, 0);
	lcdRectangle(30, 11, 98, 34, 1);
	lcdRectangle(31, 12, 97, 33, 1);
	writeSoftkeys(_skEDIT);
	lcdSelectFont(&font12x16);
	editModeHandler();
}

static uint8_t doMenu(menu_t *menu)
{
	if (!_mykey) return 0;
	
	// key handling
	if (KEY2)		// UP
	{
		if (menu->marked > 0) 
			menu->marked--;
	}
	else if (KEY3)		// DOWN
	{
		if (menu->marked < menu->len - 1) 
			menu->marked++;
	}
	else if (KEY4)		// ENTER
		return 1;

	if (menu->marked < menu->top)
		menu->top = menu->marked;
	else if (menu->marked - menu->top >= 5)
		menu->top = menu->marked - 4;
	
	// text output
	lcdSetPos(0, 58);
	if (menu->top > 0)
		lcdWriteGlyph_P(&glyArrowUp, 0);
		
	for (uint8_t i = 0; i < 5 && i < menu->len; i++)
	{
		lcdSetPos(i + 1, 0);
		PGM_P item = menu->textSelector(menu->top + i);
		lcdReverse(menu->top + i == menu->marked);
		writePadded_P(item, 21);
	}

	lcdReverse(0);
	lcdSetPos(6, 58);
	if (menu->top < menu->len - 5)
		lcdWriteGlyph_P(&glyArrowDown, 0);
	
	return 0;
}

void _hMenu()
{
	if (doMenu(&mnuMain))
		loadPage(mnuMain.marked + 2);
}

#define CENTER_X	96
#define CENTER_Y	32

static void showMotor(uint8_t motor, uint8_t withDir)
{
	uint8_t x, y;
	mixer_channel_t *channel = &Config.Mixer.Channel[motor];
	
	if (channel->flags & FLAG_ESC)
	{
		x = CENTER_X + (channel->Aileron >> 2);
		y = CENTER_Y - (channel->Elevator >> 2);
	
		lcdLine(x, y, CENTER_X, CENTER_Y);
		lcdXY(CENTER_X - 2, CENTER_Y - 2);
		lcdWriteGlyph_P(&glyFC, ROP_COPY);
		lcdXY(x - 4, y - 4);
		lcdWriteGlyph_P(&glyBall, ROP_PAINT);
		lcdXY(x - 4, y - 7);
		if (channel->Rudder >= 0)
			lcdWriteGlyph_P(&glyDirCW, ROP_PAINT);
		else
			lcdWriteGlyph_P(&glyDirCCW, ROP_PAINT);
		
		lcdXY(x - 2, y - 2);
		lcdReverse(1);
		lcdSelectFont(&font4x6);
		lcdWriteChar(motor + '1');
		lcdSelectFont(NULL);
		lcdReverse(0);
		
		if (withDir)
		{
			lcdSetPos(2, 0);
			lcdWriteString_P(strDirSeen);
			lcdSetPos(5, 0);
			if (channel->Rudder >= 0)
				lcdWriteString_P(strCW);
			else
				lcdWriteString_P(strCCW);
		}
	}
	else if (withDir)
	{
		lcdSetPos(3, 64);
		if (channel->flags == FLAG_NONE)
			lcdWriteString_P(strUnused);
		else
			lcdWriteString_P(strServo);
	}
}

static void _hShowModelLayout()
{
	if (ANYKEY)
	{
		if (KEY2)	// NEXT
			subpage = (subpage + 1) % 9;
		
		lcdClear();
		lcdWriteString_P(strOutput);
		lcdWriteChar(32);
		if (subpage == 0)
		{
			lcdWriteString_P(strALL);
			for (uint8_t i = 0; i < 8; i++)
				showMotor(i, 0);
		}			
		else
		{
			lcdWriteChar(subpage + '0');
			showMotor(subpage - 1, 1);
		}
		writeSoftkeys(NULL);
	}
}

static void _hLoadModelLayout()
{
	if (KEYINIT)
		mnuMLayout.marked = Config.MixerIndex;

	if (subpage == 0)
	{
		if (doMenu(&mnuMLayout))
		{
			lcdClear();
			lcdSetPos(3, 18);
			lcdWriteString_P(strAreYouSure);
			writeSoftkeys(_skCANCELYES);
			subpage = 1;
		}
	}		
	else if (KEY4)		// YES
	{
		mixerLoadTable(mnuMLayout.marked);
		configSave();
		loadPage(PAGE_SHOW_LAYOUT);
	}
}

static void _hStart()
{
	char s[7];
	if (KEY4)	// MENU
	{
		loadPage(PAGE_MENU);
		return;
	}
	
	if (KEYINIT)
	{
		lcdSetPos(0, 36);
		lcdSelectFont(&font12x16);
		lcdWriteString_P(strSAFE);
		lcdSelectFont(NULL);
		lcdSetPos(3, 0);
		lcdWriteString_P(strSelflevel);
		lcdWriteString_P(strSpIsSp);
		lcdSetPos(5, 0);
		lcdWriteString_P(strBattery);
	}
	
	lcdSetPos(3, 84);
	if (State.SelfLevel)
		lcdWriteString_P(strON);
	else		
		lcdWriteString_P(strOFF);
	
	lcdSetPos(4, 0);
	if (State.Error)
	{
		lcdWriteString_P(strError);
		lcdWriteChar(32);
		if (State.Error & ERR_NOT_CALIBRATED)
			lcdWriteString_P(strSensorNotCal);
		else 
		{
			lcdWriteString_P(PSTR("no "));
			if (State.Error & ERR_NO_ROLL)
				lcdWriteString_P(strRoll);
			else if (State.Error & ERR_NO_PITCH)
				lcdWriteString_P(strPitch);
			else if (State.Error & ERR_NO_YAW)
				lcdWriteString_P(strYaw);
			else 
				lcdWriteString_P(strThro);
			lcdWriteString_P(PSTR(" input"));
		}
	}	
	else
		lcdWriteString_P(PSTR("Ready for departure!"));
	
	lcdSetPos(5, 9*6);
	utoa(BATT / 10, s, 10);
	lcdWriteString(s);
	lcdWriteChar('.');
	utoa(BATT % 10, s, 10);
	lcdWriteString(s);
	lcdWriteString_P(PSTR(" V "));
}

static void _hSensorTest()
{
	writeValue(0, 48, GYRO_raw[AIL], 5, -1);
	writeValue(1, 48, GYRO_raw[ELE], 5, -1);
	writeValue(2, 48, GYRO_raw[RUD], 5, -1);
	writeValue(3, 48, ACC_raw[AIL], 5, -1);
	writeValue(4, 48, ACC_raw[ELE], 5, -1);
	writeValue(5, 48, ACC_raw[RUD], 5, -1);
	writeValue(6, 48, BATT, 5, -1);
}

static void _hReceiverTest()
{
	static const prog_char* info[5][2] PROGMEM = {
		{ strLeft, strRight },
		{ strForward, strBack },
		{ strRight, strLeft },
		{ strIdle, strFull },
		{ strOff, strOn },
	};
	
	for (uint8_t i = 0; i < RX_CHANNELS; i++)
	{
		if (RX_good & _BV(i))
		{
			writeValue(i, 66, RX[i], 4, -1);
			lcdSetPos(i, 96);
			if (i == THR)
			{
				if (State.ThrottleOff)
					writePadded_P(strIdle, 5);
				else if (RX[THR] >= 90)
					writePadded_P(strFull, 5);
				else
					writeSpace(5);
			}
			else
			{
				if (abs(RX[i]) > 10)
					writePadded_P((PGM_P)pgm_read_word(&info[i][RX[i] > 0]), 5);
				else
					writeSpace(5);
			}
		}
		else
		{
			lcdSetPos(i, 66);
			lcdWriteString_P(strNoSignal);
		}
	}
}

static void _hSensorCalibration()
{
	if (subpage == 0)
	{
		if (KEY4)
		{
			subpage = 1;
			lcdClear();
			lcdSetPos(3, 18);
			lcdWriteString_P(strWait);
			lcdSetPos(3, 78);
			lcdWriteString_P(strSec);
			writeSoftkeys(_skCANCEL);
			_tStart = millis();
		}			
	}
	else if (subpage == 1)
	{
		lcdSetPos(3, 66);
		uint8_t sec = (millis() - _tStart) / 1000;
		lcdWriteChar((5-sec) + 48);
		if (sec >= 5)
		{
			sensorsCalibate();
			configSave();
			lcdSetPos(3, 0);
			lcdWriteString_P(strCalSucc);
			writeSoftkeys(NULL);
			subpage = 2;
		}
	}
	else if (KEY4)
		loadPage(PAGE_MENU);
}

static void _hESCCalibration()
{
	if (ANYKEY)
	{
		if (subpage >= length(scrESCCal))
			loadPage(PAGE_MENU);
		else
		{
			lcdClear();
			PGM_P s = (PGM_P)pgm_read_word(&scrESCCal[subpage]);
			lcdWriteString_P(s);
			writeSoftkeys(NULL);
			subpage++;
		}		
	}
}

static void _hStickCentering()
{
	NOKEYRETURN;
	
	if (KEYINIT)
	{
		if ((RX_good & 0x0F) != 0x0F)
		{
			lcdWriteString_P(scrRadioCal1);
			subpage = 1;
			writeSoftkeys(_skBACK);
		}
		else
			lcdWriteString_P(scrRadioCal0);
	}
	
	if (subpage == 0)
	{
		if (KEY4)
		{
			rxCalibrate();
			configSave();
			lcdClear();
			lcdSetPos(3, 0);
			lcdWriteString_P(strCalSucc);
			writeSoftkeys(NULL);
			subpage = 1;
		}
	}
	else if (KEY4)
		loadPage(PAGE_MENU);
}

static void _hDebug()
{
	lcdSetPos(0, 0);
	lcdWriteString_P(PSTR("MixerIndex: "));
	char s[7];
	utoa(Config.MixerIndex, s, 10);
	lcdWriteString(s);
}

static void _hFactoryReset()
{
	if (KEYINIT)
	{
		lcdSetPos(3, 18);
		lcdWriteString_P(strAreYouSure);
	}
	else if (KEY4)	// Yes
	{
		configReset();
		configSave();
		
		// force reset by enabling watchdog and enter endless loop
		cli();
		wdt_enable(WDTO_15MS);
		for(;;);
	}
}

static void _hModeSettings()
{
	NOKEYRETURN;
	pageKey(5);
	if (KEY4)	// CHANGE?
	{
		switch(subpage)
		{
			case 0: Config.SelfLevelMode = !Config.SelfLevelMode; break;
			case 1: Config.ArmingMode = !Config.ArmingMode; break;
			case 2: Config.LinkRollPitch = !Config.LinkRollPitch; break;
			case 3: Config.AutoDisarm = !Config.AutoDisarm; break;
			case 4: Config.ReceiverMode = !Config.ReceiverMode; rxInit(Config.ReceiverMode); break;
		}
		configSave();
	}
	writeString_P(0, 84, Config.SelfLevelMode ? strAUX : strStick, 5, 0);
	writeString_P(1, 84, Config.ArmingMode ? strOn : strStick, 5, 1);
	writeString_P(2, 102, Config.LinkRollPitch ? strYes : strNo, 3, 2);
	writeString_P(3, 84, Config.AutoDisarm ? strYes : strNo, 3, 3);
	writeString_P(4, 84, Config.ReceiverMode ? strYes : strNo, 3, 4);
}

static void _hPIEditor()
{
	NOKEYRETURN;
	
	static uint8_t axis;
	
	pageKey(5);

	if (KEY4) // CHANGE
	{
		if (subpage == 0)
			axis = (axis + 1) % 3;
		else
		{
			switch (subpage)
			{
				case 1: startEditMode(&Config.PID[axis].PGain, 0, 200, 0); break;
				case 2: startEditMode(&Config.PID[axis].PLimit, 0, 200, 0); break;
				case 3: startEditMode(&Config.PID[axis].IGain, 0, 200, 0); break;
				case 4: startEditMode(&Config.PID[axis].ILimit, 0, 200, 0); break;
			}
			return;
		}
	}
	
	lcdSetPos(0, 30);
	lcdReverse(subpage == 0);
	switch (axis)
	{
		case 0: writePadded_P(strRollAil, 16); break;
		case 1: writePadded_P(strPitchEle, 16); break;
		default: writePadded_P(strYawRud, 16); break;
	}
	
	writeValue(2, 60, Config.PID[axis].PGain, 5, 1);
	writeValue(3, 60, Config.PID[axis].PLimit, 5, 2);
	writeValue(4, 60, Config.PID[axis].IGain, 5, 3);
	writeValue(5, 60, Config.PID[axis].ILimit, 5, 4);
}

static void simplePageHandler(edit_element_t *elements, uint8_t len)
{
	NOKEYRETURN;
	edit_element_t element;
	pageKey(len);
	if (KEY4)	// CHANGE?
	{
		memcpy_P(&element, &elements[subpage], sizeof(element));
		startEditMode(element.valuePtr, element.loLimit, element.hiLimit, 0);
		return;
	}
	for (uint8_t i = 0; i < len; i++)
	{
		memcpy_P(&element, &elements[i], sizeof(element));
		writeValue(element.X, element.Y, *(uint8_t*)element.valuePtr, element.len, i);
	}
}

static void _hStickScaling()
{
	static edit_element_t elements[] PROGMEM = {
		{ 2, 78, &Config.StickScaling[AIL], 0, 200, 5 },
		{ 3, 78, &Config.StickScaling[ELE], 0, 200, 5 },
		{ 4, 78, &Config.StickScaling[RUD], 0, 200, 5 },
		{ 5, 78, &Config.StickScaling[THR], 0, 200, 5 },
	};
	simplePageHandler(elements, length(elements));
}

static void _hMiscSettings()
{
	static edit_element_t elements[] PROGMEM = {
		{ 0, 102, &Config.MinThrottle, 0, 20, 4 },
		{ 1, 102, &Config.HeightDampening, 0, 250, 4 },
		{ 2, 102, &Config.HeightDampeningLimit, 0, 30, 4 },
		{ 3, 102, &Config.LVA, 0, 250, 4 },
		{ 4, 102, &Config.ServoFilter, 0, 100, 4 },
	};
	simplePageHandler(elements, length(elements));
}

static void _hSelflevelSettings()
{
	static edit_element_t elements[] PROGMEM = {
		{ 0, 54, &Config.PID_SelfLevel.PGain, 0, 250, 5 },
		{ 1, 54, &Config.PID_SelfLevel.PLimit, 0, 250, 5 },
		{ 3, 96, &Config.AccTrimRoll, -128, 127, 5 },
		{ 4, 96, &Config.AccTrimPitch, -128, 127, 5 },
	};
	simplePageHandler(elements, length(elements));
}

static void _hCPPMSettings()
{
	static edit_element_t elements[] PROGMEM = {
		{ 0, 78, &Config.RX_chmap[AIL], 1, 8, 1 },
		{ 1, 78, &Config.RX_chmap[ELE], 1, 8, 1 },
		{ 2, 78, &Config.RX_chmap[RUD], 1, 8, 1 },
		{ 3, 78, &Config.RX_chmap[THR], 1, 8, 1 },
		{ 4, 78, &Config.RX_chmap[AUX], 1, 8, 1 },
	};
	simplePageHandler(elements, length(elements));
}

void menuShow()
{
	static uint8_t oldPage = 0xFF;
	
	_mykey = keyboardRead();
		
	if (KEY1 && !editMode)	// BACK
	{
		if (page > PAGE_MENU)
			loadPage(PAGE_MENU);
		else if (page == PAGE_MENU)
			loadPage(PAGE_START);
	}
	
	lcdDisable();
	if (oldPage != page)
	{
		_mykey = KEY_INIT;
		subpage = 0;
		oldPage = page;
	}
	defaultHandler();
	lcdEnable();

	if (KEYPRESS)
		buzzerBuzz(5);
}

void menuInit()
{
	loadPage(PAGE_START);
}

static PGM_P tsmMain(uint8_t index)
{
	return (PGM_P)pgm_read_word(&lstMenu[index]);
}

static PGM_P tsmLoadModelLayout(uint8_t index)
{
	return (PGM_P)pgm_read_word(&mixerTable[index].Name);
}
