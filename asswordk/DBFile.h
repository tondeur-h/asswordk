/*
 * DBFile.h
 *
 *  Created on: 18 mai 2014
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


#ifndef DBFILE_H_
#define DBFILE_H_

#include <string>
using std::string;

#include <iostream>
#include <fstream>
using std::fstream;

//include this for struct_entry
#include "Console.h"


namespace pawk {

/*!
 * This class manage the dbfile
 *
 */
class DBFile {

	public:
	fstream* dbfile; //declaring point of fstream

	DBFile(); //construct a filestream
	bool openForReadWrite(string pathDB); //open for read/write
	const std::string currentDateTime(); //get current date time
	bool resetFile(string pathDB);//open end emptying the file
	bool testFileFormat(); //test if file exist and right format
	string readValue(string param); //read a parameter
	bool readAll(std::vector<pawk::struct_entry>* v); //read all entries and sent into a vector + password
	void writeAll(std::vector<pawk::struct_entry>* v); //write all entries infile + password
	void writeHeader(string pass, string hash, string ses); //write file format and main password
	virtual ~DBFile(); //close streamfile
};

} /* namespace pawk */

#endif /* DBFILE_H_ */
