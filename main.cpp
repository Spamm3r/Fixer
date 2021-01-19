//g++ -o fixer main.cpp -std=c++11 -lcurl -ljsoncpp

#include <curl/curl.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <json/json.h>

#define clean system("clear");
#define pause system("read");

static std::string buffer;

class ValueTown
{
private:
	std::string values[31] = {"EUR", "AUD", "BRL", "CAD", "CHF", "CNY", "CZK", "DKK", "GBP",//9
			"HKD","HRK", "HUF", "IDR", "ILS", "INR", "JPY", "KRW", "MXN", "MYR", "NOK",//11
			"NZD", "PHP", "PLN", "RON", "RUB", "SEK", "SGD", "THB", "TRY", "USD", "ZAR"};//11

	std::string base, target, conval, buf, assembly="http://api.fixer.io/latest?base=";
	int conv, k = 0, j = 0;

public:

static int writer(char *data, size_t size, size_t nmemb, std::string *buffer) {
  int result = 0;
  if (buffer != NULL) {
    buffer->append(data, size * nmemb);
    result = size * nmemb;
  }
  return result;
}

void showCurrencies()
{
	std::cout << "Currencies: " << std::endl;
	for(int i=0; i<31; i++)
	{
		std::cout << i+1 << "." << values[i];
			if (i%3==0)
				std::cout << std::endl;
			else
				std::cout << " ";
	}
}

int recursiveCheck()
{
	if(j == 1 && k == 1)
	{
		std::cout << "Tell me how much money you want to convert: ";
		std::cin >> conv;
		assembly += base;
		assembly += "&symbols=";
		assembly += target;
		return 1;
	}
	else
	{
		clean;
		showCurrencies();
		std::cout << "Please insert an 3 digit symbol of base currency: ";
		std::cin >> base;
		std::cout << "Please insert an 3 digit symbol of target currency: ";
		std::cin >> target;
		for(int i=0; i<31; i++)
		{
			if(values[i]==base && values[i]!=target)
				j=1;
			else if(values[i]==target && values[i]!=base)
				k=1;
		}
		return recursiveCheck();
	}
}

int engine()
{
	clean;
	CURL *curl = curl_easy_init();
	if(curl) {
		std::cout << "    ___________  __ __________ " << std::endl;
		std::cout << "   / ____/  _/ |/ // ____/ __ \\" << std::endl;
		std::cout << "  / /_   / / |   // __/ / /_/ /" << std::endl;
		std::cout << " / /_   / / /   // __/ / /_/ /" << std::endl;
		std::cout << "/_/   /___//_/|_/_____/_/  |_| " << std::endl;
		std::cout << "Made by Tomek Kaczmarzyk" << std::endl;
		pause;
		recursiveCheck();
		Json::Value root;
		Json::Reader reader;
		Json::FastWriter fastWriter;
		curl_easy_setopt(curl, CURLOPT_URL, assembly.c_str());
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);//verboose
		buffer.clear();
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
		curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		bool parsingSuccessfull = reader.parse(buffer.c_str(), root);
		if(!parsingSuccessfull) {
			clean;
			std::cout << "There was problem with json: " << reader.getFormattedErrorMessages();
			return 0;
		}
		buf = fastWriter.write(root["rates"]);
		for(int i=0; i < buf.size(); i++)
			if(isdigit(buf[i]) || buf[i] == '.')
				conval += buf[i];
		//std::cout << assembly << std::endl; //an address off json file
		std::cout << "Target cost " << atof(conval.c_str()) << " " << base << std::endl;
		std::cout << conv << " " << base << " is " << conv*atof(conval.c_str()) << " " << target << std::endl;
		//std::cout << buffer << std::endl << assembly << std::endl; 
		return 1;
	}
	else
		clean;
		std::cout << "There was problem with curl!";
	return 0;
}

};
int main() {
	ValueTown value;
	value.engine();
}