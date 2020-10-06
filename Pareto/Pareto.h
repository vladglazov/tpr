#pragma once

#include <stdlib.h>
#include <stdio.h>
#include "../String-CLanguage/string_t.h"



// Символизирует строку таблицы: название экземпляра и указатель на начало его параметров.
typedef struct {
	string_t name;
	double* columns;
} Pareto_strValues;

// Структура, которая хранит в себе указатель на начало списка экземпляров, количество строк и количество столбцов.
typedef struct {
	string_t* titles;			// Указатель на заголовки таблицы.
	Pareto_strValues* lines;	// Представляет собой строку с именем экземпляра и его значениями.
	unsigned char* flags;		// Указатель на массив флагов. Если 0 - значит, лучше отрицательное значение. Если 1 - лучше положительные значения.
	size_t countLines;			// Количество экземпляров в таблице.
	size_t countColumns;		// Количество критериев экземпляров в таблице.
} Pareto_strValueTable;

// Печать таблицы.
void Pareto_write(const Pareto_strValueTable input)
{
	printf("\t\t");
	for (size_t i = 0; i < input.countColumns; i++)
	{
		printf("%s;\t", input.titles[i].first);
	}
	printf("\n");
	for (size_t i = 0; i < input.countLines; i++)
	{
		printf("%s;\t", input.lines[i].name.first);
		for (size_t j = 0; j < input.countColumns; j++)
		{
			printf("%f;\t", input.lines[i].columns[j]);
		}
		printf("\n");
	}
}

// Создание в памяти таблицы Парето
int Pareto_intilizalTableMalloc(Pareto_strValueTable* out, size_t countLines, size_t countColumns, size_t countChars)
{
	Pareto_strValueTable table = {
		(string_t*)malloc(sizeof(*table.titles) * countColumns),
		(Pareto_strValues*)malloc(sizeof(*table.lines) * countLines), // Указатель на экземпляры
		(unsigned char*)malloc(sizeof(*table.flags) * countColumns), // Флаги таблицы
		countLines, // Количество экземпляров
		countColumns // Критерии
	};
	if (table.lines == NULL || table.titles == NULL || table.flags == NULL)
	{
		if (table.lines != NULL) free(table.lines);
		if (table.titles != NULL) free(table.titles);
		if (table.flags != NULL) free(table.flags);
		return 1;
	}
	for (size_t i = 0; i < table.countColumns; i++)
	{
		table.titles[i].first = (char*)malloc(sizeof(*table.titles->first) * countChars);
		table.titles[i].length = countChars;

		if (table.titles[i].first == NULL)
		{
			table.titles[i--].length = 0;
			for (; i != ~(size_t)0; i--)
			{
				free(table.titles[i].first);
			}
			free(table.titles);
			free(table.lines);
			free(table.flags);
			return 4;
		}
	}
	for (size_t i = 0; i < table.countLines; i++)
	{
		table.lines[i].columns = (double*)malloc(sizeof(*table.lines->columns) * table.countColumns);
		table.lines[i].name.first = (char*)malloc(sizeof(*table.lines->name.first) * countChars);
		table.lines[i].name.length = countChars;

		for (size_t j = table.countColumns; --j != ~(size_t)0;)
		{
			table.lines[i].columns[j] = 0;
		}
		if (table.lines[i].name.length > 0)
			table.lines[i].name.first[0] = 0;


		if (table.lines[i].columns == NULL)
		{
			//printf("malloc error [lines[%lu].columns]\n", (unsigned long)i);

			for (size_t ii = i - 1; ii != ~(size_t)0; ii--)
			{
				free(table.lines[ii].columns);
				free(table.lines[ii].name.first);
			}
			free(table.lines);
			free(table.titles);
			free(table.flags);
			return 2;
		}
		if (table.lines[i].name.first == NULL)
		{
			//printf("malloc error [lines[%ul].name.first]\n", (unsigned long)i);
			free(table.lines[i].columns);
			for (size_t ii = i - 1; ii != ~(size_t)0; ii--)
			{
				free(table.lines[ii].columns);
				free(table.lines[ii].name.first);
			}
			free(table.lines);
			free(table.flags);
			free(table.titles);
			return 3;
		}
	}

	for (size_t j = table.countColumns; --j != ~(size_t)0;)
	{
		table.flags[j] = 0;
		if (table.titles[j].length > 0)
			table.titles[j].first[0] = 0;
	}

	*out = table;
	return 0;
}

// Удаление определённых линий.
// const Pareto_strValueTable input - Источник строк.
// const size_t * deleteIds - Список индентификаторов, которые должны быть удалены. Повторы разрешены. Значения в массиве ~(size_t)0 будут игнорироваться.
// size_t lengthIds - Количество идентификаторов.
Pareto_strValueTable Pareto_deleteLinesMalloc(const Pareto_strValueTable input, const size_t* deleteIds, size_t lengthIds)
{
	size_t* ids = (size_t*)malloc(lengthIds * sizeof(*ids));

	if (ids == NULL)
		return (Pareto_strValueTable) { NULL, NULL, NULL, 0, 0 }; // Не хватило памяти.

	#ifdef _MSC_VER
	memcpy_s(ids, lengthIds * sizeof(*ids), deleteIds, lengthIds * sizeof(*deleteIds));
	#else
	memcpy(ids, deleteIds, lengthIds * sizeof(*ids));
	#endif

	// Удаление одинаковых чисел из ids.
	if (lengthIds != 0) for (size_t i = lengthIds; --i != 0;)
		if (ids[i] == ~(size_t)0)
			continue;
		else for (size_t j = i; --j != ~(size_t)0;)
			if (ids[i] == ids[j])
				ids[i] = ~(size_t)0;

	size_t countids = 0; // Количество найденных

	for (size_t i = lengthIds; --i != ~(size_t)0;)
		if (ids[i] != ~(size_t)0)
			countids++;

	Pareto_strValueTable out =
	{
		NULL, // Указатель на заголовки таблицы.
		NULL, // Представляет собой строку с именем экземпляра и его значениями.
		NULL, // Указатель на массив флагов. Если 0 - значит, лучше отрицательное значение. Если 1 - лучше положительные значения.
		0,	  // Количество экземпляров в таблице.
		0	  // Количество критериев экземпляров в таблице.
	};

	size_t countMaybeWinners = input.countLines - countids;
	size_t* maybeWinners = (size_t*)malloc(countMaybeWinners * sizeof(size_t));

	if (maybeWinners == NULL)
	{
		return out;
	}

	for (size_t i = 0, iMay = 0; i < input.countLines; i++)
	{
		size_t j = lengthIds;
		while (--j != ~(size_t)0)
			if (ids[j] == i) // Лузер найден!
			{
				// Надо не дать лузеру дойти до входа!
				break;
			}
		if (j == ~(size_t)0)
			maybeWinners[iMay++] = i;
	}

	free(ids);
	ids = NULL;

	// Копирование заголовков.

	size_t maxCountStr = input.titles[0].length; // Количество необходимых символов.
	size_t countStr = input.titles[0].length; // Сумма всех символов.

	for (size_t i = input.countColumns; --i != 0;)
	{
		countStr += input.titles[i].length;
		if (maxCountStr < input.titles[i].length)
			maxCountStr = input.titles[i].length;
	}

	if (Pareto_intilizalTableMalloc(&out, countMaybeWinners, input.countColumns, maxCountStr) != 0)
	{
		free(maybeWinners);
		return out;
	}

	for (size_t i = out.countColumns; --i != ~(size_t)0;)
	#ifdef _MSC_VER
		memcpy_s(out.titles[i].first, out.titles[i].length, input.titles[i].first, input.titles[i].length);
	#else
		memcpy(out.titles[i].first, input.titles[i].first, input.titles[i].length);
	#endif

	#ifdef _MSC_VER
		memcpy_s(out.flags, out.countColumns * sizeof(*out.flags), input.flags, input.countColumns * sizeof(*input.flags));
	#else
		memcpy(out.flags, input.flags, input.countColumns * sizeof(*input.flags));
	#endif

	// Сформировать список из победителей или нейтралов.

	for (size_t i = out.countLines; --i != ~(size_t)0;)
	{
		for (size_t j = out.countColumns; --j != ~(size_t)0;)
			out.lines[i].columns[j] = input.lines[maybeWinners[i]].columns[j];
		#ifdef _MSC_VER
		memcpy_s(out.lines[i].name.first, out.lines[i].name.length, input.lines[maybeWinners[i]].name.first, input.lines[maybeWinners[i]].name.length);
		#else
		memcpy(out.lines[i].name.first, input.lines[maybeWinners[i]].name.first, input.lines[maybeWinners[i]].name.length);
		#endif
	}
	free(maybeWinners);
	return out;
}

// Отвечает на вопрос, кто по Парето лучше?
// size_t indexFirst - первый представитель.
// size_t indexSecond - второй представитель.
// Pareto_strValueTable table - данные из таблицы.
// Возвращает: 1, если первый лучше второго. 0 - если нельяз сравнить. 2 - второй лучше первого. 255 - ошибка.
unsigned char Pareto_isFirstBetter(size_t indexFirst, size_t indexSecond, const Pareto_strValueTable table)
{
	size_t better[3] = {
		0, // ничья
		0, // первый лучше
		0  // второй лучше
	};
	for (size_t i = table.countColumns; --i != ~(size_t)0; )
	{
		if (table.lines[indexFirst].columns[i] == table.lines[indexSecond].columns[i])
			better[0]++; // Если равны, то ничья.

		else if (table.flags[i]) // Стримится к положительным числам.
		{
			if (table.lines[indexFirst].columns[i] > table.lines[indexSecond].columns[i])
				better[1]++;
			else//if(first.columns[i].numerical <  second.columns[i].numerical)
				better[2]++;
		}
		else // Стримится к отрицательным числам.
		{
			if (table.lines[indexFirst].columns[i] < table.lines[indexSecond].columns[i])
				better[1]++;
			else//if(first.columns[i].numerical >  second.columns[i].numerical)
				better[2]++;
		}

		if (better[1] != 0 && better[2] != 0)
			return 0;
	}
	if (better[1] == 0)
		return 2;
	else if (better[2] == 0)
		return 1;
	else
		return 0;
}

// Реализовать программу, которая ищет множество Парето
Pareto_strValueTable Pareto_findMalloc(const Pareto_strValueTable input)
{
	// Поиск проигравших.

	size_t* loosers = (size_t*)malloc(sizeof(size_t) * input.countLines); // Лист проигравших.
	if (loosers == NULL)
		return (Pareto_strValueTable) { NULL, NULL, NULL, 0, 0 }; // Не хватило памяти.
	for (size_t i = input.countLines; --i != ~(size_t)0;)
		loosers[i] = ~(size_t)0;
	size_t loo_idx = 0; // Идентификатор листа проигравших.
	unsigned char change;
	for (size_t x = input.countLines; --x != 0;)
		for (size_t y = x - 1; --y != ~(size_t)0;)
		{
			change = Pareto_isFirstBetter(x, y, input);
			if (change == 255)
			{// Особая ошибка. Нельзя продолжить.
				free(loosers);
				return (Pareto_strValueTable) { NULL, NULL, NULL, 0, 0 };
			}
			if (change != 0)
				loosers[loo_idx++] = change == 1 ? y : x;
		}

	// Удаляем проигравших.
	Pareto_strValueTable out = Pareto_deleteLinesMalloc(input, loosers, loo_idx);

	free(loosers);

	return out;
}

// Реализует субоптимизацию Парето.
// const Pareto_strValueTable input - Данные.
// size_t idMain - Идентификатор главного критерия.
// double border - Крайнее значение, которое может сществовать.
// Возвращает новый экземпляр таблицы без элементов, которые не подходят границе border по критерию idMain.
Pareto_strValueTable Pareto_optiMalloc(const Pareto_strValueTable input, size_t idMain, double border)
{

	if (idMain >= input.countColumns || input.lines == NULL || input.flags == NULL)
		return (Pareto_strValueTable) { NULL, NULL, NULL, 0, 0 };


	size_t* loosers = (size_t*)malloc(sizeof(size_t) * input.countLines); // лист лузеров.
	if (loosers == NULL)
		return (Pareto_strValueTable) { NULL, NULL, NULL, 0, 0 }; // Не хватило памяти.
	for (size_t i = input.countLines; --i != ~(size_t)0;)
		loosers[i] = ~(size_t)0;
	size_t loo_idx = 0; // идентификатор листа лузеров


	for (size_t i = input.countLines; --i != ~(size_t)0;)
		if ((input.lines[i].columns == NULL) ||
			/* В зависимости от того, что лучше - меньше или больше, идёт сравнение. */
			(input.flags[idMain] && input.lines[i].columns[idMain] < border) ||
			(!input.flags[idMain] && input.lines[i].columns[idMain] > border))
			loosers[loo_idx++] = i;

	Pareto_strValueTable out = Pareto_deleteLinesMalloc(input, loosers, loo_idx);
	free(loosers);
	return out;
}


// Освобождает из памяти таблицу.
// Pareto_strValueTable * input - Указатель на входящую таблицу, которую необходимо освободить.
// Возвращает: код ошибки.
// 1 - строки не найдены.
// 2 - получен input NULL.
// -n - программа сработала хорошо, но были найдены указатели NULL в количестве n.
int Pareto_destructorTableFree(Pareto_strValueTable* input)
{
	size_t flag = 0;
	if (input == NULL)
	{
		return 2;
	}
	if (input->titles != NULL)
	{
		for (size_t i = input->countColumns; --i != ~(size_t)0;)
		{
			if (input->titles[i].first != NULL)
				free(input->titles[i].first);
			else flag++;
		}
		free(input->titles);
	}
	else flag++;
	if (input->lines == NULL)
		return 1;
	if (input->flags != NULL)
		free(input->flags);
	else flag++;
	for (size_t i = 0; i < input->countLines; i++)
	{
		if (input->lines[i].columns != NULL)
		{
			free(input->lines[i].columns);
		}
		else flag++;
		if (input->lines[i].name.first != NULL)
		{
			free(input->lines[i].name.first);
		}
		else flag++;
	}
	free(input->lines);
	input->countColumns = 0;
	input->countLines = 0;
	input->lines = NULL;
	input->titles = NULL;
	input->flags = NULL;
	return flag > (int)flag ? 1 << ((sizeof(int) * 8) - 1) : -(int)flag; // Защита от переполнения.
}

// Создаёт матрицу сравнений по-умолчанию.
// strValueTable * output - Указатель, куда записать результат.
// Возвращает: код ошибки.
// 1 - не хватило памяти при создании строк таблицы. Память очищается.
// 2 - не хватило памяти для создания. Память очищается.
// 3 - не хватило памяти для создания поля имени. Память очищается.
int Pareto_initDefaultTableMalloc(Pareto_strValueTable* output)
{
	int err = Pareto_intilizalTableMalloc(output, 10, 5, 32);
	if (err != 0) return err;

	memcpy(output->titles[0].first, "Вес -     ", output->titles[0].length);
	memcpy(output->titles[1].first, "Рост -    ", output->titles[1].length);
	memcpy(output->titles[2].first, "Болезни - ", output->titles[2].length);
	memcpy(output->titles[3].first, "Зарплата +", output->titles[3].length);
	memcpy(output->titles[4].first, "Языки +   ", output->titles[4].length);

	output->flags[0] = 0;
	output->flags[1] = 0;
	output->flags[2] = 0;
	output->flags[3] = 1;
	output->flags[4] = 1;

	memcpy(output->lines[0].name.first, "Алёна  ", output->lines[0].name.length);
	memcpy(output->lines[1].name.first, "Елена  ", output->lines[1].name.length);
	memcpy(output->lines[2].name.first, "Мой кот", output->lines[2].name.length);
	memcpy(output->lines[3].name.first, "Пётр   ", output->lines[3].name.length);
	memcpy(output->lines[4].name.first, "Ирина  ", output->lines[4].name.length);
	memcpy(output->lines[5].name.first, "Мама   ", output->lines[5].name.length);
	memcpy(output->lines[6].name.first, "Наташа ", output->lines[6].name.length);
	memcpy(output->lines[7].name.first, "Галина ", output->lines[7].name.length);
	memcpy(output->lines[8].name.first, "Ольга  ", output->lines[8].name.length);
	memcpy(output->lines[9].name.first, "Зинаида", output->lines[9].name.length);



	memcpy(output->lines[0].columns, (double[]) { 120.0, 175.0, 3.0, 15000.0, 1.0 }, sizeof(double[5]));
	memcpy(output->lines[1].columns, (double[]) { 70.0, 160.0, 2.0, 4000.0, 2.0 }, sizeof(double[5]));
	memcpy(output->lines[2].columns, (double[]) { 4.0, 20.0, 1.0, -2500.0, 0.0 }, sizeof(double[5]));
	memcpy(output->lines[3].columns, (double[]) { 85.0, 185.0, 1.0, 70000.0, 2.0 }, sizeof(double[5]));
	memcpy(output->lines[4].columns, (double[]) { 75.0, 172.0, 2.0, 50000.0, 2.0 }, sizeof(double[5]));
	memcpy(output->lines[5].columns, (double[]) { 85.0, 170.0, 4.0, 40000.0, 2.0 }, sizeof(double[5]));
	memcpy(output->lines[6].columns, (double[]) { 60.0, 180.0, 2.0, 30000.0, 1.0 }, sizeof(double[5]));
	memcpy(output->lines[7].columns, (double[]) { 75.0, 152.0, 1.0, 65000.0, 2.0 }, sizeof(double[5]));
	memcpy(output->lines[8].columns, (double[]) { 80.0, 151.0, 3.0, 75000.0, 1.0 }, sizeof(double[5]));
	memcpy(output->lines[9].columns, (double[]) { 74.0, 174.0, 2.0, 23000.0, 2.0 }, sizeof(double[5]));
	return 0;
}

string_t Pareto_getListNamesMalloc(const Pareto_strValueTable input)
{
	string_t out = { NULL, 0 };
	for (size_t i = input.countLines; --i != ~(size_t)0;)
		out.length += input.lines[i].name.length;
	out.first = malloc(out.length * sizeof(*out.first));
	if (out.first == NULL)
		return out;
	out.first[0] = 0;
	for (size_t i = 0; i < input.countLines; i++)
		#ifdef _MSC_VER
		sprintf_s(out.first, out.length, "%s%llu)%s\n", out.first, out.length, (unsigned long long)i, input.lines[i].name.first, input.lines[i].name.length);
		#else
		snprintf(out.first, out.length, "%.*s%llu)%.*s\n", (int)out.length, out.first, (unsigned long long)i, (int)input.lines[i].name.length, input.lines[i].name.first);
		#endif

	return out;
}

string_t Pareto_getListTitlesMalloc(const Pareto_strValueTable input)
{
	string_t out = { NULL, 0 };
	for (size_t i = input.countColumns; --i != ~(size_t)0;)
		out.length += input.titles[i].length;
	out.first = malloc(out.length * sizeof(*out.first));
	if (out.first == NULL)
		return out;
	out.first[0] = 0;
	size_t chars = 0;
	for (size_t i = 0; i < input.countColumns; i++)
		#ifdef _MSC_VER
		chars += sprintf_s(out.first + chars, out.length - chars, "%llu)%s\n", (unsigned long long)i, input.titles[i].first);
		#else
		chars += snprintf(out.first + chars, out.length - chars, "%llu)%.*s\n", (unsigned long long)i, (int)input.titles[i].length, input.titles[i].first);
		#endif

	// Оптимизация по освобождению ненужной памяти.
	string_t outSmall = { malloc((chars + 1) * sizeof(out.first)), chars + 1 };
	#ifdef _MSC_VER
	memcpy_s(outSmall.first, outSmall.length, out.first, chars + 1);
	#else
	memcpy(outSmall.first, out.first, chars + 1);
	#endif
	free(out.first); out.length = 0; out.first = NULL;

	return outSmall;
}
