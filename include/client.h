#ifndef CLIENT_H
#define CLIENT_H

#include "base.h"
#include <stdbool.h>

struct Client
{
    /**Hereda from base. */
    struct Base base;
    /**Id unico del cliente. */
    unsigned id;
    /**Nombre por el cual se registro el usuario. */
    char *username;
    /**Contrasena con la cual se registro el usuario. */
    char *password;

    // !Metodos
    bool (*make_deposit)(struct Client *const self);
    bool (*new_loan)();
    bool (*make_transation)();
    void (*show_client_status)();
    void (*exchange_foreign_money)();
    void (*save_object)();
};

/**
 * @brief Liberia la memoria de un objeto.
 * 
 * @param self Direccion de memoria en donde se encuentra alocado el objeto.
 */
void delete_object(struct Client *const self);

/**
 * @brief Construye un nuevo cliente.
 * 
 * @param self El cliente a construir
 * @return struct Client* Direccion de memoria para el cliente.
 */
struct Client *__init__(struct Client *self, const char *username,
                               const char *password, const unsigned id);

/**
 * @brief Realiza un deposito. El registro sera guardado en la database.
 * 
 * @return true Se ha realizado el deposito con el exito.
 * @return false Error.
 */
void make_deposit(struct Client *const self);

/**
 * @brief Realiza un prestamo al banco.
 * 
 * @return true Si hay suficente dinero en el banco. El registo sera guardado
 * en la database.
 * @return false El banco no tiene suficiente dinero.
 */
void new_loan(struct Client *const self);

/**
 * @brief Realiza una transaccion. El dinero usado sera restado. 
 * Por ejemplo: dar dinero a otro usuario.
 * 
 * @return true Se ha realizado el transaccion con exito. El registo sera guardado
 * en la database.
 * @return false La transaccion ha fallado.
 */
void make_transation(struct Client *const self);

/**
 * @brief Convierte de una divisa a otra.
 * 
 */
void exchange_foreign_money(struct Client *const self);

/**
 * @brief Guarda un objeto del cliente en el banco.
 * 
 */
void save_object();

void pay_loan(struct Client *const self);

double convert(const double amount, const unsigned option, const unsigned type);

void save_object_in_db(struct Client *const self);
#endif //CLIENT_H
