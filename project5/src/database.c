#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"

sqlite3* db_open(const char* filename) {
    sqlite3 *db;
    int rc = sqlite3_open(filename, &db);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Невозможно открыть базу данных: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
    return db;
}

void db_close(sqlite3* db) {
    if (db) {
        sqlite3_close(db);
    }
}

void add_company(sqlite3* db) {
    char name[256], website[256], postal_code[20], country[100], region[100];
    char district[100], city[100], street[200], house[20], apartment[20];
    int foundation_year, years_in_market, employees_count, european_branches, category_id;
    
    printf("\n=== ДОБАВЛЕНИЕ КОМПАНИИ ===\n");
    
    printf("Название: "); scanf(" %[^\n]", name);
    printf("Сайт: "); scanf(" %[^\n]", website);
    printf("Почтовый индекс: "); scanf(" %[^\n]", postal_code);
    printf("Страна: "); scanf(" %[^\n]", country);
    printf("Область: "); scanf(" %[^\n]", region);
    printf("Район: "); scanf(" %[^\n]", district);
    printf("Город: "); scanf(" %[^\n]", city);
    printf("Улица: "); scanf(" %[^\n]", street);
    printf("Дом: "); scanf(" %[^\n]", house);
    printf("Квартира: "); scanf(" %[^\n]", apartment);
    printf("Год основания: "); scanf("%d", &foundation_year);
    printf("Лет на рынке: "); scanf("%d", &years_in_market);
    printf("Количество сотрудников: "); scanf("%d", &employees_count);
    printf("Филиалов в Европе: "); scanf("%d", &european_branches);
    printf("ID категории (1-4): "); scanf("%d", &category_id);
    
    const char *sql =
        "INSERT INTO companies (name, website, postal_code, country, region, district, "
        "city, street, house, apartment, foundation_year, years_in_market, "
        "employees_count, european_branches, category_id) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
    
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Ошибка подготовки запроса: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, website, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, postal_code, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, country, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, region, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, district, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, city, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 8, street, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 9, house, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 10, apartment, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 11, foundation_year);
    sqlite3_bind_int(stmt, 12, years_in_market);
    sqlite3_bind_int(stmt, 13, employees_count);
    sqlite3_bind_int(stmt, 14, european_branches);
    sqlite3_bind_int(stmt, 15, category_id);
    
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        printf("Компания успешно добавлена!\n");
    } else {
        fprintf(stderr, "Ошибка: %s\n", sqlite3_errmsg(db));
    }
    
    sqlite3_finalize(stmt);
}

void list_all_companies(sqlite3* db) {
    const char *sql = "SELECT id, name, country, city, foundation_year, "
                      "employees_count, european_branches FROM companies;";
    
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Ошибка подготовки запроса: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    printf("\n=== ВСЕ КОМПАНИИ ===\n");
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("id: %d\n", sqlite3_column_int(stmt, 0));
        printf("name: %s\n", sqlite3_column_text(stmt, 1));
        printf("country: %s\n", sqlite3_column_text(stmt, 2));
        printf("city: %s\n", sqlite3_column_text(stmt, 3));
        printf("foundation_year: %d\n", sqlite3_column_int(stmt, 4));
        printf("employees_count: %d\n", sqlite3_column_int(stmt, 5));
        printf("european_branches: %d\n", sqlite3_column_int(stmt, 6));
        printf("------------------------\n");
    }
    
    sqlite3_finalize(stmt);
}

void find_company_by_id(sqlite3* db) {
    int id;
    printf("Введите ID компании: ");
    scanf("%d", &id);
    while (getchar() != '\n');
    
    const char *sql = "SELECT * FROM companies WHERE id = ?;";
    
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Ошибка подготовки запроса: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    sqlite3_bind_int(stmt, 1, id);
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int col_count = sqlite3_column_count(stmt);
        for (int i = 0; i < col_count-1; i++) {
            const char *col_name = sqlite3_column_name(stmt, i);
            const char *col_value = (const char*)sqlite3_column_text(stmt, i);
            printf("%s: %s\n", col_name, col_value ? col_value : "NULL");
        }
        printf("------------------------\n");
    } else {
        printf("Компания с ID %d не найдена\n", id);
    }
    
    sqlite3_finalize(stmt);
}

void find_company_by_name_pattern(sqlite3* db) {
    char pattern[256];
    printf("Введите фрагмент названия: ");
    scanf(" %[^\n]", pattern);
    
    const char *sql = "SELECT * FROM companies WHERE name LIKE '%' || ? || '%';";
    
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Ошибка подготовки запроса: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    sqlite3_bind_text(stmt, 1, pattern, -1, SQLITE_STATIC);
    
    int found = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        found = 1;
        int col_count = sqlite3_column_count(stmt);
        for (int i = 0; i < col_count-1; i++) {
            const char *col_name = sqlite3_column_name(stmt, i);
            const char *col_value = (const char*)sqlite3_column_text(stmt, i);
            printf("%s: %s\n", col_name, col_value ? col_value : "NULL");
        }
        printf("------------------------\n");
    }
    
    if (!found) {
        printf("Компании с фрагментом '%s' не найдены\n", pattern);
    }
    
    sqlite3_finalize(stmt);
}

void find_company_by_category(sqlite3* db) {
    int cat_id;
    printf("Выберите категорию:\n");
    printf("1 - Картель\n2 - Синдикат\n3 - Трест\n4 - Концерн\n");
    printf("Введите номер: ");
    scanf("%d", &cat_id);
    while (getchar() != '\n');
    
    const char *sql = "SELECT c.*, cat.cat_name FROM companies c "
                      "LEFT JOIN category cat ON c.category_id = cat.id "
                      "WHERE c.category_id = ?;";
    
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Ошибка подготовки запроса: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    sqlite3_bind_int(stmt, 1, cat_id);
    
    int found = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        found = 1;
        int col_count = sqlite3_column_count(stmt);
        for (int i = 0; i < col_count-1; i++) {
            const char *col_name = sqlite3_column_name(stmt, i);
            const char *col_value = (const char*)sqlite3_column_text(stmt, i);
            printf("%s: %s\n", col_name, col_value ? col_value : "NULL");
        }
        printf("------------------------\n");
    }
    
    if (!found) {
        printf("Компании в выбранной категории не найдены\n");
    }
    
    sqlite3_finalize(stmt);
}

void delete_company(sqlite3* db) {
    int id;
    printf("Введите ID компании для удаления: ");
    scanf("%d", &id);
    while (getchar() != '\n');
    const char *sql = "DELETE FROM companies WHERE id = ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Ошибка подготовки запроса: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    sqlite3_bind_int(stmt, 1, id);
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        printf("Компания удалена!\n");
    } else {
        fprintf(stderr, "Ошибка: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
}

void add_company_photo(sqlite3* db) {
    int id;
    char photo_name[256];
    printf("Введите ID компании: ");
    scanf("%d", &id);
    while (getchar() != '\n');
    printf("Введите имя файла из папки data: ");
    scanf(" %[^\n]", photo_name);
    
    char full_path[512];
    snprintf(full_path, sizeof(full_path), "data/%s", photo_name);
    FILE *fp = fopen(full_path, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Cannot open image file: %s\n", full_path);
        return;
    }
    fseek(fp, 0, SEEK_END);
    int flen = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *data = malloc(flen);
    int size = fread(data, 1, flen, fp);
    fclose(fp);
    
    const char *sql = "UPDATE companies SET photo = ? WHERE id = ?;";
    sqlite3_stmt *stmt;
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Ошибка подготовки запроса: %s\n", sqlite3_errmsg(db));
        free(data);
        return;
    }
    
    sqlite3_bind_blob(stmt, 1, data, size, free);
    sqlite3_bind_int(stmt, 2, id);
    
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_DONE) {
        printf("Фото успешно добавлено!\n");
    } else {
        fprintf(stderr, "Ошибка: %s\n", sqlite3_errmsg(db));
    }
    
    sqlite3_finalize(stmt);
}

void export_company_photo(sqlite3* db) {
    int id;
    char output_name[256];
    
    printf("Введите ID компании: ");
    scanf("%d", &id);
    while (getchar() != '\n');
    
    printf("Введите имя файла для сохранения: ");
    scanf(" %[^\n]", output_name);
    
    char full_path[512];
    snprintf(full_path, sizeof(full_path), "data/%s", output_name);
    
    const char *sql = "SELECT photo FROM companies WHERE id = ?;";
    sqlite3_stmt *stmt;
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Ошибка подготовки запроса: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    sqlite3_bind_int(stmt, 1, id);
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int bytes = sqlite3_column_bytes(stmt, 0);
        const void *photo_data = sqlite3_column_blob(stmt, 0);
        
        if (bytes > 0 && photo_data != NULL) {
            FILE *fp = fopen(full_path, "wb");
            if (fp) {
                fwrite(photo_data, 1, bytes, fp);
                fclose(fp);
                printf("Фото экспортировано в %s\n", full_path);
            } else {
                fprintf(stderr, "Cannot create file: %s\n", full_path);
            }
        } else {
            printf("У компании нет фото\n");
        }
    } else {
        printf("Компания с ID %d не найдена\n", id);
    }
    
    sqlite3_finalize(stmt);
}

void demo_autocommit(sqlite3* db) {
    printf("\n=== ДЕМОНСТРАЦИЯ AUTOCOMMIT ===\n");
    printf("Каждая INSERT операция фиксируется сразу\n\n");
    
    // Очистка старых тестовых данных
    sqlite3_exec(db, "DELETE FROM companies WHERE name LIKE 'AutoTest%';", 0, 0, 0);
    
    for (int i = 1; i <= 3; i++) {
        // Указываем ВСЕ поля, которые NOT NULL
        char sql[1024];
        snprintf(sql, sizeof(sql),
            "INSERT INTO companies (name, website, postal_code, country, region, district, "
            "city, street, house, apartment, foundation_year, years_in_market, "
            "employees_count, european_branches, category_id) "
            "VALUES ('AutoTest%d', 'test%d.com', '000000', 'TestCountry', 'TestRegion', 'TestDistrict', "
            "'TestCity', 'TestStreet', '1', '1', 2020, 5, 100, 2, 1);",
            i, i);
        
        printf("Выполнение INSERT %d... ", i);
        char *err_msg = 0;
        if (sqlite3_exec(db, sql, 0, 0, &err_msg) == SQLITE_OK) {
            printf("AUTOCOMMIT - данные сохранены\n");
        } else {
            printf("ОШИБКА: %s\n", err_msg);
            sqlite3_free(err_msg);
        }
    }
}

void demo_transaction(sqlite3* db) {
    printf("\n=== ДЕМОНСТРАЦИЯ ТРАНЗАКЦИИ ===\n");
    printf("Все операции выполняются в одной транзакции\n\n");
    
    char *err_msg = 0;
    
    // Начало транзакции
    sqlite3_exec(db, "BEGIN TRANSACTION;", 0, 0, &err_msg);
    
    // Очистка старых тестовых данных
    sqlite3_exec(db, "DELETE FROM companies WHERE name LIKE 'TransTest%';", 0, 0, 0);
    
    int success = 1;
    for (int i = 1; i <= 3; i++) {
        char sql[1024];
        snprintf(sql, sizeof(sql),
            "INSERT INTO companies (name, website, postal_code, country, region, district, "
            "city, street, house, apartment, foundation_year, years_in_market, "
            "employees_count, european_branches, category_id) "
            "VALUES ('TransTest%d', 'test%d.com', '000000', 'TestCountry', 'TestRegion', 'TestDistrict', "
            "'TestCity', 'TestStreet', '1', '1', 2020, 5, 100, 2, 1);",
            i, i);
        
        printf("Выполнение INSERT %d... ", i);
        if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK) {
            printf("ОШИБКА: %s\n", err_msg);
            sqlite3_free(err_msg);
            success = 0;
            break;
        }
        printf("OK\n");
    }
    
    if (success) {
        sqlite3_exec(db, "COMMIT;", 0, 0, &err_msg);
        printf("\nТРАНЗАКЦИЯ ПОДТВЕРЖДЕНА - все данные сохранены\n");
    } else {
        sqlite3_exec(db, "ROLLBACK;", 0, 0, &err_msg);
        printf("\nТРАНЗАКЦИЯ ОТМЕНЕНА - данные не сохранены\n");
    }
}
