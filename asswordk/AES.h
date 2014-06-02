/*
 * AES.h
 *
 *  Created on: 27 mai 2014
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

#ifndef AES_H_
#define AES_H_

#include <string>
using std::string;

#include "cryptopp/modes.h"
#include "cryptopp/aes.h"
#include "cryptopp/filters.h"
using CryptoPP::StringSource;
using CryptoPP::StringSink;

#include "cryptopp/hex.h"
using CryptoPP::HexDecoder;
using CryptoPP::HexEncoder;


namespace AAES {

/*!
 *this class allows to crypt and decrypt a plaintext with a key string
 *the iv is initialize in hard coding at this time
 */
class AES {
public:
	/*
	 * to know:
	 * cipher format is a byte(char redefine) array as (byte*)
	 */

	AES(); //initiate de IV
	string crypt(std::string plaintext, std::string key); //crypt a plaintext with the key
	string decrypt(string cipher, string key); //decrypt a cipher format with the key
	string ciphertoString(string cipher); //convert cipher format to string
	string stringtoCipher(string str); //convert string format to cipher
	virtual ~AES();
};

} /* namespace AAES */

#endif /* AES_H_ */
