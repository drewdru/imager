#ifndef PLUGININFO_H
#define PLUGININFO_H

#include <QObject>

class PluginInfo
{
    QString pluginPath;
    QString arguments;
    QString authorInfo;
    float version;
public:
    PluginInfo();
    PluginInfo(QString pluginPath, QString arguments, QString authorInfo, float version);
    QString getPluginPath();
    QString getArguments();
    QString getAuthorInfo();
    float getVersion();

//    friend ostream &operator<<(ostream &stream, Trigon obj);//потоковый вывод экземпляра
//    friend istream &operator>>(istream &stream, Trigon &obj);//потоковый ввод экземпляра
};
//#include <iostream>
//#include <iomanip>
//#include <fstream>//потоковая работа с файлами
//#include <time.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <locale.h>
//#include <math.h>
//ostream &operator<<(ostream &stream, Trigon obj)
//{
//	for (size_t i =0; i<obj.NumOfAngles();i++)
//		stream<<"("<<obj.polygon[i].x<<","<<obj.polygon[i].y<<")\n";
//	return stream;
//}
////потоковый ввод экземпляра
//istream &operator>>(istream &stream, Trigon &obj)
//{
//	for (size_t i =0; i<obj.NumOfAngles();i++)
//	{
//		Point_in_4 m;
//		try
//		{
//			cout<<"Введите координаты (x y): ";
//			stream>>obj.polygon[i].x>>obj.polygon[i].y;
//			if((obj.polygon[i].x>0)&&(obj.polygon[i].y<0))
//				throw m;
//		}
//		catch(Point_in_4 m){m.debug_print(); i--;}
//		catch(...){cerr<<"Произошла не предвиденная ошибка\n"; i--;}
//	}
//	return stream;
//}

#endif // PLUGININFO_H
