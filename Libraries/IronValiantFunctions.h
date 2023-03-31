// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "IronValiantFunctions.generated.h"

/**
 * 
 */
UCLASS()
class IRONVALIANT_API UIronValiantFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure,meta=(DefaultToSelf = "Object", HidePin = "Object"))
	static FVector GetPlayerLocation(UObject* Object);
};
