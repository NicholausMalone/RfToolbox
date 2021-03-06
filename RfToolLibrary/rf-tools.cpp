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

#include "stdafx.h"
#include "rf-tools.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <experimental/filesystem>
#include "Eigen\Dense"
#include "FTWW\fftw3.h"

using namespace std;
namespace filesys = std::experimental::filesystem;
#define M_PI 3.1415
//! Features to ADD //
//! http://qucs.sourceforge.net/tech/node98.html //
//! http://www2.electron.frba.utn.edu.ar/~jcecconi/Bibliografia/04%20-%20Param_S_y_VNA/Parameters%20Conversion%20Table.pdf //
//! http://ieeexplore.ieee.org/stamp/stamp.jsp?arnumber=4657394 // 

RfTools::RfTools()
{
	setlocale(LC_NUMERIC, "C");
}

RfTools::~RfTools()
{
}

void RfTools::Embed(ParameterObject& RX, ParameterObject& TX, ParameterObject& paramsToEmbed)
{
	// Match RX and TX to parameters to embed
	rxSpline = spline(RX.f, RX.S);
	txSpline = spline(TX.f, TX.S)

	paramsToEmbed = (RX.matrix()*paramsToEmbed.matrix().eval()*TX.matrix()).array();

}

void RfTools::Deembed(ParameterObject& RX, ParameterObject& TX, ParameterObject& paramsToDeembed)
{
	paramsToDeembed = (RX.matrix().inverse()*paramsToDeembed.matrix().eval()*TX.matrix().inverse()).array();
}

ParamMatrix RfTools::ZtoS(ParamMatrix& Z)
{
	ParamMatrix test;
	return test;
}

ParamMatrix RfTools::StoZ(ParamMatrix& S)
{
	ParamMatrix test;
	return test;
}

void RfTools::SEtoMM(ParamMatrix& params, PortPairing p)
{
	Eigen::Matrix4d M;
	M << 1, -1, 0, 0,
		0, 0, 1, -1,
		1, 1, 0, -1,
		0, 0, 1, 1;

	Eigen::Matrix4d swap;
	swap << 1, 0, 0, 0,
		0, 0, 1, 0,
		0, 1, 0, 0,
		0, 0, 0, 1;

	Eigen::Matrix4cd temp;


	if (p == OneThree) {
		temp = swap * params.matrix() * swap;
	}
	else {
		temp = params.matrix();
	}

	temp = M * temp.eval() * M.inverse();

	params = temp.array();
}

void RfTools::MMtoSE(ParamMatrix& params, PortPairing p)
{
	if (p == OneThree) {

	}

	ParamMatrix test;
}

void RfTools::SEtoMM(vector<ParamMatrix>& params, PortPairing p)
{
	Eigen::Matrix4d M;
	M << 1, -1, 0, 0,
		0, 0, 1, -1,
		1, 1, 0, -1,
		0, 0, 1, 1;

	Eigen::Matrix4d swap;
	swap << 1, 0, 0, 0,
			0, 0, 1, 0,
			0, 1, 0, 0,
			0, 0, 0, 1;

	double a = 1 / sqrt(2);
	M = a * M.eval();

	Eigen::Matrix4cd temp;

	for (auto& value : params) {
		
		if (p == OneThree) {
			temp = swap*value.matrix()*swap;
		}
		else {
			temp = value.matrix();
		}

		temp = M * temp.eval() * M.inverse();
		value = temp.array();
	}
}

void RfTools::MMtoSE(vector<ParamMatrix>& params, PortPairing p)
{
	if (p == OneThree) {


	}
	ParamMatrix test;
}

void RfTools::StoT(ParamMatrix& params)
{
	Eigen::Matrix2cd temp;

	temp = params.matrix();
	params(0, 0) = -temp.determinant()/temp(1,0);
	params(1, 0) = -temp(1,1)/temp(1,0);
	params(0, 1) = -temp(0,0)/temp(1, 0);
	params(1, 1) = 1.0/temp(1, 0);
	
}

void RfTools::TtoS(ParamMatrix& params)
{
	Eigen::Matrix2cd temp;

	temp = params.matrix();
	params(0, 0) = temp(0,1) / temp(1, 1);
	params(1, 0) = 1.0 / temp(1, 1);
	params(0, 1) = temp.determinant() / temp(1, 1);
	params(1, 1) = -temp(1, 0) / temp(1, 1);
	
}

ParameterObject RfTools::Load2P(const char* filename)
{
	ifstream File;
	ParameterObject spar;
	File.open(filename);
	if (!File.good())return spar;

	FUnit fUnit = GHz;
	SFormat sFormat = MA;
	double R;
	double freq, firstS11, secondS11, firstS21, secondS21, firstS12, secondS12, firstS22, secondS22;

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
		sscanf(line, "%lf %lf %lf %lf %lf %lf %lf %lf %lf", &freq, &firstS11, &secondS11, &firstS21, &secondS21, &firstS12, &secondS12, &firstS22, &secondS22);

		ParamMatrix lineMatrix(2, 2);
		lineMatrix(0, 0) = Eigen::dcomplex(firstS11, secondS11);
		lineMatrix(0, 1) = complex<double>(firstS12, secondS12);
		lineMatrix(1, 0) = complex<double>(firstS21, secondS21);
		lineMatrix(1, 1) = complex<double>(firstS22, secondS22);


		ParamMatrix tempS(2, 2);

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


		if (fUnit == GHz)freq *= 1e9;
		if (fUnit == MHz)freq *= 1e6;
		if (fUnit == KHz)freq *= 1e3;
		if (spar.f.size()>0 && freq<spar.f.back()) { cout << "Ignoring Noise Parameters in file" << endl;break; } //either error or noise parameters
		spar.f.push_back(freq);

		spar.S.push_back(tempS);
	}
	File.close();

	return spar;
}

//ParameterObject RfTools::Load3P(const char* filename)
//{
//	ifstream File;
//	ParameterObject spar;
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

ParameterObject RfTools::Load4P(const char* filename)
{
	ifstream File;
	ParameterObject spar;
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

ParameterObject RfTools::LoadCSV(const char* filename)
{
	ifstream File;
	ParameterObject spar;
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

		ParamMatrix lineMatrix(4, 4);
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

		ParamMatrix tempS(4, 4);

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
				if (abs(spar.S[i](nx, ny))>1e-20 && nx*ny>0)
				{
					if (nx>1 || ny >1)is4port = true;
					is2port = true;
					i = spar.S[1].rows();
				}
		}
	}

	return spar;
}

bool RfTools::SaveSnPFile(ParameterObject& spar, const char* filename, SFormat format)
{
	ofstream File;
	File.open(filename);
	bool fileOpened = false;

	if (File.good())fileOpened = true;
	char line[4096];
	if (fileOpened)
	{
		WriteSnPFile(spar, File, format);
	}

	if (fileOpened)File.close();
	return fileOpened;

}

bool RfTools::SaveCSVFile(ParameterObject& spar, const char* filename, SFormat format)
{
	ofstream File;
	File.open(filename);
	bool fileOpened = false;

	if (File.good())fileOpened = true;
	char line[4096];
	if (fileOpened)
	{
		WriteCSVFile(spar, File, format);
	}

	if (fileOpened)File.close();
	return fileOpened;

}

void RfTools::WriteSnPFile(ParameterObject& spar, ostream& stream, SFormat format)
{
	time_t timer = time(NULL);
	char line[4096];

	stream << "!Created with vna, " << ctime(&timer) << endl;
	stream << "# Hz " << (spar.paramType == S ? "S" : "Z") << " RI R 50" << endl;

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

void RfTools::WriteCSVFile(ParameterObject& spar, ostream& stream, SFormat format)
{
	char line[4096];

	stream << "f";

	if (spar.isMixedMode) {
		const char mixedModeLbls[4][4][5] = { { { 'D','D','1','1' },{ 'D','D','1','2' },{ 'D','C','1','1' },{ 'D','C','1','2' } },
											  { { 'D','D','2','1' },{ 'D','D','2','2' },{ 'D','C','2','1' },{ 'D','C','2','2' } },
											  { { 'C','D','1','1' },{ 'C','D','1','2' },{ 'C','C','1','1' },{ 'C','C','1','2' } },
											  { { 'C','D','2','1' },{ 'C','D','2','2' },{ 'C','C','2','1' },{ 'C','C','2','2' } } };

		for (int i = 0; i < spar.S[0].rows(); i++) {
			for (int j = 0; j < spar.S[0].rows(); j++) {
				stream << "," << "S" << mixedModeLbls[i][j] << "_real";
				stream << "," << "S" << mixedModeLbls[i][j] << "_imag";
			}
		}	
	}
	else {
		for (int i = 0; i < spar.S[0].rows(); i++) {
			for (int j = 0; j < spar.S[0].rows(); j++) {
				stream << "," << "S" << to_string(i + 1) << to_string(j + 1) << "_real";
				stream << "," << "S" << to_string(i + 1) << to_string(j + 1) << "_imag";
			}
		}
	}

	stream << endl;

	//if (format == DB) {
	//	double degToRad = (M_PI / 180);
	//	tempS.real() = (lineMatrix.imag()*degToRad).cos() * Eigen::pow(10, (lineMatrix.real() / 20));
	//	tempS.imag() = (lineMatrix.imag()*degToRad).sin() * Eigen::pow(10, (lineMatrix.real() / 20));
	//}
	//if (format == MA) {
	//	double degToRad = (M_PI / 180);
	//	tempS.real() = (lineMatrix.imag()*degToRad).cos() * lineMatrix.real();
	//	tempS.imag() = (lineMatrix.imag()*degToRad).sin() * lineMatrix.real();
	//}
	//// If in real and imaginary, do nothing
	//if (format == RI) {
	//	tempS = lineMatrix;
	//}

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

void RfTools::SaveCSVFileIfft(ParameterObject& spar, const char* filename)
{
	const int N = spar.f.size();
	fftw_complex *in, *out;
	fftw_plan p;

	in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);
	out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);

	p = fftw_plan_dft_1d(N, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);

	for (int i = 0; i < N; i++) {
		double* ptr = in[i];
		*ptr = spar.S[i](1, 0).real();       // set first double, real part
		*(ptr + 1) = spar.S[i](1, 0).imag();       // set second double, imaginary part
	}

	fftw_execute(p); /* repeat as needed */

	ofstream File;
	File.open(filename);
	bool fileOpened = false;

	if (File.good())fileOpened = true;

	if (fileOpened)
	{
		File << "IFFT_Real,IFFT_Imag" << endl;

		// Get values
		for (int i = 0; i < N; i++) {
			double* ptr = out[i];
			double real = *ptr;
			double imag = *(ptr + 1);

			File << real << "," << imag << endl;
		}
	}

	if (fileOpened)File.close();

	fftw_destroy_plan(p);
	fftw_free(in); fftw_free(out);
}

vector<string> RfTools::GetFilesInDir(const string &dirPath, const char* extType)
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
bool RfTools::iequals(const string& a, const string& b)
{
	unsigned int sz = a.size();
	if (b.size() != sz)
		return false;
	for (unsigned int i = 0; i < sz; ++i)
		if (tolower(a[i]) != tolower(b[i]))
			return false;
	return true;
}

vector<SplineSet> spline(vector<double> &x, vector<double> &y)
{
	int n = x.size() - 1;
	vector<double> a;
	a.insert(a.begin(), y.begin(), y.end());
	vector<double> b(n);
	vector<double> d(n);
	vector<double> h;

	for (int i = 0; i < n; ++i)
		h.push_back(x[i + 1] - x[i]);

	vector<double> alpha;
	for (int i = 0; i < n; ++i)
		alpha.push_back(3 * (a[i + 1] - a[i]) / h[i] - 3 * (a[i] - a[i - 1]) / h[i - 1]);

	vector<double> c(n + 1);
	vector<double> l(n + 1);
	vector<double> mu(n + 1);
	vector<double> z(n + 1);
	l[0] = 1;
	mu[0] = 0;
	z[0] = 0;

	for (int i = 1; i < n; ++i)
	{
		l[i] = 2 * (x[i + 1] - x[i - 1]) - h[i - 1] * mu[i - 1];
		mu[i] = h[i] / l[i];
		z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
	}

	l[n] = 1;
	z[n] = 0;
	c[n] = 0;

	for (int j = n - 1; j >= 0; --j)
	{
		c[j] = z[j] - mu[j] * c[j + 1];
		b[j] = (a[j + 1] - a[j]) / h[j] - h[j] * (c[j + 1] + 2 * c[j]) / 3;
		d[j] = (c[j + 1] - c[j]) / 3 / h[j];
	}

	vector<SplineSet> output_set(n);
	for (int i = 0; i < n; ++i)
	{
		output_set[i].a = a[i];
		output_set[i].b = b[i];
		output_set[i].c = c[i];
		output_set[i].d = d[i];
		output_set[i].x = x[i];
	}
	return output_set;
}

double getY(double x, vector<SplineSet> mySplineSet)
{
	int j;
	for (j = 0; j < (int)mySplineSet.size(); j++)
	{
		if (mySplineSet[j].x > x)
		{
			if (j == 0)
				j++;
			break;
		}
	}
	j--;

	double dx = x - mySplineSet[j].x;
	double y = mySplineSet[j].a + mySplineSet[j].b * dx + mySplineSet[j].c * dx* dx +
		mySplineSet[j].d * dx* dx * dx;

	return y;

}

vector<vector<SplineSet>> splineMatrixCoeff(vector<double> &f, ParamMatrix &y)
{
	spline(f, y(0, 0));
	spline(f, y(0, 1));
	spline(f, y(1, 0));
	spline(f, y(1, 1));

	return output_set;
}