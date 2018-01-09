

#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <cstdio>


const char *formatString = "\
------------------------------------------------------------------------------\n\
-- DateTime.vhd\n\
-- This file is automatically generated and contains the date and time\n\
-- that synthesis was started for this FPGA.\n\
------------------------------------------------------------------------------\n\
\n\
library IEEE;\n\
use IEEE.std_logic_1164.all;\n\
use ieee.numeric_std.all;\n\
\n\
entity DateTime is \n\
port    ( \n\
   YEAR              : out std_logic_vector(11 downto 0);  \n\
   DAYofYEAR         : out std_logic_vector(8 downto 0);  \n\
   HOURS             : out std_logic_vector(4 downto 0);\n\
   MINUTES           : out std_logic_vector(5 downto 0);\n\
   SECONDS           : out std_logic_vector(5 downto 0)\n\
);\n\
END DateTime;\n\
\n\
architecture RTL of DateTime is\n\
\n\
signal year_int   : integer := %4d;\n\
signal day_int    : integer := %d;\n\
signal hour_int   : integer := %d;\n\
signal minute_int : integer := %d;\n\
signal second_int : integer := %d;\n\
\n\
begin\n\
\n\
YEAR        <= std_logic_vector(to_unsigned(year_int, 12));\n\
DAYofYEAR   <= std_logic_vector(to_unsigned(day_int, 9));\n\
HOURS       <= std_logic_vector(to_unsigned(hour_int, 5));\n\
MINUTES     <= std_logic_vector(to_unsigned(minute_int, 6));\n\
SECONDS     <= std_logic_vector(to_unsigned(second_int, 6));\n\
\n\
END RTL;\n\
";

int main(int argc, char **argv)
{
	SYSTEMTIME lt;
	GetLocalTime(&lt);
	int dayCountArray[] = { 31, 28, 31, 30, 31, 30, 31, 30, 31, 30, 31 };
	int currMonth = lt.wMonth;
	int currYear = lt.wYear;
	if (currYear % 4 == 0)
	{
		dayCountArray[1]++;
	}

	int dayOfYear = 0;
	for (int i = 1; i < currMonth; i++)
	{
		dayOfYear += dayCountArray[i];
	}
	dayOfYear += lt.wDay;

	char newString[8192];
	sprintf(newString, formatString, currYear, dayOfYear, lt.wHour, lt.wMinute, lt.wSecond);
	
	std::cout << "Got date: Y " << currYear << ", D " << dayOfYear << ", H " << lt.wHour << ", M " << lt.wMinute << ", S " << lt.wSecond << std::endl;

	std::string pathString;
	if (argc >= 2)
	{
		pathString.append(argv[1]);
		std::cout << "Got path string " << pathString << std::endl;
		char lastChar = pathString.back();
		if (lastChar != '\\')
		{
			pathString.append("\\");
			std::cout << "Appending backslash" << std::endl;
		}
	}
	pathString.append("DateTime.vhd");

	std::cout << "Got path string " << pathString << std::endl;
	
	std::cout << "Writing formatted date information" << std::endl;
	std::ofstream myfile;
	myfile.open(pathString.c_str());
	if (myfile.good())
	{
		myfile << newString << std::endl;
		if (!myfile.good())
		{
			std::cout << "There was an error as we tried to write a string to file " << pathString << std::endl;
		}
	}
	else
	{
		std::cout << "There was an error opening file " << pathString << std::endl;
	}
	
	myfile.close();

	std::cout << "Done." << std::endl;

	return 0;
}