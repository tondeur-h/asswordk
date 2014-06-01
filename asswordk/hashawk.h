/*
 * hashawk.h
 *
 *  Created on: 1 juin 2014
 *      Author: herve
 */

#ifndef HASHAWK_H_
#define HASHAWK_H_

#include <string>
using std::string;

namespace hashawk {

/*
 *
 */
class hashawk {
public:
	hashawk();
	string md5(string message); //old md5 hash
	string sha(string message); //sha512 hash
	virtual ~hashawk();
};

} /* namespace hashawk */

#endif /* HASHAWK_H_ */
