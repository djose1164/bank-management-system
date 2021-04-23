/**
 * @file database.c
 * @author @djose1164
 * @brief Implementacion/definicion del header database.h.
 * 
 * @version 1.0
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../include/database.h"
#include <string.h>

static short counter;

// Variables globales.
sqlite3 *db;
sqlite3_stmt *res = NULL;
static bool temp = true;
const char *database_name = "test.db";

/**
 * @brief Habilita el suficiente espacion en memoria para los strings.
 * 
 * @param string El string que se va a guardar.
 * @return char* El return de la direccion de memoria.
 */

char *allocate_str(int len)
{
    char *str = malloc(sizeof(char) * (len + 1));
    if (!str)
    {
        fprintf(stderr, "Couldn't allocate the memory.\n");
        exit(-1);
    }

    return str;
}

static void check_error(int conn, sqlite3 *db)
{
    if (conn != SQLITE_OK)
    {
        fprintf(stderr, "Error: %s.\n"
                        "The program is gonna close.\n",
                sqlite3_errmsg(db));
        exit(-1);
    }
}

static int __init_database__(const char *database_name)
{
    char *table_query;

#ifndef CONNECTED
#define CONNECTED
    int conn = sqlite3_open(database_name, &db);
    check_error(conn, db);

    // Crea una query para luego ser usada para crear la database
    table_query = "CREATE TABLE IF NOT EXISTS clients("
                  "id INTEGER PRIMARY KEY NOT NULL, "
                  "username TEXT,"
                  "password TEXT,"
                  "deposit_count INT DEFAULT 0,"
                  "loan_count INT DEFAULT 0,"
                  "deposit_total DOUBLE DEFAULT 0.0,"
                  "loan_total DOUBLE DEFAULT 0.0);";
    __create_table__(table_query);

    return 0;
#endif //CONNECTED

    // Error
    return -1;
}

static void __create_table__(const char *query)
{
    char *errmsg;
    int conn = sqlite3_exec(db, query, 0, 0, &errmsg);
    check_error(conn, db);
}

static int __validate__(const char *const username, const char *const password)
{
    char *errmsg;
    int conn;
    __init_database__(database_name);

    // Array de punteros a los datos a validar.2
    const char *to_validate[] = {
        username,
        password};

    // Array de punteros para la consulta. Solo cambiar si tenes conocimiento de SQL.
    char *queries = {
        "SELECT username, password "
        "FROM clients "
        "WHERE username = ? AND "
        "password = ?;"};

    // Prepara la coneccion.
    conn = sqlite3_prepare_v2(db, queries, -1, &res, NULL);
    if (conn == SQLITE_ERROR)
        return -1;
    check_error(conn, db);

    for (size_t i = 0; i <= (sizeof *queries) / (sizeof queries[0]); i++)
    {
        // Enlaza el valor (cambia ? por su debido valor).
        conn = sqlite3_bind_text(res, i + 1, to_validate[i], -1, NULL);
        check_error(conn, db);
    }

    int step;

    if ((step = sqlite3_step(res)) != SQLITE_ROW)
    {
        sqlite3_finalize(res);
        sqlite3_close(db);
        return false;
    }
    else
    {
        sqlite3_finalize(res);
        return true;
    }
    return -1;
}

int validate(const char *username, const char *password)
{
    return __validate__(username, password);
}

//! Realizar una consulta.

bool __make_query__(const char *query)
{
    __init_database__(database_name);
    char *errmsg;

    int callback(void *data, int column_count, char **columns, char **columns_names);

    int conn = sqlite3_exec(db, query, callback, NULL, &errmsg);
    if (conn == SQLITE_ERROR)
        return false;
    check_error(conn, db);
    // Para la ultima linea de la tabla.
    printf("*--------*--------------------*----------*----------*\n");
    return true;
}

//! Anandir nuevo usuario a la database.

void add_user(const char *username, const char *password)
{
    __init_database__(database_name);

    char *sql;
    char *table_query;
    const char *data[] = {
        username,
        password};

    // Almacena los datos por caa cada estructura vacia.
    sql = "INSERT INTO clients(username, password) "
          "VALUES(?, ?);";

    // Prepara la coneccion.
    conn = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
    check_error(conn, db);

    for (size_t i = 1; i <= 2; i++)
    {

        // Enlaza el valor (cambia ? por su debido valor).
        conn = sqlite3_bind_text(res, i, data[i - 1], -1, NULL);
        check_error(conn, db);
    }

    // Esta funcion es IMPORTANTISIMA. Cuanto tiempo perdi joder xD.
    int step = sqlite3_step(res);
    // La de arriba ^^^

    sqlite3_finalize(res);
}

//!-*-*-*-*-*-* Actualizar los datos -*-*-*-*-*-*

bool update(const unsigned id, const char *new_name,
            const unsigned *new_sellPrice, const int *new_availableQuantity)
{
    if (id <= 0)
        return false;
    /*
    if (new_name)
        return __update_name__(id, new_name);
    else if (new_sellPrice)
        return __update_price__(id, *new_sellPrice);
    else if (new_availableQuantity)
        return __update_quantity__(id, *new_availableQuantity);
*/
    return false;
}
/*
static bool __update_name__(const unsigned id, const char *new_name)
{
    int conn;
    char *errmsg;
    __init_database__(database_name);

    if (new_name == NULL)
        return false;

    char *sql = "UPDATE products "
                "SET nombre = ?"
                "WHERE id = ?;";

    conn = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
    check_error(conn, db);

    conn = sqlite3_bind_text(res, 1, new_name, -1, NULL);
    check_error(conn, db);

    conn = sqlite3_bind_int(res, 2, id);
    check_error(conn, db);

    conn = sqlite3_step(res);
    sqlite3_finalize(res);
    return conn == SQLITE_DONE;
}
*/

//! Obtener valores.

void *get_column_value(const unsigned id, const unsigned __request_value)
{
    __init_database__(database_name);
    if (id <= 0)
        return NULL;
    /*
    if (__request_value == NAME)
        return __get_name__(id);
    else if (__request_value == PRICE)
        return __get_price__(id);
    //else if (__request_value == QUANTITY)
    //    return __get_quantity__(id);
    else
        return (void *)-1;
        */
}

static void *__get_name__(const unsigned id)
{
    int conn;
    char *str;
    char *sql = "SELECT nombre "
                "FROM products "
                "WHERE id = ?;";
    conn = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
    check_error(conn, db);
    conn = sqlite3_bind_int(res, 1, id);
    check_error(conn, db);
    conn = sqlite3_step(res);

    str = conn == SQLITE_ROW ? (char *)sqlite3_column_text(res, 0) : NULL;
    void *temp = (void *)str;
    return temp;
}

unsigned get_id(const char *username, const char *password)
{
    __init_database__(database_name);

    int conn;
    unsigned temp;
    char *sql = "SELECT id "
                "FROM clients "
                "WHERE username = ? AND "
                "password = ?;";
    conn = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
    check_error(conn, db);
    conn = sqlite3_bind_text(res, 1, username, -1, NULL);
    check_error(conn, db);
    conn = sqlite3_bind_text(res, 2, password, -1, NULL);
    check_error(conn, db);
    conn = sqlite3_step(res);

    temp = conn == SQLITE_ROW ? sqlite3_column_int(res, 0) : 0;
    sqlite3_finalize(res);
    return temp;
}

void save_new_deposit(const unsigned id, const double cash)
{
    char *sql;
    int conn;

    /**Establece y/o crea la database. */
    __init_database__(database_name);

    /**Consulta.*/
    sql = "UPDATE clients "
          "SET deposit_count = deposit_count + 1, "
          "deposit_total = deposit_total + ?"
          "WHERE id = ?;";

    conn = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
    check_error(conn, db);

    sqlite3_bind_double(res, 1, cash);
    check_error(conn, db);
    sqlite3_bind_int(res, 2, id);
    check_error(conn, db);

    printf("%u %lf\n", id, cash);

    conn = sqlite3_step(res);
    if (conn == SQLITE_DONE)
        printf("\t\aDeposito realizado con exito!\n");
    else
        printf("\t\aNo se ha podido realizar el deposito!\n");
    sqlite3_finalize(res);
    //system("cls||clear");
}

void save_new_loan(const unsigned id, const double cash)
{
    char *sql;
    int conn;

    /**Establece y/o crea la database. */
    __init_database__(database_name);

    /**Consulta.*/
    sql = "UPDATE clients "
          "SET loan_count = loan_count + 1, "
          "loan_total = loan_total + ?"
          "WHERE id = ?;";

    conn = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
    check_error(conn, db);

    sqlite3_bind_double(res, 1, cash);
    check_error(conn, db);
    sqlite3_bind_int(res, 2, id);
    check_error(conn, db);

    conn = sqlite3_step(res);
    if (conn == SQLITE_DONE)
        printf("\t\aPrestamo realizado con exito!\n");
    else
        printf("\t\aNo se ha podido realizar el prestamo!\n");
    sqlite3_finalize(res);
    //system("cls||clear");
}

void subtract_cash(const unsigned from, const unsigned to, double cash)
{
    __init_database__(database_name);

    int conn;
    char *sql = "UPDATE clients "
                "SET deposit_total = deposit_total + ? "
                "WHERE id = ? AND deposit_total > ?;";

    conn = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
    check_error(conn, db);
    sqlite3_bind_double(res, 1, cash);
    check_error(conn, db);
    sqlite3_bind_int(res, 2, to);
    check_error(conn, db);
    sqlite3_bind_double(res, 3, from);
    check_error(conn, db);

    if (sqlite3_step(res) == SQLITE_DONE)
    {
        sqlite3_reset(res);
        printf("Transaccion realizada con exito!\n");
        sql = "UPDATE clients "
              "SET deposit_total = deposit_total - ?"
              "WHERE id = ?;";
        conn = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
        sqlite3_bind_double(res, 1, cash);
        sqlite3_bind_int(res, 2, from);

        if (sqlite3_step(res) == SQLITE_DONE)
        {
            sqlite3_finalize(res);
        }
        else
        {
            fprintf(stderr, "Error on subtract_cash!\n");
            exit(-1);
            sqlite3_finalize(res);
            sqlite3_close(db);
        }
        
        
    }else
        printf("No se ha podido realizar la transaccion.\n");
}

int callback(void *data, int column_count, char **columns, char **columns_names)
{
    if (temp)
        printf("*--------*--------------------*----------*----------*\n"
               "|%-8s|%-20s|%-10s|%-10s|\n",
               columns_names[0], columns_names[1], columns_names[2],
               columns_names[3]);

    temp = false;

    fflush(stdout);
    printf("*--------*--------------------*----------*----------*\n"
           "|%-8s|%-20s|%-10s|%-10s|\n",
           columns[0], columns[1], columns[2], columns[3]);

    return 0;
}