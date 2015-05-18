// Fill out your copyright notice in the Description page of Project Settings.

#include "StealthGame.h"
#include "StealthGameInstance.h"


UStealthGameInstance::UStealthGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UStealthGameInstance::SetLevelsActivated(int32 pLevelsActivated)
{
	LevelsActivated = pLevelsActivated;
}


