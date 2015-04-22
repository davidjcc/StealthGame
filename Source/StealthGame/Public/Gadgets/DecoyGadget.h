// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Gadgets/GadgetBase.h"
#include "DecoyGadget.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHGAME_API ADecoyGadget : public AGadgetBase
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh;

public:

	ADecoyGadget(const FObjectInitializer& ObjectInitializer);
	
};
