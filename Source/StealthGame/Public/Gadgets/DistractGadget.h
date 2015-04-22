// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Gadgets/GadgetBase.h"
#include "DistractGadget.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHGAME_API ADistractGadget : public AGadgetBase
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

public:

	ADistractGadget(const FObjectInitializer& ObjectInitializer);
	
	
};
