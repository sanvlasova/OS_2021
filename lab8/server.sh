#!/bin/bash
g++ -c server.cpp
g++ -o server server.o -lpthread -lrt
