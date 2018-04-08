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

#ifdef VNA_TOOLS_EXPORTS  
#define VNA_TOOLS_API __declspec(dllexport)   
#else  
#define VNA_TOOLS_API __declspec(dllimport)   
#endif  


#ifndef _VNA_TOOLS_H_
#define _VNA_TOOLS_H_

#include <vector>
#include "typedefs.h"

using namespace std;

class VNATools
{
public:
	VNATools();
	~VNATools();
	/////! can load s1p and s2p files
	//static VNA_TOOLS_API sparamobj Load2P(const char* filename);

	/////! can load s3p files
	//static VNA_TOOLS_API sparamobj Load3P(const char* filename);

	///! can load s4p files
	static VNA_TOOLS_API sparamobj Load4P(const char* filename);

	///! can load s4p files
	///! Converts a complex impedance into an S-parameter (normalized to 1)
	static VNA_TOOLS_API ParamMatrix ZtoS(ParamMatrix Z);

	///! Converts an S-parameter int a complex impedance
	static VNA_TOOLS_API ParamMatrix StoZ(ParamMatrix S);

	///! Degrees to radians and vise versa
	static VNA_TOOLS_API double RadtoDeg(double rad);
	static VNA_TOOLS_API double DegtoRad(double deg);

	///! Converts Single Ended to Mixed-Mode Parameters
	static VNA_TOOLS_API ParamMatrix SEtoMM(ParamMatrix S);

	///! Converts Mixed-Mode to Single Ended Parameters
	static VNA_TOOLS_API ParamMatrix MMtoSE(ParamMatrix S);

	///! Saves s1p, s2p, s3p and s4p touchstone files.
	static VNA_TOOLS_API bool SaveTouchstoneFile(sparamobj& spar, const char* filename, const char parameterType = 'S');

	///! Saves s1p, s2p, s3p and s4p CSV files.
	static VNA_TOOLS_API bool SaveCSVFile(sparamobj& spar, const char* filename, const char parameterType = 'S');

	///! Writes s1p, s2p, s3p and s4p touchstone files to stream.
	static VNA_TOOLS_API void WriteTouchstoneFile(sparamobj& spar, ostream& stream, const char parameterType = 'S');

	///! Writes s1p, s2p, s3p and s4p CSV files to stream.
	static VNA_TOOLS_API void WriteCSVFile(sparamobj& spar, ostream& stream, bool includeHeader = true, const char parameterType = 'S');

	///! Get files in directory.  extType is the type of extension ex. csv, s4p
	static VNA_TOOLS_API vector<string> GetFilesInDir(const string &dirPath, const char* extType);

private:
	static bool iequals(const string& a, const string& b);	
};

#endif // _VNA_TOOLS_H_