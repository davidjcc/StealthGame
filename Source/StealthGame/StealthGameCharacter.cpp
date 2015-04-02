// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "StealthGame.h"
#include "StealthGameCharacter.h"
#include "Powerup.h"
#include "TeleportGadget.h"
#include "GuardCharacter.h"
#include "Engine.h"

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

	InputComponent->BindAction("ThrowGadget", IE_Pressed, this, &AStealthGameCharacter::ThrowTeleportGadget);
}

void AStealthGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DeathCheck();
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
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Colliding with guard!");
		bIsInStealth = false;
	}
}

void AStealthGameCharacter::ZoomCamIn()
{
	if (CameraBoom->TargetArmLength <= CamMax && CameraBoom->TargetArmLength >= CamMin)
	{
		CameraBoom->TargetArmLength -= CamZoomValue;
		if (CameraBoom->TargetArmLength < CamMin)
			CameraBoom->TargetArmLength = CamMin;
	}
}

void AStealthGameCharacter::ZoomCamOut()
{
	if (CameraBoom->TargetArmLength <= CamMax && CameraBoom->TargetArmLength >= CamMin)	{
		
		CameraBoom->TargetArmLength += CamZoomValue;		
		if (CameraBoom->TargetArmLength > CamMax)
			CameraBoom->TargetArmLength = CamMax;
	}
}

void AStealthGameCharacter::UpdateHealth(float pHealth)
{
	if (!bInfiniteHealth)
		Health = FMath::Clamp(Health + pHealth, 0.0f, 100.f);
}

void AStealthGameCharacter::UpdateStealthValue(float pStealthValue)
{
	StealthValue = FMath::Clamp(StealthValue + pStealthValue, 0.0f, 100.0f);
}

void AStealthGameCharacter::UpdateNumTeleportGadgets(int32 pTeleportGadgets)
{
	if (!bInfiniteTeleport)
		NumTeleportGadgets = FMath::Clamp(NumTeleportGadgets + pTeleportGadgets, 0, 5);
}

void AStealthGameCharacter::DeathCheck()
{
	if (Health < 0.f)
	{
		// TODO: Implement this function
	}
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

bool AStealthGameCharacter::CanThrowGadget()
{
	return (NumTeleportGadgets > 0) && (NumTeleportGadgets <= 5) && (GadgetClass != NULL);
}

void AStealthGameCharacter::ThrowTeleportGadget()
{
	if (CanThrowGadget())
	{
		FVector SpawnLoc = GetMesh()->GetSocketLocation("ThrowSocket");
		FRotator SpawnRot = GetActorRotation();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = Instigator;
		SpawnParams.Owner = this;
		ATeleportGadget* Gadget = GetWorld()->SpawnActor<ATeleportGadget>(GadgetClass, SpawnLoc, SpawnRot, SpawnParams);
		Gadget->SetOwner(this);

		if (!bInfiniteTeleport)
			NumTeleportGadgets = FMath::Clamp(NumTeleportGadgets - 1, 0, 5);
	}
}

void AStealthGameCharacter::TeleportPlayer(FVector pNewLocation)
{
	this->SetActorLocation(pNewLocation);
}
