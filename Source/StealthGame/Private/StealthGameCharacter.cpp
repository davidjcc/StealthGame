// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "StealthGame.h"
#include "StealthGameCharacter.h"
#include "Powerup.h"
#include "GuardCharacter.h"
#include "Engine.h"
#include "GadgetBase.h"

AStealthGameCharacter::AStealthGameCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	CollisionComp = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, "CollisionComp");
	CollisionComp->SetSphereRadius(100.f);
	CollisionComp->AttachTo(RootComponent);

	CollisionComp->OnComponentHit.AddDynamic(this, &AStealthGameCharacter::OnCollision);

	NumTeleportGadgets = FMath::Clamp(NumTeleportGadgets, 0, 5);
	GadgetInventory.SetNum(3);
	NumGadgetsLeft.SetNum(3);

	ClearInventory();
	UpdateStealthValue(0.0f, false);
}

void AStealthGameCharacter::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	// set up game play key bindings
	check(InputComponent);

	InputComponent->BindAction("ZoomCamIn", IE_Pressed, this, &AStealthGameCharacter::ZoomCamIn);
	InputComponent->BindAction("ZoomCamOut", IE_Pressed, this, &AStealthGameCharacter::ZoomCamOut);

	InputComponent->BindAction("ActivateStealth", IE_Pressed, this, &AStealthGameCharacter::ActivateStealth);
	InputComponent->BindAction("DeactivateStealth", IE_Released, this, &AStealthGameCharacter::DeactivateStealth);

	InputComponent->BindAction("StopMovement", IE_Pressed, this, &AStealthGameCharacter::StopMovement);

	InputComponent->BindAction("ActivateThrowMode", IE_Pressed, this, &AStealthGameCharacter::ActivateThrowMode);
	InputComponent->BindAction("ActivateThrowMode", IE_Released, this, &AStealthGameCharacter::DeactivateThrowMode);

	InputComponent->BindAction("SelectGadget1", IE_Pressed, this, &AStealthGameCharacter::SelectTeleportGadget);
	InputComponent->BindAction("SelectGadget2", IE_Pressed, this, &AStealthGameCharacter::SelectDecoyGadget);
	InputComponent->BindAction("SelectGadget3", IE_Pressed, this, &AStealthGameCharacter::SelectDistractionGadget);
}

void AStealthGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StealthCheck();
}

void AStealthGameCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AStealthGameCharacter::OnCollision(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AGuardCharacter* Guard = Cast<AGuardCharacter>(OtherActor);
	if (Guard != NULL)
	{
		bIsInStealth = false;
	}
}

void AStealthGameCharacter::ZoomCamIn()
{
	if (CameraBoom->TargetArmLength <= CamMax && CameraBoom->TargetArmLength >= CamMin)
	{
		CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength - CamZoomValue, CamMin, CamMax);
	}
}

void AStealthGameCharacter::ZoomCamOut()
{
	if (CameraBoom->TargetArmLength <= CamMax && CameraBoom->TargetArmLength >= CamMin)	{
		
		CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength + CamZoomValue, CamMin, CamMax);
	}
}

void AStealthGameCharacter::UpdateHealth(float pHealth)
{
	if (!bInfiniteHealth)
		Health = FMath::Clamp(Health + pHealth, 0.0f, 100.f);

	DeathCheck();
}

float AStealthGameCharacter::GetStealthValue()
{
	return StealthValue;
}

void AStealthGameCharacter::UpdateStealthValue(float pStealthValue, bool pUpdateCurrentValue)
{
	if (pUpdateCurrentValue)
	{
		StealthValue = FMath::Clamp(StealthValue + pStealthValue, 0.0f, 100.0f);
	}
	else
	{
		StealthValue = pStealthValue;
	}
}

int32 AStealthGameCharacter::GetNumSelectedGadget(int32 pArrayIndex)
{
	return NumGadgetsLeft[pArrayIndex];
}

void AStealthGameCharacter::UpdateNumSelectedGadget(int32 pArrayIndex, int32 pUpdateValue)
{
	NumGadgetsLeft[pArrayIndex] = FMath::Clamp(NumGadgetsLeft[pArrayIndex] + pUpdateValue, 0, 3);
}

void AStealthGameCharacter::StealthCheck()
{
	if (bIsInStealth && StealthValue >= 0.0f)
	{
		if (!bInfiniteStealth)
			StealthValue -= StealthDecay;
	}	

	if (StealthValue < 0.f)
	{
		StealthValue = 0.f;
		DeactivateStealth();
	}
}

void AStealthGameCharacter::ActivateStealth()
{
	if (!bIsInStealth)
	{
		GetMesh()->SetMaterial(0, StealthMaterial);
		bIsInStealth = true;
	}
}

void AStealthGameCharacter::DeactivateStealth()
{
	if (bIsInStealth)
	{
		bIsInStealth = false;
		GetMesh()->SetMaterial(0, DefaultMaterial);
	}
}

void AStealthGameCharacter::ThrowGadget()
{
	if (CanSpawnGadget())
	{
		bIsUsingGadget = true;

		const FVector SpawnLocation = GetMesh()->GetSocketLocation(ThrowSocket);
		const FRotator SpawnRotation = GetActorRotation();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = Instigator;
		SpawnParams.Owner = this;

		AGadgetBase* SpawnGadget = GetWorld()->SpawnActor<AGadgetBase>(GadgetInventory[InventoryIndex], SpawnLocation, SpawnRotation, SpawnParams);
		CurrentGadget = SpawnGadget;
		SpawnGadget->Activate();

		DetermineNumGadgetAmount();

		bIsUsingGadget = false;
	}
}

void AStealthGameCharacter::ActivateThrowMode()
{
	if (!bThrowMode)
	{
		bThrowMode = true;
	}
}

void AStealthGameCharacter::DeactivateThrowMode()
{
	if (bThrowMode)
	{
		bThrowMode = false;
		ThrowGadget();
	}
}

void AStealthGameCharacter::SelectTeleportGadget()
{
	InventoryIndex = 0;
}

void AStealthGameCharacter::SelectDecoyGadget()
{
	InventoryIndex = 1;
}

void AStealthGameCharacter::SelectDistractionGadget()
{
	InventoryIndex = 2;
}

bool AStealthGameCharacter::CanSpawnGadget()
{
	return NumGadgetsLeft[InventoryIndex] > 0;

// 	bool canSpawn = false;
// 	for (int i = 0; i < GadgetInventory.Num(); ++i)
// 	{
// 		if (GadgetInventory[i] != NULL && NumGadgetsLeft[i] != 0)
// 			canSpawn = true;
// 		else canSpawn = false;
// 	}
// 
// 	return canSpawn;
}

void AStealthGameCharacter::DetermineNumGadgetAmount()
{
	if (!bInfiniteGadgets){
		NumGadgetsLeft[InventoryIndex] = FMath::Clamp(NumGadgetsLeft[InventoryIndex] - 1, 0, 100);
	}
}

void AStealthGameCharacter::ClearInventory()
{
	for (int i = 0; i < NumGadgetsLeft.Num(); i++)
	{
		NumGadgetsLeft[i] = 0;
	}
}
