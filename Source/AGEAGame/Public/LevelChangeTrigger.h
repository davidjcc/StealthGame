// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "LevelChangeTrigger.generated.h"

/**
*
*/
UCLASS()
class AGEAGAME_API ALevelChangeTrigger : public AActor
{
	GENERATED_UCLASS_BODY()

	// Box trigger component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level Change")
	UBoxComponent* BoxTrigger;

	// Point light component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level Change")
	UPointLightComponent* PointLight;
	
	// The static mesh component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level Change")
	UStaticMeshComponent* StaticMeshComponent;
	
	// The new level to change to once the trigger is activated
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Change")
	FString LevelName;

	// The sound to play when the trigger is entered
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Change")
	USoundCue* Sound;

	// The intensity of the point light
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Change")
	float LightIntensity;
	
	UFUNCTION()
	void LevelTriggerEnter(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bHit, const FHitResult & hitresult);

	// The rate in which the mesh rotates
	UPROPERTY(VisibleAnywhere, BLueprintReadWrite, Category = "Level Change")
	float RotationAngle;

	// Used in Tick, this will rotate the actor every update by a defined angle and rate
	void RotateActorPerTick(float RotationAngle, float RotationRate);

	// Override the tick function
	virtual void Tick(float DeltaTime) override;

	bool isSoundPlaying;

private:

	// Bool to set whether or not to show debugging info to the log, e.g print to screen when the trigger is activated
	UPROPERTY(EditAnywhere, Category = "Level Change")
	bool ShowDebugInformation;


};
