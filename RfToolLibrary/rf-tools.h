/*
* DeEmbed
* Copyright (C) Frans Schreuder 2016 <info@schreuderelectronics.com>
*
software is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* software is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once  

#ifdef RF_TOOLS_EXPORTS  
#define RF_TOOLS_API __declspec(dllexport)   
#else  
#define RF_TOOLS_API __declspec(dllimport)   
#endif  


#ifndef _RF_TOOLS_H_
#define _RF_TOOLS_H_

#include <vector>
#include "typedefs.h"

using namespace std;

class RfTools
{
public:
	RfTools();
	~RfTools();
	///! can load s1p and s2p files
	static RF_TOOLS_API ParameterObject Load2P(const char* filename);

	/////! can load s3p files
	//static RF_TOOLS_API ParameterObject Load3P(const char* filename);

	///! can load s4p files
	static RF_TOOLS_API ParameterObject Load4P(const char* filename);

	///! can load s4p files
	///! Converts a complex impedance into an S-parameter (normalized to 1)
	static RF_TOOLS_API ParamMatrix ZtoS(ParamMatrix& Z);

	///! Converts an S-parameter int a complex impedance
	static RF_TOOLS_API ParamMatrix StoZ(ParamMatrix& S);

	///! Converts Single Ended to Mixed-Mode Parameters
	static RF_TOOLS_API void SEtoMM(ParamMatrix& params, PortPairing p = OneThree);

	///! Converts Mixed-Mode to Single Ended Parameters
	static RF_TOOLS_API void MMtoSE(ParamMatrix& params, PortPairing p = OneThree);

	///! Converts Single Ended to Mixed-Mode Parameters
	static RF_TOOLS_API void SEtoMM(vector<ParamMatrix>& params, PortPairing p = OneThree);

	///! Converts Mixed-Mode to Single Ended Parameters
	static RF_TOOLS_API void MMtoSE(vector<ParamMatrix>& params, PortPairing p = OneThree);

	///! Saves s1p, s2p, s3p and s4p SnP files.
	static RF_TOOLS_API bool SaveSnPFile(ParameterObject& spar, const char* filename, ParamType parameterType = S);

	///! Saves s1p, s2p, s3p and s4p CSV files.
	static RF_TOOLS_API bool SaveCSVFile(ParameterObject& spar, const char* filename, ParamType parameterType = S);

	///! Writes s1p, s2p, s3p and s4p SnP files to stream.
	static RF_TOOLS_API void WriteSnPFile(ParameterObject& spar, ostream& stream, ParamType parameterType = S);

	///! Writes s1p, s2p, s3p and s4p CSV files to stream.
	static RF_TOOLS_API void WriteCSVFile(ParameterObject& spar, ostream& stream, ParamType parameterType = S);

	///! Writes s1p, s2p, s3p and s4p CSV files to stream.
	static RF_TOOLS_API void SaveCSVFileIfft(ParameterObject& spar, const char* filename);

	///! Get files in directory.  extType is the type of extension ex. csv, s4p
	static RF_TOOLS_API vector<string> GetFilesInDir(const string &dirPath, const char* extType);

private:
	static bool iequals(const string& a, const string& b);	
};

#endif // _RF_TOOLS_H_