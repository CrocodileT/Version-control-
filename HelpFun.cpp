#include "Header.h"
/*
Вспомогательные функции
*/

//Поиск элемента в строке
int FoundIndex(std::string s, char c) {
	int i = 0;
	while (s[i] != c) { i++; }
	return (i);
}

//Срез строки
std::string sr(std::string s, int fs, int ls) {
	std::string res;
	if (fs + 1 == ls) { res += s[fs]; }
	else {
		for (int i = fs; i < ls; i++) { res += s[i]; }
	}
	return (res);
}

//Проверяем на то, допустимы ли индексы указанные в какой-то введеной пользоваетелем команде
bool Error_range(int len, int i, int j) {
	if (i > len || i < 0 || j > len || j < 0) {
		std::cout << "Error range:" << std::endl;
		std::cout << "\t Change fail or change command" << std::endl;
		return (false);
	}
	return (true);
}

//Переводит строку в число
void make_int(std::string s, int *num, int *ind) {
	std::string l;
	int i = *ind;
	while (s[i] != ';') { l += s[(i++)]; }
	*ind = i + 1;
	*num = stoi(l);
	return;
}

//Переводит число в строку
void make_str(std::string s, std::string *data, int *ind, int size) {
	int i = 0;
	while (i < size) { *data += s[*ind + i++]; }
	*ind += i + 1;
	return;
}
