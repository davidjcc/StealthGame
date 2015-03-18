// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Weapon.h"
#include "Rifle.h"
#include "AIGuardCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AGEAGAME_API AAIGuardCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Movement")
	float RunSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Movement")
	float WalkSpeed;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void GuardUpdateHealth(float UpdateHealthValue);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Guard Weapon")
	AWeapon* CurrentWeapon;


protected:
	
};
