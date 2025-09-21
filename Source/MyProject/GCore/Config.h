#pragma once

#include "Config/FlightConfigs.h"
#include "Config/EnvConfigs.h"

#define PRINTSCREEN(Text) \
if (GEngine) { \
GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Text); \
}

// Variant with printf-style formatting
#define PRINTSCREENF(Format, ...) \
if (GEngine) { \
GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(Format, ##__VA_ARGS__)); \
}


class Config
{
public:
	
};
