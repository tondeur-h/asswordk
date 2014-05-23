/*
 * tools.cpp
 *
 *  Created on: 15 mai 2014
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

#include "tools.h"


namespace std {


std::string* dep;

/*!
 * trim fonction that eliminate white space ahead, behind string
 * @param str
 * @return
 */
std::string trim(std::string str)
{
	if (str.empty()) return ""; //return empty if empty ???
	std::size_t first = str.find_first_not_of(' ');
    std::size_t last  = str.find_last_not_of(' ');
    return str.substr(first, last-first+1);
}

/*!
 * ltrim, remove white space ahead the string
 * @param str
 * @return
 */
std::string ltrim(std::string str)
{
	if (str.empty()) return ""; //return empty if empty ???
    std::size_t first = str.find_first_not_of(' ');
    std::size_t last  = str.length();
    return str.substr(first, last-first+1);
}

/*!
 * rtrim remove white space behind
 * @param str
 * @return
 */
std::string rtrim(std::string str)
{
	if (str.empty()) return ""; //return empty if empty ???
	std::size_t last  = str.length();
    return str.substr(0, last+1);
}


/*!
 * up case the string
 * @param str
 * @return
 */
std::string ucase(std::string str){
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
	return str;
}

/*!
 * down case the string
 * @param str
 * @return
 */
std::string lcase(std::string str){
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

/*!
 * cut a string in multiple string using the delim char to cut the string
 * @param str
 * @param delim
 * @return
 */
std::string* scanner(std::string str,char delim){
	dep=new string[20]; //max 20 string can be cutting
	for (int l=0;l<20;l++) {dep[l]="";} //blank the array
	istringstream iss(str); //convert str to istringstream to cut it
	    std::string mot;
	    int index=0;
	    while ( std::getline( iss, mot, delim ) )
	    {
	        dep[index]=mot; //sent each word into an array
	        index++;
	        if (index==20) return dep; //stop cutting if more than 20 words
	    }
return dep; //return the array
}

/*!
 * free memory using by scanner hope to find better function
 */
void close_scanner(){
	delete[] dep; //delete the array
}

/*!
 * clear screen console
 */
void clrscr() {
	printf("\033[H\033[2J"); //use ansi string to do that
}

/*!
 * set color caracters and background color for your console
 * @param param
 */
void color(const char* param) {
	printf("\033[%sm",param); //ansi string too
}

/*!
 * reset to normal white caracters and black background
 */
void normal_color(){printf("\033[0m");} //and also ansi string



/*!
 * rewrite getch to change attribute of the termios
 * @return
 */
int getch() {
    int ch;
    struct termios t_old, t_new; //define 2 termios structures

    tcgetattr(STDIN_FILENO, &t_old); //get termios attributes from STDIN_FILENO
    t_new = t_old; //copy it as new
    t_new.c_lflag &= ~(ICANON | ECHO); //change locals flags
    tcsetattr(STDIN_FILENO, TCSANOW, &t_new); //apply changes now with new flags

    ch = getchar(); //get char from user and return it

    tcsetattr(STDIN_FILENO, TCSANOW, &t_old); //reset old attributes
    return ch;
}


/*!
 * change standard input to star caracters
 * @return
 */
string getpass()
{
	//remplace with caracters for futur uses
bool show_asterisk=true;
//define specials caracters to know
  const char BACKSPACE=127;
  const char RETURN=10;

  string password;
  unsigned char ch=0;

  //loop getting string
  while((ch=getch())!=RETURN)
    {
       if(ch==BACKSPACE)
         { //if backspace is needed
            if(password.length()!=0)
              {
                 if(show_asterisk)
                 cout <<"\b \b";
                 password.resize(password.length()-1);
              }
         }
       else
         {//make the string password
             password+=ch;
             if(show_asterisk)
                 cout <<'*';
         }
    }
  cout <<endl;
  return password;
}


} //fin namespace
