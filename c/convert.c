#include <stdio.h>

enum
{
    COMPRAR = 1,
    VENDER,
    EUROS,
    DOLARES
};

double convert(const double amount, const unsigned option, const unsigned type)
{
    switch (option)
    {
    case COMPRAR:
        switch (type)
        {
        case DOLARES:
            return (amount * 57.0);
        case EUROS:
            return (amount * 71.0);
        default:
            fprintf(stderr, "Opcion invalida\n");
            break;
        }
    case VENDER:
        switch (type)
        {
        case DOLARES:
            return (amount * 56.0);
        case EUROS:
            return (amount * 66.0);
        default:
            fprintf(stderr, "Opcion invalida\n");
            break;
        }
    default:
        fprintf(stderr, "En convert(); tipo invalido!\n");
        break;
    }
}
