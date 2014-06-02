/*
 * AES.cpp
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


#include "AES.h"

namespace AAES {

byte iv[ CryptoPP::AES::BLOCKSIZE ];


AES::AES() {
	// IV setup

		iv[0]=34;
		iv[1]=45;
		iv[2]=67;
		iv[3]=12;
		iv[4]=78;
		iv[5]=87;
		iv[6]=98;
		iv[7]=23;
}

AES::~AES() {}



string AES::crypt(string plaintext, string key){
string ciphertext;


CryptoPP::AES::Encryption aesEncryption((byte*)key.c_str(), CryptoPP::AES::DEFAULT_KEYLENGTH);
CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption( aesEncryption, iv );

CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink( ciphertext ) );
stfEncryptor.Put( reinterpret_cast<const unsigned char*>( plaintext.c_str() ), plaintext.length() + 1 );
stfEncryptor.MessageEnd();

return ciphertext;
}


string AES::decrypt(string cipher, string key){
string decryptedtext;

CryptoPP::AES::Decryption aesDecryption((byte*)key.c_str(), CryptoPP::AES::DEFAULT_KEYLENGTH);
CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption( aesDecryption, iv );

CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink( decryptedtext ) );
stfDecryptor.Put( reinterpret_cast<const unsigned char*>( cipher.c_str() ), cipher.size() );
stfDecryptor.MessageEnd();

return decryptedtext;
}



/*!
 * convert cipher format to string hexadecimal
 * @param cipher
 * @return string hex
 */
string AES::ciphertoString(string cipher){
// generate encoded
	string encoded;

	encoded.clear();
	StringSource(cipher, true,
		new HexEncoder(
			new StringSink(encoded)
		) // HexEncoder
	); // StringSource
	//cout << "#-- encoded text: " << encoded << endl;
return encoded;
}

/*!
 * convert string hexadecimal format to cipher format.
 * @param str
 * @return string
 */
string AES::stringtoCipher(string str){
// generate encoded
	string encoded;

	encoded.clear();
	StringSource(str, true,
		new HexDecoder(
			new StringSink(encoded)
		) // HexEncoder
	); // StringSource
	//cout << "#-- encoded text: " << encoded << endl;
return encoded;
}


} /* namespace AAES */
