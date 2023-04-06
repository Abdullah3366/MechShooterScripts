// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

class USkeletalMeshComponent;
class USceneComponent;
class UParticleSystem;
class AProjectile;

UCLASS()
class IRONVALIANT_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();
	UPROPERTY(BlueprintReadWrite)FVector End;

	virtual void Tick(float DeltaTime) override;

	void GunCoolDownFinished();
	void GunReloadFinished();
	void Fire();
	void Reload();

	bool ShotFired;

	FTimerHandle TH_GunCoolDown;
	FTimerHandle TH_GunReloading;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	int CurrentAmmo = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "True"))
	float ReloadTime = 2.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "True"))
	USkeletalMeshComponent* GunMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "True"))
	USceneComponent* Muzzle;
	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "True"))
	float Bloom = 20.f;

	int MaxAmmo = 600;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time", meta = (AllowPrivateAccess = "True"))
	float GunCoolDown = 1.f;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectile> BulletClass;
	AProjectile* Bullet;

	bool IsCooling = false;
	bool IsReloading = false;

};
