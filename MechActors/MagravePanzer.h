// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MechBase.h"
#include "MagravePanzer.generated.h"

class AGun;
class AShoulderMountedRocket;
class ALothairAxe;

UCLASS()
class IRONVALIANT_API AMagravePanzer : public AMechBase
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere)
	float PanzerBoostSpeed;
	UPROPERTY(EditAnywhere)
	float PanzerBoostAcceleration;


	///Melee Variables///

	float AttackTimer = 0.f;
	//bool Attacking = false;
	bool Morphing = false;
	bool Cancelling = false;
	//FString ActiveAttack;
	//FTimerHandle TH_AttackTimer;

	enum AttackList {LeftLightAttack, RightLightAttack, TopLightAttack, LeftHeavyAttack, RightHeavyAttack, TopHeavyAttack, ZoneAttack};

	enum AttackDirection {Left, Top, Right};
	
	AttackList ActiveAttack;

	AttackList NextAttack;
	AttackDirection ActiveAttackDirection;

public:

	AMagravePanzer();

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	//bool Attacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool Attacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	bool AttackAnim = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FString ActiveAttackName = "NULL";


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



private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> MainGunClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> ShoulderRocketClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ALothairAxe> AxeClass;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "True"))
	//USkeletalMeshComponent* AxeMesh;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "True"))
	ALothairAxe* Axe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character" , meta = (AllowPrivateAccess = "True"))
	AGun* PrimaryWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character" , meta = (AllowPrivateAccess = "True"))
	AGun* SecondaryWeapon;

	void Fire();
	void SecondaryFire();
	//void MeleeAttack();
	void Reload();
	void SetActiveWeaponPrimary();
	void SetActiveWeaponSecondary();
	void SwitchMode();
	void CancelAttack();

	//Melee Functions//

	void ActivateLightAttack();
	void ActivateHeavyAttack();
	void ActivateZoneAttack();
};
