// Fill out your copyright notice in the Description page of Project Settings.

#include "AGEAGame.h"
#include "AIGuardCharacter.h"

AAIGuardCharacter::AAIGuardCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

void AAIGuardCharacter::GuardUpdateHealth(float UpdateHealthValue)
{
	Health += UpdateHealthValue;

	if (Health <= 0)
	{
		this->Destroy();
	}
}

void AAIGuardCharacter::FireWeapon()
{
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->ProcessWeapon();
	}
}

void AAIGuardCharacter::GiveDefaultWeapon()
{
	AWeapon * spawn = GetWorld()->SpawnActor<AWeapon>(WeaponSpawn);
	if (spawn)
	{
		CurrentWeapon = spawn;
		CurrentWeapon->SetOwningPawn(this);
		CurrentWeapon->OnEquip();
	}
}

void AAIGuardCharacter::BeginPlay()
{
	Super::BeginPlay();

	GiveDefaultWeapon();
}

void AAIGuardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AAIGuardCharacter::TakeDamage(float DamageAmount, struct FDamageEvent
	const& DamageEvent, class AController* EventInstigator,
		class AActor* DamageCauser)
{
	return Health;
}