// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "StealthGameCharacter.generated.h"

class ATeleportGadget;
class AGadgetBase;

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
	float StealthValue = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stealth, meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* DefaultMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stealth, meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* StealthMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gadget, meta = (AllowPrivateAccess = "true"))
	int32 NumTeleportGadgets = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gadget, meta = (AllowPrivateAccess = "true"))
	int32 NumDecoy = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gadget, meta = (AllowPrivateAccess = "true"))
	int32 NumDistract = 0;

	float StealthDecay = 0.3f;

	UPROPERTY(BlueprintReadOnly, Category = Stealth, meta = (AllowPrivateAccess = "true"))
	bool bIsInStealth = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stealth, meta = (AllowPrivateAccess = "true"))
	USoundBase* DistractionNoise;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gadget, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ATeleportGadget> TeleportGadgetClass = NULL;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gadget, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AGadgetBase> DecoyGadgetClass = NULL;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gadget, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AGadgetBase> DistractGadgetClass = NULL;

	UPROPERTY(EditDefaultsOnly, Category = Stealth)
	bool bInfiniteStealth = false;

	UPROPERTY(EditDefaultsOnly, Category = Health)
	bool bInfiniteHealth = false;

	UPROPERTY(EditDefaultsOnly, Category = TeleportGadget)
	bool bInfiniteGadgets = false;

	// If true then the player's rotation will match that of the cursor
	UPROPERTY(BlueprintReadOnly, Category = TeleportGadget, meta = (AllowPrivateAccess = "true"))
	bool bThrowMode = false;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 InventoryIndex = 1;

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
	UFUNCTION(BlueprintCallable, Category = Health)
	void UpdateHealth(float pHealth);

	UFUNCTION(BlueprintCallable, Category = Stealth)
	float GetStealthValue();
	UFUNCTION(BlueprintCallable, Category = Stealth)
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

	void UseSelectedGadget();
	void UseTeleportGadget();
	UFUNCTION(BlueprintImplementableEvent)
	void UseDecoyGadget();
	void UseDistractionGadget();

	void ActivateThrowMode();
	void DeactivateThrowMode();
	void SetThrowMode(bool pThrowMode) { bThrowMode = pThrowMode; }

	void SelectTeleportGadget();
	void SelectDecoyGadget();
	void SelectDistractionGadget();

};

