// Fill out your copyright notice in the Description page of Project Settings.

#include "AGEAGame.h"
#include "Rocket.h"
#include "AGEAGameCharacter.h"
#include "AIGuardCharacter.h"
#include "Engine.h"
#include "GameFramework/ProjectileMovementComponent.h"

ARocket::ARocket(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
	// Setup the collision sphere
	CollisionComp = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, "Collision Comp");
	CollisionComp->InitSphereRadius(10.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ARocket::OnCollision);
	RootComponent = CollisionComp;

	// Setup the static mesh component
	StaticMeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, "Static Mesh Comp");
	StaticMeshComp->AttachTo(RootComponent);

	// Setup the projectile movement component
	ProjectileMovement = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, "Projectile Comp");
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = false;
	ProjectileMovement->bShouldBounce = false;

	InitialLifeSpan = 5.f;

	ParticleSystem = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, "Particle Effect");
	ParticleSystem->AttachTo(RootComponent);

	NoiseEmitter = ObjectInitializer.CreateDefaultSubobject<UPawnNoiseEmitterComponent>(this, TEXT("Noise Emitter"));
}

void ARocket::OnCollision(AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepHitResult)
{
	if (OtherActor != NULL && OtherActor != this && OtherComp != NULL)
	{
		AAIGuardCharacter *Guard = Cast<AAIGuardCharacter>(OtherActor);
		if (Guard)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "You blew up the enemy");
			OtherComp->AddImpulseAtLocation(GetVelocity() * 100.f, GetActorLocation());
			ParticleSystem->ActivateSystem();

			NoiseEmitter->MakeNoise(this, 10.0f, GetActorLocation());


			Guard->Destroy();
			Destroy();
		}		
	}
}
