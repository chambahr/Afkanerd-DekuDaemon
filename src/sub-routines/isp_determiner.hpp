#ifndef ISP_DETERMINER_H_INCLUDE_
#define ISP_DETERMINER_H_INCLUDE_
using namespace std;

namespace isp_determiner {
	//TODO: Create dynamic version of get_isp
	//TODO: Add parsers for country code values
	string get_isp( string number ) {
		if(number[0] == '6') {
			switch(number[1]) {
				case '5':
					switch(number[2]) {
						case '0':
						case '1':
						case '2':
						case '3':
						case '4':
							return "MTN";
						break;

						case '5':
						case '6':
						case '7':
						case '8':
						case '9':
							return "ORANGE";
						break;

					}
				break;

				case '6': return "NEXTTELL";

				case '7': return "MTN";
				break;

				case '8':
					  switch(number[2]) {
						case '0':
						case '1':
						case '2':
						case '3':
						case '4':
							return "MTN";
						break;
					  }
				break;

				case '9': return "ORANGE";
				break;
			}
		}
		return "UNKNOWN";
	}
}


#endif