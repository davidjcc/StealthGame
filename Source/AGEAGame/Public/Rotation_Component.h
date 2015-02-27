// Fill out your copyright notice in the Description page of Project Settings.

// Used if you want an actor to rotate every tick (e.g. pickups)

#pragma once

#include "Components/ActorComponent.h"
#include "Rotation_Component.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AGEAGAME_API URotation_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URotation_Component();

	// Called when the game starts
	virtual void InitializeComponent() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	// Properties decide in which directions the actor will rotate per tick
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RotateComponent)
	FRotator RotationProperties;

	void RotateActorPerTick(float DeltaTime);
	
};
