#include <Python.h>
#include "../commom/func.h"
#include "../wordseg/seg.h"
#include "textcategorization.h"
commom::Func f;
classfy::DictClassfy dict(4,"./dict/classfication/typelist/");
seg::Wordseg mseg;
std::string Categorizate(const char* s){
	std::string str = s;
	commom::DEBUG_INFO("seginput");
	commom::DEBUG_INFO(str);
	std::string str1 = mseg.Segement(str);
	commom::DEBUG_INFO("seg");
	commom::DEBUG_INFO(str1);
	if(str1 == "")return "";
	std::vector<std::string> vec;
	f._Split(" ", str1 , vec);
	commom::DEBUG_INFO(f.ConvertToStr(vec.size()));
	int classtype =  0;		
	float value_first = 2, value_second = 1, value_third = 0, alpha = 0.1;
	if( vec.size() >= 30 ){
		classtype =dict.Classfy(str1, value_first);
	}
	else if(vec.size() >= 15){
		classtype =dict.Classfy(str1, value_first);
		if( classtype == 0 ){
			classtype =dict.Classfy(str1, value_second);
		}
	}
	else if(vec.size() >=5){
		classtype =dict.Classfy(str1, value_second);
	}
	else{
		classtype =dict.Classfy(str1, value_third);
	}
	commom::DEBUG_INFO(f.ConvertToStr(classtype));
	commom::DEBUG_INFO(dict.tagdict[classtype]);
	return dict.tagdict[classtype];
}

//python 包装
static  PyObject* wrap_classfy(PyObject* self, PyObject* args){
	const char* str;
	std::string result;
	if (! PyArg_ParseTuple(args, "s", &str)) //把python的变量转换成c的变量
		return NULL;
	commom::DEBUG_INFO("input");
	commom::DEBUG_INFO(str);
	result = Categorizate(str);
	return Py_BuildValue("s", result.c_str());//把c的返回值n转换成python的对象
}

static PyMethodDef textclassfyMethods[] ={
	{"Categorizate", wrap_classfy, METH_VARARGS, "textclassfy!"}, 
	{NULL, NULL}
};

extern "C"   {
	void inittextclassfy(){
		PyObject* m;
		m = Py_InitModule("textclassfy", textclassfyMethods);
	}

};

