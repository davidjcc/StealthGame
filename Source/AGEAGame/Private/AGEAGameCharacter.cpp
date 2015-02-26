// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "AGEAGame.h"
#include "AGEAGameCharacter.h"
#include "HealthPowerup.h"
#include "ParticleDefinitions.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Engine.h"
#include "Engine/Blueprint.h"

//////////////////////////////////////////////////////////////////////////
// AAGEAGameCharacter

AAGEAGameCharacter::AAGEAGameCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	CurrentWeapon = NULL;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Setup the Box collider
	BoxCollisionComp = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, "BoxCollision");
	BoxCollisionComp->AttachParent = RootComponent;
	BoxCollisionComp->SetRelativeScale3D(FVector(1, 1, 5));
	BoxCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AAGEAGameCharacter::OnPlayerCollision);

	// Create the particle system
	ParticleSystem = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, "Particle Effect");
	ParticleSystem->AttachParent = RootComponent;

	// Create the noise emitter
	NoiseEmitter = ObjectInitializer.CreateDefaultSubobject<UPawnNoiseEmitterComponent>(this, TEXT("Noise Emitter"));
	
	PlayerHealth = 100;
	CameraZoomRate = 75.f;
	StealthValue = 100.f;
	IsInStealth = false;
	StealthDecayRate = 0.3f;
	isAttacking = false;

	Inventory.SetNum(3, false);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAGEAGameCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAxis("MoveForward", this, &AAGEAGameCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AAGEAGameCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AAGEAGameCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AAGEAGameCharacter::LookUpAtRate);

	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &AAGEAGameCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &AAGEAGameCharacter::TouchStopped);

	//// Handle camera zoom in and out key bindings
	InputComponent->BindAction("ZoomCameraOut", IE_Pressed, this, &AAGEAGameCharacter::ZoomCameraOut);
	InputComponent->BindAction("ZoomCameraIn", IE_Pressed, this, &AAGEAGameCharacter::ZoomCameraIn);
	
	// Handle the player's invisibility key binding
	InputComponent->BindAction("ToggleStealth", IE_Pressed, this, &AAGEAGameCharacter::ToggleStealth);
	InputComponent->BindAction("ToggleStealth", IE_Released, this, &AAGEAGameCharacter::ToggleStealth);

	// Handle whether the player is punching or not
	InputComponent->BindAction("Attack", IE_Pressed, this, &AAGEAGameCharacter::ToggleAttack);
	InputComponent->BindAction("Attack", IE_Released, this, &AAGEAGameCharacter::ToggleAttack);

	// Handle the player distraction noise
	InputComponent->BindAction("MakeNoise", IE_Pressed, this, &AAGEAGameCharacter::MakeDistractionNoise);

	// Handle the weapon fire binding
	InputComponent->BindAction("FireWeapon", IE_Pressed, this, &AAGEAGameCharacter::FireWeapon);

	// Handle the weapon equip
	InputComponent->BindAction("EquipWeaponSlot1", IE_Pressed, this, &AAGEAGameCharacter::EquipPistol);
	InputComponent->BindAction("EquipWeaponSlot2", IE_Pressed, this, &AAGEAGameCharacter::EquipShotgun);
	InputComponent->BindAction("EquipWeaponSlot3", IE_Pressed, this, &AAGEAGameCharacter::EquipRocketLauncher);
}


void AAGEAGameCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void AAGEAGameCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void AAGEAGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AAGEAGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AAGEAGameCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AAGEAGameCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AAGEAGameCharacter::ZoomCameraIn()
{
	CameraBoom->TargetArmLength -= CameraZoomRate;

	if (CameraBoom->TargetArmLength <= CAM_MIN)
	{
		CameraBoom->TargetArmLength = CAM_MIN;
	}

}

void AAGEAGameCharacter::ZoomCameraOut()
{
	CameraBoom->TargetArmLength += CameraZoomRate;

	if (CameraBoom->TargetArmLength >= CAM_MAX)
	{
		CameraBoom->TargetArmLength = CAM_MAX;
	}
}

void AAGEAGameCharacter::PlayerTakeDamage(float damage)
{
	PlayerHealth -= damage;

	// Min the health at 0
	if (PlayerHealth <= 0)
		PlayerHealth = 0;
}

void AAGEAGameCharacter::CollectPickup()
{
	// Get overlapping actors and store them in an array
	TArray<AActor*> CollectedActors;
	BoxCollisionComp->GetOverlappingActors(CollectedActors);

	// For each actor collected
	for (int32 i = 0; i < CollectedActors.Num(); i++)
	{
		// Cast the actor to a battery pickup
		AHealthPowerup* const testPowerup = Cast<AHealthPowerup>(CollectedActors[i]);

		// If cast is successful, and the powerup is valid and active
		if (testPowerup && !testPowerup->IsPendingKill() && testPowerup->bIsActive)
		{
			PlayerHealth += testPowerup->UpdateHealthValue;
			testPowerup->OnPickedUp();
			testPowerup->bIsActive = false;
		}
	}
}

void AAGEAGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StealthCheck();

	DeathCheck();

	CollectPickup();
}

void AAGEAGameCharacter::ToggleStealth()
{
	if (!IsInStealth)
	{
		IsInStealth = true;
		GetMesh()->SetMaterial(0, StealthMaterial);
	}
	else
	{
		IsInStealth = false;
		GetMesh()->SetMaterial(0, DefaultMaterial);
	}
}

void AAGEAGameCharacter::SetStealth(bool disguise)
{
	IsInStealth = disguise;
}

void AAGEAGameCharacter::SetIsAttacking(bool attacking)
{
	isAttacking = attacking;
}

void AAGEAGameCharacter::ToggleAttack()
{
	if (isAttacking)
	{
		isAttacking = false;
	}
	else
	{
		isAttacking = true;
	}
}

void AAGEAGameCharacter::MakeDistractionNoise()
{
	NoiseEmitter->MakeNoise(this, 10.0f, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(this, DistractionSound, GetActorLocation());
}

void AAGEAGameCharacter::FireWeapon()
{
	if (CurrentWeapon != NULL)
	{
		CurrentWeapon->ProcessWeapon();
	}
}

void AAGEAGameCharacter::OnPlayerCollision(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bHit, const FHitResult & hitresult)
{
	// If OtherActor is a pistol
	ARifle* Rifle = Cast<ARifle>(OtherActor);
	if (Rifle)
	{
		// Because the inventory is a subclassof we have to get the class
		Inventory[0] = Rifle->GetClass();
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "I just picked up a " + Rifle->WeaponConfig.WeaponName);
		Rifle->Destroy();
	}

	// If OtherActor is a shotgun
	AShotgun *Shotgun = Cast<AShotgun>(OtherActor);
	if (Shotgun)
	{
		// Because the inventory is a subclassof we have to get the class
		Inventory[1] = Shotgun->GetClass();
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "I just picked up a " + Shotgun->WeaponConfig.WeaponName);
		Shotgun->Destroy();
	}

	// If OtherActor is a Rocket Launcher
	ARocketLauncher *RocketLauncher = Cast<ARocketLauncher>(OtherActor);
	if (RocketLauncher)
	{
		Inventory[2] = RocketLauncher->GetClass();
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "I just picked up a " + RocketLauncher->WeaponConfig.WeaponName);
		RocketLauncher->Destroy();
	}
}

void AAGEAGameCharacter::EquipPistol()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "Attempting to equip pistol");

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	AWeapon *Spawner = GetWorld()->SpawnActor<AWeapon>(Inventory[0]);
	if (Spawner)
	{
		if (CurrentWeapon != NULL)
		{
			for (int32 i = 0; i < 3; i++)
			{
				if (Inventory[i] != NULL && Inventory[i]->GetDefaultObject<AWeapon>()->WeaponConfig.WeaponName == CurrentWeapon->WeaponConfig.WeaponName)
				{
					Inventory[i] = NULL;
					Inventory[i] = CurrentWeapon->GetClass();
					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "I put " + CurrentWeapon->WeaponConfig.WeaponName + " away in slot " + FString::FromInt(i));
				}
			}
			CurrentWeapon->Destroy();
			Spawner->CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Spawner->AttachRootComponentTo(GetMesh(), "WeaponSocket");
			CurrentWeapon = Spawner;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "My current weapon is " + CurrentWeapon->WeaponConfig.WeaponName);
		}
		else
		{
			Spawner->CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Spawner->AttachRootComponentTo(GetMesh(), "WeaponSocket");
			CurrentWeapon = Spawner;
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "My current weapon is " + CurrentWeapon->WeaponConfig.WeaponName);
		}
	}
}

void AAGEAGameCharacter::EquipShotgun()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	AWeapon *Spawner = GetWorld()->SpawnActor<AWeapon>(Inventory[1]);
	if (Spawner)
	{
		if (CurrentWeapon != NULL)
		{
			for (int32 i = 0; i < 3; i++)
			{
				if (Inventory[i] != NULL && Inventory[i]->GetDefaultObject<AWeapon>()->WeaponConfig.WeaponName == CurrentWeapon->WeaponConfig.WeaponName)
				{
					Inventory[i] = NULL;
					Inventory[i] = CurrentWeapon->GetClass();
					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "I put " + CurrentWeapon->WeaponConfig.WeaponName + " away in slot " + FString::FromInt(i));
				}
			}
			CurrentWeapon->Destroy();
			Spawner->CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Spawner->AttachRootComponentTo(GetMesh(), "WeaponSocket");
			CurrentWeapon = Spawner;
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "My current weapon is " + CurrentWeapon->WeaponConfig.WeaponName);
		}
		else
		{
			Spawner->CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Spawner->AttachRootComponentTo(GetMesh(), "WeaponSocket");
			CurrentWeapon = Spawner;
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "My current weapon is " + CurrentWeapon->WeaponConfig.WeaponName);
		}
	}
}

void AAGEAGameCharacter::EquipRocketLauncher()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	AWeapon *Spawner = GetWorld()->SpawnActor<AWeapon>(Inventory[2]);
	if (Spawner)
	{
		if (CurrentWeapon != NULL)
		{
			for (int32 i = 0; i < 3; i++)
			{
				if (Inventory[i] != NULL && Inventory[i]->GetDefaultObject<AWeapon>()->WeaponConfig.WeaponName == CurrentWeapon->WeaponConfig.WeaponName)
				{
					Inventory[i] = NULL;
					Inventory[i] = CurrentWeapon->GetClass();
					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "I put " + CurrentWeapon->WeaponConfig.WeaponName + " away in slot " + FString::FromInt(i));
				}
			}
			CurrentWeapon->Destroy();
			Spawner->CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Spawner->AttachRootComponentTo(GetMesh(), "WeaponSocket");
			CurrentWeapon = Spawner;
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "My current weapon is " + CurrentWeapon->WeaponConfig.WeaponName);
		}
		else
		{
			Spawner->CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Spawner->AttachRootComponentTo(GetMesh(), "WeaponSocket");
			CurrentWeapon = Spawner;
		}
	}
}

void AAGEAGameCharacter::DeathCheck()
{
	if (PlayerHealth <= 0)
	{
		GetWorld()->ServerTravel("/Game/Maps/GameOver");
	}

}

void AAGEAGameCharacter::StealthCheck()
{
	if (IsInStealth) {
		StealthValue -= StealthDecayRate;
		if (StealthValue <= 0.f) {
			StealthValue = 0.f;
			ToggleStealth();
			GetMesh()->SetMaterial(0, StealthMaterial);
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "In Stealth");
		}
	}
	else {
		GetMesh()->SetMaterial(0, DefaultMaterial);
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "Not in Stealth");
	}
}

void AAGEAGameCharacter::UpdateStealthValue(float StealthValue)
{
	this->StealthValue += StealthValue;

	if (StealthValue >= 100.0f)
		StealthValue = 100.0f;
	if (StealthValue <= 0.0f)
		StealthValue = 0.0f;
}
