// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Mesh"));
	GunMesh->SetupAttachment(RootComponent);

	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(GunMesh);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentAmmo = MaxAmmo;
	srand(static_cast <unsigned> (time(0)));
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::Fire()
{
	if (IsCooling == false && IsReloading == false && CurrentAmmo > 0)
	{
		GetWorldTimerManager().SetTimer(TH_GunCoolDown, this, &AGun::GunCoolDownFinished, 1.f, false, GunCoolDown);
		IsCooling = true;
	//	UE_LOG(LogTemp, Warning, TEXT("Fire main gun"));
		if (BulletClass)
		{
			APawn* OwnerPawn = Cast<APawn>(GetOwner());
			if (OwnerPawn == nullptr) return;
			AController* OwnerController = OwnerPawn->GetController();
			if (OwnerController == nullptr) return;
			FVector VPLocation;
			FRotator VPRotation;
			OwnerController->GetPlayerViewPoint(VPLocation, VPRotation);
			if(OwnerPawn->IsPlayerControlled())
			{
				End = VPLocation + VPRotation.Vector() * 100000.f;
			}
			FHitResult Hit;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(OwnerPawn);
			bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, VPLocation, End, ECollisionChannel::ECC_GameTraceChannel1,Params);
			FRotator Direction;

			CurrentAmmo--;
		//	UE_LOG(LogTemp, Warning, TEXT("Current Ammo = %int"), CurrentAmmo);

			if (bSuccess) //Found a Target
			{
				UKismetSystemLibrary::DrawDebugPoint(GetWorld(), Hit.Location, 10, FColor::Red, true);
				Direction = UKismetMathLibrary::FindLookAtRotation(Muzzle->GetComponentLocation(), Hit.Location);
				if (GetOwner()->GetVelocity().Size() == 0.f || !OwnerPawn->IsPlayerControlled())
				{
				//	UE_LOG(LogTemp, Warning, TEXT("Accurate Shot with target"));
					Bullet = GetWorld()->SpawnActor<AProjectile>(BulletClass, Muzzle->GetComponentLocation(), Direction);
					Bullet->SetOwner(this->GetOwner());
				}
				else
				{
				//	UE_LOG(LogTemp, Warning, TEXT("Bloomed Shot with target"));
					float rotx = 0.f;
					float roty = -Bloom + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (Bloom + Bloom)));
					float rotz = -Bloom + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (Bloom + Bloom)));
					Bullet = GetWorld()->SpawnActor<AProjectile>(BulletClass, Muzzle->GetComponentLocation(), Direction + FRotator(rotx, roty, rotz));
					Bullet->SetOwner(this->GetOwner());
				}
			}
			else      //Didnt Find Target
			{
				Direction = UKismetMathLibrary::FindLookAtRotation(Muzzle->GetComponentLocation(), End);
				if (GetOwner()->GetVelocity().Size() == 0.f)
				{
				//	UE_LOG(LogTemp, Warning, TEXT("Accurate Shot without target"));
					Bullet = GetWorld()->SpawnActor<AProjectile>(BulletClass, Muzzle->GetComponentLocation(), Direction);
					Bullet->SetOwner(this->GetOwner());
				}
				else
				{
				//	UE_LOG(LogTemp, Warning, TEXT("Bloomed Shot witout target"));
					float rotx = 0.f;
					float roty = -Bloom + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (Bloom + Bloom)));
					float rotz = -Bloom + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (Bloom + Bloom)));
					Bullet = GetWorld()->SpawnActor<AProjectile>(BulletClass, Muzzle->GetComponentLocation(), Direction + FRotator(rotx, roty, rotz));
					Bullet->SetOwner(this->GetOwner());
				}
			}
			//ShotFired = true;
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("main gun Not ready"));
			//ShotFired = false;
		}
		ShotFired = true;
	}
	else
	{
		ShotFired = false;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Fire executed fine"));
	
}

void AGun::Reload()
{
	//UE_LOG(LogTemp, Warning, TEXT("main gun has started reloading"));
	IsReloading = true;
	GetWorldTimerManager().SetTimer(TH_GunReloading, this, &AGun::GunReloadFinished, 1.f, false, ReloadTime);
}

void AGun::GunCoolDownFinished()
{
	IsCooling = false;
	//UE_LOG(LogTemp, Warning, TEXT("main gun delay finished"));
	GetWorldTimerManager().ClearTimer(TH_GunCoolDown);
}

void AGun::GunReloadFinished()
{
	IsReloading = false;
	CurrentAmmo = MaxAmmo;
	//UE_LOG(LogTemp, Warning, TEXT("main gun has finished reloading"));
	GetWorldTimerManager().ClearTimer(TH_GunReloading);
}

