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
	/*¼ÓÔØ¹Ø¼ü´Ê´Êµä*/
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
		//commom::DEBUG_INFO(path);
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
		return true;
	}

	/*´ÊµäÆ¥Åä·ÖÀà*/
	int DictClassfy::Classfy(std::string& str, int value){
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
		score[6] /=2.5;
		int maxindex = f.MaxArray(score,typenum) + 1;
		int maxscore = score[maxindex-1];
		std::cout<<maxindex<<"  "<<maxscore<<"   "<<value<<"  "<<std::endl;
		if(maxindex == 7){
			score[maxindex-1] = 0;
			int secondindex = f.MaxArray(score,typenum) + 1;
			int secondscore = score[secondindex-1];
			if((secondscore > value +1)&&(maxscore < 3*secondscore)) return secondindex;
		}
		if( maxscore <= value )	return 0;
		else	return maxindex;
	}


	std::string DictClassfy::Categorizate(std::string& str){
		std::vector<std::string> vec;
		f._Split(" ", str , vec);
		int classtype =  0;		
		float value_first = 2, value_second = 1, value_third = 0, alpha = 0.1;
		if( vec.size() >= 30 ){
			classtype =Classfy(str, value_first);
		}
		else if(vec.size() >= 15){
			classtype =Classfy(str, value_first);
			if( classtype == 0 ){
				classtype =Classfy(str, value_second);
			}
		}
		else if(vec.size() >=5){
			classtype =Classfy(str, value_second);
		}
		else{
			classtype =Classfy(str, value_third);
		}
		return tagdict[classtype];
	}





	Feature::Feature(){
		Init();
		//TODO
		for(int i = 0; i< dict.worddict.size(); i++){
			for(std::map<std::string, float>::iterator it = dict.worddict.at(i).begin(); it!= dict.worddict.at(i).end(); it++){
				hashmap[it->first]++;
			}
		}
		int k = 0;
		for(std::map<std::string, int>::iterator it = hashmap.begin(); it != hashmap.end(); it++){
			hashmap[it->first] = k++;
		}
	}
	Feature::~Feature(){}
	bool Feature::Init(){		
		dict.LoadDict(25,"./dict/classfication/wordlist/");
		//commom::DEBUG_INFO(f.ConvertToStr(dict.worddict.size()));
		return true;
	}
	bool Feature::GetSample(const char* infile, const char* samplefile){
		FILE *fi = fopen(infile,"r");
		FILE *fo= fopen(samplefile,"ab+");
		if(fi == NULL){
			commom::LOG_INFO("open file error");
		}			
		char buffer[MAX_LENTH];			
		while ( f.ReadLine(buffer,MAX_LENTH,fi)!=NULL){
			std::string str = f.GetLine(buffer);
			str = f.ConvertToStr(Categorizate(str)) + " " + str + "\n";
			f.WiteLine(str.c_str(), fo);
		}
		fclose(fi);	 
		fclose(fo);
		return true;
	}
	bool Feature::CreatFeature(const char* infile, const char* featurefile){
		return true;
	}
	int Feature::Categorizate(std::string& str){
		std::vector<std::string> vec;
		f._Split(" ", str , vec);
		//commom::DEBUG_INFO(f.ConvertToStr(vec.size()));
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
		//std::cout<<classtype<<std::endl;
		return classtype;
	}

}
