/*
 * hashawk.cpp
 *
 *  Created on: 1 juin 2014
 *      Author: herve
 */

#include "hashawk.h"

#include "cryptopp/md5.h"
using CryptoPP::Weak1::MD5;

#include "cryptopp/sha.h"
using CryptoPP::SHA512;

#include "cryptopp/filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;

#include "cryptopp/hex.h"
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

namespace hashawk {


hashawk::hashawk() {
	// TODO Auto-generated constructor stub
}

/*!
 * Hash a string with md5 algorithm
 * @param message
 * @return string coded md5
 */
string hashawk::md5(string message){
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
string hashawk::sha(string message){
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


hashawk::~hashawk() {
	// TODO Auto-generated destructor stub
}

} /* namespace pawk */
