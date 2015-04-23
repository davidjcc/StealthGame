// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "DecoyGadgetPawn.generated.h"

UCLASS()
class STEALTHGAME_API ADecoyGadgetPawn : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADecoyGadgetPawn();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	
	
};
