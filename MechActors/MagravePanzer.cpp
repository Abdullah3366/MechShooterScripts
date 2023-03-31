// Fill out your copyright notice in the Description page of Project Settings.


#include "MagravePanzer.h"
#include "IronValiant\Weapons\LothairAxe.h"
#include "IronValiant\Weapons\Gun.h"
#include "Camera/CameraComponent.h"

AMagravePanzer::AMagravePanzer()
{
	//AxeMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Axe Mesh"));
	//AxeMesh->SetupAttachment(CharacterMesh, "Axe_Socket");
	//AxeMesh->AttachToComponent(CharacterMesh, FAttachmentTransformRules::KeepRelativeTransform, "Axe_Socket");
}


void AMagravePanzer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AMagravePanzer::Fire);
	PlayerInputComponent->BindAction(TEXT("SecondaryFire"), IE_Pressed, this, &AMagravePanzer::SecondaryFire);
	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &AMagravePanzer::Reload);
	PlayerInputComponent->BindAction(TEXT("PrimaryWeapon"), IE_Pressed, this, &AMagravePanzer::SetActiveWeaponPrimary);
	PlayerInputComponent->BindAction(TEXT("SecondaryWeapon"), IE_Pressed, this, &AMagravePanzer::SetActiveWeaponSecondary);
	PlayerInputComponent->BindAction(TEXT("SwitchMode"), IE_Pressed, this, &AMagravePanzer::SwitchMode);
	PlayerInputComponent->BindAction(TEXT("CancelAttack"), IE_Pressed, this, &AMagravePanzer::CancelAttack);
}


void AMagravePanzer::BeginPlay()
{
	Super::BeginPlay();
	Super::BoostSpeed = PanzerBoostSpeed;
	Super::BoostAcceleration = PanzerBoostAcceleration;

	if (AxeClass)
	{
		Axe = GetWorld()->SpawnActor<ALothairAxe>(AxeClass);
		Axe->AttachToComponent(CharacterMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Axe_Socket"));
		Axe->SetOwner(this);
	}
	//Axe = GetWorld()->SpawnActor<ALothairAxe>(AxeClass);
	//PrimaryWeapon->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("GripPoint"));

	if (MainGunClass)
	{
		PrimaryWeapon = GetWorld()->SpawnActor<AGun>(MainGunClass);
		PrimaryWeapon->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("GripPoint"));
		PrimaryWeapon->SetActorRelativeLocation(FVector(43.f, 33.f, 43.f));
		PrimaryWeapon->SetOwner(this);
	}

	if (ShoulderRocketClass)
	{
		SecondaryWeapon = GetWorld()->SpawnActor<AGun>(ShoulderRocketClass);
		SecondaryWeapon->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("SecondaryGripPoint"));
		//SecondaryWeapon->SetActorRelativeLocation(FVector(63.f, 63.f, 63.f));
		//SecondaryWeapon->AddActorLocalRotation(0.f, 90.f, 0.f);
		SecondaryWeapon->SetOwner(this);
	}

}

// Called every frame
void AMagravePanzer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//This part is a jugaad and needs to be fixed
	AttackAnim = false;
	if (Attacking)
	{
		Axe->Attacking = true;
	}
	if (!Attacking)
	{
		Axe->Attacking = false;
	}

	// Attack Timer Logic

	if (Attacking)
	{
		//UE_LOG(LogTemp, Warning, TEXT("In the attack phase"));

		AttackTimer = AttackTimer + DeltaTime;

		UE_LOG(LogTemp, Warning, TEXT("%f"), AttackTimer);

		if (ActiveAttack == LeftLightAttack || ActiveAttack == RightLightAttack || ActiveAttack == TopLightAttack)
		{
			if (AttackTimer >= 0.250 && AttackTimer<=0.650)
			{
				UE_LOG(LogTemp, Warning, TEXT("In left light attack finished"));
				Attacking = false;
				AttackTimer = 0.f;
				UE_LOG(LogTemp, Warning, TEXT("Light Attack Finished"));
			}
		}

		if (ActiveAttack == LeftHeavyAttack || ActiveAttack == RightHeavyAttack)
		{
			if (AttackTimer >= 0.550 && AttackTimer <= 0.950)
			{
				Attacking = false;
				AttackTimer = 0.f;
				UE_LOG(LogTemp, Warning, TEXT("Left/Right Heavy Attack Finished"));
			}
		}

		if (ActiveAttack == TopHeavyAttack)
		{
			if (AttackTimer >= 0.600 && AttackTimer <= 0.400)
			{
				Attacking = false;
				AttackTimer = 0.f;
				UE_LOG(LogTemp, Warning, TEXT("Top Heavy Attack Finished"));
			}
		}

		if (Cancelling)
		{
			UE_LOG(LogTemp, Warning, TEXT("Cancelling"));
			if (AttackTimer >= 0.450)
			{
				Attacking = false;
				AttackTimer = 0.f;
				UE_LOG(LogTemp, Warning, TEXT("Minimum Cancel time finished"));
			}
		}

		if (Morphing)
		{
			UE_LOG(LogTemp, Warning, TEXT("Morphing"));
			if (AttackTimer >= 0.400)
			{
				//Attacking = false;
				AttackTimer = 0.f;
				Morphing = false;
				UE_LOG(LogTemp, Warning, TEXT("Minimum Morphing time finished"));
			}
		}
	}

		//Updating last look input

		if (fabs(VerticalAxisValue) > fabs(HorizontalAxisValue))
		{
			ActiveAttackDirection = Top;
			//UE_LOG(LogTemp, Warning, TEXT("Current Direction Top"));
			//Axe->AttachToComponent(CharacterMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Axe_Socket"));
		}
		else
		{
			if (HorizontalAxisValue > 0.f)
			{
				ActiveAttackDirection = Right;
				//UE_LOG(LogTemp, Warning, TEXT("Current Direction Right"));
				//Axe->AttachToComponent(CharacterMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Left_AxeSocket"));
			}
			if (HorizontalAxisValue < 0.f)
			{
				ActiveAttackDirection = Left;
				//UE_LOG(LogTemp, Warning, TEXT("Current Direction Left"));
				//Axe->AttachToComponent(CharacterMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Axe_Socket"));
			}
		}
}

void AMagravePanzer::Fire()
{
	
	UE_LOG(LogTemp, Warning, TEXT("Fire called once"));
	//AttackAnim = false;
	if (ActiveMode == "Ranged")
	{
		UE_LOG(LogTemp, Warning, TEXT("Fire Primary Weapon"));        
		PrimaryWeapon->Fire();
	}

	if (ActiveMode == "Combat")
	{
		//AttackOn = true;
		
		if (Attacking == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("Light Melee Attack"));
			ActivateLightAttack();
		}
		
		if (Attacking == true && AttackTimer < 0.400)
		{
			if (ActiveAttack == LeftHeavyAttack && (ActiveAttackDirection == Right || ActiveAttackDirection == Top))
			{
				UE_LOG(LogTemp, Warning, TEXT("Heavy Attack was morphed into light attack"));
				Morphing = true;
				ActivateLightAttack();
			}
			if (ActiveAttack == RightHeavyAttack && (ActiveAttackDirection == Left || ActiveAttackDirection == Top))
			{
				UE_LOG(LogTemp, Warning, TEXT("Heavy Attack was morphed into light attack"));
				Morphing = true;
				ActivateLightAttack();
			}
			if (ActiveAttack == TopHeavyAttack && (ActiveAttackDirection == Right || ActiveAttackDirection == Left))
			{
				UE_LOG(LogTemp, Warning, TEXT("Heavy Attack was morphed into light attack"));
				Morphing = true;
				ActivateLightAttack();
			}
		}

		if (ActiveAttack == RightLightAttack || ActiveAttack == RightHeavyAttack)
		{
			//Axe->AttachToComponent(CharacterMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Left_AxeSocket"));
		}

		AttackAnim = true;

		//UE_LOG(LogTemp, Warning, TEXT("%s"), *ActiveAttackName);
	}
}

void AMagravePanzer::SecondaryFire()
{
	///Ranged///

	if (ActiveMode == "Ranged")
	{
		UE_LOG(LogTemp, Warning, TEXT("Fire Secondary Weapon"));
		SecondaryWeapon->Fire();
	}

	///Melee///

	if (ActiveMode == "Combat")
	{
		if (Attacking == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("Heavy Melee Attack"));
			//Axe->AttachToComponent(CharacterMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Left_AxeSocket"));
			ActivateHeavyAttack();
		}
		AttackAnim = true;
	}
}

void AMagravePanzer::Reload()
{
	if (ActiveWeapon == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reload Primary"));
		PrimaryWeapon->Reload();
	}
	if (ActiveWeapon == 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reload Secondary"));
		SecondaryWeapon->Reload();
	}
}

void AMagravePanzer::SetActiveWeaponPrimary()
{
	UE_LOG(LogTemp, Warning, TEXT("Active Weapon Set to Primary"));
	ActiveWeapon = 1;
	ActiveWeaponName = "Active Weapon = Main Gun";
}

void AMagravePanzer::SetActiveWeaponSecondary()
{
	UE_LOG(LogTemp, Warning, TEXT("Active Weapon Set to Secondary"));
	ActiveWeapon = 2;
	ActiveWeaponName = "Active Weapon = Shoulder Rocket";
}

void AMagravePanzer::SwitchMode()
{
	UE_LOG(LogTemp, Warning, TEXT("Switching Combat Mode"));
	if (ActiveMode == "Ranged")
	{
		ActiveMode = "Combat";
	}
	else
	{
		ActiveMode = "Ranged";
	}
}
///////Melee Functions////////

void AMagravePanzer::ActivateLightAttack()
{
	//UE_LOG(LogTemp, Warning, TEXT("Activate Light Attack callled"));
	
	Attacking = true;
	//AttackOn = true;
	AttackTimer = 0.f;

	if (ActiveAttackDirection == Left)
	{
		ActiveAttack = LeftLightAttack;
		ActiveAttackName = "LeftLightAttack";
	}
	if (ActiveAttackDirection == Right)
	{
		ActiveAttack = RightLightAttack;
		ActiveAttackName = "RightLightAttack";
	}
	if (ActiveAttackDirection == Top)
	{
		ActiveAttack = TopLightAttack;
		ActiveAttackName = "TopLightAttack";
	}
}

void AMagravePanzer::ActivateHeavyAttack()
{
	Attacking = true;
	AttackTimer = 0.f;

	if (ActiveAttackDirection == Left)
	{
		ActiveAttack = LeftHeavyAttack;
		ActiveAttackName = "LeftHeavyAttack";
	}
	if (ActiveAttackDirection == Right)
	{
		ActiveAttack = RightHeavyAttack;
		ActiveAttackName = "RightHeavyAttack";
	}
	if (ActiveAttackDirection == Top)
	{
		ActiveAttack = TopHeavyAttack;
		ActiveAttackName = "TopHeavyAttack";
	}
}

void AMagravePanzer::ActivateZoneAttack()
{
	//Attacking = true;
}

void AMagravePanzer::CancelAttack()
{
	if (Attacking && AttackTimer <= 0.450)
	{
		if (ActiveAttack == LeftHeavyAttack || ActiveAttack == RightHeavyAttack || ActiveAttack == TopHeavyAttack)
		{
			//Attacking = false;
			//AttackTimer = 0.f;
			Cancelling = true;
			// Time should still go to 450ms need to fix this


			UE_LOG(LogTemp, Warning, TEXT("Heavy Attack Canceled"));
		}
	}
}