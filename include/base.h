#ifndef BASE_H
#define BASE_H

/**
 * @brief Informacion compartida entre el client y el bank.
 * 
 */
struct Base
{
    /**La cantidad total de los prestamos realizados. */
    double loan_total;
    /**Total de los depositos hechos. */
    double deposit_total;
    /**El dinero disponible. Se obtiene de los depositos realizados. */
    double available_cash;
    /**Cantidad de prestamos realizados. */
    unsigned loans_made;
    /**Cantidad de los depositos realizados. */
    unsigned deposits_made;

    // !Metodos
    void (*show_avialable_cash)();
    void (*show_loan_total)();
    void (*show_loans_made)();
    // !Setters
    void (*add_cash)();
    void (*subtract_cash)();
};

/**
 * @brief Muestra la cantidad de dinero disponible. Si es inferior a uno
 * no se podria llevar acabo ninga transaccion.
 * 
 */
static void show_avialable_cash();

/**
 * @brief Muestra el total de los prestamos realizados.
 * 
 */
static void show_loan_total();

/**
 * @brief Muestra la cantidad de los prestamos realizados hasta ahora.
 * 
 */
static void show_loan_made_quantity();

/**
 * @brief Anade dinero. Esto sera registrado en la base de datos.
 * 
 */
static void add_cash();

/**
 * @brief Descuenta dinero. Esto sera registrado en la base de datos.
 * 
 */
void subtract_cash();

#endif //BASE_H
