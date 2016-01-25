all:
	g++ -o main main.cpp textcategorization.cpp ../commom/func.cpp ../commom/func.h  -I /home/seanxywang/app/include/ -L$BOOST_LIB -D_FILE_OFFSET_BITS=64 	
	g++ -fPIC typecategorization.cpp -o textclassfy.so textcategorization.cpp ../wordseg/seg.cpp ../wordseg/hmm.cpp ../wordseg/tree.cpp ../commom/func.cpp ../commom/func.h -shared -I /home/seanxywang/app/include/python2.7/ -I /home/seanxywang/app/lib/python2.7/config/ -L /home/seanxywang/app/lib/python2.7/config/
clean:
	$(RM) -rf main textclassfy.so
