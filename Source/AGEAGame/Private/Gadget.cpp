// Fill out your copyright notice in the Description page of Project Settings.

#include "AGEAGame.h"
#include "AGEAGameCharacter.h"
#include "Gadget.h"
#include "Engine.h"


// Sets default values
AGadget::AGadget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Gadget");
	//CollisionComp->OnComponentHit.AddDynamic(this, &AGadget::OnHit);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComp->UpdatedComponent = CollisionComp;
	MovementComp->InitialSpeed = 2000.f;
	MovementComp->MaxSpeed = 2000.f;
	MovementComp->bRotationFollowsVelocity = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->AttachParent = RootComponent;

	InitialLifeSpan = 2.0f;

}

// Called when the game starts or when spawned
void AGadget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGadget::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AGadget::OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	 //Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "Gadget Hit Something");

		TeleportLocation = OtherActor->GetActorLocation();
		Destroy();
	}
}

void AGadget::Activate()
{
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

