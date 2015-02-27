// Fill out your copyright notice in the Description page of Project Settings.

#include "AGEAGame.h"
#include "HealthPowerup.h"


AHealthPowerup::AHealthPowerup(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	UpdateHealthValue = 20.f;
}

void AHealthPowerup::OnCollision(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bHit, const FHitResult & hitresult)
{
	AAGEAGameCharacter* Player = Cast<AAGEAGameCharacter>(OtherActor);
	if (Player) {
		Player->UpdateHealth(UpdateHealthValue);
	}

	UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());

	Destroy();
}
