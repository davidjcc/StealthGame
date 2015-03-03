// Fill out your copyright notice in the Description page of Project Settings.

#include "AGEAGame.h"
#include "HealthPowerup.h"
#include "AGEAGameCharacter.h"


AHealthPowerup::AHealthPowerup(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	UpdateHealthValue = 20.f;
}
