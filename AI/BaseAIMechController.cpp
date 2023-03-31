#include "IronValiant/AI/BaseAIMechController.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "Kismet/KismetMathLibrary.h"

void ABaseAIMechController::BeginPlay()
{
	Super::BeginPlay();
	Player = GetWorld()->GetFirstPlayerController()->GetPawn();
	GetWorldTimerManager().SetTimer(PerceptionTimer,this,&ABaseAIMechController::Perceiving,0.1,true);
}

void ABaseAIMechController::Perceiving()
{
	FRotator LookAtRotation = UKismetMathLibrary::FindRelativeLookAtRotation(GetPawn()->GetActorTransform(),Player->GetActorLocation());
	FVector2D YawPitch = FVector2D(LookAtRotation.Yaw,LookAtRotation.Pitch);
	float Distance = GetPawn()->GetDistanceTo(Player);

	if (Distance < SightDistance && LineOfSightTo(Player) && (YawPitch < FVector2D(VisionAngle) && YawPitch >(FVector2D(VisionAngle * -1))))
	{
		bLostSight = true;
		SetFocus(Player);
		GetBlackboardComponent()->SetValueAsBool("CanSeePlayer",true);
		return;
	}
	if(bLostSight)
	{
		bLostSight = false;
		GetWorldTimerManager().SetTimer(LoseSightTimer,this,&ABaseAIMechController::LoseSight, LoseSightTime,false,LoseSightTime);
		return;
	}
}

void ABaseAIMechController::LoseSight()
{
	ClearFocus(EAIFocusPriority::Gameplay);
	GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", false);
	GetBlackboardComponent()->SetValueAsVector("LastPlayerLocation",Player->GetActorLocation());
	GetWorldTimerManager().ClearTimer(LoseSightTimer);
}
