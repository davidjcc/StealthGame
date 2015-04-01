// Fill out your copyright notice in the Description page of Project Settings.

#include "StealthGame.h"
#include "AssaultRifle.h"


AAssaultRifle::AAssaultRifle(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	WeaponConfig.MaxAmmo = 200;
	WeaponConfig.ClipSize = 32;
	WeaponConfig.TimeBetweenShots = 0.f;
	WeaponConfig.ShotCost = 1;
	WeaponConfig.WeaponRange = 3000.f;
	WeaponConfig.WeaponName = "Assault Rifle";
	CurrentAmmo = 200;
	CurrentClip = 32;
	ProjectileType = EWeaponProjectile::ProjectileType::EBullet;

}

