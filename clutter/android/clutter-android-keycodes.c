/* Clutter -  An OpenGL based 'interactive canvas' library.
 * Android backend - initial entry point
 *
 * Copyright (C) 2012 Intel Corporation
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *  Lionel Landwerlin <lionel.g.landwerlin@linux.intel.com>
 */

#include "clutter-android-keycodes.h"

#include "clutter-keysyms.h"

#include <android/keycodes.h>

static const guint keycodes[AKEYCODE_BUTTON_MODE + 1] = {
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_UNKNOWN         = 0, */
  CLUTTER_KEY_Left, /* AKEYCODE_SOFT_LEFT       = 1, */
  CLUTTER_KEY_Right, /* AKEYCODE_SOFT_RIGHT      = 2, */
  CLUTTER_KEY_Home, /* AKEYCODE_HOME            = 3, */
  CLUTTER_KEY_Back, /* AKEYCODE_BACK            = 4, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_CALL            = 5, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_ENDCALL         = 6, */
  CLUTTER_KEY_0, /* AKEYCODE_0               = 7, */
  CLUTTER_KEY_1, /* AKEYCODE_1               = 8, */
  CLUTTER_KEY_2, /* AKEYCODE_2               = 9, */
  CLUTTER_KEY_3, /* AKEYCODE_3               = 10, */
  CLUTTER_KEY_4, /* AKEYCODE_4               = 11, */
  CLUTTER_KEY_5, /* AKEYCODE_5               = 12, */
  CLUTTER_KEY_6, /* AKEYCODE_6               = 13, */
  CLUTTER_KEY_7, /* AKEYCODE_7               = 14, */
  CLUTTER_KEY_8, /* AKEYCODE_8               = 15, */
  CLUTTER_KEY_9, /* AKEYCODE_9               = 16, */
  CLUTTER_KEY_asterisk, /* AKEYCODE_STAR            = 17, */
  CLUTTER_KEY_ssharp, /* AKEYCODE_POUND           = 18, */
  CLUTTER_KEY_KP_Up, /* AKEYCODE_DPAD_UP         = 19, */
  CLUTTER_KEY_KP_Down, /* AKEYCODE_DPAD_DOWN       = 20, */
  CLUTTER_KEY_KP_Left, /* AKEYCODE_DPAD_LEFT       = 21, */
  CLUTTER_KEY_KP_Right, /* AKEYCODE_DPAD_RIGHT      = 22, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_DPAD_CENTER     = 23, */
  CLUTTER_KEY_AudioRaiseVolume, /* AKEYCODE_VOLUME_UP       = 24, */
  CLUTTER_KEY_AudioLowerVolume, /* AKEYCODE_VOLUME_DOWN     = 25, */
  CLUTTER_KEY_PowerOff, /* AKEYCODE_POWER           = 26, */
  CLUTTER_KEY_WebCam, /* AKEYCODE_CAMERA          = 27, */
  CLUTTER_KEY_Clear, /* AKEYCODE_CLEAR           = 28, */
  CLUTTER_KEY_a, /* AKEYCODE_A               = 29, */
  CLUTTER_KEY_b, /* AKEYCODE_B               = 30, */
  CLUTTER_KEY_c, /* AKEYCODE_C               = 31, */
  CLUTTER_KEY_d, /* AKEYCODE_D               = 32, */
  CLUTTER_KEY_e, /* AKEYCODE_E               = 33, */
  CLUTTER_KEY_f, /* AKEYCODE_F               = 34, */
  CLUTTER_KEY_g, /* AKEYCODE_G               = 35, */
  CLUTTER_KEY_h, /* AKEYCODE_H               = 36, */
  CLUTTER_KEY_i, /* AKEYCODE_I               = 37, */
  CLUTTER_KEY_j, /* AKEYCODE_J               = 38, */
  CLUTTER_KEY_k, /* AKEYCODE_K               = 39, */
  CLUTTER_KEY_l, /* AKEYCODE_L               = 40, */
  CLUTTER_KEY_m, /* AKEYCODE_M               = 41, */
  CLUTTER_KEY_n, /* AKEYCODE_N               = 42, */
  CLUTTER_KEY_o, /* AKEYCODE_O               = 43, */
  CLUTTER_KEY_p, /* AKEYCODE_P               = 44, */
  CLUTTER_KEY_q, /* AKEYCODE_Q               = 45, */
  CLUTTER_KEY_r, /* AKEYCODE_R               = 46, */
  CLUTTER_KEY_s, /* AKEYCODE_S               = 47, */
  CLUTTER_KEY_t, /* AKEYCODE_T               = 48, */
  CLUTTER_KEY_u, /* AKEYCODE_U               = 49, */
  CLUTTER_KEY_v, /* AKEYCODE_V               = 50, */
  CLUTTER_KEY_w, /* AKEYCODE_W               = 51, */
  CLUTTER_KEY_x, /* AKEYCODE_X               = 52, */
  CLUTTER_KEY_y, /* AKEYCODE_Y               = 53, */
  CLUTTER_KEY_z, /* AKEYCODE_Z               = 54, */
  CLUTTER_KEY_comma, /* AKEYCODE_COMMA           = 55, */
  CLUTTER_KEY_period, /* AKEYCODE_PERIOD          = 56, */
  CLUTTER_KEY_Alt_L, /* AKEYCODE_ALT_LEFT        = 57, */
  CLUTTER_KEY_Alt_R, /* AKEYCODE_ALT_RIGHT       = 58, */
  CLUTTER_KEY_Shift_L, /* AKEYCODE_SHIFT_LEFT      = 59, */
  CLUTTER_KEY_Shift_R, /* AKEYCODE_SHIFT_RIGHT     = 60, */
  CLUTTER_KEY_Tab, /* AKEYCODE_TAB             = 61, */
  CLUTTER_KEY_space, /* AKEYCODE_SPACE           = 62, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_SYM             = 63, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_EXPLORER        = 64, */
  CLUTTER_KEY_Mail, /* AKEYCODE_ENVELOPE        = 65, */
  CLUTTER_KEY_Return, /* AKEYCODE_ENTER           = 66, */
  CLUTTER_KEY_BackSpace, /* AKEYCODE_DEL             = 67, */
  CLUTTER_KEY_grave, /* AKEYCODE_GRAVE           = 68, */
  CLUTTER_KEY_minus, /* AKEYCODE_MINUS           = 69, */
  CLUTTER_KEY_equal, /* AKEYCODE_EQUALS          = 70, */
  CLUTTER_KEY_bracketleft, /* AKEYCODE_LEFT_BRACKET    = 71, */
  CLUTTER_KEY_bracketright, /* AKEYCODE_RIGHT_BRACKET   = 72, */
  CLUTTER_KEY_backslash, /* AKEYCODE_BACKSLASH       = 73, */
  CLUTTER_KEY_semicolon, /* AKEYCODE_SEMICOLON       = 74, */
  CLUTTER_KEY_apostrophe, /* AKEYCODE_APOSTROPHE      = 75, */
  CLUTTER_KEY_slash, /* AKEYCODE_SLASH           = 76, */
  CLUTTER_KEY_at, /* AKEYCODE_AT              = 77, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_NUM             = 78, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_HEADSETHOOK     = 79, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_FOCUS           = 80,   // *Camera* focus */
  CLUTTER_KEY_plus, /* AKEYCODE_PLUS            = 81, */
  CLUTTER_KEY_Menu, /* AKEYCODE_MENU            = 82, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_NOTIFICATION    = 83, */
  CLUTTER_KEY_Search, /* AKEYCODE_SEARCH          = 84, */
  CLUTTER_KEY_AudioPlay, /* AKEYCODE_MEDIA_PLAY_PAUSE= 85, */
  CLUTTER_KEY_AudioStop, /* AKEYCODE_MEDIA_STOP      = 86, */
  CLUTTER_KEY_AudioNext, /* AKEYCODE_MEDIA_NEXT      = 87, */
  CLUTTER_KEY_AudioPrev, /* AKEYCODE_MEDIA_PREVIOUS  = 88, */
  CLUTTER_KEY_AudioRewind, /* AKEYCODE_MEDIA_REWIND    = 89, */
  CLUTTER_KEY_Forward, /* AKEYCODE_MEDIA_FAST_FORWARD = 90, */
  CLUTTER_KEY_AudioMute, /* AKEYCODE_MUTE            = 91, */
  CLUTTER_KEY_Page_Up, /* AKEYCODE_PAGE_UP         = 92, */
  CLUTTER_KEY_Page_Down, /* AKEYCODE_PAGE_DOWN       = 93, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_PICTSYMBOLS     = 94, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_SWITCH_CHARSET  = 95, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_A        = 96, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_B        = 97, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_C        = 98, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_X        = 99, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_Y        = 100, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_Z        = 101, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_L1       = 102, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_R1       = 103, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_L2       = 104, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_R2       = 105, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_THUMBL   = 106, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_THUMBR   = 107, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_START    = 108, */
  CLUTTER_KEY_Select, /* AKEYCODE_BUTTON_SELECT   = 109, */
  CLUTTER_KEY_VoidSymbol /* AKEYCODE_BUTTON_MODE     = 110, */
};

static const guint shifted_keycodes[AKEYCODE_BUTTON_MODE + 1] = {
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_UNKNOWN         = 0, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_SOFT_LEFT       = 1, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_SOFT_RIGHT      = 2, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_HOME            = 3, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BACK            = 4, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_CALL            = 5, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_ENDCALL         = 6, */
  CLUTTER_KEY_parenright, /* AKEYCODE_0               = 7, */
  CLUTTER_KEY_exclam, /* AKEYCODE_1               = 8, */
  CLUTTER_KEY_at, /* AKEYCODE_2               = 9, */
  CLUTTER_KEY_numbersign, /* AKEYCODE_3               = 10, */
  CLUTTER_KEY_dollar, /* AKEYCODE_4               = 11, */
  CLUTTER_KEY_percent, /* AKEYCODE_5               = 12, */
  CLUTTER_KEY_upcaret, /* AKEYCODE_6               = 13, */
  CLUTTER_KEY_ampersand, /* AKEYCODE_7               = 14, */
  CLUTTER_KEY_asterisk, /* AKEYCODE_8               = 15, */
  CLUTTER_KEY_parenleft, /* AKEYCODE_9               = 16, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_STAR            = 17, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_POUND           = 18, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_DPAD_UP         = 19, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_DPAD_DOWN       = 20, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_DPAD_LEFT       = 21, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_DPAD_RIGHT      = 22, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_DPAD_CENTER     = 23, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_VOLUME_UP       = 24, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_VOLUME_DOWN     = 25, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_POWER           = 26, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_CAMERA          = 27, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_CLEAR           = 28, */
  CLUTTER_KEY_A, /* AKEYCODE_A               = 29, */
  CLUTTER_KEY_B, /* AKEYCODE_B               = 30, */
  CLUTTER_KEY_C, /* AKEYCODE_C               = 31, */
  CLUTTER_KEY_D, /* AKEYCODE_D               = 32, */
  CLUTTER_KEY_E, /* AKEYCODE_E               = 33, */
  CLUTTER_KEY_F, /* AKEYCODE_F               = 34, */
  CLUTTER_KEY_G, /* AKEYCODE_G               = 35, */
  CLUTTER_KEY_H, /* AKEYCODE_H               = 36, */
  CLUTTER_KEY_I, /* AKEYCODE_I               = 37, */
  CLUTTER_KEY_J, /* AKEYCODE_J               = 38, */
  CLUTTER_KEY_K, /* AKEYCODE_K               = 39, */
  CLUTTER_KEY_L, /* AKEYCODE_L               = 40, */
  CLUTTER_KEY_M, /* AKEYCODE_M               = 41, */
  CLUTTER_KEY_N, /* AKEYCODE_N               = 42, */
  CLUTTER_KEY_O, /* AKEYCODE_O               = 43, */
  CLUTTER_KEY_P, /* AKEYCODE_P               = 44, */
  CLUTTER_KEY_Q, /* AKEYCODE_Q               = 45, */
  CLUTTER_KEY_R, /* AKEYCODE_R               = 46, */
  CLUTTER_KEY_S, /* AKEYCODE_S               = 47, */
  CLUTTER_KEY_T, /* AKEYCODE_T               = 48, */
  CLUTTER_KEY_U, /* AKEYCODE_U               = 49, */
  CLUTTER_KEY_V, /* AKEYCODE_V               = 50, */
  CLUTTER_KEY_W, /* AKEYCODE_W               = 51, */
  CLUTTER_KEY_X, /* AKEYCODE_X               = 52, */
  CLUTTER_KEY_Y, /* AKEYCODE_Y               = 53, */
  CLUTTER_KEY_Z, /* AKEYCODE_Z               = 54, */
  CLUTTER_KEY_less, /* AKEYCODE_COMMA           = 55, */
  CLUTTER_KEY_period, /* AKEYCODE_PERIOD          = 56, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_ALT_LEFT        = 57, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_ALT_RIGHT       = 58, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_SHIFT_LEFT      = 59, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_SHIFT_RIGHT     = 60, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_TAB             = 61, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_SPACE           = 62, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_SYM             = 63, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_EXPLORER        = 64, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_ENVELOPE        = 65, */
  CLUTTER_KEY_Return, /* AKEYCODE_ENTER           = 66, */
  CLUTTER_KEY_BackSpace, /* AKEYCODE_DEL             = 67, */
  CLUTTER_KEY_asciitilde, /* AKEYCODE_GRAVE           = 68, */
  CLUTTER_KEY_underscore, /* AKEYCODE_MINUS           = 69, */
  CLUTTER_KEY_plus, /* AKEYCODE_EQUALS          = 70, */
  CLUTTER_KEY_braceleft, /* AKEYCODE_LEFT_BRACKET    = 71, */
  CLUTTER_KEY_braceright, /* AKEYCODE_RIGHT_BRACKET   = 72, */
  CLUTTER_KEY_vertbar, /* AKEYCODE_BACKSLASH       = 73, */
  CLUTTER_KEY_colon, /* AKEYCODE_SEMICOLON       = 74, */
  CLUTTER_KEY_quotedbl, /* AKEYCODE_APOSTROPHE      = 75, */
  CLUTTER_KEY_question, /* AKEYCODE_SLASH           = 76, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_AT              = 77, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_NUM             = 78, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_HEADSETHOOK     = 79, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_FOCUS           = 80,   // *Camera* focus */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_PLUS            = 81, */
  CLUTTER_KEY_Menu, /* AKEYCODE_MENU            = 82, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_NOTIFICATION    = 83, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_SEARCH          = 84, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_MEDIA_PLAY_PAUSE= 85, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_MEDIA_STOP      = 86, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_MEDIA_NEXT      = 87, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_MEDIA_PREVIOUS  = 88, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_MEDIA_REWIND    = 89, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_MEDIA_FAST_FORWARD = 90, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_MUTE            = 91, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_PAGE_UP         = 92, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_PAGE_DOWN       = 93, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_PICTSYMBOLS     = 94, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_SWITCH_CHARSET  = 95, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_A        = 96, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_B        = 97, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_C        = 98, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_X        = 99, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_Y        = 100, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_Z        = 101, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_L1       = 102, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_R1       = 103, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_L2       = 104, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_R2       = 105, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_THUMBL   = 106, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_THUMBR   = 107, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_START    = 108, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_SELECT   = 109, */
  CLUTTER_KEY_VoidSymbol /* AKEYCODE_BUTTON_MODE     = 110, */
};

static const guint alted_keycodes[AKEYCODE_BUTTON_MODE + 1] = {
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_UNKNOWN         = 0, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_SOFT_LEFT       = 1, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_SOFT_RIGHT      = 2, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_HOME            = 3, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BACK            = 4, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_CALL            = 5, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_ENDCALL         = 6, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_0               = 7, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_1               = 8, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_2               = 9, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_3               = 10, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_4               = 11, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_5               = 12, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_6               = 13, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_7               = 14, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_8               = 15, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_9               = 16, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_STAR            = 17, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_POUND           = 18, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_DPAD_UP         = 19, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_DPAD_DOWN       = 20, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_DPAD_LEFT       = 21, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_DPAD_RIGHT      = 22, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_DPAD_CENTER     = 23, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_VOLUME_UP       = 24, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_VOLUME_DOWN     = 25, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_POWER           = 26, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_CAMERA          = 27, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_CLEAR           = 28, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_A               = 29, */
  CLUTTER_KEY_less, /* AKEYCODE_B               = 30, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_C               = 31, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_D               = 32, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_E               = 33, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_F               = 34, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_G               = 35, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_H               = 36, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_I               = 37, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_J               = 38, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_K               = 39, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_L               = 40, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_M               = 41, */
  CLUTTER_KEY_greater, /* AKEYCODE_N               = 42, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_O               = 43, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_P               = 44, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_Q               = 45, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_R               = 46, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_S               = 47, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_T               = 48, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_U               = 49, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_V               = 50, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_W               = 51, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_X               = 52, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_Y               = 53, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_Z               = 54, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_COMMA           = 55, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_PERIOD          = 56, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_ALT_LEFT        = 57, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_ALT_RIGHT       = 58, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_SHIFT_LEFT      = 59, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_SHIFT_RIGHT     = 60, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_TAB             = 61, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_SPACE           = 62, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_SYM             = 63, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_EXPLORER        = 64, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_ENVELOPE        = 65, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_ENTER           = 66, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_DEL             = 67, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_GRAVE           = 68, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_MINUS           = 69, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_EQUALS          = 70, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_LEFT_BRACKET    = 71, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_RIGHT_BRACKET   = 72, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BACKSLASH       = 73, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_SEMICOLON       = 74, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_APOSTROPHE      = 75, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_SLASH           = 76, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_AT              = 77, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_NUM             = 78, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_HEADSETHOOK     = 79, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_FOCUS           = 80,   // *Camera* focus */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_PLUS            = 81, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_MENU            = 82, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_NOTIFICATION    = 83, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_SEARCH          = 84, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_MEDIA_PLAY_PAUSE= 85, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_MEDIA_STOP      = 86, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_MEDIA_NEXT      = 87, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_MEDIA_PREVIOUS  = 88, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_MEDIA_REWIND    = 89, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_MEDIA_FAST_FORWARD = 90, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_MUTE            = 91, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_PAGE_UP         = 92, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_PAGE_DOWN       = 93, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_PICTSYMBOLS     = 94, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_SWITCH_CHARSET  = 95, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_A        = 96, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_B        = 97, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_C        = 98, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_X        = 99, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_Y        = 100, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_Z        = 101, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_L1       = 102, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_R1       = 103, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_L2       = 104, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_R2       = 105, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_THUMBL   = 106, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_THUMBR   = 107, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_START    = 108, */
  CLUTTER_KEY_VoidSymbol, /* AKEYCODE_BUTTON_SELECT   = 109, */
  CLUTTER_KEY_VoidSymbol /* AKEYCODE_BUTTON_MODE     = 110, */
};

static ClutterModifierType
_translate_modifier_state (int32_t modifier_state)
{
  ClutterModifierType clutter_modifier_state = 0;

  if (modifier_state & AMETA_ALT_ON)
    clutter_modifier_state = CLUTTER_MOD1_MASK;

  if (modifier_state & AMETA_SHIFT_ON)
    clutter_modifier_state |= CLUTTER_SHIFT_MASK;

  return clutter_modifier_state;
}

void
_clutter_android_translate_key_event (ClutterKeyEvent *event,
                                      int32_t modifier_state,
                                      AInputEvent *a_event)
{
  int32_t keycode = AKeyEvent_getKeyCode (a_event);

  if (keycode < 0 || keycode > AKEYCODE_BUTTON_MODE)
    {
      g_message ("Invalid keycode : %i", keycode);
      return;
    }

  if (modifier_state & AMETA_SHIFT_ON)
    event->keyval = shifted_keycodes[keycode];
  else if (modifier_state & AMETA_ALT_ON)
    event->keyval = alted_keycodes[keycode];
  else
    event->keyval = keycodes[keycode];

  event->modifier_state = _translate_modifier_state (modifier_state);
  event->unicode_value = clutter_keysym_to_unicode (event->keyval);
}
