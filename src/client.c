#include "../include/client.h"
#include "../include/login.h"
#include "../include/database.h"
#include <stdlib.h>
#include <stdio.h>

struct Client *__init__(struct Client *self, const char *username,
                        const char *password, const unsigned id)
{
    /**Aloca la memoria necesaria. */
    if (!self)
        self = malloc(sizeof(struct Client));
    /**No se ha podido a locar memoria; notifica y cierra el programa. */
    if (!self)
    {
        fprintf(stderr, "No se ha podido alocar memoria.\n");
        exit(-1);
    }

    self->id = id;
    self->username = (char *)username;
    self->password = (char *)password;
    /*self->new_loan = new_loan;
    self->make_transation = make_transation;
    self->show_client_status = show_client_status;
    self->exchange_foreign_money = exchange_foreign_money;
    self->save_object = save_object;*/

    return self; /**Devuelve el puntero de la memoria alocada. */
}

void delete_object(struct Client *const self)
{
    /**Verifica que no sea un puntero NULL */
    if (self)
        free(self);
    /**Es NULL. */
    else
        return;
}

bool make_deposit(struct Client *const self)
{
    /**Para el uso de fgets y sscanf. */
    char line[sizeof(double)];

    clear_screen();

    /**Pide por la cantidad de dinero que el usuario quiere depositar. */
    printf("\t\aHola! Aca podras realizar tu deposito!\n"
           "\tIngrese la cantidad a depositar: ");
    fgets(line, sizeof(line), stdin);
    sscanf(line, "%lf", &self->base.available_cash);

    save_new_deposit(self->id, self->base.available_cash);
    return true;
}