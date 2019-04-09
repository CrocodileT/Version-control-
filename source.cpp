#define _CRT_SECURE_NO_WARNINGS
#include "Header.h"

bool res_found = false; //Храним результат поиска версии по дереву
std::string res; //Здесь храним промежуточные результаты рекурсии 
bool control = false;//Если значение true функция Swap выключается 

struct Tree {
	int version;
	std::list<Tree> Node;
};

Tree head_tree; //голова дерева

//Само добавление, просто выносим push_back
void AddVersionNode(Tree *now_version, Tree *new_version) {
	now_version->Node.push_back(*new_version);
}

//Добавляем версию в родительский список
void AddVersionTree(int version_pull, Tree *Luke_I_am_your_father, Tree *Luke) {
	if (Luke_I_am_your_father->version == version_pull) {
		AddVersionNode(Luke_I_am_your_father, Luke);
		return;
	}
	else if (Luke_I_am_your_father->Node.size() == 0) { return; }
	else {
		for (auto &i : Luke_I_am_your_father->Node) {
			if (i.version == version_pull) { AddVersionNode(&i, Luke); }
			else { AddVersionTree(version_pull, &i, Luke); }
		}
	}
}

//Проходит по дереву, если находит версию
//Записывает в res_found = true , иначе остается значение false
void FoundVersion(int num_version, Tree *head_version) {
	if (head_version->version == num_version) { res_found = true; return; }
	else if (head_version->Node.size() == 0) { if (res_found != true) { res_found = false; } }
	else {
		for (auto &i : head_version->Node) { FoundVersion(num_version, &i); }
	}
}

//Определяет всех родителей нашей версии
//И записывает их в формате число!число!число!...
void VersionSequence(Tree *head_version, int num_version, std::string r) {
	if (head_version->version == num_version) { res = r; return; }
	else if (head_version->Node.size() == 0) { r.clear(); }
	else {
		r += std::to_string(head_version->version) + "!";
		for (auto &i : head_version->Node) {
			VersionSequence(&i, num_version, r);
		}
	}
}

//Записывает все изменений до нашей версии в all_change
void MakeAllChange(std::string name, Tree *head_version, int num_version, std::string *all_change) {
	std::ifstream in;
	std::string version,
		add_this_str;
	bool in_version = true;
	VersionSequence(head_version, num_version, "");
	if (size(res) != 0) {
		res += std::to_string(num_version) + "!";
	}
	int i = 0,
		len_res = size(res);

	while (i < len_res) {
		if (res[i] == '!') {
			if (version != "0") {
				in.open(NameFail(name, version, size(name)));
				std::getline(in, add_this_str);
				in.close();
				*all_change += add_this_str;
			}
			version.clear(); add_this_str.clear();
		}
		else { version += res[i]; }
		i++;
	}
	res.clear();
}

//Если находим версию, заменяем её на первую ветвь в списке
//Добавляем к списку всех наследников ветви
//И удаляем из списка эту ветвь
void DeleteVersion(int num_version, Tree *head_version) {
	if (head_version->Node.size() == 0) { return; }
	else {
		int count = 0;
		for (auto &i : head_version->Node) {
			if (i.version == num_version) {
				head_version->Node.insert(head_version->Node.end(), i.Node.begin(), i.Node.end());
				head_version->Node.pop_front();
				return;
			}
			else {
				DeleteVersion(num_version, &i);
			}
			count++;
		}
	}
}

//В первой строке хранятся данные из файла,во второй печатная версия родителя
//В итоге заменяем команды на противоположные по смыслу и возвращаем
std::string ChangeFail(std::string data_string, std::string old_data_fail) {
	std::string new_string = "", //Новая строка команда
		str_command = "", //Тут отдельно хранится каждая команда
		command = "",
		string,
		index;//Тут хранятся индексы в формате строки
	int leng_file = size(data_string),
		count = 0, //Счетчик строки
		count_data = 0, //Счетчик параметра data
		now_index = 0,//Счетчик индексов
		index1 = -1, index2 = -1;//Тут хранятся индексы в формате int

	while (count < leng_file) {
		string += data_string[count];
		if (size(command) == 0) {
			if (data_string[count] == '=') { command += '='; str_command += '='; }
			if (data_string[count] == '+') { command += '-'; str_command += '-'; }
			if (data_string[count] == '-') { command += '+'; str_command += '+'; }
		}
		else {
			if (data_string[count] == ';') {
				now_index++;
				if (now_index == 1 && index != "") {
					index1 = stoi(index);
				}
				if (now_index == 2 && index != "") { index2 = stoi(index); }
				if (command[0] == '+' && now_index == 2) {

					std::string s = sr(old_data_fail, index1, index2);
					str_command += std::to_string(size(s)) + ';' + s + '|';

					new_string.insert(0, str_command);
					old_data_fail = Print(string, old_data_fail, "");

					str_command.clear();
					command.clear();
					string.clear();
					now_index = 0;

					string.clear();
				}
				else {
					if (command[0] != '+' && command[0] != '-' || now_index < 2) { str_command += ';'; }
				}
				index.clear();
			}
			else if (data_string[count] <= 57 && now_index < 2) {
				if (command[0] != '+' && command[0] != '-' || now_index < 1) { str_command += data_string[count]; }
				index += data_string[count];
			}
			else {
				if (command[0] == '=') {
					str_command += sr(old_data_fail, index1, index2) + "|";
					new_string.insert(0, str_command);

					for (int i = 0; i < index2 - index1; i++) {
						count++;
						if (count == leng_file) { break; }
						string += data_string[count];
					}
					old_data_fail = Print(string, old_data_fail, "");
					string.clear();

					str_command.clear();
					command.clear();
					now_index = 0;

				}
				if (command[0] == '-') {
					str_command += std::to_string(index1 + index2) + ';';
					new_string.insert(0, str_command);

					for (int i = 0; i < index2; i++) {
						if (count == leng_file - 1) { break; }
						string += data_string[++count];
					}
					old_data_fail = Print(string, old_data_fail, "");

					str_command.clear();
					command.clear();
					now_index = 0;
					string.clear();
				}
			}
		}
		count++;
	}
	return(new_string);
}

//Создает вектор связанных версий без учета той, которую используем сейчас
//Используем глобальную переменную res
void make_int_array(std::vector<int> *vector) {
	std::string num = "";
	int i = 0,
		len_res = size(res);
	while (i < len_res) {
		if (res[i] != '!') { num += res[i]; }
		else { (*vector).push_back(stoi(num)); num.clear(); }
		i++;
	}
	res.clear();
}

//Свапает указатели 2-х передаваемых версий,не заменя указатели с версиями: ver1,ver2
void SwapNode(Tree *head, Tree *father, int version_change1, int ver1, int version_change2, int ver2) {
	if (head->version == version_change2) {
		std::list<Tree> v1, v2;
		v1.splice(v1.begin(), father->Node);//Тут верхний узел
		v2.splice(v2.begin(), head->Node);//Тут нижний узел

		for (auto &i : v1) {
			if (i.version == ver1) {
				AddVersionTree(version_change1, &head_tree, &i);
				break;
			}
		}
		for (auto &i : v2) {
			if (i.version != ver2) { AddVersionTree(version_change1, &head_tree, &i); }
			else { AddVersionTree(version_change2, &head_tree, &i); }
		}
		for (auto &i : v1) {
			if (i.version != ver1) { AddVersionTree(version_change2, &head_tree, &i); }
		}
		control = true;
		return;
	}
	else {
		if (head->version == version_change1) {
			father = head;
		}
		for (auto &i : head->Node) {
			if (!control) {
				SwapNode(&i, father, version_change1, ver1, version_change2, ver2);
			}
			else {
				return;
			}
		}
	}
}

//Перераситывает все данные из переданного пути
//В динамическом массив храним строки, индекс массива совпадает к именем версии
//в векторе хранятся имена версий в порядке связи от 0 до version. 
void Rebase(Tree *head, std::string name, std::string *main_str, int version) {
	std::vector<int> vector;
	std::ifstream in;
	std::ofstream out;
	std::string *versions_fail = new std::string[version + 1],
		num = "",
		data_fail,
		name_fail,
		new_data_fail;

	VersionSequence(head, version, "");

	make_int_array(&vector);
	vector.push_back(version);

	int count_version = 0,
		old_version = 0,
		len_vector = size(vector),
		i = 0;

	while (count_version < len_vector) {
		std::string all_change = "";

		if (count_version == 0) {
			MakeAllChange(name, head, version, &all_change);
			versions_fail[0] = Print(all_change, *main_str, "");
			//Записываем в нулевой индекс печать последней версии 
			out.open(NameFail(name, std::to_string(vector[count_version]), size(name)));
			out << versions_fail[count_version];
			out.close();
		}
		else {
			in.open(NameFail(name, std::to_string(vector[count_version]), size(name)));
			std::getline(in, data_fail);//Дастаёт данные из файла
			in.close();
			//Смотри на то, какая версия была до него и отправляем эту версию в Change
			MakeAllChange(name, head, vector[count_version - 1], &all_change);
			versions_fail[vector[len_vector - count_version]] = ChangeFail(data_fail, Print(all_change, *main_str, ""));

			data_fail.clear();
		}
		count_version++;
	}
	int leng_for = len_vector / 2;
	for (int j = 0; j < leng_for; j++) {
		if (j == 0) {
			SwapNode(&head_tree, &head_tree, vector[j], vector[j + 1], vector[len_vector - j - 1], -1);
		}
		else {
			SwapNode(&head_tree, &head_tree, vector[j], vector[j + 1], vector[len_vector - j - 1], vector[len_vector - j]);
		}
		control = false;
	}

	for (int j = 0; j < len_vector; j++) {
		if (j == 0) { *main_str = versions_fail[vector[0]]; }

		out.open(NameFail(name, std::to_string(vector[j]), size(name)));
		out << versions_fail[vector[j]];
		out.close();
	}
	res.clear();
}

//Функция которая осуществляет пересчет всех сыновей удаляемой версии
//Добавляя во всез сыновей data удаляемой версии
void EditFile(Tree *head, int version, std::string name, bool in_tree, std::string data) {
	if (head->version == version) {
		std::ifstream in;
		in.open(NameFail(name, std::to_string(version), size(name)));
		std::string s;
		std::getline(in, s);
		for (auto &i : head->Node) { EditFile(&i, version, name, true, s); }
		return;
	}
	if (in_tree == true) {
		std::ifstream in;
		std::ofstream out;
		std::string n = NameFail(name, std::to_string(head->version), size(name)),
			s;
		in.open(n);
		std::getline(in, s);
		in.close();
		out.open(n);
		out << data << s;
		out.close();
		for (auto &i : head->Node) { EditFile(&i, version, name, true, data); }
	}
	else if (head->Node.size() != 0) {
		for (auto &i : head->Node) { EditFile(&i, version, name, false, data); }
	}
}

int main() {
	int leng_name_fail = 0;
	int leng_str;
	int version = 0; //В этой переменной храним версию формата int
	int ind; //Индексовые переменные
	int jnd;
	int i = 0;
	int version_pull = 0; //Храним то, к какой версии делаем откат
	bool check_open_fail = false;
	std::ifstream in;
	std::ofstream out;
	std::string name_fail; //Храним полное имя файла
	std::string command; //Храним обозначения команд редактирования в формате: '+', '-', '='
	std::string name_push; //Здесь заранее создаем имя новой версии
	std::string data; //Тут подробности изменения для операция вроде: add index data
	std::string all_change; //Строка с родительскими версиями (команды редактирования)
	std::string str_change; //Все команды редактирования нынешней версии, хранится от нажатия open/push до push
	std::string main_str; //Здесь хранятся базовые рабочие данные
	std::string name_without_version; //тут хранится имя без версии формата : name.txt
	Tree now_tree;  //рабочий элемент дерева

	std::cin >> command;
	while (command != "Finish") {
		res_found = false;
		if (command == "open") {
			std::cin >> name_fail;
			leng_name_fail = size(name_fail);

			name_without_version = sr(name_fail, 0, leng_name_fail);//Имя файла без версии
			name_fail = NameFail(name_without_version, "0", leng_name_fail);//формируем имя файла исходника
			version = 0;//версия в int
			name_push = NameFail(name_without_version, std::to_string(version + 1), leng_name_fail);//формируем имя файла на версию больше

			head_tree.version = version;

			in.open(name_fail);
			std::getline(in, main_str);
			leng_str = size(main_str);
			check_open_fail = true;
		}
		//команды редактирования:
		if (command == "add") {
			std::cin >> ind; std::getline(std::cin, data); //Считываем то, что должны добавить
			data = sr(data, 1, size(data));//Срезаем лишний пробел между индексом и символами

			MakeAllChange(name_without_version, &head_tree, version_pull, &all_change);
			int leng_str_cange = size(Print(all_change + str_change, main_str, ""));
			all_change.clear();

			if (Error_range(leng_str_cange, ind, 0)) {
				change_fail("+", ind, -1, data, &str_change, check_open_fail);
			}
		}

		if (command == "edit") {
			std::cin >> ind >> jnd; std::getline(std::cin, data);
			data = sr(data, 1, size(data));
			if (size(data) < jnd - ind) { std::cout << "Incorrect 'data'!" << std::endl; }
			else {
				MakeAllChange(name_without_version, &head_tree, version_pull, &all_change);
				int leng_str_cange = size(Print(all_change + str_change, main_str, ""));
				all_change.clear();

				if (Error_range(leng_str_cange, ind, jnd)) {
					change_fail("=", ind, jnd, data, &str_change, check_open_fail);
				}
			}
		}

		if (command == "remove") {
			std::cin >> ind >> jnd;

			MakeAllChange(name_without_version, &head_tree, version_pull, &all_change);
			int leng_str_cange = size(Print(all_change + str_change, main_str, ""));
			all_change.clear();

			if (Error_range(leng_str_cange, ind, jnd - 1)) {
				change_fail("-", ind, jnd, "\*", &str_change, check_open_fail);
			}
		}

		if (command == "print") {
			if (!check_open_fail) { std::cout << "Fail didn't open!" << std::endl; }
			else {
				MakeAllChange(name_without_version, &head_tree, version_pull, &all_change);
				Print(all_change + str_change, main_str, "print");
				all_change.clear();
			}
		}
		//команды дерева:
		if (command == "push") {
			//Создаем имя нового файла и увеличиваем номер версии
			name_push = NameFail(name_without_version, std::to_string(++version), leng_name_fail);

			now_tree.version = version;
			AddVersionTree(version_pull, &head_tree, &now_tree);

			std::cout << "save version: " << version << std::endl;
			out.open(name_push);
			out << str_change;
			out.close();

			str_change.clear();
			version_pull = version;
		}

		if (command == "pull") {
			if (size(str_change) != 0) {
				std::cout << "You can't move, when you don't finish redact" << std::endl;
			}
			else {
				std::cin >> version_pull;
				FoundVersion(version_pull, &head_tree);
				if (!res_found) {
					version_pull = version;
					std::cout << "Fail!!! Version doesn't found" << std::endl;
				}
				else { all_change.clear(); }
			}
		}

		if (command == "delete_version") {
			int version_delete;
			std::cin >> version_delete;
			FoundVersion(version_delete, &head_tree);
			if (!res_found) { std::cout << "Fail!!! Version doesn't found" << std::endl; }
			else {
				EditFile(&head_tree, version_delete, name_without_version, false, data);
				if (remove(NameFail(name_without_version, std::to_string(version_delete), leng_name_fail).c_str()) != 0) {
					std::cout << "Error with \'remove\'" << std::endl;
				}
				else { DeleteVersion(version_delete, &head_tree); }
			}
		}
		if (command == "rebase") {
			std::cout << "now version: " << version_pull << std::endl;
			Rebase(&head_tree, name_without_version, &main_str, version_pull);
		}
		std::cin >> command;
	}
	in.close();
	out.close();
	system("pause");
};
