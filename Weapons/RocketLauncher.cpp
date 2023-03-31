// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketLauncher.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ARocketLauncher::ARocketLauncher()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun Mesh"));
	GunMesh->SetupAttachment(RootComponent);

	Muzzle1 = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle1"));
	Muzzle1->SetupAttachment(GunMesh);

	Muzzle2 = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle2"));
	Muzzle2->SetupAttachment(GunMesh);
}

// Called when the game starts or when spawned
void ARocketLauncher::BeginPlay()
{
	Super::BeginPlay();
	//MuzzleLocation = Muzzle->GetComponentLocation();
	CurrentAmmo = MaxAmmo;
	srand(static_cast <unsigned> (time(0)));
}

// Called every frame
void ARocketLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Firing)
	{
		BurstDelay = BurstDelay + DeltaTime;
	}

	if (BurstCount == 4)
	{
		Firing = false;
	}

	if (BurstDelay >= SetBurstDelay)
	{
		BurstDelay = 0.f;
		BurstCount++;
		Fire();
		UE_LOG(LogTemp, Warning, TEXT("burst number = %int"), BurstCount);
	}
}

void ARocketLauncher::StartBurst()
{
	if (IsCooling == false && IsReloading == false && CurrentAmmo > 0)
	{
		GetWorldTimerManager().SetTimer(TH_GunCoolDown, this, &ARocketLauncher::GunCoolDownFinished, 1.f, false, GunCoolDown);
		IsCooling = true;
		BurstCount = 1;
		Firing = true;
		Fire();
	}
}


void ARocketLauncher::Fire()
{
		UE_LOG(LogTemp, Warning, TEXT("right Shot"));
		if (RocketClass)
		{
			APawn* OwnerPawn = Cast<APawn>(GetOwner());
			if (OwnerPawn == nullptr) return;
			AController* OwnerController = OwnerPawn->GetController();
			if (OwnerController == nullptr) return;
			FVector VPLocation;
			FRotator VPRotation;
			OwnerController->GetPlayerViewPoint(VPLocation, VPRotation);
			if (OwnerPawn->IsPlayerControlled())
			{
				End = VPLocation + VPRotation.Vector() * 100000.f;
			}
			FHitResult Hit;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(OwnerPawn);
			bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, VPLocation, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
			FRotator Direction;

			CurrentAmmo--;
			UE_LOG(LogTemp, Warning, TEXT("Current Ammo = %int"), CurrentAmmo);

			if (bSuccess) //Found a Target
			{
				UKismetSystemLibrary::DrawDebugPoint(GetWorld(), Hit.Location, 10, FColor::Red, true);
				Direction = UKismetMathLibrary::FindLookAtRotation(Muzzle1->GetComponentLocation(), Hit.Location);
				if (GetOwner()->GetVelocity().Size() == 0.f)
				{
					UE_LOG(LogTemp, Warning, TEXT("Accurate Shot with target"));
					if (BurstCount == 1 || BurstCount == 3)
					{
						Rocket = GetWorld()->SpawnActor<AProjectile>(RocketClass, Muzzle1->GetComponentLocation(), Direction);
					}
					if (BurstCount == 2 || BurstCount == 4)
					{
						Rocket = GetWorld()->SpawnActor<AProjectile>(RocketClass, Muzzle2->GetComponentLocation(), Direction);
					}
					Rocket->SetOwner(this->GetOwner());
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Bloomed Shot with target"));
					float rotx = 0.f;
					float roty = -Bloom + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (Bloom + Bloom)));
					float rotz = -Bloom + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (Bloom + Bloom)));
					Rocket = GetWorld()->SpawnActor<AProjectile>(RocketClass, Muzzle1->GetComponentLocation(), Direction + FRotator(rotx, roty, rotz));
					Rocket->SetOwner(this->GetOwner());
				}
			}
			else      //Didnt Find Target
			{
				Direction = UKismetMathLibrary::FindLookAtRotation(Muzzle1->GetComponentLocation(), End);
				if (GetOwner()->GetVelocity().Size() == 0.f)
				{
					UE_LOG(LogTemp, Warning, TEXT("Accurate Shot without target"));
					Rocket = GetWorld()->SpawnActor<AProjectile>(RocketClass, Muzzle1->GetComponentLocation(), Direction);
					Rocket->SetOwner(this->GetOwner());
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Bloomed Shot witout target"));
					float rotx = 0.f;
					float roty = -Bloom + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (Bloom + Bloom)));
					float rotz = -Bloom + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (Bloom + Bloom)));
					if (BurstCount == 1 || BurstCount == 3)
					{
						Rocket = GetWorld()->SpawnActor<AProjectile>(RocketClass, Muzzle1->GetComponentLocation(), Direction + FRotator(rotx, roty, rotz));
					}
					if (BurstCount == 2 || BurstCount == 4)
					{
						Rocket = GetWorld()->SpawnActor<AProjectile>(RocketClass, Muzzle2->GetComponentLocation(), Direction + FRotator(rotx, roty, rotz));
					}
					Rocket->SetOwner(this->GetOwner());
				}
			}
			//ShotFired = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("main gun Not ready"));
			//ShotFired = false;
		}
		ShotFired = true;
}

void ARocketLauncher::Reload()
{
	UE_LOG(LogTemp, Warning, TEXT("main gun has started reloading"));
	IsReloading = true;
	GetWorldTimerManager().SetTimer(TH_GunReloading, this, &ARocketLauncher::GunReloadFinished, 1.f, false, ReloadTime);
}

void ARocketLauncher::GunCoolDownFinished()
{
	IsCooling = false;
	UE_LOG(LogTemp, Warning, TEXT("main gun delay finished"));
	GetWorldTimerManager().ClearTimer(TH_GunCoolDown);
}

void ARocketLauncher::GunReloadFinished()
{
	IsReloading = false;
	CurrentAmmo = MaxAmmo;
	UE_LOG(LogTemp, Warning, TEXT("main gun has finished reloading"));
	GetWorldTimerManager().ClearTimer(TH_GunReloading);
}

