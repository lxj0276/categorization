all:
	g++ -o main main.cpp textcategorization.cpp ../commom/func.cpp ../commom/func.h  -I /home/seanxywang/app/include/ -L$BOOST_LIB -D_FILE_OFFSET_BITS=64 	
clean:
	$(RM) -rf test
