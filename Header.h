#ifndef HEADER_H
#define HEADER_H
#include<iostream>
#include<string>
#include<vector>
#include<list>
#include<fstream>

int FoundIndex(std::string, char);
std::string sr(std::string, int, int);
bool Error_range(int, int, int);
void make_int(std::string, int*, int*);
void make_str(std::string, std::string*, int*, int);

std::string NameFail(std::string, std::string, int);
void change_fail(std::string, int, int, std::string, std::string*, bool);
std::string Print(std::string, std::string, std::string);

#endif
