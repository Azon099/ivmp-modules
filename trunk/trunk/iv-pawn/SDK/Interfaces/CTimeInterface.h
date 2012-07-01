//============== Copyright � 2011 IV:MP Team. All rights reserved. ==============
// File: CTimeInterface.h
//======================================================================

#pragma once

#include "InterfaceCommon.h"

class CTimeInterface
{
public:
	virtual void SetTime(const unsigned char ucHour, const unsigned char ucMinute) = 0;
	virtual void GetTime(unsigned char * ucHour, unsigned char * ucMinute) = 0;

	virtual void SetMinuteDuration(const unsigned int uiMinuteDuration) = 0;
	virtual unsigned int GetMinuteDuration() = 0;

	virtual void SetDayOfWeek(const unsigned char ucDayOfWeek) = 0;
	virtual unsigned char GetDayOfWeek() = 0;
};