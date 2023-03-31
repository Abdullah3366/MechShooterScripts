// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIMechController.generated.h"

/**
 * 
 */
UCLASS()
class IRONVALIANT_API ABaseAIMechController : public AAIController
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float LoseSightTime;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SightDistance = 5000;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	uint8 VisionAngle = 90;

	APawn* Player;
	bool bLostSight;

	FTimerHandle PerceptionTimer;
	FTimerHandle LoseSightTimer;
protected:
	void BeginPlay();

private:
	void Perceiving();
	void LoseSight();
};
