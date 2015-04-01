// Fill out your copyright notice in the Description page of Project Settings.

#include "StealthGame.h"
#include "GuardCharacter.h"
#include "Engine.h"


// Sets default values
AGuardCharacter::AGuardCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGuardCharacter::BeginPlay()
{
	Super::BeginPlay();

	GiveDefaultWeapon();
	
}

// Called every frame
void AGuardCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AGuardCharacter::UpdateHealth(const float pHealth)
{
	Health += pHealth;
}

void AGuardCharacter::FireWeapon()
{
	if (CurrentWeapon != NULL)
	{
		CurrentWeapon->ProcessWeapon();
	}
}

void AGuardCharacter::GiveDefaultWeapon()
{
	AWeapon * spawn = GetWorld()->SpawnActor<AWeapon>(WeaponSpawn);
	if (spawn)
	{
		CurrentWeapon = spawn;
		CurrentWeapon->SetOwningPawn(this);
		CurrentWeapon->OnEquip();
	}
}

