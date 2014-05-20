/*
 * tools.h
 *
 *  Created on: 15 mai 2014
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

#ifndef TOOLS_H_
#define TOOLS_H_

#include <string>
#include <cctype>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <cstdio>


namespace std {


std::string trim(std::string str); //trim all whitespace
std::string ltrim(std::string str); //trim left whitespace
std::string rtrim(std::string str); //trim right whitespace
std::string ucase(std::string str); //up case caracters
std::string lcase(std::string str); //down case caracters
std::string* scanner(std::string str,char delim); //cut a sentence into words according to delimiter
void close_scanner(); //just to free memory for scanner function
void clrscr(); //clear screen
void normal_color(); //set color to normal color in console...
void color(const char* param); //change colors caracters into console

/*   param must be a const char *, empty (same as "0") or with
     several values separate with ; among
         0  reinit         1  higkt intensity (caracters)
         5  blinking             7  reverse video
         30, 31, 32, 33, 34, 35, 36, 37 caracters colors
         40, 41, 42, 43, 44, 45, 46, 47 background color

            colors, RGB following logic, respectively being
               black, red, green, yellow, blue, magenta, cyan and white */


}

#endif /* TOOLS_H_ */
