/*
 * Soubor:  proj1.c
 * Datum:   11.11.2007
 * Autor:   Radim Kubi�, xkubis03@stud.fit.vutbr.cz
 * Projekt: ��seln� soustavy, projekt �. 1 pro p�edm�t IZP
 * Popis:   Program p�ev�d� p�irozen� ��sla mezi ��seln�mi soustavami
 *          o ��seln�m z�kladu 2 a� 36 v rozmez� ��sel datov�ho typu
 *          unsigned long long.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const int MIN_BASE = 2;
const int MAX_BASE = 36;

/* K�dy chyb a stav� programu */
enum errors
{
 OK = 0,    /* Bez chyby */
 BAD_CMD,   /* Chyba p��kazov�ho ��dku */
 BAD_IN,    /* Chybn� vstup */
 BAD_VALUE, /* Vstupn� ��slo je v�t�� ne� lze vyj�d�it v unsigned long long */
 OTHER,     /* Nezn�m� chyba */
 HELP,      /* N�pov�da */
 END,       /* Zar�ka */
};

/* Pole textov�ch �et�zc� vypisovan�ch funkc� printErr. */
const char *ERRMSG[] =
{
 /* OK */
 "V�e je v po��dku.\n",
 /* BAD_CMD */
 "Chybn� parametry p��kazov�ho ��dku!\n",
 /* BAD_IN */
 "Chyba ve vstupn�ch datech!\n",
 /* BAD_VALUE */
 "Vstupn� hodnota je v�t�� ne� unsigned long long!",
 /* OTHER */
 "Nezn�m� chyba!\n",
 /* HELP */
 "Program ��seln� soustavy\n"
 "Autor: Radim Kubi� (c) 2007\n"
 "Program p�ev�d� p�irozen� ��sla mezi ��seln�mi soustavami o ��seln�m\n"
 "z�kladu 2 a� 36 v rozmez� datov�ho typu unsigned long long.\n"
 "Pou�it�: proj1 -h\n"
 "         proj1 OUT\n"
 "Popis parametr�:\n"
 "  -h   - vyp�e n�pov�du\n"
 "  OUT  - z�klad v�stupn� ��seln� soustavy\n"
 "P�i pou�it� parametru OUT program o�ek�v� vstup:\n"
 "  Z ZVALUE\n"
 "   - Z je z�klad ��seln� soustavy, ve kter� je vstupn� hodnota ��sla ZVALUE\n"
};

/**
 * Struktura obsahuj�c� hodnoty parametr� p��kazov� ��dky.
 */
typedef struct params
{
 int out;   /* Z�klad ��seln� soustavy v�stupn�ho ��sla. */
 int code;  /* Stavov� k�d programu. */
} TParams;

/**
 * Vytiskne k�d chyby nebo text popisuj�c� stav programu.
 * @param code k�d chyby nebo stavu programu
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
 * Zpracuje argumenty p��kazov�ho ��dku a vr�t� je ve struktu�e TParams.
 * Pokud je form�t argument� chybn�, ukon�� program s chybov�m k�dem.
 * @param argc Po�et argument�.
 * @param argv Pole textov�ch �et�zc� s argumenty.
 * @return Vrac� analyzovan� argumenty p��kazov�ho ��dku.
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
 { // �patn� po�et parametr�
  result.code = BAD_CMD;
 }
 else
 { // po�et parametr� je spr�vn�
  if(strcmp("-h", argv[1]) == 0)
  { // tisk n�pov�dy
   result.code = HELP;
   return result;
  }
  // p�evod argv[1] na ��selnou hodnotu -> result.out
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
  // kontrola z�kladu v�stupn� ��seln� soustavy
  if(result.out < MIN_BASE || result.out > MAX_BASE)
  {
   result.code = BAD_IN;
  }
 }

 return result;
}

/**
 * P�e�te ��selnou hodnotu ze vstupu podle zadan�ho z�kladu ��seln� soustavy
 * a vr�t� jeho hodnotu. Pokud se na vstupu objev� znaky, kter� do dan�
 * ��seln� soustavy nepat��, funkce vr�t� chybov� k�d. Pokud na za��tku vstupu
 * budou b�l� znaky, funkce je p�esko��.
 * @param base  Z�klad ��seln� soustavy.
 * @param value P�ed�v�no odkazem. V t�to prom�nn� bude v�sledek, pokud v�e
 *              prob�hlo v po��dku.
 * @return Vrac� chybov� k�d, pokud se vyskytne chyba.
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
 * Vytiskne ��slo na standardn� v�stup v zadan� ��seln� soustav�.
 * @param value Hodnota ��sla, kter� se bude tisknout. Mus� to b�t kladn� ��slo.
 * @param base  Z�klad ��seln� soustavy, ve kter� se bude ��slo tisknout.
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
 * Hlavn� program.
 */
int main(int argc, char *argv[])
{
 int code = OK;
 unsigned long long value = 0;

 TParams params = getParams(argc, argv);

 if (params.code != OK)
 { // n�co nestandardn�ho
  printErr(params.code);
  return (params.code == HELP)? EXIT_SUCCESS : EXIT_FAILURE;
 }

 code = readValue(10, &value);

 if(code != OK)
 { // n�co nestandardn�ho
  printErr(code);
  return EXIT_FAILURE;
 }

 if(value > MAX_BASE || value < MIN_BASE)
 { // n�co nestandardn�ho
  printf("aha - %llu\n", value);
  printErr(BAD_IN);
  return EXIT_FAILURE;
 }

 code = readValue(value, &value);

 if(code != OK)
 { // n�co nestandardn�ho
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
