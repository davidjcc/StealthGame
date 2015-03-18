// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon.h"
#include "Rocket.h"
#include "RocketLauncher.generated.h"

/**
 * 
 */
UCLASS()
class AGEAGAME_API ARocketLauncher : public AWeapon
{
	GENERATED_UCLASS_BODY()

	virtual void ProjectileFire() override;
	
};
