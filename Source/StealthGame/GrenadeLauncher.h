// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon.h"
#include "GrenadeLauncher.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHGAME_API AGrenadeLauncher : public AWeapon
{
	GENERATED_BODY()

	AGrenadeLauncher(const FObjectInitializer& ObjectInitializer);
	
	void ProjectileFire();
};
