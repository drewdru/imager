#include "hue.h"


#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
extern "C" PM_WIDGET_EXPORT void PluginInfo(QJsonObject &json)
{
    QJsonObject argument;
    argument["type"] = "slider";
    argument["name"] = "hue";
    argument["min"] = "0";
    argument["max"] = "360";
    QJsonArray argumentList;
    argumentList.append(argument);

    json["arguments"] = argumentList;
    json["pluginName"] = "Hue";
    json["authorInfo"] = "Author: Ovsyannikov A.K. (drew.dru)";
    json["version"] = PLUGIN_VERSION;
}

extern "C" PM_WIDGET_EXPORT ProcessImageInterface *createObject()
{
    return new Hue();
}

Hue::Hue()
{

}

int Hue::Process(uchar* bitmap, size_t n, QVector<QString> argv)
{
    if(!bitmap)
        return 1;
    if(argv.size() < 1)
        return 2;
    hue(bitmap, n, argv.at(0).toInt());
    return 0;
}

void Hue::hue(uchar* bitmap, size_t n, int newH)
{
    double* temp = new double[n];
    RGB_TO_HSL(bitmap, temp, n);
    for (size_t i = 1; i < n-1; i += 4)
    {
        temp[i + 1] = newH;
    }
    HSL_TO_RGB(bitmap, temp, n);
    delete[] temp;
}

void Hue::RGB_TO_HSL(uchar* bitmap, double* newBitmap, size_t n)
{
    double H = 0, S = 0, L = 0;

    for (size_t i = 1; i < n-1; i += 4)
    {
        double R = bitmap[i + 1] / 255.0; //
        double G = bitmap[i] / 255.0; 
        double B = bitmap[i - 1] / 255.0; //

        double temp = G >= B ? G : B;
        double Max = R >= temp ? R : temp;

        temp = G <= B ? G : B;
        double Min = R <= temp ? R : temp;

        //Get hue
        if (Max == Min)
        {
            H = 0;
        }
        else if (Max == R && G >= B)
        {
            H = 60.0 * (G - B) / (Max - Min);
        }
        else if (Max == R && G < B)
        {
            H = 60.0 * (G - B) / (Max - Min) + 360.0;
        }
        else if (Max == G)
        {
            H = 60.0 * (B - R) / (Max - Min) + 120.0;
        }
        else if (Max == B)
        {
            H = 60.0 * (R - G) / (Max - Min) + 240.0;
        }

        //Gel lightness
        L = (Max + Min) / 2.0;

        //Get saturation
        if (L == 0 || Max == Min)
        {
            S = 0;
        }
        else if (0 < L && L <= 0.5)
        {
            S = (Max - Min) / (Max + Min);
        }
        else if (L > 0.5)
        {
            S = (Max - Min) / (2 - (Max + Min));
        }
        newBitmap[i + 1] = H;
        newBitmap[i] = S;
        newBitmap[i - 1] = L;
    }
}

void Hue::HSL_TO_RGB(uchar* bitmap, double* newBitmap, size_t n)
{
    int R, G, B;

    for (size_t i = 1; i < n-1; i += 4)
    {
        if (newBitmap[i] == 0)
        {
            R = (int)round(newBitmap[i - 1] * 255.0); //
            G = (int)round(newBitmap[i - 1] * 255.0); //
            B = (int)round(newBitmap[i - 1] * 255.0); //
        }
        else
        {
            double Q = (newBitmap[i - 1] < 0.5) ? \
                        (newBitmap[i - 1] * (1.0 + newBitmap[i])) : \
                        (newBitmap[i - 1] + newBitmap[i] - (newBitmap[i - 1] * newBitmap[i]));
            double P = (2.0 * newBitmap[i - 1]) - Q;

            double HK = newBitmap[i + 1] / 360.0;
            double* T = new double[3];   //Temp R,G,B

            T[0] = HK + (1.0 / 3.0);   // R
            T[1] = HK;         // G
            T[2] = HK - (1.0 / 3.0);   // B

            for (int i = 0; i < 3; i++)
            {
                if (T[i] < 0) T[i] += 1.0;
                if (T[i] > 1) T[i] -= 1.0;

                if ((T[i] * 6) < 1)
                {
                    T[i] = P + ((Q - P) * 6.0 * T[i]);
                }
                else if ((T[i] * 2.0) < 1)
                {
                    T[i] = Q;
                }
                else if ((T[i] * 3.0) < 2)
                {
                    T[i] = P + (Q - P) * ((2.0 / 3.0) - T[i]) * 6.0;
                }
                else
                {
                    T[i] = P;
                }
            }

            bitmap[i + 1] = (uchar)(T[0] * 255.0);
            bitmap[i] = (uchar)(T[1] * 255.0);
            bitmap[i - 1] = (uchar)(T[2] * 255.0);
            delete[] T;
        }
    }
}
