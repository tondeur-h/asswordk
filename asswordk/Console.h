/*
 * Console.h
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

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <string>
#include <vector>


namespace pawk {


/*!
 * Structure used to represent a entry password for the application
 * We create a vector later base on this structure to manage multiple entries
 */
struct struct_entry {
	std::string login;
	std::string password;
	int pwlenght; //not use
	std::string url; //url or computer name
	std::string notes; //some comments on this credentials
	std::string container; //Give a container where to sort credentials
};

/*!
 *Main loop class
 */
class Console {


public:
	std::string prompt; //prompt to display
	std::string cmd_line; //command line from user
	bool quit; //boolean if is true halt application
	bool identified; //if is true say that you already identified into application
	struct_entry entry; //define a entry variable
	std::vector<struct_entry>* entries; //vector that represent all the dbfile into memory
	std::string mainpassword; //main password used as key for blowfish algorithm.

	Console(); //create console loop object
	void cmd_loop(); //main loop waiting for user commands
	void hello(); //display hello message
	void help(std::string value); //display help on commands
	long int ascii2number(std::string numascii); //test if ascii is a number
	std::string generate_password(long int size);//generate a password with a specific size
	void new_entry(); //command new
	void list_entry(unsigned int id); //command list
	void listcont_entry(std::string ctn); //command list
	void modify_entry(int id); //command modify
	void delete_entry(int id); //command delete
	void purge(); //command purge
	void search_entries(std::string login);//command search
	void browse_entry(int id);//command browse default browser for internet
	void copy_entry_password(int id); //command copy allows to copy the password in the X clipboard
	void duplicate_entry(int id); //duplicate an entry and allows to modify it
	void print_entry(int id); //command print
	void execute_cmd(std::string cmdl,std::string value); //switch command to execute
	void run_cmd(int cmdi,std::string value); //distribute command
	bool identifying(); //ask to identify if not!
	bool create_credential();//create a new credential
	bool change_password();//for changing mainpassword by user
	bool read_config_file();//read the default config file... at home/xxx/.asswork.cfg
	virtual ~Console();
};

} /* namespace pawk */

#endif /* CONSOLE_H_ */
