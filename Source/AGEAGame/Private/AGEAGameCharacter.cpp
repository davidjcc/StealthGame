// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "AGEAGame.h"
#include "AGEAGameCharacter.h"
#include "HealthPowerup.h"
#include "StealthPowerup.h"
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
	Inventory.SetNum(3, false);

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


}

//////////////////////////////////////////////////////////////////////////
// Input

void AAGEAGameCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &AAGEAGameCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AAGEAGameCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	
	// Handle the player's invisibility key binding
	InputComponent->BindAction("ActivateStealth", IE_Pressed, this, &AAGEAGameCharacter::ActivateStealth);
	InputComponent->BindAction("ActivateStealth", IE_Released, this, &AAGEAGameCharacter::DeactivateStealth);

	// Handle whether the player is punching or not
	InputComponent->BindAction("Attack", IE_Pressed, this, &AAGEAGameCharacter::ToggleAttack);
	InputComponent->BindAction("Attack", IE_Released, this, &AAGEAGameCharacter::ToggleAttack);

	// Handle the player distraction noise
	InputComponent->BindAction("MakeNoise", IE_Pressed, this, &AAGEAGameCharacter::MakeDistractionNoise);

	// Handle the weapon fire binding
	InputComponent->BindAction("FireWeapon", IE_Pressed, this, &AAGEAGameCharacter::FireWeapon);

	// Activate the sneaking system
	InputComponent->BindAction("ActivateSneak", IE_Pressed, this, &AAGEAGameCharacter::ActivateSneak);
	InputComponent->BindAction("ActivateSneak", IE_Released, this, &AAGEAGameCharacter::DeactivateSneak);

	// Inventory system
	InputComponent->BindAction("NextWeapon", IE_Pressed, this, &AAGEAGameCharacter::NextWeapon);
	InputComponent->BindAction("PrevWeapon", IE_Pressed, this, &AAGEAGameCharacter::PrevWeapon);
}

//void AAGEAGameCharacter::BeginPlay()
//{
//	GiveDefaultWeapon();
//	
//}

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

void AAGEAGameCharacter::NextWeapon()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "Trying to select next weapon");
	if (Inventory[CurrentWeapon->WeaponConfig.Priority]->WeaponConfig.Priority != Inventory.Num()) {
		if(Inventory[CurrentWeapon->WeaponConfig.Priority + 1] == NULL) {
			for (int32 i = CurrentWeapon->WeaponConfig.Priority + 1; i < Inventory.Num(); i++) {
				if (Inventory[i] && Inventory[i]->IsA(AWeapon::StaticClass())) {
					EquipWeapon(Inventory[i]);
				}
			}
		}
		else {
			EquipWeapon(Inventory[CurrentWeapon->WeaponConfig.Priority + 1]);
		}
	}
	else {
		EquipWeapon(Inventory[CurrentWeapon->WeaponConfig.Priority]);
	}
}

void AAGEAGameCharacter::PrevWeapon()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "Trying to select previous weapon");
	if (Inventory[CurrentWeapon->WeaponConfig.Priority]->WeaponConfig.Priority != 0) {
		if (Inventory[CurrentWeapon->WeaponConfig.Priority - 1] == NULL) {
			for (int32 i = CurrentWeapon->WeaponConfig.Priority - 1; i > 0; i--) {
				if (Inventory[i] && Inventory[i]->IsA(AWeapon::StaticClass())) {
					EquipWeapon(Inventory[i]);
				}
			}
		}
		else {
			EquipWeapon(Inventory[CurrentWeapon->WeaponConfig.Priority - 1]);
		}
	}
	else {
		EquipWeapon(Inventory[CurrentWeapon->WeaponConfig.Priority]);
	}
}

void AAGEAGameCharacter::EquipWeapon(AWeapon * Weapon)
{
	if (CurrentWeapon != NULL) {
		CurrentWeapon = Inventory[CurrentWeapon->WeaponConfig.Priority];
		CurrentWeapon->OnUnequip();
		CurrentWeapon = Weapon;
		Weapon->SetOwningPawn(this);
		Weapon->OnEquip();
	}
	else {
		CurrentWeapon = Weapon;
		CurrentWeapon = Inventory[CurrentWeapon->WeaponConfig.Priority];
		CurrentWeapon->SetOwningPawn(this);
		Weapon->OnEquip();
	}
}

void AAGEAGameCharacter::GiveDefaultWeapon()
{
	AWeapon * Spawner = GetWorld()->SpawnActor<AWeapon>(WeaponSpawn);
	if (Spawner) {
		Inventory[Spawner->WeaponConfig.Priority] = Spawner;
		CurrentWeapon = Inventory[Spawner->WeaponConfig.Priority];
		CurrentWeapon->SetOwningPawn(this);
		CurrentWeapon->OnEquip();
	}
}

void AAGEAGameCharacter::PlayerTakeDamage(float damage)
{
	PlayerHealth -= damage;

	// Min the health at 0
	if (PlayerHealth < 0.0f)
		PlayerHealth = 0;
}

void AAGEAGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DeathCheck();

	StealthCheck();

}

void AAGEAGameCharacter::ActivateStealth()
{
	if (StealthValue > 0.0f && !IsInStealth) {
		IsInStealth = true;
		GetMesh()->SetMaterial(0, StealthMaterial);
	}
}

void AAGEAGameCharacter::DeactivateStealth()
{
	IsInStealth = false;
	GetMesh()->SetMaterial(0, DefaultMaterial);
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
	if (isAttacking) {
		isAttacking = false;
	} else {
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
	if (CurrentWeapon != NULL)	{
		CurrentWeapon->ProcessWeapon();
	}
}

void AAGEAGameCharacter::OnPlayerCollision(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bHit, const FHitResult & hitresult)
{
	AWeapon* Weapon = Cast<AWeapon>(OtherActor);
	if (Weapon) {
		ProcessWeaponPickup(Weapon);
	}
}

void AAGEAGameCharacter::ProcessWeaponPickup(AWeapon * Weapon)
{
	if (Weapon != NULL) {
		if (Inventory[Weapon->WeaponConfig.Priority] == NULL) {
			AWeapon * Spawner = GetWorld()->SpawnActor<AWeapon>(Weapon->GetClass());
			if (Spawner) {
				Inventory[Spawner->WeaponConfig.Priority] = Spawner;
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, "You picked up a " + Inventory[Spawner->WeaponConfig.Priority]->WeaponConfig.WeaponName);
			}
			Weapon->Destroy();
		}
		else {
			if (Inventory[Weapon->WeaponConfig.Priority]->CurrentAmmo >= 0 && Weapon->CurrentAmmo <= (Inventory[Weapon->WeaponConfig.Priority]->WeaponConfig.MaxAmmo - Inventory[Weapon->WeaponConfig.Priority]->CurrentAmmo)) {
				Inventory[Weapon->WeaponConfig.Priority]->CurrentAmmo += Weapon->CurrentAmmo;
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Added " + Weapon->CurrentAmmo);
				Weapon->Destroy();
			}
			else if (Inventory[Weapon->WeaponConfig.Priority]->CurrentAmmo > Inventory[Weapon->WeaponConfig.Priority]->WeaponConfig.MaxAmmo) {
					Inventory[Weapon->WeaponConfig.Priority]->CurrentAmmo = Inventory[Weapon->WeaponConfig.Priority]->WeaponConfig.MaxAmmo;
			}
		}
	}
}

void AAGEAGameCharacter::DeathCheck()
{
	if (PlayerHealth <= 0) {
		GetWorld()->ServerTravel("/Game/Maps/GameOver");
	}
}

void AAGEAGameCharacter::StealthCheck()
{
	if (IsInStealth) {
		StealthValue -= StealthDecayRate;
	}
	if (StealthValue <= 0.0f) {
		DeactivateStealth();
	}

}

void AAGEAGameCharacter::UpdateStealthValue(float StealthValue)
{
	this->StealthValue += StealthValue;

	if (this->StealthValue >= 100.0f)
		this->StealthValue = 100.0f;
	if (this->StealthValue <= 0.0f)
		this->StealthValue = 0.0f;
}

void AAGEAGameCharacter::ActivateSneak()
{
	GetCharacterMovement()->MaxWalkSpeed = 100.0f;
}

void AAGEAGameCharacter::DeactivateSneak()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
}

void AAGEAGameCharacter::UpdateHealth(float UpdateHealthValue) 
{
	PlayerHealth += UpdateHealthValue; 
}
