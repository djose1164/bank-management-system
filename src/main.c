#include <stdio.h>
#include "../include/login.h"
#include <string.h>

/************************************************************/
/*           Programacion para mecatronicos                 */
/*  Nombre: Jose S. Daniel Victoriano Bello                 */
/*  Matricula: 2020-10646                                   */
/*  Seccion: Miercoles                                      */
/*  Practica: Proyecto.                            			*/
/*  Fecha: 24/4/2021                                        */
/************************************************************/

/**
 * @brief Main es solo usado para inicar la ejecucion del programa.
 * 
 * @param argc Numero de argv
 * @param argv Los argumentos.
 * @return int 0 si fue exitoso.
 */
int main(const unsigned argc, const char **argv)
{
	/**Inicia la ejecucion del programa. */
	login_user();

	return (0);
}
