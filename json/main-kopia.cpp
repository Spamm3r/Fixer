#include <curl/curl.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <json/json.h>

class ValueTown
{
public:
static std::string buffer;

static int writer(char *data, size_t size, size_t nmemb, std::string *buffer) {
  int result = 0;
  if (buffer != NULL) {
    buffer->append(data, size * nmemb);
    result = size * nmemb;
  }
  return result;
}

static void engine()
{
	CURL *curl = curl_easy_init();
	if(curl) {
		std::string base, target, conval, buf, assembly="http://api.fixer.io/latest?base=";
		int conv;
		Json::Value root;
		Json::Reader reader;
		Json::FastWriter fastWriter;
		std::cout << "Podaj symbol z jakiego chcesz konwertowac: ";
		std::cin >> base;
		std::cout << "Podaj symbol do jakiego chcesz konwertowac: ";
		std::cin >> target;
		std::cout << "Podaj kwote: ";
		std::cin >> conv;
		assembly += base;
		assembly += "&symbols=";
		assembly += target;
		curl_easy_setopt(curl, CURLOPT_URL, assembly.c_str());
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
		buffer.clear();
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		bool parsingSuccessfull = reader.parse(buffer.c_str(), root);
		if(!parsingSuccessfull) {
			std::cout << "Nie mozna odczytac danych z jsona: " << reader.getFormattedErrorMessages();
			return 0;
		}
		buf = fastWriter.write(root["rates"]);
		for(int i=0; i < buf.size(); i++)
			if(isdigit(buf[i]) || buf[i] == '.')
				conval += buf[i];
		std::cout << conv << " " << base << " to " << conv*atof(conval.c_str()) << " " << target << std::endl;
		//std::cout << buffer << std::endl << assembly << std::endl;
	}
	else
		std::cout << "Nie mozna zainicjalizowac silnika HTTP";
}

};
int main() {
	ValueTown value;
	value.engine();
}
