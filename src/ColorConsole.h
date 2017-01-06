//
// Copyright (c) zww0815@foxmail.com.  All rights reserved.
//
//
#ifndef _COLOR_CONSOLE_H_
#define _COLOR_CONSOLE_H_

#include <strstream>
#include <iostream>

enum ConsoleColorType
{
    black = 0,
    dark_blue = 1,
    dark_green = 2,
    dark_aqua, dark_cyan = 3,
    dark_red = 4,
    dark_purple = 5, dark_pink = 5, dark_magenta = 5,
    dark_yellow = 6,
    dark_white = 7,
    gray = 8,
    blue = 9,
    green = 10,
    aqua = 11, cyan = 11,
    red = 12,
    purple = 13, pink = 13, magenta = 13,
    yellow = 14,
    white = 15
};

static HANDLE g_stdConOut;//Standard Output Handle
static bool g_colorProtect = false;//if g_colorProtect is true, background and text colors will never be the same

static ConsoleColorType g_textCol;   //current text color
static ConsoleColorType g_backCol;   //current back color
static ConsoleColorType g_defTextCol;//original text color
static ConsoleColorType g_defBackCol;//original back color

inline void UpdateColors()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(g_stdConOut, &csbi);
    g_textCol = ConsoleColorType(csbi.wAttributes & 15);
    g_backCol = ConsoleColorType((csbi.wAttributes & 0xf0) >> 4);
}

inline void SetColor(ConsoleColorType textcolor, ConsoleColorType backcolor)
{
    if (g_colorProtect && textcolor == backcolor)
    {
        return;
    }

    g_textCol = textcolor;
    g_backCol = backcolor;
    unsigned short wAttributes = ((unsigned int)g_backCol << 4) | (unsigned int)g_textCol;
    SetConsoleTextAttribute(g_stdConOut, wAttributes);
}

inline void SetTextColor(ConsoleColorType textColor)
{
    if (g_colorProtect && textColor == g_backCol)
    {
        return;
    }

    g_textCol = textColor;
    unsigned short wAttributes = ((unsigned int)g_backCol << 4) | (unsigned int)g_textCol;
    SetConsoleTextAttribute(g_stdConOut, wAttributes);
}

inline void SetBackColor(ConsoleColorType backColor)
{
    if (g_colorProtect && g_textCol == backColor)
    {
        return;
    }

    g_backCol = backColor;
    unsigned short wAttributes = ((unsigned int)g_backCol << 4) | (unsigned int)g_textCol;
    SetConsoleTextAttribute(g_stdConOut, wAttributes);
}

inline void ConsoleColorInit()
{
    g_stdConOut = GetStdHandle(STD_OUTPUT_HANDLE);
    UpdateColors();
    g_defTextCol = white;//g_textCol; 
    g_defBackCol = black;//g_backCol;
}

template<class elem, class traits>
inline std::basic_ostream<elem, traits>& operator<<(std::basic_ostream<elem, traits>& os, ConsoleColorType col)
{
    os.flush();
    SetTextColor(col);
    return os;
}

template<class elem, class traits>
inline std::basic_istream<elem, traits>& operator >> (std::basic_istream<elem, traits>& is, ConsoleColorType col)
{
    std::basic_ostream<elem, traits>* p = is.tie();
    if (p != NULL)
    {
        p->flush();
    }

    SetTextColor(col);
    return is;
}

class ColorConsole
{
public:
    ColorConsole()
        :ScreenBufferHeigh(999)
        , ScreenBufferWidth(100)
    {
    }

    ~ColorConsole()
    {
    }

    void ShowLastSystemError();

    SHORT GetDefaultScreenBufHeigh() { return ScreenBufferHeigh; };
    SHORT GetDefaultScreenBufWidth() { return ScreenBufferWidth; };

    void Initialize()
    {
        std::strstream ssException;
        ConsoleColorInit();

        HWND console = GetConsoleWindow();
        GetWindowRect(console, &ConsoleRect); //stores the console's current dimensions

        try
        {
            HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
            if (!h)
            {
                //return directly if called in non-console executable process.
                return;
            }
            COORD largestSize = GetLargestConsoleWindowSize(h);
            const SHORT width = (largestSize.X - 40) > 100 ? largestSize.X - 40 : 100;
            const SHORT Height = (largestSize.Y - 10) > 25 ? largestSize.Y - 10 : 25;
            SetWindowSize(width, Height);//max X=232 Y=66
        }
        catch (std::logic_error& ex)
        {
            ssException << ex.what() << '\n';
            std::cout << ssException.str();
        }
        catch (std::exception& ex)
        {
            ssException << ex.what() << "\nSystem error: ";
            std::cout << ssException.str();
            ShowLastSystemError();
        }
    }

    void SetWindowSize(SHORT x, SHORT y)
    {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

        if (h == INVALID_HANDLE_VALUE)
            throw std::runtime_error("Unable to get stdout handle.");

        // If either dimension is greater than the largest console window we can have,
        // there is no point in attempting the change.
        {
            COORD largestSize = GetLargestConsoleWindowSize(h);
            //std::cout << "x: " << largestSize.X << "y: " << largestSize.Y << std::endl;
            if (x > largestSize.X)
                throw std::invalid_argument("The x dimension is too large.");
            if (y > largestSize.Y)
                throw std::invalid_argument("The y dimension is too large.");
        }


        CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
        if (!GetConsoleScreenBufferInfo(h, &bufferInfo))
            throw std::runtime_error("Unable to retrieve screen buffer info.");

        SMALL_RECT& winInfo = bufferInfo.srWindow;
        COORD windowSize = { winInfo.Right - winInfo.Left + 1, winInfo.Bottom - winInfo.Top + 1 };

        if (windowSize.X > x || windowSize.Y > y)
        {
            // window size needs to be adjusted before the buffer size can be reduced.
            SMALL_RECT info =
            {
                0,
                0,
                x < windowSize.X ? x - 1 : windowSize.X - 1,
                y < windowSize.Y ? y - 1 : windowSize.Y - 1
            };

            if (!SetConsoleWindowInfo(h, TRUE, &info))
                throw std::runtime_error("Unable to resize window before resizing buffer.");
        }

        SetOutBufferSize(x, ScreenBufferHeigh);

        SMALL_RECT info = { 0, 0, x - 1, y - 1 };
        if (!SetConsoleWindowInfo(h, TRUE, &info))
            throw std::runtime_error("Unable to resize window after resizing buffer.");
    }

    void SetOutBufferSize(SHORT xSize, SHORT ySize)
    {
        COORD size = { xSize, ySize };
        if (!SetConsoleScreenBufferSize(g_stdConOut, size))
        {
            throw std::runtime_error("Unable to resize screen buffer.");
        }
    }

    void ResetWindowSize()
    {
        HWND console = GetConsoleWindow();
        MoveWindow(console, ConsoleRect.left,
            ConsoleRect.top, ConsoleRect.right - ConsoleRect.left,
            ConsoleRect.bottom - ConsoleRect.top, TRUE);
    }

private:
    const SHORT ScreenBufferHeigh;
    const SHORT ScreenBufferWidth;
    RECT ConsoleRect;
};

#endif //_COLOR_CONSOLE_H_

