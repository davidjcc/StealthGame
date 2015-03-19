// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Weapon.h"
#include "Rifle.h"
#include "AIGuardCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AGEAGAME_API AAIGuardCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	float Health = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Movement")
	float RunSpeed = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character Movement")
	float WalkSpeed = 600.0f;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void GuardUpdateHealth(float UpdateHealthValue);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Guard Weapon")
	TSubclassOf<class AWeapon> WeaponSpawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Guard Weapon")
	AWeapon* CurrentWeapon = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Guard Weapon")
	void FireWeapon();

	void GiveDefaultWeapon();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent 
		const& DamageEvent, class AController* EventInstigator, 
			class AActor* DamageCauser);

protected:
	
};
