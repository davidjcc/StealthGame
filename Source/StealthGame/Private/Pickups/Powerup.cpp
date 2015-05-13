// Fill out your copyright notice in the Description page of Project Settings.

#include "StealthGame.h"
#include "Powerup.h"
#include "StealthGameCharacter.h"
#include "Engine.h"


// Sets default values
APowerup::APowerup(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	bIsActive = true;

	CollisionComp = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, "Collision Comp");
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APowerup::OnPlayerOverlap);
	RootComponent = CollisionComp;

	PowerupMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, "Pickup Mesh");
	PowerupMesh->SetRelativeLocation(FVector(30.f, 10.f, 0.f));
	PowerupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PowerupMesh->AttachParent = RootComponent;
}

// Called when the game starts or when spawned
void APowerup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APowerup::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void APowerup::Pickup()
{
	SetActive(false);
	PlayPickupSound(PickupSound);
	Destroy();
}

UAudioComponent* APowerup::PlayPickupSound(USoundCue* pPickupSound)
{
	UAudioComponent* AC = nullptr;
	if (pPickupSound)
	{
		AC = UGameplayStatics::PlaySoundAttached(pPickupSound, this->GetRootComponent());
	}

	return AC;
}

void APowerup::OnPlayerOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	AStealthGameCharacter* player = Cast<AStealthGameCharacter>(OtherActor);

	if (player && this->IsActive())
	{
		switch (PowerupType)
		{
			// If HealthPowerup
		case EPowerup::EHealthPowerup:
			player->UpdateHealth(25.f);
			Pickup();
			break;

			// If StealthPowerup
		case EPowerup::EStealthPowerup:
			Pickup();
			player->UpdateStealthValue(25.f);			
			break;

		case EPowerup::ETeleportGadget:
			Pickup();		
			player->UpdateNumSelectedGadget(0, 1);
			break;

		case EPowerup::EDecoyGadget:
			Pickup();
			player->UpdateNumSelectedGadget(1, 1);
			break;

		default:
			// no default behavior
			break;
		}
	}
}

EPowerup::EPowerupType APowerup::GetPowerupType()
{
	return PowerupType;
}

void APowerup::SetPowerupType(EPowerup::EPowerupType pPowerupType)
{
	PowerupType = pPowerupType;
}
