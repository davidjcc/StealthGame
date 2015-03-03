// Fill out your copyright notice in the Description page of Project Settings.

#include "AGEAGame.h"
#include "StealthPowerup.h"
#include "AGEAGameCharacter.h"

AStealthPowerup::AStealthPowerup(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	UpdateStealthValue = 10.0f;
}


