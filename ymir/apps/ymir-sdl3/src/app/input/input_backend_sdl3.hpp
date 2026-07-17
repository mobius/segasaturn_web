#pragma once

#include "input_primitives.hpp"

#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_scancode.h>

#include <ymir/core/types.hpp>

#include <utility>

namespace app::input {

KeyboardKey SDL3ScancodeToKeyboardKey(SDL_Scancode key);
SDL_Scancode KeyboardKeyToSDL3Scancode(KeyboardKey key);

KeyModifier SDL3ToKeyModifier(SDL_Keymod modifier);
SDL_Keymod KeyModifierToSDL3(KeyModifier modifier);

MouseButton SDL3ToMouseButton(Uint8 button);
Uint8 MouseButtonToSDL3(MouseButton button);

GamepadButton SDL3ToGamepadButton(SDL_GamepadButton button);
SDL_GamepadButton GamepadButtonToSDL3(GamepadButton button);

GamepadAxis1D SDL3ToGamepadAxis1D(SDL_GamepadAxis axis);
SDL_GamepadAxis GamepadAxis1DToSDL3(GamepadAxis1D axis);

GamepadAxis2D SDL3ToGamepadAxis2D(SDL_GamepadAxis axisH, SDL_GamepadAxis axisV);
std::pair<SDL_GamepadAxis, SDL_GamepadAxis> GamepadAxis2DToSDL3(GamepadAxis2D axis);

SDL_GamepadAxis SDL3GetOrthogonal2DAxis(SDL_GamepadAxis axis);

} // namespace app::input
