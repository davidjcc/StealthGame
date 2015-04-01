// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "StealthGameCharacter.h"
#include "TeleportGadget.generated.h"

UCLASS()
class STEALTHGAME_API ATeleportGadget : public AActor
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleDefaultsOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Teleport, meta = (AllowPrivateAccess = "true"))
	FVector TeleportLocation;

	AStealthGameCharacter* MyPawn;

public:
	// Sets default values for this actor's properties
	ATeleportGadget(const FObjectInitializer & ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void OnGadgetHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Return the projectile movement component
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() { return MovementComponent; }

	// Return collision
	FORCEINLINE class USphereComponent* GetCollisionComp() { return CollisionComp; }

	FVector GetTeleportLocation();

	// Teleports the player to the location of this when it hits another actor
	UFUNCTION(BlueprintImplementableEvent, Category = "Teleport")
	void Activate();

	void SetOwningPawn(AStealthGameCharacter* NewOwner);


	
	
};
