#include "textcategorization.h"
namespace wxy{
	/**/
	bool com(const pair<int, int>& x, const pair<int, int>& y ){
		return x.first < y.first;
	}
	template <class T> int maxarray(T* prob, int n) {
		if((n < 1 )||(prob == NULL))return -1;
		T temp = prob[0];	int index =0;
		for(int i =0; i< n; i++){
			if(temp < prob[i] ){
				temp = prob[i]; 	index = i;
			}
		}
		return index;
	}

	template <class T> int minarray(T* prob, int n) {
		if((n < 1 )||(prob == NULL))return -1;
		T temp = prob[0];	int index =0;
		for(int i =0; i< n; i++){
			if(temp > prob[i] ){
				temp = prob[i]; 	index = i;
			}
		}
		return index;
	}

	DictClassfy::DictClassfy(){
		typenum = 0;
	}
	DictClassfy::~DictClassfy(){
		worddict.clear();
	}

	/*加载关键词词典*/
	bool DictClassfy::LoadDict(int x, const char* dictdir){
		typenum = x;
		map<string, float>p;
		for(int i = 0; i< x; i++){
			p.clear();
			string path = dictdir+ f.convert2str(i+1) +".list";
			FILE *pf = fopen(path.c_str(),"r");
			if(pf == NULL)	continue;			
			char buffer[MAX_LENTH];	
			vector<string> ret;
			while ( f.readline(buffer,MAX_LENTH,pf)!=NULL){
				string str = f.getstr(buffer);
				if((str == "")||(str == " ")){
					continue;
				}

				f.split(" ", str , ret);
				if( ret.size() == 0 )continue;
				if((ret.at(0) == " ")||(ret.at(0) == ""0)) continue
				else if( ret.size() == 2 )	{p[ret.at(0)] = atof(ret.at(1).c_str());
				else 	p[ret.at(0)] = 1;
			}
			worddict.push_back(p);
			fclose(pf);	 
		}
		string path =string( dictdir)+"dict.list";
		FILE *pf = fopen(path.c_str(),"r");
		char buffer[MAX_LENTH];	
		vector<string> ret;
		while ( f.readline(buffer,MAX_LENTH,pf)!=NULL){
			string str = f.getstr(buffer);
			f.split(" ", str , ret);
			if( ret.size() == 2 )	tagdict[atoi(ret.at(0).c_str())] = ret.at(1);
		}
		fclose(pf);	
		return true;
	}

	/*词典匹配分类*/
	int DictClassfy::Classfy(string& str, int value){
		float* score = new float[typenum];
		memset( score,0,sizeof(float)*typenum );
		vector<string> words; 	f._split(" ", str , words);
		for(int i = 0;  i< typenum;  i++){		
			for(int j =0;  j< words.size();  j++){
				if( worddict[i].find(words.at(j)) !=  worddict[i].end() ) {
					score[i] += worddict[i][words.at(j)];
				}
			}
		}
		score[6] /=2.5;
		int maxindex = maxarray(score,typenum) + 1;
		int maxscore = score[maxindex-1];
		if(maxindex == 7){
			score[maxindex-1] = 0;
			int secondindex = maxarray(score,typenum) + 1;
			int secondscore = score[secondindex-1];
			if((secondscore > value +1)&&(maxscore < 3*secondscore)) return secondindex;
		}
		if( maxscore <= value )	return 0;
		else	return maxindex;
	}

	SimHashClassfy::SimHashClassfy(){}
	SimHashClassfy::~SimHashClassfy(){}

	//初始化
	bool SimHashClassfy::Init(int x, const char* idfpath, const char* vecpath, const char* sampdir){
		typenum = x;
		hash.LoadIdf(idfpath);
		hash.LoadVector(vecpath);
		for(int i =0; i< x; i++){
			map<uint64_t, int>dict;
			string path = sampdir + f.convert2str(i+1)+".dict";
			hash.LoadSample(path.c_str(), dict);
			samplist.push_back(dict);
		}
		return true;
	}

	//SIMHASH分类
	pair<int, float> SimHashClassfy::Classfy(string& str, float value){
		uint64_t k = hash.SimHash(str, 3);
		float* dis = new float[typenum];
		memset(dis, 0, sizeof(float)*typenum);
		for(int i =0; i< typenum; i++){
			dis[i] = hash.EverageDiatance(k,samplist.at(i));
		}
		int index = maxarray(dis, typenum);
		pair<int, float>temp;
		temp.first = index +1;
		temp.second = dis[index];
		if( dis[index] < value){
			temp.first = 0;
			temp.second = 0;
		}
		return temp;
	}


	DocVecClassfy::DocVecClassfy(){
		typenum = 0;
		docvecs = NULL;
	}

	DocVecClassfy::~DocVecClassfy(){
		delete docvecs;
	}

	bool DocVecClassfy::Init(){
		myvec.LoadVector(200, "./dict/vector/vec200");
		return true;
	}

	//load vec dict
	/*
	bool DocVecClassfy::LoadDict(int x, const char* dictdir){
		if( x <2 ) return false;
		typenum = x;
		docvecs = new Wordvector[typenum];
		for( int i =0;  i< typenum;  i++ ){
			string dictpath = dictdir + f.convert2str(i+1) + ".dict";
			if( docvecs[i].Loadvector(200,dictpath.c_str()) == false ){
				return false;
			}
		}
		return true;
	}
	*/
	bool DocVecClassfy::LoadWords(int x, const char * dictdir){
		if( x <2 ) return false;
		cout<<"LoadDict"<<endl;
		typenum = x;
		map<string, float>p;
		for(int i = 0; i< x; i++){
			p.clear();
			string path = dictdir+ f.convert2str(i+1) +".list";
			FILE *pf = fopen(path.c_str(),"r");
			if(pf == NULL) {
				words.push_back(p);
				continue;
			}
			char buffer[MAX_LENTH];	
			vector<string> ret;
			while ( f.readline(buffer,MAX_LENTH,pf)!=NULL){
				string str = f.getstr(buffer);
				f._split(" ", str , ret);
				if( ret.size() == 0 )continue;
				if( ret.size() == 2 )	p[ret.at(0)] = atof(ret.at(1).c_str());
				else	p[ret.at(0)] = 1;
			}
			words.push_back(p);
			fclose(pf);	
		}
		cout<<words.size()<<endl;
		return true;
	}

	//短文本向量分类
	/*
	int DocVecClassfy::Classfy(string& str, float value){
		int n = typenum;
		float* v = new float[myvec.m_size];
		memset( v,0,sizeof(float)*myvec.m_size );
		vector<wdlen> vec;
		float* d = new float[n];
		memset( d,0,sizeof(float)*n );
		myvec.train(v,str, 1);
		for( int i =0;  i< n;  i++ ){
			docvecs[i].topcosin(v,vec,15);		
			for( int j =0;  j<15; j++ )	d[i] += vec.at(j).d;
			d[i]/= 15;
		}
		int y = maxarray(d,n) +1;
		float s = d[y-1];
		delete d;
		delete v;		
		if( s < value ){
			return 0;
		}else{
			return y;
		}
		return 0;
	}
	*/
	//关键词向量分类
	pair<int, float> DocVecClassfy::KeyWordsClassfy(string& str, float value){
		//cout<<"dict classfy"<<endl;
		int n = typenum;
		float* v = new float[myvec.m_size];
		memset( v,0,sizeof(float)*myvec.m_size );
		vector<wdlen> vec;
		float* d = new float[n];
		memset( d,0,sizeof(float)*n );
		myvec.Train(v,str, 1);
		//cout<<str<<endl;
		for( int i =0;  i< n;  i++ ){
			//cout<<"i"<<i<<endl;
			int num = 0;
			for(map<string, float >::iterator it = words.at(i).begin(); it != words.at(i).end(); it++){				
				float score = myvec.Distance(v, it->first);
				//cout<<score<<endl;				
				if(score != 0){
					num++;
					d[i] += score;
				}
			}
			if(num == 0){
				d[i] = 0;
			}
			else{
				d[i] /= num;
			}			
		}
		delete v;
		int index = maxarray(d,n);
		pair<int, float>temp;
		temp.first = index +1;
		temp.second = d[index];
		if( d[index] < value){
			temp.first = 0;
			temp.second = 0;
		}
		return temp;
	}
	/*

	WordVecClassfy::WordVecClassfy(){
		typenum =0;
	}
	WordVecClassfy::~WordVecClassfy(){}
	*/
	Categorization::Categorization(){
		init();
	}

	Categorization::~Categorization(){}
	bool Categorization::init(){		
		dict.LoadDict(25,"./dict/classfication/wordlist/");
		//hash.Init(25, "./dict/vector/idf", "./dict/vector/vec200", "./dict/classfication/strsample/");		
		//doc.Init();
		//doc.LoadWords(25,"./dict/classfication/keywordlist/");
		
	}
	string Categorization::Categorizate(string& str){
		string classtag = "",  tag = "dict";
		vector<string> ret;
		f._split(" ", str , ret);

		int classtype =  0;
		
		float value_first = 2, value_second = 1, value_third = 0, alpha = 0.1;
		if( ret.size() >= 30 ){
			classtype =dict.Classfy(str, value_first);
		}
		else if(ret.size() >= 15){
			classtype =dict.Classfy(str, value_first);
			if( classtype == 0 ){
				classtype =dict.Classfy(str, value_second);
			}
		}
		else if(ret.size() >=5){
			classtype =dict.Classfy(str, value_second);
		}
		else{
			classtype =dict.Classfy(str, value_third);
		}

		//SIMHASH
		/*
		if(classtype == 0){
			classtype = hash.Classfy(str,0.04).first;;
			tag = f.convert2str(hash.Classfy(str,0.04).second);
		}
		*/
		/*
		if(classtype == 0){
			pair<int, float> temp = doc.KeyWordsClassfy(str,0.1);
			classtype = temp.first;
			tag = f.convert2str(temp.second);
		}			
		*/
		classtag= dict.tagdict[classtype];
		return classtag + " " + tag;
	}
}
