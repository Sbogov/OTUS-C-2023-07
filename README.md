# Построение графа вызово функций для файла с исходным кодом на языке С
Проектная работа по курсу OTUS "Программист С" студента группы OTUS-C-2023-07

Программа выполняющая рабор файла с кодом на языке С, выдающая выходной файл в формате [Graphviz](https://graphviz.org/docs/outputs/canon/) — простой текстовый формат, позволяющий строить изображения.
Далее с помощью **Graphviz** рисуется граф вызовов функций, который можно использовать для исследовательских целей.

## Содержание
- [Технологии](#технологии)
- [Структура проекта](#Структура проекта)
- [Использование](#использование)
- [Парсинг](#парсинг)
- [Тестирование](#тестирование)
- [Построение графа](#построение графа)
- [FAQ](#faq)
- [To do](#to do)
- [FAQ](#faq)
- [Команда проекта](#команда проекта)
- [Источники](#источники)

## Технологии
- [libclang](https://clang.llvm.org/doxygen/group__CINDEX.html)
- [Graphviz](https://graphviz.org/)

## Структура проекта
- src - директория с исходным кодом программы; 
- tests - директория с прилагаемыми примерами, для проведения тестирования работы программы; 

## Использование
Для запуска проекта необходимы:

- Компилятор clang

	Установите с помощью команды
	'''sh
	$ sudo apt-get install clang
	'''
	
- Библиотеки libclang

	Установите с помощью команды
	'''sh
	$ sudo apt-get install libclang-dev
	'''
	
- llvm - для подключения библиотеки libclang

	Установите с помощью команды
	'''sh
	$ sudo apt-get install llvm
	'''
	
- Graphviz - для построения гарафа
	Установите с помощью команды
	'''sh
	$ sudo apt-get install graphviz
	'''

Для просмотра построенного графа вызовов в проекте используется стандартная в *Ubuntu* утилита для просмотра изображений [*EOG*](https://howtoinstall.co/package/eog)

## Парсинг

### Требования
Для запуска проекта необходимо наличие установленных утилит, описанных в разделе [Использование](#использование). Сборка и все "предустановленные" команды выполняются из дикертории */build*

### Создание билда
Чтобы выполнить *Debug* сборку, выполните команду:
	'''sh
	$ cmake -DCMAKE_BUILD_TYPE=Debug ..
	'''
	
В *Debug* версии дополнительно присутствует проверочная печать обхода программой узлов AST единицы парсинга.

Чтобы выполнить *Release* сборку, выполните команду:
	'''sh
	$ cmake -DCMAKE_BUILD_TYPE=Release ..
	'''

### Запуск
Для запуска проекта выполните команду:
	'''sh
	$ ./clang_graphviz <source_file_for_analize> \[clang_arg1] \[clang_arg2] \[...]
	'''
	
	где 
	- source_file_for_analize - анализируемый файл
	- \[clang_arg1] \[clang_arg2] \[...] - аргументы командной строки, которые быдут переданы исполняемому файлу clang. Эти параметры командной строки будут разобраны и повлияют на то, как будет разобран блок трансляции.
	По умолчанию данные опции не используются. Обратите внимание, что следующие опции игнорируются: '-c', '-emit-ast', '-fsyntax-only' (по умолчанию) и '-o <выводной файл>'. 

### Временные файлы
- ***call_graph.dot*** - DOT файл для Graphviz
- ***call_graph.png*** - отрисованный с помощью Graphviz граф вызовов функций

Находятся в директории сборки */build*

## Тестирование
Тестирование выполняется на примерах с исходным кодом на языке C.

Для запуска программы на тестовых примерах предусмотрены "предустановленные" команды:
		'''sh
		$ make test1
		$ make test2
		'''

## Построение графа
После парсинга файла для построения графа, выполните "предустановленную" команду:
		'''sh
		$ make graph
		'''

## FAQ

### Зачем был разработан данный проект?
- Для использования в исследовательских целях в рамках рабочих задач.


## To do
 - [x] Создать рабочий проект
 - [x] Устранить замечания
 - [x] Добавить хороший README
 
## Команда проекта
 
 - [Богов Сергей](https://t.me/Caitalar) - инженер-программист, разработчик проекта
 
 
## Источники
- [Документация libclang](https://clang.llvm.org/doxygen/group__CINDEX.html)
