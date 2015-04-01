// Fill out your copyright notice in the Description page of Project Settings.

#include "StealthGame.h"
#include "GrenadeLauncher.h"
#include "Engine.h"

AGrenadeLauncher::AGrenadeLauncher(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer)
{
	WeaponConfig.MaxAmmo = 20;
	WeaponConfig.ClipSize = 6;
	WeaponConfig.TimeBetweenShots = 0.f;
	WeaponConfig.ShotCost = 1;
	WeaponConfig.WeaponRange = 3000.f;
	WeaponConfig.WeaponName = "Grenade Launcher";
	CurrentAmmo = 20;
	CurrentClip = 4;
	ProjectileType = EWeaponProjectile::ProjectileType::EProjectile;
}


void AGrenadeLauncher::ProjectileFire()
{
	Super::ProjectileFire();

	if (ProjectileClass != NULL)
	{
		FVector SpawnLoc = WeaponMesh->GetSocketLocation("MuzzleFlash");
		FRotator SpawnRot = WeaponMesh->GetSocketRotation("MuzzleFlash");
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		AGrenadeLauncher_Projectile* GLProjectile = GetWorld()->SpawnActor<AGrenadeLauncher_Projectile>(SpawnLoc, SpawnRot, SpawnParams);
		GLProjectile->SetOwner(this);

		if (GLProjectile)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Firing projectile");
		}

		if (!WeaponConfig.InfiniteAmmo)
		{
			CurrentClip -= WeaponConfig.ShotCost;
		}
	}
}
