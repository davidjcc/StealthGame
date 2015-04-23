// Fill out your copyright notice in the Description page of Project Settings.

#include "StealthGame.h"
#include "DistractGadget.h"




ADistractGadget::ADistractGadget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GadgetType = EGadgetType::EDISTRACT;
}
