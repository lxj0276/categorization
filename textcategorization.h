#ifndef  _TEXTCATEGORIZATION_H_
#define _TEXTCATEGORIZATION_H_
#include "../commom/func.h"
#include "../commom/model.h"
#include "../commom/textanalysis.h"
#include <algorithm>
#include<math.h>
#include<string>
#include<map>
#include<vector>
#include "regex.hpp"
namespace wxy{
	class DictClassfy{
	public:
		Func f;
		int typenum;
		vector<map<string, float> >worddict;
		map<int, string>tagdict;
	public:
		DictClassfy();
		~DictClassfy();
		int Classfy(string& str, int value);
		bool LoadDict(int x, const char* dictdir);		
	};
	class SimHashClassfy{
	public:
		Func f;
		Hash hash;
		int typenum;
		vector<map<uint64_t, int> >samplist;

	public:
		SimHashClassfy();
		~SimHashClassfy();
		bool Init(int x, const char* idfpath, const char* vecpath, const char* sampdir);
		pair<int, float> Classfy(string& str, float value);
	};

class DocVecClassfy{
	public:
		Func f;
		int typenum;
		Textvector myvec;
		Textvector* docvecs;
		vector<map<string, float >  >words;
	public:
		DocVecClassfy();
		~DocVecClassfy();
		bool  Init();
		//int Classfy(string& str, float value);
		pair<int, float> KeyWordsClassfy(string& str, float value);
		//bool LoadDict(int x, const char* dictdir);		
		bool LoadWords(int x, const char * dictdir);
	};


	class Categorization{
	public:
		Func f;
		DictClassfy dict;
		SimHashClassfy hash;
		DocVecClassfy doc;
	public:
		Categorization();
		~Categorization();
		bool init();
		string Categorizate(string& str);
	};
}
#endif
