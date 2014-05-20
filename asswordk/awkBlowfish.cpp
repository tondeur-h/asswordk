/*
 * awkBlowfish.cpp
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

#include "awkBlowfish.h"

#include "cryptopp/cryptlib.h"
using CryptoPP::Exception;

#include "cryptopp/hex.h"
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

#include "cryptopp/filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;

#include "cryptopp/blowfish.h"
using CryptoPP::Blowfish;

#include "cryptopp/modes.h"
using CryptoPP::CBC_Mode;

#include "cryptopp/secblock.h"
using CryptoPP::SecByteBlock;

#include "cryptopp/md5.h"
using CryptoPP::Weak1::MD5;

#include "cryptopp/sha.h"
using CryptoPP::SHA512;


namespace ABlowfish {

byte iv[Blowfish::BLOCKSIZE];

/*!
 * destructor do nothing...
 */
awkBlowfish::~awkBlowfish() {}

/*!
 * constructor of this class
 * create an array of 8 bytes
 * to initialise an IV (Initialization Vector)
 * At this time just a fixed IV is used
 */
awkBlowfish::awkBlowfish()
{
		iv[0]=34;
		iv[1]=45;
		iv[2]=67;
		iv[3]=12;
		iv[4]=78;
		iv[5]=87;
		iv[6]=98;
		iv[7]=23;
}

/*!
 *
 * @param plain
 * @param key
 * @return string as cipher format
 */
string awkBlowfish::crypt(string plain, string key){
	string cipher, encoded; //define string using

	encoded.clear(); //clear encoded to be sure

	//encode key in hexadecimal
	StringSource(key.c_str(), true,new HexEncoder(new StringSink(encoded))); // StringSource
	//cout << "#-- key: " << encoded << endl;

	// generate iv
	encoded.clear();
	StringSource(iv, sizeof(iv), true,
		new HexEncoder(
			new StringSink(encoded)
		) // HexEncoder
	); // StringSource
	//cout << "#-- iv: " << encoded << endl;


	try
	{
		//cout << "#-- plain text: " << plain << endl;

		//init key with IV
		CBC_Mode< Blowfish >::Encryption e;
		e.SetKeyWithIV((byte*)key.c_str(), key.size(), iv);

		// The StreamTransformationFilter adds padding
		//  as required. ECB and CBC Mode must be padded
		//  to the block size of the cipher.
		StringSource(plain, true,
			new StreamTransformationFilter(e,
				new StringSink(cipher)
			) // StreamTransformationFilter
		); // StringSource

		//cout << "#-- cipher text: " << cipher << endl;
	}
	catch(const CryptoPP::Exception& e)
	{
		cerr << e.what() << endl;
		exit(1);
	}

return cipher;

}  //end crypt


/*!
 * convert cipher format to string hexadecimal
 * @param cipher
 * @return string hex
 */
string awkBlowfish::ciphertoString(string cipher){
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
string awkBlowfish::stringtoCipher(string str){
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


/*!
 * decrypt a cipher format crypted sentence
 * @param cipher
 * @param key
 * @return string as decoded
 */
string awkBlowfish::decrypt(string cipher, string key){
string  recovered;

	try
	{
		CBC_Mode< Blowfish >::Decryption d;
		d.SetKeyWithIV((byte*)key.c_str(), key.size(), iv);

		// The StreamTransformationFilter removes
		//  padding as required.
		StringSource s(cipher, true,
			new StreamTransformationFilter(d,
				new StringSink(recovered)
			) // StreamTransformationFilter
		); // StringSource

		//cout << "#-- recovered text: " << recovered << endl;
	}
	catch(const CryptoPP::Exception& e)
	{
		cerr <<e.what() << endl;
		exit(1);
	}

	return recovered;

} //end decrypt


/*!
 * Hash a string with md5 algorithm
 * @param message
 * @return string coded md5
 */
string awkBlowfish::md5(string message){
MD5 hash;
byte digest[ MD5::DIGESTSIZE ];

hash.CalculateDigest( digest, (byte*) message.c_str(), message.length() );

CryptoPP::HexEncoder encoder;
std::string output;
encoder.Attach( new StringSink( output ) );
encoder.Put( digest, sizeof(digest) );
encoder.MessageEnd();

//std::cout <<"#-- output:"<<output << std::endl;
return output;
}

/*!
 *  Hash a string with SHA512 algorithm
 * @param message
 * @return string coded sha512
 */
string awkBlowfish::sha(string message){
SHA512 hash;
byte digest[ SHA512::DIGESTSIZE ];

hash.CalculateDigest( digest, (byte*) message.c_str(), message.length() );

CryptoPP::HexEncoder encoder;
std::string output;
encoder.Attach( new StringSink( output ) );
encoder.Put( digest, sizeof(digest) );
encoder.MessageEnd();

//std::cout <<"#-- output:"<< output << std::endl;
return output;
}

} /* namespace ABlowfish */
