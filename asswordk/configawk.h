/*
 * configawk.h
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




#ifndef CONFIGAWK_H_
#define CONFIGAWK_H_

#include <string>
#include <libconfig.h++>
#include <cstdlib>


namespace pawk {


/*
 *class define for reading config file
 *this class used libconfig++ for this purpose
 */
const std::string VERSION="0.2";
const std::string APPLICATION="asswordk";

class configawk {

public:
	configawk(); //constructor init values
	bool init_cfg(std::string path); //open and read config file
	bool verify_version(); //verif magic word
	void read_password(const libconfig::Setting& root); //read password rules
	void read_misc(const libconfig::Setting& root); //read timer value for clrscr
	void read_encryption(const libconfig::Setting& root); //read hash and ses type encryption
	void read_colors(const libconfig::Setting& root); //list all colors to define...
	virtual ~configawk();

	//local attributes define...
	bool upcase,lowcase,number,OL,LL;
	int clrscr;
	std::string ses, hash, list;
	libconfig::Config cfg;



};

} /* namespace pawk */

#endif /* CONFIGAWK_H_ */