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
#include <libintl.h>

namespace pawk {


/*!
 * constructor, just define some variables
 */
configawk::configawk() {}


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
std::cerr<<gettext("No config file found...Use default values!")<<std::endl;
return false;
} //exception for parse error
catch(const libconfig::ParseException &pex)
{
std::cerr << gettext("Parse error at ") << pex.getFile() << ":" << pex.getLine()<< " - " << pex.getError() << std::endl;
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
	    std::cerr << gettext("No 'magic word' setting in configuration file.") << std::endl;
		  return false;
	  }
return true;
}




/*!
 * read value in config file...
 * @param root
 */
std::string configawk::read_config_string(const libconfig::Setting& root,std::string group,std::string field, std::string default_value){
	 // Read colors config
	std::string ret_val;
	  try
	  {
	    const libconfig::Setting &colors = root["asswordk"][group];
	    colors.lookupValue(field,ret_val);
	  }
	  catch(const libconfig::SettingNotFoundException &nfex)
	  {
	   ret_val=default_value; //reset to normal, is the default value...
	  }
return ret_val;
}



/*!
 * read value in config file...
 * @param root
 */
int configawk::read_config_int(const libconfig::Setting& root,std::string group,std::string field, int default_value){
	 // Read colors config
	int ret_val;
	  try
	  {
	    const libconfig::Setting &colors = root["asswordk"][group];
	    colors.lookupValue(field,ret_val);
	  }
	  catch(const libconfig::SettingNotFoundException &nfex)
	  {
	   ret_val=default_value; //reset to normal, is the default value...
	  }
return ret_val;
}




/*!
 * read value in config file...
 * @param root
 */
bool configawk::read_config_bool(const libconfig::Setting& root,std::string group,std::string field, bool default_value){
	 // Read colors config
	bool ret_val;
	  try
	  {
	    const libconfig::Setting &colors = root["asswordk"][group];
	    colors.lookupValue(field,ret_val);
	  }
	  catch(const libconfig::SettingNotFoundException &nfex)
	  {
	   ret_val=default_value; //reset to normal, is the default value...
	  }
return ret_val;
}

} /* namespace pawk */
