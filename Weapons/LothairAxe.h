// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LothairAxe.generated.h"

class USkeletalMeshComponent;
class UCapsuleComponent;
class USceneComponent;
class AMechBase;

UCLASS()
class IRONVALIANT_API ALothairAxe : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALothairAxe();


	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* AxeMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UCapsuleComponent* Capsule;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AxeDamage = 10.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool Attacking = false;

	AMechBase* Enemy;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
