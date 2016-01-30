#ifndef  _TEXTCATEGORIZATION_H_
#define _TEXTCATEGORIZATION_H_
#include "../commom/func.h"
#include "../textanalysis/textanalysis.h"
namespace classfy{
class DictClassfy{
	public:
		int typenum;
		commom::Func f;		
		text::Hash mhash;
		std::map<int, std::string>tagdict;
		std::vector<std::map<std::string, float> >worddict;		
	public:
		DictClassfy();
		DictClassfy(int n, const char* path);
		~DictClassfy();
		bool Classfy(std::string& str, float value, int& type);
		bool _Classfy(std::string& str, float value, int& type);
		bool LoadDict(int x, const char* dictdir);		
		bool _LoadDict(int x, const char* dictdir);		
		bool  Categorizate(std::string& typestr , std::string& str);
};
}
#endif
