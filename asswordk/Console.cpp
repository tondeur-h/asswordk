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

//dbfile manage
#include "DBFile.h"

//clear screen auto
#include "Timer.h"


#include "configawk.h"
using pawk::configawk;

#include "hashawk.h"
using hashawk::hashawk;

//crypto++ manager
#include "awkBlowfish.h"

//crypto++ manager
#include "AES.h"


namespace pawk {

DBFile* db; //object dbfile

//define here all global variable for config...
configawk* cfgawk;
int timeclr;
string color_list, color_print, color_error, color_hello, color_identify, color_warning, color_help, color_success;

string hash;
string ses;

bool upcase;
bool lowcase;
bool number;
bool special;
bool OL;
bool LL;


/*!
 * callback fonction that will be call when time is out
 * @param timer
 */
void clr_auto(Timer* timer)
{
   std::clrscr(); //clear screen
	timer->Stop(); //stop timer
}




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
 * generate a simple password...
 * can be ameliorate...
 * @param size
 * @return
 */
std::string Console::generate_password (long int size){

    std::string Str;
	string str_char;
	string num="0123456789";
	string up="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	string low="abcdefghijklmnopqrstuvwxyz";
	string spec="!@#$%^&*";

	//select char to use from the choose in the config file.
	if (number){str_char+=num;}
	if (upcase){str_char+=up;}
	if (lowcase){str_char+=low;}
	if (special){str_char+=spec;}

	//calculate the length of the string
	int stringLength = str_char.length() - 1;

	    srand(time(0));
	    for(long int i = 0; i < size; ++i){
	    	char letter=str_char[rand() % stringLength];
	    	if (OL && (letter=='O' || letter=='0')){
	    		//no O and zero
	    		letter=str_char[rand() % stringLength];
	    	}
	    	if (LL && (letter=='l' || letter=='1')){
	    		    		//no l and one
	    		    		letter=str_char[rand() % stringLength];
	    		    	}

	    	Str += letter;

	    }

	    return Str;
} //end function generaate_password



/*!
 * run default browser
 */
void Console::browse_entry(int id){
	string call_line="xdg-open '";
	//stop if entries is empty
	if (entries->size()<=0){std::color(color_error.c_str());cout<<gettext("No entries in this credential!")<<endl;std::normal_color();return;}

	//construct call adresses if id is ok
	if ((unsigned)id < entries->size()){
		//construct the call command line
	call_line+=entries->at(id).url;
	call_line+="' 2>/dev/null &";

		//call the application
	int retcall=std::system(call_line.c_str());
		if (retcall!=0){std::color(color_error.c_str());
			cout<<gettext("Can not running")<<" "<<entries->at(id).url<<endl;
			std::normal_color();
		} else
		{
			std::color(color_success.c_str());
			cout<<gettext("Browser opening :")<<" "<<entries->at(id).url<<endl;
			std::normal_color();
		}

	}
	else
	{
		std::color(color_warning.c_str());
		cout<<gettext("This entry")<<" "<<id<<" "<<gettext("can not be used for this operation!")<<endl;
		std::normal_color();
	}
} //end funtion browse




/*!
 * run xclip to copy password in the clipboard
 */
void Console::copy_entry_password(int id){

	//stop if entries is empty
		if (entries->size()<=0){std::color(color_error.c_str());cout<<gettext("No entries in this credential!")<<endl;std::normal_color();return;}


	//ask before do to be sure and confirm with password ask
		std::color(color_warning.c_str());
		cout<<gettext("Before i leave you to modify this entry, please login with your password...")<<endl;
		std::normal_color();


	//fail identification for security
	identified=false;
	//ask to be identified again
	if (identifying()){

			string call_line="echo '";
			int rtcall=0;

			ABlowfish::awkBlowfish a;
			AAES::AES c;

			//construct copy call xclip command line

			if ((unsigned)id < entries->size()){
			if (ses.compare("AES")==0) {
				call_line+=c.decrypt(c.stringtoCipher(entries->at(id).password), mainpassword);
			} else
			{
				call_line+=a.decrypt(a.stringtoCipher(entries->at(id).password), mainpassword);
			}

			//try with xsel....
				call_line+="' | xsel -bi 2>/dev/null";
				//cout <<"# -- Call : "<<call_line<<endl;
				rtcall=std::system(call_line.c_str());

				//if fail with xsel try xclip
				if (rtcall!=0){
					call_line="echo '";

					if (ses.compare("AES")==0) {
							call_line+=c.decrypt(c.stringtoCipher(entries->at(id).password), mainpassword);
						} else
						{
							call_line+=a.decrypt(a.stringtoCipher(entries->at(id).password), mainpassword);
						}

					call_line+="' | xclip -i -selection clipboard 2>/dev/null";
					//cout <<"# -- Call : "<<call_line<<endl;
					rtcall=std::system(call_line.c_str());
						//if fail said it to the user...
						if (rtcall!=0){
						std::color(color_error.c_str());
						cout<<gettext("Fail to copy the password in the clipboard, xsel and xclip and not installed...")<<endl;
						std::normal_color();
						return;
						} else
						{
						std::color(color_success.c_str());
						cout<<gettext("The password is copied in the clipboard...")<<endl;
						std::normal_color();
						}
				} else
				{
					std::color(color_success.c_str());
					cout<<gettext("Password is copying in the clipboard...")<<endl;
					std::normal_color();
				}

			}
			else
			{
				std::color(color_warning.c_str());
				cout<<gettext("This entry")<<" "<<id<<" "<<gettext("can not be used for this operation!")<<endl;
				std::normal_color();
			}
	}
	else
	{
		//fail identifying => panic stop all
		std::color(color_error.c_str());
		cout<<gettext("Sorry - As you fail to identifying you correctly, i refuse to copy it!")<<endl<<endl;
		std::normal_color();
		return;
	}
} //end funtion browse




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
string container;


	//ask questions about entry
	//get login cannot be empty...
	while (login.empty()){
	cout<<gettext("Login ?");
	getline(cin,login);
	if (login.empty()){std::color(color_warning.c_str());cout<<gettext("Can not be empty!")<<endl;std::normal_color();}
	}

	//get password can be empty
	cout<<gettext("Password (blank to generate one)?");
	getline(cin,password);


	if (password.length()==0){
		pwl=false;
		while(!pwl){
			cout<<gettext("Lenght of the password to generate ?");
			getline(cin,pwlenght);

			pwl=(ascii2number(pwlenght)>0) & (ascii2number(pwlenght)>=4);
			if (!pwl) {std::color(color_warning.c_str());cout<<gettext("It's not a number or is value is less than  4! retry...")<<endl;std::normal_color();}
			else
			{
				password=generate_password(ascii2number(pwlenght));
				cout<<gettext("The password generated is :")<<" "<<password<<endl;
				std::color(color_warning.c_str());
				cout<<gettext("Do not forget to note it !")<<endl;
				std::normal_color();
			}
		}
	}

	//get url cannot be empty
	while (url.empty()){
	cout<<gettext("Url or Machine name ?");
	getline(cin,url);
	if (url.empty()){std::color(color_warning.c_str());cout<<gettext("Cannot be empty!")<<endl;std::normal_color();}
	}

	//get notes can be empty
	cout<<gettext("Notes about this connection ?");
	getline(cin,notes);


	//get container can be empty
		cout<<gettext("Container class of this connection ?");
		getline(cin,container);


//save informations
	hashawk::hashawk b;
	ABlowfish::awkBlowfish a;
	AAES::AES c;

	entry.login=login;
	if (ses.compare("AES")==0) {
			entry.password=c.ciphertoString(c.crypt(password,mainpassword));
		} else
		{
			entry.password=a.ciphertoString(a.crypt(password,mainpassword));
		}
	entry.pwlenght=0;
	entry.url=url;
	entry.notes=notes;
	entry.container=container;

	//add to vector entries this entry...
	entries->push_back(entry);

	db=new DBFile(); //create access dbfile
	db->resetFile("/usr/share/asswordk/asswordk.db");
	if (hash.compare("SHA512")==0) {db->writeHeader(b.sha(mainpassword),hash,ses);} else {db->writeHeader(b.md5(mainpassword),hash,ses);}
	//write all entries left
	db->writeAll(entries);
	//close the dbfile if all is reading...
	delete db;

	//show modifications...
	std::clrscr();
	list_entry(0);
} //end function new entry




/*!
 * this function delete a entry into the credential...
 * @param id
 */
void Console::delete_entry(int id){
	//test nb entries....
	if (entries->size()>0){

		//ask before do to be sure and confirm with password ask
			std::color(color_warning.c_str());
			cout<<gettext("Before i leave you to delete this entry, please login with your password...")<<endl;
			std::normal_color();

		//fail identification for security
		identified=false;
		//ask to be identified again
		if (identifying()){
			//erase entry
			entries->erase(entries->begin()+id);
			//say ok done
			std::color(color_success.c_str());
			cout<<gettext("Delete entry")<<" "<<id<<" "<<gettext("ok...")<<endl;
			cout<<gettext("Compacting id entries...")<<endl;
			std::normal_color();
			//print list of entries left...
			list_entry(0);
			//save values
			db=new DBFile(); //create access dbfile
			db->resetFile("/usr/share/asswordk/asswordk.db");
			hashawk::hashawk a;
			if (hash.compare("SHA512")==0) {db->writeHeader(a.sha(mainpassword),hash,ses);} else {db->writeHeader(a.md5(mainpassword),hash,ses);}
			//write all entries left
			db->writeAll(entries);
			//close the dbfile if all is reading...
			delete db;
		}
		else
		{
			//fail identifying => panic stop all
			std::color(color_error.c_str());
			cout<<gettext("Sorry - As you fail to identifying you correctly, i refuse to delete it!")<<endl<<endl;
			std::normal_color();
			return;
		}
	} //end test number entries
	else
	{
		std::color(color_error.c_str());
		cout<<gettext("No remaining entries in the credential, impossible satisfy your ask...")<<endl;
		std::normal_color();
	}
} //end delete function




/*!
 * function to list all the entry from db,
 * just show login@url/computer
 * @param id
 */
void Console::list_entry(unsigned int max_id){
//cout<<"#-- max id :"<<max_id<<endl;
int count_line;

count_line=0;

for(unsigned int nbl=0;nbl<entries->size();nbl++){
if (nbl>=max_id){
	std::color(color_list.c_str());
	cout<<"id "<<nbl<<" - "<<entries->at(nbl).login<<"@"<<entries->at(nbl).url<<" // "<<entries->at(nbl).container<<endl;
	count_line++;
	//***********************manage pages******************
		if(count_line>=20){
			std::normal_color();
			cout<<gettext("Press \"ENTER KEY\" for next page.")<<endl;
			cin.ignore(1);
			std::clrscr();
			count_line=0;
		}
	//*****************************************************
}
}
//return to normal color
std::normal_color();
//if entries vector is empty then signal to user.
if (entries->size()==0){
	std::color(color_warning.c_str());
	cout<<gettext("Entries list is empty...")<<endl;
	std::normal_color();
}

} //end list function



/*!
 * function to list all the entry from db,
 * just show login@url/computer
 * @param id
 */
void Console::listcont_entry(std::string ctn){

int count_line;
string container;
string::size_type found_container;


count_line=0;

for(unsigned int nbl=0;nbl<entries->size();nbl++){

	//search container match...
	container=entries->at(nbl).container;
	found_container=container.find(ctn);

	//cout<<"#-- id :"<<nbl<<" "<<container<<endl;

	//OK FOUND so list
	if (found_container!=string::npos){
	std::color(color_list.c_str());
	cout<<"id "<<nbl<<" - "<<entries->at(nbl).login<<"@"<<entries->at(nbl).url<<" // "<<entries->at(nbl).container<<endl;
	count_line++;
	/***********************manage pages******************/
		if(count_line>=20){
			std::normal_color();
			cout<<gettext("Press \"ENTER KEY\" for next page.")<<endl;
			cin.ignore(1);
			std::clrscr();
			count_line=0;
		}
	/*****************************************************/
}
}
//return to normal color
std::normal_color();
//if entries vector is empty then signal to user.
if (entries->size()==0){
	std::color(color_warning.c_str());
	cout<<gettext("Entries list is empty...")<<endl;
	std::normal_color();
}

} //end list function




/*!
 * duplicate an entry and get to modify it...
 * @param id
 */
void Console::duplicate_entry(int id){

	//test id if exist in entries list
	if ((unsigned int)id>(entries->size())){
		std::color(color_error.c_str());
		cout<<gettext("This entry id does'nt exist...!")<<endl;
		std::normal_color();
		return;
	}

	entry=entries->at(id);
	int num_last=entries->size();
	entries->push_back(entry);
	std::color(color_success.c_str());
	cout<<gettext("Duplicate entry is done...")<<endl;
	std::normal_color();

	modify_entry(num_last);
} //end function duplicate


/*!
 * function that help to modify en entry if exist...
 * @param id
 */
void Console::modify_entry(int id){
	//local entry variables
	string login;
	string password;
	string notes;
	string url;
	string container;


	//test id if exist in entries list
	if ((unsigned int)id>(entries->size())){
		std::color(color_error.c_str());
		cout<<gettext("This entry id does'nt exist...!")<<endl;
		std::normal_color();
		return;
	}

	//ask before do to be sure and confirm with password ask
			std::color(color_warning.c_str());
			cout<<gettext("Before i leave you to modify this entry, please login with your password...")<<endl;
			std::normal_color();

		//fail identification for security
		identified=false;
		//ask to be identified again
			if (identifying()){
		//get entry from entries
		entry=entries->at(id);

			//ask questions about entry
			//get login cannot be empty...
			cout<<gettext("Login, keep blank for no change")<<" ("<<entry.login<<")?";
			getline(cin,login);
			if (login.empty()){login=entry.login;}



			//get password can be empty
			ABlowfish::awkBlowfish a;
			AAES::AES c;
			hashawk::hashawk b;

			if (ses.compare("AES")==0){
				cout<<gettext("Password, keep blank for no change")<<" ("<<c.decrypt(c.stringtoCipher(entry.password),mainpassword)<<")?";
			} else
			{
				cout<<gettext("Password, keep blank for no change")<<" ("<<a.decrypt(a.stringtoCipher(entry.password),mainpassword)<<")?";
			}

			getline(cin,password);

			if (ses.compare("AES")==0){
				if (password.empty()){password=c.decrypt(c.stringtoCipher(entry.password),mainpassword);}
			} else
			{
				if (password.empty()){password=a.decrypt(a.stringtoCipher(entry.password),mainpassword);}
			}

			//get url cannot be empty
			cout<<gettext("Url or Machine, keep blank for no change")<<" ("<<entry.url<<")?";
			getline(cin,url);
			if (url.empty()){url=entry.url;}


			//get notes can be empty
			cout<<gettext("Notes, keep blank for no change")<<" ("<<entry.notes<<")?";
			getline(cin,notes);
			if (notes.empty()){notes=entry.notes;}


			//get container can be empty
			cout<<gettext("Container, keep blank for no change")<<" ("<<entry.container<<")?";
			getline(cin,container);
			if (container.empty()){container=entry.container;}


		//save informations

			entry.login=login;
			if (ses.compare("AES")==0){
				entry.password=c.ciphertoString(c.crypt(password,mainpassword));
			} else
			{
				entry.password=a.ciphertoString(a.crypt(password,mainpassword));
			}

			entry.pwlenght=0;
			entry.url=url;
			entry.notes=notes;
			entry.container=container;

			//change entry in the vector entries...
			entries->at(id)=entry;

			db=new DBFile(); //create access dbfile
			db->resetFile("/usr/share/asswordk/asswordk.db");
			if (hash.compare("SHA512")==0) {db->writeHeader(b.sha(mainpassword),hash,ses);} else {db->writeHeader(b.md5(mainpassword),hash,ses);}
			//write all entries left
			db->writeAll(entries);
			//close the dbfile if all is reading...
			delete db;

			//show modifications...
			list_entry(0);
		}
		else
		{
			//fail identifying => panic stop all
			std::color(color_error.c_str());
			cout<<gettext("Sorry - As you fail to identifying you correctly, i refuse to delete it!")<<endl<<endl;
			std::normal_color();
			return;
		}
} //end modify function




/*!
 * list all entries that contains your search string in the login field.
 * @param search_login
 */
void Console::search_entries(string search_str){
//define usefull string for keep in memory the current entry.
string login;
string::size_type found_login;
string url;
string::size_type found_url;
string notes;
string::size_type found_notes;
string container;
string::size_type found_container;


//int represent the number of found lines...
int nb_found=0;

//test if search_entry not empty, if is it so quit
	if (search_str.empty()){
		std::color(color_error.c_str());
		cout<<gettext("You can not search for an empty string...Sorry!")<<endl;
		std::normal_color();
		return;
	}
//for each entry in the vector print this that match

	for (unsigned int it=0;it<entries->size();it++){
		//read vector.login.url.notes
		login=entries->at(it).login;
		url=entries->at(it).url;
		notes=entries->at(it).notes;
		container=entries->at(it).container;

		found_login=login.find(search_str);
		found_url=url.find(search_str);
		found_notes=notes.find(search_str);
		found_container=container.find(search_str);

		if (found_login!=string::npos || found_url!=string::npos || found_notes!=string::npos || found_container!=string::npos){
			//OK FOUND..
			std::color(color_list.c_str());
			cout<<"id "<<it<<" - "<<entries->at(it).login<<"@"<<entries->at(it).url<<" // "<<entries->at(it).container<<endl;
			nb_found++;
			std::normal_color();
		} //end if
	} //end for

	//give an answers
	if (nb_found==1){
		std::color(color_success.c_str());
		cout<<nb_found<<" "<<gettext("entry that match your search string")<<" \""<<search_str<<"\"."<<endl;
		std::normal_color();
	}
	if (nb_found>1) {
		std::color(color_success.c_str());
		cout<<nb_found<<" "<<gettext("entries that match your search string")<<" \""<<search_str<<"\"."<<endl;
		std::normal_color();
	}
	if (nb_found==0)
	{
		std::color(color_warning.c_str());
		cout<<gettext("No entry match your search string...!")<<endl;
		std::normal_color();
	}
}//end function search




/*!
 * Show un full entry wich pass is id
 * @param id
 */
void Console::print_entry(int id){
//cout<<"#-- view id :"<<id<<endl;

//if id is ok then show all
if ((unsigned int)id<(entries->size())){

	//ask before do to be sure and confirm with password ask
	std::color(color_warning.c_str());
	cout<<gettext("Before i leave you to modify this entry, please login with your password...")<<endl;
	std::normal_color();


	//fail identification for security
	identified=false;
	//ask to be identified again
	if (identifying()){

			std::color(color_print.c_str());
			cout<<"ID : "<<id<<endl;
			cout<<"Login : "<< entries->at(id).login<<endl;

			ABlowfish::awkBlowfish crypto;
			AAES::AES cryptoA;

			//decrypt password for user
			if (ses.compare("AES")==0){
				cout<<"Password : "<< cryptoA.decrypt(cryptoA.stringtoCipher(entries->at(id).password),mainpassword)<<endl;
			} else
			{
				cout<<"Password : "<< crypto.decrypt(crypto.stringtoCipher(entries->at(id).password),mainpassword)<<endl;
			}

			cout<<"Url :"<<entries->at(id).url<<endl;
			cout<<"Notes :"<<entries->at(id).notes<<endl;
			cout<<"Container : "<<entries->at(id).container<<endl;
			std::normal_color();

			//create the timer for auto clean screen
			Timer clr_timer(timeclr*1000,clr_auto);
			//waiting loop
			while(clr_timer.IsActive())clr_timer.Probe();

	}
	else
	{
		//fail identifying => panic stop all
		std::color(color_error.c_str());
		cout<<gettext("Sorry - As you fail to identifying you correctly, i refuse to print it!")<<endl<<endl;
		std::normal_color();
		return;
	}

} else
{
	//oops id not right
	std::color(color_error.c_str());
	cout<<gettext("Error, id not found...")<<endl;
	std::normal_color();
}
} //end print function




/*!
 * purge all the entries into the entries vector and save the file in disk
 */
void Console::purge(){
//ask before do to be sure and confirm with password ask
	std::color(color_warning.c_str());
	cout<<gettext("Before i leave you to purge all entries, please login with your password...")<<endl;
	std::normal_color();

//fail identification for security
identified=false;
//ask to be identified again
if (identifying()){
	//right clear all entries
	entries->clear();

	db=new DBFile(); //create access dbfile
	db->openForReadWrite("/usr/share/asswordk/asswordk.db");
	hashawk::hashawk a;
	if (hash.compare("SHA512")==0) {db->writeHeader(a.sha(mainpassword),hash,ses);} else {db->writeHeader(a.md5(mainpassword),hash,ses);}
	//close the dbfile if all is reading...
	delete db;

	std::color(color_success.c_str());
	cout<<gettext("OK - Purge is done!")<<endl<<endl;
	std::normal_color();
}
else
{
	//fail identifying => panic stop all
	std::color(color_error.c_str());
	cout<<gettext("Sorry - As you fail to identifying you correctly, i stop working...")<<" :(!"<<endl<<endl;
	std::normal_color();
	exit(3);
}
} //end purge function




/*!
 * print hello header from the application
 */
void Console::hello(){
std::clrscr();
color(color_hello.c_str());
cout<<"asswordk version 0.3"<<endl;
normal_color();

cout<<gettext("Passwords Manager Application")<<endl;
cout<<"Copyright (c) 2014 Tondeur hervé\nhttp://www.tondeurh.fr"<<endl;
cout<<gettext("Terms of use of this program are defined by the General Public Licence (GPL) Version 3 <http://gnu.org/licenses/gpl.html>")<<endl;
cout<<gettext("This is free software: you are free to change and redistribute it.")<<endl;
cout<<gettext("There is NO WARRANTY, to the extent permitted by law.")<<endl;
cout<<gettext("Type command \"help\" to get help...")<<endl;
cout<<endl;
}




/*!
 * method that print general help and specific help
 * @param value
 */
void Console::help(string value){
	//cout<<"#-- "<<value<<endl;

int cmd=0;
value=std::ucase(value);
//convert parameter for help action into integer value
cmd=(value.compare("QUIT")==0)?1:cmd;
cmd=(value.compare("EXIT")==0)?1:cmd;
cmd=(value.compare("HELP")==0)?2:cmd;
cmd=(value.compare("LIST")==0)?3:cmd;
cmd=(value.compare("LS")==0)?3:cmd;
cmd=(value.compare("NEW")==0)?4:cmd;
cmd=(value.compare("ADD")==0)?4:cmd;
cmd=(value.compare("DELETE")==0)?5:cmd;
cmd=(value.compare("DEL")==0)?5:cmd;
cmd=(value.compare("MODIFY")==0)?6:cmd;
cmd=(value.compare("MOD")==0)?6:cmd;
cmd=(value.compare("PURGE")==0)?7:cmd;
cmd=(value.compare("PG")==0)?7:cmd;
cmd=(value.compare("CLEAR")==0)?8:cmd;
cmd=(value.compare("CLR")==0)?8:cmd;
cmd=(value.compare("HELLO")==0)?9:cmd;
cmd=(value.compare("PRINT")==0)?10:cmd;
cmd=(value.compare("P")==0)?10:cmd;
cmd=(value.compare("PASSWORD")==0)?11:cmd;
cmd=(value.compare("PW")==0)?11:cmd;
cmd=(value.compare("SEARCH")==0)?12:cmd;
cmd=(value.compare("SH")==0)?12:cmd;
cmd=(value.compare("BROWSE")==0)?13:cmd;
cmd=(value.compare("BR")==0)?13:cmd;
cmd=(value.compare("COPY")==0)?14:cmd;
cmd=(value.compare("CP")==0)?14:cmd;
cmd=(value.compare("DUPLICATE")==0)?15:cmd;
cmd=(value.compare("DUP")==0)?15:cmd;
cmd=(value.compare("LISTCONT")==0)?16:cmd;
cmd=(value.compare("LC")==0)?16:cmd;


switch (cmd){
case 1://help quit
		cout<<gettext("Commands help")<<endl<<"============================"<<endl;
		color(color_help.c_str());
		cout<<gettext("Syntax")<<" : quit(exit)"<<endl<<endl;
		normal_color();
		cout<<"=>"<<gettext("Type this command with no parameter to quit this application.")<<endl<<endl;
		break;
case 2: //help help
		cout<<gettext("Commands help")<<endl<<"============================"<<endl;
		color(color_help.c_str());
		cout<<gettext("Syntax")<<" : help <cmd>"<<endl<<endl;
		normal_color();
		cout<<"=>"<<gettext("Show details of a help command.")<<endl;
		cout<<gettext("<cmd> can take one of this values following :")<<endl<<endl;
		cout<<"list(ls)\tlistcont(lc)\tnew(add)\tmodify(mod)\tdelete(del)\tprint(p)\tpassword(pw)"<<endl;
		cout<<"duplicate(dup)\tpurge(pg)\tsearch(sh)\tbrowse(br)\tcopy(cp)\tclear(clr)\tquit(exit)"<<endl;
		cout<<"help\thello"<<endl<<endl;

		cout<<"=>"<<gettext("If <cmd> is empty, show general help.")<<endl<<endl;
		break;
case 3: //help list
		cout<<gettext("Commands help")<<endl<<"============================"<<endl;
		color(color_help.c_str());
		cout<<gettext("Syntax")<<" : list(ls) [id]"<<endl<<endl;
		normal_color();
		cout<<"=>"<<gettext("Print the list of credentials.")<<endl;
		cout<<gettext("[id] point the credential number to begin printing, if id is not set, all credentials entries are printing.")<<endl;
		cout<<gettext("ls is the short command to call list")<<endl<<endl;
		break;
case 4: //help new
		cout<<gettext("Commands help")<<endl<<"============================"<<endl;
		color(color_help.c_str());
		cout<<gettext("Syntax")<<" : new(add)\n"<<endl;
		normal_color();
		cout<<"=>"<<gettext("This command allows you to create a new credential entry.add is the short command.")<<endl<<endl;
		break;
case 5: //help delete
		cout<<gettext("Commands help")<<endl<<"============================"<<endl;
		color(color_help.c_str());
		cout<<gettext("Syntax")<<" : delete(del) <id>"<<endl<<endl;
		normal_color();
		cout<<"=>"<<gettext("Delete a credential entry from the list of credentials, id must be the number of the credential to delete.")<<endl;
		cout<<gettext("Use list command before to get the id number.")<<endl;
		cout<<gettext("If id is ommited, assume 0.")<<endl;
		cout<<gettext("You can Use the short command del as well.")<<endl;
		cout<<gettext("You must identifying yourself before deleting entry.")<<endl;
		break;
case 6: //help modify
		cout<<gettext("Commands help")<<endl<<"============================"<<endl;
		color(color_help.c_str());
		cout<<gettext("Syntax")<<" : modify(mod) <id>"<<endl<<endl;
		normal_color();
		cout<<"=>"<<gettext("Modify an entry from the credentials list, id must be the number of the credential to modify.")<<endl;
		cout<<gettext("Use list command before to get the id number.")<<endl;
		cout<<gettext("You must identifying yourself before.")<<endl;
		break;
case 7: //help purge
		cout<<gettext("Commands help")<<endl<<"============================"<<endl;
		color(color_help.c_str());
		cout<<gettext("Syntax")<<" : purge(pg)"<<endl<<endl;
		normal_color();
		cout<<"=>"<<gettext("Delete all entries from credential.")<<endl;
		cout<<gettext("Be carefull evething will be lost.")<<endl;
		cout<<gettext("You must identifying yourself before.")<<endl;
		break;
case 8: //help clear
		cout<<gettext("Commands help")<<endl<<"============================"<<endl;
		color(color_help.c_str());
		cout<<gettext("Syntax")<<" : clear(clr)"<<endl<<endl;
		normal_color();
		cout<<"=>"<<gettext("Clean screen for confidentiality.")<<endl<<endl;
		break;
case 9: //help hello
		cout<<gettext("Commands help")<<endl<<"============================"<<endl;
		color(color_help.c_str());
		cout<<gettext("Syntax")<<" : hello"<<endl<<endl;
		normal_color();
		cout<<"=>"<<gettext("Show application name and copyright.")<<endl<<endl;
		break;
case 10: //help print
		cout<<gettext("Commands help")<<endl<<"============================"<<endl;
		color(color_help.c_str());
		cout<<gettext("Syntax")<<" : print(p) <id>"<<endl<<endl;
		normal_color();
		cout<<"=>"<<gettext("Print the detail of an entry from the credentials, id must be the number of the credential to print.")<<endl;
		cout<<gettext("Use list command before to get the id number.")<<endl;
		cout<<gettext("Care that screen will be clean in about 5 secondes after printing...")<<endl;
		cout<<gettext("You must identifying yourself before printing entry.")<<endl;
		break;
case 11: //help password
		cout<<gettext("Commands help")<<endl<<"============================"<<endl;
		color(color_help.c_str());
		cout<<gettext("Syntax")<<" : password(pw)"<<endl<<endl;
		normal_color();
		cout<<"=>"<<gettext("Change the main password.")<<endl<<endl;
		break;
case 12: //help search
		cout<<gettext("Commands help")<<endl<<"============================"<<endl;
		color(color_help.c_str());
		cout<<gettext("Syntax")<<" : search(sh) <login>"<<endl<<endl;
		normal_color();
		cout<<"=>"<<gettext("List all the credential that login or url or notes, match exactly with your search string.")<<endl<<endl;
		break;
case 13: //help browse
		cout<<gettext("Commands help")<<endl<<"============================"<<endl;
		color(color_help.c_str());
		cout<<gettext("Syntax")<<" : browse(br) <id>"<<endl<<endl;
		normal_color();
		cout<<"=>"<<gettext("Open the url with the default browser of the choosen credential (if possible).")<<endl;
		cout<<gettext("If id is not set, 0 is assume.")<<endl;
		cout<<gettext("You must have xdg-open installed on you system.")<<endl<<endl;
		break;
case 14: //help copy
		cout<<gettext("Commands help")<<endl<<"============================"<<endl;
		color(color_help.c_str());
		cout<<gettext("Syntax")<<" : copy(cp) <id>"<<endl<<endl;
		normal_color();
		cout<<"=>"<<gettext("Copy the password of the application in the X clipboard if xclip application is installed (if possible).")<<endl;
		cout<<gettext("If id is not set, 0 is assume.")<<endl;
		cout<<gettext("You must identifying yourself before copy a password.")<<endl;
		cout<<gettext("You must have xsel or xclip installed on your system.")<<endl<<endl;
		break;
case 15: //help duplicate
		cout<<gettext("Commands help")<<endl<<"============================"<<endl;
		color(color_help.c_str());
		cout<<gettext("Syntax")<<" : duplicate(dup) <id>"<<endl<<endl;
		normal_color();
		cout<<"=>"<<gettext("Duplicate an entry and let you to modify it.")<<endl;
		cout<<gettext("If id is not set, 0 is assume.")<<endl<<endl;
		break;
case 16: //help list CONT
		cout<<gettext("Commands help")<<endl<<"============================"<<endl;
		color(color_help.c_str());
		cout<<gettext("Syntax")<<" : listcont(lc) [container string]"<<endl<<endl;
		normal_color();
		cout<<"=>"<<gettext("Print the list of credentials.")<<endl;
		cout<<gettext("[id] point the credential container string to sort, if string is not set, all credentials entries are listed.")<<endl;
		cout<<gettext("lc is the short command to call list")<<endl<<endl;
		break;
default: //general help
	cout<<"=================================="<<endl;
	cout<<gettext("Commands list")<<endl<<endl;
	cout<<"list(ls)\tlistcont(lc)\tnew(add)\tmodify(mod)\tdelete(del)\tprint(p)\tpassword(pw)"<<endl;
	cout<<"duplicate(dup)\tpurge(pg)\tsearch(sh)\tbrowse(br)\tcopy(cp)\tclear(clr)\tquit(exit)"<<endl;
	cout<<"help\thello"<<endl<<endl;
	cout<<"=>"<<gettext("To get details on a command use :")<<endl;
	cout<<gettext("help <cmd>, where cmd is the name of a command.")<<endl<<endl;
}
}//end function hello




/*!
 * switch routine for select function from commande ask
 * @param cmdi as input int to simplify switch selection
 * @param value string parameter
 */
void Console::run_cmd(int cmdi,string value){

	switch (cmdi){
	case 1:
			std::clrscr();
			std::color(color_success.c_str());
			cout<<gettext("bye!")<<endl;
			std::normal_color();
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
	case 12:
			search_entries(value);
			break;
	case 13:
			browse_entry(std::atoi(value.c_str()));
			break;
	case 14:
			copy_entry_password(std::atoi(value.c_str()));
			break;
	case 15:
			duplicate_entry(std::atoi(value.c_str()));
			break;
	case 16:
			listcont_entry(value.c_str());
			break;
	default:
			cout<<gettext("Unknow command (type help)!")<<endl;

	}
}




/*!
 * function to change the main password of the user...
 */
bool Console::change_password(){
	string pass1,pass2;
	//ask to be identified
	//break identified relation...

	cout<<gettext("You wish to change your main password...")<<endl<<endl;
	identified=false;
	//ask again now
	if (!identifying()){identified=true;return false;}

	//ask new password
	cout<<gettext("Please enter a new password for protect your credentials : (enter a password follow by the enter key)?");
	pass1=std::getpass();
		//if pass1 is empty stop
		if (pass1.empty()){
			std::color(color_error.c_str());
			cout<<gettext("This new password cannot be empty!...")<<endl;
			std::normal_color();
			return false;
		}

	//confirm it
	cout<<gettext("Please confirm your password : (enter a password follow by the enter key)?");
	pass2=std::getpass();
		//if pass1!=pass2 stop
		if (pass1.compare(pass2)!=0){
			std::color(color_error.c_str());
		cout<<gettext("Your confirm password is not the same!...")<<endl;
		std::normal_color();
		return false;
	}

		//hash in sha512 this password
		hashawk::hashawk a;
		string encoded;
		if (hash.compare("SHA512")==0) {encoded=a.sha(pass1);} else {encoded=a.md5(pass1);}
		mainpassword=pass1;
		identified=true;


		//create the dbfile ans save coded password
		db=new pawk::DBFile();
		db->openForReadWrite("/usr/share/asswordk/asswordk.db");
		db->writeHeader(encoded,hash,ses); //write header for mainpassword
		db->writeAll(entries); //save all credentials too
		delete db;

		//success
		std::color(color_success.c_str());
		cout<<gettext("Your new password has been successfully changed, use this one next time.")<<endl;
		std::normal_color();
	return true;
}//end function change password




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
cmd=(cmdl.compare("EXIT")==0)?1:cmd;
cmd=(cmdl.compare("HELP")==0)?2:cmd;
cmd=(cmdl.compare("LIST")==0)?3:cmd;
cmd=(cmdl.compare("LS")==0)?3:cmd;
cmd=(cmdl.compare("NEW")==0)?4:cmd;
cmd=(cmdl.compare("ADD")==0)?4:cmd;
cmd=(cmdl.compare("DELETE")==0)?5:cmd;
cmd=(cmdl.compare("DEL")==0)?5:cmd;
cmd=(cmdl.compare("MODIFY")==0)?6:cmd;
cmd=(cmdl.compare("MOD")==0)?6:cmd;
cmd=(cmdl.compare("PURGE")==0)?7:cmd;
cmd=(cmdl.compare("PG")==0)?7:cmd;
cmd=(cmdl.compare("CLEAR")==0)?8:cmd;
cmd=(cmdl.compare("CLR")==0)?8:cmd;
cmd=(cmdl.compare("HELLO")==0)?9:cmd;
cmd=(cmdl.compare("PRINT")==0)?10:cmd;
cmd=(cmdl.compare("P")==0)?10:cmd;
cmd=(cmdl.compare("PASSWORD")==0)?11:cmd;
cmd=(cmdl.compare("PW")==0)?11:cmd;
cmd=(cmdl.compare("SEARCH")==0)?12:cmd;
cmd=(cmdl.compare("SH")==0)?12:cmd;
cmd=(cmdl.compare("BROWSE")==0)?13:cmd;
cmd=(cmdl.compare("BR")==0)?13:cmd;
cmd=(cmdl.compare("COPY")==0)?14:cmd;
cmd=(cmdl.compare("CP")==0)?14:cmd;
cmd=(cmdl.compare("DUPLICATE")==0)?15:cmd;
cmd=(cmdl.compare("DUP")==0)?15:cmd;
cmd=(cmdl.compare("LISTCONT")==0)?16:cmd;
cmd=(cmdl.compare("LC")==0)?16:cmd;


run_cmd(cmd,value);
}




/*!
 * create interactive console
 * and initialize variables needed
 */
Console::Console() {
read_config_file(); //read config file before running
hello(); //print hello message
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
		std::color(color_error.c_str());
		cout<<gettext("Error - Database file not found...")<<endl;
		cout<<gettext("You must create a database file, see documentation...")<<endl;
		std::normal_color();
		delete db;
		return;
	}

	//test if mainpassword and header ok...
	if (!db->testFileFormat()){
		std::color(color_success.c_str());
		cout<<gettext("Credential is empty, create one please...")<<endl;
		std::normal_color();
		delete db;

		//propose to create a new one...
		if 	(!create_credential()){return;}
	}

	//test and set HASH and SES encryption
		db=new pawk::DBFile();
		db->openForReadWrite("/usr/share/asswordk/asswordk.db");
		string readpass;
		hash=db->readValue("hash");
		ses=db->readValue("ses");
		delete db;
		cout<<gettext("Database encryption format is")<<" "<<hash<<"/"<<ses<<endl<<endl;


	//before continuing, user must be loged...
	if (!identifying()){
	cout<<"."<<endl;
	//bye bye
	exit(3);
	} else
	{//say hello
		std::color(color_hello.c_str());
		cout<<gettext("Hello - you're welcome!")<<endl;
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
}//end function cmd_loop




/*!
 * nothing to do
 */
Console::~Console() {
delete entries; //delete vector for all database...
//delete db;
} //end destructor




/*!
 *function that ask to identifying you if needed
 * @return
 */
bool Console::identifying(){
if (!identified){
string temppass;
	//ask password
std::color(color_identify.c_str()); //inverse video
cout<<gettext("You must be identified to continue (enter your password follow by the enter key) ?");
std::normal_color(); //reset

temppass=std::getpass();


	//encode it in sha512
	hashawk::hashawk a;
	string encoded;

	if (hash.compare("SHA512")==0) {encoded=a.sha(temppass);} else {encoded=a.md5(temppass);}

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
		std::color(color_error.c_str());
		cout<<gettext("This password is not correct...")<<endl;
		std::normal_color();
		return false;
	}
}//end test identified
return true;
} //end function identifying




/*!
 *Create a new credential if not exist enough...
 * @return
 */
bool Console::create_credential(){
	string pass1, pass2;
//ask for a password
cout<<gettext("Please enter a password for protect your new credentials : (enter a password follow by the enter key)?");
pass1=std::getpass();
	//if pass1 is empty stop
	if (pass1.empty()){
		std::color(color_error.c_str());
		cout<<gettext("This password cannot be empty!...")<<endl;
		std::normal_color();
		return false;
	}

//confirm it
cout<<gettext("Please confirm your password : (enter a password follow by the enter key)?");
pass2=std::getpass();
	//if pass1!=pass2 stop
	if (pass1.compare(pass2)!=0){
		std::color(color_error.c_str());
	cout<<gettext("Your confirm password is not the same!...")<<endl;
	std::normal_color();
	return false;
}

	//hash in sha512 this password
	hashawk::hashawk a;
	string encoded;
	if (hash.compare("SHA512")==0) {encoded=a.sha(pass1);} else {encoded=a.md5(pass1);}


	//create the dbfile ans save coded password
	db=new pawk::DBFile();
	db->openForReadWrite("/usr/share/asswordk/asswordk.db");
	db->writeHeader(encoded,hash,ses);
	delete db;
	//default return...
	return true;
}//end function create_credential




/*!
 *Read config file if exist
 *otherwise set default values...
 * @return
 */
bool Console::read_config_file(){
	//default values setting...
	timeclr=5;

	//create config object
	cfgawk=new configawk();
	//expand home directory...
	string home=getenv ("HOME");
	if (!cfgawk->init_cfg(home +"/.asswordk.cfg")) {return false;}
	if (!cfgawk->verify_version()) {return false;}


	//get root from the config file
	const libconfig::Setting& root = cfgawk->cfg.getRoot();

	//read config time clearscreen
	timeclr=cfgawk->read_config_int(root,"misc","clrscr",5);

	//read colors
	color_identify=cfgawk->read_config_string(root,"colors","identify","37;7");
	color_list=cfgawk->read_config_string(root,"colors","list","35");
	color_error=cfgawk->read_config_string(root,"colors","error","31");
	color_warning=cfgawk->read_config_string(root,"colors","warning","33;7");
	color_success=cfgawk->read_config_string(root,"colors","success","32");
	color_print=cfgawk->read_config_string(root,"colors","print","32");
	color_hello=cfgawk->read_config_string(root,"colors","hello","32;7");
	color_help=cfgawk->read_config_string(root,"colors","help","36");

	//read encryption
	hash=cfgawk->read_config_string(root,"encryption","hash","SHA512");
	ses=cfgawk->read_config_string(root,"encryption","ses","AES");


	//read password rules
	upcase=cfgawk->read_config_bool(root,"password","upcase",true);
	lowcase=cfgawk->read_config_bool(root,"password","lowcase",true);
	number=cfgawk->read_config_bool(root,"password","number",true);
	special=cfgawk->read_config_bool(root,"password","special",true);
	OL=cfgawk->read_config_bool(root,"password","OL",false);
	LL=cfgawk->read_config_bool(root,"password","LL",false);

	//destroy config object to free memory...
	delete cfgawk;
	return true;
}

} /* namespace pawk */

//End Of File
