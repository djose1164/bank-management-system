/**
 * @file Database.h
 * @brief Prototipos y declaraciones para el manejo de la base de datos.
 * 
 * IMPORTANT: No definar nada aca.
 * 
 * @author @djose1164
 */

#ifndef DATABASE_H
#define DATABASE_H

// Librerias necesarias.
#ifdef __WIN32
#include "../lib/include/sqlite3.h"
#else
#include <sqlite3.h>
#endif //! __WIN32
#include <stdbool.h>
#include <stddef.h>
#include "../include/client.h"

/**Struct donde se almacenara los datos a guardar. Deberas pasar esto a su
 * debida funcion.
 */
struct users_to_insert
{
    // Nombre del usuario a guardar.
    char *username;
    // Password del usuario a guardar.
    char *password;
    // Esta llena?
    bool full;
};

enum return_validate
{
    admin,
    guest,
    not_found
};

enum
{
    COMPRAR = 1,
    VENDER,
    EUROS,
    DOLARES
};

// Variables para el manejo de la base de datos.
extern sqlite3 *db;
extern sqlite3_stmt *res;
extern const char *database_name;
static char *errmsg;
static int conn;
// El limite de usario que podran register por ejecucion del programa.
extern const size_t MAX_USERS;

//! *-*-*-*-*-*-*- Metodos internos, no deberian modificarse sin saber SQL.*-*-*-*-*-*-*-
/**
 * @brief Verifica que no haya error en la query.
 * En caso de lo que haya informara de ello y acabara la
 * ejecucion del programa.
 * 
 * @param conn Resultado de la query.
 * @param db Database usada.
 */
static void check_error(int conn, sqlite3 *db);

/**
 * @brief Para uso interno.
 * 
 * TODO: Hacerlo mas abstracto.
 * 
 */
static int __validate__(const char *const username, const char *const password);

/**
 * @brief Crea una nueva database si no existe.
 * Ej: Si pasas como nombre test.db y test.db no esta en tu actual directorio (donde estas ejucutando,
 * el programa) creara una nueva database; en caso de que exista creara una conexion.
 * 
 * IMPORTANT: Para uso interno
 * 
 * @param database_name 
 * @return int 
 */
static int __init_database__(const char *database_name);

/**
 * @brief Crea una nueva tabla. Para ver como crear un tabla visita el siguiente link:
 * https://www.tutorialesprogramacionya.com/sqliteya/detalleconcepto.php?punto=67&codigo=67&inicio=60
 * El link fue escogido aleatoriamente. En caso de dudas ver YT o algo.
 * 
 * @param query Codigo SQL para crear la tabla.
 * @return true Si fue exitoso.
 * @return false Si fallo.
 */
static void __create_table__(const char *query);

/**
 * IMPORTANT: Para uso interno.
 * 
 * @brief Funcion abstracta para complementar a otras o ser su base.
 * 
 * @param table_name El nombre de la tabla.
 * @param columns_name El nombre de la columna a la cual deseas agregar datos.
 * @param rows La cantidad de datos por fila 
 */
void __insert_into__(struct Client *const self);

/**
 * @brief Para uso interno.
 * 
 * @param len 
 * @return char* 
 */
static char *allocate_str(int len);

/**
 * @brief Devuelve el nombre del producto por su ID.
 * 
 * IMPORTANT: Solo get_column_value debe retornar esta funcion.
 * 
 * @param id ID del producto.
 * @return char* Devuelve un string si se pudo encontrar. NULL de lo contrario.
 */
static void *__get_name__(const unsigned id);

//! *-*-*-*-*-*-*- Metodos externos, pueden usarse sin problemas.*-*-*-*-*-*-*-
/**
 * @brief Anade a un nuevo usario a la database.
 * 
 * @param username Nombre del usuario.
 * @param password La contraseña.
 * @return int 
 */
void add_user(const char *username, const char *password);

/**
 * @brief Usalo para verificar el usario y la contraseña del usario.
 * 
 * @param username El username registrado.
 * @param password La contraseña regristrada.
 * @return int Retorna 0 si esta registrado. Retorna 1 si el username esta incorrecto
 *         Retorna -1 si el password esta incorrecto.
 */
extern int validate(const char *const username, const char *const password);

/**
 * @brief Para comentar.
 * 
 * @return true 
 * @return false 
 */
bool update();

/**
 * @brief Devolvera el valor de una columna. El retorna debe ser casteado con el
 * tipo de dato q desea.
 * 
 * @param id Obtendra el valor deseado del producto q tenga este ID.
 * @param __request_value Pasarse una opcion del enum __request_value
 * @return void* Un objeto q puede ser casteado a cualquier data type.
 */
void *get_column_value(const unsigned id, const unsigned __request_value);

/**
 * @brief Muestra el status del usuario
 * 
 */
void show_client_status(struct Client *const self);

void save_new_deposit(const unsigned id, const double cash);
void save_new_loan(const unsigned id, const double cash);
void save_new_loan();
void subtract_cash(const unsigned from, const unsigned to, double cash);
unsigned get_id(const char *username, const char *password);
bool payment(const unsigned id, const double cash);
void buy_divisas(const unsigned id, const double amount, const double received,
                 const unsigned option, const unsigned type);
static void init_bank();
#endif // !DATABASE_H