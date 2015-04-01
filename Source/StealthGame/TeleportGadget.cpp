// Fill out your copyright notice in the Description page of Project Settings.

#include "StealthGame.h"
#include "TeleportGadget.h"
#include "Engine.h"


// Sets default values
ATeleportGadget::ATeleportGadget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Gadget");
	CollisionComp->OnComponentHit.AddDynamic(this, &ATeleportGadget::OnGadgetHit);
	//CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ATeleportGadget::OnGadgetBeginOverlap);

	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	RootComponent = CollisionComp;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComponent->UpdatedComponent = CollisionComp;
	MovementComponent->InitialSpeed = 2000.f;
	MovementComponent->MaxSpeed = 2000.f;
	MovementComponent->bRotationFollowsVelocity = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->AttachParent = RootComponent;

	InitialLifeSpan = 2.0f;

}

// Called when the game starts or when spawned
void ATeleportGadget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATeleportGadget::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ATeleportGadget::OnGadgetHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if((OtherActor != NULL) && (OtherActor != this))
	{
		Activate();
		Destroy();
	}
}

void ATeleportGadget::SetOwningPawn(AStealthGameCharacter* NewOwner)
{
	if (MyPawn != NewOwner)
	{
		MyPawn = NewOwner;
	}
}
