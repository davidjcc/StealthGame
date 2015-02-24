// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Rocket.generated.h"

/**
*
*/
UCLASS()
class AGEAGAME_API ARocket : public AActor
{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	class UProjectileMovementComponent* ProjectileMovement;

	UFUNCTION()
	void OnCollision(AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepHitResult);

	// The partice effect for the rocket trail
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	UParticleSystemComponent* ParticleSystem;

	UPawnNoiseEmitterComponent* NoiseEmitter;
};
