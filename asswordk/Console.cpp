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

//output
#include "Console.h"
#include <iostream>
using std::cout;
using std::cin;
using std::string;

//for iterator
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
 * convert a ascii number format into a integer value...
 * @param numascii
 * @return
 */
long int Console::ascii2number(std::string numascii){
	//return int value stranlated from ascii, if wrong return 0
return std::strtol(numascii.c_str(),0,10);
}




/*!
 *
 * @param size
 * @return
 */
std::string Console::generate_password (long int size){

	static const char alphanum[] =
	"0123456789"
	"!@#$%^&*"
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz";

	int stringLength = sizeof(alphanum) - 1;
	    srand(time(0));
	    std::string Str;
	    for(long int i = 0; i < size; ++i){Str += alphanum[rand() % stringLength];}
return Str;
}




/*!
 * Define a new entry into the password database
 */
void Console::new_entry(){
//local entry variables
string login;
string password;
string pwlenght;
bool pwl;
string notes;
string url;


	//ask questions about entry
	//get login cannot be empty...
	while (login.empty()){
	cout<<"login ?";
	getline(cin,login);
	if (login.empty()){cout<<"Cannot be empty!"<<endl;}
	}

	//get password can be empty
	cout<<"password (blank to generate one)?";
	getline(cin,password);


	if (password.length()==0){
		pwl=false;
		while(!pwl){
			cout<<"Longueur du mot de passe ?";
			getline(cin,pwlenght);

			pwl=(ascii2number(pwlenght)>0) & (ascii2number(pwlenght)>=4);
			if (!pwl) {cout<<"It's not a number or is value is less than  4! retry..."<<endl;}
			else
			{
				password=generate_password(ascii2number(pwlenght));
				cout<<"The password generated is : "<<password<<endl;
			}
		}
	}

	//get url cannot be empty
	while (url.empty()){
	cout<<"url or machine name ?";
	getline(cin,url);
	if (url.empty()){cout<<"Cannot be empty!"<<endl;}
	}

	//get notes can be empty
	cout<<"notes about this connection ?";
	getline(cin,notes);


//save informations
	ABlowfish::awkBlowfish a;

	entry.login=login;
	entry.password=a.ciphertoString(a.crypt(password,mainpassword));
	entry.pwlenght=0;
	entry.url=url;
	entry.notes=notes;

	//add to vector entries this entry...
	entries->push_back(entry);

	db=new DBFile(); //create access dbfile
	db->resetFile("/usr/share/asswordk/asswordk.db");
	db->writeHeader(a.sha(mainpassword));
	//write all entries left
	db->writeAll(entries);
	//close the dbfile if all is reading...
	delete db;

	//show modifications...
	list_entry(0);
}




/*!
 * this function delete a entry into the credential...
 * @param id
 */
void Console::delete_entry(int id){
	if (entries->size()>0){
		//erase entry
		entries->erase(entries->begin()+id);
		//say ok done
		cout<<"delete entry "<<id<<" ok..."<<endl;
		cout<<"Compact id entries..."<<endl;
		//print list of entries left...
		list_entry(0);
		//save values
		db=new DBFile(); //create access dbfile
		db->resetFile("/usr/share/asswordk/asswordk.db");
		ABlowfish::awkBlowfish a;
		db->writeHeader(a.sha(mainpassword));
		//write all entries left
		db->writeAll(entries);
		//close the dbfile if all is reading...
		delete db;
	}
	else
	{
		cout<<"No entry left in the credential, impossible to satisfy your ask..."<<endl;
	}
} //end delete function




/*!
 * function to list all the entry from db,
 * just show login@url/computer
 * @param id
 */
void Console::list_entry(unsigned int max_id){
//cout<<"#-- max id :"<<max_id<<endl;

std::color("35");
for(unsigned int nbl=0;nbl<entries->size();nbl++){
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
//ask before do to be sure and confirm with password ask
cout<<"Before let you purge all entries, please identify you with your password..."<<endl;

//fail identification for security
identified=false;
//ask to be identified again
if (identifying()){
	//right clear all entries
	entries->clear();

	db=new DBFile(); //create access dbfile
	db->openForReadWrite("/usr/share/asswordk/asswordk.db");
	ABlowfish::awkBlowfish a;
	db->writeHeader(a.sha(mainpassword));
	//close the dbfile if all is reading...
	delete db;

	cout<<"OK - Purge is done!\n"<<endl;
}
else
{
	//fail identifying => panic stop all
	cout<<"Sorry - As you fail to identifying you correctly, i stop working... :(!\n"<<endl;
	exit(3);
}
} //end purge function




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
cout<<"Terms of use of this program are defined by the General Public Licence (GPL) Version 3"<<std::endl;
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
cmd=(value.compare("PASSWORD")==0)?11:cmd;


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
		cout<<"list\tnew\tmodify\tdelete\tprint\tpassword"<<std::endl;
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
		cout<<"=>Delete a credential entry from the list of credentials, id must be the number of the credential to delete.\nUse \"list\" command before to get the id number.\nIf id is ommited, assume 0\n"<<std::endl;
		break;
case 6: //help modify
		cout<<"commands help\n============================"<<std::endl;
		color("32");
		cout<<"Syntax : modify <id>\n"<<std::endl;
		normal_color();
		cout<<"=>Modify an entry from the credentials list, id must be the number of the credential to modify.\nUse \"list\" command before to get the id number.\n"<<std::endl;
		break;
case 7: //help purge
		cout<<"commands help\n============================"<<std::endl;
		color("32");
		cout<<"Syntax : purge\n"<<std::endl;
		normal_color();
		cout<<"=>Delete all entries from credential\n Be carefull evething will be lost.\nYou must identifying yourself before."<<std::endl;
		break;
case 8: //help clear
		cout<<"commands help\n============================"<<std::endl;
		color("32");
		cout<<"Syntax : clear\n"<<std::endl;
		normal_color();
		cout<<"=>Erase screen for confidentiality.\n"<<std::endl;
		break;
case 9: //help hello
		cout<<"commands help\n============================"<<std::endl;
		color("32");
		cout<<"Syntax : hello\n"<<std::endl;
		normal_color();
		cout<<"=>Show application name and copyright.\n"<<std::endl;
		break;
case 10: //help print
		cout<<"commands help\n============================"<<std::endl;
		color("32");
		cout<<"Syntax : print <id>\n"<<std::endl;
		normal_color();
		cout<<"=>Print the detail of an entry from the credentials, id must be the number of the credential to print.\nUse \"list\" command before to get the id number.\n"<<std::endl;
		break;
case 11: //help password
		cout<<"commands help\n============================"<<std::endl;
		color("32");
		cout<<"Syntax : password\n"<<std::endl;
		normal_color();
		cout<<"=>Change the mainpassword.\n"<<std::endl;
		break;
default: //general help
	cout<<"=================================="<<std::endl;
	cout<<"Commands list\n"<<std::endl;
	cout<<"list\tnew\tmodify\tdelete\tprint\tpassword"<<std::endl;
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
			std::clrscr();
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
	case 11:
			change_password();
			break;
	default:
			cout<<"Unknow command (type help)!"<<std::endl;

	}
}




/*!
 * function to change the main password of the user...
 */
bool Console::change_password(){
	return true;
}



/*!
 * test command function
 * dispatch call functions
 * @param cmdl
 * @param value
 */
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
cmd=(cmdl.compare("PASSWORD")==0)?11:cmd;

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




/*!
 *function that ask to identifying you if needed
 * @return
 */
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




/*!
 *Create a new credential if not exist enough...
 * @return
 */
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
