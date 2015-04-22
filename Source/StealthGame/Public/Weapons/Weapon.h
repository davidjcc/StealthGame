// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GrenadeLauncher_Projectile.h"
#include "Weapon.generated.h"

class AStealthGameCharacter;

#define TRACE_WEAPON ECC_GameTraceChannel1

// The different types of weapon projectiles
UENUM(BlueprintType)
namespace EWeaponProjectile
{
	// This defines the type of projectile a weapon will have.
	enum ProjectileType
	{
		// Pistols, Rifles, etc...
		EBullet			UMETA(DisplayName = "Bullet"),

		// Shotguns, Automatic Rifles, etc...
		ESpread			UMETA(DisplayName = "Spread"),

		// Rocket and Grenade launchers, etc...
		EProjectile		UMETA(DisplayName = "Projectile")
	};
}

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY();

	// The maximum amount of ammo a gun can hold
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	int32 ClipSize;

	// The time between each shot fired
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float TimeBetweenShots;

	// The amount of ammo a shot costs, e.g. a semi auto gun shot can cost 3 bullets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo)
	int32 ShotCost;

	// How far the gun can shoot to, e.g. a shotgun will have a lower range than a rifle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float WeaponRange;

	// The random amount of spread a bullet has so it isn't always shooting directly straight
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float WeaponSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	FString WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float WeaponDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	int32 Priority;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	bool InfiniteAmmo;

	// Defaults
	FWeaponData()
	{
		MaxAmmo = 500;
		ClipSize = 20;
		TimeBetweenShots = 0.2f;
		ShotCost = 1;
		WeaponRange = 500.f;
		WeaponSpread = 20.f;
		InfiniteAmmo = true;
	}
};


UCLASS()
class STEALTHGAME_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon(const FObjectInitializer & ObjectInitializer);

	// Process the type of weapon being used
	void ProcessWeapon();

	// Fire the weapon after being processed
	void FireWeapon();

	virtual void ProjectileFire();

	// Collision component
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	UBoxComponent* CollisionBox;

	// Skeletal mesh component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	FWeaponData WeaponConfig;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	TEnumAsByte<EWeaponProjectile::ProjectileType> ProjectileType;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TSubclassOf<class AGrenadeLauncher_Projectile> ProjectileClass;

	// Draw a debug line of the weapon firing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	bool bDrawDebugLine = false;

	// The sound to play when the weapon is fired
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	USoundCue* WeaponFireSound;

	// The sound to play when the weapon is out of ammo
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	USoundCue* EmptyWeaponSound;

	// The particle effect for the muzzle flash
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
	UParticleSystemComponent* MuzzleFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	int32 CurrentClip;

	void SetOwningPawn(ACharacter * NewOwner);

	void AttachToPlayer();
	void DetachFromPlayer();

	void OnEquip();
	void OnUnequip();

	void ReloadAmmo();

	UAudioComponent* PlayWeaponSound(USoundCue * Sound);

	void PlayMuzzleFX();

protected:

	FHitResult WeaponTrace(const FVector &TraceFrom, const FVector &TraceTo) const;

	void ProcessInstantHit(const FHitResult &Impact, const FVector &Origin, const FVector &ShootDirection, int32 RandomSeed, float ReticleSpread);

	ACharacter * MyPawn;	
};
