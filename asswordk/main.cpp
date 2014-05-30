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




/*!
 * Main entry point
 * A this time, did not accept options line arguments
 * wait for a future version...
 * @param argc
 * @param argv
 * @return
 */
int main (int argc, char* argv[]){

if (argc>1) {/*futur command line options not implemented a this time*/}

//don't care about nomber of command line arguments
if (argc>0)
{
	cons=new pawk::Console(); //create console object
	cons->read_config_file(); //read config file before running
	cons->cmd_loop(); //go to loop routine, this the main loop
}

//as bye bye, destroy console and quit
delete cons; //destroy
return 0; //exit success
}

