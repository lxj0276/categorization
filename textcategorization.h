#ifndef  _TEXTCATEGORIZATION_H_
#define _TEXTCATEGORIZATION_H_
#include "../commom/func.h"
#include "../textanalysis/textanalysis.h"
namespace classfy{
class DictClassfy{
	public:
		int typenum;
		commom::Func f;		
		std::map<int, std::string>tagdict;
		std::vector<std::map<std::string, float> >worddict;		
	public:
		DictClassfy();
		DictClassfy(int n, const char* path);
		~DictClassfy();
		int Classfy(std::string& str, int value);
		bool LoadDict(int x, const char* dictdir);		
		std::string Categorizate(std::string& str);
};

class Feature{
	public:
		commom::Func f;
		DictClassfy dict;
		std::map<std::string, int>hashmap;
	public:
		bool Init();
		Feature();
		~Feature();
		bool GetSample(const char* infile, const char* samplefile);
		bool CreatFeature(const char* infile, const char* featurefile);
		int Categorizate(std::string& str);
	};
}
#endif
