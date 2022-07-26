#ifndef ASN1KEY
#define ASN1KEY
#include<string>


class ASN1Key{


	public:
		ASN1Key();
		~ASN1Key();

	std::string parseDSA();
	std::string parseRSA();
};


#endif
