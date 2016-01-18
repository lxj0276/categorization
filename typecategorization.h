#ifndef  _TYPECATEGORIZATION_H_
#define _TYPECATEGORIZATION_H_

#include "../commom/func.h"
#include "textcategorization.h"
#include "../textanalysis/textanalysis.h"
namespace classfy{
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
