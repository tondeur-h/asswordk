/*
 * Timer.cpp
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


#include "Timer.h"

namespace pawk {

Timer::Timer(unsigned int Delay, void (*pToFunction)(Timer* timer))
        :itsDelay(Delay),itspToFunction(pToFunction),isActive(true)
{
    gettimeofday(&itsStart,0);
}

Timer::~Timer()
{
    itspToFunction=0;
}

bool Timer::Probe()
{
    timeval CurrentTime;
    gettimeofday(&CurrentTime,0);
    if(isActive==false || itsDelay > ((CurrentTime.tv_sec-itsStart.tv_sec)*1000 + (CurrentTime.tv_usec-itsStart.tv_usec)/1000))
    {
        return false;
    }
    else
    {
        itspToFunction(this);
        gettimeofday(&itsStart,0);
        return true;
    }
}
bool Timer::Probe(timeval& SomeTime)
{
    if(isActive==false || itsDelay > ((SomeTime.tv_sec-itsStart.tv_sec)*1000 + SomeTime.tv_usec-itsStart.tv_usec/1000))
    {
        return false;
    }
    else
    {
        itspToFunction(this);
        gettimeofday(&itsStart,0);
        return true;
    }
}

} /* namespace pawk */
