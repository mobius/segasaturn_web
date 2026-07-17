#include "input_backend_sdl3.hpp"

#include <ymir/util/bitmask_enum.hpp>

namespace app::input {

KeyboardKey SDL3ScancodeToKeyboardKey(SDL_Scancode key) {
    switch (key) {
    case SDL_SCANCODE_A: return KeyboardKey::A;
    case SDL_SCANCODE_B: return KeyboardKey::B;
    case SDL_SCANCODE_C: return KeyboardKey::C;
    case SDL_SCANCODE_D: return KeyboardKey::D;
    case SDL_SCANCODE_E: return KeyboardKey::E;
    case SDL_SCANCODE_F: return KeyboardKey::F;
    case SDL_SCANCODE_G: return KeyboardKey::G;
    case SDL_SCANCODE_H: return KeyboardKey::H;
    case SDL_SCANCODE_I: return KeyboardKey::I;
    case SDL_SCANCODE_J: return KeyboardKey::J;
    case SDL_SCANCODE_K: return KeyboardKey::K;
    case SDL_SCANCODE_L: return KeyboardKey::L;
    case SDL_SCANCODE_M: return KeyboardKey::M;
    case SDL_SCANCODE_N: return KeyboardKey::N;
    case SDL_SCANCODE_O: return KeyboardKey::O;
    case SDL_SCANCODE_P: return KeyboardKey::P;
    case SDL_SCANCODE_Q: return KeyboardKey::Q;
    case SDL_SCANCODE_R: return KeyboardKey::R;
    case SDL_SCANCODE_S: return KeyboardKey::S;
    case SDL_SCANCODE_T: return KeyboardKey::T;
    case SDL_SCANCODE_U: return KeyboardKey::U;
    case SDL_SCANCODE_V: return KeyboardKey::V;
    case SDL_SCANCODE_W: return KeyboardKey::W;
    case SDL_SCANCODE_X: return KeyboardKey::X;
    case SDL_SCANCODE_Y: return KeyboardKey::Y;
    case SDL_SCANCODE_Z: return KeyboardKey::Z;

    case SDL_SCANCODE_1: return KeyboardKey::Alpha1;
    case SDL_SCANCODE_2: return KeyboardKey::Alpha2;
    case SDL_SCANCODE_3: return KeyboardKey::Alpha3;
    case SDL_SCANCODE_4: return KeyboardKey::Alpha4;
    case SDL_SCANCODE_5: return KeyboardKey::Alpha5;
    case SDL_SCANCODE_6: return KeyboardKey::Alpha6;
    case SDL_SCANCODE_7: return KeyboardKey::Alpha7;
    case SDL_SCANCODE_8: return KeyboardKey::Alpha8;
    case SDL_SCANCODE_9: return KeyboardKey::Alpha9;
    case SDL_SCANCODE_0: return KeyboardKey::Alpha0;

    case SDL_SCANCODE_RETURN: return KeyboardKey::Return;
    case SDL_SCANCODE_ESCAPE: return KeyboardKey::Escape;
    case SDL_SCANCODE_BACKSPACE: return KeyboardKey::Backspace;
    case SDL_SCANCODE_TAB: return KeyboardKey::Tab;
    case SDL_SCANCODE_SPACE: return KeyboardKey::Spacebar;

    case SDL_SCANCODE_MINUS: return KeyboardKey::MinusUnderscore;
    case SDL_SCANCODE_EQUALS: return KeyboardKey::EqualsPlus;
    case SDL_SCANCODE_LEFTBRACKET: return KeyboardKey::LeftBracket;
    case SDL_SCANCODE_RIGHTBRACKET: return KeyboardKey::RightBracket;
    case SDL_SCANCODE_BACKSLASH: return KeyboardKey::Backslash;
    case SDL_SCANCODE_NONUSHASH: return KeyboardKey::PoundTilde;
    case SDL_SCANCODE_SEMICOLON: return KeyboardKey::Semicolon;
    case SDL_SCANCODE_APOSTROPHE: return KeyboardKey::Apostrophe;
    case SDL_SCANCODE_GRAVE: return KeyboardKey::GraveAccent;
    case SDL_SCANCODE_COMMA: return KeyboardKey::Comma;
    case SDL_SCANCODE_PERIOD: return KeyboardKey::Period;
    case SDL_SCANCODE_SLASH: return KeyboardKey::Slash;

    case SDL_SCANCODE_CAPSLOCK: return KeyboardKey::CapsLock;

    case SDL_SCANCODE_F1: return KeyboardKey::F1;
    case SDL_SCANCODE_F2: return KeyboardKey::F2;
    case SDL_SCANCODE_F3: return KeyboardKey::F3;
    case SDL_SCANCODE_F4: return KeyboardKey::F4;
    case SDL_SCANCODE_F5: return KeyboardKey::F5;
    case SDL_SCANCODE_F6: return KeyboardKey::F6;
    case SDL_SCANCODE_F7: return KeyboardKey::F7;
    case SDL_SCANCODE_F8: return KeyboardKey::F8;
    case SDL_SCANCODE_F9: return KeyboardKey::F9;
    case SDL_SCANCODE_F10: return KeyboardKey::F10;
    case SDL_SCANCODE_F11: return KeyboardKey::F11;
    case SDL_SCANCODE_F12: return KeyboardKey::F12;

    case SDL_SCANCODE_PRINTSCREEN: return KeyboardKey::PrintScreen;
    case SDL_SCANCODE_SCROLLLOCK: return KeyboardKey::ScrollLock;
    case SDL_SCANCODE_PAUSE: return KeyboardKey::Pause;
    case SDL_SCANCODE_INSERT: return KeyboardKey::Insert;
    case SDL_SCANCODE_HOME: return KeyboardKey::Home;
    case SDL_SCANCODE_PAGEUP: return KeyboardKey::PageUp;
    case SDL_SCANCODE_DELETE: return KeyboardKey::Delete;
    case SDL_SCANCODE_END: return KeyboardKey::End;
    case SDL_SCANCODE_PAGEDOWN: return KeyboardKey::PageDown;
    case SDL_SCANCODE_RIGHT: return KeyboardKey::Right;
    case SDL_SCANCODE_LEFT: return KeyboardKey::Left;
    case SDL_SCANCODE_DOWN: return KeyboardKey::Down;
    case SDL_SCANCODE_UP: return KeyboardKey::Up;

    case SDL_SCANCODE_NUMLOCKCLEAR: return KeyboardKey::NumLock;
    case SDL_SCANCODE_KP_DIVIDE: return KeyboardKey::KeyPadDivide;
    case SDL_SCANCODE_KP_MULTIPLY: return KeyboardKey::KeyPadMultiply;
    case SDL_SCANCODE_KP_MINUS: return KeyboardKey::KeyPadSubtract;
    case SDL_SCANCODE_KP_PLUS: return KeyboardKey::KeyPadAdd;
    case SDL_SCANCODE_KP_ENTER: return KeyboardKey::KeyPadEnter;
    case SDL_SCANCODE_KP_1: return KeyboardKey::KeyPad1;
    case SDL_SCANCODE_KP_2: return KeyboardKey::KeyPad2;
    case SDL_SCANCODE_KP_3: return KeyboardKey::KeyPad3;
    case SDL_SCANCODE_KP_4: return KeyboardKey::KeyPad4;
    case SDL_SCANCODE_KP_5: return KeyboardKey::KeyPad5;
    case SDL_SCANCODE_KP_6: return KeyboardKey::KeyPad6;
    case SDL_SCANCODE_KP_7: return KeyboardKey::KeyPad7;
    case SDL_SCANCODE_KP_8: return KeyboardKey::KeyPad8;
    case SDL_SCANCODE_KP_9: return KeyboardKey::KeyPad9;
    case SDL_SCANCODE_KP_0: return KeyboardKey::KeyPad0;
    case SDL_SCANCODE_KP_PERIOD: return KeyboardKey::KeyPadPeriod;

    case SDL_SCANCODE_NONUSBACKSLASH: return KeyboardKey::NonUSBackslash;
    case SDL_SCANCODE_APPLICATION: return KeyboardKey::Application;
    case SDL_SCANCODE_POWER: return KeyboardKey::Power;
    case SDL_SCANCODE_KP_EQUALS: return KeyboardKey::KeyPadEquals;
    case SDL_SCANCODE_F13: return KeyboardKey::F13;
    case SDL_SCANCODE_F14: return KeyboardKey::F14;
    case SDL_SCANCODE_F15: return KeyboardKey::F15;
    case SDL_SCANCODE_F16: return KeyboardKey::F16;
    case SDL_SCANCODE_F17: return KeyboardKey::F17;
    case SDL_SCANCODE_F18: return KeyboardKey::F18;
    case SDL_SCANCODE_F19: return KeyboardKey::F19;
    case SDL_SCANCODE_F20: return KeyboardKey::F20;
    case SDL_SCANCODE_F21: return KeyboardKey::F21;
    case SDL_SCANCODE_F22: return KeyboardKey::F22;
    case SDL_SCANCODE_F23: return KeyboardKey::F23;
    case SDL_SCANCODE_F24: return KeyboardKey::F24;
    case SDL_SCANCODE_EXECUTE: return KeyboardKey::Execute;
    case SDL_SCANCODE_HELP: return KeyboardKey::Help;
    case SDL_SCANCODE_MENU: return KeyboardKey::Menu;
    case SDL_SCANCODE_SELECT: return KeyboardKey::Select;
    case SDL_SCANCODE_STOP: return KeyboardKey::Stop;
    case SDL_SCANCODE_AGAIN: return KeyboardKey::Again;
    case SDL_SCANCODE_UNDO: return KeyboardKey::Undo;
    case SDL_SCANCODE_CUT: return KeyboardKey::Cut;
    case SDL_SCANCODE_COPY: return KeyboardKey::Copy;
    case SDL_SCANCODE_PASTE: return KeyboardKey::Paste;
    case SDL_SCANCODE_FIND: return KeyboardKey::Find;
    case SDL_SCANCODE_MUTE: return KeyboardKey::Mute;
    case SDL_SCANCODE_VOLUMEUP: return KeyboardKey::VolumeUp;
    case SDL_SCANCODE_VOLUMEDOWN: return KeyboardKey::VolumeDown;
    case SDL_SCANCODE_KP_COMMA: return KeyboardKey::KeyPadComma;
    case SDL_SCANCODE_KP_EQUALSAS400: return KeyboardKey::KeyPadEqualSign;

    case SDL_SCANCODE_INTERNATIONAL1: return KeyboardKey::Intl1;
    case SDL_SCANCODE_INTERNATIONAL2: return KeyboardKey::Intl2;
    case SDL_SCANCODE_INTERNATIONAL3: return KeyboardKey::Intl3;
    case SDL_SCANCODE_INTERNATIONAL4: return KeyboardKey::Intl4;
    case SDL_SCANCODE_INTERNATIONAL5: return KeyboardKey::Intl5;
    case SDL_SCANCODE_INTERNATIONAL6: return KeyboardKey::Intl6;
    case SDL_SCANCODE_INTERNATIONAL7: return KeyboardKey::Intl7;
    case SDL_SCANCODE_INTERNATIONAL8: return KeyboardKey::Intl8;
    case SDL_SCANCODE_INTERNATIONAL9: return KeyboardKey::Intl9;
    case SDL_SCANCODE_LANG1: return KeyboardKey::LANG1;
    case SDL_SCANCODE_LANG2: return KeyboardKey::LANG2;
    case SDL_SCANCODE_LANG3: return KeyboardKey::LANG3;
    case SDL_SCANCODE_LANG4: return KeyboardKey::LANG4;
    case SDL_SCANCODE_LANG5: return KeyboardKey::LANG5;
    case SDL_SCANCODE_LANG6: return KeyboardKey::LANG6;
    case SDL_SCANCODE_LANG7: return KeyboardKey::LANG7;
    case SDL_SCANCODE_LANG8: return KeyboardKey::LANG8;
    case SDL_SCANCODE_LANG9: return KeyboardKey::LANG9;

    case SDL_SCANCODE_ALTERASE: return KeyboardKey::AltErase;
    case SDL_SCANCODE_SYSREQ: return KeyboardKey::SysReq;
    case SDL_SCANCODE_CANCEL: return KeyboardKey::Cancel;
    case SDL_SCANCODE_CLEAR: return KeyboardKey::Clear;
    case SDL_SCANCODE_PRIOR: return KeyboardKey::Prior;
    case SDL_SCANCODE_RETURN2: return KeyboardKey::Return2;
    case SDL_SCANCODE_SEPARATOR: return KeyboardKey::Separator;
    case SDL_SCANCODE_OUT: return KeyboardKey::Out;
    case SDL_SCANCODE_OPER: return KeyboardKey::Oper;
    case SDL_SCANCODE_CLEARAGAIN: return KeyboardKey::ClearAgain;
    case SDL_SCANCODE_CRSEL: return KeyboardKey::CrSelProps;
    case SDL_SCANCODE_EXSEL: return KeyboardKey::ExSel;

    case SDL_SCANCODE_KP_00: return KeyboardKey::KeyPad00;
    case SDL_SCANCODE_KP_000: return KeyboardKey::KeyPad000;
    case SDL_SCANCODE_THOUSANDSSEPARATOR: return KeyboardKey::ThousandsSeparator;
    case SDL_SCANCODE_DECIMALSEPARATOR: return KeyboardKey::DecimalSeparator;
    case SDL_SCANCODE_CURRENCYUNIT: return KeyboardKey::CurrencyUnit;
    case SDL_SCANCODE_CURRENCYSUBUNIT: return KeyboardKey::CurrencySubUnit;
    case SDL_SCANCODE_KP_LEFTPAREN: return KeyboardKey::KeyPadLeftParenthesis;
    case SDL_SCANCODE_KP_RIGHTPAREN: return KeyboardKey::KeyPadRightParenthesis;
    case SDL_SCANCODE_KP_LEFTBRACE: return KeyboardKey::KeyPadLeftBrace;
    case SDL_SCANCODE_KP_RIGHTBRACE: return KeyboardKey::KeyPadRightBrace;
    case SDL_SCANCODE_KP_TAB: return KeyboardKey::KeyPadTab;
    case SDL_SCANCODE_KP_BACKSPACE: return KeyboardKey::KeyPadBackspace;
    case SDL_SCANCODE_KP_A: return KeyboardKey::KeyPadA;
    case SDL_SCANCODE_KP_B: return KeyboardKey::KeyPadB;
    case SDL_SCANCODE_KP_C: return KeyboardKey::KeyPadC;
    case SDL_SCANCODE_KP_D: return KeyboardKey::KeyPadD;
    case SDL_SCANCODE_KP_E: return KeyboardKey::KeyPadE;
    case SDL_SCANCODE_KP_F: return KeyboardKey::KeyPadF;
    case SDL_SCANCODE_KP_XOR: return KeyboardKey::KeyPadXOR;
    case SDL_SCANCODE_KP_POWER: return KeyboardKey::KeyPadPower;
    case SDL_SCANCODE_KP_PERCENT: return KeyboardKey::KeyPadPercent;
    case SDL_SCANCODE_KP_LESS: return KeyboardKey::KeyPadLess;
    case SDL_SCANCODE_KP_GREATER: return KeyboardKey::KeyPadGreater;
    case SDL_SCANCODE_KP_AMPERSAND: return KeyboardKey::KeyPadAmpersand;
    case SDL_SCANCODE_KP_DBLAMPERSAND: return KeyboardKey::KeyPadDoubleAmpersand;
    case SDL_SCANCODE_KP_VERTICALBAR: return KeyboardKey::KeyPadVerticalBar;
    case SDL_SCANCODE_KP_DBLVERTICALBAR: return KeyboardKey::KeyPadDoubleVerticalBar;
    case SDL_SCANCODE_KP_COLON: return KeyboardKey::KeyPadColon;
    case SDL_SCANCODE_KP_HASH: return KeyboardKey::KeyPadHash;
    case SDL_SCANCODE_KP_SPACE: return KeyboardKey::KeyPadSpace;
    case SDL_SCANCODE_KP_AT: return KeyboardKey::KeyPadAt;
    case SDL_SCANCODE_KP_EXCLAM: return KeyboardKey::KeyPadExclamation;
    case SDL_SCANCODE_KP_MEMSTORE: return KeyboardKey::KeyPadMemoryStore;
    case SDL_SCANCODE_KP_MEMRECALL: return KeyboardKey::KeyPadMemoryRecall;
    case SDL_SCANCODE_KP_MEMCLEAR: return KeyboardKey::KeyPadMemoryClear;
    case SDL_SCANCODE_KP_MEMADD: return KeyboardKey::KeyPadMemoryAdd;
    case SDL_SCANCODE_KP_MEMSUBTRACT: return KeyboardKey::KeyPadMemorySubtract;
    case SDL_SCANCODE_KP_MEMMULTIPLY: return KeyboardKey::KeyPadMemoryMultiply;
    case SDL_SCANCODE_KP_MEMDIVIDE: return KeyboardKey::KeyPadMemoryDivide;
    case SDL_SCANCODE_KP_PLUSMINUS: return KeyboardKey::KeyPadPlusMinus;
    case SDL_SCANCODE_KP_CLEAR: return KeyboardKey::KeyPadClear;
    case SDL_SCANCODE_KP_CLEARENTRY: return KeyboardKey::KeyPadClearEntry;
    case SDL_SCANCODE_KP_BINARY: return KeyboardKey::KeyPadBinary;
    case SDL_SCANCODE_KP_OCTAL: return KeyboardKey::KeyPadOctal;
    case SDL_SCANCODE_KP_DECIMAL: return KeyboardKey::KeyPadDecimal;
    case SDL_SCANCODE_KP_HEXADECIMAL: return KeyboardKey::KeyPadHexadecimal;

    case SDL_SCANCODE_LCTRL: return KeyboardKey::LeftControl;
    case SDL_SCANCODE_LSHIFT: return KeyboardKey::LeftShift;
    case SDL_SCANCODE_LALT: return KeyboardKey::LeftAlt;
    case SDL_SCANCODE_LGUI: return KeyboardKey::LeftGui;
    case SDL_SCANCODE_RCTRL: return KeyboardKey::RightControl;
    case SDL_SCANCODE_RSHIFT: return KeyboardKey::RightShift;
    case SDL_SCANCODE_RALT: return KeyboardKey::RightAlt;
    case SDL_SCANCODE_RGUI: return KeyboardKey::RightGui;

    default: return KeyboardKey::None;
    }
}

SDL_Scancode KeyboardKeyToSDL3Scancode(KeyboardKey key) {
    switch (key) {
    case KeyboardKey::A: return SDL_SCANCODE_A;
    case KeyboardKey::B: return SDL_SCANCODE_B;
    case KeyboardKey::C: return SDL_SCANCODE_C;
    case KeyboardKey::D: return SDL_SCANCODE_D;
    case KeyboardKey::E: return SDL_SCANCODE_E;
    case KeyboardKey::F: return SDL_SCANCODE_F;
    case KeyboardKey::G: return SDL_SCANCODE_G;
    case KeyboardKey::H: return SDL_SCANCODE_H;
    case KeyboardKey::I: return SDL_SCANCODE_I;
    case KeyboardKey::J: return SDL_SCANCODE_J;
    case KeyboardKey::K: return SDL_SCANCODE_K;
    case KeyboardKey::L: return SDL_SCANCODE_L;
    case KeyboardKey::M: return SDL_SCANCODE_M;
    case KeyboardKey::N: return SDL_SCANCODE_N;
    case KeyboardKey::O: return SDL_SCANCODE_O;
    case KeyboardKey::P: return SDL_SCANCODE_P;
    case KeyboardKey::Q: return SDL_SCANCODE_Q;
    case KeyboardKey::R: return SDL_SCANCODE_R;
    case KeyboardKey::S: return SDL_SCANCODE_S;
    case KeyboardKey::T: return SDL_SCANCODE_T;
    case KeyboardKey::U: return SDL_SCANCODE_U;
    case KeyboardKey::V: return SDL_SCANCODE_V;
    case KeyboardKey::W: return SDL_SCANCODE_W;
    case KeyboardKey::X: return SDL_SCANCODE_X;
    case KeyboardKey::Y: return SDL_SCANCODE_Y;
    case KeyboardKey::Z: return SDL_SCANCODE_Z;

    case KeyboardKey::Alpha1: return SDL_SCANCODE_1;
    case KeyboardKey::Alpha2: return SDL_SCANCODE_2;
    case KeyboardKey::Alpha3: return SDL_SCANCODE_3;
    case KeyboardKey::Alpha4: return SDL_SCANCODE_4;
    case KeyboardKey::Alpha5: return SDL_SCANCODE_5;
    case KeyboardKey::Alpha6: return SDL_SCANCODE_6;
    case KeyboardKey::Alpha7: return SDL_SCANCODE_7;
    case KeyboardKey::Alpha8: return SDL_SCANCODE_8;
    case KeyboardKey::Alpha9: return SDL_SCANCODE_9;
    case KeyboardKey::Alpha0: return SDL_SCANCODE_0;

    case KeyboardKey::Return: return SDL_SCANCODE_RETURN;
    case KeyboardKey::Escape: return SDL_SCANCODE_ESCAPE;
    case KeyboardKey::Backspace: return SDL_SCANCODE_BACKSPACE;
    case KeyboardKey::Tab: return SDL_SCANCODE_TAB;
    case KeyboardKey::Spacebar: return SDL_SCANCODE_SPACE;

    case KeyboardKey::MinusUnderscore: return SDL_SCANCODE_MINUS;
    case KeyboardKey::EqualsPlus: return SDL_SCANCODE_EQUALS;
    case KeyboardKey::LeftBracket: return SDL_SCANCODE_LEFTBRACKET;
    case KeyboardKey::RightBracket: return SDL_SCANCODE_RIGHTBRACKET;
    case KeyboardKey::Backslash: return SDL_SCANCODE_BACKSLASH;
    case KeyboardKey::PoundTilde: return SDL_SCANCODE_NONUSHASH;
    case KeyboardKey::Semicolon: return SDL_SCANCODE_SEMICOLON;
    case KeyboardKey::Apostrophe: return SDL_SCANCODE_APOSTROPHE;
    case KeyboardKey::GraveAccent: return SDL_SCANCODE_GRAVE;
    case KeyboardKey::Comma: return SDL_SCANCODE_COMMA;
    case KeyboardKey::Period: return SDL_SCANCODE_PERIOD;
    case KeyboardKey::Slash: return SDL_SCANCODE_SLASH;

    case KeyboardKey::CapsLock: return SDL_SCANCODE_CAPSLOCK;

    case KeyboardKey::F1: return SDL_SCANCODE_F1;
    case KeyboardKey::F2: return SDL_SCANCODE_F2;
    case KeyboardKey::F3: return SDL_SCANCODE_F3;
    case KeyboardKey::F4: return SDL_SCANCODE_F4;
    case KeyboardKey::F5: return SDL_SCANCODE_F5;
    case KeyboardKey::F6: return SDL_SCANCODE_F6;
    case KeyboardKey::F7: return SDL_SCANCODE_F7;
    case KeyboardKey::F8: return SDL_SCANCODE_F8;
    case KeyboardKey::F9: return SDL_SCANCODE_F9;
    case KeyboardKey::F10: return SDL_SCANCODE_F10;
    case KeyboardKey::F11: return SDL_SCANCODE_F11;
    case KeyboardKey::F12: return SDL_SCANCODE_F12;

    case KeyboardKey::PrintScreen: return SDL_SCANCODE_PRINTSCREEN;
    case KeyboardKey::ScrollLock: return SDL_SCANCODE_SCROLLLOCK;
    case KeyboardKey::Pause: return SDL_SCANCODE_PAUSE;
    case KeyboardKey::Insert: return SDL_SCANCODE_INSERT;
    case KeyboardKey::Home: return SDL_SCANCODE_HOME;
    case KeyboardKey::PageUp: return SDL_SCANCODE_PAGEUP;
    case KeyboardKey::Delete: return SDL_SCANCODE_DELETE;
    case KeyboardKey::End: return SDL_SCANCODE_END;
    case KeyboardKey::PageDown: return SDL_SCANCODE_PAGEDOWN;
    case KeyboardKey::Right: return SDL_SCANCODE_RIGHT;
    case KeyboardKey::Left: return SDL_SCANCODE_LEFT;
    case KeyboardKey::Down: return SDL_SCANCODE_DOWN;
    case KeyboardKey::Up: return SDL_SCANCODE_UP;

    case KeyboardKey::NumLock: return SDL_SCANCODE_NUMLOCKCLEAR;
    case KeyboardKey::KeyPadDivide: return SDL_SCANCODE_KP_DIVIDE;
    case KeyboardKey::KeyPadMultiply: return SDL_SCANCODE_KP_MULTIPLY;
    case KeyboardKey::KeyPadSubtract: return SDL_SCANCODE_KP_MINUS;
    case KeyboardKey::KeyPadAdd: return SDL_SCANCODE_KP_PLUS;
    case KeyboardKey::KeyPadEnter: return SDL_SCANCODE_KP_ENTER;
    case KeyboardKey::KeyPad1: return SDL_SCANCODE_KP_1;
    case KeyboardKey::KeyPad2: return SDL_SCANCODE_KP_2;
    case KeyboardKey::KeyPad3: return SDL_SCANCODE_KP_3;
    case KeyboardKey::KeyPad4: return SDL_SCANCODE_KP_4;
    case KeyboardKey::KeyPad5: return SDL_SCANCODE_KP_5;
    case KeyboardKey::KeyPad6: return SDL_SCANCODE_KP_6;
    case KeyboardKey::KeyPad7: return SDL_SCANCODE_KP_7;
    case KeyboardKey::KeyPad8: return SDL_SCANCODE_KP_8;
    case KeyboardKey::KeyPad9: return SDL_SCANCODE_KP_9;
    case KeyboardKey::KeyPad0: return SDL_SCANCODE_KP_0;
    case KeyboardKey::KeyPadPeriod: return SDL_SCANCODE_KP_PERIOD;

    case KeyboardKey::NonUSBackslash: return SDL_SCANCODE_NONUSBACKSLASH;
    case KeyboardKey::Application: return SDL_SCANCODE_APPLICATION;
    case KeyboardKey::Power: return SDL_SCANCODE_POWER;
    case KeyboardKey::KeyPadEquals: return SDL_SCANCODE_KP_EQUALS;
    case KeyboardKey::F13: return SDL_SCANCODE_F13;
    case KeyboardKey::F14: return SDL_SCANCODE_F14;
    case KeyboardKey::F15: return SDL_SCANCODE_F15;
    case KeyboardKey::F16: return SDL_SCANCODE_F16;
    case KeyboardKey::F17: return SDL_SCANCODE_F17;
    case KeyboardKey::F18: return SDL_SCANCODE_F18;
    case KeyboardKey::F19: return SDL_SCANCODE_F19;
    case KeyboardKey::F20: return SDL_SCANCODE_F20;
    case KeyboardKey::F21: return SDL_SCANCODE_F21;
    case KeyboardKey::F22: return SDL_SCANCODE_F22;
    case KeyboardKey::F23: return SDL_SCANCODE_F23;
    case KeyboardKey::F24: return SDL_SCANCODE_F24;
    case KeyboardKey::Execute: return SDL_SCANCODE_EXECUTE;
    case KeyboardKey::Help: return SDL_SCANCODE_HELP;
    case KeyboardKey::Menu: return SDL_SCANCODE_MENU;
    case KeyboardKey::Select: return SDL_SCANCODE_SELECT;
    case KeyboardKey::Stop: return SDL_SCANCODE_STOP;
    case KeyboardKey::Again: return SDL_SCANCODE_AGAIN;
    case KeyboardKey::Undo: return SDL_SCANCODE_UNDO;
    case KeyboardKey::Cut: return SDL_SCANCODE_CUT;
    case KeyboardKey::Copy: return SDL_SCANCODE_COPY;
    case KeyboardKey::Paste: return SDL_SCANCODE_PASTE;
    case KeyboardKey::Find: return SDL_SCANCODE_FIND;
    case KeyboardKey::Mute: return SDL_SCANCODE_MUTE;
    case KeyboardKey::VolumeUp: return SDL_SCANCODE_VOLUMEUP;
    case KeyboardKey::VolumeDown: return SDL_SCANCODE_VOLUMEDOWN;
    case KeyboardKey::KeyPadComma: return SDL_SCANCODE_KP_COMMA;
    case KeyboardKey::KeyPadEqualSign: return SDL_SCANCODE_KP_EQUALSAS400;

    case KeyboardKey::Intl1: return SDL_SCANCODE_INTERNATIONAL1;
    case KeyboardKey::Intl2: return SDL_SCANCODE_INTERNATIONAL2;
    case KeyboardKey::Intl3: return SDL_SCANCODE_INTERNATIONAL3;
    case KeyboardKey::Intl4: return SDL_SCANCODE_INTERNATIONAL4;
    case KeyboardKey::Intl5: return SDL_SCANCODE_INTERNATIONAL5;
    case KeyboardKey::Intl6: return SDL_SCANCODE_INTERNATIONAL6;
    case KeyboardKey::Intl7: return SDL_SCANCODE_INTERNATIONAL7;
    case KeyboardKey::Intl8: return SDL_SCANCODE_INTERNATIONAL8;
    case KeyboardKey::Intl9: return SDL_SCANCODE_INTERNATIONAL9;
    case KeyboardKey::LANG1: return SDL_SCANCODE_LANG1;
    case KeyboardKey::LANG2: return SDL_SCANCODE_LANG2;
    case KeyboardKey::LANG3: return SDL_SCANCODE_LANG3;
    case KeyboardKey::LANG4: return SDL_SCANCODE_LANG4;
    case KeyboardKey::LANG5: return SDL_SCANCODE_LANG5;
    case KeyboardKey::LANG6: return SDL_SCANCODE_LANG6;
    case KeyboardKey::LANG7: return SDL_SCANCODE_LANG7;
    case KeyboardKey::LANG8: return SDL_SCANCODE_LANG8;
    case KeyboardKey::LANG9: return SDL_SCANCODE_LANG9;

    case KeyboardKey::AltErase: return SDL_SCANCODE_ALTERASE;
    case KeyboardKey::SysReq: return SDL_SCANCODE_SYSREQ;
    case KeyboardKey::Cancel: return SDL_SCANCODE_CANCEL;
    case KeyboardKey::Clear: return SDL_SCANCODE_CLEAR;
    case KeyboardKey::Prior: return SDL_SCANCODE_PRIOR;
    case KeyboardKey::Return2: return SDL_SCANCODE_RETURN2;
    case KeyboardKey::Separator: return SDL_SCANCODE_SEPARATOR;
    case KeyboardKey::Out: return SDL_SCANCODE_OUT;
    case KeyboardKey::Oper: return SDL_SCANCODE_OPER;
    case KeyboardKey::ClearAgain: return SDL_SCANCODE_CLEARAGAIN;
    case KeyboardKey::CrSelProps: return SDL_SCANCODE_CRSEL;
    case KeyboardKey::ExSel: return SDL_SCANCODE_EXSEL;

    case KeyboardKey::KeyPad00: return SDL_SCANCODE_KP_00;
    case KeyboardKey::KeyPad000: return SDL_SCANCODE_KP_000;
    case KeyboardKey::ThousandsSeparator: return SDL_SCANCODE_THOUSANDSSEPARATOR;
    case KeyboardKey::DecimalSeparator: return SDL_SCANCODE_DECIMALSEPARATOR;
    case KeyboardKey::CurrencyUnit: return SDL_SCANCODE_CURRENCYUNIT;
    case KeyboardKey::CurrencySubUnit: return SDL_SCANCODE_CURRENCYSUBUNIT;
    case KeyboardKey::KeyPadLeftParenthesis: return SDL_SCANCODE_KP_LEFTPAREN;
    case KeyboardKey::KeyPadRightParenthesis: return SDL_SCANCODE_KP_RIGHTPAREN;
    case KeyboardKey::KeyPadLeftBrace: return SDL_SCANCODE_KP_LEFTBRACE;
    case KeyboardKey::KeyPadRightBrace: return SDL_SCANCODE_KP_RIGHTBRACE;
    case KeyboardKey::KeyPadTab: return SDL_SCANCODE_KP_TAB;
    case KeyboardKey::KeyPadBackspace: return SDL_SCANCODE_KP_BACKSPACE;
    case KeyboardKey::KeyPadA: return SDL_SCANCODE_KP_A;
    case KeyboardKey::KeyPadB: return SDL_SCANCODE_KP_B;
    case KeyboardKey::KeyPadC: return SDL_SCANCODE_KP_C;
    case KeyboardKey::KeyPadD: return SDL_SCANCODE_KP_D;
    case KeyboardKey::KeyPadE: return SDL_SCANCODE_KP_E;
    case KeyboardKey::KeyPadF: return SDL_SCANCODE_KP_F;
    case KeyboardKey::KeyPadXOR: return SDL_SCANCODE_KP_XOR;
    case KeyboardKey::KeyPadPower: return SDL_SCANCODE_KP_POWER;
    case KeyboardKey::KeyPadPercent: return SDL_SCANCODE_KP_PERCENT;
    case KeyboardKey::KeyPadLess: return SDL_SCANCODE_KP_LESS;
    case KeyboardKey::KeyPadGreater: return SDL_SCANCODE_KP_GREATER;
    case KeyboardKey::KeyPadAmpersand: return SDL_SCANCODE_KP_AMPERSAND;
    case KeyboardKey::KeyPadDoubleAmpersand: return SDL_SCANCODE_KP_DBLAMPERSAND;
    case KeyboardKey::KeyPadVerticalBar: return SDL_SCANCODE_KP_VERTICALBAR;
    case KeyboardKey::KeyPadDoubleVerticalBar: return SDL_SCANCODE_KP_DBLVERTICALBAR;
    case KeyboardKey::KeyPadColon: return SDL_SCANCODE_KP_COLON;
    case KeyboardKey::KeyPadHash: return SDL_SCANCODE_KP_HASH;
    case KeyboardKey::KeyPadSpace: return SDL_SCANCODE_KP_SPACE;
    case KeyboardKey::KeyPadAt: return SDL_SCANCODE_KP_AT;
    case KeyboardKey::KeyPadExclamation: return SDL_SCANCODE_KP_EXCLAM;
    case KeyboardKey::KeyPadMemoryStore: return SDL_SCANCODE_KP_MEMSTORE;
    case KeyboardKey::KeyPadMemoryRecall: return SDL_SCANCODE_KP_MEMRECALL;
    case KeyboardKey::KeyPadMemoryClear: return SDL_SCANCODE_KP_MEMCLEAR;
    case KeyboardKey::KeyPadMemoryAdd: return SDL_SCANCODE_KP_MEMADD;
    case KeyboardKey::KeyPadMemorySubtract: return SDL_SCANCODE_KP_MEMSUBTRACT;
    case KeyboardKey::KeyPadMemoryMultiply: return SDL_SCANCODE_KP_MEMMULTIPLY;
    case KeyboardKey::KeyPadMemoryDivide: return SDL_SCANCODE_KP_MEMDIVIDE;
    case KeyboardKey::KeyPadPlusMinus: return SDL_SCANCODE_KP_PLUSMINUS;
    case KeyboardKey::KeyPadClear: return SDL_SCANCODE_KP_CLEAR;
    case KeyboardKey::KeyPadClearEntry: return SDL_SCANCODE_KP_CLEARENTRY;
    case KeyboardKey::KeyPadBinary: return SDL_SCANCODE_KP_BINARY;
    case KeyboardKey::KeyPadOctal: return SDL_SCANCODE_KP_OCTAL;
    case KeyboardKey::KeyPadDecimal: return SDL_SCANCODE_KP_DECIMAL;
    case KeyboardKey::KeyPadHexadecimal: return SDL_SCANCODE_KP_HEXADECIMAL;

    case KeyboardKey::LeftControl: return SDL_SCANCODE_LCTRL;
    case KeyboardKey::LeftShift: return SDL_SCANCODE_LSHIFT;
    case KeyboardKey::LeftAlt: return SDL_SCANCODE_LALT;
    case KeyboardKey::LeftGui: return SDL_SCANCODE_LGUI;
    case KeyboardKey::RightControl: return SDL_SCANCODE_RCTRL;
    case KeyboardKey::RightShift: return SDL_SCANCODE_RSHIFT;
    case KeyboardKey::RightAlt: return SDL_SCANCODE_RALT;
    case KeyboardKey::RightGui: return SDL_SCANCODE_RGUI;

    default: return SDL_SCANCODE_UNKNOWN;
    }
}

KeyModifier SDL3ToKeyModifier(SDL_Keymod modifier) {
    KeyModifier out = KeyModifier::None;
    if (modifier & SDL_KMOD_ALT) {
        out |= KeyModifier::Alt;
    }
    if (modifier & SDL_KMOD_CTRL) {
        out |= KeyModifier::Control;
    }
    if (modifier & SDL_KMOD_SHIFT) {
        out |= KeyModifier::Shift;
    }
    if (modifier & SDL_KMOD_GUI) {
        out |= KeyModifier::Super;
    }
    return out;
}

SDL_Keymod KeyModifierToSDL3(KeyModifier modifier) {
    SDL_Keymod out = SDL_KMOD_NONE;
    auto bmMod = BitmaskEnum(modifier);
    if (bmMod.AnyOf(KeyModifier::Alt)) {
        out |= SDL_KMOD_ALT;
    }
    if (bmMod.AnyOf(KeyModifier::Control)) {
        out |= SDL_KMOD_CTRL;
    }
    if (bmMod.AnyOf(KeyModifier::Shift)) {
        out |= SDL_KMOD_SHIFT;
    }
    if (bmMod.AnyOf(KeyModifier::Super)) {
        out |= SDL_KMOD_GUI;
    }
    return out;
}

MouseButton SDL3ToMouseButton(Uint8 button) {
    switch (button) {
    case SDL_BUTTON_LEFT: return MouseButton::Left;
    case SDL_BUTTON_RIGHT: return MouseButton::Right;
    case SDL_BUTTON_MIDDLE: return MouseButton::Middle;
    case SDL_BUTTON_X1: return MouseButton::Extra1;
    case SDL_BUTTON_X2: return MouseButton::Extra2;
    default: return MouseButton::None;
    }
}

Uint8 MouseButtonToSDL3(MouseButton button) {
    switch (button) {
    case MouseButton::Left: return SDL_BUTTON_LEFT;
    case MouseButton::Right: return SDL_BUTTON_RIGHT;
    case MouseButton::Middle: return SDL_BUTTON_MIDDLE;
    case MouseButton::Extra1: return SDL_BUTTON_X1;
    case MouseButton::Extra2: return SDL_BUTTON_X2;
    default: return 0;
    }
}

GamepadButton SDL3ToGamepadButton(SDL_GamepadButton button) {
    switch (button) {
    case SDL_GAMEPAD_BUTTON_SOUTH: return GamepadButton::A;
    case SDL_GAMEPAD_BUTTON_EAST: return GamepadButton::B;
    case SDL_GAMEPAD_BUTTON_WEST: return GamepadButton::X;
    case SDL_GAMEPAD_BUTTON_NORTH: return GamepadButton::Y;
    case SDL_GAMEPAD_BUTTON_BACK: return GamepadButton::Back;
    case SDL_GAMEPAD_BUTTON_GUIDE: return GamepadButton::Guide;
    case SDL_GAMEPAD_BUTTON_START: return GamepadButton::Start;
    case SDL_GAMEPAD_BUTTON_LEFT_STICK: return GamepadButton::LeftThumb;
    case SDL_GAMEPAD_BUTTON_RIGHT_STICK: return GamepadButton::RightThumb;
    case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER: return GamepadButton::LeftBumper;
    case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER: return GamepadButton::RightBumper;
    case SDL_GAMEPAD_BUTTON_DPAD_UP: return GamepadButton::DpadUp;
    case SDL_GAMEPAD_BUTTON_DPAD_DOWN: return GamepadButton::DpadDown;
    case SDL_GAMEPAD_BUTTON_DPAD_LEFT: return GamepadButton::DpadLeft;
    case SDL_GAMEPAD_BUTTON_DPAD_RIGHT: return GamepadButton::DpadRight;
    case SDL_GAMEPAD_BUTTON_MISC1: return GamepadButton::Misc1;
    case SDL_GAMEPAD_BUTTON_RIGHT_PADDLE1: return GamepadButton::RightPaddle1;
    case SDL_GAMEPAD_BUTTON_LEFT_PADDLE1: return GamepadButton::LeftPaddle1;
    case SDL_GAMEPAD_BUTTON_RIGHT_PADDLE2: return GamepadButton::RightPaddle2;
    case SDL_GAMEPAD_BUTTON_LEFT_PADDLE2: return GamepadButton::LeftPaddle2;
    case SDL_GAMEPAD_BUTTON_TOUCHPAD: return GamepadButton::TouchPad;
    case SDL_GAMEPAD_BUTTON_MISC2: return GamepadButton::Misc2;
    case SDL_GAMEPAD_BUTTON_MISC3: return GamepadButton::Misc3;
    case SDL_GAMEPAD_BUTTON_MISC4: return GamepadButton::Misc4;
    case SDL_GAMEPAD_BUTTON_MISC5: return GamepadButton::Misc5;
    case SDL_GAMEPAD_BUTTON_MISC6: return GamepadButton::Misc6;
    default: return GamepadButton::None;
    }
}

SDL_GamepadButton GamepadButtonToSDL3(GamepadButton button) {
    switch (button) {
    case GamepadButton::A: return SDL_GAMEPAD_BUTTON_SOUTH;
    case GamepadButton::B: return SDL_GAMEPAD_BUTTON_EAST;
    case GamepadButton::X: return SDL_GAMEPAD_BUTTON_WEST;
    case GamepadButton::Y: return SDL_GAMEPAD_BUTTON_NORTH;
    case GamepadButton::Back: return SDL_GAMEPAD_BUTTON_BACK;
    case GamepadButton::Guide: return SDL_GAMEPAD_BUTTON_GUIDE;
    case GamepadButton::Start: return SDL_GAMEPAD_BUTTON_START;
    case GamepadButton::LeftThumb: return SDL_GAMEPAD_BUTTON_LEFT_STICK;
    case GamepadButton::RightThumb: return SDL_GAMEPAD_BUTTON_RIGHT_STICK;
    case GamepadButton::LeftBumper: return SDL_GAMEPAD_BUTTON_LEFT_SHOULDER;
    case GamepadButton::RightBumper: return SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER;
    case GamepadButton::DpadUp: return SDL_GAMEPAD_BUTTON_DPAD_UP;
    case GamepadButton::DpadDown: return SDL_GAMEPAD_BUTTON_DPAD_DOWN;
    case GamepadButton::DpadLeft: return SDL_GAMEPAD_BUTTON_DPAD_LEFT;
    case GamepadButton::DpadRight: return SDL_GAMEPAD_BUTTON_DPAD_RIGHT;
    case GamepadButton::Misc1: return SDL_GAMEPAD_BUTTON_MISC1;
    case GamepadButton::RightPaddle1: return SDL_GAMEPAD_BUTTON_RIGHT_PADDLE1;
    case GamepadButton::LeftPaddle1: return SDL_GAMEPAD_BUTTON_LEFT_PADDLE1;
    case GamepadButton::RightPaddle2: return SDL_GAMEPAD_BUTTON_RIGHT_PADDLE2;
    case GamepadButton::LeftPaddle2: return SDL_GAMEPAD_BUTTON_LEFT_PADDLE2;
    case GamepadButton::TouchPad: return SDL_GAMEPAD_BUTTON_TOUCHPAD;
    case GamepadButton::Misc2: return SDL_GAMEPAD_BUTTON_MISC2;
    case GamepadButton::Misc3: return SDL_GAMEPAD_BUTTON_MISC3;
    case GamepadButton::Misc4: return SDL_GAMEPAD_BUTTON_MISC4;
    case GamepadButton::Misc5: return SDL_GAMEPAD_BUTTON_MISC5;
    case GamepadButton::Misc6: return SDL_GAMEPAD_BUTTON_MISC6;
    default: return SDL_GAMEPAD_BUTTON_INVALID;
    }
}

GamepadAxis1D SDL3ToGamepadAxis1D(SDL_GamepadAxis axis) {
    switch (axis) {
    case SDL_GAMEPAD_AXIS_LEFTX: return GamepadAxis1D::LeftStickX;
    case SDL_GAMEPAD_AXIS_LEFTY: return GamepadAxis1D::LeftStickY;
    case SDL_GAMEPAD_AXIS_RIGHTX: return GamepadAxis1D::RightStickX;
    case SDL_GAMEPAD_AXIS_RIGHTY: return GamepadAxis1D::RightStickY;
    case SDL_GAMEPAD_AXIS_LEFT_TRIGGER: return GamepadAxis1D::LeftTrigger;
    case SDL_GAMEPAD_AXIS_RIGHT_TRIGGER: return GamepadAxis1D::RightTrigger;
    default: return GamepadAxis1D::None;
    }
}

SDL_GamepadAxis GamepadAxis1DToSDL3(GamepadAxis1D axis) {
    switch (axis) {
    case GamepadAxis1D::LeftStickX: return SDL_GAMEPAD_AXIS_LEFTX;
    case GamepadAxis1D::LeftStickY: return SDL_GAMEPAD_AXIS_LEFTY;
    case GamepadAxis1D::RightStickX: return SDL_GAMEPAD_AXIS_RIGHTX;
    case GamepadAxis1D::RightStickY: return SDL_GAMEPAD_AXIS_RIGHTY;
    case GamepadAxis1D::LeftTrigger: return SDL_GAMEPAD_AXIS_LEFT_TRIGGER;
    case GamepadAxis1D::RightTrigger: return SDL_GAMEPAD_AXIS_RIGHT_TRIGGER;
    default: return SDL_GAMEPAD_AXIS_INVALID;
    }
}

GamepadAxis2D SDL3ToGamepadAxis2D(SDL_GamepadAxis axisH, SDL_GamepadAxis axisV) {
    if (axisH == SDL_GAMEPAD_AXIS_LEFTX && axisV == SDL_GAMEPAD_AXIS_LEFTY) {
        return GamepadAxis2D::LeftStick;
    } else if (axisH == SDL_GAMEPAD_AXIS_RIGHTX && axisV == SDL_GAMEPAD_AXIS_RIGHTY) {
        return GamepadAxis2D::RightStick;
    } else {
        return GamepadAxis2D::None;
    }
}

std::pair<SDL_GamepadAxis, SDL_GamepadAxis> GamepadAxis2DToSDL3(GamepadAxis2D axis) {
    switch (axis) {
    case GamepadAxis2D::LeftStick: return {SDL_GAMEPAD_AXIS_LEFTX, SDL_GAMEPAD_AXIS_LEFTY};
    case GamepadAxis2D::RightStick: return {SDL_GAMEPAD_AXIS_RIGHTX, SDL_GAMEPAD_AXIS_RIGHTY};
    default: return {SDL_GAMEPAD_AXIS_INVALID, SDL_GAMEPAD_AXIS_INVALID};
    }
}

SDL_GamepadAxis SDL3GetOrthogonal2DAxis(SDL_GamepadAxis axis) {
    switch (axis) {
    case SDL_GAMEPAD_AXIS_LEFTX: return SDL_GAMEPAD_AXIS_LEFTY;
    case SDL_GAMEPAD_AXIS_LEFTY: return SDL_GAMEPAD_AXIS_LEFTX;
    case SDL_GAMEPAD_AXIS_RIGHTX: return SDL_GAMEPAD_AXIS_RIGHTY;
    case SDL_GAMEPAD_AXIS_RIGHTY: return SDL_GAMEPAD_AXIS_RIGHTX;
    default: return SDL_GAMEPAD_AXIS_INVALID;
    }
}

} // namespace app::input
