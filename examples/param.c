#include <sqlite3.h> 
#include <stdio.h> 

int main(void) {     
    sqlite3 *db; 
    char *err_msg = 0; 
    sqlite3_stmt *res; 
    
    int rc = sqlite3_open("test2.db", &db); 
    
    if (rc != SQLITE_OK) {         
        fprintf(stderr, "Невозможно открыть базу данных: %s\n", sqlite3_errmsg(db)); 
        sqlite3_close(db);     
        return 1; 
    } 
    
    char *sql = "SELECT ID, NAME FROM company WHERE ID = ?";         
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);     
    if (rc == SQLITE_OK) {         
        sqlite3_bind_int(res, 1, 3); 
    } else {         
        fprintf(stderr, "Не удалось выполнить оператор: %s\n", sqlite3_errmsg(db)); 
    } 
    
    int step = sqlite3_step(res);     
    if (step == SQLITE_ROW) {         
        printf("%s: ", sqlite3_column_text(res, 0)); 
        printf("%s\n", sqlite3_column_text(res, 1));         
    } 
    sqlite3_finalize(res); 
    sqlite3_close(db); 
    return 0; 
}
