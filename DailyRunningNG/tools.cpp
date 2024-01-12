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

// 载入PNG图并去透明部分
void putimagePNG(int  picture_x, int picture_y, IMAGE* picture) //x为载入图片的X坐标，y为Y坐标
{
    DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
    DWORD* draw = GetImageBuffer();
    DWORD* src = GetImageBuffer(picture); //获取picture的显存指针
    int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
    int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
    int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
    int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
    int dstX = 0;    //在显存里像素的角标

    // 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
    for (int iy = 0; iy < picture_height; iy++)
    {
        for (int ix = 0; ix < picture_width; ix++)
        {
            int srcX = ix + iy * picture_width; //在显存里像素的角标
            int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
            int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
            int sg = ((src[srcX] & 0xff00) >> 8);   //G
            int sb = src[srcX] & 0xff;              //B
            if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
            {
                dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; //在显存里像素的角标
                int dr = ((dst[dstX] & 0xff0000) >> 16);
                int dg = ((dst[dstX] & 0xff00) >> 8);
                int db = dst[dstX] & 0xff;
                draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
                    | ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
                    | (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
            }
        }
    }
}

// 适用于 y <0 以及x<0的任何情况
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

// 适用于 y <0 以及y>0的任何情况
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

//设A[x01,y01,x02,y02]  B[x11,y11,x12,y12].
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

//读取最高分
int ReadGrade()
{
    FILE* file = fopen("MaxScoreHistory.dat", "r"); //以只读的方式打开文件
    if (file == NULL) //打开文件失败
    {
        file = fopen("MaxScoreHistory.dat", "w"); //以只写的方式打开文件
        fprintf(file, "0 "); //将初始得分写入文件
        fclose(file);
        return 0; // 返回初始得分
    }
    int max;
    if (fscanf(file, "%d", &max) != 1) //读取文件中的最高得分到max
    {
        fclose(file);
        return 0;
    }
    fclose(file);
    return max;
}

//写入最高分
void WriteGrade(int grade)
{
    FILE* file = fopen("MaxScoreHistory.dat", "w"); //以只写的方式打开文件
    if (file == NULL) //打开文件失败
    {
        printf("Fail to Write\n");
        return;
    }
    fprintf(file, "%d ", grade); //将本局游戏得分写入文件
    fclose(file);
}

//获取当前时间
long long ReadPresentTime()
{
    int year, month, day, hour, minute, second;
    char dateStr[50] = { 0 }; // 用于存储农历日期字符串
    // 获取当前公历年月日
    getDate(&year, &month, &day, dateStr);
    // 获取当前时间
    getTime(&hour, &minute, &second);

    // 计算当前时间的时间戳，可以根据需要调整格式，这里假定为YYYYMMDDHHMMSS形式
    long long datetime = year * 10000000000LL + month * 100000000LL +
        day * 1000000LL + hour * 10000LL + minute * 100LL + second;

    return datetime;
}

//读取日志的日期
long long ReadLastDate()
{
    int year, month, day, hour, minute, second;
    char dateStr[50] = { 0 }; // 用于存储农历日期字符串
    // 获取当前公历年月日
    getDate(&year, &month, &day, dateStr);
    // 获取当前时间
    getTime(&hour, &minute, &second);

    FILE* file = fopen("DateList.dat", "r"); // 以只读的方式打开文件
    if (file == NULL) // 如果文件不存在
    {
        file = fopen("DateList.dat", "w"); // 以只写的方式创建并打开文件
        if (file != NULL) // 确保文件成功打开
        {
            // 计算当前时间的时间戳，可以根据需要调整格式，这里假定为YYYYMMDDHHMMSS形式
            long long datetime = year * 10000000000LL + month * 100000000LL +
                day * 1000000LL + hour * 10000LL + minute * 100LL + second;

            fprintf(file, "%lld", datetime); // 将当前日期和时间以整数形式写入文件
            fclose(file);
            return datetime; // 返回写入的日期和时间
        }
        return 0; // 无法打开文件时返回0
    }

    // 如果文件存在，读取文件内容
    long long LastDateReturn;
    if (fscanf(file, "%lld", &LastDateReturn) == 1) // 读取文件中的日期和时间
    {
        fclose(file);
        return LastDateReturn; // 返回文件中的日期和时间
    }

    // 如果从文件中读取失败
    fclose(file);
    return 0;
}

bool ifDateOverSevenDays(long long CurrentDate, long long LastDate)
{
    //定义很简单
    long long currentYear, currentMonth, currentDay, currentHour, currentMinute, currentSecond;
    long long lastYear, lastMonth, lastDay, lastHour, lastMinute, lastSecond;
    //提取年
    currentYear = CurrentDate / 10000000000LL;
    lastYear = LastDate / 10000000000LL;
    //提取月
    currentMonth = (CurrentDate / 100000000LL) % 100LL;
    lastMonth = (LastDate / 100000000LL) % 100LL;

    //有年月就可以判定一次了
    if (currentYear > lastYear && (lastMonth != 12 || currentMonth != 1)) //不是12月和不是1月只要满足一个就一定不会是前一年底和后一年初的关系
    {
        return true;
    }

    if (currentYear > lastYear) //依然不是同年，那就是说上一个if语句是后面不满足，即是说，可能存在前一年底和后一年初的关系
    {
        if (currentMonth == 1 && lastMonth == 12) //确实是前一年底和后一年初的关系，那就提取天数
        {
            currentDay = (CurrentDate / 1000000LL) % 100LL;
            lastDay = (LastDate / 1000000LL) % 100LL;
            if (currentDay < 7 && lastDay>24) //新月7号以前或者上一月24号以后，都是7天的分界线，可能会低于七天
            {
                if (lastDay > 24 + currentDay)//低于七天，不需要检测资源
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
            else //否则就不影响啦
            {
                return true;
            }
        }
        else//那也就不影响啦
        {
            return true;
        }
    }
    //同一年，反而好判断了很多
    if (currentYear == lastYear)
    {
        //现在的月份比上一次还早，说明数据出问题了
        if (currentMonth < lastMonth)
        {
            MessageBoxA(nullptr, "数据加载错误！请联系管理员检查资源正确性！错误码：0x1111Dating_Month", "警告", MB_OK);
            exit(-1);
        }

        if (currentMonth - lastMonth > 1) //相隔最少两个月，肯定超过七天了
        {
            return true;
        }
        else if (currentMonth - lastMonth == 1) // 多一个月
        {
            // 获取月，天，时，分，秒，全部加起来，直接判断
            currentDay = (CurrentDate / 1000000LL) % 100LL;
            lastDay = (LastDate / 1000000LL) % 100LL;
            currentHour = (CurrentDate / 10000LL) % 100LL;
            lastHour = (LastDate / 10000LL) % 100LL;
            currentMinute = (CurrentDate / 100LL) % 100LL;
            lastMinute = (LastDate / 100LL) % 100LL;
            currentSecond = CurrentDate % 100LL;
            lastSecond = LastDate % 100LL;

            // 转换为统一的时间单位（秒）来进行比较，这里假定每个月有30天
            long long currentTimeInSeconds = currentDay * 24 * 3600 + currentHour * 3600 +
                currentMinute * 60 + currentSecond;
            // 上个月的天数与30天相减再加上当前天数即可得出前后两日期“总天数”的差
            long long lastTimeInSeconds = (30 - lastDay) * 24 * 3600 + lastHour * 3600 +
                lastMinute * 60 + lastSecond;

            // 检查两个日期相差7天以上（7天 == 604800秒）
            if (currentTimeInSeconds + lastTimeInSeconds >= 604800) 
            {
                return true; // 已超过7天
            }
            else 
            {
                return false; // 不足7天
            }
        }
        else // 同一个月
        {
            // 直接提取天数比较
            currentDay = (CurrentDate / 1000000LL) % 100LL;
            lastDay = (LastDate / 1000000LL) % 100LL;

            return currentDay - lastDay > 7;
        }
    
    }
    //现在比上一次还要早，说明数据出问题了，直接返回bug
    if (currentYear < lastYear)
    {
        MessageBoxA(nullptr, "数据加载错误！请联系管理员检查资源正确性！错误码：0x1111Dating_Year", "警告", MB_OK);
        exit(-1);
    }
}