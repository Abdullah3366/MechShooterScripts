// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChaosBuilding.generated.h"

UCLASS()
class IRONVALIANT_API AChaosBuilding : public AActor
{
	GENERATED_BODY()

public:
	AChaosBuilding();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UGeometryCollectionComponent* FractureComponent;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* CollisionVolume;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Health = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector PlayerVelocity;

	FVector Bounds;
	FVector Origins;
	
	virtual void BeginPlay()override;

	UFUNCTION(BlueprintCallable)
	void UpdateHealth(float Damage);
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};