// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "GadgetBase.generated.h"

/*
	BASE CLASS FOR EACH THROWABLE GADGET
*/

UCLASS()
class STEALTHGAME_API AGadgetBase : public AActor
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsActive = false;

	
public:	
	// Sets default values for this actor's properties
	AGadgetBase(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintImplementableEvent)
	void Activate();

	UFUNCTION()
	void OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	FORCEINLINE UProjectileMovementComponent* GetMovementComponent() { return MovementComponent; }
	FORCEINLINE USphereComponent* GetCollisionComponent() { return CollisionComponent; }

};
