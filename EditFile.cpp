#include "Header.h"

/*
 _Функции печати и редактирования
*/

//Образование правильного имени файла
std::string NameFail(std::string name, std::string version, int leng) {
	std::string res;
	int i = FoundIndex(name, '.');
	res += sr(name, 0, i) + version + sr(name, i, leng);
	return res;
}

//После использования комнады запоминает модификации
//и процедурно сохраняем их в указатель на строку изменений
void change_fail(std::string command, int i, int j, std::string data, std::string *string_change, bool ck_op) {
	if (!ck_op) {
		std::cout << "Fail didn't open!" << std::endl;
	}
	else {
		if (command == "=") {
			*string_change += "-" + std::to_string(i) + (";" + std::to_string(j)) + ";" + "+" + std::to_string(i) + ";" + std::to_string(size(data)) + (";" + data) + "|";
			*string_change += "=" + std::to_string(i) + (";" + std::to_string(j)) + (";" + sr(data,0,j-i)) + "|";
		}
		else if (command == "-") {
			*string_change += "-" + std::to_string(i) + (";" + std::to_string(j)) + ";";
		}
		else if (command == "+") {
			*string_change += "+" + std::to_string(i) + ";" + std::to_string(size(data)) + (";" + data) + "|";
		}
	}
}

//Печатается или возвращает данные в версии
std::string Print(std::string string_change, std::string main_string, std::string com) {
	int leng_str_change = size(string_change),
		len_main = 0,
		i = 0,
		j = 0,
		ind,
		jnd;
	std::string  l,
		data;
	bool in_com = false;
	char command;

	while (i < leng_str_change) {
		len_main = size(main_string);
		if (in_com == false) {
			command = string_change[i++];
			in_com = true;
		}
		else {
			if (command == '=') {
				//разбираем то какие инструкции в данном изменении:
				//определяем индекс(ы) изменения
				//определяем само изменение
				//и производим изменения над main_string
				make_int(string_change, &ind, &i);
				make_int(string_change, &jnd, &i);
				make_str(string_change, &data, &i, jnd-ind);
				if (Error_range(len_main, ind, jnd)) {
					if (size(data) > jnd - ind) {

					}
					for (j = ind; j < jnd; j++) { main_string[j] = data[j - ind]; }
				}
				data.clear();
				in_com = false;
			}

			if (command == '+') {
				make_int(string_change, &ind, &i);
				make_int(string_change, &jnd, &i);
				make_str(string_change, &data, &i, jnd);

				if (Error_range(len_main, ind, 0)) {
					main_string.insert(ind, data);
				}
				data.clear();
				l.clear();
				in_com = false;
			}

			if (command == '-') {
				make_int(string_change, &ind, &i);
				make_int(string_change, &jnd, &i);

				if (Error_range(len_main, ind, jnd - 1)) {
					main_string.erase(ind, jnd - ind);
				}
				l.clear();
				in_com = false;
			}
		}
	}
	if (com == "print") {
		std::cout << "   Text:" << std::endl;
		std::cout << "\t" << main_string << std::endl;
	}
	return(main_string);
}
