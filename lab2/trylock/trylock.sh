#!/bin/bash
g++ -c trylock.cpp
g++ -o trylock trylock.o -lpthread
