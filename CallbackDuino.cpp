/****************************************************************************************************
  CallbackDuino.cpp

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
0003    C DJEDJEBI    14/10/18 ArduinoTimerObject Library renamed to CallbackDuino
****************************************************************************************************/

#include "CallbackDuino.h"


/**
 * Constructeur de l objet callback
 */
CallbackDuino::CallbackDuino(){
	Create(100, NULL, false, false);
}

/**
 * Constructeur du callback en lui specifiant
 * une période d'exécution
 *
 * \param unsigned long int us : période à laquelle le callback execute la fonction qui lui est attachée
 */
CallbackDuino::CallbackDuino(unsigned long int us){
	Create(us, NULL, false, false);
}

/**
 * Constructeur du callback en lui specifiant
 * une période d'exécution
 *
 * \param unsigned long int us : période à laquelle le callback execute la fonction qui lui est attachée
 * \param CallbackType callback : la fonction à appeler après écoulement de la période spécifiée.
 */
CallbackDuino::CallbackDuino(unsigned long int us, CallBackType callback){
	Create(us, callback, false, false);
}

/**
 * Constructeur du callback en lui specifiant
 * une période d'exécution
 *
 * \param unsigned long int us : période à laquelle le callback execute la fonction qui lui est attachée
 * \param CallbackType callback : la fonction à appeler après écoulement de la période spécifiée.
 * \param bool isSingle : le paramètre indiquant si le callback doit être lancé sur une seule période.
 */
CallbackDuino::CallbackDuino(unsigned long int us, CallBackType callback, bool isSingle){
	Create(us, callback, isSingle, false);
}


/**
 * Destructeur de l objet callback
 */
CallbackDuino::~CallbackDuino(){}

/**
 * Initialisation des différents paramètres intervenant dans le fonctionnement du callback
 * 
 * \param unsigned long int us : période à laquelle le callback execute la fonction qui lui est attachée
 * \param CallbackType callback : la fonction à appeler après écoulement de la période spécifiée.
 * \param bool isSingle : le paramètre indiquant si le callback doit être lancé sur une seule période.
 * \param bool hasEventDuration : paramètre indiquant si le callback doit être exécuté, de manière répétée, mais sur une durée gloabale donnée
 */
void CallbackDuino::Create(unsigned long int us, CallBackType callback, bool isSingle, bool hasEventDuration){
	setInterval(us);
	setEnabled(false);
	setSingleShot(isSingle);
	setEventDuration(hasEventDuration, NULL);
	setOnTimer(callback);
	LastTime = 0;
}

/**
 * Setter pour modifier l'interval d'appel de la fonction associée au callback
 *
 * \param unsigned long int : période d'execution de la fonction à appellera
 */
void CallbackDuino::setInterval(unsigned long int us){
	usInterval = (us > 0) ? us : 0;
}


/**
 * Setter pour modifier la durée globale d'appel de la fonction associée au callback
 *
 * \param bool bool_hasEventDuration : booléen indiquant si le callback a une durée global
 * \param unsigned long int us : la durée globale voulue pour ce callback
 */
void CallbackDuino::setEventDuration(bool bool_hasEventDuration, unsigned long int us){
	blEventDuration = bool_hasEventDuration;
	usEventDuration = (us > 0) ? us : 0;
}


/**
 * Setter pour modifier l'état de la variable booléenne indiquant si le callback doit être activé
 *
 * \param bool Enable : booléen indiquant l'état activé ou non du callback
 */
void CallbackDuino::setEnabled(bool Enabled){
	blEnabled = Enabled;
}

/**
 * Setter pour modifier l'état de la variable booléenne indiquant si le callback doit être excuté seulement pour un cycle
 *
 * \param bool isSingle : booléen indiquant si le callback s'excureta juste sur un cycle.
 */
void CallbackDuino::setSingleShot(bool isSingle){
	blSingleShot = isSingle;
}

/**
 * Setter pour fournir la fonction qui doit être appelée par le callback à l'issue de la période.
 *
 * \param CallBackType callback : la fonction qui sera appelée à l'issue d'une période du callback.
 */
void CallbackDuino::setOnTimer(CallBackType callback){
	onRun = callback;
}

/**
 * Setter pour fournir la fonction qui doit être appelée par le callback à l'issue de la durée globlale prévue pour son fonctionnement.
 *
 * \param CallBackType callback : la fonction qui sera appelée à l'issue de la durée globale du callback, lors de l'arrêt du timer.
 */
void CallbackDuino::setOffTimer(CallBackType callback){
	blOffTimer = true;
	onStop = callback;
}

/**
 * Methode pour mettre en marche le callback
 */
void CallbackDuino::start(){
	LastTime = micros();
	startTime = micros();
	setEnabled(true);	
}

/**
 * Methode pour remettre en route le callback après une mise en pause
 */
void CallbackDuino::resume(){
	LastTime = micros() - DiffTime;
	setEnabled(true);
}

/**
 * Methode pour arrêter le callback
 */
void CallbackDuino::stop(){
	setEnabled(false);
	if(hasOffFunction())
	{
		onStop();
	}
}

/**
 * Methode pour redémarrer le callback
 */
void CallbackDuino::restart(){
	stop();	
	start();
}


/**
 * Methode pour mettre en pause le callback
 */
void CallbackDuino::pause(){
	DiffTime = micros() - LastTime;
	setEnabled(false);
	if(hasOffFunction())
	{
		onStop();
	}
}


/**
 * Methode pour executer la fonction associée au callback si la période d'activation est atteinte
 */
void CallbackDuino::update(){
	if(tick())
	{
		onRun();
	}
}


/**
 * Methode pour vérifier si la période d'activation de la fonction associée au callback est atteinte
 */
bool CallbackDuino::tick(){
	unsigned long int val, dureeTick, dureeEvent;
	val = micros();
	if(val < LastTime) // //micros restarted
	{
		dureeTick = (unsigned long int)(4294967295 - LastTime + val);
		dureeEvent = (unsigned long int)(4294967295 - startTime + val);
	}
	else
	{
		dureeTick = (unsigned long int)(val - LastTime);
		dureeEvent = (unsigned long int)(val - startTime);
	}


	if(!blEnabled)
	{
		return false;
	}
	if (dureeTick >= usInterval) 
	{
		LastTime = micros();
		if(isSingleShot())
		{
			setEnabled(false);
		}
		if(hasEventDuration())
		{
			if(dureeEvent >= usEventDuration)
			{
				setEnabled(false);	
				if(hasOffFunction())			
				{
						onStop();
				}
				setEventDuration(false, NULL);
				return false;
			}
		}	
	    return true;
	}
	return false;
}

/**
 * Getter pour récupérer l'interval séparant deux exécutions de la fonction associée au callback
 *
 * \return unsigned long int : usInterval
 */
unsigned long int CallbackDuino::getInterval(){
	return usInterval;
}

/**
 * Getter pour récupérer le lapse de temps séparant la dernière exécution de la fonction du callback à l'instant présents.
 *
 * \return unsigned long int : durée écoulée
 */
unsigned long int CallbackDuino::getCurrentTime(){
	return (unsigned long int)(micros() - LastTime);
}

/**
 * Getter pour récupérer la fonction censée être exécutée par le callback à l'issue de la période paramètrée.
 *
 * \return CallBackType onRun : fonction à exécuter à chaque cycle
 */
CallBackType CallbackDuino::getOnTimerCallback(){
	return onRun;
}

/**
 * Booléen indiquant si le callback est activé
 *
 * \return bool blEnabled : attribut indiquant si le callback est activé
 */
bool CallbackDuino::isEnabled(){
	return blEnabled;
}


/**
 * Booléen indiquant si le callback est paramètré pour exécuter sa fonction associée une seule fois
 *
 * \return bool blSingleShot : attribut indiquant si le callback possède un seul cycle
 */
bool CallbackDuino::isSingleShot(){
	return blSingleShot;
}

/**
 * Booléen indiquant si le callback possède une fonction à exécuter lors de sa mise en arrêt
 *
 * \return bool blOffTimer : attribut indiquant si le callback possède une fonction à exécuter lors de son extinction
 */
bool CallbackDuino::hasOffFunction(){
	return blOffTimer;
}

/**
 * Getter indiquant si le callback possède une durée globale d'exécution
 *
 * \return bool blEventDuration : attribut indiquant si le callback possède une durée globale
 */
bool CallbackDuino::hasEventDuration(){
	return blEventDuration;
}