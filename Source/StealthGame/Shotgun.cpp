// Fill out your copyright notice in the Description page of Project Settings.

#include "StealthGame.h"
#include "Shotgun.h"


AShotgun::AShotgun(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	WeaponConfig.MaxAmmo = 24;
	WeaponConfig.ClipSize = 6;
	WeaponConfig.TimeBetweenShots = 0.f;
	WeaponConfig.ShotCost = 1;
	WeaponConfig.WeaponRange = 1000.f;
	WeaponConfig.WeaponName = "Shotgun";
	CurrentAmmo = 6;
	CurrentClip = 12;
	ProjectileType = EWeaponProjectile::ProjectileType::ESpread;
}
