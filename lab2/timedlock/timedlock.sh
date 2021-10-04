#!/bin/bash
g++ -c timedlock.cpp
g++ -o timedlock timedlock.o -lpthread
