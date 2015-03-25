// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Gadget.generated.h"

UCLASS()
class AGEAGAME_API AGadget : public AActor
{
	GENERATED_BODY()
public:

	UPROPERTY(VisibleDefaultsOnly, Category = Collision)
	class USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	class UProjectileMovementComponent* MovementComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Mesh)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Teleport)
	FVector TeleportLocation;
	
	// Sets default values for this actor's properties
	AGadget(const FObjectInitializer & ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Return the projectile movment component
	FORCEINLINE class UProjectileMovementComponent * GetProjectileMovement() { return MovementComp; }

	// Return collision
	FORCEINLINE class USphereComponent * GetCollisionComp() { return CollisionComp; }

	FVector GetTeleportLocation();

	void Activate();
	
	
};
