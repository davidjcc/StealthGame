// Fill out your copyright notice in the Description page of Project Settings.

#include "StealthGame.h"
#include "GadgetBase.h"


// Sets default values
AGadgetBase::AGadgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InitialLifeSpan = 3.0f;

	CollisionComponent = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("CollisionComponent"));
	CollisionComponent->OnComponentHit.AddDynamic(this, &AGadgetBase::OnHit);
	CollisionComponent->SetSphereRadius(200.0f);
	CollisionComponent->SetSimulatePhysics(true);
	RootComponent = CollisionComponent;

	MovementComponent = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("MovementComponent"));

}

void AGadgetBase::OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != NULL && OtherActor != this)
	{
		bIsActive = true;
		Activate();
	}
}
