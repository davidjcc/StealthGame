// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Powerup.h"
#include "HealthPowerup.generated.h"

/**
 * 
 */
UCLASS()
class AGEAGAME_API AHealthPowerup : public APowerup
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Power Up")
	float UpdateHealthValue;	
};
