// Fill out your copyright notice in the Description page of Project Settings.

#include "StealthGame.h"
#include "DecoyGadgetPawn.h"


// Sets default values
ADecoyGadgetPawn::ADecoyGadgetPawn()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called to bind functionality to input
void ADecoyGadgetPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

