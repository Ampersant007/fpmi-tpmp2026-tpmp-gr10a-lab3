#include <stdio.h>
#include <stdlib.h>
#include "database.h"

#define DB_PATH "data/test.db"

void print_menu() {
    printf("\n========================================\n");
    printf("  УПРАВЛЕНИЕ МЕЖДУНАРОДНЫМИ КОМПАНИЯМИ\n");
    printf("========================================\n");
    printf("1.  Добавить компанию\n");
    printf("2.  Показать все компании\n");
    printf("3.  Найти компанию по ID\n");
    printf("4.  Найти компанию по фрагменту названия\n");
    printf("5.  Найти компании по категории\n");
    printf("6.  Удалить компанию\n");
    printf("7.  Добавить фото компании\n");
    printf("8.  Экспортировать фото компании\n");
    printf("9.  ДЕМО: Autocommit режим\n");
    printf("10. ДЕМО: Транзакция\n");
    printf("0.  Выход\n");
    printf("----------------------------------------\n");
    printf("Выберите опцию: ");
}

int main() {
    sqlite3 *db = db_open(DB_PATH);
    if (!db) {
        fprintf(stderr, "Не удалось открыть базу данных\n");
        return 1;
    }
    
    int choice;
    
    do {
        print_menu();
        scanf("%d", &choice);
        getchar();
        
        switch(choice) {
            case 1:
                add_company(db);
                break;
            case 2:
                list_all_companies(db);
                break;
            case 3:
                find_company_by_id(db);
                break;
            case 4:
                find_company_by_name_pattern(db);
                break;
            case 5:
                find_company_by_category(db);
                break;
            case 6:
                delete_company(db);
                break;
            case 7:
                add_company_photo(db);
                break;
            case 8:
                export_company_photo(db);
                break;
            case 9:
                demo_autocommit(db);
                break;
            case 10:
                demo_transaction(db);
                break;
            case 0:
                printf("Выход из программы...\n");
                break;
            default:
                printf("Неверная опция! Попробуйте снова.\n");
        }
    } while(choice != 0);
    
    db_close(db);
    return 0;
}
