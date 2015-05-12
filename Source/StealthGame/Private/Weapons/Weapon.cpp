// Fill out your copyright notice in the Description page of Project Settings.

#include "StealthGame.h"
#include "Weapon.h"
#include "Engine.h"
#include "StealthGameCharacter.h"


// Sets default values
AWeapon::AWeapon(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	CollisionBox = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, "CollisionBox");
	RootComponent = CollisionBox;

	WeaponMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, "WeaponMesh");
	WeaponMesh->AttachTo(RootComponent);

	MuzzleFX = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, "MuzzleFX");
	MuzzleFX->AttachTo(WeaponMesh, "MuzzleFlash");

	bDrawDebugLine = false;
}

void AWeapon::ProcessWeapon()
{
	// Bullet
	if (ProjectileType == EWeaponProjectile::EBullet)
	{
		if (CurrentClip > 0)
		{
			FireWeapon();
			PlayMuzzleFX();
			PlayWeaponSound(WeaponFireSound);
			if (!WeaponConfig.InfiniteAmmo)
				CurrentClip -= WeaponConfig.ShotCost;
		}
		else
		{
			ReloadAmmo();
		}
	}

	// Spread
	if (ProjectileType == EWeaponProjectile::ESpread)
	{
		if (CurrentClip > 0)
		{
			for (int32 i = 0; i <= WeaponConfig.WeaponSpread; i++)
			{
				FireWeapon();
			}
			PlayMuzzleFX();
			PlayWeaponSound(WeaponFireSound);
			if (!WeaponConfig.InfiniteAmmo)
				CurrentClip -= WeaponConfig.ShotCost;
		}
		else
		{
			ReloadAmmo();
		}
	}

	// Projectile
	if (ProjectileType == EWeaponProjectile::EProjectile)
	{
		if (CurrentClip > 0)
		{
			ProjectileFire();
			PlayMuzzleFX();
			PlayWeaponSound(WeaponFireSound);
			if (!WeaponConfig.InfiniteAmmo)
				CurrentClip -= WeaponConfig.ShotCost;
		}
		else
		{
			ReloadAmmo();
		}
	}
}

void AWeapon::FireWeapon()
{
	// Setup of the random seed and stream
	const int32 RandomSeed = FMath::Rand();
	FRandomStream WeaponRandomStream(RandomSeed);

	const float CurrentSpread = WeaponConfig.WeaponSpread;
	// Stops the weapon spread from going behind the player/gun. Shoot in a forward direction
	const float SpreadCone = FMath::DegreesToRadians(WeaponConfig.WeaponSpread * 0.5);
	const FVector AimDirection = WeaponMesh->GetSocketRotation("MuzzleFlash").Vector();
	const FVector StartTrace = WeaponMesh->GetSocketLocation("MuzzleFlash");
	const FVector ShootDirection = WeaponRandomStream.VRandCone(AimDirection, SpreadCone, SpreadCone);
	const FVector EndTrace = StartTrace + ShootDirection * WeaponConfig.WeaponRange;
	const FHitResult Impact = WeaponTrace(StartTrace, EndTrace);

	// Process the hit after firing weapon
	ProcessInstantHit(Impact, StartTrace, ShootDirection, RandomSeed, CurrentSpread);

}

void AWeapon::ProjectileFire()
{

}

FHitResult AWeapon::WeaponTrace(const FVector &TraceFrom, const FVector &TraceTo) const
{
	static FName WeaponFireTag = FName("WeaponTrace");

	FCollisionQueryParams TraceParams(WeaponFireTag, true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.AddIgnoredActor(this);

	FHitResult Hit(ForceInit);

	GetWorld()->LineTraceSingle(Hit, TraceFrom, TraceTo, TRACE_WEAPON, TraceParams);

	// Return the hit result
	return Hit;
}

void AWeapon::ProcessInstantHit(const FHitResult &Impact, const FVector &Origin,
	const FVector &ShootDirection, int32 RandomSeed, float ReticleSpread)
{
	// Calculate the end trace and the point the trace will reach
	const FVector EndTrace = Origin + ShootDirection * WeaponConfig.WeaponRange;
	const FVector EndPoint = Impact.GetActor() ? Impact.ImpactPoint : EndTrace;

	// Draw a debug line 
	if (bDrawDebugLine)
	{
		DrawDebugLine(this->GetWorld(), Origin, Impact.TraceEnd, FColor::Black, true, 10000, 10.f);
	}

	// Damage the player if they get hit
	AStealthGameCharacter *Player = Cast<AStealthGameCharacter>(Impact.GetActor());
	if (Player && !Player->IsInStealth())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Hit the player");
		Player->UpdateHealth(-(WeaponConfig.WeaponDamage));
	}
}

void AWeapon::SetOwningPawn(ACharacter * NewOwner)
{
	if (MyPawn != NewOwner)
	{
		Instigator = NewOwner;
		MyPawn = NewOwner;
	}
}

void AWeapon::AttachToPlayer()
{
	if (MyPawn)
	{
		DetachFromPlayer();

		USkeletalMeshComponent * Character = MyPawn->GetMesh();
		WeaponMesh->SetHiddenInGame(false);
		WeaponMesh->AttachTo(Character, "WeaponSocket");
	}
}

void AWeapon::DetachFromPlayer()
{
	WeaponMesh->DetachFromParent();
	WeaponMesh->SetHiddenInGame(true);
}

void AWeapon::OnEquip()
{
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttachToPlayer();
}

void AWeapon::OnUnequip()
{
	DetachFromPlayer();
}

void AWeapon::ReloadAmmo()
{
	if (CurrentAmmo > 0)
	{
		if (CurrentAmmo < WeaponConfig.ClipSize)
		{
			CurrentClip = CurrentAmmo;
		}
		else
		{
			CurrentAmmo -= WeaponConfig.ClipSize;
			CurrentClip += WeaponConfig.ClipSize;
		}
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "No Ammo");
	}
}

UAudioComponent* AWeapon::PlayWeaponSound(USoundCue* Sound)
{
	UAudioComponent* AC = nullptr;
	if (Sound && MyPawn)
	{
		AC = UGameplayStatics::PlaySoundAttached(Sound, MyPawn->GetRootComponent());
	}

	return AC;
}

void AWeapon::PlayMuzzleFX()
{
	if (MuzzleFX)
	{
		MuzzleFX->ActivateSystem();
	}
}