// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GrenadeLauncher_Projectile.generated.h"

class AGrenadeLauncher;

UCLASS()
class STEALTHGAME_API AGrenadeLauncher_Projectile : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleDefaultsOnly, Category = GLProjectile, meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleDefaultsOnly, Category = GLProjectile, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(VisibleDefaultsOnly, Category = GLProjectile, meta = (AllowPrivateAcces = "true"))
	class UProjectileMovementComponent* MovementComp;

	UPROPERTY(EditDefaultsOnly, Category = GLProjectile, meta = (AllowPrivateAcces = "true"))
	UParticleSystemComponent* ExplosionFX;

	AGrenadeLauncher* MyPawn;

	// Sets default values for this actor's properties
	AGrenadeLauncher_Projectile(const FObjectInitializer &ObjectInitializer);

	FORCEINLINE UProjectileMovementComponent* GetMovementComponent() { return MovementComp; }

	UFUNCTION()
	void OnCollision(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void SetOwner(AGrenadeLauncher* Owner);
};
