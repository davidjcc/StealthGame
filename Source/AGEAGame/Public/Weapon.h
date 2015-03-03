// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class AAGEAGameCharacter;

#define TRACE_WEAPON ECC_GameTraceChannel1

/**
 * 
 */

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

USTRUCT()
struct FWeaponData
{
	GENERATED_USTRUCT_BODY();

	// The maximium amount of ammo a gun can hold
	UPROPERTY(EditDefaultsOnly, Category = Ammo)
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	int32 ClipSize;

	// The time between each shot fired
	UPROPERTY(EditDefaultsOnly, Category = Config)
	float TimeBetweenShots;

	// The amount of ammo a shot costs, e.g. a semi auto gun shot can cost 3 bullets
	UPROPERTY(EditDefaultsOnly, Category = Ammo)
	int32 ShotCost;

	// How far the gun can shoot to, e.g. a shotgun will have a lower range than a rifle
	UPROPERTY(EditDefaultsOnly, Category = Config)
	float WeaponRange;

	// The random amount of spread a bullet has so it isn't always shooting directly straight
	UPROPERTY(EditDefaultsOnly, Category = Config)
	float WeaponSpread;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	FString WeaponName;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	float WeaponDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	int32 Priority;
};

UCLASS()
class AGEAGAME_API AWeapon : public AActor
{
	GENERATED_UCLASS_BODY()

	// Process the type of weapon being used
	void ProcessWeapon();

	// Fire the weapon after being processed
	void FireWeapon();

	virtual void ProjectileFire();

	// Collision component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	UBoxComponent* CollisionBox;

	// Skeletal mesh component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	USkeletalMeshComponent* WeaponMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = Config)
	FWeaponData WeaponConfig;

	UPROPERTY(EditDefaultsOnly, Category = Config)
	TEnumAsByte<EWeaponProjectile::ProjectileType> ProjectileType;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class ARocket> ProjectileClass;

	// Draw a debug line of the weapon firing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	bool bDrawDebugLine;

	// The sound to play when the weapon is fired
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	USoundCue* WeaponFireSound;

	// The particle effect for the muzzle flash
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	UParticleSystemComponent* ParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	int32 CurrentClip;

	void SetOwningPawn(AAGEAGameCharacter * NewOwner);

	void AttachToPlayer();
	void DetachFromPlayer();

	void OnEquip();
	void OnUnequip();

protected:

	FHitResult WeaponTrace(const FVector &TraceFrom, const FVector &TraceTo) const;

	void ProcessInstantHit(const FHitResult &Impact, const FVector &Origin,	const FVector &ShootDirection, int32 RandomSeed, float ReticleSpread);

	AAGEAGameCharacter * MyPawn;
};
