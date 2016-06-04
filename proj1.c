/*
 * Soubor:  proj1.c
 * Datum:   11.11.2007
 * Autor:   Radim Kubi¹, xkubis03@stud.fit.vutbr.cz
 * Projekt: Èíselné soustavy, projekt è. 1 pro pøedmìt IZP
 * Popis:   Program pøevádí pøirozená èísla mezi èíselnými soustavami
 *          o èíselném základu 2 a¾ 36 v rozmezí èísel datového typu
 *          unsigned long long.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const int MIN_BASE = 2;
const int MAX_BASE = 36;

/* Kódy chyb a stavù programu */
enum errors
{
 OK = 0,    /* Bez chyby */
 BAD_CMD,   /* Chyba pøíkazového øádku */
 BAD_IN,    /* Chybný vstup */
 BAD_VALUE, /* Vstupní èíslo je vìt¹í ne¾ lze vyjádøit v unsigned long long */
 OTHER,     /* Neznámá chyba */
 HELP,      /* Nápovìda */
 END,       /* Zará¾ka */
};

/* Pole textových øetìzcù vypisovaných funkcí printErr. */
const char *ERRMSG[] =
{
 /* OK */
 "V¹e je v poøádku.\n",
 /* BAD_CMD */
 "Chybné parametry pøíkazového øádku!\n",
 /* BAD_IN */
 "Chyba ve vstupních datech!\n",
 /* BAD_VALUE */
 "Vstupní hodnota je vìt¹í ne¾ unsigned long long!",
 /* OTHER */
 "Neznámá chyba!\n",
 /* HELP */
 "Program Èíselné soustavy\n"
 "Autor: Radim Kubi¹ (c) 2007\n"
 "Program pøevádí pøirozená èísla mezi èíselnými soustavami o èíselném\n"
 "základu 2 a¾ 36 v rozmezí datového typu unsigned long long.\n"
 "Pou¾ití: proj1 -h\n"
 "         proj1 OUT\n"
 "Popis parametrù:\n"
 "  -h   - vypí¹e nápovìdu\n"
 "  OUT  - základ výstupní èíselné soustavy\n"
 "Pøi pou¾ití parametru OUT program oèekává vstup:\n"
 "  Z ZVALUE\n"
 "   - Z je základ èíselné soustavy, ve které je vstupní hodnota èísla ZVALUE\n"
};

/**
 * Struktura obsahující hodnoty parametrù pøíkazové øádky.
 */
typedef struct params
{
 int out;   /* Základ èíselné soustavy výstupního èísla. */
 int code;  /* Stavový kód programu. */
} TParams;

/**
 * Vytiskne kód chyby nebo text popisující stav programu.
 * @param code kód chyby nebo stavu programu
 */
void printErr(int code)
{
 if(code < OK || code >= END)
 {
  code = OTHER;
 }

 FILE *outstream = stdout;
 if(code <= OTHER)
 {
  outstream = stderr;
 }

 fprintf(outstream, "%s", ERRMSG[code]);
}

/**
 * Zpracuje argumenty pøíkazového øádku a vrátí je ve struktuøe TParams.
 * Pokud je formát argumentù chybný, ukonèí program s chybovým kódem.
 * @param argc Poèet argumentù.
 * @param argv Pole textových øetìzcù s argumenty.
 * @return Vrací analyzované argumenty pøíkazového øádku.
 */
TParams getParams(int argc, char *argv[])
{
 int index = 0;
 TParams result =
 { // inicializace struktury
  .out = 0,
  .code = OK,
 };

 if(argc != 2)
 { // ¹patný poèet parametrù
  result.code = BAD_CMD;
 }
 else
 { // poèet parametrù je správný
  if(strcmp("-h", argv[1]) == 0)
  { // tisk nápovìdy
   result.code = HELP;
   return result;
  }
  // pøevod argv[1] na èíselnou hodnotu -> result.out
  while(argv[1][index])
  {
   if(isdigit(argv[1][index]))
   {
    result.out *= 10;
    result.out += (argv[1][index] - '0');
   }
   else
   {
    result.code = BAD_IN;
    break;
   }
   index++;
  }
  // kontrola základu výstupní èíselné soustavy
  if(result.out < MIN_BASE || result.out > MAX_BASE)
  {
   result.code = BAD_IN;
  }
 }

 return result;
}

/**
 * Pøeète èíselnou hodnotu ze vstupu podle zadaného základu èíselné soustavy
 * a vrátí jeho hodnotu. Pokud se na vstupu objeví znaky, které do dané
 * èíselné soustavy nepatøí, funkce vrátí chybový kód. Pokud na zaèátku vstupu
 * budou bílé znaky, funkce je pøeskoèí.
 * @param base  Základ èíselné soustavy.
 * @param value Pøedáváno odkazem. V této promìnné bude výsledek, pokud v¹e
 *              probìhlo v poøádku.
 * @return Vrací chybový kód, pokud se vyskytne chyba.
 */
int readValue(unsigned long long base, unsigned long long *value)
{
 char znak;
 int help = 0;

 *value = 0;

 while((znak = getchar()))
 {
  if(isdigit(znak))
  {
   help = (znak - '0');
  }
  else if(isupper(znak))
  {
   help = (znak - 'A' + 10);
  }
  else if(islower(znak))
  {
   help = (znak - 'a' + 10);
  }
  else if(isspace(znak) && *value != 0)
  {
   break;
  }
  else if(isspace(znak) && *value == 0)
  {
   continue;
  }
  else
  {
   return BAD_IN;
  }

  if(*value > (((~(unsigned long long)0)-help)/base))
  {
   return BAD_VALUE;
  }
  if(help > (base-1))
  {
   return BAD_IN;
  }
  else
  {
   *value *= base;
   *value += help;
  }
 }

 return OK;
}

/**
 * Vytiskne èíslo na standardní výstup v zadané èíselné soustavì.
 * @param value Hodnota èísla, které se bude tisknout. Musí to být kladné èíslo.
 * @param base  Základ èíselné soustavy, ve které se bude èíslo tisknout.
 */
void printValue(unsigned long long value, int base)
{
 int hangover=0;
 char pole[65];
 int index = 0;

 while(value > 0)
 {

  hangover = value % base;
  value = (value - hangover) / base;

  if(hangover > 9)
  {
   pole[index] = hangover + 'A' - 10;
  }
  else
  {
   pole[index] = hangover + '0';
  }

  index++;
 }

 for(index=index-1; index>=0; index--)
 {
  putchar(pole[index]);
 }
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Hlavní program.
 */
int main(int argc, char *argv[])
{
 int code = OK;
 unsigned long long value = 0;

 TParams params = getParams(argc, argv);

 if (params.code != OK)
 { // nìco nestandardního
  printErr(params.code);
  return (params.code == HELP)? EXIT_SUCCESS : EXIT_FAILURE;
 }

 code = readValue(10, &value);

 if(code != OK)
 { // nìco nestandardního
  printErr(code);
  return EXIT_FAILURE;
 }

 if(value > MAX_BASE || value < MIN_BASE)
 { // nìco nestandardního
  printf("aha - %llu\n", value);
  printErr(BAD_IN);
  return EXIT_FAILURE;
 }

 code = readValue(value, &value);

 if(code != OK)
 { // nìco nestandardního
  printErr(code);
  return EXIT_FAILURE;
 }

 printValue(params.out, 10);

 putchar(' ');

 printValue(value, params.out);

 putchar('\n');

 return EXIT_SUCCESS;
}

/* konec proj1.c */
