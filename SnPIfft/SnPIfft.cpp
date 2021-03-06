// SnPIfft.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "rf-tools.h"
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;


bool fileExists(const string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

int main(int argc, char** argv)
{

	cout << "*----------------------------------- SnP to CSV BETA -----------------------------*" << endl;
	cout << "*---------------------------------------------------------------------------------*" << endl;
	cout << "*          Current version only accepts S4P SnP v1.0 File formats.         *" << endl;
	cout << "*                                                                                 *" << endl;
	cout << "*  Parameter 1: SnP File                                                   *" << endl;
	cout << "*  Parameter 2: (Default \"s\") \"s\" or \"m\" produce single or mixed-mode results.    *" << endl;
	cout << "*              Ex. SnPtoCSV \"C:\\Test\measurementFile.s4p\" m                        *" << endl;
	cout << "*                                                                                 *" << endl;
	cout << "*---------------------------------------------------------------------------------*" << endl;
	cout << "" << endl;

	string filePath = "";
	bool outputIsMixedMode = false;

	if (argc != 2 && argc != 3) {
		cout << "Arguments invalid." << endl;
	}
	else {
		filePath = argv[1];
		if (argc == 3) {
			outputIsMixedMode = !strcmp(argv[2], "m");
		}
	}

	// Check directories
	if (!fileExists(filePath)) {
		return EXIT_FAILURE;
	}

	size_t pos = filePath.find_last_of(".");

	if (pos == string::npos) {
		return EXIT_FAILURE;
	}

	string csvFilePath = filePath.substr(0, pos) + "_iift.csv";

	ParameterObject spar = RfTools::Load4P(filePath.c_str());

	if (outputIsMixedMode) {
		RfTools::SEtoMM(spar.S);
	}

	RfTools::SaveCSVFileIfft(spar, csvFilePath.c_str());

	cout << "CSV file created. " << endl;

	return 0;
}

