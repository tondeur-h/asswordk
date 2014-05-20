/*
 * main.cpp
 *
 *  Created on: 14 mai 2014
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


#include <string>
#include "Console.h"

//declare console loop
pawk::Console* cons;

int main (int argc, char* argv[]){

if (argc>1) {/*futur command line options*/}
if (argc>0)
{
	cons=new pawk::Console(); //create console loop object
	cons->cmd_loop(); //go to loop routine
}

//as bye bye, destroy console and quit
delete cons;
return 0;
}

