// Fill out your copyright notice in the Description page of Project Settings.


#include "LothairAxe.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "IronValiant/MechActors/MechBase.h"

// Sets default values
ALothairAxe::ALothairAxe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	//RootComponent = Root;

	AxeMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Axe Mesh"));
	RootComponent = AxeMesh;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	Capsule->SetupAttachment(AxeMesh);
}

// Called when the game starts or when spawned
void ALothairAxe::BeginPlay()
{
	Super::BeginPlay();
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ALothairAxe::OnOverlapBegin);
	
}

// Called every frame
void ALothairAxe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALothairAxe::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
	/*Enemy = Cast<AMechBase>(OtherActor);
	if (Enemy->Blocking)
	{
		UE_LOG(LogTemp, Warning, TEXT("Axe collided but block was on"));
	}*/
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *SweepResult.BoneName.ToString());
}



