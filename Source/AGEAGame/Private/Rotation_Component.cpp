// Fill out your copyright notice in the Description page of Project Settings.

#include "AGEAGame.h"
#include "Rotation_Component.h"


// Sets default values for this component's properties
URotation_Component::URotation_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;

	RotationProperties.Pitch = 0.0f;
	RotationProperties.Yaw = 0.0f;
	RotationProperties.Roll = 0.0f;
}


// Called when the game starts
void URotation_Component::InitializeComponent()
{
	Super::InitializeComponent();

	// ...
	
}


// Called every frame
void URotation_Component::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	RotateActorPerTick(DeltaTime);
}

void URotation_Component::RotateActorPerTick(float DeltaTime)
{
	FRotator ActorRot = GetOwner()->GetActorRotation();
	ActorRot.Pitch += RotationProperties.Pitch * DeltaTime;
	ActorRot.Yaw += RotationProperties.Yaw * DeltaTime;
	ActorRot.Roll += RotationProperties.Roll * DeltaTime;

	GetOwner()->SetActorRotation(ActorRot);
}

