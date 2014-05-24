/*
 * Timer.h
 *
 *  Created on: 24 mai 2014
 *      Author: Tondeur Herve
 *
 *  This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef TIMER_H_
#define TIMER_H_
#include <sys/time.h>

namespace pawk {

class Timer
{
    public:
        Timer(unsigned int Delay, void (*pToFunction)(Timer* timer));
        virtual ~Timer();
        bool Probe();
        bool Probe(timeval& SomeTime); //could be useful if there're 1000 timers in row
        void Start() { isActive=true;}
        void Stop() { isActive=false;}
        bool IsActive() { return isActive; }
        void SetDelay(unsigned int Delay) { itsDelay=Delay; }
    protected:
    private:
        unsigned int itsDelay;
        bool isActive;
        timeval itsStart;
        void (*itspToFunction)(Timer*);
};

} /* namespace pawk */


#endif // TIMER_H_
