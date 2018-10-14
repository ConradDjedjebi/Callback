/****************************************************************************************************
  Callback.h

  License: GNU GPLv3

  A callback generator library for Arduino
  Written by A BORDIN and C DJEDJEBI

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.
  You should have received a copy of the GNU General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

Version Modified By Date     Comments
------- ----------- -------- --------
0001    A BORDIN      13/04/15 ArduinoTimerObject Library (https://github.com/aron-bordin/ArduinoTimerObject)
0002    C DJEDJEBI    12/10/18 Added support for event duration (setEventDuration, hasEventDuration, etc.)
0003    C DJEDJEBI    12/10/18 ArduinoTimerObject Library renamed to Callback
****************************************************************************************************/

#include "stdlib.h"
#include "Arduino.h"

#ifndef CALLBACK_H
#define CALLBACK_H
typedef void (*CallBackType)();


class Callback{
	private:
		void Create(unsigned long int us, CallBackType callback, bool isSingle, bool hasEventDuration);
		unsigned long int usInterval;
		unsigned long int usEventDuration;
		bool blEnabled;
		bool blSingleShot;
		bool blOffTimer = false;
		bool blEventDuration;
		CallBackType onRun;
		CallBackType onStop;
		bool tick();
		unsigned long startTime;
		unsigned long LastTime;
		unsigned long DiffTime;//used when We pause the Callback and need to resume
	public:
		Callback(void);
		Callback(unsigned long int us);
		Callback(unsigned long int us, CallBackType callback);
		Callback(unsigned long int us, CallBackType callback, bool isSingle);
		~Callback();

		void setInterval(unsigned long int us);
		void setEventDuration(bool bool_hasEventDuration, unsigned long int us);
		void setEnabled(bool Enabled);
		void setSingleShot(bool isSingle);
		void setOnTimer(CallBackType callback);
		void setOffTimer(CallBackType callback);
		void start();
		void resume();
		void pause();
		void stop();
		void restart();
		void update();


		unsigned long int getInterval();
		unsigned long int getCurrentTime();
		CallBackType getOnTimerCallback();

		bool isEnabled();
		bool isSingleShot();
		bool hasOffFunction();
		bool hasEventDuration();

};


#endif // CALLBACK_H