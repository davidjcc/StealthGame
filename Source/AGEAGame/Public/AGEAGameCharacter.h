 // Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "Weapon.h"
#include "Rifle.h"
#include "Shotgun.h"
#include "RocketLauncher.h"
#include "AGEAGameCharacter.generated.h"

const float CAM_MIN = 0.0f;// 300.f;
const float CAM_MAX = 1000.f;

UCLASS(config=Game)
class AAGEAGameCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	UCameraComponent* FollowCamera;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	// Override the tick function
	virtual void Tick(float DeltaTime) override;

	// Player's health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Health")
	float PlayerHealth;

	// Update the player's health minus the damage
	UFUNCTION(BlueprintCallable, Category = "Player Health")
	void PlayerTakeDamage(float Damage);

	// Toggles the player's stealth
	UFUNCTION(BlueprintCallable, Category = "Player Stealth")
	void ToggleStealth();

	UFUNCTION(BlueprintCallable, Category = "Player Stealth")
	void SetStealth(bool isInStealth);

	// Player's stealth value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stealth")
	float StealthValue;

	// Is the player invisible?
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Stealth")
	bool IsInStealth;

	// The particle effect for the disguise switch
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Stealth")
	UParticleSystemComponent* ParticleSystem;

	// The material when the player is in disguise
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stealth")
	UMaterialInterface* StealthMaterial;

	// THe player's default material
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stealth")
	UMaterialInterface* DefaultMaterial;

	// The rate in which the invisiblity meter decreases defaults to 0.04f;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Player")
	float StealthDecayRate;

	// The sound to play when the player distracts a guard
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	USoundCue* DistractionSound;

	// Camera zoom rate
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	float CameraZoomRate;

	// Is the player attacking?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Attack")
	bool isAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Distraction")
	UPawnNoiseEmitterComponent* NoiseEmitter;

	UFUNCTION(BlueprintCallable, Category = "Player Distraction")
	void MakeDistractionNoise();

	// Called when we press a key, to collect any power ups
	UFUNCTION(BlueprintCallable, Category = "Player")
	void CollectPickup();

	void FireWeapon();

	AWeapon *CurrentWeapon;

	void EquipPistol();
	void EquipShotgun();
	void EquipRocketLauncher();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
	UBoxComponent* BoxCollisionComp;

	UFUNCTION()
	void OnPlayerCollision(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bHit, const FHitResult & hitresult);

	void DeathCheck();

	void StealthCheck();
	void UpdateStealthValue(float UpdateStealthValue);

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

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	// Zooms the camera in
	void ZoomCameraIn();

	// Zooms the camera out
	void ZoomCameraOut();

	// Player's inventory
	TArray<TSubclassOf<AWeapon>> Inventory;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface
};

