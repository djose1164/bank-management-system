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

void make_deposit(struct Client *const self)
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
}

void new_loan(struct Client *const self)
{
    /**El dinero el cual sera prestado. */
    double requested_cash;
    /**Donde se recogera el input. Prar fgets. */
    char line[sizeof(double)];

    clear_screen();

    /**Pide por teclado el dinero a solicitar. */
    printf("\t\aHola! Aca podra tomar un prestamo!\n"
           "\tIngrese la cantidad a solicitar: ");
    fgets(line, sizeof(line), stdin);
    sscanf(line, "%lf", &requested_cash);

    self->base.loans_made += 1;
    self->base.loan_total += requested_cash;

    save_new_loan(self->id, requested_cash);
}
/*
void show_client_status(struct Client *const self)
{
    self->base.available_cash = self->base.deposit_total +
                                self->base.loan_total;
}
*/
void make_transation(struct Client *const self)
{
    char line[sizeof(double)];
    unsigned id;
    double cash;

    clear_screen();

    printf("\t\aHola! Aca podras pasar dinero a otra persona.\n"
           "\tPara ello deberas suministar el ID, y luego la cantidad a tranferir!\n"
           "Ingresa el ID: ");
    fgets(line, sizeof(line), stdin);
    sscanf(line, "%u", &id);

    printf("Ingresa la cantidad a tranferir: ");
    fgets(line, sizeof(line), stdin);
    sscanf(line, "%lf", &cash);

    subtract_cash(self->id, id, cash);
}

void pay_loan(struct Client *const self)
{
    char line[sizeof(double)];
    double cash;

    clear_screen();
    
    printf("\t\aHola! Aca podra pagar tus prestamos!\n"
           "Ingrese el monto a pagar: ");
    fgets(line, sizeof(line), stdin);
    sscanf(line, "%lf", &cash);

    if (payment(self->id, cash))
        printf("TU pago se ha registrado con exito!\n");
    else
        fprintf(stderr, "No se ha podido realizar el pago.\n");
}