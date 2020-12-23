#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

int main() {

	std::string key("MOMENTOANGOLARE");
	std::string toconvert;
	std::string directory = "persistent/";
	std::cout << "Insert file name to convert: ";
	std::cin >> toconvert;
	std::string input;
	std::string output;
	std::cout << "Conversion type:" << std::endl;
	std::cout << "1. dat to csv		2. csv to dat" << std::endl;
	int option;
	std::cin >> option;
	if(option == 1){
		input = directory + toconvert + ".dat";
		output = directory + toconvert + ".csv";
	}
	else if(option == 2){
		input = directory + toconvert + ".csv";
		output = directory + toconvert + ".dat";
	}
	std::ifstream in(input);
	std::stringstream ss;
	ss << in.rdbuf();
	std::string value(ss.str());
	in.close();

	std::string fullKey(key);
	// XOR cipher works if key is at least as long as message
	while(fullKey.size() < value.size()) {
		fullKey += key;
	}

	for(std::string::size_type i = 0; i < value.size(); ++i) {
		value[i] ^= fullKey[i % fullKey.size()];
	}

	std::ofstream out(output);
	out << value;
	out.close();

	return 0;
}
