### 1. Перечислите способы создания базы данных sqlite:
- `sqlite3 database.db` — создаёт файл базы данных и открывает интерактивную консоль.
- DB Browser for SQLite, Valentina Studio — позволяют создавать БД через графический интерфейс.
- вызов функции `sqlite3_open("database.db", &db)` автоматически создаст файл базы данных, если он не существует.

### 2. С помощью какой команды в консоли sqlite можем просмотреть список баз данных и подключённых файлов баз данных?
```sql
.databases
```
### 3. Приведите перечень команд для экспорта данных из таблицы базы данных в файл с расширением .csv.
```sql
.mode csv
.output data.csv
SELECT * FROM companies;
.output stdout
```
### 4. Приведите перечень команд для экспорта отдельной таблицы и всей базы данных в файл с расширением .sql и сжатый файл, например в файл с расширением .sql.tgz.
**Экспорт всей базы данных:**
```sql
.output test.sql
.dump
.output stdout
```
**Экспорт отдельной таблицы:**
```sql
.output test.sql
.dump companies
.output stdout
```
**Сжатие в .sql.tgz:**
```bash
sqlite3 database.db .dump | tar -czf test.sql.tgz
```
### 5. Как вывести из таблицы данных по строкам и по столбцам?
**По столбцам:**
```sql
.mode column
.headers on
SELECT * FROM companies;
```
**По строкам:**
```sql
.mode line
SELECT * FROM companies;
```
### 6. Для чего используется команда .headers в консоли sqlite?
Команда `.headers on`  включает, а `.headers off` отключает вывод заголовков столбцов в результатах запросов.
### 7. Какая команды используется для вывода настроек окружения в sqlite?
```sql
.show
```
### 8. С помощью какой команды выводится список таблиц базы данных в консоли sqlite?
```sql
.tables
```
### 9. Приведите пример запроса выборки из 2-х таблиц.
```sql
SELECT c.id, c.name, cat.cat_name
FROM companies c
INNER JOIN category cat ON c.category_id = cat.id;
```
### 10. Приведите пример запроса для обновления данных в строках таблицы в зависимости от значения определённого поля.
```sql
UPDATE companies
SET employees_count = 50000
WHERE id = 1;
```
### 11. Приведите пример функции, которая открывает соединение с файлом базы данных SQLite и возвращает объект соединения с базой данных, который будет использоваться другими функциями SQLite?
```c
#include <sqlite3.h>
sqlite3 *db;
int rc = sqlite3_open("database.db", &db);
if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
}
```
### 12. Приведите пример синтаксиса функции sqlite3_exec и объясните результат выполнения.
```c
int sqlite3_exec(
    sqlite3 *db,                                 // соединение с БД
    const char *sql,                             // SQL-запрос
    int (*callback)(void*, int, char**, char**), // callback для обработки результатов
    void *data,                                  // данные для callback
    char **errmsg                                // указатель на сообщение об ошибке
);
```
### 13. Какая функция закрывает соединение с базой данных, ранее открытое вызовом sqlite3_open()? Приведите пример синтаксиса.
```c
sqlite3_close(db);
```
### 14. Приведите пример фрагмента кода на языке С для создания таблицы в базе данных sqlite и объясните его.
```c
#include <sqlite3.h>

sqlite3 *db;
char *errmsg = 0;

// Открытие базы данных
sqlite3_open("companies.db", &db);          //открывает или создаёт файл базы данных

// SQL для создания таблицы
const char *sql = "CREATE TABLE companies ("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                  "name TEXT NOT NULL"
                  ");";

// Выполнение SQL-запроса
int rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
if (rc != SQLITE_OK) {         
    fprintf(stderr, "Ошибка: %s\n", errmsg);    //При ошибке выводится сообщение 
    sqlite3_free(errmsg);                       // Освобождение памяти
}
sqlite3_close(db);
```
### 15. Приведите пример фрагмента кода на языке С для вставки данных в таблицу в базе данных sqlite и объясните его.
**Параметризованная вставка:**
```c
sqlite3_stmt *stmt;
const char *sql = "INSERT INTO companies (name, employees_count) VALUES (?, ?);";
sqlite3_prepare_v2(db, sql, -1, &stmt, 0);                    //подготавка запроса с плейсхолдерами `?`
sqlite3_bind_text(stmt, 1, "Apple Inc.", -1, SQLITE_STATIC);  //привязка значения к параметрам
sqlite3_bind_int(stmt, 2, 164000);

if (sqlite3_step(stmt) == SQLITE_DONE) {                      //выполнение запроса
    printf("Данные добавлены\n");
} else {
    fprintf(stderr, "Ошибка: %s\n", sqlite3_errmsg(db));
}
sqlite3_finalize(stmt);                                      //освобождение ресурсов подготовленного запроса
```
### 16. Приведите пример фрагмента кода на языке С выполнением AUTOCOMMIT и TRANSACTION и объясните в чем особенности использования их.
**AUTOCOMMIT:**
```c
char *sql = "DROP TABLE IF EXISTS Friends;" 
                "CREATE TABLE Friends(Id INTEGER PRIMARY KEY, Name TEXT);" 
                "INSERT INTO Friends(Name) VALUES ('Tom');" 
                "INSERT INTO Friends(Name) VALUES ('Rebecca');";
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
}
```

**TRANSACTION:**
```c
char *sql = "DROP TABLE IF EXISTS Friends;"
                "BEGIN TRANSACTION;" 
                "CREATE TABLE Friends(Id INTEGER PRIMARY KEY, Name TEXT);" 
                "INSERT INTO Friends(Name) VALUES ('Tom');" 
                "INSERT INTO Friends(Name) VALUES ('Rebecca');"
                "COMMIT;";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
```

**Особенности:**
| AUTOCOMMIT | TRANSACTION |
|------------|-------------|
| Каждая SQL-операция фиксируется немедленно | Группа операций фиксируется вместе |
| При ошибке невозможно откатить предыдущие операции | При ошибке можно выполнить ROLLBACK |
