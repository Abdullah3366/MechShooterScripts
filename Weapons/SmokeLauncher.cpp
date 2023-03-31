// Fill out your copyright notice in the Description page of Project Settings.


#include "SmokeLauncher.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASmokeLauncher::ASmokeLauncher()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Mesh"));
	GunMesh->SetupAttachment(RootComponent);

	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(GunMesh);
}

// Called when the game starts or when spawned
void ASmokeLauncher::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASmokeLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Firing)
	{
		Timer = Timer + DeltaTime;

		if (Timer > 0.1f)
		{
			LaunchSmoke();
			offset = offset + 0.2f;
			Timer = 0.f;
			SmokeCount = SmokeCount + 1;
		}

		if (SmokeCount > 10)
		{
			Firing = false;
			SmokeCount = 0;
			offset = -0.4f;
		}
	}
}

void ASmokeLauncher::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire Launcher"));
	Firing = true;
}

void ASmokeLauncher::LaunchSmoke()
{
	UE_LOG(LogTemp, Warning, TEXT("Launche Smoke"));
	if (BulletClass)
	{
		ActorForward = this->GetActorForwardVector();
		VectorDirection = ActorForward + FVector(0, offset, 0);
		Direction = VectorDirection.Rotation();
		Bullet = GetWorld()->SpawnActor<AProjectile>(BulletClass, Muzzle->GetComponentLocation(), Direction);
		Bullet->SetOwner(this->GetOwner());
	}
}

