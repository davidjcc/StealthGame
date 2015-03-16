// Fill out your copyright notice in the Description page of Project Settings.

#include "AGEAGame.h"
#include "Weapon.h"
#include "Engine.h"
#include "AGEAGameCharacter.h"


AWeapon::AWeapon(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	CollisionBox = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, "CollisionBox");
	RootComponent = CollisionBox;

	WeaponMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, "WeaponMesh");
	WeaponMesh->AttachTo(RootComponent);

	ParticleSystem = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, "Particle Effect");
	ParticleSystem->AttachTo(RootComponent, "MF");

	bDrawDebugLine = false;
}

void AWeapon::ProcessWeapon()
{
	if (ProjectileType == EWeaponProjectile::EBullet)
	{
		FireWeapon();
	}

	if (ProjectileType == EWeaponProjectile::ESpread)
	{
		for (int32 i = 0; i <= WeaponConfig.WeaponSpread; i++)
		{
			FireWeapon();
		}
	}

	if (ProjectileType == EWeaponProjectile::EProjectile)
	{
		ProjectileFire();
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
	const FVector AimDirection = WeaponMesh->GetSocketRotation("MF").Vector();
	const FVector StartTrace = WeaponMesh->GetSocketLocation("MF");
	const FVector ShootDirection = WeaponRandomStream.VRandCone(AimDirection, SpreadCone, SpreadCone);
	const FVector EndTrace = StartTrace + ShootDirection * WeaponConfig.WeaponRange;
	const FHitResult Impact = WeaponTrace(StartTrace, EndTrace);

	// Play sound and particle effect
	UGameplayStatics::PlaySoundAtLocation(this, WeaponFireSound, GetActorLocation());
	ParticleSystem->ActivateSystem();

	// Process the hit after firing weapon
	ProcessInstantHit(Impact, StartTrace, ShootDirection, RandomSeed, CurrentSpread);
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
	AAGEAGameCharacter *GameCharacter = Cast<AAGEAGameCharacter>(Impact.GetActor());
	if (GameCharacter)
	{
		GameCharacter->UpdateHealth(this->WeaponConfig.WeaponDamage);
	}
}

void AWeapon::ProjectileFire()
{
	// Play sound and particle effect
	UGameplayStatics::PlaySoundAtLocation(this, WeaponFireSound, GetActorLocation());
	//ParticleSystem->ActivateSystem();
}


void AWeapon::SetOwningPawn(AAGEAGameCharacter * NewOwner)
{
	if (MyPawn != NewOwner) {
		Instigator = NewOwner;
		MyPawn = NewOwner;
	}
}

void AWeapon::AttachToPlayer()
{
	if (MyPawn) {
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

