#ifndef PLUGINERROR_H
#define PLUGINERROR_H

class PluginError
{
public:
    PluginError();
};

#endif // PLUGINERROR_H

//#include <iostream>
//#include <iomanip>
//#include <fstream>//потоковая работа с файлами
//#include <time.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <locale.h>
//#include <math.h>

//#define MAX_val 1000

//using namespace std;

//class ERR_fig
//{
//protected:
//	char* err_f;
//public:
//	ERR_fig(){err_f = "errors_log.txt\0";}
//	virtual void debug_print() const
//	{
//		cerr<<"Ошибка!\n";
//		ofstream txtout(err_f, ios::out|ios::app);//открытие файла для считывания
//		txtout<<"Ошибка!\n";
//		txtout.close();//закрытие файла
//	}
//};

//class Point_in_line:public ERR_fig
//{
//public:
//	virtual void debug_print() const
//	{
//		cerr<<"Ошибка! Слишком много углов в одну линию\n";
//		ofstream txtout(err_f, ios::out|ios::app);//открытие файла для считывания
//		txtout<<"Ошибка! Слишком много углов в одну линию\n";
//		txtout.close();//закрытие файла
//	}
//};

//class Point_in_Point:public ERR_fig
//{
//public:
//	virtual void debug_print() const
//	{
//		cerr<<"Ошибка! Совпадение координат углов\n";
//		ofstream txtout(err_f, ios::out|ios::app);//открытие файла для считывания
//		txtout<<"Ошибка! Совпадение координат углов\n";
//		txtout.close();//закрытие файла
//	}
//};

//class Point_in_4:public ERR_fig
//{
//public:
//	virtual void debug_print() const
//	{
//		cerr<<"Ошибка! Координаты точки не должны находиться в 4-й четверти\n";
//		ofstream txtout(err_f, ios::out|ios::app);//открытие файла для считывания
//		txtout<<"Ошибка! Координаты точки не должны находиться в 4-й четверти\n";
//		txtout.close();//закрытие файла
//	}
//};

//class MaxFigPer:public ERR_fig
//{
//public:
//	virtual void debug_print() const
//	{
//		cerr<<"Ошибка! Периметр превышает максимльное значение: "<<MAX_val<<endl;
//		ofstream txtout(err_f, ios::out|ios::app);//открытие файла для считывания
//		txtout<<"Ошибка! Периметр превышает максимльное значение: "<<MAX_val<<endl;
//		txtout.close();//закрытие файла
//	}
//};
