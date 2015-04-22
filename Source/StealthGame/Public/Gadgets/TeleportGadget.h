// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Gadgets/GadgetBase.h"
#include "TeleportGadget.generated.h"

/**
 * 
 */
UCLASS()
class ATeleportGadget : public AGadgetBase
{
	GENERATED_BODY()
private:

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

public:

	ATeleportGadget(const FObjectInitializer& ObjectInitializer);
	
	
};
