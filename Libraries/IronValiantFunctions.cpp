// Fill out your copyright notice in the Description page of Project Settings.


#include "IronValiant/Libraries/IronValiantFunctions.h"

FVector UIronValiantFunctions::GetPlayerLocation(UObject* Object)
{
	return Object->GetWorld()? Object->GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() : FVector();
}
