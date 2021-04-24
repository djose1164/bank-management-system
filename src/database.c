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
                  "loan_total DOUBLE DEFAULT 0.0,"
                  "euros DOUBLE DEFAULT 0.0,"
                  "dollars DOUBLE DEFAULT 0.0);";
    __create_table__(table_query);

    init_bank();
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

void show_client_status(struct Client *const self)
{
    __init_database__(database_name);
    char *errmsg;
    int conn;
    char *sql = "SELECT deposit_count, loan_count, deposit_total, loan_total,"
                "euros, dollars "
                "FROM clients "
                "WHERE id = ?;";

    int callback(void *data, int column_count, char **columns, char **columns_names);

    conn = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
    check_error(conn, db);

    conn = sqlite3_bind_int(res, 1, self->id);
    check_error(conn, db);

    system("cls||clear");

    int step = sqlite3_step(res);

    if (step == SQLITE_ROW)
        printf("\t\aHola! Ahora mismo estas viendo el estado acutal de tu cuenta!\n\n"
               "En la primera columna estan el numero de veces que haz realizado un deposito.\n"
               "En la segunda, la cantidad de veces que haz realizado un prestamo.\n"
               "Y por ultimo, en las dos ultimas columnas, se muestran los totales!\n\n"
               "*---------------*---------------*---------------*---------------*\n"
               "|%-15s|%-15s|%-15s|%-15s|\n",
               sqlite3_column_name(res, 0),
               sqlite3_column_name(res, 1),
               sqlite3_column_name(res, 2),
               sqlite3_column_name(res, 3),
               sqlite3_column_name(res, 4),
               sqlite3_column_name(res, 5));

    printf("*---------------*---------------*---------------*---------------*\n");

    for (size_t i = 0; i < 4; ++i)
    {
        printf("|%-15s", sqlite3_column_text(res, i));

        if (i == 3)
            printf("|\n");
    }
    // Para la ultima linea de la tabla.
    printf("*---------------*---------------*---------------*---------------*\n");
    sqlite3_finalize(res);
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
    }
    else
        printf("No se ha podido realizar la transaccion.\n");
}

bool payment(const unsigned id, const double cash)
{
    int conn;
    char *sql = "UPDATE clients "
                "SET loan_total = loan_total - ? "
                "WHERE id = ?;";

    conn = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
    check_error(conn, db);

    conn = sqlite3_bind_double(res, 1, cash);
    check_error(conn, db);
    conn = sqlite3_bind_int(res, 2, id);

    bool status = sqlite3_step(res) == SQLITE_DONE;
    sqlite3_finalize(res);
    return status;
}

void buy_divisas(const unsigned id, const double amount, const double received,
                 const unsigned option, const unsigned type)
{
    int conn;
    char *sql;

    /**
     * @brief Primero, se verifica que haya suficiente dinero en el barco para
     * la compra y/o venta de las divisas. Si hay procedera con la compra/venta
     * de las divisas. En la compra, se verificara que el usuario tiene suficiente
     * dinero, si lo tiene se le descontara lo necesario y el banco le dara la 
     * divisa.
     * La divisa que el banco de sera restada del banco.
     * Para la venta, primero se verifica q el banco tiene suficiente dinero
     * para adquirir la divisa, si lo tiene se le dara dado el dinero al usuario
     * y se le descontara la divisa. 
     * 
     */
    switch (option)
    {
    case COMPRAR:
        /**
         * @brief Verifica q el banco y el cliente tienen fundos suficientes para
         * la operacion; si cuentan con el saldo suficiente, se le descuenta
         * al cliente.
         * 
         */
        sql = "UPDATE clients "
              "SET deposit_total = deposit_total - ? "
              "WHERE id = ? AND deposit_total > ? AND "
              "(SELECT available_cash_eur FROM bank) > ?;";
        conn = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
        check_error(conn, db);

        conn = sqlite3_bind_double(res, 1, amount);
        check_error(conn, db);
        conn = sqlite3_bind_int(res, 2, id);
        check_error(conn, db);
        conn = sqlite3_bind_double(res, 3, amount);
        check_error(conn, db);
        conn = sqlite3_bind_int(res, 4, received);
        check_error(conn, db);

        /**
         * @brief Si se pudo restar el dinero necesario del client entonces
         * se continua con la transferencia de la divisa: desde el banco 
         * al cliente.
         * 
         */
        if ((sqlite3_step(res) == SQLITE_DONE))
        {
            sqlite3_reset(res);
            switch (type)
            {
            case EUROS:
                /**
                 * Descuenta la divisa que se le dara del banco al cliente, y
                 * esta misma se le agrega al cliente.
                 */
                sql = "UPDATE clients "
                      "SET euros = euros + ? "
                      "WHERE id = ?;";

                conn = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
                check_error(conn, db);

                conn = sqlite3_bind_double(res, 1, received);
                check_error(conn, db);
                conn = sqlite3_bind_int(res, 2, id);
                check_error(conn, db);

                if (sqlite3_step(res) == SQLITE_DONE)
                {
                    sqlite3_reset(res);
                    sql = "UPDATE bank "
                          "SET available_cash_eur = available_cash_eur - ?;";
                    conn = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
                    check_error(conn, db);
                    conn = sqlite3_bind_double(res, 1, received);
                    check_error(conn, db);
                    if (sqlite3_step(res) == SQLITE_DONE)
                    {
                        printf("Las divisas se han agregado a tu cuenta!\n");
                        sqlite3_finalize(res);
                        return;
                    }
                }
                else
                {
                    fprintf(stderr, "Error al comprar las divasas.\n");
                    sqlite3_finalize(res);
                    sqlite3_close(db);
                    exit(-1);
                }
                break;

            case DOLARES:
                sql = "UPDATE clients "
                      "SET dollars = dollars + ? "
                      "WHERE id = ?;";

                conn = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
                check_error(conn, db);

                conn = sqlite3_bind_double(res, 1, received);
                check_error(conn, db);
                conn = sqlite3_bind_int(res, 2, id);
                check_error(conn, db);

                if (sqlite3_step(res) == SQLITE_DONE)
                {
                    sqlite3_reset(res);
                    sql = "UPDATE bank "
                          "SET available_cash_usd = available_cash_usd - ?;";
                    conn = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
                    check_error(conn, db);
                    conn = sqlite3_bind_double(res, 1, received);
                    check_error(conn, db);
                    if (sqlite3_step(res) == SQLITE_DONE)
                    {
                        printf("Las divisas se han agregado a tu cuenta!\n");
                        sqlite3_finalize(res);
                        return;
                    }
                }
                else
                {
                    fprintf(stderr, "Error al comprar las divasas.\n");
                    sqlite3_finalize(res);
                    sqlite3_close(db);
                    exit(-1);
                }
                break;

            default:
                break;
            }
        }
        break;

    case VENDER:
        sql = "UPDATE bank "
              "SET available_cash_dop = available_cash_dop - ? "
              "WHERE available_cash_dop > ? AND "
              "(SELECT euros FROM clients Where id = ?) > ?;";
        conn = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
        check_error(conn, db);

        conn = sqlite3_bind_double(res, 1, amount);
        check_error(conn, db);
        conn = sqlite3_bind_int(res, 2, id);
        check_error(conn, db);
        conn = sqlite3_bind_double(res, 3, amount);
        check_error(conn, db);
        conn = sqlite3_bind_int(res, 4, received);
        check_error(conn, db);

        /**
         * @brief Si se pudo restar el dinero necesario del client entonces
         * se continua con la transferencia de la divisa: desde el banco 
         * al cliente.
         * 
         */
        if ((sqlite3_step(res) == SQLITE_DONE))
        {
            sqlite3_reset(res);
            switch (type)
            {
            case EUROS:
                /**
                 * Descuenta la divisa que se le dara del banco al cliente, y
                 * esta misma se le agrega al cliente.
                 */
                sql = "UPDATE clients "
                      "SET euros = euros + ? "
                      "WHERE id = ?;";

                conn = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
                check_error(conn, db);

                conn = sqlite3_bind_double(res, 1, received);
                check_error(conn, db);
                conn = sqlite3_bind_int(res, 2, id);
                check_error(conn, db);

                if (sqlite3_step(res) == SQLITE_DONE)
                {
                    sqlite3_reset(res);
                    sql = "UPDATE bank "
                          "SET available_cash_eur = available_cash_eur - ?;";
                    conn = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
                    check_error(conn, db);
                    conn = sqlite3_bind_double(res, 1, received);
                    check_error(conn, db);
                    if (sqlite3_step(res) == SQLITE_DONE)
                    {
                        printf("Las divisas se han agregado a tu cuenta!\n");
                        sqlite3_finalize(res);
                        return;
                    }
                }
                else
                {
                    fprintf(stderr, "Error al comprar las divasas.\n");
                    sqlite3_finalize(res);
                    sqlite3_close(db);
                    exit(-1);
                }
                break;

            case DOLARES:
                sql = "UPDATE clients "
                      "SET dollars = dollars + ? "
                      "WHERE id = ?;";

                conn = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
                check_error(conn, db);

                conn = sqlite3_bind_double(res, 1, received);
                check_error(conn, db);
                conn = sqlite3_bind_int(res, 2, id);
                check_error(conn, db);

                if (sqlite3_step(res) == SQLITE_DONE)
                {
                    sqlite3_reset(res);
                    sql = "UPDATE bank "
                          "SET available_cash_usd = available_cash_usd - ?;";
                    conn = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
                    check_error(conn, db);
                    conn = sqlite3_bind_double(res, 1, received);
                    check_error(conn, db);
                    if (sqlite3_step(res) == SQLITE_DONE)
                    {
                        printf("Las divisas se han agregado a tu cuenta!\n");
                        sqlite3_finalize(res);
                        return;
                    }
                }
                else
                {
                    fprintf(stderr, "Error al comprar las divasas.\n");
                    sqlite3_finalize(res);
                    sqlite3_close(db);
                    exit(-1);
                }
                break;

            default:
                break;
            }
        }
        break;
    }
}

static void init_bank()
{
    int conn;
    char *sql;
    char *table_query;

    sql = "SELECT id FROM clients WHERE id = 1;";

    conn = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
    check_error(conn, db);
    printf("## Before\n");
    if (!(sqlite3_step(res) == SQLITE_ROW))
    {
        sqlite3_reset(res);
        // Crea una tabla para el banco.
        table_query = "CREATE TABLE IF NOT EXISTS bank("
                      "available_cash_dop DOUBLE DEFAULT 100000.00,"
                      "available_cash_usd DOUBLE DEFAULT 100000.00,"
                      "available_cash_eur DOUBLE DEFAULT 100000.00);";
        __create_table__(table_query);

        /**Agrega dinero al banco. */
        char *sql = "INSERT INTO bank("
                    "available_cash_dop) VALUES("
                    "100000.00);";
        conn = sqlite3_exec(db, sql, NULL, NULL, NULL);
        check_error(conn, db);
    }

    sqlite3_finalize(res);
}