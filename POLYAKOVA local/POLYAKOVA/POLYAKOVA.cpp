// POLYAKOVA.cpp: главный файл проекта.
#include "stdafx.h" 
#include <stdio.h>//ввод-вывод
#include <stdlib.h>
#include <conio.h>//консольный ввод-вывод
#include <string.h>//работа со строками
#include <malloc.h>//динамическое выделение памяти
#include <clocale>//работа с локалью
#include <windows.h>

using namespace std;
using namespace System;
using namespace System::IO;

#define ENTER 13// Опред. констант для кнопок
#define ESC 27
#define UP 72
#define DOWN 80
#define HOME 75
#define END 77
/*
char dan[8][58]= 
{
             "Исполнитель с наибольшим количеством альбомов     ",
             "Самый непопулярный исполнитель                    ",
             "Список \"Русских\" исполнителей до 2000 года        ",
             "Алфавитный список всех жанров                     ",
             "Исполнители с количеством альбомов больше n       ",
             "Диаграмма популярности жанров                     ",
             "Исполнители с одинаковой популярн. из разных стран",
             "Выход                                             ",
};// Массив строк для меню, каждая строка - пункт меню
char BlankLine[ ]="                                                    ";
// Массив символов для пустой строки
*/

struct sp
{
	char genri[20];// Название жанра
	long summa;// Суммарная популярность жанра
	struct sp* pred; // Указатель на предыдущий элемент в списке
	struct sp* sled;// Указатель на следующий элемент в списке
} *spisok;

struct z
{
	char name[20];// Имя исполнителя
	char genre[20];// Жанр исполнителя
	long year;// Год выпуска альбома
	int popl;// Популярность (в тыс. чел.)
	char cntry[20]; // Страна исполнителя
	int albm;// Количество альбомов
};
// Структура для хранения данных о сравнении по популярности
struct v
{
	char cntry1[20];// Страна 1
	char cntry2[20]; // Страна 2
	char name1[20];// Имя исполнителя 1
	char name2[20];// Имя исполнителя 2
};
int menu(int, char **); //шаблоны используемых функций
void minim(int, struct z*);
void maxim(int, struct z*);
void listing(int, struct z*);
void udalit(struct sp**, char *);
void kolvo(int, struct z *);
void alfalist(int, struct z*, struct sp**);
void vstavka(int, struct z*, char*, struct sp**);
void listing(int, struct z*);
void diagram(int, struct z*);
void popu(int,struct z *);

void maxim(int NC,struct z* music)// Функция для нахожд исполн. с наиб. колвом альбомов
{
int i=0; struct z best;// Структура для хранения данных о лучшем исполнителе
strcpy(best.name,music[0].name);
strcpy(best.genre,music[0].genre);
best.albm=music[0].albm;
for(i=1;i<NC;i++)// Структура для хранения данных о лучшем исполнителе
	if (music[i].albm>best.albm)
		{
		strcpy(best.name,music[i].name);
		strcpy(best.genre,music[i].genre);
		best.albm=music[i].albm;
		}
	Console::ForegroundColor=ConsoleColor::Cyan;
Console::BackgroundColor=ConsoleColor::Black;
Console::CursorLeft=10;
Console::CursorTop=15;
printf("у исполнителя %s жанра %s",best.name,best.genre);
Console::CursorLeft=10;// Установка позиции курсора по горизонтали
Console::CursorTop=16;// Установка позиции курсора по вертикали
printf("%ld альбомов",best.albm);
getch();
}
void minim(int NC,struct z* music)// Функция для нахождения самого непопулярного исполнителя
{
int i=0; struct z worst;// Структура для хранения данных 
strcpy(worst.name,music[0].name);// Инициализация данных
strcpy(worst.genre,music[0].genre);
worst.popl=music[0].popl;
for(i=1;i<NC;i++)
	if (music[i].popl<worst.popl)
		{
		strcpy(worst.name,music[i].name);
		strcpy(worst.genre,music[i].genre);
		worst.popl=music[i].popl;
		}
	Console::ForegroundColor=ConsoleColor::Cyan;
Console::BackgroundColor=ConsoleColor::Black;
Console::CursorLeft=10;
Console::CursorTop=15;
printf("Исполнитель - %s Жанра %s",worst.name,worst.genre);
Console::CursorLeft=10;
Console::CursorTop=16;
printf("слушатели %d тыс.чел.",worst.popl);
getch();
}
void listing(int NC,struct z* music)// Функция для вывода списка "Русских" исполнителей до 2000 года
{
int i;
struct z* nt;
Console::ForegroundColor=ConsoleColor::DarkMagenta;
Console::BackgroundColor=ConsoleColor::Yellow;
Console::Clear();
printf("\n\r Список \"Русских\" исполнителей до 2000 года ");
printf("\n\r =====================================================\n\r");
for(i=0,nt=music;i<NC;nt++,i++)
        if (nt->year>=2000 && strcmp(nt->cntry,"Россия")==0)
		printf("\n\r  %-20s  %ld г.",nt->name,nt->year);
getch();
}

void vstavka(int NC,struct z *music,char *slovo, int k)// Функция для вставки нового элемента в список жанров
{
	int i;
struct sp *nov,*nt,*z=0; // Указатели на элементы списка жанров
for(nt=spisok; nt!=0 && strcmp(nt->genri,slovo)<0; z=nt, nt=nt->sled);
if(nt && strcmp(nt->genri,slovo)==0) return;// Если жанр уже есть в списке, то ничего не делаем
nov=(struct sp*)malloc(sizeof(struct sp));// Выделение памяти для нового элемента списка
strcpy(nov->genri,slovo);
nov->pred=z;
nov->sled=nt;
nov->summa=0;
for(i=0;i<NC;i++)// Подсчет суммарной популярности жанра
	if(strcmp(music[i].genre,slovo)==0)
		nov->summa+=music[i].popl;
if(!z) spisok=nov;// Вставка нового элемента в список 
if(z) z->sled=nov;
if(nt) 
{nt->pred=nov; nov->sled=nt;}
return;
}
void udalit(struct sp** spisok, char* slovo) {
    struct sp* nt = *spisok; 
    struct sp* pred = NULL; // пр

    while (nt != NULL && strcmp(nt->genri, slovo) != 0) {
        pred = nt;
        nt = nt->sled; 
    }
    if (nt != NULL) {
        if (pred == NULL) {
            *spisok = nt->sled; 
            if (nt->sled != NULL) {
                nt->sled->pred = NULL;
            }
        } 
        else {
            pred->sled = nt->sled; 
            if (nt->sled != NULL) {
                nt->sled->pred = pred; 
            }
        }
		free(nt);
	}
		else
		{
			if (nt == NULL) {
				printf("Элемент не нашёлся\n");
_getch();
        }
    }
}
// Функция для вывода списка всех жанров в алфавитном порядке
void alfalist(int NC,struct z* music)
{
int i, k=0;
struct sp* nt, *z = 0;
spisok=0;// Инициализация указателя на начало списка жанров

Console::ForegroundColor=ConsoleColor::DarkMagenta;
Console::BackgroundColor=ConsoleColor::Yellow;
Console::Clear();
// Создание списка жанров
if(!spisok) 
	  for(i=0;i<NC;i++)
	  {
		vstavka(NC,music,music[i].genre, i);//Вставка каждого жанра в список
		k++;
	  }

Console::Clear();
printf("\n Алфавитный список жанров");
Console::CursorLeft = 30;
printf("Обратный список");
printf("\n ==============================");
Console::CursorLeft = 30;
printf("==========================");
for(nt=spisok; nt!=0; nt=nt->sled) //Вывод в прямом порядке
	printf("\n %-20s %-15ld",nt->genri,nt->summa);
Console::CursorTop = 3;//в обратном порядке
for(nt=spisok, z=0; nt!=0; z=nt, nt=nt->sled);
for(nt=z; nt!=0; nt=nt->pred)
{
	Console::CursorLeft = 30;
	printf("%-20s %-15ld\n",nt->genri,nt->summa);
}
///////////////////
    char fam_del[50];
	printf("\n\nВведите жанр для удаления: ");
	SetConsoleCP(1251);
    scanf("%s", fam_del);
	SetConsoleCP(866);
    udalit(&spisok, fam_del);  
    Console::Clear();
    printf("\n Алфавитный список жанров");
    Console::CursorLeft = 30;
    printf("Обратный список");
    printf("\n ==============================");
    Console::CursorLeft = 30;
    printf("==========================");

    // Вывод в прямом порядке
    for (nt = spisok; nt != 0; nt = nt->sled)
        printf("\n %-20s %-15ld", nt->genri, nt->summa);

    // Вывод в обратном порядке
    Console::CursorTop = 3;
    for (nt = spisok, z = 0; nt != 0; z = nt, nt = nt->sled);
    for (nt = z; nt != 0; nt = nt->pred) {
        Console::CursorLeft = 30;
        printf("%-20s %-15ld\n", nt->genri, nt->summa);
    }
	printf("\nвы удалили свой жанр( душуУ): %-20s", fam_del);
_getch();
}
// Функция для вывода списка исполнителей с количеством альбомов больше заданного
void kolvo(int NC,struct z * music){
int i;
struct z* nt;
int albm;//Количество альбомов 
bool found = false; // Флаг для отслеживания найденных исполнителей
Console::ForegroundColor=ConsoleColor::DarkMagenta;
Console::BackgroundColor=ConsoleColor::Yellow;
Console::Clear();
  printf(" Список исполнителей по альбомам\n");
  printf(" ===================================================\n");
  printf(" Введите кол-во альбомов: ");
  scanf("%d", &albm);
  printf(" Исполнители, выпустившие больше %d альбомов:\n", albm);
  for (i = 0, nt = music; i < NC; nt++, i++) {
    if (nt->albm > albm) {
		printf("\n - %s (%s):%d", nt->name, nt->genre,nt->albm);
        found = true; // Устанавливаем флаг, если нашли исполнителя
    }
  }
  if (!found) {
        printf("\n\r Исполнителей, выпустивших больше %d альбомов, нет.\n\r", albm);
    }
  getch();
}
void diagram(int NC,struct z* music)// Функция для построения диаграммы популярности жанров
{
struct sp *nt;// Указатель на текущий элемент структуры
  int len,i,NColor; // Переменные для циклов и цвета
  long sum = 0 ; // Сумма популярности всех жанров
  char str1[20]; // Строка для хранения названия жанра
  char str2[20]; // Строка для хранения процента популярности
  System::ConsoleColor Color; // Переменная для цвета фона
Console::ForegroundColor=ConsoleColor::DarkMagenta;
Console::BackgroundColor=ConsoleColor::Yellow;
Console::Clear();
for(i=0;i<NC;i++) sum = sum+music[i].popl;  // Подсчет суммарной популярности всех жанров
if(!spisok)
	for(i=0;i<NC;i++)
		vstavka(NC,music,music[i].genre, i);
Color=ConsoleColor::Black;  // Инициализация переменных цвета
NColor=0;
for(nt=spisok,i=0; nt!=0; nt=nt->sled,i++)
	 {
	  sprintf(str1,"%s",nt->genri);// Формирование строки с названием жанра
	  sprintf(str2,"%3.1f%%",(nt->summa*100./sum));// Формирование строки с процентом популярности

	  Console::ForegroundColor=ConsoleColor::DarkMagenta;
	  Console::BackgroundColor=ConsoleColor::Yellow;
	   Console::CursorLeft=5;	    
	   Console::CursorTop=i+1;
	   printf(str1);
	   Console::CursorLeft=20;
	   printf("%s",str2);
	   Console::BackgroundColor=++Color; NColor++;
	   Console::CursorLeft=30; 
	   for(len=0; len<nt->summa*100/sum; len++) // Вывод столбца диаграммы
		   printf(" ");
	   if(NColor==14)  // Сброс цвета фона после 14 столбцов
		   {
			Color=ConsoleColor::Black; 
		    NColor=0; 
		   }
	   }
getch();
return ;
}
// Функция для вывода исполнителей с одинаковой популярностью из разных стран
void popu(int NC,struct z * music) {
int i=0, k=0, s=0, popl=0, itog=0; // Переменные для циклов и сравнения
    struct v vivod; // Структура о сравнении по популярности
    struct z best; // Структура о лучшем исполнителе
	for(k=0;k<NC;k++)
		if(s==1)
			break;
		else
		{
			strcpy(best.cntry,music[k].cntry);
			strcpy(best.name,music[k].name);
			popl=music[k].popl;

			for(i=0;i<NC;i++)
			{
				if(i!=k)
					if(popl==music[i].popl && strcmp(best.cntry,music[i].cntry)!=0)
						{
							strcpy(vivod.cntry1,best.cntry);
							strcpy(vivod.cntry2,music[i].cntry);
							strcpy(vivod.name1,best.name);
							strcpy(vivod.name2,music[i].name);
							itog=music[i].popl;
							s=1; // Установка флага, чтобы выйти из внешнего цикла
						}
			}	
				
		}
	Console::ForegroundColor=ConsoleColor::Cyan;
	Console::BackgroundColor=ConsoleColor::Black;
	Console::CursorLeft=10;
	Console::CursorTop=16;
	if(s==1)
	{
		printf("у исполнителя %s страны %s \n\n          и исполнителя %s страны %s",vivod.name1,vivod.cntry1,vivod.name2,vivod.cntry2);
			Console::CursorTop=17;
			Console::CursorLeft=55;
		printf("Популярность %d тыс. чел.",itog);
	}
	else
		printf("никого...");
	
	getch();
}

int main(array<System::String ^> ^args)
{
	int i,n;
		FILE *in;
		struct z *music;
		setlocale (LC_CTYPE,"Russian");
		Console::CursorVisible::set(false);//отключение выдимости курсора
		Console::BufferHeight=Console::WindowHeight;
		Console::BufferWidth=Console::WindowWidth;
		if((in=fopen("ispoln.dat","r"))==NULL) // Открытие файла с данными о музыке
		{
			printf("\nФайл ispoln.dat не открыт !");
     	 	_getch(); 
			exit(1);
		}
char *(dan[8])= 
{
             "Исполнитель с наибольшим количеством альбомов     ",
             "Самый непопулярный исполнитель                    ",
             "Список \"Русских\" исполнителей до 2000 года        ",
             "Алфавитный список всех жанров                     ",
             "Исполнители с количеством альбомов больше n       ",
             "Диаграмма популярности жанров                     ",
             "Исполнители с одинаковой популярн. из разных стран",
             "Выход                                             ",
};// Массив строк для меню, каждая строка - пункт меню
char BlankLine[ ]="                                                    ";
// Массив символов для пустой строки

int NC; // Объявление NC как локальную переменную
fscanf(in,"%d",&NC);// Чтение количества записей в файле
music=(struct z*)malloc(NC*sizeof(struct z));//выделение памяти
printf("\n%-19s| %-14s| %-6s| %-14s| %-14s| %s", "Исполнитель","Жанр","Год","Прослуш.(тыс.чел.)","Страна","кол-во альбомов");
printf("\n_________________________________________________________________________________________________");
for(i=0;i<NC;i++)
	fscanf(in,"%s%s%ld%d%s%d", music[i].name, music[i].genre, &music[i].year, &music[i].popl, music[i].cntry,&music[i].albm);

for(i=0;i<NC;i++)//Вывод таблицы с данными о музыке
	printf("\n%-20s %-15s %-10d %-15d %-17s %d", music[i].name, music[i].genre, music[i].year, music[i].popl, music[i].cntry,music[i].albm);

	_getch();

while(1)
	{
	Console::ForegroundColor=ConsoleColor::White;
	Console::BackgroundColor=ConsoleColor::Black;
	Console::Clear();
	Console::ForegroundColor=ConsoleColor::DarkMagenta;
	Console::BackgroundColor=ConsoleColor::White;
    Console::CursorLeft=10;
	Console::CursorTop=4;
	printf(BlankLine);

	for(i=0;i<8;i++)
			{
			Console::CursorLeft=10;
			Console::CursorTop=i+5;
			printf(" %s ",dan[i]);
			}
		Console::CursorLeft=10;
		Console::CursorTop=13;
		printf(BlankLine);
		// Выбор пункта меню пользователем
      n=menu(8, dan); 
      switch(n)  {
          case 1: maxim(NC,music); break; //ф. нахожд. исполнителя с наибольшим количеством альбомов
          case 2: minim(NC,music); break; //ф. нахожд самого непопулярного исполнителя
          case 3: listing(NC,music); break; //ф. список "Русских" исполнителей
          case 4: alfalist(NC,music); break; //ф. список жанров в алфавитном порядке
          case 5: kolvo(NC,music); break; //ф. список исполнителей по количеству альбомов
          case 6: diagram(NC,music); break; //ф. диаграмма популярности жанров
          case 7: popu(NC,music); break; //Ф. исполнители с одинаковой популярностью
          case 8: exit(0); // Завершение программы
		         }
	 }
return 0;
}


int menu(int n, char **dan)
{
int y1=0,y2=n-1;
char c=1;
while (c!=ESC)//при нажатии кнопки ESC, меню закроется
	{
	switch(c) {
		  case DOWN: y2=y1; y1++; break;// Перемещение курсора вниз
		  case UP: y2=y1; y1--; break;// Перемещение курсора вниз
		  case ENTER: return y1+1;// Возврат номера выбранного пункта
		  case HOME: y2=y1; y1=0; break;//первый вопрос.
		  case END: y2=y1; y1=n-1; break;//последний вопрос.

	               }
	// Ограничение индекса выбранного вопроса
	if(y1>n-1){y2=n-1;y1=0;}//условие,когда срабатывает кнопка DOWN
	if(y1<0) {y2=0;y1=n-1;}//условие,когда срабатывает кнопка UP
	Console::ForegroundColor=ConsoleColor::Yellow;
	Console::BackgroundColor=ConsoleColor::DarkMagenta;
	Console::CursorLeft=11;
	Console::CursorTop=y1+5;
	printf("%s",dan[y1]);
	Console::ForegroundColor=ConsoleColor::DarkMagenta;
	Console::BackgroundColor=ConsoleColor::White;
	Console::CursorLeft=11;
	Console::CursorTop=y2+5;
	printf("%s",dan[y2]);
	c=getch();
	} // конец while(c!=ESC)...
exit(0);
}

