#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

// Открытие и закрытие базы данных
sqlite3* db_open(const char* filename);
void db_close(sqlite3* db);

// запросы для компаний
void add_company(sqlite3* db);
void list_all_companies(sqlite3* db);
void find_company_by_id(sqlite3* db);
void find_company_by_name_pattern(sqlite3* db);
void find_company_by_category(sqlite3* db);
void delete_company(sqlite3* db);

// Работа с фото
void add_company_photo(sqlite3* db);
void export_company_photo(sqlite3* db);

// Демонстрация autocommit и транзакций
void demo_autocommit(sqlite3* db);
void demo_transaction(sqlite3* db);

#endif
