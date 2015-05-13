// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "LevelChangeTrigger.h"
#include "StealthGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHGAME_API UStealthGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UStealthGameInstance(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(BlueprintReadWrite)
	int32 LevelsActivated = 0;

	UFUNCTION(BlueprintCallable, Category = LevelsActivated)
	void IncrementLevelsActivated();
};
