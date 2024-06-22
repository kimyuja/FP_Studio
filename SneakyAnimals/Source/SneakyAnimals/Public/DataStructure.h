// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "DataStructure.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKYANIMALS_API UDataStructure : public UObject
{
	GENERATED_BODY()
	
};

USTRUCT(Atomic, BlueprintType)
struct FStructure_Available_Avatars : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Avatar;
};

USTRUCT(Atomic, BlueprintType)
struct FStructure_UserProfile : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Username;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* User_Avatar;
};