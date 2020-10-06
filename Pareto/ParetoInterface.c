#include "Pareto.h"
#include "../UserInterface-CLanguage/UserInterface.h"
#include <locale.h>

void main(void)
{
	setlocale(LC_ALL, "ru");
	Pareto_strValueTable table;
	if (Pareto_initDefaultTableMalloc(&table) != 0) // Создание таблицы
	{
		printf("Ошибка при создании таблицы.\n");
		return;
	}
	printf("\n------------------------------\n");
	Pareto_write(table); // Печать первоначальной таблицы
	printf("\n------------------------------\n");
	Pareto_strValueTable table2 = Pareto_findMalloc(table); // Получения по Парето новой таблицы.
	Pareto_destructorTableFree(&table); // Старая таблица более не нужна.
	Pareto_write(table2); // Печать новой таблицы.

	string_t titles = Pareto_getListTitlesMalloc(table2);
	do {
		printf("Выберите критерий для сокращения.\n");
		size_t checkByUser = ~(size_t)0;
		if (table2.countColumns < (unsigned char)~(unsigned char)0)
			checkByUser = UserInterface_GetCheck(titles.first, (unsigned char)table2.countColumns);
		else
			checkByUser = (size_t)UserInterface_GetUnsignedLongLongIntLimit(titles.first, 0, table2.countColumns - 1);

		Pareto_strValueTable table3 = Pareto_optiMalloc(table2, checkByUser, UserInterface_GetFloat("граница = ")); // Оптимизация таблицы по Парето
		Pareto_destructorTableFree(&table2);
		Pareto_write(table3);
		table2 = table3;
	} while (UserInterface_GetCheck("Продолжить?\n0) Выйти\n1) Выбрать ещё\n", 1));
	Pareto_destructorTableFree(&table2);
	free(titles.first); titles.first = NULL; titles.length = 0;
}