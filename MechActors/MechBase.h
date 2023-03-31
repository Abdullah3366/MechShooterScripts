// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "MechBase.generated.h"

DECLARE_DELEGATE(FOnGameOver)

class USpringArmComponent;
class UCameraComponent;
class UCapsuleComponent;
class UCharacterMovementComponent;
class USkeletalMeshComponent;
class UCapsuleComponent;
class UHitboxComponent;


UCLASS()
class IRONVALIANT_API AMechBase : public ACharacter
{
	GENERATED_BODY()

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookRight(float AxisValue);
	void LookUp(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);
	void BoostStart();
	void BoostStop();
	void CharacterJump();
	void JetUp(float AxisValue);
	void FlyStart();
	void FlyStop();
	void BoostDodge();
	void ZoomIn();
	void ZoomOut();

	UPROPERTY(EditAnywhere)
	float RotationRate = 10.f;
	

public:
	// Sets default values for this character's properties
	AMechBase();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimMontage* AMDodgeRight;
	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	bool Blocking = false;
	bool bIsAlive = true;

	FOnGameOver GameOver;

	void Block();
	void StopBlock();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int ZoomStatus = 0;
	float BoostSpeed;
	float BoostAcceleration;
	float NormalSpeed;
	float NormalAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float SystemHeat = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxSystemHeat = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float RunningTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool IsDodge = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool DoDodge = false;

	float HeatRate = 10.f;
	float BoostHeat = 40.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool IsHeating = false;
	UPROPERTY(BlueprintReadOnly)
	bool IsCooling = true;

	bool IsJump = true;



	float HorizontalAxisValue = 0.f;
	float VerticalAxisValue = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "True"))
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "True"))
	UCharacterMovementComponent* Movement;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "True"))
	USkeletalMeshComponent* CharacterMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "True"))
	UCameraComponent* Camera;

	FTimerHandle TH_BoostType;
	FTimerHandle TH_CoolingDelay;
	FTimerHandle TH_JumpType;

	bool flying = false;

	void CoolingDelaySet();

	void SetBoostType();

	void SetJumpType();

	bool CanMove = true;
};
