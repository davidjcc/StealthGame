// Fill out your copyright notice in the Description page of Project Settings.

#include "StealthGame.h"
#include "TeleportGadget.h"




ATeleportGadget::ATeleportGadget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GadgetType = EGadgetType::ETELEPORT;

	MovementComponent->InitialSpeed = 2000.0f;
	MovementComponent->MaxSpeed = 2000.0f;
}
