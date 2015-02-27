// Fill out your copyright notice in the Description page of Project Settings.

#include "AGEAGame.h"
#include "StealthPowerup.h"
#include "AGEAGameCharacter.h"

AStealthPowerup::AStealthPowerup(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	UpdateStealthValue = 10.0f;
}

void AStealthPowerup::OnCollision(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bHit, const FHitResult & hitresult)
{
	AAGEAGameCharacter* Player = Cast<AAGEAGameCharacter>(OtherActor);
	if (Player) {
		Player->UpdateStealthValue(UpdateStealthValue);
	}

	UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());

	Destroy();
}


