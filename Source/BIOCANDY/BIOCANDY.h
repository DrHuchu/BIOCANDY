// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#define CALLINFO (FString(__FUNCTION__) + TEXT("(")+ FString::FromInt(__LINE__) +TEXT(")"))
#define PRINT_CALLINFO() UE_LOG(LogTemp, Warning, TEXT("%s"),*CALLINFO) //치환
#define PRINT_LOG(fmt, ...) UE_LOG(LogTemp, Warning, TEXT("%s %s"),*CALLINFO, *FString::Printf(fmt, ##__VA_ARGS__)) //가변인수 매크로 __VA_ARGS__