// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WitchsHouseStruct.generated.h"

/**
 * 
 */
class SNEAKYANIMALS_API WitchsHouseStruct
{
public:
	WitchsHouseStruct();
	~WitchsHouseStruct();
};

USTRUCT(Atomic, BlueprintType)
struct FWitchStruct
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* itemImg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText itemCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 itemCount; 
};