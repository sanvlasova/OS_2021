#!/bin/bash
g++ -c lab72.cpp
g++ -o lab72 lab72.o -lpthread -lrt

sudo setcap cap_sys_resource=eip lab72
