/*
* DeEmbed
* Copyright (C) Frans Schreuder 2016 <info@schreuderelectronics.com>
* https://github.com/fransschreuder/DeEmbed
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

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

//#define CLI_ONLY

#include <complex>
#include <vector>
#include "Eigen\Dense"

using namespace std;
using Eigen::Array;
using Eigen::dcomplex;

// Maximum size is 4x4 matrix, helps with dynamic memory allocation
typedef Array<dcomplex, Eigen::Dynamic, Eigen::Dynamic, 0, 4, 4> ParamMatrix;

///! Contains the S-parameter matrix and the frequency range
typedef  struct {
	double Z0;
	vector<ParamMatrix> S;
	vector<double> f;
}	sparamobj;

///! Amplitude can be in dBm, W or mW
typedef enum { GHz, MHz, KHz, Hz }FUnit;
typedef enum { DB, MA, RI }SFormat;

#endif //TYPEDEFS_H