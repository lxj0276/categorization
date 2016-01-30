#include "textcategorization.h"
namespace classfy{
	DictClassfy::DictClassfy(){
		typenum = 0;
	}
	DictClassfy::DictClassfy(int n, const char* path){
		LoadDict(n,path);
	}
	DictClassfy::~DictClassfy(){
		worddict.clear();
	}
	/*加载关键词词典*/
	bool DictClassfy::LoadDict(int x, const char* dictdir){
		typenum = x;
		std::map<std::string, float>p;
		std::vector<std::string> vec;
		for(int i = 0; i< x; i++){
			p.clear();
			std::string path = dictdir+ f.ConvertToStr(i+1) +".list";
			FILE *fi = fopen(path.c_str(),"r");
			if(fi == NULL)	continue;			
			char buffer[MAX_LENTH];			
			while ( f.ReadLine(buffer,MAX_LENTH,fi)!=NULL){
				std::string str = f.GetLine(buffer);
				if((str == "")||(str == " "))continue;
				f.Split(" ", str , vec);
				if(( vec.size() == 0 )||(vec.at(0) == " ")||(vec.at(0) == ""))continue;
				else if( vec.size() == 2 )	p[vec.at(0)] = atof(vec.at(1).c_str());
				else 	p[vec.at(0)] = 1;
			}
			worddict.push_back(p);
			fclose(fi);	 
		}
		std::string path =std::string( dictdir)+"dict.list";
		FILE *fi = fopen(path.c_str(),"r");
		if( fi == NULL){
			commom::DEBUG_INFO("open file error");
		}
		char buffer[MAX_LENTH];	
		while ( f.ReadLine(buffer,MAX_LENTH,fi)!=NULL){
			std::string str = f.GetLine(buffer);
			f.Split(" ", str , vec);
			if( vec.size() == 2 )	tagdict[atoi(vec.at(0).c_str())] = vec.at(1);
		}
		fclose(fi);	
		commom::DEBUG_INFO("LoadDict OK");
		return true;
	}

	bool DictClassfy::_LoadDict(int x, const char* dictdir){
		typenum = x;
		for(int i = 0; i< x; i++){
			std::map<std::string, float>p;
			worddict.push_back(p);
		}
		std::vector<std::string> vec;
		std::string path = std::string(dictdir)+ "list";
		FILE *fi = fopen(path.c_str(),"r");
		if(fi == NULL) {
			commom::DEBUG_INFO("open file error");
			commom::DEBUG_INFO(path);
			return false;
		}
		char buffer[MAX_LENTH];			
		while ( f.ReadLine(buffer,MAX_LENTH,fi)!=NULL){
			std::string str = f.GetLine(buffer);
			if((str == "")||(str == " "))continue;
			f.Split(" ", str , vec);
			if(( vec.size() == 0 )||(vec.at(0) == " ")||(vec.at(0) == ""))continue;
			else if( vec.size() == 3 )	{
				worddict.at(atoi(vec.at(1).c_str()) - 1)[vec.at(0)] = atoi(vec.at(2).c_str());
			}else{
				worddict.at(atoi(vec.at(1).c_str()) -1)[vec.at(0)] =  1;
			}
		}
		fclose(fi);	 
		for(int i = 0; i< x; i++){
			commom::DEBUG_INFO(f.ConvertToStr(worddict.at(i).size()));
		}
		path =std::string( dictdir)+"dict.list";
		fi = fopen(path.c_str(),"r");
		if( fi == NULL){
			commom::DEBUG_INFO("open file error");
		}
		while ( f.ReadLine(buffer,MAX_LENTH,fi)!=NULL){
			std::string str = f.GetLine(buffer);
			f.Split(" ", str , vec);
			if( vec.size() == 2 )	tagdict[atoi(vec.at(0).c_str())] = vec.at(1);
		}
		fclose(fi);	
		commom::DEBUG_INFO("LoadDict OK");
		return true;
	}


	/*词典匹配分类*/
	bool DictClassfy::Classfy(std::string& str, float value, int& type){
		float* score = new float[typenum];
		memset( score,0,sizeof(float)*typenum );
		std::vector<std::string> words; 	
		f._Split(" ", str , words);
		for(int i = 0;  i< typenum;  i++){		
			for(int j =0;  j< words.size();  j++){
				if( worddict[i].find(words.at(j)) !=  worddict[i].end() ) {
					score[i] += worddict[i][words.at(j)];
				}
			}
		}
		//权值调整
		if(typenum > 7){
			score[6] /=2.5;
			int maxindex = f.MaxArray(score,typenum) + 1;
			int maxscore = score[maxindex-1];
			if(maxindex == 7){
				commom::DEBUG_INFO(f.ConvertToStr(maxscore));
				score[maxindex-1] = 0;
				int secondindex = f.MaxArray(score,typenum) + 1;
				int secondscore = score[secondindex-1];
				if((secondscore > value +1)&&(maxscore < 3*secondscore)){
					type = secondindex;
					return true;
				}
			}
			commom::DEBUG_INFO(f.ConvertToStr(maxscore));
			if( maxscore <= value )	{
				type = 0;
				return true;
			}
			else	{
				commom::DEBUG_INFO(f.ConvertToStr(maxindex));
				type = maxindex;
				return true;
			}
		}else{
			if(words.size() <= 5 ){
				score[1] = 0;
			}
			int maxindex = f.MaxArray(score,typenum) + 1;
			int maxscore = score[maxindex-1];
			if(maxindex == 3){
				commom::DEBUG_INFO(f.ConvertToStr(maxscore));
				score[maxindex-1] = 0;
				int secondindex = f.MaxArray(score,typenum) + 1;
				int secondscore = score[secondindex-1];
				if((secondscore > value +1)&&(maxscore < 3*secondscore)){
					type = secondindex;
					return true;
				}
			}
			if( maxscore <= value)	{
				type = 0;
				return true;
			}else{
				type = maxindex;
				return true;
			}			
		}
	}

	bool DictClassfy::_Classfy(std::string& str, float value, int& type){
		float* score = new float[typenum];
		memset( score,0,sizeof(float)*typenum );
		std::vector<std::string> words; 	
		f._Split(" ", str , words);
		for(int i = 0;  i< typenum;  i++){		
			for(int j =0;  j< words.size();  j++){
				std::string wd = mhash.Int64toBinaryStr(mhash.StrHash(words.at(j)));
				if( worddict[i].find(wd) !=  worddict[i].end() ) {
					score[i] += worddict[i][wd];
				}
			}
		}
		//权值调整
		if(typenum > 7){
			score[6] /=2.5;
			int maxindex = f.MaxArray(score,typenum) + 1;
			int maxscore = score[maxindex-1];
			if(maxindex == 7){
				//commom::DEBUG_INFO(f.ConvertToStr(maxscore));
				score[maxindex-1] = 0;
				int secondindex = f.MaxArray(score,typenum) + 1;
				int secondscore = score[secondindex-1];
				if((secondscore > value +1)&&(maxscore < 3*secondscore)){
					type = secondindex;
					return true;
				}
			}
			//commom::DEBUG_INFO(f.ConvertToStr(maxscore));
			if( maxscore <= value )	{
				type = 0;
				return true;
			}
			else	{
				//commom::DEBUG_INFO(f.ConvertToStr(maxindex));
				type = maxindex;
				return true;
			}
		}else{
			if(words.size() <= 5 ){
				score[1] = 0;
			}
			int maxindex = f.MaxArray(score,typenum) + 1;
			int maxscore = score[maxindex-1];
			score[2] /=2.5;
			if(maxindex == 3){
				//commom::DEBUG_INFO(f.ConvertToStr(maxscore));
				score[maxindex-1] = 0;
				int secondindex = f.MaxArray(score,typenum) + 1;
				int secondscore = score[secondindex-1];
				if((secondscore > value +1)&&(maxscore < 3*secondscore)){
					type = secondindex;
					return true;
				}
			}
			if( maxscore <= value)	{
				type = 0;
				return true;
			}else{
				type = maxindex;
				return true;
			}			
		}
	}

	bool DictClassfy::Categorizate(std::string& typestr ,std::string& str){
		std::vector<std::string> vec;
		f._Split(" ", str , vec);
		int classtype =  0;		
		float value_first = 2, value_second = 1, value_third = 0, alpha = 0.1;
		if( vec.size() >= 30 ){
			_Classfy(str, value_first,classtype);
		}
		else if(vec.size() >= 15){
			_Classfy(str, value_first,classtype);
			if( classtype == 0 ){
				_Classfy(str, value_second,classtype);
			}
		}
		else if(vec.size() >=5){
			_Classfy(str, value_second,classtype);
		}
		else{
			_Classfy(str, value_third,classtype);
		}
		//commom::DEBUG_INFO("type");
		//commom::DEBUG_INFO(f.ConvertToStr(classtype));
		typestr =  tagdict[classtype];
		return true;
	}
}
