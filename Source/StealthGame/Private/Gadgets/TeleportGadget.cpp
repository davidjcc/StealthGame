// Fill out your copyright notice in the Description page of Project Settings.

#include "StealthGame.h"
#include "TeleportGadget.h"




ATeleportGadget::ATeleportGadget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, "Mesh");
	Mesh->AttachTo(RootComponent);
}
