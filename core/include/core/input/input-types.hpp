#pragma once

/**
 * @file core/input/input-types.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 * 
 * @addtogroup Input
 * @{
 */

#include <string>
#include <stdexcept>

#include "SDL2/SDL_events.h"

/** Enum class for Input Sources */
enum class InputTypes {
    Keyboard,
    Mouse,
    Joystick,
    Controller,
    Sensor,
    Touch
};

/** Enum class of Input Event types */
enum class InputEventType
{
    KeyDown                  = SDL_KEYDOWN,
    KeyUp                    = SDL_KEYUP,
    TextEditing              = SDL_TEXTEDITING,
    TextInput                = SDL_TEXTINPUT,
    KeyMapChanged            = SDL_KEYMAPCHANGED,
    MouseMotion              = SDL_MOUSEMOTION,
    MouseButtonDown          = SDL_MOUSEBUTTONDOWN,
    MouseButtonUp            = SDL_MOUSEBUTTONUP,
    MouseWheel               = SDL_MOUSEWHEEL,
    JoyAxisMotion            = SDL_JOYAXISMOTION,
    JoyBallMotion            = SDL_JOYBALLMOTION,
    JoyHatMotion             = SDL_JOYHATMOTION,
    JoyButtonDown            = SDL_JOYBUTTONDOWN,
    JoyButtonUp              = SDL_JOYBUTTONUP,
    JoyDeviceAdded           = SDL_JOYDEVICEADDED,
    JoyDeviceRemoved         = SDL_JOYDEVICEREMOVED,
    ControllerAxisMotion     = SDL_CONTROLLERAXISMOTION,
    ControllerButtonDown     = SDL_CONTROLLERBUTTONDOWN,
    ControllerButtonUp       = SDL_CONTROLLERBUTTONUP,
    ControllerDeviceAdded    = SDL_CONTROLLERDEVICEADDED,
    ControllerDeviceRemoved  = SDL_CONTROLLERDEVICEREMOVED,
    ControllerDeviceRemapped = SDL_CONTROLLERDEVICEREMAPPED,
    FingerDown               = SDL_FINGERDOWN,
    FingerUp                 = SDL_FINGERUP,
    FingerMotion             = SDL_FINGERMOTION,
    DollarGesture            = SDL_DOLLARGESTURE,
    DollarRecord             = SDL_DOLLARRECORD,
    MultiGesture             = SDL_MULTIGESTURE,
    ClipboardUpdate          = SDL_CLIPBOARDUPDATE,
    DropFile                 = SDL_DROPFILE,
    DropText                 = SDL_DROPTEXT,
    DropBegin                = SDL_DROPBEGIN,
    DropComplete             = SDL_DROPCOMPLETE
};

/** InputEventType enum to string conversion ( aka maintenance hell) */
constexpr inline const char *to_string(const InputEventType &e)
{
    switch (e)
    {
    case InputEventType::KeyDown: return "InputEventType::KeyDown";
    case InputEventType::KeyUp: return "InputEventType::KeyUp";
    case InputEventType::TextEditing: return "InputEventType::TextEditing";
    case InputEventType::TextInput: return "InputEventType::TextInput";
    case InputEventType::KeyMapChanged: return "InputEventType::KeyMapChanged";
    case InputEventType::MouseMotion: return "InputEventType::MouseMotion";
    case InputEventType::MouseButtonDown: return "InputEventType::MouseButtonDown";
    case InputEventType::MouseButtonUp: return "InputEventType::MouseButtonUp";
    case InputEventType::MouseWheel: return "InputEventType::MouseWheel";
    case InputEventType::JoyAxisMotion: return "InputEventType::JoyAxisMotion";
    case InputEventType::JoyBallMotion: return "InputEventType::JoyBallMotion";
    case InputEventType::JoyHatMotion: return "InputEventType::JoyHatMotion";
    case InputEventType::JoyButtonDown: return "InputEventType::JoyButtonDown";
    case InputEventType::JoyButtonUp: return "InputEventType::JoyButtonUp";
    case InputEventType::JoyDeviceAdded: return "InputEventType::JoyDeviceAdded";
    case InputEventType::JoyDeviceRemoved: return "InputEventType::JoyDeviceRemoved";
    case InputEventType::ControllerAxisMotion: return "InputEventType::ControllerAxisMotion";
    case InputEventType::ControllerButtonDown: return "InputEventType::ControllerButtonDown";
    case InputEventType::ControllerButtonUp: return "InputEventType::ControllerButtonUp";
    case InputEventType::ControllerDeviceAdded: return "InputEventType::ControllerDeviceAdded";
    case InputEventType::ControllerDeviceRemoved: return "InputEventType::ControllerDeviceRemoved";
    case InputEventType::ControllerDeviceRemapped: return "InputEventType::ControllerDeviceRemapped";
    case InputEventType::FingerDown: return "InputEventType::FingerDown";
    case InputEventType::FingerUp: return "InputEventType::FingerUp";
    case InputEventType::FingerMotion: return "InputEventType::FingerMotion";
    case InputEventType::DollarGesture: return "InputEventType::DollarGesture";
    case InputEventType::DollarRecord: return "InputEventType::DollarRecord";
    case InputEventType::MultiGesture: return "InputEventType::MultiGesture";
    case InputEventType::ClipboardUpdate: return "InputEventType::ClipboardUpdate";
    case InputEventType::DropFile: return "InputEventType::DropFile";
    case InputEventType::DropText: return "InputEventType::DropText";
    case InputEventType::DropBegin: return "InputEventType::DropBegin";
    case InputEventType::DropComplete: return "InputEventType::DropComplete";
    default: throw std::invalid_argument("Unimplemented enum to string conversion");
    }
}

/** @} endgroup Input */
