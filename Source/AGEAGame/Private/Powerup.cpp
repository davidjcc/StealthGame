// Fill out your copyright notice in the Description page of Project Settings.

#include "AGEAGame.h"
#include "Powerup.h"


APowerup::APowerup(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Allow the actor to tick
	PrimaryActorTick.bCanEverTick = true;

	// Active when the pickup is created
	bIsActive = true;

	// Create the root CollisionSphere to handle the pickup's collision
	CollisionSphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, "Collision Sphere");
	// Set the sphere component as the root component
	RootComponent = CollisionSphere;

	// Create the static mesh component
	PickupMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, "Pickup Mesh");

	// Attach the static mesh component to the root component
	PickupMesh->AttachParent = RootComponent;
}