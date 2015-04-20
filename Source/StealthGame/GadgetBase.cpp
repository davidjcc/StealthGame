// Fill out your copyright notice in the Description page of Project Settings.

#include "StealthGame.h"
#include "GadgetBase.h"


// Sets default values
AGadgetBase::AGadgetBase(const class FObjectInitializer & ObjectInit) : Super(ObjectInit)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = ObjectInit.CreateDefaultSubobject<USphereComponent>(this, "CollisionComponent");
	CollisionComp->SetSphereRadius(50.f);
	RootComponent = CollisionComp;
	
	Mesh = ObjectInit.CreateDefaultSubobject<UStaticMeshComponent>(this, "Mesh");
	Mesh->AttachTo(RootComponent);

}

void AGadgetBase::SetOwner(AStealthGameCharacter* NewPawn)
{
	if (NewPawn != MyPawn)
	{
		MyPawn = NewPawn;
	}
}

