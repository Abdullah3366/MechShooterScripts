// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RocketLauncher.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class UParticleSystem;
class AProjectile;

UCLASS()
class IRONVALIANT_API ARocketLauncher : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARocketLauncher();
	UPROPERTY(BlueprintReadWrite)FVector End;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GunCoolDownFinished();
	void GunReloadFinished();

	void Fire();
	void Reload();
	void StartBurst();

	bool ShotFired;

	FTimerHandle TH_GunCoolDown;
	FTimerHandle TH_GunReloading;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	int CurrentAmmo = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* GunMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "True"))
	USceneComponent* Muzzle1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "True"))
	USceneComponent* Muzzle2;
	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "True"))
		float Bloom = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "True"))
		float SetBurstDelay = 0.5f;

	int MaxAmmo = 600;

	int BurstCount = 0;
	float BurstDelay = 0.f;

	bool Firing = false;

	//FVector MuzzleLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "True"))
	float ReloadTime = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "True"))
	float GunCoolDown = 1.f;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectile> RocketClass;
	AProjectile* Rocket;

	bool IsCooling = false;
	bool IsReloading = false;

};