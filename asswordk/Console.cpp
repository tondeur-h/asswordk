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
	cout<<"Liste des entrées vide..."<<endl;
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
	cout<<"Erreur id inexistant..."<<endl;
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
cout<<"Logiciel de gestion de mots de passes"<<std::endl;
cout<<"Copyright (c) 2014 Tondeur hervé\nhttp://www.tondeurh.fr"<<std::endl;
cout<<"Les conditions d'utilisations de ce programme sont définies par les termes de la Licence Publique Générale (GPL) Version 3"<<std::endl;
cout<<"Commande \"help\" pour obtenir de l'aide..."<<endl;
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
		cout<<"aide commandes\n============================"<<std::endl;
		color("32");
		cout<<"Syntaxe : quit\n"<<std::endl;
		normal_color();
		cout<<"=>permet de quitter l'application.\n"<<std::endl;
		break;
case 2: //help help
		cout<<"aide commandes\n============================"<<std::endl;
		color("32");
		cout<<"Syntaxe : help <cmd>\n"<<std::endl;
		normal_color();
		cout<<"=>Affiche le détail de l'aide sur les commandes.\n<cmd> peut prendre les valeurs suivantes:\n"<<std::endl;
		cout<<"list\tnew\tmodify\tdelete\tprint"<<std::endl;
		cout<<"purge\tclear\thelp\thello\tquit\n"<<std::endl;
		cout<<"=>si <cmd> est vide, affiche l'aide générale.\n"<<std::endl;
		break;
case 3: //help list
		cout<<"aide commandes\n============================"<<std::endl;
		color("32");
		cout<<"Syntaxe : list [id]\n"<<std::endl;
		normal_color();
		cout<<"=>Affiche la liste des comptes.\n[id] indique le num d'id du début de la liste, si n n'est pas précisé, on affiche toutes les entrées.\n"<<std::endl;
		break;
case 4: //help new
		cout<<"aide commandes\n============================"<<std::endl;
		color("32");
		cout<<"Syntaxe : new\n"<<std::endl;
		normal_color();
		cout<<"=>Créer une nouvelle entrée.\n"<<std::endl;
		break;
case 5: //help delete
		cout<<"aide commandes\n============================"<<std::endl;
		color("32");
		cout<<"Syntaxe : delete <id>\n"<<std::endl;
		normal_color();
		cout<<"=>Supprimer une entrée dans la liste des mots de passes, id représente son numéro d'entrée.\n"<<std::endl;
		break;
case 6: //help delete
		cout<<"aide commandes\n============================"<<std::endl;
		color("32");
		cout<<"Syntaxe : modify <id>\n"<<std::endl;
		normal_color();
		cout<<"=>Modifier une entrée dans la liste des mots de passes, id représente son numéro d'entrée.\n"<<std::endl;
		break;
case 7: //help delete
		cout<<"aide commandes\n============================"<<std::endl;
		color("32");
		cout<<"Syntaxe : purge\n"<<std::endl;
		normal_color();
		cout<<"=>Supprimer toutes les entrées des mots de passes de la base.\n"<<std::endl;
		break;
case 8: //help delete
		cout<<"aide commandes\n============================"<<std::endl;
		color("32");
		cout<<"Syntaxe : clear\n"<<std::endl;
		normal_color();
		cout<<"=>Effacer l'écran.\n"<<std::endl;
		break;
case 9: //help delete
		cout<<"aide commandes\n============================"<<std::endl;
		color("32");
		cout<<"Syntaxe : hello\n"<<std::endl;
		normal_color();
		cout<<"=>Affiche le nom et le copyright de l'application.\n"<<std::endl;
		break;
case 10: //help delete
		cout<<"aide commandes\n============================"<<std::endl;
		color("32");
		cout<<"Syntaxe : print <id>\n"<<std::endl;
		normal_color();
		cout<<"=>Afficher le détail d'une entrée de la liste des mots de passes, id représente son numéro d'entrée.\n"<<std::endl;
		break;
default: //general help
	cout<<"=================================="<<std::endl;
	cout<<"Liste des commandes\n"<<std::endl;
	cout<<"list\tnew\tmodify\tdelete\tprint"<<std::endl;
	cout<<"purge\tclear\thelp\thello\tquit\n"<<std::endl;
	cout<<"détail de chaque commande utilisez : \nhelp <cmd>"<<std::endl;
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
			cout<<"Commande inconnue!"<<std::endl;

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
db=new DBFile(); //create access dbfile
}


/*!
 * read user commande from prompt
 */
void Console::cmd_loop(){
	string* line; //line to read

	//open db file...
	//if file not exist quit this function
	if (!db->openForReadWrite("/usr/share/asswordk/asswordk.db")){
	cout<<"Erreur - Fichier base de données absent..."<<endl;
	//propose to create a new one...
	//TODO

	return;
	}

	//before continuing, user must be loged...
	if (!identifying()){
	cout<<"Erreur d'identification... désolé."<<endl;
	delete db; //close the db file... and stop
	exit(3);
	}


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
cout<<"Vous devez être identifié pour continuer (mot de passe) ?";
std::getline(cin,temppass);

	//encode it in sha512
	ABlowfish::awkBlowfish sha;
	string encoded=sha.sha(temppass);

	//read encoded string in database file
	string readpass;
	//TODO

	//if the same return true else send message and return false
	if (temppass.compare(readpass)==0){
		mainpassword=temppass;
		identified=true;
		return true;
	}
	else
	{
		cout<<"mot de passe incorrect..."<<endl;
		return false;
	}
}//end test identified
return true;
}

} /* namespace pawk */
