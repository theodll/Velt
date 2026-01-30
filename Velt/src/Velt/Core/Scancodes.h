#pragma once

/*
Simple DirectMedia Layer
Copyright (C) 1997-2026 Sam Lantinga <slouken@libsdl.org>

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

namespace Velt
{
	enum class Scancode
    {
        VELT_SCANCODE_UNKNOWN = 0,

        /**
         *  \name Usage page 0x07
         *
         *  These values are from usage page 0x07 (USB keyboard page).
         */
         /* @{ */

        VELT_SCANCODE_A = 4,
        VELT_SCANCODE_B = 5,
        VELT_SCANCODE_C = 6,
        VELT_SCANCODE_D = 7,
        VELT_SCANCODE_E = 8,
        VELT_SCANCODE_F = 9,
        VELT_SCANCODE_G = 10,
        VELT_SCANCODE_H = 11,
        VELT_SCANCODE_I = 12,
        VELT_SCANCODE_J = 13,
        VELT_SCANCODE_K = 14,
        VELT_SCANCODE_L = 15,
        VELT_SCANCODE_M = 16,
        VELT_SCANCODE_N = 17,
        VELT_SCANCODE_O = 18,
        VELT_SCANCODE_P = 19,
        VELT_SCANCODE_Q = 20,
        VELT_SCANCODE_R = 21,
        VELT_SCANCODE_S = 22,
        VELT_SCANCODE_T = 23,
        VELT_SCANCODE_U = 24,
        VELT_SCANCODE_V = 25,
        VELT_SCANCODE_W = 26,
        VELT_SCANCODE_X = 27,
        VELT_SCANCODE_Y = 28,
        VELT_SCANCODE_Z = 29,

        VELT_SCANCODE_1 = 30,
        VELT_SCANCODE_2 = 31,
        VELT_SCANCODE_3 = 32,
        VELT_SCANCODE_4 = 33,
        VELT_SCANCODE_5 = 34,
        VELT_SCANCODE_6 = 35,
        VELT_SCANCODE_7 = 36,
        VELT_SCANCODE_8 = 37,
        VELT_SCANCODE_9 = 38,
        VELT_SCANCODE_0 = 39,

        VELT_SCANCODE_RETURN = 40,
        VELT_SCANCODE_ESCAPE = 41,
        VELT_SCANCODE_BACKSPACE = 42,
        VELT_SCANCODE_TAB = 43,
        VELT_SCANCODE_SPACE = 44,

        VELT_SCANCODE_MINUS = 45,
        VELT_SCANCODE_EQUALS = 46,
        VELT_SCANCODE_LEFTBRACKET = 47,
        VELT_SCANCODE_RIGHTBRACKET = 48,
        VELT_SCANCODE_BACKSLASH = 49, /**< Located at the lower left of the return
                                      *   key on ISO keyboards and at the right end
                                      *   of the QWERTY row on ANSI keyboards.
                                      *   Produces REVERSE SOLIDUS (backslash) and
                                      *   VERTICAL LINE in a US layout, REVERSE
                                      *   SOLIDUS and VERTICAL LINE in a UK Mac
                                      *   layout, NUMBER SIGN and TILDE in a UK
                                      *   Windows layout, DOLLAR SIGN and POUND SIGN
                                      *   in a Swiss German layout, NUMBER SIGN and
                                      *   APOSTROPHE in a German layout, GRAVE
                                      *   ACCENT and POUND SIGN in a French Mac
                                      *   layout, and ASTERISK and MICRO SIGN in a
                                      *   French Windows layout.
                                      */
        VELT_SCANCODE_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
                                      *   instead of 49 for the same key, but all
                                      *   OSes I've seen treat the two codes
                                      *   identically. So, as an implementor, unless
                                      *   your keyboard generates both of those
                                      *   codes and your OS treats them differently,
                                      *   you should generate VELT_SCANCODE_BACKSLASH
                                      *   instead of this code. As a user, you
                                      *   should not rely on this code because VELT
                                      *   will never generate it with most (all?)
                                      *   keyboards.
                                      */
        VELT_SCANCODE_SEMICOLON = 51,
        VELT_SCANCODE_APOSTROPHE = 52,
        VELT_SCANCODE_GRAVE = 53, /**< Located in the top left corner (on both ANSI
                                  *   and ISO keyboards). Produces GRAVE ACCENT and
                                  *   TILDE in a US Windows layout and in US and UK
                                  *   Mac layouts on ANSI keyboards, GRAVE ACCENT
                                  *   and NOT SIGN in a UK Windows layout, SECTION
                                  *   SIGN and PLUS-MINUS SIGN in US and UK Mac
                                  *   layouts on ISO keyboards, SECTION SIGN and
                                  *   DEGREE SIGN in a Swiss German layout (Mac:
                                  *   only on ISO keyboards), CIRCUMFLEX ACCENT and
                                  *   DEGREE SIGN in a German layout (Mac: only on
                                  *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
                                  *   French Windows layout, COMMERCIAL AT and
                                  *   NUMBER SIGN in a French Mac layout on ISO
                                  *   keyboards, and LESS-THAN SIGN and GREATER-THAN
                                  *   SIGN in a Swiss German, German, or French Mac
                                  *   layout on ANSI keyboards.
                                  */
        VELT_SCANCODE_COMMA = 54,
        VELT_SCANCODE_PERIOD = 55,
        VELT_SCANCODE_SLASH = 56,

        VELT_SCANCODE_CAPSLOCK = 57,

        VELT_SCANCODE_F1 = 58,
        VELT_SCANCODE_F2 = 59,
        VELT_SCANCODE_F3 = 60,
        VELT_SCANCODE_F4 = 61,
        VELT_SCANCODE_F5 = 62,
        VELT_SCANCODE_F6 = 63,
        VELT_SCANCODE_F7 = 64,
        VELT_SCANCODE_F8 = 65,
        VELT_SCANCODE_F9 = 66,
        VELT_SCANCODE_F10 = 67,
        VELT_SCANCODE_F11 = 68,
        VELT_SCANCODE_F12 = 69,

        VELT_SCANCODE_PRINTSCREEN = 70,
        VELT_SCANCODE_SCROLLLOCK = 71,
        VELT_SCANCODE_PAUSE = 72,
        VELT_SCANCODE_INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
                                       does send code 73, not 117) */
        VELT_SCANCODE_HOME = 74,
        VELT_SCANCODE_PAGEUP = 75,
        VELT_SCANCODE_DELETE = 76,
        VELT_SCANCODE_END = 77,
        VELT_SCANCODE_PAGEDOWN = 78,
        VELT_SCANCODE_RIGHT = 79,
        VELT_SCANCODE_LEFT = 80,
        VELT_SCANCODE_DOWN = 81,
        VELT_SCANCODE_UP = 82,

        VELT_SCANCODE_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                                         */
        VELT_SCANCODE_KP_DIVIDE = 84,
        VELT_SCANCODE_KP_MULTIPLY = 85,
        VELT_SCANCODE_KP_MINUS = 86,
        VELT_SCANCODE_KP_PLUS = 87,
        VELT_SCANCODE_KP_ENTER = 88,
        VELT_SCANCODE_KP_1 = 89,
        VELT_SCANCODE_KP_2 = 90,
        VELT_SCANCODE_KP_3 = 91,
        VELT_SCANCODE_KP_4 = 92,
        VELT_SCANCODE_KP_5 = 93,
        VELT_SCANCODE_KP_6 = 94,
        VELT_SCANCODE_KP_7 = 95,
        VELT_SCANCODE_KP_8 = 96,
        VELT_SCANCODE_KP_9 = 97,
        VELT_SCANCODE_KP_0 = 98,
        VELT_SCANCODE_KP_PERIOD = 99,

        VELT_SCANCODE_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                                            *   keyboards have over ANSI ones,
                                            *   located between left shift and Z.
                                            *   Produces GRAVE ACCENT and TILDE in a
                                            *   US or UK Mac layout, REVERSE SOLIDUS
                                            *   (backslash) and VERTICAL LINE in a
                                            *   US or UK Windows layout, and
                                            *   LESS-THAN SIGN and GREATER-THAN SIGN
                                            *   in a Swiss German, German, or French
                                            *   layout. */
        VELT_SCANCODE_APPLICATION = 101, /**< windows contextual menu, compose */
        VELT_SCANCODE_POWER = 102, /**< The USB document says this is a status flag,
                                   *   not a physical key - but some Mac keyboards
                                   *   do have a power key. */
        VELT_SCANCODE_KP_EQUALS = 103,
        VELT_SCANCODE_F13 = 104,
        VELT_SCANCODE_F14 = 105,
        VELT_SCANCODE_F15 = 106,
        VELT_SCANCODE_F16 = 107,
        VELT_SCANCODE_F17 = 108,
        VELT_SCANCODE_F18 = 109,
        VELT_SCANCODE_F19 = 110,
        VELT_SCANCODE_F20 = 111,
        VELT_SCANCODE_F21 = 112,
        VELT_SCANCODE_F22 = 113,
        VELT_SCANCODE_F23 = 114,
        VELT_SCANCODE_F24 = 115,
        VELT_SCANCODE_EXECUTE = 116,
        VELT_SCANCODE_HELP = 117,    /**< AL Integrated Help Center */
        VELT_SCANCODE_MENU = 118,    /**< Menu (show menu) */
        VELT_SCANCODE_SELECT = 119,
        VELT_SCANCODE_STOP = 120,    /**< AC Stop */
        VELT_SCANCODE_AGAIN = 121,   /**< AC Redo/Repeat */
        VELT_SCANCODE_UNDO = 122,    /**< AC Undo */
        VELT_SCANCODE_CUT = 123,     /**< AC Cut */
        VELT_SCANCODE_COPY = 124,    /**< AC Copy */
        VELT_SCANCODE_PASTE = 125,   /**< AC Paste */
        VELT_SCANCODE_FIND = 126,    /**< AC Find */
        VELT_SCANCODE_MUTE = 127,
        VELT_SCANCODE_VOLUMEUP = 128,
        VELT_SCANCODE_VOLUMEDOWN = 129,
        /* not sure whether there's a reason to enable these */
        /*     VELT_SCANCODE_LOCKINGCAPSLOCK = 130,  */
        /*     VELT_SCANCODE_LOCKINGNUMLOCK = 131, */
        /*     VELT_SCANCODE_LOCKINGSCROLLLOCK = 132, */
        VELT_SCANCODE_KP_COMMA = 133,
        VELT_SCANCODE_KP_EQUALSAS400 = 134,

        VELT_SCANCODE_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                                footnotes in USB doc */
        VELT_SCANCODE_INTERNATIONAL2 = 136,
        VELT_SCANCODE_INTERNATIONAL3 = 137, /**< Yen */
        VELT_SCANCODE_INTERNATIONAL4 = 138,
        VELT_SCANCODE_INTERNATIONAL5 = 139,
        VELT_SCANCODE_INTERNATIONAL6 = 140,
        VELT_SCANCODE_INTERNATIONAL7 = 141,
        VELT_SCANCODE_INTERNATIONAL8 = 142,
        VELT_SCANCODE_INTERNATIONAL9 = 143,
        VELT_SCANCODE_LANG1 = 144, /**< Hangul/English toggle */
        VELT_SCANCODE_LANG2 = 145, /**< Hanja conversion */
        VELT_SCANCODE_LANG3 = 146, /**< Katakana */
        VELT_SCANCODE_LANG4 = 147, /**< Hiragana */
        VELT_SCANCODE_LANG5 = 148, /**< Zenkaku/Hankaku */
        VELT_SCANCODE_LANG6 = 149, /**< reserved */
        VELT_SCANCODE_LANG7 = 150, /**< reserved */
        VELT_SCANCODE_LANG8 = 151, /**< reserved */
        VELT_SCANCODE_LANG9 = 152, /**< reserved */

        VELT_SCANCODE_ALTERASE = 153,    /**< Erase-Eaze */
        VELT_SCANCODE_SYSREQ = 154,
        VELT_SCANCODE_CANCEL = 155,      /**< AC Cancel */
        VELT_SCANCODE_CLEAR = 156,
        VELT_SCANCODE_PRIOR = 157,
        VELT_SCANCODE_RETURN2 = 158,
        VELT_SCANCODE_SEPARATOR = 159,
        VELT_SCANCODE_OUT = 160,
        VELT_SCANCODE_OPER = 161,
        VELT_SCANCODE_CLEARAGAIN = 162,
        VELT_SCANCODE_CRSEL = 163,
        VELT_SCANCODE_EXSEL = 164,

        VELT_SCANCODE_KP_00 = 176,
        VELT_SCANCODE_KP_000 = 177,
        VELT_SCANCODE_THOUSANDSSEPARATOR = 178,
        VELT_SCANCODE_DECIMALSEPARATOR = 179,
        VELT_SCANCODE_CURRENCYUNIT = 180,
        VELT_SCANCODE_CURRENCYSUBUNIT = 181,
        VELT_SCANCODE_KP_LEFTPAREN = 182,
        VELT_SCANCODE_KP_RIGHTPAREN = 183,
        VELT_SCANCODE_KP_LEFTBRACE = 184,
        VELT_SCANCODE_KP_RIGHTBRACE = 185,
        VELT_SCANCODE_KP_TAB = 186,
        VELT_SCANCODE_KP_BACKSPACE = 187,
        VELT_SCANCODE_KP_A = 188,
        VELT_SCANCODE_KP_B = 189,
        VELT_SCANCODE_KP_C = 190,
        VELT_SCANCODE_KP_D = 191,
        VELT_SCANCODE_KP_E = 192,
        VELT_SCANCODE_KP_F = 193,
        VELT_SCANCODE_KP_XOR = 194,
        VELT_SCANCODE_KP_POWER = 195,
        VELT_SCANCODE_KP_PERCENT = 196,
        VELT_SCANCODE_KP_LESS = 197,
        VELT_SCANCODE_KP_GREATER = 198,
        VELT_SCANCODE_KP_AMPERSAND = 199,
        VELT_SCANCODE_KP_DBLAMPERSAND = 200,
        VELT_SCANCODE_KP_VERTICALBAR = 201,
        VELT_SCANCODE_KP_DBLVERTICALBAR = 202,
        VELT_SCANCODE_KP_COLON = 203,
        VELT_SCANCODE_KP_HASH = 204,
        VELT_SCANCODE_KP_SPACE = 205,
        VELT_SCANCODE_KP_AT = 206,
        VELT_SCANCODE_KP_EXCLAM = 207,
        VELT_SCANCODE_KP_MEMSTORE = 208,
        VELT_SCANCODE_KP_MEMRECALL = 209,
        VELT_SCANCODE_KP_MEMCLEAR = 210,
        VELT_SCANCODE_KP_MEMADD = 211,
        VELT_SCANCODE_KP_MEMSUBTRACT = 212,
        VELT_SCANCODE_KP_MEMMULTIPLY = 213,
        VELT_SCANCODE_KP_MEMDIVIDE = 214,
        VELT_SCANCODE_KP_PLUSMINUS = 215,
        VELT_SCANCODE_KP_CLEAR = 216,
        VELT_SCANCODE_KP_CLEARENTRY = 217,
        VELT_SCANCODE_KP_BINARY = 218,
        VELT_SCANCODE_KP_OCTAL = 219,
        VELT_SCANCODE_KP_DECIMAL = 220,
        VELT_SCANCODE_KP_HEXADECIMAL = 221,

        VELT_SCANCODE_LCTRL = 224,
        VELT_SCANCODE_LSHIFT = 225,
        VELT_SCANCODE_LALT = 226, /**< alt, option */
        VELT_SCANCODE_LGUI = 227, /**< windows, command (apple), meta */
        VELT_SCANCODE_RCTRL = 228,
        VELT_SCANCODE_RSHIFT = 229,
        VELT_SCANCODE_RALT = 230, /**< alt gr, option */
        VELT_SCANCODE_RGUI = 231, /**< windows, command (apple), meta */

        VELT_SCANCODE_MODE = 257,    /**< I'm not sure if this is really not covered
                                     *   by any of the above, but since there's a
                                     *   special VELT_KMOD_MODE for it I'm adding it here
                                     */

                                     /* @} *//* Usage page 0x07 */

                                     /**
                                      *  \name Usage page 0x0C
                                      *
                                      *  These values are mapped from usage page 0x0C (USB consumer page).
                                      *
                                      *  There are way more keys in the spec than we can represent in the
                                      *  current scancode range, so pick the ones that commonly come up in
                                      *  real world usage.
                                      */
                                      /* @{ */

        VELT_SCANCODE_SLEEP = 258,                   /**< Sleep */
        VELT_SCANCODE_WAKE = 259,                    /**< Wake */

        VELT_SCANCODE_CHANNEL_INCREMENT = 260,       /**< Channel Increment */
        VELT_SCANCODE_CHANNEL_DECREMENT = 261,       /**< Channel Decrement */

        VELT_SCANCODE_MEDIA_PLAY = 262,          /**< Play */
        VELT_SCANCODE_MEDIA_PAUSE = 263,         /**< Pause */
        VELT_SCANCODE_MEDIA_RECORD = 264,        /**< Record */
        VELT_SCANCODE_MEDIA_FAST_FORWARD = 265,  /**< Fast Forward */
        VELT_SCANCODE_MEDIA_REWIND = 266,        /**< Rewind */
        VELT_SCANCODE_MEDIA_NEXT_TRACK = 267,    /**< Next Track */
        VELT_SCANCODE_MEDIA_PREVIOUS_TRACK = 268, /**< Previous Track */
        VELT_SCANCODE_MEDIA_STOP = 269,          /**< Stop */
        VELT_SCANCODE_MEDIA_EJECT = 270,         /**< Eject */
        VELT_SCANCODE_MEDIA_PLAY_PAUSE = 271,    /**< Play / Pause */
        VELT_SCANCODE_MEDIA_SELECT = 272,        /* Media Select */

        VELT_SCANCODE_AC_NEW = 273,              /**< AC New */
        VELT_SCANCODE_AC_OPEN = 274,             /**< AC Open */
        VELT_SCANCODE_AC_CLOSE = 275,            /**< AC Close */
        VELT_SCANCODE_AC_EXIT = 276,             /**< AC Exit */
        VELT_SCANCODE_AC_SAVE = 277,             /**< AC Save */
        VELT_SCANCODE_AC_PRINT = 278,            /**< AC Print */
        VELT_SCANCODE_AC_PROPERTIES = 279,       /**< AC Properties */

        VELT_SCANCODE_AC_SEARCH = 280,           /**< AC Search */
        VELT_SCANCODE_AC_HOME = 281,             /**< AC Home */
        VELT_SCANCODE_AC_BACK = 282,             /**< AC Back */
        VELT_SCANCODE_AC_FORWARD = 283,          /**< AC Forward */
        VELT_SCANCODE_AC_STOP = 284,             /**< AC Stop */
        VELT_SCANCODE_AC_REFRESH = 285,          /**< AC Refresh */
        VELT_SCANCODE_AC_BOOKMARKS = 286,        /**< AC Bookmarks */

        /* @} *//* Usage page 0x0C */


        /**
         *  \name Mobile keys
         *
         *  These are values that are often used on mobile phones.
         */
         /* @{ */

        VELT_SCANCODE_SOFTLEFT = 287, /**< Usually situated below the display on phones and
                                          used as a multi-function feature key for selecting
                                          a software defined function shown on the bottom left
                                          of the display. */
        VELT_SCANCODE_SOFTRIGHT = 288, /**< Usually situated below the display on phones and
                                           used as a multi-function feature key for selecting
                                           a software defined function shown on the bottom right
                                           of the display. */
        VELT_SCANCODE_CALL = 289, /**< Used for accepting phone calls. */
        VELT_SCANCODE_ENDCALL = 290, /**< Used for rejecting phone calls. */

        /* @} *//* Mobile keys */

        /* Add any other keys here. */

        VELT_SCANCODE_RESERVED = 400,    /**< 400-500 reserved for dynamic keycodes */

        VELT_SCANCODE_COUNT = 512 /**< not a key, just marks the number of scancodes for array bounds */

    };
}