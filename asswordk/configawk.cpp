/*
 * configawk.cpp
 *
 *  Created on: 31 mai 2014
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



#include "configawk.h"
#include <iostream>

namespace pawk {



/*!
 * constructor, just define some variables
 */
configawk::configawk() {
	lowcase=true;
	upcase=true;
	OL=false;
	LL=false;
	number=true;
	clrscr=5;
}


/*!
 * to do nothing...
 */
configawk::~configawk() {
	// TODO Auto-generated destructor stub
}



/*!
 * Open and read config file,
 * test parse
 * @param path
 * @return
 */
bool configawk::init_cfg(std::string path){

// Read the file. If there is an error, report it and exit.
try
{
//open and read file
cfg.readFile(path.c_str());
} //exception for IO error
catch(const libconfig::FileIOException &fioex)
{
std::cerr<<"No config file found...Use default values!"<<std::endl;
return false;
} //exception for parse error
catch(const libconfig::ParseException &pex)
{
std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()<< " - " << pex.getError() << std::endl;
return false;
}
//all is ok
return true;
}




/*!
 * verify if there is the magic word in this file
 * to be sure the config file is the right format
 * and version of the application
 * @return
 */
bool configawk::verify_version(){
	// Get the store name.
	  try
	  {
		//read application config value, must be 'asswordk'
	    std::string application = cfg.lookup("application");
	    if (application.compare(APPLICATION)!=0) {return false;}
	    //read version, must be '0.2'
	    std::string version = cfg.lookup("version");
	    if (version.compare(VERSION)!=0) {return false;}
	  }//exception handler...
	  catch(const libconfig::SettingNotFoundException &nfex)
	  {
	    std::cerr << "No 'magic word' setting in configuration file." << std::endl;
		  return false;
	  }
return true;
}




/*!
 * read password rules generator
 * @param root
 */
void configawk::read_password(const libconfig::Setting& root){
	 // Read password config
	  try
	  {
	    const libconfig::Setting &password = root["asswordk"]["password"];

	    password.lookupValue("upcase",upcase);
	    password.lookupValue("lowcase",lowcase);
	    password.lookupValue("number",number);
	    password.lookupValue("OL",OL);
	    password.lookupValue("LL",LL);

	  }
	  catch(const libconfig::SettingNotFoundException &nfex)
	  {
	   upcase=true;
	   lowcase=true;
	   number=true;
	   OL=false;
	   LL=false;
	  }
}




/*!
 * read timer clearscreen value...
 * @param root
 */
void configawk::read_misc(const libconfig::Setting& root){
	 // Read miscelleanous config
	  try
	  {
	    const libconfig::Setting &misc = root["asswordk"]["misc"];
	    misc.lookupValue("clrscr",clrscr);
	    if (clrscr<1) {clrscr=1;}
	  }
	  catch(const libconfig::SettingNotFoundException &nfex)
	  {
		  clrscr=5;
	  }
}




/*!
 * read encryption algorith to use...
 * @param root
 */
void configawk::read_encryption(const libconfig::Setting& root){
	 // Read encryption config
	  try
	  {
	    const libconfig::Setting &encryption = root["asswordk"]["encryption"];

	    encryption.lookupValue("hash",hash);
	    encryption.lookupValue("ses",ses);
	  }
	  catch(const libconfig::SettingNotFoundException &nfex)
	  {
	   hash="SHA512";
	   ses="AES";
	  }
}




/*!
 * read color for displaying strings in console...
 * @param root
 */
void configawk::read_colors(const libconfig::Setting& root){
	 // Read colors config
	  try
	  {
	    const libconfig::Setting &colors = root["asswordk"]["colors"];
	    colors.lookupValue("list",list);
	  }
	  catch(const libconfig::SettingNotFoundException &nfex)
	  {
	   list="0"; //reset to normal, is the default value...
	  }
}

} /* namespace pawk */
