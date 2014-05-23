/*
 * Console.cpp
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

#include "Console.h"
#include <iostream>
using std::cout;
using std::cin;
using std::string;

#include <algorithm>

//include for manage screen color etc...
#include "tools.h"
using std::color;
using std::normal_color;

//crypto++ manager
#include "awkBlowfish.h"

//dbfile manage
#include "DBFile.h"


namespace pawk {

DBFile* db; //object dbfile


/*!
 * Define a new entry into the password database
 */
void Console::new_entry(){
//local entry variables
string login;
string password;
int pwlength;

	//ask questions about entry
	cout<<"nom du login ?";
	cin>>login;
	cout<<"mot de passe (\"new\" pour en generer un)?";
	cin>>password;

	cout<<password.length()<<std::endl;

	if (password.length()==0){
		cout<<"Longueur du mot de passe ?";
		cin>>pwlength;

		/*		if (){
					cout<<"Erreur de longueur, ne peut pas être null ou supérieur a 20";
					return;
			}*/
	}


//save them

}

void Console::delete_entry(int id){

}


/*!
 * function to list all the entry from db,
 * just show login@url/computer
 * @param id
 */
void Console::list_entry(int max_id){
//cout<<"#-- max id :"<<max_id<<endl;

//read all entries
//db->readAll(entries);

std::color("35");
for(int nbl=0;nbl<entries->size();nbl++){
if (nbl>=max_id){
	cout<<"id "<<nbl<<" - "<<entries->at(nbl).login<<"@"<<entries->at(nbl).url<<endl;
}
}
//return to normal color
std::normal_color();
//if entries vector is empty then signal to user.
if (entries->size()==0){
	cout<<"Entries list is empty..."<<endl;
}

} //end list function

/*!
 * function that help to modify en entry if exist...
 * @param id
 */
void Console::modify_entry(int id){

} //end modify function


/*!
 * Show un full entry wich pass is id
 * @param id
 */
void Console::print_entry(int id){
//cout<<"#-- view id :"<<id<<endl;

//if id is ok then show all
if ((unsigned int)id<(entries->size())){
	std::color("32");
	cout<<"id : "<<id<<endl;
	cout<<"login : "<< entries->at(id).login<<endl;
	ABlowfish::awkBlowfish crypto;
	//decrypt password for user
	cout<<"password : "<< crypto.decrypt(crypto.stringtoCipher(entries->at(id).password),"c9tzzf2")<<endl;
	cout<<"url :"<<entries->at(id).url<<endl;
	cout<<"notes :"<<entries->at(id).notes<<endl;
	std::normal_color();
} else
{
	//oops id not right
	cout<<"Error, id not found..."<<endl;
}

} //end print function

/*!
 * purge all the entries into the entries vector and save the file in disk
 */
void Console::purge(){
//clear all entries
entries->clear();
}

/*!
 * print hello header from the application
 */
void Console::hello(){
std::clrscr();
color("32");
cout<<"asswordk version 0.1"<<std::endl;
normal_color();
cout<<"Passwords manager application"<<std::endl;
cout<<"Copyright (c) 2014 Tondeur hervé\nhttp://www.tondeurh.fr"<<std::endl;
cout<<"the uses terms of this program are defined with General Public Licence (GPL) Version 3"<<std::endl;
cout<<"Type command \"help\" to get helps..."<<endl;
cout<<std::endl;
}

/*!
 * metho that print general help and specific help
 * @param value
 */
void Console::help(string value){
	//cout<<"#-- "<<value<<std::endl;

	int cmd=0;
	value=std::ucase(value);
	//convert parameter for help action into integer value
	cmd=(value.compare("QUIT")==0)?1:cmd;
	cmd=(value.compare("HELP")==0)?2:cmd;
	cmd=(value.compare("LIST")==0)?3:cmd;
	cmd=(value.compare("NEW")==0)?4:cmd;
	cmd=(value.compare("DELETE")==0)?5:cmd;
	cmd=(value.compare("MODIFY")==0)?6:cmd;
	cmd=(value.compare("PURGE")==0)?7:cmd;
	cmd=(value.compare("CLEAR")==0)?8:cmd;
	cmd=(value.compare("HELLO")==0)?9:cmd;
	cmd=(value.compare("PRINT")==0)?10:cmd;

switch (cmd){
case 1://quit help
		cout<<"commands help\n============================"<<std::endl;
		color("32");
		cout<<"Syntax : quit\n"<<std::endl;
		normal_color();
		cout<<"=>Type this command with no parameter to quit this application.\n"<<std::endl;
		break;
case 2: //help help
		cout<<"commands help\n============================"<<std::endl;
		color("32");
		cout<<"Syntax : help <cmd>\n"<<std::endl;
		normal_color();
		cout<<"=>Show details of a help command.\n<cmd> can take one of this values following :\n"<<std::endl;
		cout<<"list\tnew\tmodify\tdelete\tprint"<<std::endl;
		cout<<"purge\tclear\thelp\thello\tquit\n"<<std::endl;
		cout<<"=>If <cmd> is empty, show general help.\n"<<std::endl;
		break;
case 3: //help list
		cout<<"commands help\n============================"<<std::endl;
		color("32");
		cout<<"Syntax : list [id]\n"<<std::endl;
		normal_color();
		cout<<"=>Print the list of credentials.\n[id] point the credential number to begin printing, if id is not set, all credentials entries are printing.\n"<<std::endl;
		break;
case 4: //help new
		cout<<"commands help\n============================"<<std::endl;
		color("32");
		cout<<"Syntax : new\n"<<std::endl;
		normal_color();
		cout<<"=>This command permit to create a new credential entry.\n"<<std::endl;
		break;
case 5: //help delete
		cout<<"commands help\n============================"<<std::endl;
		color("32");
		cout<<"Syntax : delete <id>\n"<<std::endl;
		normal_color();
		cout<<"=>Delete a credential entry from the list of credentials, id must be the number of the credential to delete.\nUse \"list\" command before to get the id number.\n"<<std::endl;
		break;
case 6: //help delete
		cout<<"commands help\n============================"<<std::endl;
		color("32");
		cout<<"Syntax : modify <id>\n"<<std::endl;
		normal_color();
		cout<<"=>Modify an entry from the credentials list, id must be the number of the credential to modify.\nUse \"list\" command before to get the id number.\n"<<std::endl;
		break;
case 7: //help delete
		cout<<"commands help\n============================"<<std::endl;
		color("32");
		cout<<"Syntax : purge\n"<<std::endl;
		normal_color();
		cout<<"=>Delete all entries from credential\n Be carefull evething will be lost.\n"<<std::endl;
		break;
case 8: //help delete
		cout<<"commands help\n============================"<<std::endl;
		color("32");
		cout<<"Syntax : clear\n"<<std::endl;
		normal_color();
		cout<<"=>Erase screen for confidentiality.\n"<<std::endl;
		break;
case 9: //help delete
		cout<<"commands help\n============================"<<std::endl;
		color("32");
		cout<<"Syntax : hello\n"<<std::endl;
		normal_color();
		cout<<"=>Show application name and copyright.\n"<<std::endl;
		break;
case 10: //help delete
		cout<<"commands help\n============================"<<std::endl;
		color("32");
		cout<<"Syntax : print <id>\n"<<std::endl;
		normal_color();
		cout<<"=>Print the detail of an entry from the credentials, id must be the number of the credential to print.\nUse \"list\" command before to get the id number.\n"<<std::endl;
		break;
default: //general help
	cout<<"=================================="<<std::endl;
	cout<<"Commands list\n"<<std::endl;
	cout<<"list\tnew\tmodify\tdelete\tprint"<<std::endl;
	cout<<"purge\tclear\thelp\thello\tquit\n"<<std::endl;
	cout<<"To get detail for a command use : \nhelp <cmd>, where cmd is the name of a command.\n"<<std::endl;
}
}



/*!
 * switch routine for select function from commande ask
 * @param cmdi as input int to simplify switch selection
 * @param value string parameter
 */
void Console::run_cmd(int cmdi,string value){

	switch (cmdi){
	case 1:
			cout<<"bye!"<<std::endl;
			quit=true;
			break;
	case 2:
			help(value);
			break;
	case 3:
			list_entry(std::atoi(value.c_str()));
			break;
	case 4:
			new_entry();
			break;
	case 5:
			delete_entry(std::atoi(value.c_str()));
			break;
	case 6:
			modify_entry(std::atoi(value.c_str()));
			break;
	case 7:
			purge();
			break;
	case 8:
			std::clrscr();
			break;
	case 9:
			hello();
			break;
	case 10:
			print_entry(std::atoi(value.c_str()));
			break;
	default:
			cout<<"Unknow command (type help)!"<<std::endl;

	}
}


void Console::execute_cmd(string cmdl,string value){
int cmd;

cmd=0;
cmdl=std::ucase(std::trim(cmdl));

cmd=(cmdl.compare("QUIT")==0)?1:cmd;
cmd=(cmdl.compare("HELP")==0)?2:cmd;
cmd=(cmdl.compare("LIST")==0)?3:cmd;
cmd=(cmdl.compare("NEW")==0)?4:cmd;
cmd=(cmdl.compare("DELETE")==0)?5:cmd;
cmd=(cmdl.compare("MODIFY")==0)?6:cmd;
cmd=(cmdl.compare("PURGE")==0)?7:cmd;
cmd=(cmdl.compare("CLEAR")==0)?8:cmd;
cmd=(cmdl.compare("HELLO")==0)?9:cmd;
cmd=(cmdl.compare("PRINT")==0)?10:cmd;

run_cmd(cmd,value);
}

/*!
 * create interactive console
 * and initialize variables needed
 */
Console::Console() {
hello();
prompt="asswordk#> ";
quit=false; //boolean quit variable for quit prompt loop
identified=false; //not identified by default
mainpassword=""; //no main password by default
entries=new std::vector<struct_entry>(); //create vector for all database
}


/*!
 * read user commande from prompt
 */
void Console::cmd_loop(){
	string* line; //line to read

	//open db file...
	db=new DBFile(); //create access dbfile

	//if file not exist quit this function
	if (!db->openForReadWrite("/usr/share/asswordk/asswordk.db")){
		cout<<"Error - Database file not found..."<<endl;
		cout<<"You must create a databasefile, see documentation..."<<endl;
		delete db;
		return;
	}

	//test if mainpassword and header ok...
	if (!db->testFileFormat()){
		cout<<"Credential is empty, create one please..."<<endl;	;
		delete db;

		//propose to create a new one...
		if 	(!create_credential()){return;}
	}


	//before continuing, user must be loged...
	if (!identifying()){
	cout<<"Error - you are not correctly identified... sorry."<<endl;
	//bye bye
	exit(3);
	} else
	{//say hello
		std::color("32");
		cout<<"Hello - you're welcome!"<<endl;
		std::normal_color();
	}


	db=new DBFile(); //create access dbfile
	db->openForReadWrite("/usr/share/asswordk/asswordk.db");
	//read all entries
	db->readAll(entries);
	//close the dbfile if all is reading...
	delete db;

	do{
	cmd_line.clear();
	cin.clear();
	//print the prompt on screen
	cout<<prompt.c_str();
	//read user asks
	std::getline (cin,cmd_line);

	//parse user asks in multiples words
	if (!cmd_line.empty()){
		line=std::scanner(cmd_line,' ');
		//execute commande line user ask if possible
		execute_cmd(line[0],line[1]);
		std::close_scanner(); //free memory from scanner function
	}
	} while (!quit);
}

/*!
 * nothing to do
 */
Console::~Console() {

delete entries; //delete vector for all database...
//delete db;
}

bool Console::identifying(){
if (!identified){
string temppass;
	//ask password
std::color("7"); //inverse video
cout<<"You must be identified to continue (enter your password follow by enter key) ?";
std::color("0"); //reset
//std::color("30"); //black color to hide password=> need a better way to do that
temppass=std::getpass();
//std::getline(cin,temppass);
//std::normal_color(); //reset background and foreground colors.
std::clrscr(); //clear screen

	//encode it in sha512
	ABlowfish::awkBlowfish sha;
	string encoded=sha.sha(temppass);

	//read encoded string in database file
	db=new pawk::DBFile();
	db->openForReadWrite("/usr/share/asswordk/asswordk.db");
	string readpass;
	readpass=db->readValue("mainpassword");
	delete db;

	//if the same return true else send message and return false
	if (encoded.compare(readpass)==0){
		mainpassword=temppass;
		identified=true;
		return true;
	}
	else
	{
		cout<<"This password is not correct..."<<endl;
		return false;
	}
}//end test identified
return true;
}

bool Console::create_credential(){
	string pass1, pass2;
//ask for a password
cout<<"Please enter a password for protect your new credentials : (enter a password follow by a enter key)?";
pass1=std::getpass();
	//if pass1 is empty stop
	if (pass1.empty()){
		cout<<"This password cannot be empty!..."<<endl;
		return false;
	}

//confirm it
cout<<"Please confirm your password : (enter a password follow by a enter key)?";
pass2=std::getpass();
	//if pass1!=pass2 stop
	if (pass1.compare(pass2)!=0){
	cout<<"Your confirm password is not the same!..."<<endl;
	return false;
}

	//hash in sha512 this password
	ABlowfish::awkBlowfish sha;
	string encoded=sha.sha(pass1);


	//create the dbfile ans save coded password
	db=new pawk::DBFile();
	db->openForReadWrite("/usr/share/asswordk/asswordk.db");
	db->writeHeader(encoded);
	delete db;
	//default return...
	return true;
}

} /* namespace pawk */
