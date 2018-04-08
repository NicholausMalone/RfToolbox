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
* https://github.com/fransschreuder/DeEmbed
*/
#define NOMINMAX
#include "stdafx.h"
#include "vna-tools.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <experimental/filesystem>
#include "Eigen\Dense"

using namespace std;
namespace filesys = std::experimental::filesystem;
#define M_PI 3.1415



// Features to ADD //
// http://qucs.sourceforge.net/tech/node98.html //
// http://www2.electron.frba.utn.edu.ar/~jcecconi/Bibliografia/04%20-%20Param_S_y_VNA/Parameters%20Conversion%20Table.pdf //
// http://ieeexplore.ieee.org/stamp/stamp.jsp?arnumber=4657394 // 

VNATools::VNATools()
{
	setlocale(LC_NUMERIC, "C");
}

VNATools::~VNATools()
{
}

ParamMatrix VNATools::ZtoS(ParamMatrix Z)
{
	ParamMatrix test;
	return test;
}

ParamMatrix VNATools::StoZ(ParamMatrix S)
{
	ParamMatrix test;
	return test;
}

ParamMatrix VNATools::SEtoMM(ParamMatrix S)
{
	ParamMatrix test;
	return test;
}

ParamMatrix VNATools::MMtoSE(ParamMatrix S)
{
	ParamMatrix test;
	return test;
}

double VNATools::RadtoDeg(double rad)
{
	return rad * (180 / M_PI);
}

double VNATools::DegtoRad(double deg)
{
	return deg / (180 / M_PI);
}

//sparamobj VNATools::Load2P(const char* filename)
//{
//	ifstream File;
//	sparamobj spar;
//	File.open(filename);
//	if (!File.good())return spar;
//
//	spar.S.resize(2);
//	for (int i = 0; i<2; i++)
//		spar.S[i].resize(2);
//	FUnit fUnit = GHz;
//	SFormat sFormat = MA;
//	double R;
//	double freq, firstS11, secondS11, firstS21, secondS21, firstS12, secondS12, firstS22, secondS22;
//
//	char rawline[4096];
//	char* line;
//	while (File.getline(rawline, 4095))
//	{
//		line = rawline;
//		while (*line != '\0' && isspace(*line))line++; //strip leading spaces
//		if (line[0] == '!' || line[0] == '\0')continue; //ignore comment lines or emty lines
//		if (line[0] == '#')
//		{
//			while (*line != '\0' && (isspace(*line) || *line == '#'))line++; //strip leading '#' and spaces
//
//																			 //	# <frequency unit> <parameter> <format> R <n>
//			char sFUnit[256], sSFormat[256], dontcare[256];
//			sscanf(line, "%s %s %s %s %lf", sFUnit, dontcare, sSFormat, dontcare, &R);
//			if (lstrcmpiA(sFUnit, "GHZ") == 0)fUnit = GHz;
//			if (lstrcmpiA((sFUnit), "MHZ") == 0)fUnit = MHz;
//			if (lstrcmpiA((sFUnit), "KHZ") == 0)fUnit = KHz;
//			if (lstrcmpiA((sFUnit), "HZ") == 0)fUnit = Hz;
//
//			if (lstrcmpiA((sSFormat), "DB") == 0)sFormat = DB;
//			if (lstrcmpiA((sSFormat), "MA") == 0)sFormat = MA;
//			if (lstrcmpiA((sSFormat), "RI") == 0)sFormat = RI;
//			continue;
//		}
//		sscanf(line, "%lf %lf %lf %lf %lf %lf %lf %lf %lf", &freq, &firstS11, &secondS11, &firstS21, &secondS21, &firstS12, &secondS12, &firstS22, &secondS22);
//		if (fUnit == GHz)freq *= 1e9;
//		if (fUnit == MHz)freq *= 1e6;
//		if (fUnit == KHz)freq *= 1e3;
//		if (spar.f.size()>0 && freq<spar.f.back()) { cout << "Ignoring Noise Parameters in file" << endl;break; } //either error or noise parameters
//		spar.f.push_back(freq);
//		complex<double> tempS11;
//		if (sFormat == DB)tempS11 = complex<double>(pow(10, firstS11 / 20), 0)*exp(complex<double>(0, secondS11)*complex<double>(M_PI / 180, 0));
//		if (sFormat == MA)tempS11 = complex<double>(firstS11, 0)*exp(complex<double>(0, secondS11)*complex<double>(M_PI / 180, 0));
//		if (sFormat == RI)tempS11 = complex<double>(firstS11, secondS11);
//		spar.S[0][0].push_back(tempS11);
//		complex<double> tempS21;
//		if (sFormat == DB)tempS21 = complex<double>(pow(10, firstS21 / 20), 0)*exp(complex<double>(0, secondS21)*complex<double>(M_PI / 180, 0));
//		if (sFormat == MA)tempS21 = complex<double>(firstS21, 0)*exp(complex<double>(0, secondS21)*complex<double>(M_PI / 180, 0));
//		if (sFormat == RI)tempS21 = complex<double>(firstS21, secondS21);
//		spar.S[1][0].push_back(tempS21);
//		complex<double> tempS12;
//		if (sFormat == DB)tempS12 = complex<double>(pow(10, firstS12 / 20), 0)*exp(complex<double>(0, secondS12)*complex<double>(M_PI / 180, 0));
//		if (sFormat == MA)tempS12 = complex<double>(firstS12, 0)*exp(complex<double>(0, secondS12)*complex<double>(M_PI / 180, 0));
//		if (sFormat == RI)tempS12 = complex<double>(firstS12, secondS12);
//		spar.S[0][1].push_back(tempS12);
//		complex<double> tempS22;
//		if (sFormat == DB)tempS22 = complex<double>(pow(10, firstS22 / 20), 0)*exp(complex<double>(0, secondS22)*complex<double>(M_PI / 180, 0));
//		if (sFormat == MA)tempS22 = complex<double>(firstS22, 0)*exp(complex<double>(0, secondS22)*complex<double>(M_PI / 180, 0));
//		if (sFormat == RI)tempS22 = complex<double>(firstS22, secondS22);
//		spar.S[1][1].push_back(tempS22);
//	}
//	File.close();
//	bool is2port = false;
//	bool is4port = false;
//	for (int i = 0;i<(int)spar.S[0][0].size();i++)
//	{
//		for (int nx = 0;nx<2;nx++)
//		{
//			for (int ny = 0;ny<2;ny++)
//				if (abs(spar.S[i](nx,ny))>1e-20 && nx*ny>0)
//				{
//					//if(nx>1 || ny >1)is4port=true;
//					is2port = true;
//					i = spar.S[0][0].size();
//				}
//		}
//	}
//	if (!is4port)
//	{
//		spar.S.resize(2);
//		spar.S[0].resize(2);
//		spar.S[1].resize(2);
//	}
//	if (!is2port)
//	{
//		spar.S.resize(1);
//		spar.S[0].resize(1);
//	}
//
//	return spar;
//}
//
//sparamobj VNATools::Load3P(const char* filename)
//{
//	ifstream File;
//	sparamobj spar;
//	File.open(filename);
//	if (!File.good())return spar;
//
//	spar.S.resize(3);
//	for (int i = 0; i<3; i++)
//		spar.S[i].resize(3);
//	FUnit fUnit = GHz;
//	SFormat sFormat = MA;
//	double R;
//	double freq, firstS11, secondS11, firstS12, secondS12, firstS13, secondS13,
//		firstS21, secondS21, firstS22, secondS22, firstS23, secondS23,
//		firstS31, secondS31, firstS32, secondS32, firstS33, secondS33;
//
//	char rawline[4096];
//	char* line;
//	while (File.getline(rawline, 4095))
//	{
//		line = rawline;
//		while (*line != '\0' && isspace(*line))line++; //strip leading spaces
//		if (line[0] == '!' || line[0] == '\0')continue; //ignore comment lines or emty lines
//		if (line[0] == '#')
//		{
//			while (*line != '\0' && (isspace(*line) || *line == '#'))line++; //strip leading '#' and spaces
//
//																			 //	# <frequency unit> <parameter> <format> R <n>
//			char sFUnit[256], sSFormat[256], dontcare[256];
//			sscanf(line, "%s %s %s %s %lf", sFUnit, dontcare, sSFormat, dontcare, &R);
//			if (lstrcmpiA(sFUnit, "GHZ") == 0)fUnit = GHz;
//			if (lstrcmpiA((sFUnit), "MHZ") == 0)fUnit = MHz;
//			if (lstrcmpiA((sFUnit), "KHZ") == 0)fUnit = KHz;
//			if (lstrcmpiA((sFUnit), "HZ") == 0)fUnit = Hz;
//
//			if (lstrcmpiA((sSFormat), "DB") == 0)sFormat = DB;
//			if (lstrcmpiA((sSFormat), "MA") == 0)sFormat = MA;
//			if (lstrcmpiA((sSFormat), "RI") == 0)sFormat = RI;
//			continue;
//		}
//		sscanf(line, "%lf %lf %lf %lf %lf %lf %lf", &freq, &firstS11, &secondS11, &firstS12, &secondS12, &firstS13, &secondS13);
//
//		if (!File.getline(rawline, 4095))break;
//		line = rawline;
//		while (*line != '\0' && isspace(*line))line++; //strip leading spaces
//		while (line[0] == '!' || line[0] == '\0') //ignore comment lines or empty lines
//		{
//			if (!File.getline(rawline, 4095))break;
//			line = rawline;
//			while (*line != '\0' && isspace(*line))line++; //strip leading spaces
//		}
//		sscanf(line, "%lf %lf %lf %lf %lf %lf", &firstS21, &secondS21, &firstS22, &secondS22, &firstS23, &secondS23);
//
//		if (!File.getline(rawline, 4095))break;
//		line = rawline;
//		while (*line != '\0' && isspace(*line))line++; //strip leading spaces
//		while (line[0] == '!' || line[0] == '\0') //ignore comment lines or empty lines
//		{
//			if (!File.getline(rawline, 4095))break;
//			line = rawline;
//			while (*line != '\0' && isspace(*line))line++; //strip leading spaces
//		}
//		sscanf(line, "%lf %lf %lf %lf %lf %lf", &firstS31, &secondS31, &firstS32, &secondS32, &firstS33, &secondS33);
//
//		if (fUnit == GHz)freq *= 1e9;
//		if (fUnit == MHz)freq *= 1e6;
//		if (fUnit == KHz)freq *= 1e3;
//		if (spar.f.size()>0 && freq<spar.f.back()) { cout << "Ignoring Noise Parameters in file" << endl;break; } //either error or noise parameters
//		spar.f.push_back(freq);
//		complex<double> tempS11;
//		if (sFormat == DB)tempS11 = complex<double>(pow(10, firstS11 / 20), 0)*exp(complex<double>(0, secondS11)*complex<double>(M_PI / 180, 0));
//		if (sFormat == MA)tempS11 = complex<double>(firstS11, 0)*exp(complex<double>(0, secondS11)*complex<double>(M_PI / 180, 0));
//		if (sFormat == RI)tempS11 = complex<double>(firstS11, secondS11);
//		spar.S[0][0].push_back(tempS11);
//		complex<double> tempS12;
//		if (sFormat == DB)tempS12 = complex<double>(pow(10, firstS12 / 20), 0)*exp(complex<double>(0, secondS12)*complex<double>(M_PI / 180, 0));
//		if (sFormat == MA)tempS12 = complex<double>(firstS12, 0)*exp(complex<double>(0, secondS12)*complex<double>(M_PI / 180, 0));
//		if (sFormat == RI)tempS12 = complex<double>(firstS12, secondS12);
//		spar.S[0][1].push_back(tempS12);
//		complex<double> tempS13;
//		if (sFormat == DB)tempS13 = complex<double>(pow(10, firstS13 / 20), 0)*exp(complex<double>(0, secondS13)*complex<double>(M_PI / 180, 0));
//		if (sFormat == MA)tempS13 = complex<double>(firstS13, 0)*exp(complex<double>(0, secondS13)*complex<double>(M_PI / 180, 0));
//		if (sFormat == RI)tempS13 = complex<double>(firstS13, secondS13);
//		spar.S[0][2].push_back(tempS13);
//		complex<double> tempS21;
//		if (sFormat == DB)tempS21 = complex<double>(pow(10, firstS21 / 20), 0)*exp(complex<double>(0, secondS21)*complex<double>(M_PI / 180, 0));
//		if (sFormat == MA)tempS21 = complex<double>(firstS21, 0)*exp(complex<double>(0, secondS21)*complex<double>(M_PI / 180, 0));
//		if (sFormat == RI)tempS21 = complex<double>(firstS21, secondS21);
//		spar.S[1][0].push_back(tempS21);
//		complex<double> tempS22;
//		if (sFormat == DB)tempS22 = complex<double>(pow(10, firstS22 / 20), 0)*exp(complex<double>(0, secondS22)*complex<double>(M_PI / 180, 0));
//		if (sFormat == MA)tempS22 = complex<double>(firstS22, 0)*exp(complex<double>(0, secondS22)*complex<double>(M_PI / 180, 0));
//		if (sFormat == RI)tempS22 = complex<double>(firstS22, secondS22);
//		spar.S[1][1].push_back(tempS22);
//		complex<double> tempS23;
//		if (sFormat == DB)tempS23 = complex<double>(pow(10, firstS23 / 20), 0)*exp(complex<double>(0, secondS23)*complex<double>(M_PI / 180, 0));
//		if (sFormat == MA)tempS23 = complex<double>(firstS23, 0)*exp(complex<double>(0, secondS23)*complex<double>(M_PI / 180, 0));
//		if (sFormat == RI)tempS23 = complex<double>(firstS23, secondS23);
//		spar.S[1][2].push_back(tempS23);
//		complex<double> tempS31;
//		if (sFormat == DB)tempS31 = complex<double>(pow(10, firstS31 / 20), 0)*exp(complex<double>(0, secondS31)*complex<double>(M_PI / 180, 0));
//		if (sFormat == MA)tempS31 = complex<double>(firstS31, 0)*exp(complex<double>(0, secondS31)*complex<double>(M_PI / 180, 0));
//		if (sFormat == RI)tempS31 = complex<double>(firstS31, secondS31);
//		spar.S[2][0].push_back(tempS31);
//		complex<double> tempS32;
//		if (sFormat == DB)tempS32 = complex<double>(pow(10, firstS32 / 20), 0)*exp(complex<double>(0, secondS32)*complex<double>(M_PI / 180, 0));
//		if (sFormat == MA)tempS32 = complex<double>(firstS32, 0)*exp(complex<double>(0, secondS32)*complex<double>(M_PI / 180, 0));
//		if (sFormat == RI)tempS32 = complex<double>(firstS32, secondS32);
//		spar.S[2][1].push_back(tempS32);
//		complex<double> tempS33;
//		if (sFormat == DB)tempS33 = complex<double>(pow(10, firstS33 / 20), 0)*exp(complex<double>(0, secondS33)*complex<double>(M_PI / 180, 0));
//		if (sFormat == MA)tempS33 = complex<double>(firstS33, 0)*exp(complex<double>(0, secondS33)*complex<double>(M_PI / 180, 0));
//		if (sFormat == RI)tempS33 = complex<double>(firstS33, secondS33);
//		spar.S[2][2].push_back(tempS33);
//	}
//	File.close();
//	bool is2port = false;
//	bool is3port = false;
//	for (int i = 0;i<(int)spar.S[1].size();i++)
//	{
//		for (int nx = 0;nx<3;nx++)
//		{
//			for (int ny = 0;ny<3;ny++)
//				if (abs(spar.S[i](nx,ny))>1e-20 && nx*ny>0)
//				{
//					if (nx>1 || ny >1)is3port = true;
//					is2port = true;
//					i = spar.S[1].size();
//				}
//		}
//	}
//	if (!is3port)
//	{
//		spar.S.resize(2);
//		spar.S[0].resize(2);
//		spar.S[1].resize(2);
//	}
//	if (!is2port)
//	{
//		spar.S.resize(1);
//		spar.S[0].resize(1);
//	}
//
//	return spar;
//}

sparamobj VNATools::Load4P(const char* filename)
{
	ifstream File;
	sparamobj spar;
	File.open(filename);
	if (!File.good())return spar;

	FUnit fUnit = GHz;
	SFormat sFormat = MA;
	double R;
	double freq, firstS11, secondS11, firstS12, secondS12, firstS13, secondS13, firstS14, secondS14,
		firstS21, secondS21, firstS22, secondS22, firstS23, secondS23, firstS24, secondS24,
		firstS31, secondS31, firstS32, secondS32, firstS33, secondS33, firstS34, secondS34,
		firstS41, secondS41, firstS42, secondS42, firstS43, secondS43, firstS44, secondS44;

	char rawline[4096];
	char* line;
	while (File.getline(rawline, 4095))
	{
		line = rawline;
		while (*line != '\0' && isspace(*line))line++; //strip leading spaces
		if (line[0] == '!' || line[0] == '\0')continue; //ignore comment lines or emty lines
		if (line[0] == '#')
		{
			while (*line != '\0' && (isspace(*line) || *line == '#'))line++; //strip leading '#' and spaces

																			 //	# <frequency unit> <parameter> <format> R <n>
			char sFUnit[256], sSFormat[256], dontcare[256];
			sscanf(line, "%s %s %s %s %lf", sFUnit, dontcare, sSFormat, dontcare, &R);
			if (lstrcmpiA(sFUnit, "GHZ") == 0)fUnit = GHz;
			if (lstrcmpiA((sFUnit), "MHZ") == 0)fUnit = MHz;
			if (lstrcmpiA((sFUnit), "KHZ") == 0)fUnit = KHz;
			if (lstrcmpiA((sFUnit), "HZ") == 0)fUnit = Hz;

			if (lstrcmpiA((sSFormat), "DB") == 0)sFormat = DB;
			if (lstrcmpiA((sSFormat), "MA") == 0)sFormat = MA;
			if (lstrcmpiA((sSFormat), "RI") == 0)sFormat = RI;
			continue;
		}
		sscanf(line, "%lf %lf %lf %lf %lf %lf %lf %lf %lf", &freq, &firstS11, &secondS11, &firstS12, &secondS12, &firstS13, &secondS13, &firstS14, &secondS14);

		if (!File.getline(rawline, 4095))break;
		line = rawline;
		while (*line != '\0' && isspace(*line))line++; //strip leading spaces
		while (line[0] == '!' || line[0] == '\0') //ignore comment lines or empty lines
		{
			if (!File.getline(rawline, 4095))break;
			line = rawline;
			while (*line != '\0' && isspace(*line))line++; //strip leading spaces
		}
		sscanf(line, "%lf %lf %lf %lf %lf %lf %lf %lf", &firstS21, &secondS21, &firstS22, &secondS22, &firstS23, &secondS23, &firstS24, &secondS24);

		if (!File.getline(rawline, 4095))break;
		line = rawline;
		while (*line != '\0' && isspace(*line))line++; //strip leading spaces
		while (line[0] == '!' || line[0] == '\0') //ignore comment lines or empty lines
		{
			if (!File.getline(rawline, 4095))break;
			line = rawline;
			while (*line != '\0' && isspace(*line))line++; //strip leading spaces
		}
		sscanf(line, "%lf %lf %lf %lf %lf %lf %lf %lf", &firstS31, &secondS31, &firstS32, &secondS32, &firstS33, &secondS33, &firstS34, &secondS34);

		if (!File.getline(rawline, 4095))break;
		line = rawline;
		while (*line != '\0' && isspace(*line))line++; //strip leading spaces
		while (line[0] == '!' || line[0] == '\0') //ignore comment lines or empty lines
		{
			if (!File.getline(rawline, 4095))break;
			line = rawline;
			while (*line != '\0' && isspace(*line))line++; //strip leading spaces
		}
		sscanf(line, "%lf %lf %lf %lf %lf %lf %lf %lf", &firstS41, &secondS41, &firstS42, &secondS42, &firstS43, &secondS43, &firstS44, &secondS44);

		ParamMatrix lineMatrix(4,4);
		lineMatrix(0, 0) = Eigen::dcomplex(firstS11, secondS11);
		lineMatrix(0, 1) = complex<double>(firstS12, secondS12);
		lineMatrix(0, 2) = complex<double>(firstS13, secondS13);
		lineMatrix(0, 3) = complex<double>(firstS14, secondS14);
		lineMatrix(1, 0) = complex<double>(firstS21, secondS21);
		lineMatrix(1, 1) = complex<double>(firstS22, secondS22);
		lineMatrix(1, 2) = complex<double>(firstS23, secondS23);
		lineMatrix(1, 3) = complex<double>(firstS24, secondS24);
		lineMatrix(2, 0) = complex<double>(firstS31, secondS31);
		lineMatrix(2, 1) = complex<double>(firstS32, secondS32);
		lineMatrix(2, 2) = complex<double>(firstS33, secondS33);
		lineMatrix(2, 3) = complex<double>(firstS34, secondS34);
		lineMatrix(3, 0) = complex<double>(firstS41, secondS41);
		lineMatrix(3, 1) = complex<double>(firstS42, secondS42);
		lineMatrix(3, 2) = complex<double>(firstS43, secondS43);
		lineMatrix(3, 3) = complex<double>(firstS44, secondS44);

		if (fUnit == GHz)freq *= 1e9;
		if (fUnit == MHz)freq *= 1e6;
		if (fUnit == KHz)freq *= 1e3;
		if (spar.f.size()>0 && freq<spar.f.back()) { cout << "Ignoring Noise Parameters in file" << endl;break; } //either error or noise parameters
		spar.f.push_back(freq);
		
		ParamMatrix tempS(4,4);

		if (sFormat == DB) {
			double degToRad = (M_PI / 180);
			tempS.real() = (lineMatrix.imag()*degToRad).cos() * Eigen::pow(10, (lineMatrix.real() / 20));
			tempS.imag() = (lineMatrix.imag()*degToRad).sin() * Eigen::pow(10, (lineMatrix.real() / 20));
		}
		if (sFormat == MA) {
			double degToRad = (M_PI / 180);
			tempS.real() = (lineMatrix.imag()*degToRad).cos() * lineMatrix.real();
			tempS.imag() = (lineMatrix.imag()*degToRad).sin() * lineMatrix.real();
		}
		// If in real and imaginary, do nothing
		if (sFormat == RI) {
			tempS = lineMatrix;
		}

		spar.S.push_back(tempS);
	}
	File.close();
	bool is2port = false;
	bool is4port = false;
	for (int i = 0;i<(int)spar.S[1].rows();i++)
	{
		for (int nx = 0;nx<4;nx++)
		{
			for (int ny = 0;ny<4;ny++)
				if (abs(spar.S[i](nx,ny))>1e-20 && nx*ny>0)
				{
					if (nx>1 || ny >1)is4port = true;
					is2port = true;
					i = spar.S[1].rows();
				}
		}
	}

	return spar;
}

bool VNATools::SaveTouchstoneFile(sparamobj& spar, const char* filename, const char parameterType)
{
	ofstream File;
	File.open(filename);
	bool fileOpened = false;

	if (File.good())fileOpened = true;
	char line[4096];
	if (fileOpened)
	{
		WriteTouchstoneFile(spar, File, parameterType);
	}

	if (fileOpened)File.close();
	return fileOpened;

}

bool VNATools::SaveCSVFile(sparamobj& spar, const char* filename, const char parameterType)
{
	ofstream File;
	File.open(filename);
	bool fileOpened = false;

	if (File.good())fileOpened = true;
	char line[4096];
	if (fileOpened)
	{
		WriteCSVFile(spar, File, true, parameterType);
	}

	if (fileOpened)File.close();
	return fileOpened;

}

void VNATools::WriteTouchstoneFile(sparamobj& spar, ostream& stream, const char parameterType)
{
	time_t timer = time(NULL);
	char line[4096];

	stream << "!Created with vna, " << ctime(&timer) << endl;
	stream << "# Hz " << parameterType << " RI R 50" << endl;

	for (int i = 0;i<(int)spar.f.size();i++)
	{
		if (spar.S[0].rows() == 1)
			sprintf(line, "%lf\t%le\t%le", spar.f[i], spar.S[i](0,0).real(), spar.S[i](0,0).imag());
		else if (spar.S[0].rows() == 2)
			sprintf(line, "%lf\t%le\t%le\t%le\t%le\t%le\t%le\t%le\t%le", spar.f[i],
				spar.S[i](0,0).real(), spar.S[i](0,0).imag(),
				spar.S[i](1,0).real(), spar.S[i](1,0).imag(),
				spar.S[i](0,1).real(), spar.S[i](0,1).imag(),
				spar.S[i](1,1).real(), spar.S[i](1,1).imag());
		else if (spar.S[0].rows() == 4)
			sprintf(line, "%lf\t%le\t%le\t%le\t%le\t%le\t%le\t%le\t%le\n\t%le\t%le\t%le\t%le\t%le\t%le\t%le\t%le\t%le\t%le\t%le\t%le\t%le\t%le\t%le\t%le\t%le\t%le\t%le\t%le\t%le\t%le\t%le\t%le", spar.f[i],
				spar.S[i](0,0).real(), spar.S[i](0,0).imag(),
				spar.S[i](0,1).real(), spar.S[i](0,1).imag(),
				spar.S[i](0,2).real(), spar.S[i](0,2).imag(),
				spar.S[i](0,3).real(), spar.S[i](0,3).imag(),
				spar.S[i](1,0).real(), spar.S[i](1,0).imag(),
				spar.S[i](1,1).real(), spar.S[i](1,1).imag(),
				spar.S[i](1,2).real(), spar.S[i](1,2).imag(),
				spar.S[i](1,3).real(), spar.S[i](1,3).imag(),
				spar.S[i](2,0).real(), spar.S[i](2,0).imag(),
				spar.S[i](2,1).real(), spar.S[i](2,1).imag(),
				spar.S[i](2,2).real(), spar.S[i](2,2).imag(),
				spar.S[i](2,3).real(), spar.S[i](2,3).imag(),
				spar.S[i](3,0).real(), spar.S[i](3,0).imag(),
				spar.S[i](3,1).real(), spar.S[i](3,1).imag(),
				spar.S[i](3,2).real(), spar.S[i](3,2).imag(),
				spar.S[i](3,3).real(), spar.S[i](3,3).imag());

		stream << line << endl;
	}
}

void VNATools::WriteCSVFile(sparamobj& spar, ostream& stream, bool includeHeader, const char parameterType)
{
	char line[4096];

	if (includeHeader) {
		stream << "f";
		for (int i = 0; i < spar.S[0].rows(); i++) {
			for (int j = 0; j < spar.S[0].rows(); j++) {
				stream << "," << parameterType << to_string(i + 1) << to_string(j + 1) << "_real";
				stream << "," << parameterType << to_string(i + 1) << to_string(j + 1) << "_imag";
			}
		}
		stream << endl;
	}

	for (int i = 0;i<(int)spar.f.size();i++)
	{
		if (spar.S[0].rows() == 1)
			sprintf(line, "%lf,%le,%le", spar.f[i], spar.S[i](0,0).real(), spar.S[i](0,0).imag());
		else if (spar.S[0].rows() == 2)
			sprintf(line, "%lf,%le,%le,%le,%le,%le,%le,%le,%le", spar.f[i],
				spar.S[i](0,0).real(), spar.S[i](0,0).imag(),
				spar.S[i](1,0).real(), spar.S[i](1,0).imag(),
				spar.S[i](0,1).real(), spar.S[i](0,1).imag(),
				spar.S[i](1,1).real(), spar.S[i](1,1).imag());
		else if (spar.S[0].rows() == 4)
			sprintf(line, "%lf,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le", spar.f[i],
				spar.S[i](0,0).real(), spar.S[i](0,0).imag(),
				spar.S[i](0,1).real(), spar.S[i](0,1).imag(),
				spar.S[i](0,2).real(), spar.S[i](0,2).imag(),
				spar.S[i](0,3).real(), spar.S[i](0,3).imag(),
				spar.S[i](1,0).real(), spar.S[i](1,0).imag(),
				spar.S[i](1,1).real(), spar.S[i](1,1).imag(),
				spar.S[i](1,2).real(), spar.S[i](1,2).imag(),
				spar.S[i](1,3).real(), spar.S[i](1,3).imag(),
				spar.S[i](2,0).real(), spar.S[i](2,0).imag(),
				spar.S[i](2,1).real(), spar.S[i](2,1).imag(),
				spar.S[i](2,2).real(), spar.S[i](2,2).imag(),
				spar.S[i](2,3).real(), spar.S[i](2,3).imag(),
				spar.S[i](3,0).real(), spar.S[i](3,0).imag(),
				spar.S[i](3,1).real(), spar.S[i](3,1).imag(),
				spar.S[i](3,2).real(), spar.S[i](3,2).imag(),
				spar.S[i](3,3).real(), spar.S[i](3,3).imag());
		stream << line << endl;
	}
}

vector<string> VNATools::GetFilesInDir(const string &dirPath, const char* extType)
{
	// Create a vector of string
	std::vector<std::string> listOfFiles;
	try {
		// Check if given path exists and points to a directory
		if (filesys::exists(dirPath) && filesys::is_directory(dirPath))
		{
			// Create a Recursive Directory Iterator object and points to the starting of directory
			filesys::directory_iterator iter(dirPath);

			// Create a Recursive Directory Iterator object pointing to end.
			filesys::directory_iterator end;

			// Iterate till end
			while (iter != end)
			{
				string iterExt = iter->path().string().substr(iter->path().string().find_last_of(".") + 1);
				if (iequals(iterExt, extType)) {
					// Add the name in vector
					listOfFiles.push_back(iter->path().string());
				}					
				

				error_code ec;
				// Increment the iterator to point to next entry in recursive iteration
				iter.increment(ec);
				if (ec) {
					std::cerr << "Error While Accessing : " << iter->path().string() << " :: " << ec.message() << '\n';
				}
			}
		}
	}
	catch (std::system_error & e)
	{
		std::cerr << "Exception :: " << e.what();
	}
	return listOfFiles;
}


// Private functions
bool VNATools::iequals(const string& a, const string& b)
{
	unsigned int sz = a.size();
	if (b.size() != sz)
		return false;
	for (unsigned int i = 0; i < sz; ++i)
		if (tolower(a[i]) != tolower(b[i]))
			return false;
	return true;
}