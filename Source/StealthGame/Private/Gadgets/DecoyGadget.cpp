// Fill out your copyright notice in the Description page of Project Settings.

#include "StealthGame.h"
#include "DecoyGadget.h"
#include "DecoyGadgetPawn.h"




ADecoyGadget::ADecoyGadget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GadgetType = EGadgetType::EDECOY;
}
