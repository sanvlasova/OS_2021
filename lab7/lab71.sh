#!/bin/bash
g++ -c lab71.cpp
g++ -o lab71 lab71.o -lpthread -lrt

sudo setcap cap_sys_resource=eip lab71

