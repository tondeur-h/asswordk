/*
 * DBFile.cpp
 *
 *  Created on: 18 mai 2014
 *      Author: Tondeur Herve
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

#include "DBFile.h"

namespace pawk {

/*!
 *get current date time
 * @return
 */
const std::string DBFile::currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}



/*!
 * constructor of this class do nothing
 */
DBFile::DBFile(){dbfile=0;}


/*!
 * open as reading and write a db file
 * @param pathDB
 * @return bool if ok
 */
bool DBFile::openForReadWrite(string pathDB){
	//open as red/write...
	dbfile=new fstream(pathDB.c_str(),std::ios::out | std::ios::in);
	//if failed then return false to indicate to créate one or indicate true if good
	if (*dbfile){ return true;}
return false;
}


/*!
 * open as reading and write a db file
 * @param pathDB
 * @return bool if ok
 */
bool DBFile::resetFile(string pathDB){
	//open as red/write...
	dbfile=new fstream(pathDB.c_str(),std::ios::out);
	//if failed then return false to indicate to créate one or indicate true if good
	if (*dbfile){ return true;}
return false;
}


/*!
 * if dbfile is open test the file format
 * must have in is first line the word
 * "#asswordk", without double quotes
 * @return
 */
bool DBFile::testFileFormat(){
//test if exist and is open
if (!*dbfile){return false;}
//test file format read first line
string firstline;
std::getline(*dbfile,firstline);


if (firstline.compare("#asswordk")==0 && !readValue("mainpassword").empty())
{
return true;
}
//default fail return
return false;
}


/*!
 * read a value for a parameter that is pass as parameter of this function
 * @param param
 * @return string as value of the parameter or empty
 */
string DBFile::readValue(string param){
//dbfile->seekg(0); //reset position in file...
string line;
string value="";

//for each line compare whith "param="
//if found, cut and return value...
while (!dbfile->eof()){
	getline(*dbfile,line);
	//std::cout<<"#--line :"<<line<<std::endl;
	string::size_type found=line.find(param,0);
	if (found!=string::npos){
		//std::cout<<"#-- found: "<<found<<std::endl;
		//cut substring value to return
		value=line.substr(param.size()+1,line.size()-(param.size()+1));
		break; //yes!, found quit while
	}
}
return value;
}


/*!
 *read all entries from dbfile, not the main password
 * @param v
 */
bool DBFile::readAll(std::vector<pawk::struct_entry>* v){
string::size_type found=string::npos;
string line;
struct_entry entry;
string id;
//seek after [BEGIN] tag into file...
dbfile->seekg(0);
//search [BEGIN]
while ((!dbfile->eof()) && (found!=string::npos)){
	getline (*dbfile,line);
	found=line.find("[BEGIN]",0);
}
//if end of file nothing to read so stop...
if (dbfile->eof()) {return false;}

//erase all vector entries
v->clear();

// read each entry and send them into vector
while (!dbfile->eof()){
//search id and iterate on each entry
	id=readValue("id");
	if (dbfile->eof()) {return true;}
	entry.login=readValue("login");
	entry.password=readValue("password");
	entry.url=readValue("url");
	entry.notes=readValue("notes");
	entry.container=readValue("container");
	v->push_back(entry);
}

//default return
return false;
}


/*!
 * write all entries from vector into a file
 * that doesnt write the password
 * @param v
 */
void DBFile::writeAll(std::vector<pawk::struct_entry>* v){
*dbfile<<"[BEGIN]\n";
//std::cout<<"# -- v size : "<<v->size()<<std::endl;
	for (unsigned int it=0;it<v->size();it++){
		*dbfile<<"id="<<it<<"\n";
		*dbfile<<"login="<<v->at(it).login<<"\n";
		*dbfile<<"password="<<v->at(it).password<<"\n";
		*dbfile<<"url="<<v->at(it).url<<"\n";
		*dbfile<<"notes="<<v->at(it).notes<<"\n";
		*dbfile<<"container="<<v->at(it).container<<"\n";
	}
*dbfile<<"[END]";
}


/*!
 * write the format and mainpassword
 * must be call first before writeall.
 * @param pass
 */
void DBFile::writeHeader(string pass, string hash, string ses){
*dbfile<<"#asswordk\n";
*dbfile<<"#please do not edit manually\n";
*dbfile<<currentDateTime()<<"\n\n";
*dbfile<<"hash="<<hash<<"\n";
*dbfile<<"ses="<<ses<<"\n\n";
*dbfile<<"mainpassword="<<pass<<"\n\n";
}


/*!
 *close dbfile and destroy object
 */
DBFile::~DBFile() {
dbfile->close();
delete dbfile;
}

} /* namespace pawk */
