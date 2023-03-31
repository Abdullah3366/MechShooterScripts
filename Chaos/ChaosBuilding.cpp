// Fill out your copyright notice in the Description page of Project Settings.

#include "IronValiant/Chaos/ChaosBuilding.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Components/StaticMeshComponent.h"

AChaosBuilding::AChaosBuilding()
{
	PrimaryActorTick.bCanEverTick = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	FractureComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Fracture Mesh"));
	CollisionVolume = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Collision"));

	SetRootComponent(Root);
	FractureComponent->SetupAttachment(GetRootComponent());
	CollisionVolume->SetupAttachment(GetRootComponent());
	
	FractureComponent->bCastDynamicShadow = false;

	CollisionVolume->bHiddenInGame = true;
	FractureComponent->SetCollisionProfileName("Destructible");
	CollisionVolume->SetCollisionProfileName("IgnoreOnlyPawn");
	CollisionVolume->SetMobility(EComponentMobility::Stationary);
}

void AChaosBuilding::BeginPlay()
{
	Super::BeginPlay();
	Bounds = FractureComponent->Bounds.BoxExtent;
	Origins = FractureComponent->Bounds.Origin;
	CollisionVolume->OnComponentHit.AddDynamic(this, &AChaosBuilding::OnComponentHit);
}

void AChaosBuilding::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	CollisionVolume->DestroyComponent();
}

void AChaosBuilding::UpdateHealth(float Damage)
{
	if(Health <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1,3,FColor::Red,"Collapse");
		SetLifeSpan(5);
	}
	Health -= FMath::Min(Health, Damage);
}