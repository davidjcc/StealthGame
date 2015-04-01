// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Powerup.generated.h"

UENUM(BlueprintType)
namespace EPowerup
{
	enum EPowerupType
	{
		EHealthPowerup		UMETA(DisplayName = "Health"),
		EStealthPowerup		UMETA(DisplayName = "Stealth"),
		ETeleportGadget		UMETA(DisplayName = "Teleport Gadget")
	};
}

UCLASS()
class STEALTHGAME_API APowerup : public AActor
{
	GENERATED_BODY()

	bool bIsActive = true;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Powerup, meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Powerup, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PowerupMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Powerup, meta = (AllowPrivateAccess = "true"))
	USoundCue* PickupSound;

	UPROPERTY(EditDefaultsOnly, Category = Powerup, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EPowerup::EPowerupType> PowerupType;
	
public:	
	// Sets default values for this actor's properties
	APowerup(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	bool IsActive() { return bIsActive; }

	void SetActive(bool pIsActive) { bIsActive = pIsActive; }

	void Pickup();

	UAudioComponent* PlayPickupSound(USoundCue* pPickupSound);

	UFUNCTION()
	void OnPlayerOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	
};
