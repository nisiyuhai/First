﻿// WordCount.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>


void GetLine(FILE* f)
{
    int letter = 0, line = 0;
    //接收字符，行数
    int mark = 0;
    while (!feof(f))
    {
        letter = fgetc(f);
        if (letter == '\n')
            line++;
    }
    printf("共有行数 %d\n\n", line);
}

void GetWord(FILE* f)
{
    int letter = 0, word = 0;
    //接收字符，词数
    int mark = 0;
    //0为非字符，1为字符串
    while (!feof(f))
    {
        letter = fgetc(f);
        if ((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z') || letter == '_')
        {
            if (mark == 0)
                mark = 1;
        }
        else if (letter == '.') //防止头文件和浮点数的'.'被误判
        {
            if (mark == 0 || mark == 1)
                continue;
        }
        else
        {
            if (mark == 1)
            {
                word++;
                mark = 0;
            }
            else
                continue;
        }
    }
    printf("共有词数 %d\n\n", word);
}

void MoreData(FILE* f)
{
    int letter = 0, code = 0, empty = 0, note = 0, mark = 0, notemark = 0;
    /*
    接受字符，代码行数，空行数，注释行数，类型标记，注释行标记
    mark的参数：0空行，1存在格式化字符的空行，2代码行，3注释行
    notemark参数：0未判定，1单行注释，2多行注释
    */
    while (!feof(f))
    {
        letter = fgetc(f);
        if (mark == 0 || mark == 1) //判定空行
        {
            if (letter == ' ')
                continue;
            else if (letter == '\n')
            {
                empty++;
                mark = 0;
                continue;
            }
            else if ((letter == '{' || letter == '}') && mark == 0)
            {
                mark = 1;
                continue;
            }


            else
            {
                if (letter == '/')
                    mark = 3;
                else
                    mark = 2;
            }
        }

        if (mark == 2) //判定代码行
        {
            while (!feof(f) && letter != '\n')
                letter = fgetc(f);
            mark = 0;
            code++;
            continue;
        }

        if (mark == 3) //判定注释行
        {
            while (!feof(f))
            {
                letter = fgetc(f);

                if (letter == '/' && notemark == 0) //单行注释起始判定
                    notemark = 1;
                else if (letter == '*' && notemark == 0) //多行注释起始判定
                    notemark = 2;
                else if (letter == '*' && notemark == 2) //多行注释结束判定
                    notemark = 3;

                if (notemark == 1) //单行注释结束判定
                {
                    while (!feof(f) && letter != '\n')
                        letter = fgetc(f);
                    note++;
                    mark = 0;
                    notemark = 0;
                    break;
                }

                if (notemark == 2)  //多行注释中间行数计算
                {
                    while (!feof(f) && letter != '\n')
                        letter = fgetc(f);
                    note++;
                }

                if (notemark == 3 && letter == '/') //多行注释结束判定
                {
                    while (!feof(f) && letter != '\n')
                        letter = fgetc(f);
                    note++;
                    mark = 0;
                    notemark = 0;
                    break;
                }
                else if (notemark == 3 && letter != '*') //判定是否为注释中的*
                    notemark = 2;
            }
        }
    }
    printf("共有空行数 %d  代码行数 %d  注释行数 %d\n", empty, code - 1, note); //除去eof行
}

void GetLetter(FILE* f)
{
    int letter = 0, num = 0;
    //接收字符，字符数
    while (!feof(f))
    {
        letter = fgetc(f);
        if (letter == '\n') //除换行标记外均视为字符
            continue;
        num++;
    }
    printf("共有字符数 %d\n\n", num - 1);  //除去eof标志
}

int main(int argc, char* argv[])
{
    int choose = 0;
    char com; //获取具体命令
    FILE* fp = NULL;
    char* func = (char*)malloc(sizeof(char) * 50); //接收命令参数
    if (argv[2] != NULL && argv[1] != NULL)
    {
        fp = fopen(argv[2], "r");
        func = argv[1];
        com = func[1];
        if (fp == NULL)
        {
            printf("文件打开失败。\n");
            exit(1);
        }
    }
    else
    {
        printf("参数输入错误。\n");
        return 0;
    }
    if (com == 'c')
        choose = 1;
    else if (com == 'w')
        choose = 2;
    else if (com == 'l')
        choose = 3;
    else
        choose = 101;
    while (1)
    {
        switch (choose)
        {
        case 1:
            GetLetter(fp);
            rewind(fp);
            break;
        case 2:
            GetWord(fp);
            rewind(fp);
            break;
        case 3:
            GetLine(fp);
            rewind(fp);
            break;
        case 4:
            MoreData(fp);
            rewind(fp);
            break;
        case 100:
            fclose(fp);
            free(func);
            exit(0);
        case 101:
            printf("错误输入\n\n");
            while (getchar() != '\n') //清空所有输入
                continue;
            break;
        }
        printf("输入：q 退出\t w 获取词数\t l 获取行数\t c 获取字符数\t a 获取更多信息\t");
        scanf("%c", &com);
        getchar();
        if (com == 'c')
            choose = 1;
        else if (com == 'w')
            choose = 2;
        else if (com == 'l')
            choose = 3;
        else if (com == 'a')
            choose = 4;
        else if (com == 'q')
            choose = 100;
        else
            choose = 101;
    }
    //test();
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
