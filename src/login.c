/**
 * @file login.c
 * @author Jose S. Daniel (djose1164@gmail.com)
 * @brief Un login con dos funciones principales: registro y logeo de un usario
 * y un menu principal de opciones.
 * @version 0.1
 * @date 2021-04-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <stdint.h>
#ifdef __WIN32
#include <windows.h> /* Windows dectetado. */
#include <conio.h>
#elif __linux__ /* Linux detectado. */
#include "../include/getch.h"
#include <unistd.h>
#endif //__WIN32
#include "../include/database.h"
#include "../include/login.h"

// Usuario actual que esta ejecutando el programa.
static struct actual_user actual_user;

// Tiempo que durara el copilador parado.
const short time = 1;

#define LONGITUD 50

void set_password(char *const password)
{
	/**
	 * @brief Por cada letra que el usario escriba pone un asterisco.
	 * Ej: Input:
	 * 			djose1164
	 *	  Output:
	 *			********
	 */

	/**Variable donde se almacenara cada caracter leido. */
	char c;
	for (size_t i = 0; (c = getch()); ++i)
	{
		/**
		 * @brief Muestra asterisco mintras c sea diferente a newline.
		 * 
		 */
		if (c == '\n' || c == '\r')
		{
			/**Se a encontrado una newline; lo cambia por null character. */
			password[i] = '\0';
			return;
		}
		else if (c == 8)
		{
			if (i > 0)
			{
				printf("\b \b"); //mueve el cursor hacia la izquierda
			}
		}
		else if (i < LONGITUD)
		{
			printf("*");
			password[i] = c;
		}
	}
}

// *-*-*-*-*-*-*-*-*-*-*-*- Login para el Menu *-*-*-*-*-*-*-*-*-*-*-*-

int login_menu()
{
	/**Este es el inicio, luego de entrar al sistema */
	unsigned options = 0;

	clear_screen();

	// Para que no se sienta la espera.
	printf("Empezando sistema de carga...\n");
	system_loading(time);

	/**+-+-+-+-+-+-Empieza el menu+-+-+-+-+-+- */

	// TODO: #14 Mejorar con un for y un contador de intentos.
	for (size_t i = 3; i > 0 || options > 5; --i)
	{
		system("cls||clear");

		if ((options <= 0 && i != 3) || options > 5)
			printf("Heyyy, debes eligir una opcion correcta!!!\n"
				   "Porque, pues te quedan %zu intentos.\n",
				   i);

		printf("\n\t\aQue quieres hacer? Ingresa el numero de la opcion deseada:  \n\n"
			   "\t1- Realizar un deposito.\n"
			   "\t2- Tomar un prestamo.\n"
			   "\t3- Transacciones.\n"
			   "\t4- Consulta de balance.\n"
			   "\t5- Pago de prestamo.\n"
			   "\t6- Cambio de moneda extranjera.\n"
			   "\t7- Guardar un objeto.\n"
			   "\t8- Salir.\n"); // El usuario saldra DEPOSITOcuando presione 5.
		scanf(" %d", &options);
		getchar();

		if (options >= DEPOSITO && options <= SALIR)
			break;
	}

	switch (options)
	{
	case DEPOSITO:
		if (inventory_menu())
			return login_menu();
	case PRESTAMO:
		return compras_menu();
	case TRANSACCIONES:
		return ventas_menu();

	case CONSULTA_BALANCE:
		return contabilidad_menu();
	case PAGO_PRESTAMO:
		
		break;
	case CAMBIO_MONEDA_EXTRANJERA:
		
		break;
	case GUARDAR_OBJETO:
		
		break;
	case SALIR:
		fflush(stdout);
		system("cls||clear");
		printf("Hackear a la NASA dejo de ser un sueno.\n");
		exit(0);
	default:
		fprintf(stderr, "Verifica que hayas ingresado tus credenciales correctamente o "
						"envia un issue detallando el posible bug.\n");
		break;
	}
	putchar('\n');
	return 0;
}

// *-*-*-*-*-*-*-*-*-*-*-*- Login para el Menu de user *-*-*-*-*-*-*-*-*-*-*-*-
/** IMPORTANT: Esta debe ser la primiera pantalla que el usario vea. */

int login_user()
{

	/**Donde se guardara la opcion eligida por el usuario. */
	unsigned temp = 0;
	int temp_validate;
	/** Cuenta las veces que intenta el usuario*/
	int chances = 0;

	// Para que no se sienta la espera.
	printf("Empezando sistema de carga...\n");
	system_loading(time);
	do
	{ /**Mientras el usuario no entre una opcion valida el loop se repetira. */
		printf("\n\t\t\t\aBienvenido al Banco Hackeando a la NASA!\n"
			   "\tSi ya estas registrado ingresa 2.\n"
			   "\tSi eres nuevo ingresa 1.\n"
			   "\t1) Registrarse.\n"
			   "\t2) Logearse.\n"
			   "\tOpcion: ");
		scanf(" %d", &temp);
		getchar();
		
		clear_screen()

		/**Imprime al usuario q coga una opcion correcta. */
		if (temp > 2)
			printf("Por favor elige una opcion correta.\n");
	} while (temp > 2);

	/**Dependiendo del valor en temp, el usuario se logeara o registrara. */
	switch (temp)
	{
	case 1: // Registrarse.

		printf("\t\t\aHola! Aca podras registrarte. Por favor llena los siguientes campos.\n"
			   "Username: ");
		fgets(actual_user.username, sizeof(actual_user.username), stdin);
		// Cambiar \n con \0
		actual_user.username[strcspn(actual_user.username, "\n")] = 0;
		(void)get_username();
		printf("Password: ");
		set_password(actual_user.password);

		printf("\nEs admin: ");
		scanf(" %d", &actual_user.is_admin);
		getchar();

		system("clear||cls");
		// Anade al usuario.
		add_user(actual_user.username, actual_user.password, actual_user.is_admin);

		// Luego entra en un blucle hasta que presione la letra de salir.
		for (; login_menu();)
			;

		return 0;

	case 2: // Logearse.
		for (size_t i = 3; i > 0;)
		{
			printf("\t\t\aHola! Estas actualmenteen en el login. Ingresa tus "
				   "credenciales para poder ingresar al sistema!\n\n");

			if (i != 3 && i >= 1)
				printf("\t\t\aUps! Tus credenciales no aparecen en la base de datos.\n"
					   "\t\aAsegurate de haber ingresado tus datos correctament. "
					   "Intentos restantes: %zu \n\n",
					   i);

			printf("\t\aUsername: ");
			fgets(actual_user.username, sizeof(actual_user.username), stdin);
			// Cambiar \n con \0
			actual_user.username[strcspn(actual_user.username, "\n")] = 0;

			printf("\t\aPassword: ");
			set_password(actual_user.password);

			temp_validate = validate(actual_user.username, actual_user.password);
			if (!temp_validate)
			// TODO: mostrar el login menu y/o mostrar un mensaje de que se ha logeado.
			{
				for (; login_menu();)
					;
				return 0;
			}
			else if (temp_validate == -1)
			{
				system("cls||clear");
				printf("\t\t\aUps! Esta es la primera vez que estas ejucutando el programa.\n\n"
					   "\tPor favor registrarse primero.\n"
					   "\tPresione cualquier tecla para finalizar la ejecucion...\n");
				getch();
				exit(-1);
			}
			else
			{
				--i;
			}
			// Limpia la pantalla para la segunda vuelta.
			system("cls||clear");
		}
	}

	return -1; // Error
}

void system_loading(int time)
{
#ifdef __WIN32
	time += time * 1000;
#endif //__WIN32

	for (size_t i = 0; i <= 100; i++)
	{
		if (i % 25 == 0)
		{
			fflush(stdout);
			printf("Hackeando a la NASA %zu%% completado.\r", i); /* Ahora hace 
			esencia con el nombre, jajaja. */
		}
		else
			continue;

		if (i == 100)
		{
			fflush(stdout);
			system("cls||clear");
		};
#ifdef __WIN32
		Sleep(time);
#else
		sleep(time);
#endif //__WIN32
	}
}

char *get_username()
{
	return actual_user.username;
}

void clear_screen()
{
	fflush(stdout);
	system("cls||clear");
}