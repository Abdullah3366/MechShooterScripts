// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MechBase.h"
#include "Lothair.generated.h"


class AGun;
class ARocketLauncher;
class ASmokeLauncher;
class ALothairAxe;
class UCapsuleComponent;

UCLASS()
class IRONVALIANT_API ALothair : public AMechBase
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere)
	float PanzerBoostSpeed;
	UPROPERTY(EditAnywhere)
	float PanzerBoostAcceleration;
		
	float RightDirBuffer = 0.f;
	float LeftDirBuffer = 0.f;
	float TopDirBuffer = 0.f;


	///Melee Variables///

	float AttackTimer = 0.f;
	bool Morphing = false;
	bool Cancelling = false;

	enum AttackList { LeftLightAttack, RightLightAttack, TopLightAttack, LeftHeavyAttack, RightHeavyAttack, TopHeavyAttack, ZoneAttack, GuardBreak, Block};

	enum AttackType {Light, Heavy, Zone};

	enum AttackDirection { Left, Top, Right };

	AttackType ComboChain[3];
	FString ComboChainString;

	FString LLLString[3] = { "Light", "Light", "Light" };

	FString LLL = "LLL";

	AttackList ActiveAttack;

	AttackList NextAttack;
	bool IsCombo = false;
	int combocounter = 0;
	bool ChangingDirection = false;

	bool DealingDamage = false;

	float BlockTimer = 0;

	bool FirePressed = false;
	bool SecondaryFirePressed = false;
	float ZoneTimer = 0;
	bool checkzone = false;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	int ActiveWeapon = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FString ActiveMode = "Ranged";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FString ActiveWeaponName = "Active Weapon = Main Gun";
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	float InputDeadZone = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	float LightAttackCoolDown = 0.45f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	float HeavyAttackCoolDown = 0.60f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	float AttackCancelWindow = 0.450f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	float BlockCoolDown = 0.450f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	float MorphWindow = 0.50f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee")
	float StunTime = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "True"))
	AGun* PrimaryWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "True"))
	ARocketLauncher* SecondaryWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "True"))
	ASmokeLauncher* SmokeLauncher;


	bool Blocking = false;

	UFUNCTION(BlueprintCallable)
	void Fire();

	void SecondaryFire();
	void FireReleased();
	void SecondaryFireReleased();
	void LaunchSmoke();

	UFUNCTION(BlueprintCallable)
	void Reload();

	void SetActiveWeaponPrimary();
	void SetActiveWeaponSecondary();
	void SwitchMode();

	//Melee Functions//

	void ActivateLightAttack(int ComboCounter);
	void ActivateHeavyAttack();
	void ActivateZoneAttack();
	void CancelAttack();
	void ActivateGuardBreak();
	void ActivateBlockParry();
	bool CheckParry();
	void GetStunned();
	void TestCombo();


	bool Stunned = false;
	float StunTimer = 0.f;

	ALothair();

	AttackDirection ActiveAttackDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		bool Attacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		bool AttackAnim = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		FString ActiveAttackName = "NULL";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		FString ActiveAttackDirectionString = "NULL";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		UAnimMontage* AMShiftToLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		UAnimMontage* AMShiftToRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		UAnimMontage* AMLeftLightAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		UAnimMontage* AMRightLightAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		UAnimMontage* AMLeftHeavyAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		UAnimMontage* AMRightHeavyAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		UAnimMontage* AMTopLightAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		UAnimMontage* AMTopHeavyAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		UAnimMontage* AMZoneAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		UAnimMontage* AMHHHRightAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		UAnimMontage* AMHHHLeftAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		UAnimMontage* AMHHHTopAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		UAnimMontage* AMFireGun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		UAnimMontage* AMGuardBreak;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		UAnimMontage* AMBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		UAnimMontage* AMParry;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		UAnimMontage* AMHitReactionLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		UAnimMontage* AMHitReactionRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		UAnimMontage* AMHitReaction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		UAnimMontage* AMLightLightCombo;

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> MainGunClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARocketLauncher> RocketLauncherClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASmokeLauncher> SmokeLauncherClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ALothairAxe> AxeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "True"))
	ALothairAxe* Axe;

};
