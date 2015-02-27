// Fill out your copyright notice in the Description page of Project Settings.

#include "AGEAGame.h"
#include "StealthPowerup.h"

//AStealthPowerup::AStealthPowerup(const FObjectInitializer& ObjectInitializer)
//: Super(ObjectInitializer)
//{
//	UpdateStealthValue = 10.0f;
//}


void AStealthPowerup ::OnPickedUp_Implementation()
{
	// Call the implementation of this function
	Super::OnPickedUp_Implementation();

	// Destroy the actor when the pickup is collected
	Destroy();

	UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
}



