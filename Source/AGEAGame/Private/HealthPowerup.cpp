// Fill out your copyright notice in the Description page of Project Settings.

#include "AGEAGame.h"
#include "HealthPowerup.h"


AHealthPowerup::AHealthPowerup(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	UpdateHealthValue = 20.f;
}

void AHealthPowerup::OnPickedUp_Implementation()
{
	// Call the implementation of this function
	Super::OnPickedUp_Implementation();

	// Destroy the actor when the pickup is collected
	Destroy();

	FVector ActorLocation = GetActorLocation();
	PlaySoundAtLocation(SoundToPlay, ActorLocation);
}

