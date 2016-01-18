#include <boost/python.hpp>
#include "../commom/func.h"
#include "../wordseg/seg.h"
#include "textcategorization.h"
#include "../textanalysis/textanalysis.h"
classfy::DictClassfy dict;
seg::Wordseg mseg;
commom::Func f;
bool Init(){
	dict.LoadDict(5,"./dict/classfication/typelist/");
}
int Categorizate(const char* s){
	std::string str = s;
	std::vector<std::string> vec;
	f._Split(" ", str , vec);
	int classtype =  0;		
	float value_first = 2, value_second = 1, value_third = 0, alpha = 0.1;
	if( vec.size() >= 30 ){
		classtype =dict.Classfy(str, value_first);
	}
	else if(vec.size() >= 15){
		classtype =dict.Classfy(str, value_first);
		if( classtype == 0 ){
			classtype =dict.Classfy(str, value_second);
		}
	}
	else if(vec.size() >=5){
		classtype =dict.Classfy(str, value_second);
	}
	else{
		classtype =dict.Classfy(str, value_third);
	}
	return classtype;
}
BOOST_PYTHON_MODULE(classfy){
	boost::python::def("init", Init,	"Translate message.");
	boost::python::def("categorizate", Categorizate,	boost::python::args("s"), "Translate message.");
}
