// CreateCSVSummary.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "rf-tools.h"
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

vector<string> splitFileName(string& filename) {
	std::string delimiter = "_";
	vector<string> splitFilename;

	size_t start = 0;
	int currentPos = 0;
	size_t endOfFileName = filename.find_last_of(".", filename.size());

	if (endOfFileName == string::npos) {
		return splitFilename;
	}

	std::string token;
	while ((start = filename.find(delimiter,start+1)) != std::string::npos) {
		int nextDelim = filename.find(delimiter, start+1);
		int end = nextDelim != std::string::npos ? nextDelim : endOfFileName;
		token = filename.substr(start+1, end-start-1);
		splitFilename.push_back(token);
	}
	return splitFilename;
}

vector<string> getFilenameVars(string& filename) {
	vector<string> filenameTokens = splitFileName(filename);
	vector<string> filenameVars;

	for (auto const& var : filenameTokens) {
		size_t pos = string::npos;
		string variableName;

		if ((pos = var.find("=")) != string::npos) {
			variableName = var.substr(0, pos);
			filenameVars.push_back(variableName);
		}
	}
	return filenameVars;
}

vector<string> getFilenameVals(string& filename) {
	vector<string> filenameTokens = splitFileName(filename);
	vector<string> filenameVals;

	for (auto const& var : filenameTokens) {
		size_t pos = string::npos;
		string variableValue;

		if ((pos = var.find("=")) != string::npos) {
			variableValue = var.substr(pos+1, var.size()-pos);
			filenameVals.push_back(variableValue);
		}
	}

	return filenameVals;
}

bool directoryExists(string& directoryPath) {
	struct stat info;

	if (stat(directoryPath.c_str(), &info) != 0) {
		cout << "cannot access " << directoryPath << endl;
		return false;
	}
	else if (info.st_mode & S_IFDIR)
	{
		return true;
	}
	else {
		cout << directoryPath << " is not a directory" << endl;
		return false;
	}
}

int main(int argc, char** argv)
{
	auto start = std::chrono::high_resolution_clock::now();

	cout << "*------------------------------ CSV SUMMARY CREATOR BETA -------------------------*" << endl;
	cout << "*---------------------------------------------------------------------------------*" << endl;
	cout << "*          Current version only accepts S4P SnP v1.0 File formats.         *" << endl;
	cout << "*                                                                                 *" << endl;
	cout << "*  Parameter 1: SnP File Directory                                         *" << endl;
	cout << "*  Parameter 2: Summary Save Directory                                            *" << endl;
	cout << "*  Parameter 3: (Default \"n\") \"y\" or \"n\" indicating if settings are in file name. *" << endl;
	cout << "*  Parameter 4: (Default \"s\") \"s\" or \"m\" produce single or mixed-mode results.    *" << endl;
	cout << "*  Parameter 5: (Default \"Summary\")  A desired filename for the CSV File.         *" << endl;
	cout << "*              Ex. CreateCSVSummary \"C:\\Test\" \"C:\\Test\\Save\" y m                  *" << endl;
	cout << "*                                                                                 *" << endl;
	cout << "*  Filename settings are assumed to begin with \"_\" and are assigned with \"=\".     *" << endl;
	cout << "*             Ex. \"C:\\Test\MeasurementFile_Var1=Val1_Var2=Val2.s4p\"                *" << endl;
	cout << "*---------------------------------------------------------------------------------*" << endl;
	cout << "" << endl;

	string dirPath = "";
	string dirSavePath = "";
	vector<string> filenameVars;
	bool isSettingsInFileName = false;
	bool outputIsMixedMode = false;
	string csvFileName = "Summary";

	/* COMMENT THIS OUT */
	string tx = "C:\\Users\\a0232381\\Desktop\\Test\\AlgorithmTest\\Trace\\tx0.s4p";
	string rx = "C:\\Users\\a0232381\\Desktop\\Test\\AlgorithmTest\\Trace\\rx0.s4p";

	if (argc != 3 && argc != 4 && argc != 5 && argc != 6) {
		cout << "Invalid number of arguments." << endl;
		return EXIT_FAILURE;
	}
	else {
		dirPath = argv[1];
		dirSavePath = argv[2];
		if (argc == 4 || argc == 5 || argc == 6) {
			isSettingsInFileName = !strcmp(argv[3], "y");

			if (argc == 5) {
				outputIsMixedMode = !strcmp(argv[4], "m");
			}

			if (argc == 6) {
				csvFileName = argv[5];
			}
		}
	}

	// Check directories
	if (!directoryExists(dirPath) || !directoryExists(dirSavePath)) {
		return EXIT_FAILURE;
	}

	// Get File List
	vector<string> fileList = RfTools::GetFilesInDir(dirPath, "s4p");

	if (fileList.size() == 0) {
		cout << "There are no files in the SnP directory given." << endl;
		return EXIT_FAILURE;
	}

	ofstream summaryFile(dirSavePath + "\\" + csvFileName + ".csv");
	ofstream logFile(dirSavePath + "\\Log.txt");

	// Write Header (S-Parameters)
	ParameterObject firstSParam = RfTools::Load4P(fileList[0].c_str());

	/* DEEMBED */
	ParameterObject RX = RfTools::Load4P(rx.c_str());
	ParameterObject TX = RfTools::Load4P(tx.c_str());

	summaryFile << "f";
	if (outputIsMixedMode) {
		const char mixedModeLbls[4][4][5] = { { { 'D','D','1','1' },{ 'D','D','1','2' },{ 'D','C','1','1' },{ 'D','C','1','2' } },
		{ { 'D','D','2','1' },{ 'D','D','2','2' },{ 'D','C','2','1' },{ 'D','C','2','2' } },
		{ { 'C','D','1','1' },{ 'C','D','1','2' },{ 'C','C','1','1' },{ 'C','C','1','2' } },
		{ { 'C','D','2','1' },{ 'C','D','2','2' },{ 'C','C','2','1' },{ 'C','C','2','2' } } };

		for (int i = 0; i < firstSParam.S[0].rows(); i++) {
			for (int j = 0; j < firstSParam.S[0].rows(); j++) {
				summaryFile << "," << "S" << mixedModeLbls[i][j] << "_real";
				summaryFile << "," << "S" << mixedModeLbls[i][j] << "_imag";
			}
		}
	}
	else {
		for (int i = 0; i < firstSParam.S[0].rows(); i++) {
			for (int j = 0; j < firstSParam.S[0].rows(); j++) {
				summaryFile << "," << "S" << to_string(i + 1) << to_string(j + 1) << "_real";
				summaryFile << "," << "S" << to_string(i + 1) << to_string(j + 1) << "_imag";
			}
		}
	}


	// Write Header (Settings)
	if (isSettingsInFileName) {
		filenameVars = getFilenameVars(fileList[0]);
	}
	else {
		filenameVars = vector<string>({ "FileNum" });
	}

	for (auto const& var : filenameVars) {
		summaryFile << "," << var;
	}

	summaryFile << endl;

	// Write Data
	char line[4096];
	for (int r = 0; r < fileList.size(); r++) {
		
		ParameterObject spar = RfTools::Load4P(fileList[r].c_str());

		/* DEEMBED */
		// RfTools::Deembed(RX.S, TX.S, spar.S);

		if (outputIsMixedMode) {
			RfTools::SEtoMM(spar.S);
		}

		logFile << "FileNum " << r << ": " << fileList[r] << endl;

		// Get Filename Settings
		vector<string> filenameVals;
		if (isSettingsInFileName) {
			filenameVals = getFilenameVals(fileList[r]);
		}
		else {
			filenameVals = vector<string>({ to_string(r) });
		}

		for (int i = 0;i<(int)spar.f.size();i++)
		{
			if (spar.S[0].rows() == 1)
				sprintf(line, "%lf,%le,%le", spar.f[i], spar.S[i](0, 0).real(), spar.S[i](0, 0).imag());
			else if (spar.S[0].rows() == 2)
				sprintf(line, "%lf,%le,%le,%le,%le,%le,%le,%le,%le", spar.f[i],
					spar.S[i](0, 0).real(), spar.S[i](0, 0).imag(),
					spar.S[i](1, 0).real(), spar.S[i](1, 0).imag(),
					spar.S[i](0, 1).real(), spar.S[i](0, 1).imag(),
					spar.S[i](1, 1).real(), spar.S[i](1, 1).imag());
			else if (spar.S[0].rows() == 4)
				sprintf(line, "%lf,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le,%le", spar.f[i],
					spar.S[i](0, 0).real(), spar.S[i](0, 0).imag(),
					spar.S[i](0, 1).real(), spar.S[i](0, 1).imag(),
					spar.S[i](0, 2).real(), spar.S[i](0, 2).imag(),
					spar.S[i](0, 3).real(), spar.S[i](0, 3).imag(),
					spar.S[i](1, 0).real(), spar.S[i](1, 0).imag(),
					spar.S[i](1, 1).real(), spar.S[i](1, 1).imag(),
					spar.S[i](1, 2).real(), spar.S[i](1, 2).imag(),
					spar.S[i](1, 3).real(), spar.S[i](1, 3).imag(),
					spar.S[i](2, 0).real(), spar.S[i](2, 0).imag(),
					spar.S[i](2, 1).real(), spar.S[i](2, 1).imag(),
					spar.S[i](2, 2).real(), spar.S[i](2, 2).imag(),
					spar.S[i](2, 3).real(), spar.S[i](2, 3).imag(),
					spar.S[i](3, 0).real(), spar.S[i](3, 0).imag(),
					spar.S[i](3, 1).real(), spar.S[i](3, 1).imag(),
					spar.S[i](3, 2).real(), spar.S[i](3, 2).imag(),
					spar.S[i](3, 3).real(), spar.S[i](3, 3).imag());
			summaryFile << line;

			// Write Filename Settings
			for (auto const& var : filenameVals) {
				summaryFile << "," << var;
			}

			summaryFile << endl;

		}
	}

	summaryFile.close();
	logFile.close();


	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);

	int microseconds = duration.count();

	cout << "Time taken by function: "
		<< duration.count() << " microseconds" << endl;
	cin.get();

    return 0;
}

