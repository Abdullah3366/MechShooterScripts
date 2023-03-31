// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SmokeLauncher.generated.h"

class AProjectile;
class USceneComponent;
class UParticleSystem;

UCLASS()
class IRONVALIANT_API ASmokeLauncher : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASmokeLauncher();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "True"))
	USkeletalMeshComponent* GunMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "True"))
	USceneComponent* Muzzle;


	FVector ActorForward;
	FVector VectorDirection;
	FRotator Direction;

	float offset = -0.4f;
	int SmokeCount = 0;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Fire();

	void LaunchSmoke();

	bool Firing = false;

	float Timer = 0.f;

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectile> BulletClass;
	AProjectile* Bullet;

};
