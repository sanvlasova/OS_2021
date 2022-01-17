#!/bin/bash
g++ -c parent.cpp
g++ -c child.cpp

g++ -o parent parent.o -lpthread
g++ -o child child.o -lpthread

./parent
