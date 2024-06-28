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

USTRUCT(Atomic, BlueprintType)
struct FUserProfileResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FStructure_UserProfile S_UserProfile;

    UPROPERTY(BlueprintReadWrite)
    bool success;
};

USTRUCT(Atomic, BlueprintType)
struct FStructure_SessionInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString ServerName;

    UPROPERTY(BlueprintReadWrite)
    int32 MaxPlayers;
};

USTRUCT(Atomic, BlueprintType)
struct FSessionResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Session")
    FString ServerName;

    UPROPERTY(BlueprintReadOnly, Category = "Session")
    int32 CurrentPlayers;

    UPROPERTY(BlueprintReadOnly, Category = "Session")
    int32 MaxPlayers;

    UPROPERTY(BlueprintReadOnly, Category = "Session")
    int32 PingInMs;
};

USTRUCT(Atomic, BlueprintType)
struct FMyBlueprintSessionResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString ServerName;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentPlayers;

	UPROPERTY(BlueprintReadOnly)
	int32 MaxPlayers;

	UPROPERTY(BlueprintReadOnly)
	int32 PinginMs;
};