#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
int main(int argc, char *argv[])//MethodName,dllPath
{

	char* arguments[4];
	if(argc<3)
	{
		arguments[0] = argv[0];
		arguments[1] = "_";
		arguments[2] = "./test.dll";
		arguments[3] = "_";
		argc = 4;
	}
	else
	{
		arguments[0] = argv[0];
		arguments[1] = argv[1];
		arguments[2] = argv[2];
		arguments[3] = argv[3];
		argc = 4;
		int i = 0;
		printf("Input: ");
		while (i<5) {
			char str[256];
			fflush(stdout);
			scanf("%s", str);

			printf(" %s ", str);
			++i;
		}
	}

	QApplication a(argc, arguments);

    MainWindow w;
	w.show();
    return a.exec();
}
