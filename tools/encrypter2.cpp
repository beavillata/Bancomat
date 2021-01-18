#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

//PER COMPILARE: make encryption -> non c'è più bisogno di modificare questi dati!
//Ho aggiunto il cin quindi dovremmo essere salvi
//Visto che di default il compilatore sputa fuori un a.out che fa casino con quello del
//programma principale ho modificato il make -> ora sputa fuori encrypter!
//PER ESEGUIRE: ./encrypter

const std::string KEY = "MOMENTOANGOLARE",
	DIRECTORY = "persistent/";

int main() {

	std::string files[] = {"accounts", "credentials", "external", "movements"};
	std::string file;
	int option;
	std::cout << "Conversion type:" << std::endl;
	std::cout << "[1] DAT to CSV \t [2] CSV to DAT" << std::endl;
	std::cin >> option;


	std::string input, output;
	for(int i=0;i<4;i++)
	{
		file=files[i];
		switch(option) {
		case 1:
			input = DIRECTORY + file + ".dat";
			output = DIRECTORY + file + ".csv";
			break;
		case 2:
			input = DIRECTORY + file + ".csv";
			output = DIRECTORY + file + ".dat";
			break;
		default:
			return 1;
		}

		std::ifstream in(input);
		std::stringstream ss;
		ss << in.rdbuf();
		std::string value(ss.str());
		in.close();

		// Doing the actual (de)ciphering
		for(int i = 0; i < value.size(); ++i) {
			value[i] ^= KEY[i % KEY.size()];
		}

		std::ofstream out(output);
		out << value;
		out.close();
	}
	return 0;
}
