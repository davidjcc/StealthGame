// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "StealthGameCharacter.generated.h"

class ATeleportGadget;

UCLASS(Blueprintable)
class AStealthGameCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	const float CamZoomValue = 50.f;
	const float CamMax = 2000.f;
	const float CamMin = 300.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CollisionComp, meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
	float Health = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stealth, meta = (AllowPrivateAccess = "true"))
	float StealthValue = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stealth, meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* DefaultMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stealth, meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* StealthMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stealth, meta = (AllowPrivateAccess = "true"))
	int32 NumTeleportGadgets = 5;

	float StealthDecay = 0.3f;

	UPROPERTY(BlueprintReadOnly, Category = Stealth, meta = (AllowPrivateAccess = "true"))
	bool bIsInStealth = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stealth, meta = (AllowPrivateAccess = "true"))
	USoundBase* DistractionNoise;

	UPROPERTY(EditDefaultsOnly, Category = Gadget)
	TSubclassOf<ATeleportGadget> GadgetClass = NULL;

	UPROPERTY(EditDefaultsOnly, Category = Stealth)
	bool bInfiniteStealth = false;

	UPROPERTY(EditDefaultsOnly, Category = Health)
	bool bInfiniteHealth = false;

	UPROPERTY(EditDefaultsOnly, Category = TeleportGadget)
	bool bInfiniteTeleport = false;
public:
	AStealthGameCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	virtual void Tick(float DeltaTime);

	virtual void BeginPlay();

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UFUNCTION()
	void OnCollision(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void ZoomCamIn();
	void ZoomCamOut();

	float GetHealth() const { return Health; }
	void UpdateHealth(float pHealth);

	float GetStealthValue()	const { return StealthValue;	}
	void UpdateStealthValue(float pStealthValue);

	int32 GetNumTeleportGadgets(){	return NumTeleportGadgets; }
	void UpdateNumTeleportGadgets(int32 pTeleportGadgets);

	void DeathCheck();
	void StealthCheck();

	void ActivateStealth();
	void DeactivateStealth();
	bool IsInStealth() { return bIsInStealth; }

	// Stops the characters movement when space bar is pressed
	void StopMovement() { GetCharacterMovement()->StopMovementImmediately(); }

	void MakeDistractionNoise();

	bool CanThrowGadget();
	void ThrowTeleportGadget();

	void TeleportPlayer(FVector pNewLocation);

};
