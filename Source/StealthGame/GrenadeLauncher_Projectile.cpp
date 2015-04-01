// Fill out your copyright notice in the Description page of Project Settings.

#include "StealthGame.h"
#include "GrenadeLauncher_Projectile.h"
#include "GrenadeLauncher.h"
#include "StealthGameCharacter.h"
#include "Engine.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AGrenadeLauncher_Projectile::AGrenadeLauncher_Projectile(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, "CollisionComp");
	RootComponent = CollisionComp;
	CollisionComp->SetSphereRadius(3.f);
	CollisionComp->OnComponentHit.AddDynamic(this, &AGrenadeLauncher_Projectile::OnCollision);

	StaticMeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, "StaticMeshComp");
	StaticMeshComp->AttachTo(RootComponent);

	MovementComp = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, "MovementComp");
	MovementComp->UpdatedComponent = CollisionComp;
	MovementComp->InitialSpeed = 3000.f;
	MovementComp->MaxSpeed = 3000.f;
	MovementComp->bRotationFollowsVelocity = false;
	MovementComp->bShouldBounce = false;
	
	ExplosionFX = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, "ExplosionFX");
	ExplosionFX->AttachTo(RootComponent);
}

void AGrenadeLauncher_Projectile::OnCollision(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != NULL && OtherActor != this)
	{
		AStealthGameCharacter* Player = Cast<AStealthGameCharacter>(OtherActor);
		if (Player)
		{
			Player->UpdateHealth(-(MyPawn->WeaponConfig.WeaponDamage));
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "Hit Player");
		}
		ExplosionFX->ActivateSystem();
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.f, GetActorLocation());
		Destroy();
	}
}

void AGrenadeLauncher_Projectile::SetOwner(AGrenadeLauncher* pNewOwner)
{
	if (MyPawn != pNewOwner && MyPawn != NULL)
	{
		MyPawn = pNewOwner;
	}
}
