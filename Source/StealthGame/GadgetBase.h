// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "StealthGameCharacter.h"
#include "GadgetBase.generated.h"

UCLASS()
class STEALTHGAME_API AGadgetBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGadgetBase(const class FObjectInitializer & ObjectInit);
	
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Collision)
	USphereComponent* CollisionComp;

	void SetOwner(AStealthGameCharacter* NewPawn);

	// Teleports the player to the location of this when it hits another actor
	UFUNCTION(BlueprintImplementableEvent)
	void Activate();
	
protected:

	AStealthGameCharacter* MyPawn;
	
};
