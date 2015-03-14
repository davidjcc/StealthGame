 // Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "Weapon.h"
#include "Rifle.h"
#include "Shotgun.h"
#include "RocketLauncher.h"

#include "AGEAGameCharacter.generated.h"

UCLASS(config=Game)
class AAGEAGameCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	bool IsPlayer = false;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	UCameraComponent* FollowCamera;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate = 45.0f;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate = 45.0f;

	UFUNCTION(BlueprintCallable, Category = Event)
	virtual void BeginPlay() override;

	// Override the tick function
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health = 100.0f;

	UFUNCTION(BlueprintCallable, Category = "Stealth")
	void ActivateStealth();

	UFUNCTION(BlueprintCallable, Category = "Stealth")
	void DeactivateStealth();

	UFUNCTION(BlueprintCallable, Category = "Stealth")
	void SetStealth(bool isInStealth);

	// Player's stealth value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stealth")
	float StealthValue = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stealth")
	bool IsInStealth = false;

	// The particle effect for the disguise switch
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stealth")
	UParticleSystemComponent* ParticleSystem;

	// The material when the player is in disguise
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stealth")
	UMaterialInterface* StealthMaterial;

	// THe player's default material
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stealth")
	UMaterialInterface* DefaultMaterial;

	// The rate in which the invisiblity meter decreases defaults to 0.04f;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Stealth")
	float StealthDecayRate = 0.3f;

	// The sound to play when the player distracts a guard
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distraction")
	USoundCue* DistractionSound;

	// Is the player attacking?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool isAttacking = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Distraction")
	UPawnNoiseEmitterComponent* NoiseEmitter;

	UFUNCTION(BlueprintCallable, Category = "Distraction")
	void MakeDistractionNoise();

	////////////////////////
	//////WEAPON STUFF//////
	///////////////////////
	
	// Default weapon the player starts with
	UPROPERTY(EditDefaultsOnly, Category = DefaultInv)
	TSubclassOf<class AWeapon> WeaponSpawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AWeapon *CurrentWeapon;
	void ProcessWeaponPickup(AWeapon * Weapon);
	void FireWeapon();
	void NextWeapon();
	void PrevWeapon();
	void EquipWeapon(AWeapon * Weapon);

	UFUNCTION(BlueprintCallable, Category = Event)
	void GiveDefaultWeapon();

	// Player's inventory
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TArray<class AWeapon*> Inventory;

	void ProcessWeaponPickup();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	UBoxComponent* BoxCollisionComp;

	UFUNCTION()
	void OnPlayerCollision(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bHit, const FHitResult & hitresult);

	void DeathCheck();
	void StealthCheck();

	UFUNCTION(BlueprintCallable, Category = Stealth)
	void UpdateStealthValue(float UpdateStealthValue);

	UFUNCTION(BlueprintCallable, Category = Health)
	void UpdateHealth(float UpdateHealthValue);

	void ActivateSneak();
	void DeactivateSneak();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, 
		class AController* EventInstigator, class AActor* DamageCauser);


protected:
	
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	// Sets the isAttacking bool. The anim blueprint will handle the animations
	UFUNCTION(BlueprintCallable, Category = "Player Attack")
	void SetIsAttacking(bool attacking);

	// Toggles the isAttacking bool
	UFUNCTION(BlueprintCallable, Category = "Player Attack")
	void ToggleAttack();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface
};

