// Fill out your copyright notice in the Description page of Project Settings.


#include "MechBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AMechBase::AMechBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Movement = GetCharacterMovement();

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
}

// Called when the game starts or when spawned
void AMechBase::BeginPlay()
{
	Super::BeginPlay();
	CharacterMesh = this->GetMesh();
	CharacterMesh->SetupAttachment(RootComponent);
	NormalSpeed = Movement->MaxWalkSpeed;
	NormalAcceleration = Movement->MaxAcceleration;
}

// Called every frame
void AMechBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ParryRange->overlap
	
	if (IsHeating)
	{
		SystemHeat = SystemHeat + DeltaTime * HeatRate;
	}

	if (IsCooling && SystemHeat > 0)
	{
		SystemHeat = SystemHeat - DeltaTime * HeatRate;
	}

	if (SystemHeat > MaxSystemHeat)
	{
		BoostStop();
		SystemHeat--;
	}

	if (Movement->Velocity.Size() >= 10) 
	{
		RunningTime = RunningTime + DeltaTime;
	}
	else
	{
		RunningTime = 0.f;
	}
	if (IsDodge == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("Dodge is true"));
	}

	if (flying)
	{
		AddMovementInput(GetActorUpVector() * 1.f);
	}
}

// Called to bind functionality to input
void AMechBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMechBase::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMechBase::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMechBase::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AMechBase::LookRight);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AMechBase::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AMechBase::LookRightRate);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AMechBase::FlyStart);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AMechBase::FlyStop);
	PlayerInputComponent->BindAction(TEXT("Boost"), IE_Pressed, this, &AMechBase::BoostStart);
	PlayerInputComponent->BindAction(TEXT("Boost"), IE_Released, this, &AMechBase::BoostStop);
	PlayerInputComponent->BindAction(TEXT("BoostDodge"), IE_Pressed, this, &AMechBase::BoostDodge);
	PlayerInputComponent->BindAction(TEXT("ZoomIn"), IE_Pressed, this, &AMechBase::ZoomIn);
	PlayerInputComponent->BindAction(TEXT("ZoomOut"), IE_Pressed, this, &AMechBase::ZoomOut);
}

void AMechBase::MoveForward(float Axisvalue)
{
	//AddActorLocalOffset(GetActorForwardVector, true);
	if (CanMove)
	{
		AddMovementInput(GetActorForwardVector() * Axisvalue);
	}
}

void AMechBase::MoveRight(float AxisValue)
{
	if (CanMove)
	{
		AddMovementInput(GetActorRightVector() * AxisValue);
	}
}

void AMechBase::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
	VerticalAxisValue = AxisValue;
}

void AMechBase::LookRight(float AxisValue)
{
	AddControllerYawInput(AxisValue);
	HorizontalAxisValue = AxisValue;
}

void AMechBase::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->DeltaTimeSeconds);
}

void AMechBase::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->DeltaTimeSeconds);
}

void AMechBase::BoostStart()
{
	Movement->MaxWalkSpeed = BoostSpeed;
	Movement->MaxAcceleration = BoostAcceleration;
	IsDodge = true;
	IsHeating = true;
	IsCooling = false;
	GetWorldTimerManager().SetTimer(TH_BoostType, this, &AMechBase::SetBoostType, 1.f, false, 0.200);
}

void AMechBase::BoostStop()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop BOOSTTTTT!!"));
	Movement->MaxWalkSpeed = NormalSpeed;
	Movement->MaxAcceleration = NormalAcceleration;
	if (IsDodge == true && (SystemHeat + BoostHeat) < MaxSystemHeat)
	{
		SystemHeat = SystemHeat + BoostHeat;
		DoDodge = true;
		LaunchCharacter(GetLastMovementInputVector() * 2000.f, false, false);
		PlayAnimMontage(AMDodgeRight, 1.f, NAME_None);
	}
	else
	{
		DoDodge = false;
	}

	IsHeating = false;

	if (SystemHeat > 90)
	{
		GetWorldTimerManager().SetTimer(TH_CoolingDelay, this, &AMechBase::CoolingDelaySet, 1.f, false, 2);  
	}
	else
	{
		IsCooling = true;
		IsHeating = false;
	}
}

void AMechBase::FlyStart()
{
	UE_LOG(LogTemp, Warning, TEXT("Start Flying"));
	Movement->SetMovementMode(MOVE_Flying);
	Movement->GravityScale = 0.f;
	IsJump = true;
	flying = true;
	GetWorldTimerManager().SetTimer(TH_JumpType, this, &AMechBase::SetJumpType, 1.f, false, 0.200);
}

void AMechBase::FlyStop()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop Flying"));
	Movement->SetMovementMode(MOVE_Walking);
	Movement->GravityScale = 1.f;
	flying = false;
	if (IsJump)
	{
		Jump();
	}
}

void AMechBase::BoostDodge()
{

}

void AMechBase::ZoomIn()
{
	if (ZoomStatus == 0)
	{
		Camera->FieldOfView = Camera->FieldOfView - 50.f;
	}
	if (ZoomStatus == -1)
	{
		Camera->FieldOfView = Camera->FieldOfView - 10.f;
	}
	if (ZoomStatus < 1)
	{
		ZoomStatus++;
		SpringArm->TargetArmLength = SpringArm->TargetArmLength - 50.f;
	}
}

void AMechBase::ZoomOut()
{
	if (ZoomStatus == 1)
	{
		Camera->FieldOfView = Camera->FieldOfView + 50.f;
	}
	if (ZoomStatus == 0)
	{
		Camera->FieldOfView = Camera->FieldOfView + 10.f;
	}
	if (ZoomStatus > -1)
	{
		ZoomStatus--;
		SpringArm->TargetArmLength = SpringArm->TargetArmLength + 50.f;
	}
}

void AMechBase::SetBoostType()
{
	IsDodge = false;
	DoDodge = false;
	GetWorldTimerManager().ClearTimer(TH_BoostType);
}

void AMechBase::SetJumpType()
{
	IsJump = false;
	//DoDodge = false;
	GetWorldTimerManager().ClearTimer(TH_JumpType);
}

void AMechBase::CoolingDelaySet()
{
	IsCooling = true;
	GetWorldTimerManager().ClearTimer(TH_CoolingDelay);
}


void AMechBase::Block()
{
	Blocking = true;
}

void AMechBase::StopBlock()
{
	Blocking = false;

}