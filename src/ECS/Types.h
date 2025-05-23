#pragma once
#include <bitset>
#include <cstdint>

#define ECS_BEGIN namespace ECS {
#define ECS_END }

ECS_BEGIN

// Source: https://gist.github.com/Lee-R/3839813
constexpr std::uint32_t fnv1a_32(char const* s, std::size_t count)
{
	return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u; // NOLINT (hicpp-signed-bitwise)
}

constexpr std::uint32_t operator "" _hash(char const* s, std::size_t count)
{
	return fnv1a_32(s, count);
}

//ECS
using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 5000;
using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;

// Input
enum class InputButtons
{
	W,
	A,
	S,
	D,
	Q,
	E
};



//Event
using EventType = std::uint32_t;
using ParamId = std::uint32_t;

#define METHOD_LISTENER(EventType, Listener) EventType, std::bind(&Listener, this, std::placeholders::_1)
#define FUNCTION_LISTENER(EventType, Listener) EventType, std::bind(&Listener, std::placeholders::_1)

namespace Events::Window {
	const EventType QUIT = "Events::Window::QUIT"_hash;
	const EventType RESIZED = "Events::Window::RESIZED"_hash;
	const EventType INPUT = "Events::Window::INPUT"_hash;
}

namespace Events::Window::Input {
	const ParamId INPUT = "Events::Window::Input::INPUT"_hash;
}

namespace Events::Window::Resized {
	const ParamId WIDTH = "Events::Window::Resized::WIDTH"_hash;
	const ParamId HEIGHT = "Events::Window::Resized::HEIGHT"_hash;
}

ECS_END