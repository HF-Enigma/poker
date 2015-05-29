CC = g++

CFLAGS = -std=c++11

INCLUDE = .

sources : main.cpp

headers : 

objects : 
%: %.cpp
	%(CC) -I$(INCLUDE) $(CFLAGS) -c -o $< $@ 

.PHONY : clean
clean:
	-rm 
