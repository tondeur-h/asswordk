/*
 * awkBlowfish.h
 *
 *  Created on: 17 mai 2014
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

#ifndef AWKBLOWFISH_H_
#define AWKBLOWFISH_H_

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include <cstdlib>
using std::exit;



namespace ABlowfish {

/*!
 * This class is usefull for crypting things
 * this class used cryptopp library
 * for easily crypt and decrypt sentence with Blowfish algorithm
 * and implement old md5 hash and sha512 hash algorithms
 */
class awkBlowfish {
public:
	awkBlowfish();
	string crypt(string plain, string key); //crypt with blowfish algo
	string ciphertoString(string cipher); //convert cipher format to C++ string
	string stringtoCipher(string str); //convert C++ string to cipher format
	string decrypt(string cipher, string key); //decrypt with blowfish algo
	string md5(string message); //old md5 hash
	string sha(string message); //sha512 hash
	virtual ~awkBlowfish();
};

} /* namespace ABlowfish */

#endif /* AWKBLOWFISH_H_ */
