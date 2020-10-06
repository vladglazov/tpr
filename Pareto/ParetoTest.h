#pragma once

#include "Pareto.h"
#include "..\minctest\minctest.h"


void Pareto_test_manyintilizalTableMalloc_free(void)
{
	printf("Start many Pareto test...\n");
	Pareto_strValueTable a;
	for (size_t i = 0; i < 1000; i++) {
		Pareto_intilizalTableMalloc(&a, 100, 100, 100);
		Pareto_destructorTableFree(&a);
	}
	printf("Finish manny Pareto test.\n");
}

void Pareto_test_manyMallocFree(void)
{
	printf("Start many malloc test...\n");
	void * a;
	for (size_t i = 0; i < 100000; i++) {
		a = malloc(1024 * 4);
		free(a);
	}
	printf("Finish many free test.\n");
}

void Pareto_test_find(void)
{
	printf("Pareto find test start...\n");
	Pareto_strValueTable tst; // test
	if (Pareto_intilizalTableMalloc(&tst, 9, 3, 30) != 0)
	{
		printf("Pareto malloc error in Pareto_findMalloc.");
		minctest_ok(false);
	}

	memcpy(tst.titles[0].first, "Salary (rub) +", tst.titles[0].length);
	memcpy(tst.titles[1].first, "Duration of vacation (days) +", tst.titles[1].length);
	memcpy(tst.titles[2].first, "Travel time (minutes) -", tst.titles[2].length);

	for (size_t i = 0; i < 9; i++)
	{
		tst.lines[i].name.first[0] = '1' + i;
		tst.lines[i].name.first[1] = 0;
	}

	tst.flags[0] = 1;
	tst.flags[1] = 1;

	tst.flags[2] = 0;

	double colums[3];
	colums[0] = 900.0; colums[1] = 20.0; colums[2] = 60.0;
	memcpy_s(tst.lines[0].columns, sizeof(*tst.lines->columns)*tst.countColumns, colums, sizeof(double[3]));
	
	colums[0] = 500.0; colums[1] = 30.0; colums[2] = 20.0;
	memcpy_s(tst.lines[1].columns, sizeof(*tst.lines->columns)*tst.countColumns, colums, sizeof(double[3]));
	
	colums[0] = 700.0; colums[1] = 36.0; colums[2] = 40.0;
	memcpy_s(tst.lines[2].columns, sizeof(*tst.lines->columns)*tst.countColumns, colums, sizeof(double[3]));
	
	colums[0] = 800.0; colums[1] = 40.0; colums[2] = 50.0;
	memcpy_s(tst.lines[3].columns, sizeof(*tst.lines->columns)*tst.countColumns, colums, sizeof(double[3]));
	
	colums[0] = 400.0; colums[1] = 60.0; colums[2] = 15.0;
	memcpy_s(tst.lines[4].columns, sizeof(*tst.lines->columns)*tst.countColumns, colums, sizeof(double[3]));
	
	colums[0] = 600.0; colums[1] = 30.0; colums[2] = 10.0;
	memcpy_s(tst.lines[5].columns, sizeof(*tst.lines->columns)*tst.countColumns, colums, sizeof(double[3]));
	
	colums[0] = 900.0; colums[1] = 35.0; colums[2] = 60.0;
	memcpy_s(tst.lines[6].columns, sizeof(*tst.lines->columns)*tst.countColumns, colums, sizeof(double[3]));
	
	colums[0] = 600.0; colums[1] = 24.0; colums[2] = 10.0;
	memcpy_s(tst.lines[7].columns, sizeof(*tst.lines->columns)*tst.countColumns, colums, sizeof(double[3]));
	
	colums[0] = 650.0; colums[1] = 35.0; colums[2] = 40.0;
	memcpy_s(tst.lines[8].columns, sizeof(*tst.lines->columns)*tst.countColumns, colums, sizeof(double[3]));

	Pareto_write(tst);
	Pareto_strValueTable tst2 = Pareto_findMalloc(tst);
	Pareto_write(tst2);

	Pareto_destructorTableFree(&tst);
	Pareto_destructorTableFree(&tst2);

	printf("Pereto find test end...\n");
}

void Pareto_test_unknow()
{
	printf("Pareto intilizalTableMalloc and free\tStart test...\n");
	Pareto_strValueTable tst;
	minctest_ok(Pareto_intilizalTableMalloc(&tst, 5, 6, 10) == 0); // error to create table.

	// Тестирование значений таблицы

	minctest_ok(tst.countColumns == 6); //L"error tst.countColumns");
	minctest_ok(tst.countLines == 5); // L"error tst.countLines");

	// Тестирование пустых указателей таблицы

	minctest_ok(tst.lines != NULL); // L"error: lines NULL");
	minctest_ok(tst.titles != NULL); // L"error: tst.titles NULL");

	// Тестирование экземпляров таблицы

	for (size_t i = tst.countLines; --i != ~(size_t)0u; )
	{
		// Тестирование имён экхемпляров

		minctest_ok(tst.lines[i].name.length == 10); // L"error tst.lines[i].name.length != 10");

		for (size_t ch = 0; ch < 10; ch++)
			tst.lines[i].name.first[ch] = '0' + (char)ch;
		for (size_t ch = 0; ch < 10; ch++)
			minctest_ok(tst.lines[i].name.first[ch] == '0' + (char)ch); // L"error tst.lines[i].name.str[ch] != '0' + (char)ch");

		// Тестирование значений экземпляра

		minctest_ok(tst.lines[i].columns != NULL); // L"error tst.lines[i].columns == NULL");

		for (size_t j = tst.countColumns; --j != ~(size_t)0;)
		{
			tst.lines[i].columns[j] = 123;
			minctest_ok(tst.lines[i].columns[j] == 123); // L"error tst.lines[i].columns[j] != 123");
		}
	}

	// Тестирование заголовков таблицы
	for (size_t i = tst.countColumns; --i != ~(size_t)0;)
	{
		minctest_ok(tst.titles[i].length == 10); // L"error tst.titles[i].length != 10");
		for (size_t ch = 0; ch < 10; ch++)
			tst.titles[i].first[ch] = '0' + (char)ch;
		for (size_t ch = 0; ch < 10; ch++)
			minctest_ok(tst.titles[i].first[ch] == '0' + (char)ch); // L"error tst.titles[i].str[ch] != '0' + (char)ch");
	}

	Pareto_destructorTableFree(&tst);

	minctest_ok(tst.countColumns == 0); // L"error tst.countColumns != 0");
	minctest_ok(tst.countLines == 0); // L"error tst.countLines != 0");
	minctest_ok(tst.lines == NULL); // L"error tst.lines != NULL");
	minctest_ok(tst.titles == NULL); // L"error tst.titles != NULL");
}

// Запуск всех тестов файла Pareto.
void Pareto_test_run() {
	void(*tests[])(void) = {
		Pareto_test_find,
		Pareto_test_manyMallocFree,
		Pareto_test_manyintilizalTableMalloc_free,
		Pareto_test_unknow
	};
	char prototypeName[] = "Pareto_test ";
	string_t testName = string_malloc(sizeof(prototypeName) + 20u);
	for (size_t i = 0; i < sizeof(tests) / sizeof(size_t); i++)
	{
		sprintf_s(testName.first, testName.length, "%s%zu", prototypeName, i);
		minctest_run(testName.first, tests[i]);
	}
}
