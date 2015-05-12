// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "StealthGameGameMode.generated.h"

UCLASS(minimalapi)
class AStealthGameGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AStealthGameGameMode(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ActivatedLevels = 1;	
};

