// Fill out your copyright notice in the Description page of Project Settings.


#include "Lothair.h"
#include "IronValiant\Weapons\LothairAxe.h"
#include "IronValiant\Weapons\Gun.h"
#include "IronValiant\Weapons\RocketLauncher.h"
#include "IronValiant\Weapons\SmokeLauncher.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

ALothair::ALothair()
{
	
}

void ALothair::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ALothair::Fire);
	PlayerInputComponent->BindAction(TEXT("SecondaryFire"), IE_Pressed, this, &ALothair::SecondaryFire);
	PlayerInputComponent->BindAction(TEXT("LaunchSmoke"), IE_Pressed, this, &ALothair::LaunchSmoke);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &ALothair::FireReleased);
	PlayerInputComponent->BindAction(TEXT("SecondaryFire"), IE_Released, this, &ALothair::SecondaryFireReleased);
	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &ALothair::Reload);
	PlayerInputComponent->BindAction(TEXT("PrimaryWeapon"), IE_Pressed, this, &ALothair::SetActiveWeaponPrimary);
	PlayerInputComponent->BindAction(TEXT("SecondaryWeapon"), IE_Pressed, this, &ALothair::SetActiveWeaponSecondary);
	PlayerInputComponent->BindAction(TEXT("SwitchMode"), IE_Pressed, this, &ALothair::SwitchMode);
	PlayerInputComponent->BindAction(TEXT("CancelAttack"), IE_Pressed, this, &ALothair::CancelAttack);
	PlayerInputComponent->BindAction(TEXT("GuardBreak"), IE_Pressed, this, &ALothair::ActivateGuardBreak);
	PlayerInputComponent->BindAction(TEXT("Block/Parry"), IE_Pressed, this, &ALothair::ActivateBlockParry);
	PlayerInputComponent->BindAction(TEXT("TestCombo"), IE_Pressed, this, &ALothair::TestCombo);
}

void ALothair::BeginPlay()
{
	Super::BeginPlay();
	Super::BoostSpeed = PanzerBoostSpeed;
	Super::BoostAcceleration = PanzerBoostAcceleration;

	
	if (AxeClass)
	{
		Axe = GetWorld()->SpawnActor<ALothairAxe>(AxeClass);
		Axe->AttachToComponent(CharacterMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Axe_Socket_R"));
		Axe->SetOwner(this);
	}

	if (MainGunClass)
	{
		PrimaryWeapon = GetWorld()->SpawnActor<AGun>(MainGunClass);
		PrimaryWeapon->AttachToComponent(CharacterMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Gun_Socket"));
		PrimaryWeapon->SetOwner(this);
	}

	if (RocketLauncherClass)
	{
		SecondaryWeapon = GetWorld()->SpawnActor<ARocketLauncher>(RocketLauncherClass);
		SecondaryWeapon->AttachToComponent(CharacterMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("RocketLauncher_Socket"));
		SecondaryWeapon->SetOwner(this);
	}

	if (SmokeLauncherClass)
	{
		SmokeLauncher = GetWorld()->SpawnActor<ASmokeLauncher>(SmokeLauncherClass);
		SmokeLauncher->AttachToComponent(CharacterMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("RocketLauncher_Socket"));
		SmokeLauncher->SetOwner(this);
	}

	ActiveAttackDirectionString = "Left";
}

// Called every frame
void ALothair::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//This part is a jugaad and needs to be fixed
	AttackAnim = false;
	if (Attacking)
	{
		Axe->Attacking = true;
		CanMove = false;
	}
	if (!Attacking)
	{
		Axe->Attacking = false;
		CanMove = true;
	}

	if (Blocking)
	{
		//Attacking = false;
		BlockTimer = BlockTimer + DeltaTime;
		if (BlockTimer > 0.450)
		{
			Blocking = false;
			BlockTimer = 0;
			Super::StopBlock();
			UE_LOG(LogTemp, Log, TEXT("Block Stop"));
		}
	}

	if (Stunned)
	{
		//UE_LOG(LogTemp, Log, TEXT("Stunned"));
		StunTimer = StunTimer + DeltaTime;
		if (StunTimer >= StunTime)
		{
			Stunned = false;
		}
	}

	// Attack Timer Logic

	if (Attacking)
	{
		AttackTimer = AttackTimer + DeltaTime;

		//UE_LOG(LogTemp, Warning, TEXT("%f"), AttackTimer)

		if (Cancelling)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Cancelling"));
			if (AttackTimer >= 0.450)
			{
				Attacking = false;
				AttackTimer = 0.f;
				ComboChainString.Reset();
				//UE_LOG(LogTemp, Warning, TEXT("Minimum Cancel time finished"));
			}
		}

		if (Morphing)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Morphing"));
			if (AttackTimer >= 0.400)
			{
				//Attacking = false;
				AttackTimer = 0.f;
				Morphing = false;
				ComboChainString.Reset();
				//UE_LOG(LogTemp, Warning, TEXT("Minimum Morphing time finished"));
			}
		}

		if (Blocking)
		{
			UE_LOG(LogTemp, Warning, TEXT("in block"));
			AttackTimer = AttackTimer + DeltaTime;
			if (AttackTimer >= 0.450)
			{
				AttackTimer = 0.f;
				Blocking = false;
				ComboChainString.Reset();
				UE_LOG(LogTemp, Warning, TEXT("Blocking time finished"));
			}
		}
	}

	//Updating last look input

	if (fabs(VerticalAxisValue) > fabs(HorizontalAxisValue))
	{
		ActiveAttackDirection = Top;
	}
	else
	{
		if (HorizontalAxisValue > 0.7f)
		{
			if (ActiveAttackDirection != Right)
			{
				UE_LOG(LogTemp, Warning, TEXT("Change direction to Right"));
				ActiveAttackDirection = Left;
				ActiveAttackDirectionString = "Left";
			}
		}
		if (HorizontalAxisValue < -0.7f)
		{
			if (ActiveAttackDirection != Left)
			{
				UE_LOG(LogTemp, Warning, TEXT("Change Direction TO Left"));
				ActiveAttackDirection = Left;
				ActiveAttackDirectionString = "Left";
				Axe->AttachToComponent(CharacterMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Axe_Socket"));
			}
		}
	}

	if (checkzone == true)
	{
		ZoneTimer = ZoneTimer + DeltaTime;;
	}
}

void ALothair::Fire()
{
	if (!Stunned)
	{
		FirePressed = true;

		if (SecondaryFirePressed == true && ZoneTimer <= 100)
		{
			checkzone = false;
			ZoneTimer = 0;
			ActivateZoneAttack();
			return;
		}

		if (ActiveMode == "Ranged")
		{
			UE_LOG(LogTemp, Warning, TEXT("Fire Primary Weapon"));

			PrimaryWeapon->Fire();

			if (PrimaryWeapon->ShotFired == true)
			{
				PlayAnimMontage(AMFireGun, 1.f, NAME_None);
			}
		}

		if (ActiveMode == "Combat")
		{
			AttackOn = true;
			if (Attacking == false)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Light Melee Attack"));
				combocounter = 1;
				ActivateLightAttack(combocounter);
				ComboChain[0] = Light;
				ComboChainString = "Light";
				ComboChainString.AppendChar('L');
			}

			if (Attacking == true)
			{
				//For Morhping
				if (AttackTimer < 0.400)
				{
					if (ActiveAttack == LeftHeavyAttack && (ActiveAttackDirection == Right || ActiveAttackDirection == Top))
					{
						//UE_LOG(LogTemp, Warning, TEXT("Heavy Attack was morphed into light attack"));
						Morphing = true;
						ActivateLightAttack(combocounter);
						ComboChain[0] = Light;
						ComboChainString[0] = "Light";
					}
					if (ActiveAttack == RightHeavyAttack && (ActiveAttackDirection == Left || ActiveAttackDirection == Top))
					{
						//UE_LOG(LogTemp, Warning, TEXT("Heavy Attack was morphed into light attack"));
						Morphing = true;
						ActivateLightAttack(combocounter);
						ComboChain[0] = Light;
						ComboChainString[0] = "Light";
					}
					if (ActiveAttack == TopHeavyAttack && (ActiveAttackDirection == Right || ActiveAttackDirection == Left))
					{
						//UE_LOG(LogTemp, Warning, TEXT("Heavy Attack was morphed into light attack"));
						Morphing = true;
						ActivateLightAttack(combocounter);
						ComboChain[0] = Light;
					}
				}
			}
		}
	}
}

void ALothair::FireReleased()
{
	FirePressed = false;
}

void ALothair::SecondaryFireReleased()
{
	SecondaryFirePressed = false;
}

void ALothair::SecondaryFire()
{
	///Ranged///

	if (!Stunned)
	{
		SecondaryFirePressed = true;

		//Zone Attack Check
		if (FirePressed == true && ZoneTimer <= 100)
		{
			checkzone = false;
			ZoneTimer = 0;
			ActivateZoneAttack();
			//UE_LOG(LogTemp, Warning, TEXT("Zone Attack"));
			return;
		}

		//Rocket Fire
		if (ActiveMode == "Ranged")
		{
			UE_LOG(LogTemp, Warning, TEXT("Fire Secondary Weapon"));
			//SecondaryWeapon->Fire();
			SecondaryWeapon->StartBurst();
		}



		///Melee///
		if (ActiveMode == "Combat")
		{
			if (Attacking == false)
			{
				combocounter = 1;
				ComboChainString.AppendChar('H');
				ActivateHeavyAttack();
			}

			//Combo
			if (Attacking == true)
			{
				if (AttackTimer >= 0.650)
				{
					//	Attacking = false;
					combocounter = combocounter + 1;
					ComboChainString.AppendChar('H');
					UE_LOG(LogTemp, Warning, TEXT("%s"), *ComboChainString);
					ActivateHeavyAttack();

					// Check if combo exists
					for (int i = 0; i < combocounter; i++)
					{
						if (LLLString[i] == ComboChainString[i])
						{
							UE_LOG(LogTemp, Warning, TEXT("A potential Combo exists"));
							//combocounter = combocounter + 1;
							ActivateLightAttack(combocounter);
						}
						else
						{
							combocounter = 0;
						}
					}

				if (AttackTimer >= 0.950)
				{
					if ((ActiveAttack == LeftHeavyAttack || ActiveAttack == RightHeavyAttack))
					{
						//UE_LOG(LogTemp, Warning, TEXT("Heavy Attack combo added"));
						Attacking = false;
						combocounter = combocounter + 1;
						ComboChainString[combocounter] = "Heavy";
						ActivateHeavyAttack();
						UE_LOG(LogTemp, Warning, TEXT("%i"), combocounter);
						return;
					}
				}

					
					if (AttackTimer >= 0.650 && (ActiveAttack == LeftLightAttack || ActiveAttack == RightLightAttack || ActiveAttack == TopLightAttack))
					{
						Attacking = false;
						combocounter = combocounter + 1;
						ActivateHeavyAttack();
						UE_LOG(LogTemp, Warning, TEXT("L L H combo"));
						return;
					}
				}
			}
		}
	}
}

void ALothair::Reload()
{
	if (ActiveWeapon == 1)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Reload Primary"));
		PrimaryWeapon->Reload();
	}
	if (ActiveWeapon == 2)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Reload Secondary"));
		SecondaryWeapon->Reload();
	}
}

void ALothair::SetActiveWeaponPrimary()
{
	ActiveWeapon = 1;
	ActiveWeaponName = "Active Weapon = Main Gun";
}

void ALothair::SetActiveWeaponSecondary()
{
	ActiveWeapon = 2;
	ActiveWeaponName = "Active Weapon = Shoulder Rocket";
}

void ALothair::SwitchMode()
{
	//UE_LOG(LogTemp, Warning, TEXT("Switching Combat Mode"));
	if (ActiveMode == "Ranged")
	{
		ActiveMode = "Combat";
		PrimaryWeapon->AttachToComponent(CharacterMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("GunHolster_Socket"));
		Axe->AttachToComponent(CharacterMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Axe_Socket_L"));

	}
	else
	{
		ActiveMode = "Ranged";
		PrimaryWeapon->AttachToComponent(CharacterMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Gun_Socket"));
		Axe->AttachToComponent(CharacterMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Axe_Socket_R"));
	}
}

void ALothair::LaunchSmoke()
{
	SmokeLauncher->Fire();
}

///////Melee Functions////////

void ALothair::ActivateLightAttack(int ComboCounter)
{
	UE_LOG(LogTemp, Warning, TEXT("Activate Light Attack callled"));

	Attacking = true;
	AttackTimer = 0.f;
	if (ActiveAttackDirection == Left)
	{
		ActiveAttack = LeftLightAttack;
		ActiveAttackName = "LeftLightAttack";
		//ComboChain[combonumber] = LeftLightAttack;
		PlayAnimMontage(AMLeftLightAttack, 1.f, NAME_None);
	}
	if (ActiveAttackDirection == Right)
	{
		ActiveAttack = RightLightAttack;
		ActiveAttackName = "RightLightAttack";
	//	ComboChain[combonumber] = RightLightAttack;
		PlayAnimMontage(AMLeftLightAttack, 1.f, NAME_None);
	}
	if (ActiveAttackDirection == Top)
	{
		ActiveAttack = TopLightAttack;
		ActiveAttackName = "TopLightAttack";
		//ComboChain[combonumber] = TopLightAttack;
		PlayAnimMontage(AMTopLightAttack, 1.f, NAME_None);
	}


	if (combocounter == 3)
	{
		combocounter = 0;
		UE_LOG(LogTemp, Warning, TEXT("%s"));
		UE_LOG(LogTemp, Warning, TEXT("Light Combo Finsher"));
	}
}

void ALothair::ActivateHeavyAttack()
{
	Attacking = true;
	AttackTimer = 0.f;

	if (combocounter == 3)
	{
		if (ActiveAttackDirection == Left)
		{
			PlayAnimMontage(AMHHHLeftAttack, 1.f, NAME_None);
		}
		if (ActiveAttackDirection == Right)
		{
			PlayAnimMontage(AMHHHRightAttack, 1.f, NAME_None);
		}
		if (ActiveAttackDirection == Top)
		{
			PlayAnimMontage(AMHHHTopAttack, 1.f, NAME_None);
		}
		combocounter = 0;
		return;
	}

	if (ActiveAttackDirection == Left)
	{
		ActiveAttack = LeftHeavyAttack;
		ActiveAttackName = "LeftHeavyAttack";
		PlayAnimMontage(AMLeftHeavyAttack, 1.f, NAME_None);
	}
	if (ActiveAttackDirection == Right)
	{
		ActiveAttack = RightHeavyAttack;
		ActiveAttackName = "RightHeavyAttack";
		PlayAnimMontage(AMRightHeavyAttack, 1.f, NAME_None);
	}
	if (ActiveAttackDirection == Top)
	{
		ActiveAttack = TopHeavyAttack;
		ActiveAttackName = "TopHeavyAttack";
		PlayAnimMontage(AMTopHeavyAttack, 1.f, NAME_None);
	}
}

void ALothair::ActivateZoneAttack()
{
	ActiveAttack = ZoneAttack;
	PlayAnimMontage(AMZoneAttack, 1.f, NAME_None);
	checkzone = false;
	Attacking = true;
	//CanMove = false;
	FirePressed = false;
	SecondaryFirePressed = false;
}

void ALothair::CancelAttack()
{
	if (Attacking && AttackTimer <= 0.450)
	{
		if (ActiveAttack == LeftHeavyAttack || ActiveAttack == RightHeavyAttack || ActiveAttack == TopHeavyAttack)
		{
			Cancelling = true;
			// Time should still go to 450ms need to fix this


			UE_LOG(LogTemp, Warning, TEXT("Heavy Attack Canceled"));
		}
	}
}

void ALothair::ActivateGuardBreak()
{
	ActiveAttack = GuardBreak;
	PlayAnimMontage(AMZoneAttack, 1.f, NAME_None);   //Change this to guardbreak animation
}


void ALothair::ActivateBlockParry()
{
	AttackTimer = 0;
	UE_LOG(LogTemp, Warning, TEXT("Block Started"));
	ActiveAttack = Block;
	Blocking = true;
	Attacking = true;
	if (CheckParry())
	{
		PlayAnimMontage(AMParry, 1.f, NAME_None);
	}
	else
	{
		PlayAnimMontage(AMBlock, 1.f, NAME_None);
	}
}

bool ALothair::CheckParry()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	// Ignore any specific actors
	TArray<AActor*> ignoreActors;
	// Ignore self or remove this line to not ignore any
	ignoreActors.Init(this, 1);

	TArray<AActor*> outActors;
	// Total radius of the sphere
	float radius = 250.0f;
	// Sphere's spawn loccation within the world
	FVector sphereSpawnLocation = GetActorLocation();
	// Class that the sphere should hit against and include in the outActors array (Can be null)
	UClass* seekClass = ALothair::StaticClass(); // NULL;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), sphereSpawnLocation, radius, traceObjectTypes, seekClass, ignoreActors, outActors);

	// Optional: Use to have a visual representation of the SphereOverlapActors
	 DrawDebugSphere(GetWorld(), GetActorLocation(), radius, 12, FColor::Red, true, 10.0f);


	// Finally iterate over the outActor array
	for (AActor* overlappedActor : outActors) {
		//UE_LOG(LogTemp, Log, TEXT("OverlappedActor: %s"), *overlappedActor->GetName());
		ALothair* EnemyLothair = Cast<ALothair>(overlappedActor);

		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), EnemyLothair->GetActorLocation());
		UE_LOG(LogTemp, Log, TEXT("The Yaw is : %i"), (int)Rotation.Yaw);

		if (EnemyLothair->Attacking)
		{
			if (EnemyLothair->ActiveAttackName == "LeftLightAttack" || EnemyLothair->ActiveAttackName == "RightLightAttack" || EnemyLothair->ActiveAttackName == "TopLightAttack")
			{
				UE_LOG(LogTemp, Log, TEXT("It is a light attack"));
				if (EnemyLothair->AttackTimer >= 0.400 && EnemyLothair->AttackTimer < 0.550)
				{
					UE_LOG(LogTemp, Log, TEXT("You parried in the 150ms before the attack landed"));
					EnemyLothair->GetStunned();
				}
				else if(AttackTimer < 0.450)
				{
					UE_LOG(LogTemp, Log, TEXT("The attack was blocked but not parried"));
				}
			}
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("OverlappedActor: %s is not attacking so parry unsuccessful only blocking"), *overlappedActor->GetName());
			return false;
		}
	}

	return false;
}

void ALothair::GetStunned()
{
	StopAnimMontage();
	PlayAnimMontage(AMHitReactionRight, 1.f, NAME_None);
	Stunned = true;
	StunTimer = 0.f;
}


void ALothair::TestCombo()

	PlayAnimMontage(AMLightLightCombo, 1.f, NAME_None);
}