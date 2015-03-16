// Fill out your copyright notice in the Description page of Project Settings.

#include "AGEAGame.h"
#include "AIGuardCharacter.h"

AAIGuardCharacter::AAIGuardCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	WalkSpeed = 200.f;
	RunSpeed = 600.f;
}

void AAIGuardCharacter::GuardUpdateHealth(float UpdateHealthValue)
{
	Health += UpdateHealthValue;

	if (Health <= 0)
	{
		this->Destroy();
	}
}

