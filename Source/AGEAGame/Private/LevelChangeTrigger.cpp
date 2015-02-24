// Fill out your copyright notice in the Description page of Project Settings.

#include "AGEAGame.h"
#include "LevelChangeTrigger.h"
#include "Engine.h"
#include "AGEAGameCharacter.h"


ALevelChangeTrigger::ALevelChangeTrigger(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

	// Allow the actor to tick
	PrimaryActorTick.bCanEverTick = true;

	// Box trigger
	BoxTrigger = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, "Box Trigger");
	RootComponent = BoxTrigger;
	BoxTrigger->SetRelativeScale3D(FVector(1, 1, 5));
	BoxTrigger->bGenerateOverlapEvents = true;
	BoxTrigger->SetHiddenInGame(true);

	BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &ALevelChangeTrigger::LevelTriggerEnter);

	// Point light
	LightIntensity = 100000.f;
	PointLight = ObjectInitializer.CreateDefaultSubobject<UPointLightComponent>(this, "Point Light");
	PointLight->SetLightColor(FColor::Blue);
	PointLight->Intensity = LightIntensity;
	PointLight->AttachParent = RootComponent;

	// Static mesh
	StaticMeshComponent = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, "Static Mesh Component");
	StaticMeshComponent->AttachParent = RootComponent;

	ShowDebugInformation = false;
	RotationAngle = 180.f;
	isSoundPlaying = false;
}

void ALevelChangeTrigger::LevelTriggerEnter(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bHit, const FHitResult & hitresult)
{
	// Only the player character can trigger the level change
	AAGEAGameCharacter* Character = Cast<AAGEAGameCharacter>(OtherActor);
	if (Character)
	{
		PointLight->SetLightColor(FColor::Green);

		if (isSoundPlaying == false)
		{
			PlaySoundAtLocation(Sound, GetActorLocation());
			isSoundPlaying = true;
		}

		FString newLevel = "/Game/Maps/" + LevelName;
		GetWorld()->ServerTravel(newLevel);
	}
}


void ALevelChangeTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotateActorPerTick(RotationAngle, DeltaTime);

}

void ALevelChangeTrigger::RotateActorPerTick(float Angle, float RotationRate)
{
	FRotator actorRotation = GetActorRotation();
	actorRotation.Yaw += RotationAngle * RotationRate;
	SetActorRotation(actorRotation);
}