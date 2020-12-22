#include <string>
#include <sstream>
#include <fstream>

int main() {

	std::string key("MOMENTOANGOLARE");
	std::ifstream in("movements.csv");
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
	
	std::ofstream out("movements.csv");
	out << value;
	out.close();
	
	return 0;
}
