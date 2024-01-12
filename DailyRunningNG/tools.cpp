#include <stdio.h>
#include <Windows.h>
#include "tools.h"
#include"date_tool.h"
#include<graphics.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include <stdlib.h>
#include <conio.h>

int getDelay() {
    static unsigned long long lastTime = 0;
    unsigned long long currentTime = GetTickCount();
    if (lastTime == 0) {
        lastTime = currentTime;
        return 0;
    }
    else {
        int ret = currentTime - lastTime;
        lastTime = currentTime;
        return ret;
    }
}

// ����PNGͼ��ȥ͸������
void putimagePNG(int  picture_x, int picture_y, IMAGE* picture) //xΪ����ͼƬ��X���꣬yΪY����
{
    DWORD* dst = GetImageBuffer();    // GetImageBuffer()���������ڻ�ȡ��ͼ�豸���Դ�ָ�룬EASYX�Դ�
    DWORD* draw = GetImageBuffer();
    DWORD* src = GetImageBuffer(picture); //��ȡpicture���Դ�ָ��
    int picture_width = picture->getwidth(); //��ȡpicture�Ŀ�ȣ�EASYX�Դ�
    int picture_height = picture->getheight(); //��ȡpicture�ĸ߶ȣ�EASYX�Դ�
    int graphWidth = getwidth();       //��ȡ��ͼ���Ŀ�ȣ�EASYX�Դ�
    int graphHeight = getheight();     //��ȡ��ͼ���ĸ߶ȣ�EASYX�Դ�
    int dstX = 0;    //���Դ������صĽǱ�

    // ʵ��͸����ͼ ��ʽ�� Cp=��p*FP+(1-��p)*BP �� ��Ҷ˹���������е���ɫ�ĸ��ʼ���
    for (int iy = 0; iy < picture_height; iy++)
    {
        for (int ix = 0; ix < picture_width; ix++)
        {
            int srcX = ix + iy * picture_width; //���Դ������صĽǱ�
            int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA��͸����
            int sr = ((src[srcX] & 0xff0000) >> 16); //��ȡRGB���R
            int sg = ((src[srcX] & 0xff00) >> 8);   //G
            int sb = src[srcX] & 0xff;              //B
            if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
            {
                dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; //���Դ������صĽǱ�
                int dr = ((dst[dstX] & 0xff0000) >> 16);
                int dg = ((dst[dstX] & 0xff00) >> 8);
                int db = dst[dstX] & 0xff;
                draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //��ʽ�� Cp=��p*FP+(1-��p)*BP  �� ��p=sa/255 , FP=sr , BP=dr
                    | ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //��p=sa/255 , FP=sg , BP=dg
                    | (sb * sa / 255 + db * (255 - sa) / 255);              //��p=sa/255 , FP=sb , BP=db
            }
        }
    }
}

// ������ y <0 �Լ�x<0���κ����
void putimagePNG2(int x, int y, IMAGE* picture) {
    IMAGE imgTmp;
    if (y < 0) {
        SetWorkingImage(picture);
        getimage(&imgTmp, 0, -y,
            picture->getwidth(), picture->getheight() + y);
        SetWorkingImage();
        y = 0;
        picture = &imgTmp;
    }

    if (x < 0) {
        SetWorkingImage(picture);
        getimage(&imgTmp, -x, 0, picture->getwidth() + x, picture->getheight());
        SetWorkingImage();
        x = 0;
        picture = &imgTmp;
    } 

    putimagePNG(x, y, picture);
}

// ������ y <0 �Լ�y>0���κ����
void putimagePNG2(int x, int y, int winWidth, IMAGE* picture) {
    IMAGE imgTmp;
    if (y < 0) {
        SetWorkingImage(picture);
        getimage(&imgTmp, 0, -y,
            picture->getwidth(), picture->getheight() + y);
        SetWorkingImage();
        y = 0;
        picture = &imgTmp;
    }

    if (x < 0) {
        SetWorkingImage(picture);
        getimage(&imgTmp, -x, 0, picture->getwidth() + x, picture->getheight());
        SetWorkingImage();
        x = 0;
        picture = &imgTmp;
    }
    else if (x >= winWidth) {
        return;
    }
    else if (x > winWidth-picture->getwidth()) {
        SetWorkingImage(picture);
        getimage(&imgTmp, 0, 0, winWidth - x, picture->getheight());
        SetWorkingImage();
        picture = &imgTmp;
    }

    putimagePNG(x, y, picture);
}

//��A[x01,y01,x02,y02]  B[x11,y11,x12,y12].
bool rectIntersect(int x01, int y01, int x02, int y02,
    int x11, int y11, int x12, int y12)
{
    int zx = abs(x01 + x02 - x11 - x12);
    int x = abs(x01 - x02) + abs(x11 - x12);
    int zy = abs(y01 + y02 - y11 - y12);
    int y = abs(y01 - y02) + abs(y11 - y12);
    return  (zx <= x && zy <= y);
}

void drawBloodBar(int x, int y, int width, int height, int lineWidth, int boardColor, int emptyColor, int fillColor, float percent) {
    LINESTYLE lineStyle;
    getlinestyle(&lineStyle);
    int lineColor = getlinecolor();
    int fileColor = getfillcolor();

    if (percent < 0) {
        percent = 0;
    }

    setlinecolor(BLUE);
    setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, lineWidth);
    setfillcolor(emptyColor);
    fillrectangle(x, y, x + width, y + height);
    setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 0);
    setfillcolor(fillColor);
    setlinecolor(fillColor);
    if (percent > 0) {
        fillrectangle(x + 0.5 * lineWidth, y + lineWidth * 0.5, x + width * percent, y + height - 0.5 * lineWidth);
    }

    setlinecolor(lineColor);
    setfillcolor(fillColor);
    setlinestyle(&lineStyle);
}

//��ȡ��߷�
int ReadGrade()
{
    FILE* file = fopen("MaxScoreHistory.dat", "r"); //��ֻ���ķ�ʽ���ļ�
    if (file == NULL) //���ļ�ʧ��
    {
        file = fopen("MaxScoreHistory.dat", "w"); //��ֻд�ķ�ʽ���ļ�
        fprintf(file, "0 "); //����ʼ�÷�д���ļ�
        fclose(file);
        return 0; // ���س�ʼ�÷�
    }
    int max;
    if (fscanf(file, "%d", &max) != 1) //��ȡ�ļ��е���ߵ÷ֵ�max
    {
        fclose(file);
        return 0;
    }
    fclose(file);
    return max;
}

//д����߷�
void WriteGrade(int grade)
{
    FILE* file = fopen("MaxScoreHistory.dat", "w"); //��ֻд�ķ�ʽ���ļ�
    if (file == NULL) //���ļ�ʧ��
    {
        printf("Fail to Write\n");
        return;
    }
    fprintf(file, "%d ", grade); //��������Ϸ�÷�д���ļ�
    fclose(file);
}

//��ȡ��ǰʱ��
long long ReadPresentTime()
{
    int year, month, day, hour, minute, second;
    char dateStr[50] = { 0 }; // ���ڴ洢ũ�������ַ���
    // ��ȡ��ǰ����������
    getDate(&year, &month, &day, dateStr);
    // ��ȡ��ǰʱ��
    getTime(&hour, &minute, &second);

    // ���㵱ǰʱ���ʱ��������Ը�����Ҫ������ʽ������ٶ�ΪYYYYMMDDHHMMSS��ʽ
    long long datetime = year * 10000000000LL + month * 100000000LL +
        day * 1000000LL + hour * 10000LL + minute * 100LL + second;

    return datetime;
}

//��ȡ��־������
long long ReadLastDate()
{
    int year, month, day, hour, minute, second;
    char dateStr[50] = { 0 }; // ���ڴ洢ũ�������ַ���
    // ��ȡ��ǰ����������
    getDate(&year, &month, &day, dateStr);
    // ��ȡ��ǰʱ��
    getTime(&hour, &minute, &second);

    FILE* file = fopen("DateList.dat", "r"); // ��ֻ���ķ�ʽ���ļ�
    if (file == NULL) // ����ļ�������
    {
        file = fopen("DateList.dat", "w"); // ��ֻд�ķ�ʽ���������ļ�
        if (file != NULL) // ȷ���ļ��ɹ���
        {
            // ���㵱ǰʱ���ʱ��������Ը�����Ҫ������ʽ������ٶ�ΪYYYYMMDDHHMMSS��ʽ
            long long datetime = year * 10000000000LL + month * 100000000LL +
                day * 1000000LL + hour * 10000LL + minute * 100LL + second;

            fprintf(file, "%lld", datetime); // ����ǰ���ں�ʱ����������ʽд���ļ�
            fclose(file);
            return datetime; // ����д������ں�ʱ��
        }
        return 0; // �޷����ļ�ʱ����0
    }

    // ����ļ����ڣ���ȡ�ļ�����
    long long LastDateReturn;
    if (fscanf(file, "%lld", &LastDateReturn) == 1) // ��ȡ�ļ��е����ں�ʱ��
    {
        fclose(file);
        return LastDateReturn; // �����ļ��е����ں�ʱ��
    }

    // ������ļ��ж�ȡʧ��
    fclose(file);
    return 0;
}

bool ifDateOverSevenDays(long long CurrentDate, long long LastDate)
{
    //����ܼ�
    long long currentYear, currentMonth, currentDay, currentHour, currentMinute, currentSecond;
    long long lastYear, lastMonth, lastDay, lastHour, lastMinute, lastSecond;
    //��ȡ��
    currentYear = CurrentDate / 10000000000LL;
    lastYear = LastDate / 10000000000LL;
    //��ȡ��
    currentMonth = (CurrentDate / 100000000LL) % 100LL;
    lastMonth = (LastDate / 100000000LL) % 100LL;

    //�����¾Ϳ����ж�һ����
    if (currentYear > lastYear && (lastMonth != 12 || currentMonth != 1)) //����12�ºͲ���1��ֻҪ����һ����һ��������ǰһ��׺ͺ�һ����Ĺ�ϵ
    {
        return true;
    }

    if (currentYear > lastYear) //��Ȼ����ͬ�꣬�Ǿ���˵��һ��if����Ǻ��治���㣬����˵�����ܴ���ǰһ��׺ͺ�һ����Ĺ�ϵ
    {
        if (currentMonth == 1 && lastMonth == 12) //ȷʵ��ǰһ��׺ͺ�һ����Ĺ�ϵ���Ǿ���ȡ����
        {
            currentDay = (CurrentDate / 1000000LL) % 100LL;
            lastDay = (LastDate / 1000000LL) % 100LL;
            if (currentDay < 7 && lastDay>24) //����7����ǰ������һ��24���Ժ󣬶���7��ķֽ��ߣ����ܻ��������
            {
                if (lastDay > 24 + currentDay)//�������죬����Ҫ�����Դ
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
            else //����Ͳ�Ӱ����
            {
                return true;
            }
        }
        else//��Ҳ�Ͳ�Ӱ����
        {
            return true;
        }
    }
    //ͬһ�꣬�������ж��˺ܶ�
    if (currentYear == lastYear)
    {
        //���ڵ��·ݱ���һ�λ��磬˵�����ݳ�������
        if (currentMonth < lastMonth)
        {
            MessageBoxA(nullptr, "���ݼ��ش�������ϵ����Ա�����Դ��ȷ�ԣ������룺0x1111Dating_Month", "����", MB_OK);
            exit(-1);
        }

        if (currentMonth - lastMonth > 1) //������������£��϶�����������
        {
            return true;
        }
        else if (currentMonth - lastMonth == 1) // ��һ����
        {
            // ��ȡ�£��죬ʱ���֣��룬ȫ����������ֱ���ж�
            currentDay = (CurrentDate / 1000000LL) % 100LL;
            lastDay = (LastDate / 1000000LL) % 100LL;
            currentHour = (CurrentDate / 10000LL) % 100LL;
            lastHour = (LastDate / 10000LL) % 100LL;
            currentMinute = (CurrentDate / 100LL) % 100LL;
            lastMinute = (LastDate / 100LL) % 100LL;
            currentSecond = CurrentDate % 100LL;
            lastSecond = LastDate % 100LL;

            // ת��Ϊͳһ��ʱ�䵥λ���룩�����бȽϣ�����ٶ�ÿ������30��
            long long currentTimeInSeconds = currentDay * 24 * 3600 + currentHour * 3600 +
                currentMinute * 60 + currentSecond;
            // �ϸ��µ�������30������ټ��ϵ�ǰ�������ɵó�ǰ�������ڡ����������Ĳ�
            long long lastTimeInSeconds = (30 - lastDay) * 24 * 3600 + lastHour * 3600 +
                lastMinute * 60 + lastSecond;

            // ��������������7�����ϣ�7�� == 604800�룩
            if (currentTimeInSeconds + lastTimeInSeconds >= 604800) 
            {
                return true; // �ѳ���7��
            }
            else 
            {
                return false; // ����7��
            }
        }
        else // ͬһ����
        {
            // ֱ����ȡ�����Ƚ�
            currentDay = (CurrentDate / 1000000LL) % 100LL;
            lastDay = (LastDate / 1000000LL) % 100LL;

            return currentDay - lastDay > 7;
        }
    
    }
    //���ڱ���һ�λ�Ҫ�磬˵�����ݳ������ˣ�ֱ�ӷ���bug
    if (currentYear < lastYear)
    {
        MessageBoxA(nullptr, "���ݼ��ش�������ϵ����Ա�����Դ��ȷ�ԣ������룺0x1111Dating_Year", "����", MB_OK);
        exit(-1);
    }
}