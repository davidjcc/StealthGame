// Fill out your copyright notice in the Description page of Project Settings.

#include "StealthGame.h"
#include "GadgetBase.h"
#include "Engine.h"


// Sets default values
AGadgetBase::AGadgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InitialLifeSpan = 3.0f;

	CollisionComponent = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("CollisionComponent"));
	CollisionComponent->OnComponentHit.AddDynamic(this, &AGadgetBase::OnHit);
	CollisionComponent->SetSphereRadius(15.f);
	CollisionComponent->SetCollisionProfileName("Gadget");
	//CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetSimulatePhysics(true);
	RootComponent = CollisionComponent;

	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	Mesh->SetRelativeScale3D(FVector(4.0f, 4.0f, 4.0f));
	Mesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	Mesh->AttachTo(RootComponent);

	MovementComponent = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("MovementComponent"));
	MovementComponent->bRotationFollowsVelocity = true;
}

void AGadgetBase::OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != NULL && OtherActor != this)
	{
		bIsActive = true;
		Activate();
	}
}

void AGadgetBase::SetNumGadgetsLeft(int32 pNumGadgets)
{
	NumGadgetsLeft = pNumGadgets;
}

int32 AGadgetBase::GetNumGadgetsLeft()
{
	return NumGadgetsLeft;
}
