// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "GadgetBase.generated.h"

/*
	BASE CLASS FOR EACH THROWABLE GADGET
*/

enum EGadgetType {
	ETELEPORT,
	EDECOY,
	EDISTRACT
};

UCLASS()
class STEALTHGAME_API AGadgetBase : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsActive = false;

	EGadgetType GadgetType;

	int32 NumGadgetsLeft = 3;
	
public:	
	// Sets default values for this actor's properties
	AGadgetBase(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintImplementableEvent)
	void Activate();

	UFUNCTION()
	void OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	FORCEINLINE UProjectileMovementComponent* GetMovementComponent() { return MovementComponent; }
	FORCEINLINE USphereComponent* GetCollisionComponent() { return CollisionComponent; }

	FORCEINLINE EGadgetType GetGadgetType() { return GadgetType; }

	int32 GetNumGadgetsLeft();
	void SetNumGadgetsLeft(int32 pNumGadgets);

};
