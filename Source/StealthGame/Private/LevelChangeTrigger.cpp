// Fill out your copyright notice in the Description page of Project Settings.

#include "StealthGame.h"
#include "LevelChangeTrigger.h"
#include "StealthGameCharacter.h"
#include "StealthGameInstance.h"


// Sets default values
ALevelChangeTrigger::ALevelChangeTrigger(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("Collision"));
	Collision->SetRelativeScale3D(FVector(5.0f, 5.0f, 5.0f));
	RootComponent = Collision;

	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	Mesh->AttachTo(RootComponent);

	Light = ObjectInitializer.CreateDefaultSubobject<UPointLightComponent>(this, TEXT("Light"));
	Light->SetLightColor(NotActiveColour);
	Light->AttachTo(RootComponent);

	this->SetActorRelativeScale3D(FVector(1.f, 1.f, 1.f));
}

// Called when the game starts or when spawned
void ALevelChangeTrigger::BeginPlay()
{
	Super::BeginPlay();

	if (!bIsActive)
	{

	}
	
}

// Called every frame
void ALevelChangeTrigger::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (bIsActive)
	{
		Light->SetLightColor(ActiveColour);
	}
	else
	{
		Light->SetLightColor(NotActiveColour);
	}

}