// Fill out your copyright notice in the Description page of Project Settings.

#include "StealthGame.h"
#include "SniperRifle.h"


ASniperRifle::ASniperRifle(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	WeaponConfig.MaxAmmo = 50;
	WeaponConfig.ClipSize = 12;
	WeaponConfig.TimeBetweenShots = 0.f;
	WeaponConfig.ShotCost = 1;
	WeaponConfig.WeaponRange = 10000.f;
	WeaponConfig.WeaponName = "Sniper Rifle";
	CurrentAmmo = 50;
	CurrentClip = 5;
	ProjectileType = EWeaponProjectile::ProjectileType::EBullet;
}

