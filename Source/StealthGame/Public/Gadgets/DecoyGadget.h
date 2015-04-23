// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Gadgets/GadgetBase.h"
#include "DecoyGadget.generated.h"

class DecoyGadgetPawn;

/**
 * 
 */
UCLASS()
class STEALTHGAME_API ADecoyGadget : public AGadgetBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ADecoyGadgetPawn> DecoyPawnClass;

public:

	ADecoyGadget(const FObjectInitializer& ObjectInitializer);
	
	void SpawnDecoy();
};
