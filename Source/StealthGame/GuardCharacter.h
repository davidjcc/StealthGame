// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Weapon.h"
#include "GuardCharacter.generated.h"

UCLASS()
class STEALTHGAME_API AGuardCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float Health = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AWeapon> WeaponSpawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly , Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	AWeapon* CurrentWeapon = NULL;

public:
	// Sets default values for this character's properties
	AGuardCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void UpdateHealth(const float pHealth);

	UFUNCTION(BlueprintCallable, Category = "Guard Weapon")
	void FireWeapon();

	void GiveDefaultWeapon();
	
};
