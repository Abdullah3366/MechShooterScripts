// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class UProjectileMovementComponent;
class UParticleSystem;

UCLASS()
class IRONVALIANT_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chaos)
	float Radius = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chaos)
	float Strength = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Chaos)
	float ChaosDamage = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Properties", meta = (AllowPrivateAccess = "True"))
	float InitialSpeed = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Properties", meta = (AllowPrivateAccess = "True"))
	float MaxSpeed = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Properties", meta = (AllowPrivateAccess = "True"))
	float BulletDamage = 10.f;
	UPROPERTY(EditAnywhere)
	USoundBase* BulletSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Properties", meta = (AllowPrivateAccess = "True"))
	UParticleSystem* SmokeParticles;

	float Health = 100;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UStaticMeshComponent* BulletMesh;
	UPROPERTY(EditDefaultsOnly)
	UProjectileMovementComponent* ProjectileComp;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageType> DamageType;
};