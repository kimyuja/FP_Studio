// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataStructure.h"
#include "LineStructure.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API ULineStructure : public UDataStructure
{
	GENERATED_BODY()

	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D start;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D end;

	
};
